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
import importlib.util
import hashlib
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
# The closest failed draft of each function and its residual diff, carried into the next tier's packet and Pro packets.
DRAFTS = HOME / "drafts"
# A run stops after this many attempts in a row that do not reduce the differing lines below its best.
STALL_ATTEMPTS = 3
NEAR_MISS_LINES = 4
STALL_RULE = "\n   The runs also end early after {stall} runs in a row that do not reduce the differing lines below your best."
# Attempt output shows the whole diff up to this many lines, then only differing regions with context.
DIFF_LINES_SHOWN = 160
DIFF_CONTEXT_LINES = 3
DIFF_MARKS = "|ris<>"
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
    # A Luna miss on a group under 32 bytes goes to Sol high before Sol xhigh (owner, 2026-10-02).
    4: {"model": "gpt-6.1-sol", "effort": "high", "attempts": 5, "timeout": 10 * 60, "next": 2},
}
LUNA_GROUP_MISS_TIER = 4
BAND_TIER = 3
BAND_BYTES = (256, 512)
# Model settings for single-function trials. A trial never escalates or fails a job; misses go back to the queue.
TRIAL_SETTINGS = {
    "sol-high": {"model": "gpt-6.1-sol", "effort": "high", "attempts": 5, "timeout": 10 * 60},
    "luna-medium": {"model": "gpt-6-luna", "effort": "medium", "attempts": 5, "timeout": 10 * 60},
    "luna-max": {"model": "gpt-6-luna", "effort": "max", "attempts": 5, "timeout": 10 * 60},
    # Hard-end trial (owner, 2026-10-02): no timeout but a 2-hour cap and a 10-minute inactivity kill.
    "astra-high": {"model": "gpt-6-astra", "effort": "high", "attempts": 10, "timeout": 2 * 60 * 60},
    "sol-ultra": {"model": "gpt-6.1-sol", "effort": "ultra", "attempts": 10, "timeout": 2 * 60 * 60},
}
INACTIVITY_KILL_SECONDS = 10 * 60
# Until the weekly usage reset at 13:00 ET on 2026-10-02, a third slot may run hard-end trial jobs first (owner).
HARD_END_THIRD_SLOT_UNTIL = datetime.datetime(2026, 10, 2, 13, 0, tzinfo=datetime.timezone(datetime.timedelta(hours=-4))).timestamp()
SESSION_USAGE_LOG = HOME / "logs" / "session_usage.jsonl"
SIZE_BUCKETS = ((0, 32), (32, 64), (64, 128), (128, 256))
# Worker slots are not capped by brief rule 12 (owner, 2026-10-02): the swap guard runs as many as the Mac holds.
SLOT_THREADS = 10
STARTING_SLOTS = 4
SWAP_PAGES_PER_FIVE_MINUTES = 2048
QUIET_SECONDS_BEFORE_ADDING_A_SLOT = 15 * 60
# Owner, 2026-10-02 13:40: hard cap of 6 slots, and one slot fewer whenever the 5-minute load average is above 15.
MAXIMUM_SLOTS = 6
LOAD_AVERAGE_LIMIT = 15
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
SUBMISSION_PREFIXES = ("root/", "dot/", "integrator/", "cleanup/", "class/")
# Cleanup branches may move code out of the Factory directory; they ride alone and lose no O row (owner, 2026-10-02).
CLEANUP_PREFIX = "cleanup/"
# Facts files (owner, 2026-10-02): offsets, types, signatures, symbol names and addresses only.
FACTS_STAGING = HOME / "facts_staging"
FACTS_DIRECTORY = pathlib.Path("docs/facts")
SOURCE_QUALITY_FILE = HOME / "logs" / "source_quality.json"
ADDRESS_LITERALS_FILE = HOME / "logs" / "address_literals.json"
# The oracle (brief rule 2): a lane's change to these waits for the operator's review.
ORACLE_PATHS = ("tools/check.py", "tools/diff.py", "tools/progress.py", "tools/low/", "tools/asm-differ")
OBJECT_ROOT = pathlib.Path("build/eu/obj")
READELF = "/opt/homebrew/bin/arm-none-eabi-readelf"
# A class-mode header change rechecks the exact functions of every object that includes it; past this many it rides
# the periodic full check instead (about half a second per function).
CLASS_HEADER_RECHECK_LIMIT = 240
FULL_IMAGE_OUTPUT = pathlib.Path("build/eu/full_image_diagnostic")
# Written by the daily audit when tools/check.py demotes a row the full-image compare did not flag.
SPLIT_DISABLED_FILE = HOME / "SPLIT_DISABLED"

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


def load_rows_at(commit):
    text = git(INTEGRATION, "show", f"{commit}:{MAP}")
    return [parse_row(line) for line in text.splitlines()[1:] if line.strip()]


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


def shared_header_path(relative):
    path = pathlib.PurePosixPath(relative)
    return (path.suffix in (".h", ".hpp") and "include" in path.parts
            and path.parts[0] in ("Game", "lib") and ".." not in path.parts)


def changed_shared_headers(worktree, base):
    changed = git(worktree, "diff", "--name-only", base).splitlines()
    changed += git(worktree, "ls-files", "--others", "--exclude-standard", "--", "Game", "lib").splitlines()
    return sorted({path for path in changed if shared_header_path(path) and (worktree / path).is_file()})


def source_type_definitions(text):
    # Lexical declaration gate, not a claim of semantic C++ equivalence. Ignore forward declarations,
    # comments and string literals. Compare names against the actual shared-header inventory.
    text = re.sub(r'//[^\n]*|/\*.*?\*/|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'', " ", text, flags=re.S)
    return collections.Counter(match.group(1).split("::")[-1] for match in re.finditer(
        r"\b(?:class|struct|union)\s+((?:[A-Za-z_]\w*::)*[A-Za-z_]\w*)\s*(?:<[^;{}]*>)?\s*(?:final\s*)?(?::[^;{}]*)?\{", text))


def shared_type_violations(worktree, base, proposed):
    """Reject newly introduced source-local definitions bearing an existing shared type's name.

    Existing accepted local definitions are not retroactively rejected. Candidate headers participate,
    so adding a header and a second source-local copy in one proposal also fails.
    """
    header_paths = git(worktree, "ls-files", "--", "Game", "lib").splitlines()
    header_paths += [path for path in proposed if shared_header_path(path)]
    known = collections.defaultdict(list)
    for relative in sorted(set(header_paths)):
        if not shared_header_path(relative):
            continue
        text = proposed.get(relative)
        if text is None:
            path = worktree / relative
            if not path.is_file():
                continue
            text = path.read_text(errors="replace")
        for name in source_type_definitions(text):
            known[name].append(relative)
    violations = []
    for relative, text in proposed.items():
        if pathlib.PurePosixPath(relative).suffix not in (".cpp", ".cc", ".cxx", ".c"):
            continue
        previous = git(worktree, "show", f"{base}:{relative}", check=False)
        added = source_type_definitions(text) - source_type_definitions(previous)
        for name in sorted(added.keys() & known.keys()):
            violations.append(f"{relative}: new local {name}; use shared declaration in {known[name][0]}")
    return violations


def changed_tracked_files(worktree, base, class_files=()):
    """Files changed since the job's base commit that the job may not change. Factory jobs may change only the map and
    the factory directory and shared headers; class-mode jobs only the map and their class file and header."""
    output = subprocess.run(["git", "-C", str(worktree), "diff", "--name-only", base],
                            capture_output=True, text=True, check=True).stdout
    if class_files:
        return [path for path in output.splitlines() if path and path != str(MAP) and path not in class_files]
    allowed = (str(MAP), str(FACTORY_SOURCE_DIRECTORY) + "/")
    return [path for path in output.splitlines() if path and not (path == allowed[0] or path.startswith(allowed[1]) or (shared_header_path(path) and (worktree / path).is_file()))]


def new_source_files(worktree, base):
    tracked = git(worktree, "diff", "--name-only", base, "--", str(FACTORY_SOURCE_DIRECTORY)).splitlines()
    untracked = git(worktree, "ls-files", "--others", "--exclude-standard", "--", str(FACTORY_SOURCE_DIRECTORY)).splitlines()
    return sorted({p for p in tracked + untracked if p.endswith((".cpp", ".h")) and (worktree / p).exists()})


def commit_attempt(worktree, class_files=()):
    """The checker only credits committed source, so each attempt is a throwaway commit in the worker worktree."""
    git(worktree, "add", "-A", "--", str(FACTORY_SOURCE_DIRECTORY), str(MAP), *class_files, *changed_shared_headers(worktree, "HEAD"))
    git(worktree, "-c", "user.name=factory", "-c", "user.email=factory@localhost",
        "commit", "-q", "--allow-empty", "--no-verify", "-m", "factory attempt")


# ---------------------------------------------------------------- packets

WORKER_GUIDE = """You are a matching-decompilation worker for Super Mario 3D Land (EU Rev 2, 3DS, 32-bit ARM).
The compiler is ARMCC 4.1 build 791 with -O3 -Otime --arm_only --gnu --signed_chars --enum_is_int --force_new_nothrow.
Your whole task is in .factory/job.md: write C++ that compiles to exactly the bytes of the listed functions.

Rules:
1. Write implementations only in the file named in job.md. You may add or extend shared headers under Game/lib include directories when required for this job. Preserve their existing declarations and layouts. Never rename or delete existing files. Never commit.
2. Use plain C++. No inline assembly, no __asm, no raw bytes, no copied machine code, no pragmas that pin code.
3. Define each function with the exact symbol given. Unnamed functions are extern "C" functions named fn_XXXXXXXX.
   For a mangled symbol, include its existing shared declaration; extend the owning shared header when necessary.
4. Search shared headers before defining a type. Reuse and extend the real shared layout, using the pinned
   references in job.md. Never define a TU-local copy of a type already defined in a shared header. New durable
   types belong in shared headers. Reuse shared callee declarations. Keep unknown fn_/dat_ imports neutral.
5. Check your work only with:  . ./development_environment.sh && python {factory} attempt
   It builds, then prints MATCHED or an assembly diff (target on the left) for each function.
   One run takes 30 to 120 seconds. Wait for it to finish; never start a second run while one is going.
   You have {attempts} attempt runs. Stop as soon as everything matches or the runs are used up.{stall_rule}
6. Before you finish, delete every function that still does not match, so the file holds only matching code.
   If nothing matches, delete the file.
7. Read shared headers under Game/lib include directories and the pinned source references in job.md. Other
   source files may be read to locate shared declarations and their callers. Do not edit unrelated implementations.
8. Before you finish, matched or not, complete .factory/facts/<ADDRESS>.md for every function in job.md. Fill in the
   class guess with a confidence and the evidence, the type of each struct offset listed, an inferred C++ signature
   for each callee, and what each data reference is. Only offsets, types, signatures, symbol names and addresses:
   no disassembly, no raw bytes, no copied data.
Finish with one line per function: the address and matched or failed."""


def this_accesses(code, row):
    """(offset, size, read or write) for each load or store through r0, the this pointer on entry."""
    found = set()
    end = row["pool"] or row["end"]
    for address in range(row["start"], end, 4):
        word = struct.unpack_from("<I", code, address - TEXT_BASE)[0]
        if (word >> 28) == 0xF or ((word >> 16) & 0xF) != 0:
            continue
        if (word & 0x0E000000) == 0x04000000 and not word & (1 << 25):     # LDR, STR, LDRB, STRB with an immediate
            found.add((word & 0xFFF, 1 if word & (1 << 22) else 4, "read" if word & (1 << 20) else "write"))
        elif (word & 0x0E4000F0) == 0x004000B0:                            # LDRH, STRH with an immediate
            found.add((((word >> 4) & 0xF0) | (word & 0xF), 2, "read" if word & (1 << 20) else "write"))
    return sorted(found)


def packet_reference_material(worktree, rows_by_start, code):
    """Read curated notes and ground primary-table references in the packet's own source revision."""
    notes_path = worktree / "project/compiler_notes.md"
    notes = notes_path.read_text() if notes_path.exists() else ""
    catalog_path = worktree / "project/actor_catalog.json"
    if not catalog_path.exists():
        return notes, {}
    catalog = json.loads(catalog_path.read_text())
    if catalog.get("input_binary_sha256") != hashlib.sha256(code).hexdigest():
        raise ValueError("Actor catalog does not describe the current executable")
    function_starts = {address for address, row in rows_by_start.items() if "f" in row["type"]}
    references = collections.defaultdict(list)
    for address_text, identity in catalog["packet_lookup_by_primary_address_point"].items():
        address_point = int(address_text, 16)
        if identity.get("lookup_scope") != "exact_installed_primary_address_point_only":
            continue
        # Only the uninterrupted run of mapped function pointers from the installed address point is labeled.
        # A zero, unknown target or table header ends it; a broad containing map row never supplies a class name.
        offset, index = address_point - TEXT_BASE, 0
        while 0 <= offset <= len(code) - 4:
            target = struct.unpack_from("<I", code, offset)[0]
            if target not in function_starts:
                break
            references[target].append({"address_point": address_point, "pointer_index": index,
                                       "registry_name": identity["registry_name"],
                                       "cpp_class_name": identity.get("accepted_cpp_class_name"),
                                       "confidence": identity["confidence"]})
            index += 1
            offset += 4
    return notes, references


def actor_reference_lines(references):
    lines = []
    for reference in references:
        identity = f"registry actor `{reference['registry_name']}`"
        if reference["cpp_class_name"]:
            identity += f", evidenced catalog C++ identity `{reference['cpp_class_name']}`"
        else:
            identity += ", C++ class spelling unresolved"
        lines.append(f"- primary address point 0x{reference['address_point']:08X}, function-pointer index "
                     f"{reference['pointer_index']}: {identity}; {reference['confidence']}")
    return lines


def worker_reference_sections(notes, actor_references, addresses):
    parts = []
    if notes:
        parts += ["", "## Curated compiler reference", "",
                  "The following is the read-only project/compiler_notes.md from this job's source revision.",
                  "Propose additions only in .factory/facts/<ADDRESS>.md, with the observed source and checker result.",
                  "", notes]
    relevant = [address for address in addresses if actor_references.get(address)]
    if relevant:
        parts += ["", "## Verified actor table references", "",
                  "Registry identifiers and evidenced project C++ identities are separate. These references do not",
                  "prove an original class spelling, a unique method owner, or a complete vtable boundary.",
                  "Function-pointer indices below start at the installed primary address point; existing map-row",
                  "indices in facts are separate raw word indices. Full evidence: project/actor_catalog.json."]
        for address in relevant:
            parts += ["", f"Function 0x{address:08X}:", *actor_reference_lines(actor_references[address])]
    return parts


def write_facts_skeleton(path, row, name, readable, rows_by_start, code, vtable_slots, actor_references=None):
    """The mechanical half of a facts file; the worker fills in the class, the types and the signatures."""
    end = row["pool"] or row["end"]
    callees, data = set(), set()
    for address in range(row["start"], end, 4):
        word = struct.unpack_from("<I", code, address - TEXT_BASE)[0]
        if (word & 0x0F000000) == 0x0B000000 and (word >> 28) != 0xF:      # BL
            offset = (word & 0xFFFFFF) << 2
            target = address + 8 + (offset - (1 << 26) if offset & (1 << 25) else offset)
            if target in rows_by_start:
                callees.add(target)
    for address in range(end, row["end"], 4):
        value = struct.unpack_from("<I", code, address - TEXT_BASE)[0]
        target = rows_by_start.get(value)
        if target:
            (callees if "f" in target["type"] else data).add(value)
    lines = [f"# {name}", "", f"Address 0x{row['start']:08X}, {row['end'] - row['start']} bytes.", "",
             "## Class guess", "", "Not yet inferred.", "", "## Struct offsets touched (this is r0 on entry)", ""]
    accesses = this_accesses(code, row)
    lines += [f"- +0x{offset:X}, {size} byte{'s' if size > 1 else ''}, {kind}: type not yet inferred" for offset, size, kind in accesses] or ["- none"]
    lines += ["", "## Callees", ""]
    for target in sorted(callees):
        callee = default_name(rows_by_start[target])
        lines.append(f"- {callee} (0x{target:08X})" + (f": {readable[callee]}" if callee in readable else ": signature not yet inferred"))
    if not callees:
        lines.append("- none")
    lines += ["", "## Vtable slots", ""]
    lines += [f"- candidate table row 0x{vtable:08X}, raw word index {slot}"
              for vtable, slot in vtable_slots.get(row["start"], [])] or ["- none found"]
    verified = (actor_references or {}).get(row["start"], [])
    if verified:
        lines += ["", "Verified primary-table references (shared targets do not prove method ownership):",
                  *actor_reference_lines(verified)]
    lines += ["", "## Data references", ""]
    lines += [f"- {default_name(rows_by_start[target])} (0x{target:08X}): not yet identified" for target in sorted(data)] or ["- none"]
    path.write_text("\n".join(lines) + "\n")


def build_vtable_slots(rows, code):
    """Function address -> [(vtable address, slot)] for every data row that is mostly function pointers."""
    starts = {r["start"] for r in rows if "f" in r["type"]}
    slots = collections.defaultdict(list)
    for row in rows:
        if "f" in row["type"] or row["end"] - row["start"] < 8 or row["end"] - TEXT_BASE > len(code):
            continue
        words = struct.unpack_from("<%dI" % ((row["end"] - row["start"]) // 4), code, row["start"] - TEXT_BASE)
        hits = [i for i, w in enumerate(words) if w in starts]
        if len(hits) >= 2 and len(hits) * 2 >= len(words):
            for index in hits:
                slots[words[index]].append((row["start"], index))
    return slots


def build_packet(job, worktree, rows_by_start, symbols, code, readable, settings, vtable_slots=None):
    addresses = [int(a, 16) for a in job["addresses"].split(",")]
    tier = settings
    compiler_notes, actor_references = packet_reference_material(worktree, rows_by_start, code)
    reference_sections = worker_reference_sections(compiler_notes, actor_references, addresses)
    first = rows_by_start[addresses[0]]
    file_name = FACTORY_SOURCE_DIRECTORY / (("fn_%08X.cpp" if len(addresses) == 1 else "group_%08X.cpp") % addresses[0])
    names = []
    for address in addresses:
        row = rows_by_start[address]
        name = default_name(row)
        names.append(name)
    listing, _ = disassemble(code, first, symbols)
    if job["kind"] == "class":
        info = json.loads(job["note"].split("|")[0])
        address, name = addresses[0], names[0]
        row = rows_by_start[address]
        listing, _ = disassemble(code, row, symbols)
        parts = [f"# Job {job['id']}: class mode, {info['class']}", "",
                 f"Write `{name}` at 0x{address:08X} ({row['end'] - row['start']} bytes) as a member function in `{info['file']}`."
                 f" The class declaration is in `{info['header']}`. Edit only those two files; nothing goes to the Factory directory.",
                 "", "It belongs to the translation unit of " + info["class"] + ": a method of that class, or of a nerve or helper class",
                 "that file defines. Give it a descriptive name grounded in what it does and in the facts file; declare it in the header",
                 "if it is a member there. Use the existing fields and types; add named fields to the struct when you need them.",
                 "No raw offset casts, no duplicated extern declarations, no inline assembly.", "",
                 "After a build, read the member's mangled symbol from `arm-none-eabi-nm` on the class file's object under build/eu/obj/,",
                 f'and write .factory/symbols.json as {{"0x{address:08X}": "<mangled symbol>"}}. The attempt command checks that symbol.', "",
                 "```", listing, "```", ""]
        if (job.get("note") or "").count("|pro:"):
            answer = pathlib.Path(job["note"].split("|pro:")[1])
            if answer.exists():
                parts += ["GPT-6 Pro, which cannot compile, proposed the forms below after earlier workers failed.", "", answer.read_text()]
        parts += [f"Check with `. ./development_environment.sh && python {HOME / 'factory.py'} attempt` ({settings['attempts']} runs)."]
        factory_directory = worktree / ".factory"
        (factory_directory / "facts").mkdir(parents=True, exist_ok=True)
        write_facts_skeleton(factory_directory / "facts" / f"{address:08X}.md", row, name, readable, rows_by_start, code, vtable_slots or {}, actor_references)
        (factory_directory / "job.md").write_text("\n".join(parts + reference_sections) + "\n")
        (factory_directory / "job.json").write_text(json.dumps({
            "id": job["id"], "addresses": addresses, "symbols": names, "file": info["file"],
            "class_files": [info["header"], info["file"]], "attempt_limit": settings["attempts"],
            "base": git(worktree, "rev-parse", "HEAD")}))
        return addresses, names
    if job["kind"] == "facts":
        sources = sorted({p.relative_to(worktree).as_posix() for p in (worktree / FACTORY_SOURCE_DIRECTORY).glob("*.cpp")
                          if any(f"{n}(" in p.read_text(errors="ignore") for n in names)})
        parts = [f"# Job {job['id']}: facts only", "",
                 "These functions already match byte for byte; their source is in " + ", ".join(f"`{s}`" for s in sources) + ".",
                 "Do not write or edit any code and do not run attempt. Read the source and the listings below, then complete",
                 "the facts file for each function in .factory/facts/ (rule 8).", ""]
        for address, name in zip(addresses, names):
            listing, _ = disassemble(code, rows_by_start[address], symbols)
            parts += [f"`{name}` at 0x{address:08X}:", "", "```", listing, "```", ""]
        factory_directory = worktree / ".factory"
        (factory_directory / "facts").mkdir(parents=True, exist_ok=True)
        for address, name in zip(addresses, names):
            write_facts_skeleton(factory_directory / "facts" / f"{address:08X}.md", rows_by_start[address], name, readable,
                                 rows_by_start, code, vtable_slots or {}, actor_references)
        (factory_directory / "job.md").write_text("\n".join(parts + reference_sections) + "\n")
        (factory_directory / "job.json").write_text(json.dumps({
            "id": job["id"], "addresses": addresses, "symbols": names, "file": "", "attempt_limit": 0,
            "base": git(worktree, "rev-parse", "HEAD")}))
        return addresses, names
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
    if job["kind"] != "hardtrial":  # a hard-end trial run starts clean, with no draft from another model's run
        parts += format_drafts(addresses)  # last, so a Pro packet can swap in the newest drafts
    factory_directory = worktree / ".factory"
    factory_directory.mkdir(exist_ok=True)
    facts = factory_directory / "facts"
    facts.mkdir(exist_ok=True)
    for address, name in zip(addresses, names):
        write_facts_skeleton(facts / f"{address:08X}.md", rows_by_start[address], name, readable, rows_by_start, code,
                             vtable_slots or {}, actor_references)
    (factory_directory / "job.md").write_text("\n".join(parts + reference_sections) + "\n")
    (factory_directory / "job.json").write_text(json.dumps({
        "id": job["id"], "addresses": addresses, "symbols": names, "file": str(file_name),
        "attempt_limit": tier["attempts"], "base": git(worktree, "rev-parse", "HEAD"),
        "stall_stop": job["kind"] != "hardtrial"}))  # hard-end trial runs use all their attempts (owner, 2026-10-02)
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
    changed = changed_tracked_files(worktree, job["base"], job.get("class_files", ()))
    if changed:
        print("You changed files outside this job and its allowed shared headers. Restore these paths:", *changed, sep="\n  ")
        sys.exit(2)
    results = attempt_in(worktree, job, commit=True)
    matched = [a for a, r in results.items() if r["rank"] == "O"]
    for address, result in results.items():
        print(f"\n=== 0x{address:08X} {result['symbol']}: {'MATCHED' if result['rank'] == 'O' else 'rank ' + result['rank']}")
        if result["detail"]:
            print(result["detail"])
    print(f"\nSUMMARY: {len(matched)}/{len(results)} matched. Attempt {used + (0 if arguments.final else 1)}"
          f" of {job['attempt_limit']}.")
    if not arguments.final and len(matched) < len(results):
        record_best_drafts(worktree, job, results, used + 1)
        if job.get("stall_stop", True) and attempt_stalled(worktree, results) and used + 1 < job["attempt_limit"]:
            counter.write_text(str(job["attempt_limit"]))
            print(f"NO PROGRESS: {STALL_ATTEMPTS} runs in a row did not reduce the differing lines below your best."
                  " That was your last run. Delete non-matching functions and finish now.")
    if arguments.final:
        print("FINAL " + json.dumps(matched))


def attempt_in(worktree, job, final=False, commit=False):
    source_paths = new_source_files(worktree, job["base"]) + list(job.get("class_files", ())) + changed_shared_headers(worktree, job["base"])
    proposed = {path: (worktree / path).read_text(errors="replace") for path in source_paths if (worktree / path).is_file()}
    violations = shared_type_violations(worktree, job["base"], proposed)
    if violations:
        return {a: {"symbol": s, "rank": "build-failed", "detail": "SHARED TYPE POLICY:\n" + "\n".join(violations),
                    "diff": "", "score": None} for a, s in zip(job["addresses"], job["symbols"])}
    symbols = list(job["symbols"])
    chosen = worktree / ".factory" / "symbols.json"
    if job.get("class_files") and chosen.exists():
        # Class mode: the worker names the member function; the check looks for the symbol it reports.
        try:
            names = {int(k, 16): v for k, v in json.loads(chosen.read_text()).items()}
            symbols = [names.get(a, s) for a, s in zip(job["addresses"], symbols)]
        except (ValueError, AttributeError):
            pass
    job = dict(job, symbols=symbols)
    set_map_rows(worktree, {a: ("M", s) for a, s in zip(job["addresses"], job["symbols"])})
    if commit:
        commit_attempt(worktree, job.get("class_files", ()))
    status, output = tool(worktree, "make.py", "eu")
    if status != 0:
        tail = "\n".join(output.splitlines()[-60:])
        return {a: {"symbol": s, "rank": "build-failed", "detail": "BUILD FAILED:\n" + tail if not final else "",
                    "diff": "", "score": None}
                for a, s in zip(job["addresses"], job["symbols"])}
    results = {}
    for address, symbol in zip(job["addresses"], job["symbols"]):
        tool(worktree, "tools/check.py", symbol, timeout=300)
        lines, index = read_map_lines(worktree / MAP)
        rank = parse_row(lines[index[address]])["rank"]
        detail, diff, score = "", "", 0
        if rank != "O" and not final:
            _, diff = tool(worktree, "tools/diff.py", symbol, "-c", timeout=300)
            detail = readable_diff(diff)
            score = differing_lines(diff)
        results[address] = {"symbol": symbol, "rank": rank, "detail": detail, "diff": diff, "score": score}
    return results


def diff_marker(line):
    """The diff's mark for one listing line (changed, register, immediate, stack, or one side only), or None if it matches.
    The mark sits in column 50 unless a long target operand pushed it right."""
    if len(line) <= 50 or not re.match(r"^\s*([0-9a-f]+:|\s{40,}[<>])", line):
        return None
    if line[50] in DIFF_MARKS:
        return line[50]
    if line[49] != " ":
        found = re.compile(r"([|ris<>])(?= |$)").search(line, 50)
        return found.group(1) if found else None
    return None


def differing_lines(diff):
    """Lines the diff marks as different."""
    count = sum(1 for line in diff.splitlines() if diff_marker(line))
    return count if count else None  # no parsable diff counts as no progress


def readable_diff(diff, limit=DIFF_LINES_SHOWN, context=DIFF_CONTEXT_LINES):
    """The whole diff when it fits; otherwise the header and every differing line with a few lines of context,
    so the end of a large function is never cut off."""
    lines = diff.splitlines()
    if len(lines) <= limit:
        return "\n".join(lines)
    listing = [i for i, line in enumerate(lines) if re.match(r"^\s*([0-9a-f]+:|\s{40,}[<>])", line)]
    if not listing:
        return "\n".join(lines[:limit])
    first, last = listing[0], listing[-1]
    differing = [i for i in listing if diff_marker(lines[i])]
    keep = set()
    for i in differing:
        keep.update(range(max(first, i - context), min(last, i + context) + 1))
    shown, previous = lines[:first], first - 1
    for i in sorted(keep):
        if i > previous + 1:
            shown.append(f"    ... {i - previous - 1} matching lines ...")
        shown.append(lines[i])
        previous = i
    if previous < last:
        shown.append(f"    ... {last - previous} matching lines ...")
    shown += lines[last + 1:]
    if len(shown) > limit:
        hidden = sum(1 for line in shown[limit:] if diff_marker(line))
        shown = shown[:limit] + [f"    ... {hidden} more differing lines not shown; fix the ones above first ..."]
    return "\n".join(shown)


def record_best_drafts(worktree, job, results, attempt):
    """Keep each function's closest draft so far, with its full residual diff, in .factory/best."""
    best = worktree / ".factory" / "best"
    best.mkdir(exist_ok=True)
    sources = {path: (worktree / path).read_text(errors="ignore") for path in new_source_files(worktree, job["base"]) + changed_shared_headers(worktree, job["base"])}
    for address, result in results.items():
        if result["rank"] == "O" or not result["score"] or not sources:
            continue
        path = best / f"{address:08X}.json"
        if path.exists() and json.loads(path.read_text())["score"] <= result["score"]:
            continue
        path.write_text(json.dumps({"address": address, "symbol": result["symbol"], "score": result["score"],
                                    "attempt": attempt, "sources": sources, "diff": ANSI.sub("", result["diff"])}))


def attempt_stalled(worktree, results):
    """Record this attempt as (unmatched functions, differing lines); true once STALL_ATTEMPTS attempts in a row fail
    to beat the best before them. A near miss, NEAR_MISS_LINES or fewer differing lines, keeps every run."""
    history_path = worktree / ".factory" / "history.json"
    history = [tuple(h) for h in json.loads(history_path.read_text())] if history_path.exists() else []
    scores = [r["score"] for r in results.values() if r["rank"] != "O"]
    # A failed build or an unparsable diff is no progress.
    history.append((len(scores), 10 ** 9 if any(s is None for s in scores) else sum(scores)))
    history_path.write_text(json.dumps(history))
    best = min(history)
    if best[1] <= NEAR_MISS_LINES:
        return False
    return len(history) > STALL_ATTEMPTS and min(history[-STALL_ATTEMPTS:]) >= min(history[:-STALL_ATTEMPTS])


def load_draft(address):
    path = DRAFTS / f"{address:08X}.json"
    return json.loads(path.read_text()) if path.exists() else None


def format_drafts(addresses):
    """Packet text for the closest earlier drafts of these functions, one block per distinct source."""
    parts, shown = [], set()
    for address in addresses:
        draft = load_draft(address)
        if not draft:
            continue
        parts += ["", f"An earlier worker's closest draft of 0x{address:08X} ({draft['tier_label']}, attempt {draft['attempt']})"
                  f" still had {draft['score']} differing lines. Start from it rather than from scratch.", ""]
        for path, text in draft["sources"].items():
            if text in shown:
                parts.append(f"(Source `{path}` is the same as the draft above.)")
                continue
            shown.add(text)
            parts += [f"Draft source `{path}`:", "", "```cpp", text.rstrip(), "```"]
        parts += ["", "Its residual diff (target on the left):", "", "```", readable_diff(draft["diff"], limit=250).rstrip(), "```"]
    return parts


def source_objects(worktree):
    """Compiled objects of the source tree, keyed by source path without its suffix. The generated stubs under
    build/ are weak placeholders and are skipped."""
    root = worktree / OBJECT_ROOT
    return {str(p.relative_to(root).with_suffix("")): p for p in root.rglob("*.o") if p.relative_to(root).parts[0] != "build"}


def duplicate_definitions(worktree):
    """Strong global symbols that more than one object defines, as {symbol: sorted source stems}. The archive link
    keeps one of them silently (owner, 2026-10-02). COMDAT group members, such as template vtables and out-of-line
    inline functions, are merged by the linker by design and do not count."""
    stems = {str(path): stem for stem, path in source_objects(worktree).items()}
    if not stems:
        return {}
    listing = subprocess.run([READELF, "-gsW", *stems], capture_output=True, text=True).stdout
    owners = collections.defaultdict(set)
    parts = re.split(r"^File: (.+)$", listing, flags=re.M)
    for name, body in zip(parts[1::2], parts[2::2]):
        grouped, in_group = set(), False
        for line in body.splitlines():
            if line.startswith("COMDAT group section"):
                in_group = True
                continue
            if in_group:
                member = re.match(r"\s+\[\s*(\d+)\]\s+\S", line)
                if member:
                    grouped.add(member[1])
                    continue
                if "[Index]" in line:
                    continue
                in_group = False
            fields = line.split()
            if (len(fields) >= 8 and fields[0].endswith(":") and fields[3] in ("FUNC", "OBJECT") and fields[4] == "GLOBAL"
                    and fields[6] not in ("UND", "ABS", "COM") and fields[6] not in grouped):
                owners[fields[7]].add(stems.get(name.strip(), name.strip()))
    return {symbol: sorted(files) for symbol, files in owners.items() if len(files) > 1}


def dependent_stems(worktree, headers):
    """Source stems whose object depends on any of the headers, read from the compiler's dependency files."""
    root = worktree / OBJECT_ROOT
    wanted = {os.path.realpath(worktree / h) for h in headers}
    found = set()
    for depend in root.rglob("*.d"):
        relative = depend.relative_to(root)
        if relative.parts[0] == "build":
            continue
        for line in depend.read_text(errors="ignore").splitlines():
            fields = re.findall(r'"([^"]*)"', line)
            if len(fields) >= 2 and os.path.realpath(fields[1]) in wanted:
                found.add(str(relative.with_suffix("")))
                break
    return found


def change_stems(worktree, files):
    """Objects a change compiles into: its own source files, and every object that includes one of its headers."""
    stems = {str(pathlib.Path(f).with_suffix("")) for f in files if f.endswith((".cpp", ".cc", ".c"))}
    headers = [f for f in files if f.endswith((".h", ".hpp", ".inc"))]
    return stems | (dependent_stems(worktree, headers) if headers else set())


def offending_duplicates(duplicates, stems):
    return {symbol: files for symbol, files in duplicates.items() if set(files) & stems}


def check_rank(worktree, address, symbol):
    """Run tools/check.py on one row and return the rank it leaves in the worktree's map."""
    tool(worktree, "tools/check.py", symbol, timeout=300)
    lines, index = read_map_lines(worktree / MAP)
    return parse_row(lines[index[address]])["rank"]


def full_image_compare(worktree):
    """The full-image byte compare (make.py eu --split): every O row's bytes, linked at original addresses, against
    the original image. Returns the O rows whose bytes differ as {address: symbol}, or None when the compare did not
    reach every O row in the worktree's map (it stops before comparing when, for example, a row's size changed)."""
    root = worktree / FULL_IMAGE_OUTPUT
    before = set(root.iterdir()) if root.exists() else set()
    tool(worktree, "make.py", "eu", "--split", timeout=1800)
    created = sorted(set(root.iterdir()) - before) if root.exists() else []
    try:
        comparison = created[-1] / "comparison.json" if len(created) == 1 else None
        if comparison is None or not comparison.exists():
            return None
        compared = {int(x["address"], 16): x for x in json.loads(comparison.read_text())["source_O_intervals"]}
        exact = {r["start"] for r in load_rows(worktree / MAP) if r["rank"] == "O" and "i" not in r["type"]}
        if exact - set(compared):
            return None
        return {address: x["symbol"] for address, x in compared.items() if not x["equal"]}
    finally:
        for directory in created:
            shutil.rmtree(directory, ignore_errors=True)


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
    """Rows (by start address) whose rank differs between two map texts. A row added or removed at rank U (a rule 2
    boundary change in an evidence commit) sets no rank, so it does not count."""
    def ranks(text):
        return {line.split(",")[0]: line.split(",")[4].strip() for line in text.splitlines()[1:] if line.strip()}
    before, after = ranks(before_text), ranks(after_text)
    return sorted(k for k in set(before) | set(after)
                  if before.get(k) != after.get(k) and (before.get(k) or "U") != (after.get(k) or "U"))


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
            last, percents = None, []
            for line in open(session, errors="ignore"):
                if '"token_count"' in line:
                    try:
                        event = json.loads(line)
                        payload = event.get("payload", {})
                        last = (payload.get("info") or {}).get("total_token_usage") or last
                        primary = (payload.get("rate_limits") or {}).get("primary") or {}
                        if primary.get("used_percent") is not None:
                            percents.append((event.get("timestamp"), primary["used_percent"], primary.get("window_minutes")))
                    except ValueError:
                        pass
            if percents:
                with open(SESSION_USAGE_LOG, "a") as stream:
                    stream.write(json.dumps({"log": str(log_path), "thread": thread, "first": percents[0], "last": percents[-1]}) + "\n")
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
        # A restart soon after a full check waits out the rest of its interval instead of checking again.
        last = self.database.execute("SELECT MAX(time) FROM events WHERE kind='sync'").fetchone()[0]
        self.last_sync = last if last and time.time() - last < SYNC_INTERVAL_SECONDS else 0.0
        audited = self.database.execute("SELECT MAX(time) FROM events WHERE kind='audit'").fetchone()[0]
        self.last_audit_day = datetime.datetime.fromtimestamp(audited, EASTERN).date() if audited else None
        self.code = read_code()
        self.reload_symbols()
        self.vtable_slots = build_vtable_slots(list(self.rows_by_start.values()), self.code)
        self.expected_target = target_commit()
        self.last_pushed = remote_target()
        self.batch_failures = {}
        self.class_proposal = None
        self.class_pending = None  # a class-mode submission waiting for the periodic full check
        self.last_push_time = None
        self.consecutive_misses = 0
        self.backoff = {}
        self.lane_offsets = {}
        self.maximum_slots = min(len(slot_specs), MAXIMUM_SLOTS)
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
        if spec.get("hard_end_first") or time.time() < spec.get("hard_end_until", 0):
            with DATABASE_LOCK:
                job = self.database.execute("SELECT * FROM jobs WHERE status='open' AND kind='hardtrial' ORDER BY id LIMIT 1").fetchone()
                if job:
                    self.database.execute("UPDATE jobs SET status='leased', leased_by=?, leased_at=?, attempts=attempts+1 WHERE id=?",
                                          (slot, time.time(), job["id"]))
                    self.database.commit()
                    return dict(job)
        if spec.get("class_mode") and ((self.class_proposal and self.class_proposal.exists())
                                       or (self.class_pending and self.class_pending.exists())):
            # One class job at a time: while one waits to land, this slot does regular Sol work.
            spec = {"tiers": [1, LUNA_GROUP_MISS_TIER, 2], "ranges": [(32, 256)], "smallest_first": True}
        elif spec.get("class_mode"):
            with DATABASE_LOCK:
                job = self.database.execute("SELECT * FROM jobs WHERE status='open' AND kind='class' ORDER BY priority DESC, id LIMIT 1").fetchone()
                if job:
                    self.database.execute("UPDATE jobs SET status='leased', leased_by=?, leased_at=?, attempts=attempts+1 WHERE id=?",
                                          (slot, time.time(), job["id"]))
                    self.database.commit()
                    return dict(job)
            return None
        live_rows = {}
        try:
            for row in load_rows(REPOSITORY / MAP):
                live_rows[row["start"]] = row["rank"]
        except Exception:
            pass
        with DATABASE_LOCK:
            # Tier 1 Sol work starts at 32 bytes and increases by body size, independent of job priority.
            # Escalated work (tier 2 and up) retains its priority order at any size.
            plan = []
            for tier in spec["tiers"]:
                if tier == 1 and spec.get("ranges"):
                    plan += [(tier, low, high) for low, high in spec["ranges"]]
                else:
                    plan.append((tier, None, None))
            for tier, low, high in plan:
                query, parameters = "SELECT * FROM jobs WHERE status='open' AND tier=? AND kind IN ('group', 'single')", [tier]
                if spec.get("kinds"):
                    query += " AND kind IN (%s)" % ",".join("?" * len(spec["kinds"]))
                    parameters += list(spec["kinds"])
                if low is not None:
                    query += " AND body_bytes >= ? AND body_bytes < ?"
                    parameters += [low, high]
                order = " ORDER BY body_bytes ASC, priority DESC, id" if spec.get("smallest_first") and low is not None else " ORDER BY priority DESC"
                candidates = self.database.execute(query + order + " LIMIT 50", parameters).fetchall()
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
            for kind in spec.get("fallback_kinds", []):
                job = self.database.execute("SELECT * FROM jobs WHERE status='open' AND kind=? ORDER BY priority DESC, id LIMIT 1",
                                            (kind,)).fetchone()
                if job:
                    self.database.execute("UPDATE jobs SET status='leased', leased_by=?, leased_at=?, attempts=attempts+1 WHERE id=?",
                                          (slot, time.time(), job["id"]))
                    self.database.commit()
                    return dict(job)
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
        class_mode = job["kind"] == "class"
        hard_end = json.loads(job["note"]) if job["kind"] == "hardtrial" else None
        if hard_end:
            settings = TRIAL_SETTINGS[hard_end["label"]]
        elif job["kind"] in ("single", "group") and spec.get("settings") and job["tier"] != 1:
            settings = TIERS[job["tier"]]  # an escalated job never runs on the slot's own model
        else:
            settings = spec.get("settings") or (TRIAL_SETTINGS["luna-medium"] if class_mode and job["tier"] == 0 else TIERS[job["tier"]])
        trial = spec.get("trial")
        head = target_commit()
        reset_worktree(worktree, head)
        addresses, names = build_packet(job, worktree, self.rows_by_start, self.symbols, self.code, self.readable, settings,
                                        self.vtable_slots)
        set_map_rows(worktree, {a: ("M", s) for a, s in zip(addresses, names)})
        started = time.time()
        self.active[slot] = {"job": job["id"], "tier": job["tier"], "count": len(addresses), "first": addresses[0],
                             "bytes": job["body_bytes"], "started": started, "model": settings["model"],
                             "effort": settings["effort"], "trial": trial}
        run_id = execute(self.database,
                         "INSERT INTO runs (job_id, slot, tier, model, effort, started, outcome, note) VALUES (?,?,?,?,?,?,'running',?)",
                         (job["id"], slot, 0 if trial else job["tier"], settings["model"], settings["effort"], started,
                          f"trial {trial}" if trial else ("class" if class_mode else
                                                          (f"hardtrial {hard_end['label']} {hard_end['group']}" if hard_end else "")))).lastrowid
        prompt = WORKER_GUIDE.format(factory=HOME / "factory.py", attempts=settings["attempts"],
                                     stall_rule="" if hard_end else STALL_RULE.format(stall=STALL_ATTEMPTS)) + \
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
            if hard_end:
                # No timeout: a 2-hour cap, and a kill after 10 minutes without any new output.
                outcome, last_size, last_change = "finished", 0, time.time()
                while process.poll() is None:
                    time.sleep(30)
                    size = log_path.stat().st_size
                    if size != last_size:
                        last_size, last_change = size, time.time()
                    if time.time() - last_change > INACTIVITY_KILL_SECONDS or time.time() - started > settings["timeout"]:
                        outcome = "inactive" if time.time() - last_change > INACTIVITY_KILL_SECONDS else "timeout"
                        os.killpg(process.pid, signal.SIGTERM)
                        process.wait()
                        break
            else:
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
        class_files = job_data.get("class_files", [])
        bad_edits = changed_tracked_files(worktree, job_data["base"], class_files)
        if not class_files and changed_shared_headers(worktree, job_data["base"]):
            class_files = changed_shared_headers(worktree, job_data["base"]) + new_source_files(worktree, job_data["base"])
            job_data["class_files"] = class_files
        files = ([f for f in class_files if git(worktree, "diff", "--name-only", job_data["base"], "--", f)
                  or git(worktree, "ls-files", "--others", "--exclude-standard", "--", f)] if class_files
                 else new_source_files(worktree, job_data["base"]))
        matched = []
        self.stage_facts(worktree, job, settings, outcome)
        if job["kind"] == "facts":
            execute(self.database, "UPDATE runs SET finished=?, outcome=?, input_tokens=?, cached_tokens=?, output_tokens=?, note=? WHERE id=?",
                    (time.time(), outcome, usage["input_tokens"], usage["cached_input_tokens"], usage["output_tokens"], "facts", run_id))
            execute(self.database, "UPDATE jobs SET status=?, finished_at=?, leased_by=NULL WHERE id=?",
                    ("open" if provider_errors else "done", time.time(), job["id"]))
            log_event(self.database, "run", f"{slot} facts job {job['id']} {settings['model']} {settings['effort']}: {len(addresses)} functions, {outcome}")
            return
        results = {}
        if not bad_edits and files:
            results = attempt_in(worktree, job_data, final=True, commit=True)
            matched = [a for a, r in results.items() if r["rank"] == "O"]
        matched_bytes = sum(self.rows_by_start[a]["end"] - self.rows_by_start[a]["start"] for a in matched)
        note = f"edited existing files: {bad_edits}" if bad_edits else (f"trial {trial}" if trial else ("class" if class_mode else
                                                                            (f"hardtrial {hard_end['label']} {hard_end['group']}" if hard_end else "")))
        if provider_errors and not trial:
            note = (note + "; " if note else "") + "provider error: " + provider_errors[-1][:160]
        execute(self.database,
                "UPDATE runs SET finished=?, outcome=?, input_tokens=?, cached_tokens=?, output_tokens=?,"
                " matched_count=?, matched_bytes=?, note=? WHERE id=?",
                (time.time(), outcome, usage["input_tokens"], usage["cached_input_tokens"], usage["output_tokens"],
                 len(matched), matched_bytes, note, run_id))
        remaining = [a for a in addresses if a not in matched]
        self.keep_drafts(worktree, settings, run_id, matched, remaining)
        remaining_text = ",".join("%08X" % a for a in remaining)
        proposal_job = job["id"]
        retry = bool(provider_errors) and not policy_flags
        if hard_end:
            # No escalation: the trial job is done either way; a step 1 run that comes back clean opens step 2.
            execute(self.database, "UPDATE jobs SET status='done', finished_at=?, leased_by=NULL WHERE id=?", (time.time(), job["id"]))
            if hard_end["step"] == 1:
                clean = not policy_flags and not provider_errors and outcome == "finished"
                execute(self.database, f"UPDATE jobs SET status='{'open' if clean else 'blocked'}' WHERE kind='hardtrial' AND status='waiting'")
                log_event(self.database, "alert" if not clean else "trial",
                          f"hard-end step 1 on {settings['model']} {settings['effort']}: outcome {outcome},"
                          f" provider errors {provider_errors[-1:] or 'none'}, policy flags {len(policy_flags)}; step 2 {'opened' if clean else 'held'}")
            retry = False
            remaining = []
        next_tier = {0: 2, 1: 2}.get(job["tier"]) if class_mode else TIERS.get(job["tier"], {}).get("next")
        if (not class_mode and job["tier"] == 1 and settings["model"] == "gpt-6-luna"
                and job["kind"] == "group" and job["body_bytes"] < 32):
            next_tier = LUNA_GROUP_MISS_TIER
        if hard_end:
            pass
        elif not remaining:
            execute(self.database, "UPDATE jobs SET status='proposed', finished_at=? WHERE id=?", (time.time(), job["id"]))
        elif trial or retry:
            execute(self.database, "UPDATE jobs SET status='open', addresses=?, leased_by=NULL WHERE id=?",
                    (remaining_text, job["id"]))
        elif "pro:" in (job["note"] or ""):
            execute(self.database, "UPDATE jobs SET status='failed', addresses=?, finished_at=?, note=? WHERE id=?",
                    (remaining_text, time.time(), (job["note"].split("|")[0] + "|pro tried") if class_mode else "pro tried", job["id"]))
        elif next_tier:
            execute(self.database, "UPDATE jobs SET status='open', tier=?, addresses=?, leased_by=NULL WHERE id=?",
                    (next_tier, remaining_text, job["id"]))
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
            chosen = {a: (results.get(a) or {}).get("symbol") or n for a, n in zip(addresses, names)}
            (proposal / "matched.json").write_text(json.dumps(
                {"job": proposal_job, "run": run_id, "tier": job["tier"], "model": settings["model"],
                 "attempts": attempts_used, "minutes": (time.time() - started) / 60,
                 "addresses": matched, "symbols": chosen, "class_files": class_files, "base": job_data["base"]}))
            if class_mode:
                self.class_proposal = proposal
            self.integration_queue.put(proposal)
        self.backoff[slot] = (PROVIDER_RETRY_SECONDS[min(len(PROVIDER_RETRY_SECONDS) - 1, PROVIDER_RETRY_SECONDS.index(self.backoff[slot]) + 1)]
                              if self.backoff.get(slot) else PROVIDER_RETRY_SECONDS[0]) if retry else 0
        if not trial and not retry and not hard_end:
            self.consecutive_misses = 0 if matched else self.consecutive_misses + 1
            if self.consecutive_misses >= NO_MATCH_HALT_RUNS and not self.halted:
                self.halted = f"{NO_MATCH_HALT_RUNS} jobs in a row matched nothing"
                log_event(self.database, "alert", "factory halted: " + self.halted)
        log_event(self.database, "run", f"{slot} job {job['id']} {settings['model']} {settings['effort']}"
                  f"{' trial ' + trial if trial else ''}: {len(matched)}/{len(addresses)} matched,"
                  f" {usage['input_tokens'] - usage['cached_input_tokens']} uncached + {usage['output_tokens']} out, {outcome}"
                  + (f"; retrying at the same tier in {self.backoff[slot]} s: {provider_errors[-1][:120]}" if retry else ""))

    def keep_drafts(self, worktree, settings, run_id, matched, remaining):
        """Move this run's closest draft of each unmatched function into DRAFTS when it beats the one kept there."""
        DRAFTS.mkdir(exist_ok=True)
        for address in matched:
            (DRAFTS / f"{address:08X}.json").unlink(missing_ok=True)
        for address in remaining:
            path = worktree / ".factory" / "best" / f"{address:08X}.json"
            if not path.exists():
                continue
            draft = json.loads(path.read_text())
            kept = load_draft(address)
            if kept and kept["score"] <= draft["score"]:
                continue
            draft.update(run=run_id, tier_label=f"{settings['model']} {settings['effort']}")
            (DRAFTS / path.name).write_text(json.dumps(draft))

    def stage_facts(self, worktree, job, settings, outcome):
        """Keep each facts file, minus anything that looks like a listing or raw bytes, for the next facts commit."""
        FACTS_STAGING.mkdir(exist_ok=True)
        listing = re.compile(r"^\s*[0-9A-Fa-f]{8}\s+[a-z]{1,6}[a-z.]*\s|\.word\b|\b(?:[0-9A-Fa-f]{2}\s){8,}")
        for path in (worktree / ".factory" / "facts").glob("*.md"):
            kept = [line for line in path.read_text(errors="ignore").splitlines() if not listing.search(line)]
            kept.insert(1, f"\nWritten by factory job {job['id']} ({settings['model']} {settings['effort']}, run {outcome}).")
            (FACTS_STAGING / path.name).write_text("\n".join(kept) + "\n")

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
        # The packet already holds drafts from earlier tiers; give Pro the closest one after this run as well.
        packet = packet.split("\nAn earlier worker's closest draft", 1)[0].rstrip() + "\n"
        drafts = format_drafts(remaining)
        (directory / f"{total:05d}-{remaining[0]:08X}.md").write_text(header + packet + "\n".join(drafts) + "\n")

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
                note = (job["note"].split("|")[0] + f"|pro:{kept}") if job["kind"] == "class" else f"pro:{kept}"
                execute(self.database, "UPDATE jobs SET status='open', tier=?, priority=9000000, note=? WHERE id=?",
                        (tier, note, job["id"]))
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
                self.land_facts()
                continue
            class_proposals = [p for p in batch if (p / "matched.json").exists()
                               and json.loads((p / "matched.json").read_text()).get("class_files")]
            batch = [p for p in batch if p not in class_proposals]
            for proposal in class_proposals:
                try:
                    if self.integrate_class(proposal) == "ride":
                        self.submit_class_proposal(proposal)
                except Exception as error:
                    log_event(self.database, "error", f"class integration of {proposal.name} failed: {error}")
                    self.restore_integration()
                    self.retire(proposal, "rejected")
            try:
                if batch:
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
            # Quiet submissions and facts need no build, so a steady stream of proposals must not starve them.
            self.land_quiet_submissions()
            self.land_facts()

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
                violations = shared_type_violations(INTEGRATION, base,
                    {f: (proposal / f).read_text(errors="replace") for f in files})
                if violations:
                    log_event(self.database, "reject", f"{proposal.name}: shared type policy: {violations}")
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
            job = {"base": base, "addresses": [a for e in entries for a in e["addresses"]],
                   "symbols": [e["symbols"][a] for e in entries for a in e["addresses"]]}
            results = attempt_in(INTEGRATION, job, final=True)
            # A proposal may not define a symbol another object already defines (owner, 2026-10-02). When only
            # proposals in this batch define it, the first one keeps it.
            built = all(r["rank"] != "build-failed" for r in results.values())
            stems = [change_stems(INTEGRATION, e["files"]) for e in entries]
            doubled = collections.defaultdict(list)
            for symbol, definers in (duplicate_definitions(INTEGRATION) if built else {}).items():
                involved = [i for i, s in enumerate(stems) if s & set(definers)]
                outside = set(definers) - set().union(*stems)
                for index in (involved if outside else involved[1:]):
                    doubled[index].append(f"{symbol} ({', '.join(definers)})")
            failing = [e for i, e in enumerate(entries) if i in doubled or any(results[a]["rank"] != "O" for a in e["addresses"])]
            if failing:
                for entry in failing:
                    index = entries.index(entry)
                    if index in doubled:
                        log_event(self.database, "reject", f"{entry['proposal'].name}: defines symbols already defined: {doubled[index][:5]}")
                        execute(self.database, "UPDATE jobs SET status='failed', note=? WHERE id=?",
                                ("duplicate definition: " + "; ".join(doubled[index])[:500], entry["metadata"]["job"]))
                    else:
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

    def land_facts(self):
        """Commit the staged facts files to docs/facts/ on main as one docs-only commit."""
        staged = sorted(FACTS_STAGING.glob("*.md")) if FACTS_STAGING.exists() else []
        if not staged or self.halted:
            return
        with self.integration_lock:
            if self.target_moved_externally():
                return
            self.restore_integration()
            base = git(INTEGRATION, "rev-parse", "HEAD")
            (INTEGRATION / FACTS_DIRECTORY).mkdir(parents=True, exist_ok=True)
            for path in staged:
                shutil.copy2(path, INTEGRATION / FACTS_DIRECTORY / path.name)
            git(INTEGRATION, "add", "--", str(FACTS_DIRECTORY))
            if subprocess.run(["git", "-C", str(INTEGRATION), "diff", "--cached", "--quiet"]).returncode != 0:
                git(INTEGRATION, "commit", "-q", "-m", f"Record facts for {len(staged)} function{'s' if len(staged) != 1 else ''}\n\n"
                    "Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>")
                final = git(INTEGRATION, "rev-parse", "HEAD")
                if not build_inputs_equal(base, final) or not self.move(final, base, "factory: facts"):
                    self.restore_integration()
                    return
            for path in staged:
                path.unlink(missing_ok=True)

    def submit_class_proposal(self, proposal):
        """A class proposal whose header reaches too many exact functions rides the periodic full check: it becomes a
        class/ branch (a map-only commit naming the function, then the class files) and a submission like a root branch."""
        with self.integration_lock:
            metadata = json.loads((proposal / "matched.json").read_text())
            addresses = [int(a) for a in metadata["addresses"]]
            symbols = {int(k): v for k, v in metadata["symbols"].items()}
            class_files, job_base = metadata["class_files"], metadata["base"]
            self.restore_integration()
            base = git(INTEGRATION, "rev-parse", "HEAD")
            violations = shared_type_violations(INTEGRATION, base,
                {f: (proposal / f).read_text(errors="replace") for f in class_files if (proposal / f).is_file()})
            if violations:
                log_event(self.database, "reject", f"{proposal.name}: shared type policy: {violations}")
                execute(self.database, "UPDATE jobs SET status='failed', leased_by=NULL WHERE id=?", (metadata["job"],))
                self.retire(proposal, "rejected")
                return
            stale = [f for f in class_files if git(INTEGRATION, "rev-parse", "--verify", f"{base}:{f}", check=False)
                     != git(INTEGRATION, "rev-parse", "--verify", f"{job_base}:{f}", check=False)]
            if stale:
                log_event(self.database, "reject", f"{proposal.name}: class mode: {stale} changed on main since the job started")
                execute(self.database, "UPDATE jobs SET status='open', leased_by=NULL WHERE id=?", (metadata["job"],))
                self.retire(proposal, "rejected")
                return
            branch = f"class/{proposal.name}"
            git(INTEGRATION, "checkout", "-q", "-f", "-B", branch, base)
            current = {r["start"]: r for r in load_rows(INTEGRATION / MAP)}
            renames = {a: (None, symbols[a]) for a in addresses if current[a]["symbol"] != symbols[a]}
            if renames:
                set_map_rows(INTEGRATION, renames)
                git(INTEGRATION, "add", "--", str(MAP))
                git(INTEGRATION, "commit", "-q", "-m", "Name " + ", ".join(f"0x{a:08X} {symbols[a]}" for a in renames)
                    + " (class mode)\n\nThe class-mode worker wrote this function as a member; the row's symbol follows it.\n\n"
                    "Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>")
            for relative in class_files:
                if (proposal / relative).exists():
                    shutil.copy2(proposal / relative, INTEGRATION / relative)
            git(INTEGRATION, "add", "--", *class_files)
            git(INTEGRATION, "commit", "-q", "-m", f"Write {', '.join(symbols[a] for a in addresses)} in {class_files[-1]} (class mode, {metadata['model']})"
                "\n\nCo-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>")
            commit = git(INTEGRATION, "rev-parse", "HEAD")
            self.restore_integration()
            SUBMISSIONS.mkdir(parents=True, exist_ok=True)
            path = SUBMISSIONS / f"class-{proposal.name}.json"
            path.write_text(json.dumps({"branch": branch, "commit": commit, "claims": [symbols[a] for a in addresses],
                                        "summary": f"Class mode: {', '.join(symbols[a] for a in addresses)} in {class_files[-1]}",
                                        "attempts": metadata.get("attempts", 1), "minutes": round(metadata.get("minutes", 0), 4),
                                        "class_job": metadata["job"], "model": metadata["model"], "shared_header_job": True, "solo": True, "preserve_exact": True}, indent=2) + "\n")
            self.class_pending = path
            log_event(self.database, "submission", f"{path.stem}: rides the next periodic full check")
            self.retire(proposal, None)

    def integrate_class(self, proposal):
        """Land one class-mode proposal: the worker's class file and header replace main's, but only if main's copies
        are still the ones the job started from. The claim and every exact function in the objects the change compiles
        into must check O. Returns "ride" when a header change reaches too many exact functions to recheck here."""
        with self.integration_lock:
            if self.target_moved_externally():
                return
            metadata = json.loads((proposal / "matched.json").read_text())
            addresses = [int(a) for a in metadata["addresses"]]
            symbols = {int(k): v for k, v in metadata["symbols"].items()}
            class_files, job_base = metadata["class_files"], metadata["base"]
            self.restore_integration()
            base = git(INTEGRATION, "rev-parse", "HEAD")
            reject = None
            violations = shared_type_violations(INTEGRATION, base,
                {f: (proposal / f).read_text(errors="replace") for f in class_files if (proposal / f).is_file()})
            if violations:
                log_event(self.database, "reject", f"{proposal.name}: shared type policy: {violations}")
                execute(self.database, "UPDATE jobs SET status='failed', leased_by=NULL WHERE id=?", (metadata["job"],))
                self.retire(proposal, "rejected")
                return
            stale = [f for f in class_files if git(INTEGRATION, "rev-parse", "--verify", f"{base}:{f}", check=False)
                     != git(INTEGRATION, "rev-parse", "--verify", f"{job_base}:{f}", check=False)]
            current = {r["start"]: r for r in load_rows(INTEGRATION / MAP)}
            if stale:
                reject = f"{stale} changed on main since the job started"
            elif any(current[a]["rank"] == "O" for a in addresses):
                reject = "already exact on main"
            if not reject:
                for relative in class_files:
                    if (proposal / relative).exists():
                        shutil.copy2(proposal / relative, INTEGRATION / relative)
                set_map_rows(INTEGRATION, {a: ("M", symbols[a]) for a in addresses})
                git(INTEGRATION, "add", "--", str(MAP), *class_files)
                git(INTEGRATION, "commit", "-q", "--no-verify", "-m", f"Candidate class-mode {proposal.name}, not verified")
                candidate = git(INTEGRATION, "rev-parse", "HEAD")
                changed = [f for f in class_files if git(INTEGRATION, "diff", "--name-only", base, candidate, "--", f)]
                headers = [f for f in changed if f.endswith((".h", ".hpp"))]
                started = time.time()
                status, output = tool(INTEGRATION, "make.py", "eu", timeout=1800)
                if status != 0:
                    reject = "build fails: " + output[-300:]
                else:
                    # Recheck the claim and every exact function in the objects the change compiles into: the class
                    # file and, on a header change, every object whose dependency file names the header (owner,
                    # 2026-10-02). A header that reaches too many exact functions rides the periodic full check.
                    objects, defined = source_objects(INTEGRATION), set()
                    for stem in change_stems(INTEGRATION, changed):
                        if stem in objects:
                            listing = subprocess.run(["/opt/homebrew/bin/arm-none-eabi-nm", "--defined-only", str(objects[stem])],
                                                     capture_output=True, text=True).stdout
                            defined |= {parts[2] for parts in (line.split() for line in listing.splitlines()) if len(parts) == 3 and parts[1] in "Tt"}
                    others = [r for r in current.values() if r["rank"] == "O" and (r["symbol"] or default_name(r)) in defined]
                    if headers and len(others) > CLASS_HEADER_RECHECK_LIMIT:
                        log_event(self.database, "submission", f"{proposal.name}: {headers} reach {len(others)} exact functions;"
                                  " rides the next periodic full check")
                        self.restore_integration()
                        return "ride"
                    to_check = [(a, symbols[a]) for a in addresses] + [(r["start"], r["symbol"] or default_name(r)) for r in others]
                    failing = [symbol for address, symbol in to_check if check_rank(INTEGRATION, address, symbol) != "O"]
                    if failing:
                        reject = f"not exact after the change: {failing[:10]}"
                    if headers:
                        log_event(self.database, "class", f"{proposal.name}: header change rechecked {len(to_check)} functions"
                                  f" in {len(change_stems(INTEGRATION, changed))} objects in {int(time.time() - started)} s")
                if not reject:
                    doubled = offending_duplicates(duplicate_definitions(INTEGRATION), change_stems(INTEGRATION, changed))
                    if doubled:
                        reject = "defines symbols already defined: " + "; ".join(f"{s} ({', '.join(d)})" for s, d in list(doubled.items())[:5])
            if reject:
                log_event(self.database, "reject", f"{proposal.name}: class mode: {reject}")
                execute(self.database, "UPDATE jobs SET status=?, leased_by=NULL WHERE id=?",
                        ("open" if reject.endswith("since the job started") else "failed", metadata["job"]))
                self.restore_integration()
                self.retire(proposal, "rejected")
                return
            sizes = {a: current[a]["end"] - current[a]["start"] for a in addresses}
            stamp = datetime.datetime.now(EASTERN).isoformat(timespec="seconds")
            with open(INTEGRATION / "project/ledger.csv", "a") as stream:
                for address in addresses:
                    stream.write(f"{stamp},0x{address:08X},{symbols[address]},matched,{metadata.get('attempts', 1)},{metadata.get('minutes', 0):.4f}\n")
            git(INTEGRATION, "reset", "-q", "--soft", base)
            set_map_rows(INTEGRATION, {a: ("O", symbols[a]) for a in addresses})
            git(INTEGRATION, "add", "--", str(MAP), "project/ledger.csv", *class_files)
            git(INTEGRATION, "commit", "-q", "-m",
                f"Match {len(addresses)} function{'s' if len(addresses) > 1 else ''} in {class_files[-1]} (class mode, {metadata['model']})\n\n"
                + "".join(f"- 0x{a:08X} {symbols[a]} ({sizes[a]} bytes)\n" for a in addresses)
                + f"\nVerified by tools/check.py (the claim and every exact function in the class file{' and in every object that includes the changed header' if headers else ''}) on a candidate commit before main moved.\n\n"
                "Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>")
            final = git(INTEGRATION, "rev-parse", "HEAD")
            if not build_inputs_equal(candidate, final):
                self.halted = f"class mode {proposal.name}: final build inputs differ from the checked candidate"
                log_event(self.database, "alert", self.halted)
                self.restore_integration()
                return
            if not self.move(final, base, f"factory: class mode {proposal.name}"):
                self.restore_integration()
                return
            execute(self.database, "UPDATE jobs SET status='matched', matched=? WHERE id=?",
                    (",".join("%08X" % a for a in addresses), metadata["job"]))
            for address in addresses:
                self.rows_by_start[address]["rank"] = "O"
            log_event(self.database, "class_match", f"{len(addresses)} functions, {sum(sizes.values())} bytes, first 0x{addresses[0]:08X}"
                      f" in {class_files[-1]} ({int(time.time() - started)} s)")
            self.retire(proposal, None)

    # submissions from root/ and dot/ branches --------------------------------
    def pending_submissions(self):
        """Oldest first, except that frog's dot/ work goes before everything and cleanup branches go last (owner,
        2026-10-02: hard functions are frog's lane)."""
        def order(path):
            name = path.name
            return (0 if name.startswith("dot-") else 2 if name.startswith(CLEANUP_PREFIX.rstrip("/") + "-") else 1, path.stat().st_mtime)
        return sorted(SUBMISSIONS.glob("*.json"), key=order) if SUBMISSIONS.exists() else []

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
        try:
            request = json.loads(path.read_text())
        except (OSError, ValueError):
            request = {}
        if request.get("class_job"):
            if result["outcome"] == "accepted":
                execute(self.database, "UPDATE jobs SET status='matched', matched=? WHERE id=?",
                        (",".join(request.get("claims", [])), request["class_job"]))
                log_event(self.database, "class_match", f"{len(result.get('matched', []))} functions, {result.get('matched_bytes', 0)} bytes,"
                          f" first {request.get('claims', ['?'])[0]} via the periodic full check")
            elif result["outcome"] != "held":
                execute(self.database, "UPDATE jobs SET status='open', leased_by=NULL WHERE id=?", (request["class_job"],))
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
        forbidden = [p for p in changed if p == "project/ledger.csv" or
                     (p.startswith(str(FACTORY_SOURCE_DIRECTORY) + "/") and not branch.startswith(CLEANUP_PREFIX)
                      and not (branch.startswith("integrator/") and request.get("operator_approved"))
                      and not (branch.startswith("class/") and request.get("class_job") and request.get("shared_header_job")))]
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
        violations = shared_type_violations(INTEGRATION, merge_base,
            {p: git(INTEGRATION, "show", f"{commit}:{p}", check=False) for p in changed})
        if violations:
            raise SubmissionDecision({"outcome": "rejected", "reason": f"shared type policy: {violations}"})
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
                "addresses": addresses, "map_changed": str(MAP) in changed, "changed": changed}

    # periodic full check and push -------------------------------------------
    def full_check(self, worktree, claims=None):
        """Clean build, the regression pass over every O row, and a check of each claim ({address: symbol}).
        Returns (built, build output tail, symbols lost from O, symbols gained O).

        The regression pass is the full-image byte compare (owner, 2026-10-02). A row it reports different is
        demoted only when tools/check.py confirms. When the compare cannot reach every O row, and once a day as an
        audit, the full tools/check.py pass decides instead."""
        status, output = tool(worktree, "make.py", "eu", "-ca", timeout=1800)
        if status != 0:
            return False, output[-400:], [], set()
        audit = self.last_audit_day != datetime.datetime.now(EASTERN).date()
        flagged = None if SPLIT_DISABLED_FILE.exists() else full_image_compare(worktree)
        if flagged is None or audit:
            changes_file = worktree / "data/ver/eu/.changes"
            changes_file.unlink(missing_ok=True)
            started = time.time()
            tool(worktree, "tools/check.py", "-q", "-w", timeout=1800)
            lost, gained = lost_exact(changes_file), set(gained_exact(changes_file))
            if audit:
                self.record_audit(flagged, lost, int(time.time() - started))
            elif not SPLIT_DISABLED_FILE.exists():
                log_event(self.database, "split", "the full-image compare did not reach every O row; the full tools/check.py pass decided")
            return True, "", lost, gained
        lost = []
        for address, symbol in sorted(flagged.items()):
            if check_rank(worktree, address, symbol) == "O":
                log_event(self.database, "split", f"the full-image compare flagged {symbol}; tools/check.py keeps it O, so it stays")
            else:
                lost.append(symbol)
        gained = {symbol for address, symbol in (claims or {}).items() if check_rank(worktree, address, symbol) == "O"}
        return True, "", lost, gained

    def record_audit(self, flagged, lost, seconds):
        """The daily audit: the full tools/check.py pass beside the full-image compare on the same build. A row the
        checker demotes that the compare did not flag puts the regression pass back on tools/check.py."""
        self.last_audit_day = datetime.datetime.now(EASTERN).date()
        if flagged is None:
            log_event(self.database, "audit", f"full tools/check.py pass in {seconds} s demoted {len(lost)};"
                      " the full-image compare did not reach every O row")
            return
        missed = sorted(set(lost) - set(flagged.values()))
        log_event(self.database, "audit", f"full tools/check.py pass in {seconds} s demoted {len(lost)};"
                  f" the full-image compare flagged {len(flagged)}; demoted but not flagged: {missed[:10]}")
        if missed:
            SPLIT_DISABLED_FILE.write_text(f"{datetime.datetime.now(EASTERN).isoformat(timespec='seconds')} audit: tools/check.py"
                                           f" demoted {missed[:20]}, which the full-image compare did not flag\n")
            log_event(self.database, "alert", "audit: the full-image compare missed rows tools/check.py demoted; the regression"
                      f" pass is back on tools/check.py until the owner decides: {missed[:10]}")

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
            # A cleanup branch rides alone, so any loss of an O row is unambiguously its own.
            solo = [p for p in waiting if json.loads(p.read_text()).get("solo")
                    or json.loads(p.read_text()).get("branch", "").startswith(CLEANUP_PREFIX)]
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
            built, output, lost, gained = self.full_check(INTEGRATION, {info["addresses"][c]: c for _, info in riders for c in info["claims"]})
            missing = {path: [c for c in info["claims"] if c not in gained and rows[info["addresses"][c]]["rank"] != "O"]
                       for path, info in riders}
            # A rider may not define a symbol another object already defines (owner, 2026-10-02).
            duplicates = duplicate_definitions(INTEGRATION) if built and riders else {}
            doubled = {path: offending_duplicates(duplicates, change_stems(INTEGRATION, info["changed"])) for path, info in riders}
            doubled = {path: found for path, found in doubled.items() if found}
            cleanup = any(info["branch"].startswith(CLEANUP_PREFIX) or info["request"].get("preserve_exact") for _, info in riders)
            if cleanup and built and lost:
                # Owner guard: a cleanup branch may lose no O row. Rows main loses on its own are demoted first and
                # the branch waits a cycle; any loss beyond main's rejects it.
                git(MAIN_PROBE, "checkout", "-q", "--detach", "-f", base)
                git(MAIN_PROBE, "clean", "-fdq", "--", "Game", "lib")
                main_built, main_output, main_lost, _ = self.full_check(MAIN_PROBE)
                if not main_built:
                    self.halted = f"{TARGET_BRANCH} does not build cleanly"
                    log_event(self.database, "alert", self.halted + ": " + main_output)
                    self.restore_integration()
                    return
                beyond = sorted(set(lost) - set(main_lost))
                for path, info in riders:
                    if beyond:
                        self.decide(path, {"outcome": "rejected", "reason": f"cleanup loses exact rows: {beyond[:20]}"})
                    else:
                        log_event(self.database, "submission", f"{path.stem}: waits a cycle while main's own unconfirmed rows {main_lost[:5]} are demoted")
                riders, merged_head, checked, lost, gained, missing = [], base, base, main_lost, set(), {}
                self.restore_integration()
            if riders and (not built or lost or any(missing.values()) or doubled):
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
                if own or any(missing.values()) or doubled:
                    for path, info in riders:
                        if len(riders) == 1:
                            reason = (("the build fails with this branch merged: " + output[-300:]) if not built else
                                      "defines symbols already defined: " + "; ".join(f"{s} ({', '.join(d)})" for s, d in list(doubled[path].items())[:10])
                                      if path in doubled else
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
            if cleanup and riders:
                exact_before = {r["start"] for r in load_rows_at(base) if r["rank"] == "O"}
                exact_after = {r["start"] for r in load_rows_at(final) if r["rank"] == "O"}
                if not exact_after >= exact_before:
                    for path, info, _ in accepted:
                        self.decide(path, {"outcome": "rejected", "reason": "cleanup: exact rows after the commit do not contain"
                                           f" those before: {['%08X' % a for a in sorted(exact_before - exact_after)][:20]}"})
                    log_event(self.database, "alert", "cleanup guard rejected a branch at the final commit; main not moved")
                    self.restore_integration()
                    return
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
            self.measure_source_quality(final)
            after = sum(1 for r in self.rows_by_start.values() if r["rank"] == "O")
            log_event(self.database, "sync", f"full check in {int(time.time() - started)} s; {after} functions exact;"
                      f" {len(riders)} submissions rode along; {len(lost)} demoted;"
                      f" push {'ok' if push.returncode == 0 else 'failed: ' + push.stderr.strip()[-200:]}")


    def measure_source_quality(self, commit):
        """Exact bytes defined in Factory objects versus class files, from the clean build's symbol tables, and the
        raw address literal count under Game/. Both feed the dashboard."""
        try:
            defined = set()
            for obj in (INTEGRATION / "build/eu/obj" / FACTORY_SOURCE_DIRECTORY).rglob("*.o"):
                listing = subprocess.run(["/opt/homebrew/bin/arm-none-eabi-nm", "--defined-only", str(obj)],
                                         capture_output=True, text=True).stdout
                defined |= {parts[2] for parts in (line.split() for line in listing.splitlines()) if len(parts) == 3 and parts[1] in "Tt"}
            exact = [(r["symbol"] or default_name(r), r["end"] - r["start"]) for r in load_rows_at(commit) if r["rank"] == "O" and "f" in r["type"]]
            total = sum(size for _, size in exact)
            factory_bytes = sum(size for name, size in exact if name in defined)
            SOURCE_QUALITY_FILE.write_text(json.dumps({"commit": commit, "exact_bytes": total, "factory_bytes": factory_bytes,
                                                       "class_file_bytes": total - factory_bytes,
                                                       "class_file_share": round((total - factory_bytes) / total * 100, 1) if total else 0,
                                                       "measured": datetime.datetime.now(EASTERN).isoformat(timespec="seconds")}) + "\n")
            specification = importlib.util.spec_from_file_location("scan_address_literals", HOME / "scan_address_literals.py")
            scanner = importlib.util.module_from_spec(specification)
            specification.loader.exec_module(scanner)
            ADDRESS_LITERALS_FILE.write_text(json.dumps(scanner.scan(commit), indent=1) + "\n")
        except Exception as error:
            log_event(self.database, "error", f"source quality measurement failed: {error}")

    # main loop -----------------------------------------------------------
    def adjust_slots(self):
        """Run as many slots as the Mac holds without swapping or overheating: drop one on swap-outs or a 5-minute load
        average above LOAD_AVERAGE_LIMIT, add one after 15 quiet minutes, never above MAXIMUM_SLOTS."""
        now = time.time()
        load = os.getloadavg()[1]
        self.swap_samples = [(t, o) for t, o in self.swap_samples if t >= now - 1800] + [(now, swap_outs())]
        self.load_samples = [(t, l) for t, l in getattr(self, "load_samples", []) if t >= now - 1800] + [(now, load)]
        recent = [o for t, o in self.swap_samples if t >= now - 300]
        swapping = recent[-1] - recent[0] > SWAP_PAGES_PER_FIVE_MINUTES
        if (swapping or load > LOAD_AVERAGE_LIMIT) and self.allowed_slots > 1 and now - self.last_slot_change > 300:
            self.allowed_slots -= 1
            self.last_slot_change = now
            reason = f"swapping ({recent[-1] - recent[0]} pages in 5 min)" if swapping else f"5-minute load average {load:.1f}"
            log_event(self.database, "slots", f"{reason}; allowed slots now {self.allowed_slots}")
        elif (self.allowed_slots < self.maximum_slots and now - self.last_slot_change > QUIET_SECONDS_BEFORE_ADDING_A_SLOT
              and self.swap_samples[-1][1] - min(o for t, o in self.swap_samples if t >= now - QUIET_SECONDS_BEFORE_ADDING_A_SLOT) == 0
              and max(l for t, l in self.load_samples if t >= now - QUIET_SECONDS_BEFORE_ADDING_A_SLOT) <= LOAD_AVERAGE_LIMIT):
            self.allowed_slots += 1
            self.last_slot_change = now
            log_event(self.database, "slots", f"no swapping and load at most {LOAD_AVERAGE_LIMIT} for 15 min; allowed slots now {self.allowed_slots}")

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
        note = run["note"] or ""
        label = note[len("trial "):] if note.startswith("trial ") else (f"class mode tier {run['tier']}" if note.startswith("class") else f"tier {run['tier']}")
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
        "source_quality": source_quality(database, now),
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


def source_quality(database, now):
    """Class-file share of exact bytes, the class-file share of the last 24 hours' matched bytes, and the raw
    address literal count."""
    quality = {}
    for path, key in ((SOURCE_QUALITY_FILE, "files"), (ADDRESS_LITERALS_FILE, "literals")):
        try:
            quality[key] = json.loads(path.read_text())
        except (OSError, ValueError):
            quality[key] = {}
    with DATABASE_LOCK:
        events = database.execute("SELECT kind, text FROM events WHERE kind IN ('match', 'class_match', 'submission') AND time > ?", (now - 86400,)).fetchall()
    factory_bytes = class_bytes = 0
    for event in events:
        found = re.search(r"(\d+) bytes", event["text"])
        if not found:
            continue
        if event["kind"] == "match":
            factory_bytes += int(found[1])
        elif event["kind"] == "class_match" or (": accepted" in event["text"] and not event["text"].startswith("class-")):
            class_bytes += int(found[1])
    files, literals = quality["files"], quality["literals"]
    return {"class_file_share": files.get("class_file_share"), "factory_bytes": files.get("factory_bytes"),
            "class_file_bytes": files.get("class_file_bytes"), "measured": files.get("measured"),
            "last_24_hours_class_file_share": round(class_bytes / (factory_bytes + class_bytes) * 100, 1) if factory_bytes + class_bytes else None,
            "last_24_hours_matched_bytes": factory_bytes + class_bytes,
            "address_literals": literals.get("total"), "power_of_two_constants": literals.get("power_of_two_constants"),
            "address_literal_files": {path: counts.get("code", 0) + counts.get("rodata", 0) + counts.get("data", 0)
                                      for path, counts in literals.get("files", {}).items()
                                      if counts.get("code", 0) + counts.get("rodata", 0) + counts.get("data", 0)}}


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
    """s1 is reserved for the 256-511 byte band; s2 runs class mode; s3 is Luna; then Sol slots (s4 prefers tier 2). Luna alone takes
    every tier 1 job under 32 bytes; Sol takes 32 to 255 bytes by ascending body size (owner, 2026-10-03).
    Tier 1 jobs of 192 to 255 bytes wait until smaller eligible jobs have been leased or exhausted.
    The swap guard pauses the highest-numbered slots first."""
    kinds = ["group"] if group_only else None
    small = luna_single_bytes or 32
    roles = ["band", "class", "luna", "tier2", "tier1"] + ["tier1"] * max(0, count - 5)
    hard_end_sol_slot = 4  # s5, the first plain Sol slot, also runs hard-end trial jobs first
    specs = {}
    for index, role in enumerate(roles[:count]):
        if role == "band":
            spec = {"tiers": [BAND_TIER], "kinds": kinds, "hard_end_first": True}
        elif role == "class":
            spec = {"tiers": [], "class_mode": True}
        elif role == "luna":
            spec = {"tiers": [1], "kinds": kinds, "ranges": [(0, small)],
                    "settings": TRIAL_SETTINGS["luna-medium"], "fallback_kinds": ["facts"]}
        else:
            spec = {"tiers": [2, LUNA_GROUP_MISS_TIER, 1] if role == "tier2" else [1, LUNA_GROUP_MISS_TIER, 2], "kinds": kinds,
                    "ranges": [(small, 256)], "smallest_first": True}
        if index == hard_end_sol_slot:
            spec = dict(spec, hard_end_first=True)
        elif index == hard_end_sol_slot + 1:
            spec = dict(spec, hard_end_until=HARD_END_THIRD_SLOT_UNTIL)  # s6, when the swap guard has it enabled
        specs[f"s{index + 1}"] = dict(spec, index=index, role=role)
    return specs


def command_class_jobs(arguments):
    """Queue class-mode jobs: every unmatched function up to 511 bytes in a class's translation unit, one per job."""
    database = connect()
    rows = target_rows()
    queued = {a for (addresses,) in database.execute("SELECT addresses FROM jobs WHERE kind='class'") for a in addresses.split(",")}
    note = json.dumps({"class": arguments.name, "header": arguments.header, "file": arguments.file})
    created = 0
    for row in rows:
        size = row["end"] - row["start"]
        if not ("f" in row["type"] and arguments.start <= row["start"] < arguments.end and row["rank"] != "O" and size <= 511):
            continue
        if "%08X" % row["start"] in queued:
            continue
        tier = 0 if size < 32 else (1 if size < 256 else BAND_TIER)
        database.execute("INSERT INTO jobs (kind, addresses, body_bytes, total_bytes, tier, priority, status, sibling, note)"
                         " VALUES ('class', ?, ?, ?, ?, ?, 'open', '', ?)", ("%08X" % row["start"], size, size, tier, -row["start"], note))
        created += 1
    database.commit()
    print(f"{created} class-mode jobs for {arguments.name} ({arguments.file})")


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
    class_jobs = commands.add_parser("class-jobs")
    class_jobs.add_argument("--name", required=True)
    class_jobs.add_argument("--header", required=True)
    class_jobs.add_argument("--file", required=True)
    class_jobs.add_argument("--start", type=lambda v: int(v, 0), required=True)
    class_jobs.add_argument("--end", type=lambda v: int(v, 0), required=True)
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
    elif arguments.command == "class-jobs":
        command_class_jobs(arguments)
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
