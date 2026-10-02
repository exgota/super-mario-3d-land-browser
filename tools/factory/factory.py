#!/usr/bin/env python3
"""Matching-decompilation factory for Super Mario 3D Land.

A work queue of small unmatched functions, stateless model workers in their own git
worktrees, and a deterministic integrator. Only tools/check.py decides what matches.

Commands (run with the repository's virtual environment python):
  init                 build the job queue from the target branch map
  run [--slots T:N,..] supervise workers, integrate matches, sync with main, write STATUS.md
  status               rewrite and print STATUS.md
  attempt              (inside a worker worktree) build and check the current job
  install-merge-driver register the row-wise map.csv merge driver for every worktree
  merge-map O A B      git merge driver for data/ver/eu/map.csv
Stop a running supervisor by creating the file STOP next to this script.
"""
import argparse
import bisect
import collections
import datetime
import json
import os
import pathlib
import queue
import re
import shutil
import signal
import sqlite3
import struct
import subprocess
import sys
import threading
import time

HOME = pathlib.Path(__file__).resolve().parent
REPOSITORY = pathlib.Path.home() / "super-mario-3d-land-browser"
INTEGRATION = HOME / "workspaces" / "integration"
MAIN_PROBE = HOME / "workspaces" / "main_probe"
WORKSPACES = HOME / "workspaces"
GIT_DIRECTORIES = HOME / "git_directories"
DATABASE = HOME / "factory.sqlite"
CODEX_HOME = HOME / "codex_home"
STATUS_FILE = HOME / "STATUS.md"
STOP_FILE = HOME / "STOP"
LOGS = HOME / "logs"
PROPOSALS = HOME / "proposals"
VENV_PYTHON = REPOSITORY / ".venv" / "bin" / "python"
MAP = pathlib.Path("data/ver/eu/map.csv")
FACTORY_SOURCE_DIRECTORY = pathlib.Path("Game/backup/src/Factory")
TEXT_BASE = 0x100000
MAX_FUNCTION_BYTES = 256
GROUP_CHUNK = 25
SYNC_INTERVAL_SECONDS = 45 * 60
# Verified matches land on TARGET_BRANCH. Its ref only moves, in one compare-and-swap step, to a commit
# whose every new function tools/check.py already reported O on CANDIDATE_BRANCH, a scratch branch.
TARGET_BRANCH = "factory"
CANDIDATE_BRANCH = "integration-candidate"
EASTERN = datetime.timezone(datetime.timedelta(hours=-4))

TIERS = {
    1: {"model": "gpt-6-luna", "effort": "medium", "attempts": 5, "timeout": 20 * 60},
    2: {"model": "gpt-6.1-sol", "effort": "high", "attempts": 8, "timeout": 40 * 60},
}

ANSI = re.compile(r"\x1b\[[0-9;]*[A-Za-z]")


# ---------------------------------------------------------------- map handling

def read_map_lines(path):
    lines = path.read_text().splitlines()
    rows = {}
    for index, line in enumerate(lines[1:], start=1):
        fields = line.split(",")
        rows[int(fields[0], 16)] = index
    return lines, rows


def parse_row(line):
    fields = line.split(",")
    return {
        "start": int(fields[0], 16),
        "pool": int(fields[1], 16) if fields[1].strip() else None,
        "end": int(fields[2], 16),
        "rank": fields[4].strip(),
        "type": fields[5].strip(),
        "symbol": fields[6].strip(),
    }


def default_name(row):
    if row["symbol"]:
        return row["symbol"]
    return ("fn_%08X" if "f" in row["type"] else "dat_%08X") % row["start"]


def load_rows(path):
    lines, _ = read_map_lines(path)
    return [parse_row(line) for line in lines[1:]]


def set_map_rows(worktree, changes):
    """changes: {address: (rank or None, symbol or None)}."""
    path = worktree / MAP
    lines, index = read_map_lines(path)
    for address, (rank, symbol) in changes.items():
        fields = lines[index[address]].split(",")
        if rank is not None:
            fields[4] = rank
        if symbol is not None:
            fields[6] = symbol
        lines[index[address]] = ",".join(fields)
    path.write_text("\n".join(lines) + "\n")


def merge_map(base_path, ours_path, theirs_path):
    """Row-wise three-way merge keyed by start address. Returns True when clean."""
    def load(path):
        text = pathlib.Path(path).read_text().splitlines()
        return text[0], {line.split(",")[0]: line for line in text[1:] if line.strip()}

    header, base = load(base_path)
    _, ours = load(ours_path)
    _, theirs = load(theirs_path)
    result, clean = {}, True
    for key in set(base) | set(ours) | set(theirs):
        b, o, t = base.get(key), ours.get(key), theirs.get(key)
        if o == t:
            chosen = o
        elif o == b:
            chosen = t
        elif t == b:
            chosen = o
        else:
            clean = False
            chosen = o
        if chosen is not None:
            result[key] = chosen
    ordered = [result[key] for key in sorted(result, key=lambda k: int(k, 16))]
    pathlib.Path(ours_path).write_text("\n".join([header] + ordered) + "\n")
    return clean


# ---------------------------------------------------------------- binary helpers

def read_code():
    return (INTEGRATION / "data/ver/eu/code.bin").read_bytes()


def normalized_body(code, row):
    end = row["end"]
    pool = row["pool"] or end
    words = struct.unpack_from("<%dI" % ((end - row["start"]) // 4), code, row["start"] - TEXT_BASE)
    parts = []
    for i, word in enumerate(words):
        if row["start"] + 4 * i >= pool:
            parts.append("P")
        elif (word & 0x0E000000) == 0x0A000000:
            parts.append("%02X" % (word >> 24))
        else:
            parts.append("%08X" % word)
    return " ".join(parts)


class Symbols:
    def __init__(self, rows):
        self.rows = sorted(rows, key=lambda r: r["start"])
        self.starts = [r["start"] for r in self.rows]

    def describe(self, value):
        i = bisect.bisect_right(self.starts, value) - 1
        if i < 0:
            return None
        row = self.rows[i]
        if value >= row["end"] and value != row["start"]:
            return None
        name = default_name(row)
        offset = value - row["start"]
        return name if offset == 0 else f"{name}+0x{offset:X}"


def demangle(names):
    names = [n for n in names if n.startswith("_Z")]
    if not names:
        return {}
    output = subprocess.run(["c++filt"], input="\n".join(names), capture_output=True, text=True).stdout
    return dict(zip(names, output.splitlines()))


def disassemble(code, row, symbols):
    from capstone import Cs, CS_ARCH_ARM, CS_MODE_ARM
    disassembler = Cs(CS_ARCH_ARM, CS_MODE_ARM)
    start, end = row["start"], row["end"]
    pool = row["pool"] or end
    body = code[start - TEXT_BASE:pool - TEXT_BASE]
    lines, references = [], []
    for instruction in disassembler.disasm(body, start):
        text = f"{instruction.mnemonic} {instruction.op_str}".strip()
        note = ""
        if instruction.mnemonic.startswith("b") and instruction.op_str.startswith("#0x"):
            target = int(instruction.op_str[1:], 16)
            if not (start <= target < end):
                name = symbols.describe(target)
                if name:
                    note = f"   ; -> {name}"
                    references.append(name)
        lines.append(f"  {instruction.address:08X}  {text}{note}")
    last = lines[-1] if lines else ""
    terminators = ("bx lr", "pop {", "b #", "ldm ", "pop.w")
    if lines and not any(t in last for t in terminators) and not re.search(r"\bpc\b", last.split(";")[0].split(None, 1)[-1]):
        following = symbols.describe(end)
        if following:
            lines.append(f"  ; execution falls through into the next function, {following}: the linker removed a tail call"
                         f" to it (the final NOP replaced 'b {following}'). Write this as a tail call: return {following}(...).")
            references.append(f"tail call into {following}")
    for address in range(pool, end, 4):
        value = struct.unpack_from("<I", code, address - TEXT_BASE)[0]
        name = symbols.describe(value) if 0x100000 <= value < 0x600000 else None
        if name:
            references.append(name)
            note = f"   ; &{name}"
        else:
            as_float = struct.unpack("<f", struct.pack("<I", value))[0]
            note = f"   ; {value} / {as_float:g}f" if 1e-6 < abs(as_float) < 1e9 else f"   ; {value}"
        lines.append(f"  {address:08X}  .word 0x{value:08X}{note}")
    return "\n".join(lines), references


# ---------------------------------------------------------------- database

def connect():
    database = sqlite3.connect(DATABASE, check_same_thread=False, timeout=30)
    database.row_factory = sqlite3.Row
    database.executescript("""
    CREATE TABLE IF NOT EXISTS jobs (
        id INTEGER PRIMARY KEY, kind TEXT, addresses TEXT, body_bytes INTEGER, total_bytes INTEGER,
        tier INTEGER, priority REAL, status TEXT, sibling TEXT, attempts INTEGER DEFAULT 0,
        leased_by TEXT, leased_at REAL, finished_at REAL, matched TEXT DEFAULT '', note TEXT DEFAULT '');
    CREATE TABLE IF NOT EXISTS runs (
        id INTEGER PRIMARY KEY, job_id INTEGER, slot TEXT, tier INTEGER, model TEXT, effort TEXT,
        started REAL, finished REAL, outcome TEXT, input_tokens INTEGER DEFAULT 0,
        cached_tokens INTEGER DEFAULT 0, output_tokens INTEGER DEFAULT 0,
        matched_count INTEGER DEFAULT 0, matched_bytes INTEGER DEFAULT 0, note TEXT DEFAULT '');
    CREATE TABLE IF NOT EXISTS events (time REAL, kind TEXT, text TEXT);
    """)
    return database


DATABASE_LOCK = threading.Lock()


def execute(database, sql, parameters=()):
    with DATABASE_LOCK:
        cursor = database.execute(sql, parameters)
        database.commit()
        return cursor


def log_event(database, kind, text):
    execute(database, "INSERT INTO events VALUES (?,?,?)", (time.time(), kind, text))
    with open(LOGS / "supervisor.log", "a") as stream:
        stream.write(f"{datetime.datetime.now(EASTERN):%m-%d %H:%M:%S} {kind}: {text}\n")


# ---------------------------------------------------------------- queue

def excluded_addresses():
    excluded = set()
    for path in (INTEGRATION / "project/pro_requests").glob("*.md"):
        if re.fullmatch(r"[0-9A-Fa-f]{8}", path.stem):
            excluded.add(int(path.stem, 16))
    blocked = INTEGRATION / "project/blocked.md"
    if blocked.exists():
        excluded |= {int(h, 16) for h in re.findall(r"0x([0-9A-Fa-f]{8})", blocked.read_text())}
    ledger = INTEGRATION / "project/ledger.csv"
    if ledger.exists():
        import csv
        for entry in csv.DictReader(open(ledger)):
            if entry["outcome"] in ("abandoned", "nonmatching"):
                excluded.add(int(entry["address"], 16))
    return excluded


def command_init(arguments):
    database = connect()
    if execute(database, "SELECT COUNT(*) FROM jobs").fetchone()[0] and not arguments.force:
        print("queue already exists; pass --force to rebuild")
        return
    execute(database, "DELETE FROM jobs")
    code = read_code()
    rows = target_rows()
    excluded = excluded_addresses()
    small = [r for r in rows if r["type"] == "f" and r["end"] - r["start"] < MAX_FUNCTION_BYTES]
    groups = collections.defaultdict(list)
    for row in small:
        groups[normalized_body(code, row)].append(row)
    jobs = []
    for members in groups.values():
        open_members = [r for r in members if r["rank"] == "U" and r["start"] not in excluded]
        if not open_members:
            continue
        matched = [r for r in members if r["rank"] == "O"]
        size = members[0]["end"] - members[0]["start"]
        sibling = default_name(matched[0]) if matched else ""
        tier = 1 if size <= 64 else 2
        for i in range(0, len(open_members), GROUP_CHUNK):
            chunk = open_members[i:i + GROUP_CHUNK]
            kind = "group" if len(chunk) > 1 else "single"
            total = size * len(chunk)
            if sibling:
                priority = 3_000_000 + total
            elif kind == "group":
                priority = 2_000_000 + total
            else:
                priority = 1_000_000 - size
            jobs.append((kind, ",".join("%08X" % r["start"] for r in chunk), size, total, tier, priority, sibling))
    with DATABASE_LOCK:
        database.executemany(
            "INSERT INTO jobs (kind, addresses, body_bytes, total_bytes, tier, priority, status, sibling)"
            " VALUES (?,?,?,?,?,?,'open',?)", jobs)
        database.commit()
    counts = collections.Counter((j[0], j[4]) for j in jobs)
    print(f"{len(jobs)} jobs covering {sum(len(j[1].split(',')) for j in jobs)} functions,"
          f" {sum(j[3] for j in jobs)} bytes; {dict(counts)}")


# ---------------------------------------------------------------- git and workspaces

def git(worktree, *arguments, check=True):
    result = subprocess.run(["git", "-C", str(worktree), *arguments], capture_output=True, text=True)
    if check and result.returncode != 0:
        raise RuntimeError(f"git {' '.join(arguments)}: {result.stderr.strip()}")
    return result.stdout.strip()


def target_commit():
    return git(INTEGRATION, "rev-parse", f"refs/heads/{TARGET_BRANCH}")


def target_rows():
    """Map rows at the target branch tip, never the candidate worktree in the middle of a verification."""
    text = git(INTEGRATION, "show", f"refs/heads/{TARGET_BRANCH}:{MAP}")
    return [parse_row(line) for line in text.splitlines()[1:] if line.strip()]


def move_target(new, expected, reason):
    """Move the target ref from expected to new in one step. Fails, moving nothing, if it no longer points at expected."""
    result = subprocess.run(["git", "-C", str(INTEGRATION), "update-ref", "-m", reason,
                             f"refs/heads/{TARGET_BRANCH}", new, expected], capture_output=True, text=True)
    return result.returncode == 0


def tool(worktree, *arguments, timeout=900):
    environment = dict(os.environ)
    environment["DEVKITARM"] = "/opt/homebrew"
    environment["VIRTUAL_ENV"] = str(REPOSITORY / ".venv")
    environment["PATH"] = f"{REPOSITORY / '.venv/bin'}:{environment['PATH']}"
    result = subprocess.run([str(VENV_PYTHON), *arguments], cwd=worktree, capture_output=True,
                            text=True, env=environment, timeout=timeout)
    return result.returncode, ANSI.sub("", result.stdout + result.stderr)


def prepare_worktree(worktree):
    """Workers get their own shared clone, so their sandbox never writes into the owner's repository."""
    if not (worktree / ".git").exists():
        # The Codex sandbox keeps any .git inside a workspace read-only, so the git directory lives outside it.
        GIT_DIRECTORIES.mkdir(exist_ok=True)
        subprocess.run(["git", "clone", "-q", "--shared", "--no-checkout", "--separate-git-dir",
                        str(GIT_DIRECTORIES / worktree.name), str(REPOSITORY), str(worktree)], check=True)
        git(worktree, "fetch", "-q", "origin", TARGET_BRANCH)
        git(worktree, "checkout", "-q", "--detach", "FETCH_HEAD")
    exclude = GIT_DIRECTORIES / worktree.name / "info/exclude"
    if ".factory" not in exclude.read_text():
        exclude.write_text(exclude.read_text() + "\n.venv\n.factory\n")
    for link, target in ((".venv", REPOSITORY / ".venv"), ("data/compilers", REPOSITORY / "data/compilers")):
        path = worktree / link
        if not path.exists():
            path.parent.mkdir(parents=True, exist_ok=True)
            path.symlink_to(target)
    for name in ("code.bin", "exh.bin"):
        path = worktree / "data/ver/eu" / name
        if not path.exists():
            shutil.copy2(REPOSITORY / "data/ver/eu" / name, path)
    if not (worktree / "tools/asm-differ/diff.py").exists():
        git(worktree, "submodule", "update", "--init", "tools/asm-differ")


def reset_worktree(worktree, commit):
    git(worktree, "fetch", "-q", "origin", TARGET_BRANCH)
    git(worktree, "checkout", "-q", "--detach", "-f", commit)
    git(worktree, "clean", "-fdq", "--", "Game", "lib")
    shutil.rmtree(worktree / ".factory", ignore_errors=True)


def changed_tracked_files(worktree, base):
    """Files changed since the job's base commit, other than the map and the factory directory."""
    output = subprocess.run(["git", "-C", str(worktree), "diff", "--name-only", base],
                            capture_output=True, text=True, check=True).stdout
    allowed = (str(MAP), str(FACTORY_SOURCE_DIRECTORY) + "/")
    return [path for path in output.splitlines() if path and not (path == allowed[0] or path.startswith(allowed[1]))]


def new_source_files(worktree, base):
    tracked = git(worktree, "diff", "--name-only", base, "--", str(FACTORY_SOURCE_DIRECTORY)).splitlines()
    untracked = git(worktree, "ls-files", "--others", "--exclude-standard", "--", str(FACTORY_SOURCE_DIRECTORY)).splitlines()
    return sorted({p for p in tracked + untracked if p.endswith((".cpp", ".h")) and (worktree / p).exists()})


def commit_attempt(worktree):
    """The checker only credits committed source, so each attempt is a throwaway commit in the worker worktree."""
    git(worktree, "add", "-A", "--", str(FACTORY_SOURCE_DIRECTORY), str(MAP))
    git(worktree, "-c", "user.name=factory", "-c", "user.email=factory@localhost",
        "commit", "-q", "--allow-empty", "--no-verify", "-m", "factory attempt")


# ---------------------------------------------------------------- packets

WORKER_GUIDE = """You are a matching-decompilation worker for Super Mario 3D Land (EU Rev 2, 3DS, 32-bit ARM).
The compiler is ARMCC 4.1 build 791 with -O3 -Otime --arm_only --gnu --signed_chars --enum_is_int --force_new_nothrow.
Your whole task is in .factory/job.md: write C++ that compiles to exactly the bytes of the listed functions.

Rules:
1. Write code only in the one new file named in job.md. Never edit, rename or delete any existing file. Never commit.
2. Use plain C++. No inline assembly, no __asm, no raw bytes, no copied machine code, no pragmas that pin code.
3. Define each function with the exact symbol given. Unnamed functions are extern "C" functions named fn_XXXXXXXX.
   For a mangled symbol, declare the class or namespace locally so the definition produces that symbol.
4. Declare every type you need inside an anonymous namespace in your file. Declare callees and globals as extern
   with the names job.md gives (fn_XXXXXXXX functions as extern "C"; dat_XXXXXXXX data as extern "C" objects).
5. Check your work only with:  . ./development_environment.sh && python {factory} attempt
   It builds, then prints MATCHED or an assembly diff (target on the left) for each function.
   One run takes 30 to 120 seconds. Wait for it to finish; never start a second run while one is going.
   You have {attempts} attempt runs. Stop as soon as everything matches or the runs are used up.
6. Before you finish, delete every function that still does not match, so the file holds only matching code.
   If nothing matches, delete the file.
7. Do not read project/, tools/ or other source files unless job.md points you to them. Keep it short.
Finish with one line per function: the address and matched or failed."""


def build_packet(job, worktree, rows_by_start, symbols, code, readable):
    addresses = [int(a, 16) for a in job["addresses"].split(",")]
    tier = TIERS[job["tier"]]
    first = rows_by_start[addresses[0]]
    file_name = FACTORY_SOURCE_DIRECTORY / (("fn_%08X.cpp" if len(addresses) == 1 else "group_%08X.cpp") % addresses[0])
    names = []
    for address in addresses:
        row = rows_by_start[address]
        name = default_name(row)
        names.append(name)
    listing, _ = disassemble(code, first, symbols)
    parts = [f"# Job {job['id']}", "", f"Write: `{file_name}`", ""]
    if len(addresses) == 1:
        symbol = names[0]
        parts += [f"Function `{symbol}`" + (f" (`{readable[symbol]}`)" if symbol in readable else "") +
                  f" at 0x{addresses[0]:08X}, {first['end'] - first['start']} bytes.", "", "```", listing, "```"]
    else:
        parts += [f"{len(addresses)} functions share one body ({first['end'] - first['start']} bytes each);"
                  " only call targets and literal-pool words differ. Write the body once per symbol.", "",
                  f"Template, `{names[0]}` at 0x{addresses[0]:08X}:", "", "```", listing, "```", "",
                  "Each function and what its branches and pool words refer to:", ""]
        for address, name in zip(addresses, names):
            _, references = disassemble(code, rows_by_start[address], symbols)
            shown = ", ".join(references) if references else "no external references"
            label = f"`{name}`" + (f" (`{readable[name]}`)" if name in readable else "")
            parts.append(f"- {label} at 0x{address:08X}: {shown}")
    if job["sibling"]:
        source = find_sibling_source(job["sibling"], readable)
        if source:
            parts += ["", f"An already matching function with the same body, `{job['sibling']}`:", "", "```cpp", source, "```"]
    context_names = [job["sibling"]] if job["sibling"] else []
    context_names += [n for n in names if n.startswith("_Z")]
    shown = set()
    for name in context_names:
        for class_name in class_names(readable.get(name, name)):
            if class_name in shown:
                continue
            definition = find_class_definition(class_name)
            if definition:
                shown.add(class_name)
                parts += ["", f"Existing declaration of `{class_name}` (you may copy what you need into your file):",
                          "", "```cpp", definition, "```"]
            if len(shown) >= 2:
                break
    parts += ["", f"Check with `. ./development_environment.sh && python {HOME / 'factory.py'} attempt`"
              f" ({tier['attempts']} runs)."]
    factory_directory = worktree / ".factory"
    factory_directory.mkdir(exist_ok=True)
    (factory_directory / "job.md").write_text("\n".join(parts) + "\n")
    (factory_directory / "job.json").write_text(json.dumps({
        "id": job["id"], "addresses": addresses, "symbols": names, "file": str(file_name),
        "attempt_limit": tier["attempts"], "base": git(worktree, "rev-parse", "HEAD")}))
    return addresses, names


def class_names(readable_name):
    """Outermost-to-innermost class names from a demangled name, e.g. al::FunctorV0M<...>::clone -> FunctorV0M."""
    depth, cleaned = 0, ""
    for character in readable_name:
        if character == "<":
            depth += 1
        elif character == ">":
            depth -= 1
        elif depth == 0:
            if character == "(":
                break
            cleaned += character
    pieces = [p for p in cleaned.split("::") if p]
    return [p for p in reversed(pieces[:-1]) if p[:1].isupper()]


def find_class_definition(class_name, limit=70):
    pattern = re.compile(r"^\s*(?:template\s*<[^>]*>\s*)?(?:class|struct)\s+" + re.escape(class_name) + r"\b[^;]*$", re.M)
    for root in ("Game/backup/include", "lib/al/include", "Game/backup/src", "lib/al/src"):
        for path in sorted((INTEGRATION / root).rglob("*.h")) + sorted((INTEGRATION / root).rglob("*.hpp")):
            text = path.read_text(errors="ignore")
            match = pattern.search(text)
            if not match:
                continue
            brace = text.find("{", match.start())
            if brace == -1:
                continue
            depth, cursor = 0, brace
            while cursor < len(text):
                if text[cursor] == "{":
                    depth += 1
                elif text[cursor] == "}":
                    depth -= 1
                    if depth == 0:
                        block = text[match.start():cursor + 2].splitlines()
                        relative = path.relative_to(INTEGRATION)
                        if len(block) > limit:
                            block = block[:limit] + [f"    // ... truncated; full text in {relative}"]
                        return f"// {relative}\n" + "\n".join(block)
                cursor += 1
    return ""


def find_sibling_source(symbol, readable):
    needle = readable.get(symbol, symbol)
    needle = needle.split("(")[0]
    for path in list((INTEGRATION / "Game").rglob("*.cpp")) + list((INTEGRATION / "lib/al").rglob("*.cpp")):
        text = path.read_text(errors="ignore")
        position = text.find(needle + "(")
        while position != -1:
            brace = text.find("{", position)
            semicolon = text.find(";", position)
            if brace != -1 and (semicolon == -1 or brace < semicolon):
                depth, cursor = 0, brace
                while cursor < len(text):
                    if text[cursor] == "{":
                        depth += 1
                    elif text[cursor] == "}":
                        depth -= 1
                        if depth == 0:
                            line_start = text.rfind("\n", 0, position) + 1
                            return text[line_start:cursor + 1]
                    cursor += 1
            position = text.find(needle + "(", position + 1)
    return ""


# ---------------------------------------------------------------- attempt (inside a worker worktree)

def command_attempt(arguments):
    import fcntl
    worktree = pathlib.Path.cwd()
    # One build at a time per worktree: overlapping builds corrupt the provenance projection.
    lock = open(worktree / ".factory/attempt.lock", "w")
    fcntl.flock(lock, fcntl.LOCK_EX)
    job = json.loads((worktree / ".factory/job.json").read_text())
    counter = worktree / ".factory/attempts"
    used = int(counter.read_text()) if counter.exists() else 0
    if used >= job["attempt_limit"] and not arguments.final:
        print(f"ATTEMPT LIMIT REACHED ({used}). Delete non-matching functions and finish now.")
        sys.exit(3)
    if not arguments.final:
        counter.write_text(str(used + 1))
    changed = changed_tracked_files(worktree, job["base"])
    if changed:
        print("You changed existing files, which is not allowed. Restore them with git checkout:", *changed, sep="\n  ")
        sys.exit(2)
    results = attempt_in(worktree, job, commit=True)
    matched = [a for a, r in results.items() if r["rank"] == "O"]
    for address, result in results.items():
        print(f"\n=== 0x{address:08X} {result['symbol']}: {'MATCHED' if result['rank'] == 'O' else 'rank ' + result['rank']}")
        if result["detail"]:
            print(result["detail"])
    print(f"\nSUMMARY: {len(matched)}/{len(results)} matched. Attempt {used + (0 if arguments.final else 1)}"
          f" of {job['attempt_limit']}.")
    if arguments.final:
        print("FINAL " + json.dumps(matched))


def attempt_in(worktree, job, final=False, commit=False):
    set_map_rows(worktree, {a: ("M", s) for a, s in zip(job["addresses"], job["symbols"])})
    if commit:
        commit_attempt(worktree)
    status, output = tool(worktree, "make.py", "eu")
    if status != 0:
        tail = "\n".join(output.splitlines()[-60:])
        return {a: {"symbol": s, "rank": "build-failed", "detail": "BUILD FAILED:\n" + tail if not final else ""}
                for a, s in zip(job["addresses"], job["symbols"])}
    results = {}
    for address, symbol in zip(job["addresses"], job["symbols"]):
        tool(worktree, "tools/check.py", symbol, timeout=300)
        lines, index = read_map_lines(worktree / MAP)
        rank = parse_row(lines[index[address]])["rank"]
        detail = ""
        if rank != "O" and not final:
            _, diff = tool(worktree, "tools/diff.py", symbol, "-c", timeout=300)
            detail = "\n".join(diff.splitlines()[:90])
        results[address] = {"symbol": symbol, "rank": rank, "detail": detail}
    return results


# ---------------------------------------------------------------- supervisor

class Supervisor:
    def __init__(self, slots):
        self.database = connect()
        self.slots = slots
        self.integration_queue = queue.Queue()
        self.integration_lock = threading.Lock()
        self.active = {}
        self.stopping = False
        self.halted = ""
        self.last_sync = 0.0
        self.code = read_code()
        self.reload_symbols()

    def reload_symbols(self):
        rows = target_rows()
        self.rows_by_start = {r["start"]: r for r in rows}
        self.symbols = Symbols(rows)
        self.readable = demangle([r["symbol"] for r in rows if r["symbol"]])

    # leasing -------------------------------------------------------------
    def lease(self, slot, tier):
        live_rows = {}
        try:
            for row in load_rows(REPOSITORY / MAP):
                live_rows[row["start"]] = row["rank"]
        except Exception:
            pass
        with DATABASE_LOCK:
            candidates = self.database.execute(
                "SELECT * FROM jobs WHERE status='open' AND tier=? ORDER BY priority DESC LIMIT 50", (tier,)).fetchall()
            for job in candidates:
                addresses = [int(a, 16) for a in job["addresses"].split(",")]
                still_open = [a for a in addresses if live_rows.get(a, "U") == "U"
                              and self.rows_by_start.get(a, {}).get("rank") == "U"]
                if not still_open:
                    self.database.execute("UPDATE jobs SET status='skipped', note='taken elsewhere' WHERE id=?", (job["id"],))
                    continue
                address_text = ",".join("%08X" % a for a in still_open)
                self.database.execute(
                    "UPDATE jobs SET status='leased', leased_by=?, leased_at=?, addresses=?, attempts=attempts+1 WHERE id=?",
                    (slot, time.time(), address_text, job["id"]))
                self.database.commit()
                leased = dict(job)
                leased["addresses"] = address_text
                return leased
            self.database.commit()
        return None

    # workers -------------------------------------------------------------
    def worker_loop(self, slot, tier):
        worktree = WORKSPACES / f"worker_{slot}"
        try:
            prepare_worktree(worktree)
        except Exception as error:
            log_event(self.database, "error", f"{slot}: workspace setup failed: {error}")
            return
        while not self.stopping and not self.halted:
            job = self.lease(slot, tier)
            if not job:
                time.sleep(60)
                continue
            try:
                self.run_job(slot, worktree, job)
            except Exception as error:
                log_event(self.database, "error", f"{slot} job {job['id']}: {error}")
                execute(self.database, "UPDATE jobs SET status='open' WHERE id=?", (job["id"],))
                time.sleep(30)
            finally:
                self.active.pop(slot, None)

    def run_job(self, slot, worktree, job):
        settings = TIERS[job["tier"]]
        head = target_commit()
        reset_worktree(worktree, head)
        addresses, names = build_packet(job, worktree, self.rows_by_start, self.symbols, self.code, self.readable)
        set_map_rows(worktree, {a: ("M", s) for a, s in zip(addresses, names)})
        started = time.time()
        self.active[slot] = {"job": job["id"], "tier": job["tier"], "count": len(addresses),
                             "first": addresses[0], "bytes": job["body_bytes"], "started": started}
        run_id = execute(self.database,
                         "INSERT INTO runs (job_id, slot, tier, model, effort, started, outcome) VALUES (?,?,?,?,?,?,'running')",
                         (job["id"], slot, job["tier"], settings["model"], settings["effort"], started)).lastrowid
        prompt = WORKER_GUIDE.format(factory=HOME / "factory.py", attempts=settings["attempts"]) + \
            "\n\nStart by reading .factory/job.md."
        log_path = LOGS / "runs" / f"run_{run_id}.jsonl"
        log_path.parent.mkdir(parents=True, exist_ok=True)
        environment = dict(os.environ, CODEX_HOME=str(CODEX_HOME))
        command = ["codex", "exec", "--json", "--ephemeral", "--skip-git-repo-check", "-C", str(worktree),
                   "-s", "workspace-write", "--add-dir", str(GIT_DIRECTORIES / worktree.name), "-m", settings["model"],
                   "-c", f"model_reasoning_effort=\"{settings['effort']}\"", prompt]
        with open(log_path, "w") as log_stream:
            process = subprocess.Popen(command, stdout=log_stream, stderr=subprocess.STDOUT, stdin=subprocess.DEVNULL,
                                       env=environment, start_new_session=True)
            try:
                process.wait(timeout=settings["timeout"])
                outcome = "finished"
            except subprocess.TimeoutExpired:
                os.killpg(process.pid, signal.SIGTERM)
                process.wait()
                outcome = "timeout"
        usage = {"input_tokens": 0, "cached_input_tokens": 0, "output_tokens": 0}
        for line in open(log_path, errors="ignore"):
            if '"turn.completed"' in line:
                try:
                    turn = json.loads(line).get("usage", {})
                    for key in usage:
                        usage[key] += turn.get(key, 0)
                except ValueError:
                    pass
        # The checker, not the worker, decides.
        job_data = json.loads((worktree / ".factory/job.json").read_text())
        bad_edits = changed_tracked_files(worktree, job_data["base"])
        files = new_source_files(worktree, job_data["base"])
        matched = []
        if not bad_edits and files:
            results = attempt_in(worktree, job_data, final=True, commit=True)
            matched = [a for a, r in results.items() if r["rank"] == "O"]
        matched_bytes = sum(self.rows_by_start[a]["end"] - self.rows_by_start[a]["start"] for a in matched)
        note = f"edited existing files: {bad_edits}" if bad_edits else ""
        execute(self.database,
                "UPDATE runs SET finished=?, outcome=?, input_tokens=?, cached_tokens=?, output_tokens=?,"
                " matched_count=?, matched_bytes=?, note=? WHERE id=?",
                (time.time(), outcome, usage["input_tokens"], usage["cached_input_tokens"], usage["output_tokens"],
                 len(matched), matched_bytes, note, run_id))
        remaining = [a for a in addresses if a not in matched]
        proposal_job = job["id"]
        if not remaining:
            execute(self.database, "UPDATE jobs SET status='proposed', finished_at=? WHERE id=?", (time.time(), job["id"]))
        else:
            remaining_text = ",".join("%08X" % a for a in remaining)
            if job["tier"] < max(TIERS):
                execute(self.database, "UPDATE jobs SET status='open', tier=?, addresses=?, leased_by=NULL WHERE id=?",
                        (job["tier"] + 1, remaining_text, job["id"]))
            else:
                execute(self.database, "UPDATE jobs SET status='failed', addresses=?, finished_at=? WHERE id=?",
                        (remaining_text, time.time(), job["id"]))
            if matched:
                proposal_job = execute(
                    self.database, "INSERT INTO jobs (kind, addresses, body_bytes, total_bytes, tier, priority, status, sibling)"
                    " VALUES ('split',?,?,?,?,0,'proposed','')",
                    (",".join("%08X" % a for a in matched), job["body_bytes"], matched_bytes, job["tier"])).lastrowid
        if matched:
            proposal = PROPOSALS / f"job_{proposal_job}_run_{run_id}"
            shutil.rmtree(proposal, ignore_errors=True)
            for relative in files:
                destination = proposal / relative
                destination.parent.mkdir(parents=True, exist_ok=True)
                shutil.copy2(worktree / relative, destination)
            attempts_file = worktree / ".factory/attempts"
            attempts_used = int(attempts_file.read_text()) if attempts_file.exists() else 1
            (proposal / "matched.json").write_text(json.dumps(
                {"job": proposal_job, "run": run_id, "tier": job["tier"], "model": settings["model"],
                 "attempts": attempts_used, "minutes": (time.time() - started) / 60,
                 "addresses": matched, "symbols": {a: n for a, n in zip(addresses, names)}}))
            self.integration_queue.put(proposal)
        log_event(self.database, "run", f"{slot} job {job['id']} tier {job['tier']}: {len(matched)}/{len(addresses)} matched,"
                  f" {usage['input_tokens'] - usage['cached_input_tokens']} uncached + {usage['output_tokens']} out, {outcome}")

    # integration ---------------------------------------------------------
    def integrator_loop(self):
        while not (self.stopping and self.integration_queue.empty()):
            try:
                proposal = self.integration_queue.get(timeout=30)
            except queue.Empty:
                if time.time() - self.last_sync > SYNC_INTERVAL_SECONDS and not self.halted:
                    self.sync()
                continue
            if self.halted:
                continue
            try:
                self.integrate(proposal)
            except Exception as error:
                log_event(self.database, "error", f"integration of {proposal.name} failed: {error}")
                self.restore_integration()

    def restore_integration(self):
        """Put the scratch candidate branch back on the target tip, discarding any unverified work."""
        git(INTEGRATION, "checkout", "-q", "-f", "-B", CANDIDATE_BRANCH, f"refs/heads/{TARGET_BRANCH}")
        git(INTEGRATION, "clean", "-fdq", "--", str(FACTORY_SOURCE_DIRECTORY))

    def integrate(self, proposal):
        with self.integration_lock:
            metadata = json.loads((proposal / "matched.json").read_text())
            addresses = [int(a) for a in metadata["addresses"]]
            symbols = {int(k): v for k, v in metadata["symbols"].items()}
            self.restore_integration()
            base = git(INTEGRATION, "rev-parse", "HEAD")
            live = {r["start"]: r["rank"] for r in load_rows(REPOSITORY / MAP)}
            current = {r["start"]: r["rank"] for r in load_rows(INTEGRATION / MAP)}
            collisions = [a for a in addresses if live.get(a, "U") != "U" or current.get(a, "U") != "U"]
            if collisions:
                log_event(self.database, "skip", f"{proposal.name}: already matched elsewhere {['%08X' % a for a in collisions]}")
                return
            copied = []
            for path in proposal.rglob("*"):
                if path.is_file() and path.suffix in (".cpp", ".h"):
                    relative = path.relative_to(proposal)
                    destination = INTEGRATION / relative
                    if destination.exists():
                        raise RuntimeError(f"{relative} already exists on {TARGET_BRANCH}")
                    destination.parent.mkdir(parents=True, exist_ok=True)
                    shutil.copy2(path, destination)
                    copied.append(str(relative))
            set_map_rows(INTEGRATION, {a: ("M", symbols[a]) for a in addresses})
            # The checker only credits committed source, so the candidate is committed on the scratch branch first.
            git(INTEGRATION, "add", "--", str(MAP), *copied)
            git(INTEGRATION, "commit", "-q", "--no-verify", "-m", f"Candidate {proposal.name}, not verified")
            job = {"addresses": addresses, "symbols": [symbols[a] for a in addresses]}
            results = attempt_in(INTEGRATION, job, final=True)
            failed = [a for a, r in results.items() if r["rank"] != "O"]
            if failed:
                log_event(self.database, "reject", f"{proposal.name}: not exact on a candidate of {TARGET_BRANCH}:"
                          f" {['%08X' % a for a in failed]}")
                self.restore_integration()
                return
            sizes = {a: self.rows_by_start[a]["end"] - self.rows_by_start[a]["start"] for a in addresses}
            lines = [f"- 0x{a:08X} {symbols[a]} ({sizes[a]} bytes)" for a in addresses]
            message = (f"Match {len(addresses)} function{'s' if len(addresses) > 1 else ''} at 0x{addresses[0]:08X}"
                       f" via factory ({metadata['model']})\n\n" + "\n".join(lines) +
                       "\n\nVerified by tools/check.py on a candidate commit before the branch moved.\n\n"
                       "Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>")
            ledger = INTEGRATION / "project/ledger.csv"
            stamp = datetime.datetime.now(EASTERN).isoformat(timespec="seconds")
            share = metadata.get("minutes", 0) / len(addresses)
            with open(ledger, "a") as stream:
                for address in addresses:
                    stream.write(f"{stamp},0x{address:08X},{symbols[address]},matched,{metadata.get('attempts', 1)},{share:.4f}\n")
            # One final commit on the base: the same checked source, the map as the checker left it, and the ledger.
            git(INTEGRATION, "reset", "-q", "--soft", base)
            git(INTEGRATION, "add", "--", str(MAP), "project/ledger.csv")
            git(INTEGRATION, "commit", "-q", "-m", message)
            final = git(INTEGRATION, "rev-parse", "HEAD")
            if not move_target(final, base, f"factory: integrate {proposal.name}"):
                log_event(self.database, "alert", f"{proposal.name}: {TARGET_BRANCH} moved during verification; requeued")
                self.restore_integration()
                self.integration_queue.put(proposal)
                return
            execute(self.database, "UPDATE jobs SET status='matched', matched=? WHERE id=?",
                    (",".join("%08X" % a for a in addresses), metadata["job"]))
            for address in addresses:
                self.rows_by_start[address]["rank"] = "O"
            log_event(self.database, "match", f"{len(addresses)} functions, {sum(sizes.values())} bytes, first 0x{addresses[0]:08X}")
            shutil.rmtree(proposal, ignore_errors=True)

    def sync(self):
        """Merge the owner's main into the target branch on the candidate, verify every match, move the target, push."""
        with self.integration_lock:
            self.last_sync = time.time()
            self.restore_integration()
            base = git(INTEGRATION, "rev-parse", "HEAD")
            if TARGET_BRANCH != "main":
                result = subprocess.run(["git", "-C", str(INTEGRATION), "merge", "-q", "-m",
                                         f"Merge branch 'main' into {TARGET_BRANCH}", "main"],
                                        capture_output=True, text=True)
                if result.returncode != 0:
                    subprocess.run(["git", "-C", str(INTEGRATION), "merge", "--abort"], capture_output=True)
                    log_event(self.database, "alert", f"merge of main failed: {result.stdout.strip()[-300:]} {result.stderr.strip()[-300:]}")
                    self.restore_integration()
                    return
            status, output = tool(INTEGRATION, "make.py", "eu")
            if status != 0:
                self.halted = f"{TARGET_BRANCH} does not build after merging main"
                log_event(self.database, "alert", self.halted + ": " + output[-400:])
                self.restore_integration()
                return
            changes_file = INTEGRATION / "data/ver/eu/.changes"
            changes_file.unlink(missing_ok=True)
            tool(INTEGRATION, "tools/check.py", "-q", "-w", timeout=1800)
            regressions = []
            if changes_file.exists():
                for line in changes_file.read_text().splitlines():
                    match = re.match(r"^(\S+) -> (\S+) (\S+)", line)
                    if match and match[1] == "O" and match[2] != "O":
                        regressions.append(match[3])
            if regressions:
                inherited = self.failing_on_main(regressions)
                own = [symbol for symbol in regressions if symbol not in inherited]
                if inherited:
                    log_event(self.database, "alert", f"main itself is not exact for {inherited[:10]}; not caused by the factory")
                if own:
                    self.halted = f"regressions after sync that main does not have: {own[:10]}"
                    log_event(self.database, "alert", self.halted)
                    self.restore_integration()
                    return
            merged = git(INTEGRATION, "rev-parse", "HEAD")
            if merged != base and not move_target(merged, base, "factory: merge main after a full check"):
                log_event(self.database, "alert", f"{TARGET_BRANCH} moved during sync; nothing merged")
                self.restore_integration()
                return
            self.reload_symbols()
            push = subprocess.run(["git", "-C", str(INTEGRATION), "push", "-q", "origin", TARGET_BRANCH],
                                  capture_output=True, text=True)
            after = sum(1 for r in self.rows_by_start.values() if r["rank"] == "O")
            log_event(self.database, "sync", f"merged main; {after} functions exact, no regressions;"
                      f" push {'ok' if push.returncode == 0 else 'failed: ' + push.stderr.strip()[-200:]}")

    def failing_on_main(self, symbols):
        """Symbols the full checker also reports as lost on the owner's main tip (checker artifacts included)."""
        if not (MAIN_PROBE / ".git").exists():
            return []
        git(MAIN_PROBE, "checkout", "-q", "--detach", "-f", "main")
        status, _ = tool(MAIN_PROBE, "make.py", "eu")
        if status != 0:
            return list(symbols)
        changes_file = MAIN_PROBE / "data/ver/eu/.changes"
        changes_file.unlink(missing_ok=True)
        tool(MAIN_PROBE, "tools/check.py", "-q", "-w", timeout=1800)
        lost = set()
        if changes_file.exists():
            for line in changes_file.read_text().splitlines():
                match = re.match(r"^(\S+) -> (\S+) (\S+)", line)
                if match and match[1] == "O" and match[2] != "O":
                    lost.add(match[3])
        return [symbol for symbol in symbols if symbol in lost]

    # main loop -----------------------------------------------------------
    def run(self):
        STOP_FILE.unlink(missing_ok=True)
        for path in PROPOSALS.glob("job_*"):
            self.integration_queue.put(path)
        execute(self.database, "UPDATE jobs SET status='open', leased_by=NULL WHERE status='leased'")
        log_event(self.database, "start", f"slots {self.slots}")
        threads = [threading.Thread(target=self.integrator_loop, daemon=True)]
        for tier, count in self.slots.items():
            for index in range(count):
                threads.append(threading.Thread(target=self.worker_loop, args=(f"t{tier}_{index + 1}", tier), daemon=True))
        for thread in threads:
            thread.start()
        while True:
            write_status(self)
            if STOP_FILE.exists() and not self.stopping:
                self.stopping = True
                log_event(self.database, "stop", "stop requested; finishing current jobs")
            if self.stopping and not self.active and self.integration_queue.empty():
                break
            time.sleep(30)
        self.sync()
        write_status(self)
        log_event(self.database, "stop", "supervisor exited")


# ---------------------------------------------------------------- status

def write_status(supervisor=None):
    database = supervisor.database if supervisor else connect()
    now = time.time()
    with DATABASE_LOCK:
        jobs = database.execute("SELECT status, COUNT(*), SUM(LENGTH(addresses) - LENGTH(REPLACE(addresses, ',', '')) + 1)"
                                " FROM jobs GROUP BY status").fetchall()
        runs = database.execute("SELECT * FROM runs WHERE outcome != 'running'").fetchall()
        recent_runs = database.execute("SELECT * FROM runs WHERE finished > ?", (now - 3600,)).fetchall()
        events = database.execute("SELECT * FROM events ORDER BY time DESC LIMIT 12").fetchall()
        alerts = database.execute("SELECT * FROM events WHERE kind IN ('alert','error') ORDER BY time DESC LIMIT 5").fetchall()
    rows = target_rows()
    functions = [r for r in rows if "f" in r["type"]]
    total = sum(r["end"] - r["start"] for r in functions)
    exact = [r for r in functions if r["rank"] == "O"]
    exact_bytes = sum(r["end"] - r["start"] for r in exact)
    factory_commits = git(INTEGRATION, "log", "--oneline", f"main..{TARGET_BRANCH}", "--grep=via factory", check=False).splitlines()
    clock = lambda t: datetime.datetime.fromtimestamp(t, EASTERN).strftime("%H:%M")
    lines = [f"# Factory status, {datetime.datetime.now(EASTERN):%b %d %H:%M} ET", ""]
    state = "stopped"
    if supervisor:
        state = "halted: " + supervisor.halted if supervisor.halted else ("stopping" if supervisor.stopping else "running")
    lines += [f"State: **{state}**. Last sync with main: "
              f"{clock(supervisor.last_sync) if supervisor and supervisor.last_sync else 'not yet'}.", ""]
    lines += [f"Whole project on {TARGET_BRANCH}: **{exact_bytes / total * 100:.2f}%** of code bytes"
              f" ({exact_bytes:,} of {total:,}), {len(exact):,} of {len(functions):,} functions exact.",
              f"Factory commits not yet in main: {len(factory_commits)}.", ""]
    by_status = {r[0]: (r[1], r[2]) for r in jobs}
    lines += ["Queue: " + ", ".join(f"{k} {v[0]} jobs / {v[1]} functions" for k, v in sorted(by_status.items())), ""]
    lines += ["## Efficiency by tier", "",
              "| Tier | Model | Runs | Functions matched | Bytes matched | Uncached input | Output | Tokens per byte | Minutes per run |",
              "|---|---|---|---|---|---|---|---|---|"]
    grouped = collections.defaultdict(list)
    for run in runs:
        grouped[(run["tier"], run["model"], run["effort"])].append(run)
    for (tier, model, effort), items in sorted(grouped.items()):
        matched_bytes = sum(r["matched_bytes"] for r in items)
        uncached = sum(r["input_tokens"] - r["cached_tokens"] for r in items)
        output = sum(r["output_tokens"] for r in items)
        total_tokens = sum(r["input_tokens"] + r["output_tokens"] for r in items)
        per_byte = f"{total_tokens / matched_bytes:,.0f}" if matched_bytes else "n/a"
        minutes = sum((r["finished"] - r["started"]) for r in items) / 60 / len(items)
        lines.append(f"| {tier} | {model} {effort} | {len(items)} | {sum(r['matched_count'] for r in items)} |"
                     f" {matched_bytes:,} | {uncached:,} | {output:,} | {per_byte} | {minutes:.1f} |")
    hour_bytes = sum(r["matched_bytes"] for r in recent_runs)
    lines += ["", f"Last hour: {hour_bytes:,} bytes matched by workers in {len(recent_runs)} runs.", ""]
    if supervisor and supervisor.active:
        lines += ["## Working now", ""]
        for slot, info in sorted(supervisor.active.items()):
            lines.append(f"- {slot}: job {info['job']}, {info['count']} function(s) from 0x{info['first']:08X},"
                         f" {info['bytes']} bytes each, tier {info['tier']}, {int((now - info['started']) / 60)} min")
        lines.append("")
    if alerts:
        lines += ["## Alerts and errors", ""] + [f"- {clock(e['time'])} {e['kind']}: {e['text']}" for e in alerts] + [""]
    lines += ["## Recent events", ""] + [f"- {clock(e['time'])} {e['kind']}: {e['text']}" for e in events]
    STATUS_FILE.write_text("\n".join(lines) + "\n")
    return STATUS_FILE.read_text()


# ---------------------------------------------------------------- entry

def command_install_merge_driver(arguments):
    common = pathlib.Path(git(REPOSITORY, "rev-parse", "--git-common-dir"))
    if not common.is_absolute():
        common = REPOSITORY / common
    attributes = common / "info" / "attributes"
    attributes.parent.mkdir(exist_ok=True)
    existing = attributes.read_text() if attributes.exists() else ""
    for line in (f"{MAP} merge=mapcsv", "project/ledger.csv merge=union"):
        if line not in existing:
            existing += line + "\n"
    attributes.write_text(existing)
    git(REPOSITORY, "config", "merge.mapcsv.name", "row-wise map.csv merge keyed by start address")
    git(REPOSITORY, "config", "merge.mapcsv.driver", f"/usr/bin/python3 {HOME / 'factory.py'} merge-map %O %A %B")
    print("installed")


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    commands = parser.add_subparsers(dest="command", required=True)
    init = commands.add_parser("init")
    init.add_argument("--force", action="store_true")
    run = commands.add_parser("run")
    run.add_argument("--slots", default="1:1,2:1", help="tier:count pairs")
    commands.add_parser("status")
    attempt = commands.add_parser("attempt")
    attempt.add_argument("--final", action="store_true")
    commands.add_parser("install-merge-driver")
    merge = commands.add_parser("merge-map")
    merge.add_argument("base")
    merge.add_argument("ours")
    merge.add_argument("theirs")
    arguments = parser.parse_args()
    LOGS.mkdir(exist_ok=True)
    PROPOSALS.mkdir(exist_ok=True)
    if arguments.command == "init":
        command_init(arguments)
    elif arguments.command == "run":
        slots = {int(t): int(n) for t, n in (pair.split(":") for pair in arguments.slots.split(","))}
        Supervisor(slots).run()
    elif arguments.command == "status":
        print(write_status())
    elif arguments.command == "attempt":
        command_attempt(arguments)
    elif arguments.command == "install-merge-driver":
        command_install_merge_driver(arguments)
    elif arguments.command == "merge-map":
        sys.exit(0 if merge_map(arguments.base, arguments.ours, arguments.theirs) else 1)


if __name__ == "__main__":
    main()
