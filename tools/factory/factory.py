#!/usr/bin/env python3
"""Matching-decompilation factory for Super Mario 3D Land.

A work queue of small unmatched functions, stateless model workers in their own git
worktrees, and a deterministic integrator. Only tools/check.py decides what matches.

Commands (run with the repository's virtual environment python):
  init                 build the job queue from the target branch map
  run [--slots N]      supervise workers, integrate matches and submissions, run the periodic full check, write STATUS.md
  trial --plan ...     run single-function trials of named model settings, integrating any match
  trial-report         print match rate, minutes and tokens per byte for every trial run
  submit --branch ...  ask the integrator to verify a root/ or dot/ branch and land it on main
  status               rewrite and print STATUS.md and STATUS.json
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
# One build and one round of checks takes every queued proposal touching different files, up to these limits.
MAXIMUM_BATCH_PROPOSALS = 40
MAXIMUM_BATCH_FUNCTIONS = 400
# Verified matches land on TARGET_BRANCH. Its ref only moves, in one compare-and-swap step, to a commit
# whose every new function tools/check.py already reported O on CANDIDATE_BRANCH, a scratch branch.
TARGET_BRANCH = "main"
CANDIDATE_BRANCH = "integration-candidate"
# The build inputs tools/low/buildProvenance.py hashes against HEAD.
BUILD_INPUTS = ("Game", "lib", "data/config.json")
EASTERN = datetime.timezone(datetime.timedelta(hours=-4))

# A miss moves a job to its tier's "next"; with none, the job fails and becomes a GPT-6 Pro packet.
TIERS = {
    1: {"model": "gpt-6.1-sol", "effort": "high", "attempts": 5, "timeout": 10 * 60, "next": 2},
    2: {"model": "gpt-6.1-sol", "effort": "xhigh", "attempts": 8, "timeout": 15 * 60, "next": None},
    # Functions of 256 to 511 bytes (owner, 2026-10-02), in their own reserved slot.
    3: {"model": "gpt-6.1-sol", "effort": "xhigh", "attempts": 8, "timeout": 20 * 60, "next": None},
}
BAND_TIER = 3
BAND_BYTES = (256, 512)
# Model settings for single-function trials. A trial never escalates or fails a job; misses go back to the queue.
TRIAL_SETTINGS = {
    "sol-high": {"model": "gpt-6.1-sol", "effort": "high", "attempts": 5, "timeout": 10 * 60},
    "luna-medium": {"model": "gpt-6-luna", "effort": "medium", "attempts": 5, "timeout": 10 * 60},
    "luna-max": {"model": "gpt-6-luna", "effort": "max", "attempts": 5, "timeout": 10 * 60},
}
SIZE_BUCKETS = ((0, 32), (32, 64), (64, 128), (128, 256))
# Worker slots are not capped by brief rule 12 (owner, 2026-10-02): the swap guard runs as many as the Mac holds.
SLOT_THREADS = 10
STARTING_SLOTS = 4
SWAP_PAGES_PER_FIVE_MINUTES = 2048
QUIET_SECONDS_BEFORE_ADDING_A_SLOT = 15 * 60
NO_MATCH_HALT_RUNS = 50
MAXIMUM_PERIODIC_DEMOTIONS = 5
MINIMUM_FREE_DISK_BYTES = 5 * 1024 ** 3
HALT_ALL_FILE = HOME / "HALT_ALL"
STATUS_JSON = HOME / "STATUS.json"
# Lanes and the operator exchange files with the integrator here, inside the repository but excluded from git.
INTEGRATOR_EXCHANGE = REPOSITORY / ".integrator"
SUBMISSIONS = INTEGRATOR_EXCHANGE / "submissions"
SUBMISSION_RESULTS = INTEGRATOR_EXCHANGE / "results"
PRO_QUEUE = INTEGRATOR_EXCHANGE / "pro_queue"
PRO_ANSWERS = INTEGRATOR_EXCHANGE / "pro_answers"
PORT_STATUS = INTEGRATOR_EXCHANGE / "port_status.json"
PORT_MILESTONES = (
    "Static recompiler builds natively and reaches the first frame's GPU command stream, matching Azahar",
    "Rendering", "Input", "Audio", "World 1-1", "Browser build")
SUBMISSION_PREFIXES = ("root/", "dot/", "integrator/")
# The oracle (brief rule 2): a lane's change to these waits for the operator's review.
ORACLE_PATHS = ("tools/check.py", "tools/diff.py", "tools/progress.py", "tools/low/", "tools/asm-differ")

ANSI = re.compile(r"\x1b\[[0-9;]*[A-Za-z]")
# A provider error (model at capacity, rate limits, dropped streams) is retried at the same tier after a back-off;
# it never counts as a miss. A usage-policy flag halts everything (owner, 2026-10-02).
PROVIDER_RETRY_SECONDS = (60, 120, 300, 600, 900)
POLICY_FLAG = re.compile(r"usage polic|content polic|flagged (as|for)|violat\w* (our|the|openai)\W.{0,60}polic|safety (system|polic)", re.I)
POLICY_FLAG_IN_REPLY = re.compile(r"(flagged|violat\w*).{0,80}usage polic|usage polic.{0,80}(flagged|violat)", re.I)
LANES_FILE = HOME / "logs" / "lanes.json"
CODEX_SESSIONS = pathlib.Path.home() / ".codex" / "sessions"


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
        tier = 1
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


def build_packet(job, worktree, rows_by_start, symbols, code, readable, settings):
    addresses = [int(a, 16) for a in job["addresses"].split(",")]
    tier = settings
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
    if (job.get("note") or "").startswith("pro:"):
        answer = pathlib.Path(job["note"][len("pro:"):])
        if answer.exists():
            parts += ["", "GPT-6 Pro, which cannot compile, proposed the forms below after earlier workers failed."
                      " Try them in rank order, then adapt.", "", answer.read_text()]
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

def free_disk_bytes():
    return shutil.disk_usage(HOME).free


def swap_outs():
    output = subprocess.run(["vm_stat"], capture_output=True, text=True).stdout
    match = re.search(r"Swapouts:\s+(\d+)", output)
    return int(match[1]) if match else 0


def remote_target():
    output = git(INTEGRATION, "ls-remote", "origin", f"refs/heads/{TARGET_BRANCH}", check=False)
    return output.split()[0] if output else ""


def is_constructor_or_destructor(symbol, readable):
    """True for sead::CalendarTime::CalendarTime(...) or al::Foo<T>::~Foo(), judged from the demangled name."""
    name = readable.get(symbol, "")
    depth, cleaned = 0, ""
    for character in name:
        if character == "<":
            depth += 1
        elif character == ">":
            depth -= 1
        elif depth == 0:
            if character == "(":
                break
            cleaned += character
    pieces = [p for p in cleaned.split("::") if p]
    return len(pieces) >= 2 and (pieces[-1].startswith("~") or pieces[-1] == pieces[-2])


def lost_exact(changes_file):
    """Symbols the full checker moved from O to anything else."""
    lost = []
    if changes_file.exists():
        for line in changes_file.read_text().splitlines():
            match = re.match(r"^(\S+) -> (\S+) (\S+)", line)
            if match and match[1] == "O" and match[2] != "O":
                lost.append(match[3])
    return lost


def gained_exact(changes_file):
    gained = []
    if changes_file.exists():
        for line in changes_file.read_text().splitlines():
            match = re.match(r"^(\S+) -> (\S+) (\S+)", line)
            if match and match[1] != "O" and match[2] == "O":
                gained.append(match[3])
    return gained


def set_ranks_by_symbol(worktree, ranks):
    """ranks: {symbol: rank}. Returns the symbols whose row was found."""
    path = worktree / MAP
    lines = path.read_text().splitlines()
    found = []
    for index, line in enumerate(lines[1:], start=1):
        fields = line.split(",")
        if len(fields) > 6 and fields[6].strip() in ranks:
            fields[4] = ranks[fields[6].strip()]
            lines[index] = ",".join(fields)
            found.append(fields[6].strip())
    path.write_text("\n".join(lines) + "\n")
    return found


def rank_column_changes(before_text, after_text):
    """Rows (by start address) whose rank differs between two map texts."""
    def ranks(text):
        return {line.split(",")[0]: line.split(",")[4].strip() for line in text.splitlines()[1:] if line.strip()}
    before, after = ranks(before_text), ranks(after_text)
    return sorted(k for k in set(before) | set(after) if before.get(k) != after.get(k))


def build_inputs_equal(first, second):
    return all(git(INTEGRATION, "rev-parse", f"{first}:{path}") == git(INTEGRATION, "rev-parse", f"{second}:{path}")
               for path in BUILD_INPUTS)


def read_run_log(log_path):
    """Token usage, provider errors and usage-policy flags from a worker's codex exec log. A run that dies before
    turn.completed still reports usage through its session file's token counts, which this reads, then deletes."""
    usage = {"input_tokens": 0, "cached_input_tokens": 0, "output_tokens": 0}
    errors, flags, thread = [], [], None
    for line in open(log_path, errors="ignore"):
        try:
            event = json.loads(line)
        except ValueError:
            continue
        kind = event.get("type")
        if kind == "thread.started":
            thread = event.get("thread_id")
        elif kind == "turn.completed":
            for key in usage:
                usage[key] += event.get("usage", {}).get(key, 0)
        elif kind == "error":
            errors.append(str(event.get("message", "")))
        elif kind == "turn.failed":
            errors.append(str((event.get("error") or {}).get("message", "")))
        elif kind == "item.completed" and (event.get("item") or {}).get("type") == "agent_message":
            if POLICY_FLAG_IN_REPLY.search(str(event["item"].get("text", ""))):
                flags.append(str(event["item"]["text"]))
    flags += [message for message in errors if POLICY_FLAG.search(message)]
    errors = [message for message in errors if not POLICY_FLAG.search(message)]
    if thread:
        for session in (CODEX_HOME / "sessions").glob(f"*/*/*/rollout-*-{thread}.jsonl"):
            last = None
            for line in open(session, errors="ignore"):
                if '"token_count"' in line:
                    try:
                        last = (json.loads(line).get("payload", {}).get("info") or {}).get("total_token_usage") or last
                    except ValueError:
                        pass
            if last and last.get("input_tokens", 0) + last.get("output_tokens", 0) > usage["input_tokens"] + usage["output_tokens"]:
                usage = {key: last.get(key, 0) for key in usage}
            session.unlink(missing_ok=True)
    return usage, errors, flags


class SubmissionDecision(Exception):
    """A submission's outcome reached before the full check: rejected, held for the operator, or deferred."""

    def __init__(self, result):
        super().__init__(result.get("reason", result["outcome"]))
        self.result = result


class Supervisor:
    """slot_specs: {name: {"index": n, "tiers": [..], "kinds": [..] or None, "buckets": [(low, high)] or None,
    "settings": TIERS override or None, "trial": label or None, "trial_queue": queue.Queue or None}}."""

    def __init__(self, slot_specs, starting_slots=0):
        self.database = connect()
        self.slot_specs = slot_specs
        self.integration_queue = queue.Queue()
        self.integration_lock = threading.Lock()
        self.active = {}
        self.stopping = False
        self.halted = ""
        self.last_sync = 0.0  # the first idle moment runs a full check and push
        self.code = read_code()
        self.reload_symbols()
        self.expected_target = target_commit()
        self.last_pushed = remote_target()
        self.batch_failures = {}
        self.last_push_time = None
        self.consecutive_misses = 0
        self.backoff = {}
        self.lane_offsets = {}
        self.maximum_slots = len(slot_specs)
        self.allowed_slots = min(self.maximum_slots, starting_slots or self.maximum_slots)
        self.swap_samples = []
        self.last_slot_change = time.time()

    def reload_symbols(self):
        rows = target_rows()
        self.rows_by_start = {r["start"]: r for r in rows}
        self.symbols = Symbols(rows)
        self.readable = demangle([r["symbol"] for r in rows if r["symbol"]])

    def halt_all(self, reason):
        """Stop the factory and leave HALT_ALL for the operator, who stops every other lane and waits for the owner."""
        self.halted = reason
        HALT_ALL_FILE.write_text(f"{datetime.datetime.now(EASTERN).isoformat(timespec='seconds')} {reason}\n")
        log_event(self.database, "alert", "HALT ALL: " + reason)

    def target_moved_externally(self):
        current = target_commit()
        if current != self.expected_target:
            self.halt_all(f"{TARGET_BRANCH} moved outside the integrator: expected {self.expected_target[:9]}, found {current[:9]}")
            return True
        return False

    def move(self, new, expected, reason):
        if not move_target(new, expected, reason):
            self.halt_all(f"{TARGET_BRANCH} moved outside the integrator during '{reason}'")
            return False
        self.expected_target = new
        return True

    # leasing -------------------------------------------------------------
    def lease(self, slot):
        spec = self.slot_specs[slot]
        live_rows = {}
        try:
            for row in load_rows(REPOSITORY / MAP):
                live_rows[row["start"]] = row["rank"]
        except Exception:
            pass
        with DATABASE_LOCK:
            for tier in spec["tiers"]:
                query, parameters = "SELECT * FROM jobs WHERE status='open' AND tier=?", [tier]
                if spec.get("kinds"):
                    query += " AND kind IN (%s)" % ",".join("?" * len(spec["kinds"]))
                    parameters += list(spec["kinds"])
                if spec.get("buckets"):
                    query += " AND (" + " OR ".join("(body_bytes >= ? AND body_bytes < ?)" for _ in spec["buckets"]) + ")"
                    parameters += [bound for bucket in spec["buckets"] for bound in bucket]
                if spec.get("leave_small_singles"):
                    # Those tier 1 jobs belong to the Luna slot; they reach Sol only after Luna misses (tier 2).
                    query += " AND NOT (tier = 1 AND kind = 'single' AND body_bytes < ?)"
                    parameters.append(spec["leave_small_singles"])
                candidates = self.database.execute(query + " ORDER BY priority DESC LIMIT 50", parameters).fetchall()
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
    def worker_loop(self, slot):
        spec = self.slot_specs[slot]
        worktree = WORKSPACES / f"worker_{slot}"
        try:
            prepare_worktree(worktree)
        except Exception as error:
            log_event(self.database, "error", f"{slot}: workspace setup failed: {error}")
            return
        while not self.stopping and not self.halted:
            if spec.get("trial_queue") is not None and spec["trial_queue"].empty():
                break
            if spec["index"] >= self.allowed_slots:
                time.sleep(30)
                continue
            if spec.get("trial_queue") is not None:
                try:
                    job = spec["trial_queue"].get_nowait()
                except queue.Empty:
                    break
                execute(self.database, "UPDATE jobs SET status='leased', leased_by=?, leased_at=?, attempts=attempts+1 WHERE id=?",
                        (slot, time.time(), job["id"]))
            else:
                job = self.lease(slot)
                if not job:
                    time.sleep(60)
                    continue
            try:
                self.run_job(slot, worktree, job, spec)
            except Exception as error:
                log_event(self.database, "error", f"{slot} job {job['id']}: {error}")
                execute(self.database, "UPDATE jobs SET status='open', leased_by=NULL WHERE id=?", (job["id"],))
                time.sleep(30)
            finally:
                self.active.pop(slot, None)
            if self.backoff.get(slot):
                time.sleep(self.backoff[slot])

    def run_job(self, slot, worktree, job, spec):
        settings = spec.get("settings") or TIERS[job["tier"]]
        trial = spec.get("trial")
        head = target_commit()
        reset_worktree(worktree, head)
        addresses, names = build_packet(job, worktree, self.rows_by_start, self.symbols, self.code, self.readable, settings)
        set_map_rows(worktree, {a: ("M", s) for a, s in zip(addresses, names)})
        started = time.time()
        self.active[slot] = {"job": job["id"], "tier": job["tier"], "count": len(addresses), "first": addresses[0],
                             "bytes": job["body_bytes"], "started": started, "model": settings["model"],
                             "effort": settings["effort"], "trial": trial}
        run_id = execute(self.database,
                         "INSERT INTO runs (job_id, slot, tier, model, effort, started, outcome, note) VALUES (?,?,?,?,?,?,'running',?)",
                         (job["id"], slot, 0 if trial else job["tier"], settings["model"], settings["effort"], started,
                          f"trial {trial}" if trial else "")).lastrowid
        prompt = WORKER_GUIDE.format(factory=HOME / "factory.py", attempts=settings["attempts"]) + \
            "\n\nStart by reading .factory/job.md."
        log_path = LOGS / "runs" / f"run_{run_id}.jsonl"
        log_path.parent.mkdir(parents=True, exist_ok=True)
        environment = dict(os.environ, CODEX_HOME=str(CODEX_HOME))
        command = ["codex", "exec", "--json", "--skip-git-repo-check", "-C", str(worktree),
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
        usage, provider_errors, policy_flags = read_run_log(log_path)
        if policy_flags:
            self.halt_all(f"usage policy flag in run {run_id} ({slot}, job {job['id']}): {policy_flags[0][:200]}")
        if provider_errors:
            outcome = "error"
        # The checker, not the worker, decides.
        job_data = json.loads((worktree / ".factory/job.json").read_text())
        bad_edits = changed_tracked_files(worktree, job_data["base"])
        files = new_source_files(worktree, job_data["base"])
        matched = []
        if not bad_edits and files:
            results = attempt_in(worktree, job_data, final=True, commit=True)
            matched = [a for a, r in results.items() if r["rank"] == "O"]
        matched_bytes = sum(self.rows_by_start[a]["end"] - self.rows_by_start[a]["start"] for a in matched)
        note = f"edited existing files: {bad_edits}" if bad_edits else (f"trial {trial}" if trial else "")
        if provider_errors and not trial:
            note = (note + "; " if note else "") + "provider error: " + provider_errors[-1][:160]
        execute(self.database,
                "UPDATE runs SET finished=?, outcome=?, input_tokens=?, cached_tokens=?, output_tokens=?,"
                " matched_count=?, matched_bytes=?, note=? WHERE id=?",
                (time.time(), outcome, usage["input_tokens"], usage["cached_input_tokens"], usage["output_tokens"],
                 len(matched), matched_bytes, note, run_id))
        remaining = [a for a in addresses if a not in matched]
        remaining_text = ",".join("%08X" % a for a in remaining)
        proposal_job = job["id"]
        retry = bool(provider_errors) and not policy_flags
        if not remaining:
            execute(self.database, "UPDATE jobs SET status='proposed', finished_at=? WHERE id=?", (time.time(), job["id"]))
        elif trial or retry:
            execute(self.database, "UPDATE jobs SET status='open', addresses=?, leased_by=NULL WHERE id=?",
                    (remaining_text, job["id"]))
        elif (job["note"] or "").startswith("pro:"):
            execute(self.database, "UPDATE jobs SET status='failed', addresses=?, finished_at=?, note='pro tried' WHERE id=?",
                    (remaining_text, time.time(), job["id"]))
        elif TIERS[job["tier"]]["next"]:
            execute(self.database, "UPDATE jobs SET status='open', tier=?, addresses=?, leased_by=NULL WHERE id=?",
                    (TIERS[job["tier"]]["next"], remaining_text, job["id"]))
        else:
            execute(self.database, "UPDATE jobs SET status='failed', addresses=?, finished_at=? WHERE id=?",
                    (remaining_text, time.time(), job["id"]))
            self.queue_for_pro(worktree, job, remaining)
        if matched and remaining:
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
        self.backoff[slot] = (PROVIDER_RETRY_SECONDS[min(len(PROVIDER_RETRY_SECONDS) - 1, PROVIDER_RETRY_SECONDS.index(self.backoff[slot]) + 1)]
                              if self.backoff.get(slot) else PROVIDER_RETRY_SECONDS[0]) if retry else 0
        if not trial and not retry:
            self.consecutive_misses = 0 if matched else self.consecutive_misses + 1
            if self.consecutive_misses >= NO_MATCH_HALT_RUNS and not self.halted:
                self.halted = f"{NO_MATCH_HALT_RUNS} jobs in a row matched nothing"
                log_event(self.database, "alert", "factory halted: " + self.halted)
        log_event(self.database, "run", f"{slot} job {job['id']} {settings['model']} {settings['effort']}"
                  f"{' trial ' + trial if trial else ''}: {len(matched)}/{len(addresses)} matched,"
                  f" {usage['input_tokens'] - usage['cached_input_tokens']} uncached + {usage['output_tokens']} out, {outcome}"
                  + (f"; retrying at the same tier in {self.backoff[slot]} s: {provider_errors[-1][:120]}" if retry else ""))

    def queue_for_pro(self, worktree, job, remaining):
        """A job that failed the top tier becomes a self-contained GPT-6 Pro packet, named so a reverse sort is largest first."""
        total = job["body_bytes"] * len(remaining)
        directory = PRO_QUEUE / "failures"
        directory.mkdir(parents=True, exist_ok=True)
        packet = (worktree / ".factory/job.md").read_text()
        header = (f"# Factory failure, job {job['id']}\n\nAddresses: {', '.join('0x%08X' % a for a in remaining)}\n"
                  f"Bytes: {total} ({job['body_bytes']} per function)\nTarget commit: {target_commit()}\n"
                  "Compiler: ARMCC 4.1 build 791, -O3 -Otime --arm_only --gnu --signed_chars --enum_is_int --force_new_nothrow\n\n"
                  "Both Sol high and Sol xhigh workers failed to match this. Return up to three ranked complete C++ forms.\n\n")
        (directory / f"{total:05d}-{remaining[0]:08X}.md").write_text(header + packet)

    def take_pro_answers(self):
        """Reopen a failed job at tier 2 with Pro's answer attached, once."""
        directory = PRO_ANSWERS / "failures"
        if not directory.exists():
            return
        consumed = directory / "consumed"
        for answer in sorted(directory.glob("*.md")):
            match = re.search(r"([0-9A-F]{8})\.md$", answer.name)
            if not match:
                continue
            consumed.mkdir(exist_ok=True)
            kept = consumed / answer.name
            shutil.move(str(answer), kept)
            with DATABASE_LOCK:
                job = self.database.execute("SELECT * FROM jobs WHERE status='failed' AND addresses LIKE ? AND note != 'pro tried'",
                                            (f"{match[1]}%",)).fetchone()
            if job:
                tier = BAND_TIER if job["body_bytes"] >= BAND_BYTES[0] else 2
                execute(self.database, "UPDATE jobs SET status='open', tier=?, priority=9000000, note=? WHERE id=?",
                        (tier, f"pro:{kept}", job["id"]))
                log_event(self.database, "pro", f"job {job['id']} reopened with Pro's answer {answer.name}")

    def scan_lanes_for_policy_flags(self):
        """Read new events in the root's and the relay's Codex session files; a usage-policy flag halts everything."""
        try:
            lanes = json.loads(LANES_FILE.read_text())
        except (OSError, ValueError):
            return
        for lane in ("root", "pro_relay"):
            task = lanes.get(lane)
            if not task:
                continue
            path = self.lane_offsets.get(task, (None, 0))[0]
            if path is None:
                found = sorted(CODEX_SESSIONS.glob(f"*/*/*/rollout-*-{task}.jsonl"))
                if not found:
                    continue
                path = found[-1]
            offset = self.lane_offsets.get(task, (path, 0))[1]
            with open(path, "rb") as stream:
                stream.seek(offset)
                data = stream.read()
            complete = data[:data.rfind(b"\n") + 1]
            self.lane_offsets[task] = (path, offset + len(complete))
            for line in complete.decode(errors="ignore").splitlines():
                if "polic" not in line.lower() and "flagged" not in line.lower():
                    continue
                try:
                    payload = json.loads(line).get("payload", {})
                except ValueError:
                    continue
                kind, message = payload.get("type"), str(payload.get("message", ""))
                if (kind in ("error", "stream_error") and POLICY_FLAG.search(message)) or \
                        (kind == "agent_message" and POLICY_FLAG_IN_REPLY.search(message)):
                    self.halt_all(f"usage policy flag in the {lane} thread {task[:8]}: {message[:200]}")
                    return

    # integration ---------------------------------------------------------
    def integrator_loop(self):
        while not (self.stopping and self.integration_queue.empty()):
            if not self.halted and time.time() - self.last_sync > SYNC_INTERVAL_SECONDS:
                self.sync()
                continue
            batch = self.collect_batch()
            if self.halted:
                continue  # the proposals stay on disk and are queued again when the supervisor restarts
            if not batch:
                self.land_quiet_submissions()
                continue
            try:
                self.integrate_batch(batch)
            except Exception as error:
                log_event(self.database, "error", f"integration of a batch of {len(batch)} failed: {error}")
                self.restore_integration()
                for proposal in batch:
                    self.batch_failures[proposal.name] = self.batch_failures.get(proposal.name, 0) + 1
                    if self.batch_failures[proposal.name] >= 2:
                        self.retire(proposal, "rejected")
                    elif proposal.exists():
                        self.integration_queue.put(proposal)

    def collect_batch(self):
        try:
            batch = [self.integration_queue.get(timeout=30)]
        except queue.Empty:
            return []
        while len(batch) < MAXIMUM_BATCH_PROPOSALS:
            try:
                batch.append(self.integration_queue.get_nowait())
            except queue.Empty:
                break
        return batch

    def retire(self, proposal, kind):
        """Delete an integrated proposal, or keep a rejected or skipped one aside where a restart will not requeue it."""
        if kind is None:
            shutil.rmtree(proposal, ignore_errors=True)
            return
        destination = PROPOSALS / kind
        destination.mkdir(exist_ok=True)
        shutil.rmtree(destination / proposal.name, ignore_errors=True)
        if proposal.exists():
            shutil.move(str(proposal), destination / proposal.name)

    def restore_integration(self):
        """Put the scratch candidate branch back on the target tip, discarding any unverified work."""
        subprocess.run(["git", "-C", str(INTEGRATION), "merge", "--abort"], capture_output=True)
        git(INTEGRATION, "checkout", "-q", "-f", "-B", CANDIDATE_BRANCH, f"refs/heads/{TARGET_BRANCH}")
        git(INTEGRATION, "clean", "-fdq", "--", str(FACTORY_SOURCE_DIRECTORY), "Game", "lib")

    def integrate_batch(self, proposals):
        """Every queued proposal that touches different files goes into one candidate, one build and one round of
        checks. If any proposal misses, it is set aside and the rest go through a fresh cycle, so main only ever
        receives exactly the source that was checked."""
        with self.integration_lock:
            if self.target_moved_externally():
                return
            self.restore_integration()
            base = git(INTEGRATION, "rev-parse", "HEAD")
            live = {r["start"]: r["rank"] for r in load_rows(REPOSITORY / MAP)}
            current = {r["start"]: r["rank"] for r in load_rows(INTEGRATION / MAP)}
            entries, taken_addresses, taken_files, deferred = [], set(), set(), []
            for proposal in proposals:
                if not (proposal / "matched.json").exists():
                    continue
                metadata = json.loads((proposal / "matched.json").read_text())
                addresses = [int(a) for a in metadata["addresses"]]
                symbols = {int(k): v for k, v in metadata["symbols"].items()}
                collisions = [a for a in addresses if live.get(a, "U") != "U" or current.get(a, "U") != "U" or a in taken_addresses]
                if collisions:
                    log_event(self.database, "skip", f"{proposal.name}: already matched elsewhere {['%08X' % a for a in collisions]}")
                    self.retire(proposal, "skipped")
                    continue
                files = [str(p.relative_to(proposal)) for p in proposal.rglob("*") if p.is_file() and p.suffix in (".cpp", ".h")]
                existing = [f for f in files if (INTEGRATION / f).exists()]
                if existing:
                    log_event(self.database, "reject", f"{proposal.name}: {existing} already exist on {TARGET_BRANCH}")
                    self.retire(proposal, "rejected")
                    continue
                functions = sum(len(e["addresses"]) for e in entries)
                if any(f in taken_files for f in files) or (entries and functions + len(addresses) > MAXIMUM_BATCH_FUNCTIONS):
                    deferred.append(proposal)
                    continue
                for relative in files:
                    destination = INTEGRATION / relative
                    destination.parent.mkdir(parents=True, exist_ok=True)
                    shutil.copy2(proposal / relative, destination)
                taken_files.update(files)
                taken_addresses.update(addresses)
                entries.append({"proposal": proposal, "metadata": metadata, "addresses": addresses,
                                "symbols": symbols, "files": files})
            for proposal in deferred:
                self.integration_queue.put(proposal)
            if not entries:
                self.restore_integration()
                return
            set_map_rows(INTEGRATION, {a: ("M", e["symbols"][a]) for e in entries for a in e["addresses"]})
            # The checker only credits committed source, so the candidate is committed on the scratch branch first.
            git(INTEGRATION, "add", "--", str(MAP), *[f for e in entries for f in e["files"]])
            git(INTEGRATION, "commit", "-q", "--no-verify", "-m", f"Candidate batch of {len(entries)} proposals, not verified")
            candidate = git(INTEGRATION, "rev-parse", "HEAD")
            started = time.time()
            job = {"addresses": [a for e in entries for a in e["addresses"]],
                   "symbols": [e["symbols"][a] for e in entries for a in e["addresses"]]}
            results = attempt_in(INTEGRATION, job, final=True)
            failing = [e for e in entries if any(results[a]["rank"] != "O" for a in e["addresses"])]
            if failing:
                for entry in failing:
                    missed = ["%08X" % a for a in entry["addresses"] if results[a]["rank"] != "O"]
                    log_event(self.database, "reject", f"{entry['proposal'].name}: not exact on a candidate of {TARGET_BRANCH}: {missed}")
                    self.retire(entry["proposal"], "rejected")
                self.restore_integration()
                passing = [e for e in entries if e not in failing]
                for entry in passing:
                    self.integration_queue.put(entry["proposal"])
                log_event(self.database, "batch", f"{len(failing)} of {len(entries)} proposals missed; {len(passing)} go through a fresh cycle")
                return
            sizes = {a: self.rows_by_start[a]["end"] - self.rows_by_start[a]["start"] for e in entries for a in e["addresses"]}
            stamp = datetime.datetime.now(EASTERN).isoformat(timespec="seconds")
            sections = []
            with open(INTEGRATION / "project/ledger.csv", "a") as stream:
                for entry in entries:
                    metadata, addresses = entry["metadata"], entry["addresses"]
                    share = metadata.get("minutes", 0) / len(addresses)
                    for address in addresses:
                        stream.write(f"{stamp},0x{address:08X},{entry['symbols'][address]},matched,{metadata.get('attempts', 1)},{share:.4f}\n")
                    sections.append(f"{entry['proposal'].name} ({metadata['model']}):\n"
                                    + "\n".join(f"- 0x{a:08X} {entry['symbols'][a]} ({sizes[a]} bytes)" for a in addresses))
            functions = sum(len(e["addresses"]) for e in entries)
            first = entries[0]["addresses"][0]
            title = (f"Match {functions} function{'s' if functions > 1 else ''} at 0x{first:08X} via factory ({entries[0]['metadata']['model']})"
                     if len(entries) == 1 else f"Match {functions} functions in {len(entries)} factory proposals")
            message = (title + "\n\n" + "\n\n".join(sections) +
                       "\n\nVerified by tools/check.py on a candidate commit before the branch moved.\n\n"
                       "Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>")
            # One final commit on the base: the same checked source, the map as the checker left it, and the ledger.
            git(INTEGRATION, "reset", "-q", "--soft", base)
            git(INTEGRATION, "add", "--", str(MAP), "project/ledger.csv")
            git(INTEGRATION, "commit", "-q", "-m", message)
            final = git(INTEGRATION, "rev-parse", "HEAD")
            differing = [path for path in BUILD_INPUTS
                         if git(INTEGRATION, "rev-parse", f"{candidate}:{path}") != git(INTEGRATION, "rev-parse", f"{final}:{path}")]
            if differing:
                # The final commit must hold exactly the source the checker verified. Fail closed.
                self.halted = f"batch of {len(entries)}: final commit's {differing} trees differ from the checked candidate"
                log_event(self.database, "alert", self.halted + f"; {TARGET_BRANCH} not moved")
                self.restore_integration()
                return
            if not self.move(final, base, f"factory: integrate {len(entries)} proposal{'s' if len(entries) > 1 else ''}"):
                self.restore_integration()
                return
            for entry in entries:
                execute(self.database, "UPDATE jobs SET status='matched', matched=? WHERE id=?",
                        (",".join("%08X" % a for a in entry["addresses"]), entry["metadata"]["job"]))
                for address in entry["addresses"]:
                    self.rows_by_start[address]["rank"] = "O"
                log_event(self.database, "match", f"{len(entry['addresses'])} functions,"
                          f" {sum(sizes[a] for a in entry['addresses'])} bytes, first 0x{entry['addresses'][0]:08X}")
                self.retire(entry["proposal"], None)
            log_event(self.database, "batch", f"{len(entries)} proposals, {functions} functions, {sum(sizes.values())} bytes"
                      f" in one cycle of {int(time.time() - started)} s")

    # submissions from root/ and dot/ branches --------------------------------
    def pending_submissions(self):
        return sorted(SUBMISSIONS.glob("*.json"), key=lambda p: p.stat().st_mtime) if SUBMISSIONS.exists() else []

    def submission_changes_build(self, path):
        try:
            request = json.loads(path.read_text())
            if request.get("claims") or request.get("nonmatching"):
                return True
            branch = request.get("branch", "")
            if branch.startswith("dot/"):
                git(INTEGRATION, "fetch", "-q", "origin", f"+refs/heads/{branch}:refs/remotes/origin/{branch}", check=False)
            commit = git(INTEGRATION, "rev-parse", "--verify", f"{request.get('commit') or branch}^{{commit}}")
            changed = git(INTEGRATION, "diff", "--name-only", git(INTEGRATION, "merge-base", target_commit(), commit), commit)
            return any(p.startswith(("Game/", "lib/")) or p in ("data/config.json", str(MAP)) for p in changed.splitlines())
        except Exception:
            return True

    def decide(self, path, result):
        name = path.stem
        result.update({"submission": name, "decided": datetime.datetime.now(EASTERN).isoformat(timespec="seconds")})
        SUBMISSION_RESULTS.mkdir(parents=True, exist_ok=True)
        (SUBMISSION_RESULTS / f"{name}.json").write_text(json.dumps(result, indent=2) + "\n")
        processed = SUBMISSIONS / ("held" if result["outcome"] == "held" else "processed")
        processed.mkdir(exist_ok=True)
        shutil.move(str(path), processed / path.name)
        log_event(self.database, "submission", f"{name}: {result['outcome']}"
                  + (f", {result.get('reason', '')[:300]}" if result["outcome"] != "accepted" else
                     f", {len(result.get('matched', []))} exact, {result.get('matched_bytes', 0)} bytes, main {result['main'][:9]}"))

    def land_quiet_submissions(self):
        """Submissions that change no build input land at once; the rest ride the next periodic full check."""
        for path in self.pending_submissions():
            if self.halted or self.stopping:
                return
            if self.submission_changes_build(path):
                continue
            with self.integration_lock:
                if self.target_moved_externally():
                    return
                self.restore_integration()
                base = git(INTEGRATION, "rev-parse", "HEAD")
                try:
                    info = self.merge_submission(json.loads(path.read_text()), path.stem, base_only=True)
                except SubmissionDecision as decision:
                    self.restore_integration()
                    self.decide(path, decision.result)
                    continue
                merged = git(INTEGRATION, "rev-parse", "HEAD")
                if not build_inputs_equal(base, merged) or info["map_changed"] or info["claims"] or info["nonmatching"]:
                    self.restore_integration()
                    continue
                if not self.move(merged, base, f"factory: accept submission {path.stem} (no build input changes)"):
                    return
                self.decide(path, {"outcome": "accepted", "main": merged, "matched": [], "matched_bytes": 0,
                                   "checked": "no build inputs changed"})

    def merge_submission(self, request, name, base_only=False):
        """Validate a submission and merge it onto the candidate as it stands. Raises SubmissionDecision to reject,
        hold, or (when other submissions are already merged and this one conflicts with them) defer it."""
        branch = request.get("branch", "")
        if not branch.startswith(SUBMISSION_PREFIXES):
            raise SubmissionDecision({"outcome": "rejected", "reason": f"branch must start with one of {SUBMISSION_PREFIXES}"})
        if branch.startswith("dot/"):
            git(INTEGRATION, "fetch", "-q", "origin", f"+refs/heads/{branch}:refs/remotes/origin/{branch}", check=False)
        commit = git(INTEGRATION, "rev-parse", "--verify", f"{request.get('commit') or branch}^{{commit}}")
        claims, nonmatching = list(request.get("claims", [])), list(request.get("nonmatching", []))
        merge_base = git(INTEGRATION, "merge-base", target_commit(), commit)
        changed = git(INTEGRATION, "diff", "--name-only", merge_base, commit).splitlines()
        forbidden = [p for p in changed if p == "project/ledger.csv" or p.startswith(str(FACTORY_SOURCE_DIRECTORY) + "/")]
        if forbidden:
            raise SubmissionDecision({"outcome": "rejected", "reason": f"lanes may not change {forbidden}"})
        oracle = [p for p in changed if p == "data/config.json" or any(p == o or p.startswith(o) for o in ORACLE_PATHS)]
        if oracle and not request.get("operator_approved"):
            raise SubmissionDecision({"outcome": "held", "reason": f"oracle files {oracle} need the operator's review (brief rule 2)"})
        if str(MAP) in changed:
            if rank_column_changes(git(INTEGRATION, "show", f"{merge_base}:{MAP}"), git(INTEGRATION, "show", f"{commit}:{MAP}")):
                raise SubmissionDecision({"outcome": "rejected", "reason": "lanes never set ranks; the branch changes rank cells in map.csv"})
            for revision in git(INTEGRATION, "rev-list", f"{merge_base}..{commit}", "--", str(MAP)).splitlines():
                touched = git(INTEGRATION, "diff-tree", "--no-commit-id", "--name-only", "-r", revision).splitlines()
                extra = [p for p in touched if p != str(MAP) and not (p.startswith("project/") and p.endswith(".md"))]
                if extra:
                    raise SubmissionDecision({"outcome": "rejected", "reason": f"map.csv changes need their own evidence commit"
                                              f" (rule 2); {revision[:9]} also changes {extra}"})
        before = git(INTEGRATION, "rev-parse", "HEAD")
        merge = subprocess.run(["git", "-C", str(INTEGRATION), "merge", "--no-ff", "-q", "-m",
                                f"Merge {branch} (submission {name})", commit], capture_output=True, text=True)
        if merge.returncode != 0:
            subprocess.run(["git", "-C", str(INTEGRATION), "merge", "--abort"], capture_output=True)
            git(INTEGRATION, "reset", "-q", "--hard", before)
            if before != target_commit() and not base_only:
                raise SubmissionDecision({"outcome": "deferred"})
            raise SubmissionDecision({"outcome": "rejected", "reason": "does not merge cleanly onto main: "
                                      + (merge.stdout + merge.stderr).strip()[-400:]})
        rows = {r["start"]: r for r in load_rows(INTEGRATION / MAP)}
        by_symbol = {r["symbol"]: r["start"] for r in rows.values() if r["symbol"]}

        def resolve(claim):
            if claim in by_symbol:
                return by_symbol[claim]
            unnamed = re.fullmatch(r"fn_([0-9A-Fa-f]{8})", claim)
            if unnamed and int(unnamed[1], 16) in rows and not rows[int(unnamed[1], 16)]["symbol"]:
                return int(unnamed[1], 16)
            return None
        addresses = {claim: resolve(claim) for claim in claims + nonmatching}
        unknown = [claim for claim, address in addresses.items() if address is None or "f" not in rows[address]["type"]]
        if unknown:
            git(INTEGRATION, "reset", "-q", "--hard", before)
            raise SubmissionDecision({"outcome": "rejected", "reason": f"not function symbols in map.csv: {unknown[:10]}"})
        return {"branch": branch, "request": request, "claims": claims, "nonmatching": nonmatching,
                "addresses": addresses, "map_changed": str(MAP) in changed}

    # periodic full check and push -------------------------------------------
    def full_check(self, worktree):
        """Clean build and full-map check. Returns (built, build output tail, symbols lost from O, symbols gained O)."""
        status, output = tool(worktree, "make.py", "eu", "-ca", timeout=1800)
        if status != 0:
            return False, output[-400:], [], set()
        changes_file = worktree / "data/ver/eu/.changes"
        changes_file.unlink(missing_ok=True)
        tool(worktree, "tools/check.py", "-q", "-w", timeout=1800)
        return True, "", lost_exact(changes_file), set(gained_exact(changes_file))

    def demotions_allowed(self, demoted):
        if not demoted:
            return True
        readable = demangle(demoted)
        others = [s for s in demoted if not is_constructor_or_destructor(s, readable)]
        if len(demoted) > MAXIMUM_PERIODIC_DEMOTIONS or others:
            self.halt_all(f"the full check demoted {len(demoted)} rows, {len(others)} of them not constructors"
                          f" or destructors: {demoted[:10]}")
            return False
        return True

    def sync(self):
        """One clean build and full check serves the periodic check of main and every submission that changes build
        inputs: they ride along merged onto the candidate. A few unconfirmed constructors are demoted, more halts.
        If the combined check fails, main is checked alone in the probe worktree to tell its rows from the riders'."""
        with self.integration_lock:
            self.last_sync = time.time()
            if self.target_moved_externally():
                return
            self.restore_integration()
            base = git(INTEGRATION, "rev-parse", "HEAD")
            started = time.time()
            waiting = [p for p in self.pending_submissions() if self.submission_changes_build(p)]
            solo = [p for p in waiting if json.loads(p.read_text()).get("solo")]
            riders = []
            for path in (solo[:1] if solo else waiting):
                try:
                    riders.append((path, self.merge_submission(json.loads(path.read_text()), path.stem)))
                except SubmissionDecision as decision:
                    if decision.result["outcome"] != "deferred":
                        self.decide(path, decision.result)
            merged_head = git(INTEGRATION, "rev-parse", "HEAD")
            rows = {r["start"]: r for r in load_rows(INTEGRATION / MAP)}
            flips = {info["addresses"][claim]: ("M", claim) for _, info in riders for claim in info["claims"] + info["nonmatching"]
                     if rows[info["addresses"][claim]]["rank"] not in ("O", "M")}
            if flips:
                set_map_rows(INTEGRATION, flips)
                git(INTEGRATION, "add", "--", str(MAP))
                git(INTEGRATION, "commit", "-q", "--no-verify", "-m", f"Candidate ranks for {len(riders)} submissions, not verified")
            checked = git(INTEGRATION, "rev-parse", "HEAD")
            built, output, lost, gained = self.full_check(INTEGRATION)
            missing = {path: [c for c in info["claims"] if c not in gained and rows[info["addresses"][c]]["rank"] != "O"]
                       for path, info in riders}
            if riders and (not built or lost or any(missing.values())):
                # Tell main's own rows from the riders': check main alone in the probe worktree.
                git(MAIN_PROBE, "checkout", "-q", "--detach", "-f", base)
                git(MAIN_PROBE, "clean", "-fdq", "--", "Game", "lib")
                main_built, main_output, main_lost, _ = self.full_check(MAIN_PROBE)
                if not main_built:
                    self.halted = f"{TARGET_BRANCH} does not build cleanly"
                    log_event(self.database, "alert", self.halted + ": " + main_output)
                    self.restore_integration()
                    return
                own = sorted(set(lost) - set(main_lost)) if built else ["(the combined build fails: " + output[-200:] + ")"]
                if own or any(missing.values()):
                    for path, info in riders:
                        if len(riders) == 1:
                            reason = (("the build fails with this branch merged: " + output[-300:]) if not built else
                                      f"breaks previously exact functions: {own[:20]}" if own else
                                      f"claimed functions are not exact: {missing[path][:20]}")
                            self.decide(path, {"outcome": "rejected", "reason": reason})
                        else:
                            request = json.loads(path.read_text())
                            request["solo"] = True
                            path.write_text(json.dumps(request, indent=2) + "\n")
                    if len(riders) > 1:
                        log_event(self.database, "submission", f"{len(riders)} riders failed together; each rides alone from now on")
                    # Main alone: its own regressions follow the demotion policy below.
                    self.restore_integration()
                    riders, lost, gained, merged_head, checked = [], main_lost, set(), base, base
                built = True
            if not built:
                self.halted = f"{TARGET_BRANCH} does not build cleanly"
                log_event(self.database, "alert", self.halted + ": " + output)
                self.restore_integration()
                return
            if not self.demotions_allowed(lost):
                self.restore_integration()
                return
            # The final commit: rider merges stay; ranks, demotions and the ledger go into one integrator commit.
            git(INTEGRATION, "reset", "-q", "--soft", merged_head)
            rows = {r["start"]: r for r in load_rows(INTEGRATION / MAP)}
            ranks = {}
            for _, info in riders:
                for claim in info["claims"] + info["nonmatching"]:
                    address = info["addresses"][claim]
                    ranks[address] = ("O" if claim in gained else ("M" if rows[address]["rank"] != "O" else "O"), claim)
            if ranks:
                set_map_rows(INTEGRATION, ranks)
            if lost:
                set_ranks_by_symbol(INTEGRATION, {s: "M" for s in lost})
                log_event(self.database, "demote", f"periodic full check demoted to M: {lost}")
            stamp = datetime.datetime.now(EASTERN).isoformat(timespec="seconds")
            accepted = []
            with open(INTEGRATION / "project/ledger.csv", "a") as stream:
                for path, info in riders:
                    request = info["request"]
                    matched = [c for c in info["claims"] if c in gained]
                    for claim in matched:
                        stream.write(f"{stamp},0x{info['addresses'][claim]:08X},{claim},matched,{request.get('attempts', 1)},{request.get('minutes', 0)}\n")
                    for claim in info["nonmatching"]:
                        stream.write(f"{stamp},0x{info['addresses'][claim]:08X},{claim},nonmatching,{request.get('attempts', 1)},{request.get('minutes', 0)}\n")
                    accepted.append((path, info, matched))
            git(INTEGRATION, "add", "--", str(MAP), "project/ledger.csv")
            if subprocess.run(["git", "-C", str(INTEGRATION), "diff", "--cached", "--quiet"]).returncode != 0:
                lines = [f"Periodic full check: {len(riders)} submission{'s' if len(riders) != 1 else ''} accepted, {len(lost)} rows demoted"]
                for _, info, matched in accepted:
                    lines.append(f"\n{info['branch']}: {len(matched)} exact, {len(info['nonmatching'])} non-matching")
                    lines += [f"- {claim}" for claim in matched]
                if lost:
                    lines.append("\nDemoted to M, not confirmed by the full check:")
                    lines += [f"- {symbol}" for symbol in lost]
                lines.append("\nVerified by tools/check.py, full map, on a clean build of the candidate before main moved."
                             "\n\nCo-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>")
                git(INTEGRATION, "commit", "-q", "-m", "\n".join(lines))
            else:
                git(INTEGRATION, "reset", "-q", "--hard", merged_head)
            final = git(INTEGRATION, "rev-parse", "HEAD")
            if not build_inputs_equal(checked, final):
                self.halted = "periodic full check: final build inputs differ from the checked candidate"
                log_event(self.database, "alert", self.halted)
                self.restore_integration()
                return
            if final != base and not self.move(final, base, "factory: periodic full check"
                                               + (f" with {len(riders)} submissions" if riders else "")):
                self.restore_integration()
                return
            for path, info, matched in accepted:
                matched_bytes = sum(rows[info["addresses"][c]]["end"] - rows[info["addresses"][c]]["start"] for c in matched)
                self.decide(path, {"outcome": "accepted", "main": final, "matched": matched, "matched_bytes": matched_bytes,
                                   "checked": "rode the periodic full check"})
            remote = remote_target()
            if remote != self.last_pushed:
                self.halt_all(f"origin {TARGET_BRANCH} moved outside the integrator: expected {self.last_pushed[:9]}, found {remote[:9]}")
                return
            push = subprocess.run(["git", "-C", str(INTEGRATION), "push", "-q", "origin", f"refs/heads/{TARGET_BRANCH}"],
                                  capture_output=True, text=True)
            if push.returncode == 0:
                self.last_pushed = final
                self.last_push_time = time.time()
            self.reload_symbols()
            after = sum(1 for r in self.rows_by_start.values() if r["rank"] == "O")
            log_event(self.database, "sync", f"full check in {int(time.time() - started)} s; {after} functions exact;"
                      f" {len(riders)} submissions rode along; {len(lost)} demoted;"
                      f" push {'ok' if push.returncode == 0 else 'failed: ' + push.stderr.strip()[-200:]}")


    # main loop -----------------------------------------------------------
    def adjust_slots(self):
        """Run as many slots as the Mac holds without swapping: drop one on swap-outs, add one after 15 quiet minutes."""
        now = time.time()
        self.swap_samples = [(t, o) for t, o in self.swap_samples if t >= now - 1800] + [(now, swap_outs())]
        recent = [o for t, o in self.swap_samples if t >= now - 300]
        if recent[-1] - recent[0] > SWAP_PAGES_PER_FIVE_MINUTES and self.allowed_slots > 1 and now - self.last_slot_change > 300:
            self.allowed_slots -= 1
            self.last_slot_change = now
            log_event(self.database, "slots", f"swapping ({recent[-1] - recent[0]} pages in 5 min); allowed slots now {self.allowed_slots}")
        elif (self.allowed_slots < self.maximum_slots and now - self.last_slot_change > QUIET_SECONDS_BEFORE_ADDING_A_SLOT
              and self.swap_samples[-1][1] - min(o for t, o in self.swap_samples if t >= now - QUIET_SECONDS_BEFORE_ADDING_A_SLOT) == 0):
            self.allowed_slots += 1
            self.last_slot_change = now
            log_event(self.database, "slots", f"no swapping for 15 min; allowed slots now {self.allowed_slots}")

    def run(self, until_idle=False):
        STOP_FILE.unlink(missing_ok=True)
        for path in PROPOSALS.glob("job_*"):
            self.integration_queue.put(path)
        execute(self.database, "UPDATE jobs SET status='open', leased_by=NULL WHERE status='leased'")
        execute(self.database, "UPDATE runs SET outcome='abandoned' WHERE outcome='running'")
        # No run is in flight yet, so any worker session file left here belongs to a run that died with a supervisor.
        for session in (CODEX_HOME / "sessions").glob("*/*/*/rollout-*.jsonl"):
            session.unlink(missing_ok=True)
        log_event(self.database, "start", f"slots {sorted(self.slot_specs)}; target {TARGET_BRANCH} at {self.expected_target[:9]}")
        threads = [threading.Thread(target=self.integrator_loop, daemon=True)]
        workers = [threading.Thread(target=self.worker_loop, args=(slot,), daemon=True) for slot in self.slot_specs]
        for thread in threads + workers:
            thread.start()
        while True:
            write_status(self)
            if HALT_ALL_FILE.exists() and not self.halted:
                self.halted = HALT_ALL_FILE.read_text().strip()
            if free_disk_bytes() < MINIMUM_FREE_DISK_BYTES and not self.halted:
                self.halt_all(f"free disk is {free_disk_bytes() / 1024 ** 3:.1f} GB, under 5 GB")
            if not self.halted:
                if self.integration_lock.acquire(blocking=False):
                    try:
                        self.target_moved_externally()
                    finally:
                        self.integration_lock.release()
                self.adjust_slots()
                self.take_pro_answers()
                self.scan_lanes_for_policy_flags()
            if STOP_FILE.exists() and not self.stopping:
                self.stopping = True
                log_event(self.database, "stop", "stop requested; finishing current jobs")
            if until_idle and not any(w.is_alive() for w in workers) and not self.stopping:
                self.stopping = True
            if (self.stopping or self.halted) and not self.active and self.integration_queue.empty():
                break
            time.sleep(30)
        if not self.halted:
            self.sync()
        write_status(self)
        log_event(self.database, "stop", "supervisor exited" + (f" (halted: {self.halted})" if self.halted else ""))


# ---------------------------------------------------------------- trials

def select_trial_jobs(database, rows_by_start, plan, seed):
    """plan: [(label, count, maximum_bytes)]. Single-function jobs spread evenly over SIZE_BUCKETS below the maximum."""
    import random
    generator = random.Random(seed)
    live = {r["start"]: r["rank"] for r in load_rows(REPOSITORY / MAP)}
    chosen, used = {}, set()
    for label, count, maximum in plan:
        buckets = [(low, min(high, maximum)) for low, high in SIZE_BUCKETS if low < maximum]
        shares = [count // len(buckets) + (1 if i < count % len(buckets) else 0) for i in range(len(buckets))]
        chosen[label] = []
        for (low, high), share in zip(buckets, shares):
            candidates = [dict(row) for row in database.execute(
                "SELECT * FROM jobs WHERE kind='single' AND status='open' AND body_bytes >= ? AND body_bytes < ?",
                (low, high)).fetchall()]
            candidates = [job for job in candidates if job["id"] not in used
                          and rows_by_start.get(int(job["addresses"], 16), {}).get("rank") == "U"
                          and live.get(int(job["addresses"], 16), "U") == "U"]
            generator.shuffle(candidates)
            for job in candidates[:share]:
                used.add(job["id"])
                chosen[label].append(job)
    return chosen


def command_trial(arguments):
    plan = [(label, int(count), int(maximum)) for label, count, maximum in
            (item.split(":") for item in arguments.plan.split(","))]
    database = connect()
    rows_by_start = {r["start"]: r for r in target_rows()}
    chosen = select_trial_jobs(database, rows_by_start, plan, arguments.seed)
    specs, index = {}, 0
    for label, _, _ in plan:
        work = queue.Queue()
        for job in chosen[label]:
            work.put(job)
        for number in range(arguments.slots_per_label):
            specs[f"s{index + 1}"] = {"index": index, "tiers": [], "settings": TRIAL_SETTINGS[label],
                                      "trial": label, "trial_queue": work}
            index += 1
        print(f"{label}: {len(chosen[label])} jobs, sizes {sorted(j['body_bytes'] for j in chosen[label])}")
    Supervisor(specs).run(until_idle=True)
    print(trial_report())


def trial_report():
    database = connect()
    rows = database.execute("SELECT r.*, j.body_bytes FROM runs r JOIN jobs j ON j.id = r.job_id"
                            " WHERE r.note LIKE 'trial %' AND r.finished IS NOT NULL").fetchall()
    groups = collections.defaultdict(list)
    for row in rows:
        label = row["note"][len("trial "):]
        bucket = next(f"{low}-{high - 1}" for low, high in SIZE_BUCKETS if low <= row["body_bytes"] < high)
        groups[(label, bucket)].append(row)
        groups[(label, "all")].append(row)
    lines = ["| Label | Size bucket | Jobs | Matched | Match rate | Minutes per job | Tokens per byte (uncached + output) |",
             "|---|---|---|---|---|---|---|"]
    for (label, bucket), items in sorted(groups.items()):
        matched = sum(1 for r in items if r["matched_count"])
        matched_bytes = sum(r["matched_bytes"] for r in items)
        tokens = sum(r["input_tokens"] - r["cached_tokens"] + r["output_tokens"] for r in items)
        minutes = sum(r["finished"] - r["started"] for r in items) / 60 / len(items)
        per_byte = f"{tokens / matched_bytes:,.0f}" if matched_bytes else "n/a"
        lines.append(f"| {label} | {bucket} | {len(items)} | {matched} | {matched / len(items) * 100:.0f}% |"
                     f" {minutes:.1f} | {per_byte} |")
    text = "\n".join(lines) + "\n"
    (LOGS / "trial_report.md").write_text(text)
    return text


# ---------------------------------------------------------------- status

def write_status(supervisor=None):
    database = supervisor.database if supervisor else connect()
    now = time.time()
    with DATABASE_LOCK:
        jobs = database.execute("SELECT status, COUNT(*), SUM(LENGTH(addresses) - LENGTH(REPLACE(addresses, ',', '')) + 1),"
                                " SUM(total_bytes) FROM jobs GROUP BY status").fetchall()
        runs = database.execute("SELECT * FROM runs WHERE outcome NOT IN ('running', 'abandoned')").fetchall()
        events = database.execute("SELECT * FROM events ORDER BY time DESC LIMIT 12").fetchall()
        alerts = database.execute("SELECT * FROM events WHERE kind IN ('alert','error','demote') ORDER BY time DESC LIMIT 10").fetchall()
        matches = database.execute("SELECT time, text FROM events WHERE kind='match' AND time > ?", (now - 86400,)).fetchall()
        last_push = database.execute("SELECT MAX(time) FROM events WHERE kind='sync' AND text LIKE '%push ok%'").fetchone()[0]
    rows = target_rows()
    functions = [r for r in rows if "f" in r["type"]]
    total = sum(r["end"] - r["start"] for r in functions)
    exact = [r for r in functions if r["rank"] == "O"]
    exact_bytes = sum(r["end"] - r["start"] for r in exact)
    clock = lambda t: datetime.datetime.fromtimestamp(t, EASTERN).strftime("%H:%M")
    state = "stopped"
    if supervisor:
        state = "halted: " + supervisor.halted if supervisor.halted else ("stopping" if supervisor.stopping else "running")
    accepted = []
    for row in matches:
        found = re.search(r"(\d+) bytes", row["text"])
        if found:
            accepted.append((row["time"], int(found[1])))
    bytes_last = lambda hours: sum(b for t, b in accepted if t > now - hours * 3600)
    hourly = [sum(b for t, b in accepted if now - (h + 1) * 3600 < t <= now - h * 3600) for h in range(24)][::-1]
    tiers = []
    grouped = collections.defaultdict(list)
    for run in runs:
        label = run["note"][len("trial "):] if (run["note"] or "").startswith("trial ") else f"tier {run['tier']}"
        grouped[(label, run["model"], run["effort"])].append(run)
    for (label, model, effort), items in sorted(grouped.items()):
        matched_bytes = sum(r["matched_bytes"] for r in items)
        uncached_and_output = sum(r["input_tokens"] - r["cached_tokens"] + r["output_tokens"] for r in items)
        tiers.append({"label": label, "model": model, "effort": effort, "runs": len(items),
                      "runs_matched": sum(1 for r in items if r["matched_count"]),
                      "functions": sum(r["matched_count"] for r in items), "bytes": matched_bytes,
                      "tokens_per_byte": round(uncached_and_output / matched_bytes) if matched_bytes else None,
                      "failed_runs": sum(1 for r in items if r["outcome"] in ("error", "timeout")),
                      "failed_spend_tokens": sum(r["input_tokens"] - r["cached_tokens"] + r["output_tokens"]
                                                 for r in items if r["outcome"] in ("error", "timeout")),
                      "minutes_per_run": round(sum(r["finished"] - r["started"] for r in items) / 60 / len(items), 1)})
    slots = []
    if supervisor:
        for name, spec in sorted(supervisor.slot_specs.items(), key=lambda item: item[1]["index"]):
            info = supervisor.active.get(name)
            slots.append({"slot": name, "role": spec.get("role") or spec.get("trial") or "", "enabled": spec["index"] < supervisor.allowed_slots,
                          "job": info["job"] if info else None, "model": info["model"] if info else None,
                          "effort": info["effort"] if info else None, "functions": info["count"] if info else None,
                          "bytes": info["bytes"] if info else None,
                          "minutes": int((now - info["started"]) / 60) if info else None})
    status = {
        "generated": datetime.datetime.now(EASTERN).isoformat(timespec="seconds"), "state": state,
        "target": TARGET_BRANCH, "target_commit": target_commit(),
        "exact_functions": len(exact), "total_functions": len(functions), "exact_bytes": exact_bytes,
        "total_bytes": total, "percent": round(exact_bytes / total * 100, 3),
        "queue": [{"status": r[0], "jobs": r[1], "functions": r[2], "bytes": r[3]} for r in jobs],
        "slots": slots, "allowed_slots": supervisor.allowed_slots if supervisor else 0,
        "maximum_slots": supervisor.maximum_slots if supervisor else 0,
        "accepted_bytes": {"last_hour": bytes_last(1), "last_6_hours": bytes_last(6), "last_24_hours": bytes_last(24)},
        "accepted_bytes_by_hour": hourly, "tiers": tiers,
        "alerts": [{"time": clock(e["time"]), "kind": e["kind"], "text": e["text"]} for e in alerts],
        "events": [{"time": clock(e["time"]), "kind": e["kind"], "text": e["text"]} for e in events],
        "halt_all": HALT_ALL_FILE.read_text().strip() if HALT_ALL_FILE.exists() else "",
        "port": port_status(total),
        "integrator": integrator_queue(supervisor, last_push, now),
    }
    STATUS_JSON.write_text(json.dumps(status, indent=1) + "\n")
    lines = [f"# Factory status, {datetime.datetime.now(EASTERN):%b %d %H:%M} ET", "",
             f"State: **{state}**. Last full check: "
             f"{clock(supervisor.last_sync) if supervisor and supervisor.last_sync else 'not yet'}.", "",
             f"Whole project on {TARGET_BRANCH}: **{exact_bytes / total * 100:.2f}%** of code bytes"
             f" ({exact_bytes:,} of {total:,}), {len(exact):,} of {len(functions):,} functions exact.", "",
             "Queue: " + ", ".join(f"{q['status']} {q['jobs']} jobs / {q['functions']} functions" for q in status["queue"]), "",
             f"Accepted bytes: {bytes_last(1):,} last hour, {bytes_last(6):,} last 6 hours.", "",
             f"Integrator queue: {status['integrator']['proposals_waiting']} proposals, {status['integrator']['submissions_waiting']} submissions;"
             f" last push to origin {status['integrator']['minutes_since_push']} min ago.", "",
             "## Efficiency", "",
             "| Label | Model | Runs | Runs matched | Functions | Bytes | Tokens per byte (uncached + output) | Failed spend (error or timeout) | Minutes per run |",
             "|---|---|---|---|---|---|---|---|---|"]
    for tier in tiers:
        lines.append(f"| {tier['label']} | {tier['model']} {tier['effort']} | {tier['runs']} | {tier['runs_matched']} |"
                     f" {tier['functions']} | {tier['bytes']:,} | {tier['tokens_per_byte'] or 'n/a'} |"
                     f" {tier['failed_spend_tokens']:,} in {tier['failed_runs']} runs | {tier['minutes_per_run']} |")
    if slots:
        lines += ["", "## Slots", ""] + [
            f"- {s['slot']}: " + (f"job {s['job']}, {s['functions']} function(s), {s['bytes']} bytes each, {s['model']} {s['effort']},"
                                  f" {s['minutes']} min" if s["job"] else ("idle" if s["enabled"] else "paused (swap guard)"))
            for s in slots]
    if alerts:
        lines += ["", "## Alerts and errors", ""] + [f"- {clock(e['time'])} {e['kind']}: {e['text']}" for e in alerts]
    lines += ["", "## Recent events", ""] + [f"- {clock(e['time'])} {e['kind']}: {e['text']}" for e in events]
    STATUS_FILE.write_text("\n".join(lines) + "\n")
    return STATUS_FILE.read_text()


def integrator_queue(supervisor, last_push, now):
    """How much is waiting for the integrator, and how long origin has gone without a push."""
    submissions = sorted(SUBMISSIONS.glob("*.json")) if SUBMISSIONS.exists() else []
    riding = 0
    for path in submissions:
        try:
            request = json.loads(path.read_text())
        except (OSError, ValueError):
            continue
        riding += bool(request.get("claims") or request.get("nonmatching") or request.get("solo"))
    return {"proposals_waiting": len(list(PROPOSALS.glob("job_*"))), "submissions_waiting": len(submissions),
            "submissions_with_claims": riding,
            "last_push": datetime.datetime.fromtimestamp(last_push, EASTERN).isoformat(timespec="seconds") if last_push else "",
            "minutes_since_push": int((now - last_push) / 60) if last_push else None,
            "minutes_to_next_full_check": max(0, int((supervisor.last_sync + SYNC_INTERVAL_SECONDS - now) / 60))
            if supervisor and supervisor.last_sync else None}


def port_status(total_code_bytes):
    """Port milestones and recompiled share, from the file root keeps in .integrator/port_status.json."""
    reported = {}
    try:
        reported = json.loads(PORT_STATUS.read_text())
    except (OSError, ValueError):
        pass
    states = {m.get("number"): m for m in reported.get("milestones", []) if isinstance(m, dict)}
    milestones = [{"number": n, "name": name, "state": states.get(n, {}).get("state", "not started"),
                   "evidence": states.get(n, {}).get("evidence", "")} for n, name in enumerate(PORT_MILESTONES, start=1)]
    recompiled = reported.get("recompiled_bytes", 0) or 0
    return {"milestones": milestones, "recompiled_bytes": recompiled,
            "percent_recompiled": round(recompiled / total_code_bytes * 100, 3) if total_code_bytes else 0,
            "updated": reported.get("updated", "")}


def command_extend(arguments):
    """Add jobs for unmatched functions in a size band that the queue does not cover yet."""
    database = connect()
    code = read_code()
    rows = target_rows()
    queued = set()
    for (addresses,) in database.execute("SELECT addresses FROM jobs").fetchall():
        queued |= {int(a, 16) for a in addresses.split(",") if a}
    members_by_body = collections.defaultdict(list)
    for row in rows:
        size = row["end"] - row["start"]
        if row["type"] == "f" and arguments.minimum <= size < arguments.maximum:
            members_by_body[normalized_body(code, row)].append(row)
    jobs = []
    for members in members_by_body.values():
        open_members = [r for r in members if r["rank"] == "U" and r["start"] not in queued]
        if not open_members:
            continue
        matched = [r for r in members if r["rank"] == "O"]
        size = members[0]["end"] - members[0]["start"]
        sibling = default_name(matched[0]) if matched else ""
        for i in range(0, len(open_members), GROUP_CHUNK):
            chunk = open_members[i:i + GROUP_CHUNK]
            kind = "group" if len(chunk) > 1 else "single"
            total = size * len(chunk)
            priority = 3_000_000 + total if sibling else (2_000_000 + total if kind == "group" else 1_000_000 - size)
            jobs.append((kind, ",".join("%08X" % r["start"] for r in chunk), size, total, arguments.tier, priority, sibling))
    with DATABASE_LOCK:
        database.executemany(
            "INSERT INTO jobs (kind, addresses, body_bytes, total_bytes, tier, priority, status, sibling)"
            " VALUES (?,?,?,?,?,?,'open',?)", jobs)
        database.commit()
    print(f"{len(jobs)} jobs covering {sum(len(j[1].split(',')) for j in jobs)} functions, {sum(j[3] for j in jobs)} bytes,"
          f" tier {arguments.tier}; {dict(collections.Counter(j[0] for j in jobs))}")


def production_slot_specs(count, luna_single_bytes, group_only):
    """s1 is reserved for the 256-511 byte band; then Sol on the small queue, one Sol slot that prefers tier 2,
    one Luna slot for small singles, and further Sol slots. The swap guard pauses the highest-numbered slots first."""
    kinds = ["group"] if group_only else None
    roles = ["band", "tier1", "tier2", "luna" if luna_single_bytes and not group_only else "tier1"]
    roles += ["tier1"] * max(0, count - len(roles))
    specs = {}
    for index, role in enumerate(roles[:count]):
        if role == "band":
            spec = {"tiers": [BAND_TIER], "kinds": kinds}
        elif role == "luna":
            spec = {"tiers": [1], "kinds": ["single"], "buckets": [(0, luna_single_bytes)],
                    "settings": TRIAL_SETTINGS["luna-medium"]}
        else:
            spec = {"tiers": [2, 1] if role == "tier2" else [1, 2], "kinds": kinds, "leave_small_singles": luna_single_bytes}
        specs[f"s{index + 1}"] = dict(spec, index=index, role=role)
    return specs


def command_submit(arguments):
    """Write a submission request for the integrator. Used by root (root/ branches) and the operator (dot/, integrator/)."""
    commit = git(REPOSITORY, "rev-parse", "--verify", f"{arguments.commit or arguments.branch}^{{commit}}")
    SUBMISSIONS.mkdir(parents=True, exist_ok=True)
    name = arguments.name or re.sub(r"[^A-Za-z0-9._-]+", "-", arguments.branch) + "-" + commit[:9]
    request = {"branch": arguments.branch, "commit": commit,
               "claims": [s for s in arguments.claims.split(",") if s], "nonmatching": [s for s in arguments.nonmatching.split(",") if s],
               "summary": arguments.summary, "attempts": arguments.attempts, "minutes": arguments.minutes}
    (SUBMISSIONS / f"{name}.json").write_text(json.dumps(request, indent=2) + "\n")
    print(f"submitted {name}; the result will appear in {SUBMISSION_RESULTS / (name + '.json')}")


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
    run.add_argument("--slots", type=int, default=SLOT_THREADS, help="slot threads; the swap guard decides how many run")
    run.add_argument("--start-slots", type=int, default=STARTING_SLOTS, help="slots allowed at start")
    run.add_argument("--group-only", action="store_true", help="never lease single-function jobs")
    run.add_argument("--luna-single-bytes", type=int, default=0,
                     help="give tier 1 single-function jobs under this size to one Luna medium slot (0: no Luna)")
    extend = commands.add_parser("extend")
    extend.add_argument("--minimum", type=int, default=BAND_BYTES[0])
    extend.add_argument("--maximum", type=int, default=BAND_BYTES[1])
    extend.add_argument("--tier", type=int, default=BAND_TIER)
    trial = commands.add_parser("trial")
    trial.add_argument("--plan", required=True, help="label:jobs:maximum_bytes,... with labels from TRIAL_SETTINGS")
    trial.add_argument("--slots-per-label", type=int, default=2)
    trial.add_argument("--seed", type=int, default=20261002)
    commands.add_parser("trial-report")
    submit = commands.add_parser("submit")
    submit.add_argument("--branch", required=True)
    submit.add_argument("--commit", default="")
    submit.add_argument("--claims", default="", help="comma-separated symbols the branch makes exact")
    submit.add_argument("--nonmatching", default="", help="comma-separated symbols the branch adds as non-matching")
    submit.add_argument("--summary", default="")
    submit.add_argument("--attempts", type=int, default=1)
    submit.add_argument("--minutes", type=float, default=0)
    submit.add_argument("--name", default="")
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
        specs = production_slot_specs(arguments.slots, arguments.luna_single_bytes, arguments.group_only)
        Supervisor(specs, starting_slots=arguments.start_slots).run()
    elif arguments.command == "extend":
        command_extend(arguments)
    elif arguments.command == "trial":
        command_trial(arguments)
    elif arguments.command == "trial-report":
        print(trial_report())
    elif arguments.command == "submit":
        command_submit(arguments)
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
