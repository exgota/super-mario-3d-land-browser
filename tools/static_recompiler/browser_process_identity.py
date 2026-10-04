"""Read-only operating-system identity and scoped browser-process inspection.

No function sends a signal, invokes the browser CLI, reads CLI metadata, or
launches a process. An observation's ``state`` is present, absent, or unknown.
Only an explicit OS not-found result is absent. Permission, format, truncation
and identity-race failures remain unknown.

Identity matching is three-valued: True means the PID, UID, platform, birth and
boot identity still agree; False means confirmed absence or a different process;
None means the answer is unknown. Status, parent PID and executable are useful
observations, not stable birth-identity fields. A zombie remains present.

The caller supplies an already validated prospective session registration with
cli_daemon_path, session_name, profile_path, uid, optional cli_entry_path,
config_path and observed_processes identity records. The caller owns its randomness, source pins,
metadata, profile ownership, inactivity decision and every cleanup action.
"""

from __future__ import annotations

import ctypes
import errno
from functools import lru_cache
import os
from pathlib import Path
import re
import sys
import uuid


_MAX_PROCESS_IDENTIFIER = (1 << 31) - 1
_MAX_PROCESS_COUNT = 1 << 20
_MAX_ARGUMENT_BYTES = 4 * 1024 * 1024
_MAX_ARGUMENT_COUNT = 16384
_MAX_PATH_BYTES = 4096
_MAX_SMALL_FILE_BYTES = 65536


class _InspectionFailure(Exception):
    def __init__(self, operation: str, message: str, error_number: int | None = None,
                 *, absent: bool = False):
        super().__init__(message)
        self.operation = operation
        self.error_number = error_number
        self.absent = absent

    def record(self) -> dict:
        return {"operation": self.operation, "errno": self.error_number,
                "message": str(self)}


class _DarwinProcessInformation(ctypes.Structure):
    # Actual public SDK sys/proc_info.h struct proc_bsdinfo, LP64 ABI.
    _fields_ = [
        ("pbi_flags", ctypes.c_uint32), ("pbi_status", ctypes.c_uint32),
        ("pbi_xstatus", ctypes.c_uint32), ("pbi_pid", ctypes.c_uint32),
        ("pbi_ppid", ctypes.c_uint32), ("pbi_uid", ctypes.c_uint32),
        ("pbi_gid", ctypes.c_uint32), ("pbi_ruid", ctypes.c_uint32),
        ("pbi_rgid", ctypes.c_uint32), ("pbi_svuid", ctypes.c_uint32),
        ("pbi_svgid", ctypes.c_uint32), ("rfu_1", ctypes.c_uint32),
        ("pbi_comm", ctypes.c_char * 16), ("pbi_name", ctypes.c_char * 32),
        ("pbi_nfiles", ctypes.c_uint32), ("pbi_pgid", ctypes.c_uint32),
        ("pbi_pjobc", ctypes.c_uint32), ("e_tdev", ctypes.c_uint32),
        ("e_tpgid", ctypes.c_uint32), ("pbi_nice", ctypes.c_int32),
        ("pbi_start_tvsec", ctypes.c_uint64),
        ("pbi_start_tvusec", ctypes.c_uint64),
    ]


def _valid_process_identifier(value) -> bool:
    return type(value) is int and 1 <= value <= _MAX_PROCESS_IDENTIFIER


def _empty_identity(pid) -> dict:
    return {"state": "unknown", "pid": pid if type(pid) is int else None,
            "platform": sys.platform, "uid": None, "parent_pid": None,
            "status": None, "executable": None, "process_name": None,
            "birth": None, "boot_id": None, "error": None}


def _system_failure(operation: str, *, allow_absent: bool = False) -> _InspectionFailure:
    error_number = ctypes.get_errno()
    message = os.strerror(error_number) if error_number else "OS returned no result or errno"
    return _InspectionFailure(operation, message, error_number or None,
                              absent=allow_absent and error_number == errno.ESRCH)


@lru_cache(maxsize=1)
def _darwin_libraries():
    if ctypes.sizeof(ctypes.c_void_p) != 8 or ctypes.sizeof(_DarwinProcessInformation) != 136:
        raise _InspectionFailure("darwin_abi", "Unsupported proc_bsdinfo ABI")
    try:
        process_library = ctypes.CDLL("/usr/lib/libproc.dylib", use_errno=True)
        system_library = ctypes.CDLL("/usr/lib/libSystem.B.dylib", use_errno=True)
        process_library.proc_pidinfo.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_uint64,
                                                ctypes.c_void_p, ctypes.c_int]
        process_library.proc_pidinfo.restype = ctypes.c_int
        process_library.proc_pidpath.argtypes = [ctypes.c_int, ctypes.c_void_p, ctypes.c_uint32]
        process_library.proc_pidpath.restype = ctypes.c_int
        process_library.proc_listpids.argtypes = [ctypes.c_uint32, ctypes.c_uint32,
                                                 ctypes.c_void_p, ctypes.c_int]
        process_library.proc_listpids.restype = ctypes.c_int
        system_library.sysctlbyname.argtypes = [ctypes.c_char_p, ctypes.c_void_p,
                                                ctypes.POINTER(ctypes.c_size_t),
                                                ctypes.c_void_p, ctypes.c_size_t]
        system_library.sysctlbyname.restype = ctypes.c_int
        system_library.sysctl.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.c_uint,
                                         ctypes.c_void_p, ctypes.POINTER(ctypes.c_size_t),
                                         ctypes.c_void_p, ctypes.c_size_t]
        system_library.sysctl.restype = ctypes.c_int
    except (AttributeError, OSError) as problem:
        raise _InspectionFailure("darwin_libraries", str(problem)) from problem
    return process_library, system_library


def _darwin_named_value(name: bytes, maximum_bytes: int) -> bytes:
    _, system_library = _darwin_libraries()
    size = ctypes.c_size_t()
    ctypes.set_errno(0)
    if system_library.sysctlbyname(name, None, ctypes.byref(size), None, 0) != 0:
        raise _system_failure("sysctlbyname_size")
    if not 0 < size.value <= maximum_bytes:
        raise _InspectionFailure("sysctlbyname_size", "Named value exceeds its bound")
    buffer = ctypes.create_string_buffer(size.value)
    capacity = size.value
    ctypes.set_errno(0)
    if system_library.sysctlbyname(name, buffer, ctypes.byref(size), None, 0) != 0:
        raise _system_failure("sysctlbyname_read")
    if not 0 < size.value <= capacity:
        raise _InspectionFailure("sysctlbyname_read", "Named value was truncated or changed size")
    return buffer.raw[:size.value]


def _boot_identifier(raw: bytes, operation: str) -> str:
    try:
        text = raw.rstrip(b"\0\n").decode("ascii")
        identifier = str(uuid.UUID(text))
        if text.lower() != identifier:
            raise ValueError("Noncanonical boot UUID")
    except (UnicodeError, ValueError) as problem:
        raise _InspectionFailure(operation, "Boot identity is not a canonical UUID") from problem
    return identifier


def _darwin_information(pid: int) -> dict:
    process_library, _ = _darwin_libraries()
    information = _DarwinProcessInformation()
    ctypes.set_errno(0)
    # XNU PROC_PIDTBSDINFO uses nonzero arg to include unreaped zombies.
    count = process_library.proc_pidinfo(pid, 3, 1, ctypes.byref(information),
                                         ctypes.sizeof(information))  # PROC_PIDTBSDINFO
    if count <= 0:
        raise _system_failure("proc_pidinfo", allow_absent=True)
    if count != ctypes.sizeof(information):
        raise _InspectionFailure("proc_pidinfo", "Truncated proc_bsdinfo")
    if information.pbi_pid != pid or not information.pbi_start_tvsec or information.pbi_start_tvusec >= 1000000:
        raise _InspectionFailure("proc_pidinfo", "Invalid proc_bsdinfo identity")
    status = {1: "starting", 2: "running", 3: "sleeping", 4: "stopped", 5: "zombie"}
    return {"pid": pid, "platform": "darwin", "uid": information.pbi_uid,
            "parent_pid": information.pbi_ppid,
            "status": status.get(information.pbi_status, "unknown"),
            "process_name": os.fsdecode(information.pbi_comm),
            "birth": {"seconds": information.pbi_start_tvsec,
                      "microseconds": information.pbi_start_tvusec}}


def _darwin_executable(pid: int) -> str:
    process_library, _ = _darwin_libraries()
    buffer = ctypes.create_string_buffer(_MAX_PATH_BYTES)
    ctypes.set_errno(0)
    count = process_library.proc_pidpath(pid, buffer, len(buffer))
    if count <= 0:
        raise _system_failure("proc_pidpath", allow_absent=True)
    if count >= len(buffer) or b"\0" not in buffer.raw:
        raise _InspectionFailure("proc_pidpath", "Executable path was truncated")
    path = os.fsdecode(buffer.value)
    if not path or not os.path.isabs(path):
        raise _InspectionFailure("proc_pidpath", "Executable path is not absolute")
    return path


def _bounded_file(path: Path, maximum_bytes: int, operation: str,
                  *, allow_absent: bool = False) -> bytes:
    try:
        with path.open("rb") as source:
            raw = source.read(maximum_bytes + 1)
    except OSError as problem:
        raise _InspectionFailure(operation, str(problem), problem.errno,
                                  absent=allow_absent and problem.errno in (errno.ENOENT, errno.ESRCH)) from problem
    if len(raw) > maximum_bytes:
        raise _InspectionFailure(operation, "File exceeds its read bound")
    return raw


def _linux_proc_visibility() -> None:
    # hidepid=2 can present a permission refusal as ENOENT. It is not proof of death.
    raw = _bounded_file(Path("/proc/self/mountinfo"), _MAX_SMALL_FILE_BYTES,
                        "proc_mount_visibility")
    entries = []
    for line in raw.splitlines():
        fields = line.split()
        if len(fields) < 10 or b"-" not in fields:
            continue
        separator = fields.index(b"-")
        if fields[4] == b"/proc" and separator + 3 < len(fields) and fields[separator + 1] == b"proc":
            entries.append(fields[5].split(b",") + fields[separator + 3].split(b","))
    if len(entries) != 1 or any(option.startswith(b"hidepid=") and option != b"hidepid=0"
                                 for option in entries[0]):
        raise _InspectionFailure("proc_mount_visibility", "Proc visibility is unknown or restricted")


def _parse_linux_stat(raw: bytes, pid: int) -> dict:
    opening, closing = raw.find(b"("), raw.rfind(b")")
    try:
        if opening < 1 or closing <= opening or int(raw[:opening].strip()) != pid:
            raise ValueError("Wrong PID or comm extent")
        fields = raw[closing + 1:].split()
        if len(fields) < 20 or len(fields[0]) != 1:
            raise ValueError("Missing stat fields")
        parent_pid, birth_ticks = int(fields[1]), int(fields[19])
        if not 0 <= parent_pid <= _MAX_PROCESS_IDENTIFIER or birth_ticks < 0:
            raise ValueError("Invalid stat identity")
        state = fields[0].decode("ascii")
    except (UnicodeError, ValueError, IndexError) as problem:
        raise _InspectionFailure("proc_stat_format", "Invalid process stat") from problem
    statuses = {"R": "running", "S": "sleeping", "D": "sleeping", "T": "stopped",
                "t": "stopped", "Z": "zombie", "X": "dead", "x": "dead", "I": "idle"}
    return {"parent_pid": parent_pid, "status": statuses.get(state, "unknown"),
            "process_name": os.fsdecode(raw[opening + 1:closing]), "birth_ticks": birth_ticks}


def _linux_information(pid: int) -> dict:
    _linux_proc_visibility()
    directory = Path("/proc") / str(pid)
    information = _parse_linux_stat(_bounded_file(directory / "stat", _MAX_SMALL_FILE_BYTES,
                                                  "proc_stat", allow_absent=True), pid)
    raw_status = _bounded_file(directory / "status", _MAX_SMALL_FILE_BYTES, "proc_status")
    uid_lines = [line.split()[1:] for line in raw_status.splitlines() if line.startswith(b"Uid:")]
    try:
        if len(uid_lines) != 1 or len(uid_lines[0]) != 4:
            raise ValueError("Missing effective UID")
        uid = int(uid_lines[0][1])
        clock_rate = os.sysconf("SC_CLK_TCK")
        if not 0 <= uid < 1 << 32 or not 0 < clock_rate <= 1000000:
            raise ValueError("Invalid UID or clock rate")
    except (ValueError, OSError) as problem:
        raise _InspectionFailure("proc_status_format", "Invalid process UID or clock rate") from problem
    return {"pid": pid, "platform": "linux", "uid": uid,
            "parent_pid": information["parent_pid"], "status": information["status"],
            "process_name": information["process_name"],
            "birth": {"clock_ticks": information["birth_ticks"], "clock_ticks_per_second": clock_rate}}


def _linux_executable(pid: int) -> str:
    try:
        path = os.readlink(f"/proc/{pid}/exe")
    except OSError as problem:
        raise _InspectionFailure("proc_executable", str(problem), problem.errno) from problem
    if not os.path.isabs(path) or len(os.fsencode(path)) >= _MAX_PATH_BYTES:
        raise _InspectionFailure("proc_executable", "Executable path is invalid or exceeds its bound")
    return path


def _identity_key(record) -> tuple | None:
    if not isinstance(record, dict) or record.get("state") != "present":
        return None
    pid, uid, platform = record.get("pid"), record.get("uid"), record.get("platform")
    birth, boot_id = record.get("birth"), record.get("boot_id")
    if not _valid_process_identifier(pid) or type(uid) is not int or not 0 <= uid < 1 << 32:
        return None
    if not isinstance(birth, dict) or not isinstance(boot_id, str):
        return None
    try:
        if str(uuid.UUID(boot_id)) != boot_id:
            return None
    except ValueError:
        return None
    if platform == "darwin":
        seconds, microseconds = birth.get("seconds"), birth.get("microseconds")
        if type(seconds) is not int or seconds <= 0 or type(microseconds) is not int or not 0 <= microseconds < 1000000:
            return None
        start = (seconds, microseconds)
    elif platform == "linux":
        ticks, rate = birth.get("clock_ticks"), birth.get("clock_ticks_per_second")
        if type(ticks) is not int or ticks < 0 or type(rate) is not int or not 0 < rate <= 1000000:
            return None
        start = (ticks, rate)
    else:
        return None
    return pid, uid, platform, boot_id, start


def process_identity(pid: int) -> dict:
    """Return a bounded OS observation. Absence is never inferred from access failure."""
    record = _empty_identity(pid)
    if not _valid_process_identifier(pid):
        record["error"] = {"operation": "validate_pid", "errno": None,
                           "message": "A positive signed 32-bit process identifier is required"}
        return record
    try:
        if sys.platform == "darwin":
            information = _darwin_information(pid)
            record.update(information)
            record["boot_id"] = _boot_identifier(_darwin_named_value(b"kern.bootsessionuuid", 256), "boot_session_uuid")
            if information["status"] != "zombie":
                record["executable"] = _darwin_executable(pid)
            reread = _darwin_information(pid)
        elif sys.platform == "linux":
            information = _linux_information(pid)
            record.update(information)
            record["boot_id"] = _boot_identifier(_bounded_file(Path("/proc/sys/kernel/random/boot_id"),
                                                                128, "boot_id"), "boot_id")
            if information["status"] not in ("zombie", "dead"):
                record["executable"] = _linux_executable(pid)
            reread = _linux_information(pid)
        else:
            raise _InspectionFailure("platform", "Only Darwin LP64 and Linux procfs are supported")
        stable_fields = ("pid", "uid", "platform", "birth")
        if any(information[name] != reread[name] for name in stable_fields):
            raise _InspectionFailure("identity_reread", "Process identity changed while being read")
        record.update({name: reread[name] for name in ("parent_pid", "status", "process_name")})
        record["state"] = "present"
        if _identity_key(record) is None:
            raise _InspectionFailure("identity_format", "Invalid completed process identity")
    except _InspectionFailure as problem:
        record["state"] = "absent" if problem.absent else "unknown"
        record["error"] = problem.record()
    return record


def _parse_darwin_arguments(raw: bytes) -> list[str]:
    integer_size = ctypes.sizeof(ctypes.c_int)
    if len(raw) < integer_size + 2:
        raise _InspectionFailure("proc_arguments_format", "Missing argument extent")
    count = int.from_bytes(raw[:integer_size], sys.byteorder, signed=True)
    if not 0 < count <= _MAX_ARGUMENT_COUNT:
        raise _InspectionFailure("proc_arguments_format", "Invalid argument count")
    executable_end = raw.find(b"\0", integer_size)
    if executable_end < 0:
        raise _InspectionFailure("proc_arguments_format", "Missing executable terminator")
    position = executable_end + 1
    while position < len(raw) and raw[position] == 0:
        position += 1
    arguments = []
    for _ in range(count):
        end = raw.find(b"\0", position)
        if end < 0:
            raise _InspectionFailure("proc_arguments_format", "Truncated arguments")
        arguments.append(os.fsdecode(raw[position:end]))
        position = end + 1
    if not arguments or not arguments[0]:
        raise _InspectionFailure("proc_arguments_format", "Missing argv zero")
    return arguments


def _darwin_arguments(pid: int) -> list[str]:
    _, system_library = _darwin_libraries()
    maximum = _darwin_named_value(b"kern.argmax", ctypes.sizeof(ctypes.c_int))
    if len(maximum) != ctypes.sizeof(ctypes.c_int):
        raise _InspectionFailure("kern_argmax", "Wrong integer extent")
    capacity = int.from_bytes(maximum, sys.byteorder, signed=True)
    if not 0 < capacity <= _MAX_ARGUMENT_BYTES:
        raise _InspectionFailure("kern_argmax", "Kernel argument extent exceeds its bound")
    buffer = ctypes.create_string_buffer(capacity)
    size = ctypes.c_size_t(capacity)
    mib = (ctypes.c_int * 3)(1, 49, pid)  # CTL_KERN, KERN_PROCARGS2, pid
    ctypes.set_errno(0)
    if system_library.sysctl(mib, 3, buffer, ctypes.byref(size), None, 0) != 0:
        raise _system_failure("kern_procargs2", allow_absent=True)
    if not 0 < size.value <= capacity:
        raise _InspectionFailure("kern_procargs2", "Invalid argument read extent")
    # Environment bytes following the argc arguments are never returned.
    return _parse_darwin_arguments(buffer.raw[:size.value])


def _linux_arguments(pid: int) -> list[str]:
    raw = _bounded_file(Path(f"/proc/{pid}/cmdline"), _MAX_ARGUMENT_BYTES, "proc_cmdline")
    if not raw or not raw.endswith(b"\0"):
        raise _InspectionFailure("proc_cmdline", "Arguments are absent or unterminated")
    arguments = raw[:-1].split(b"\0")
    if not arguments[0] or len(arguments) > _MAX_ARGUMENT_COUNT:
        raise _InspectionFailure("proc_cmdline", "Invalid argument count or argv zero")
    return [os.fsdecode(argument) for argument in arguments]


def process_arguments(pid: int) -> dict:
    """Read argv between matching identity observations, without returning environment."""
    first = process_identity(pid)
    result = {"state": first["state"], "pid": first["pid"], "identity": first,
              "arguments": None, "error": first["error"]}
    if first["state"] != "present":
        return result
    try:
        if first["status"] in ("zombie", "dead"):
            raise _InspectionFailure("process_arguments", "A terminated process has no reliable argv")
        arguments = _darwin_arguments(pid) if sys.platform == "darwin" else _linux_arguments(pid)
        second = process_identity(pid)
        result["identity"] = second
        if second["state"] != "present":
            result.update(state=second["state"], error=second["error"])
            return result
        if (_identity_key(first) != _identity_key(second) or
                first["executable"] != second["executable"]):
            raise _InspectionFailure("arguments_identity_reread", "Process changed while argv was read")
        result.update(state="present", arguments=arguments, error=None)
    except _InspectionFailure as problem:
        # An argv failure is not a dead-process verdict. Independently recheck birth.
        second = process_identity(pid)
        result["identity"] = second
        result["state"] = "absent" if second["state"] == "absent" else "unknown"
        result["error"] = problem.record()
    return result


def process_identifiers(*, uid: int | None = None) -> dict:
    """Return bounded PID identifiers only. This performs no CLI/session inventory."""
    try:
        if uid is not None and (type(uid) is not int or not 0 <= uid < 1 << 32):
            raise _InspectionFailure("proc_identifiers", "Invalid effective UID filter")
        if sys.platform == "darwin":
            process_library, _ = _darwin_libraries()
            selection, selection_information = (1, 0) if uid is None else (4, uid)  # PROC_ALL_PIDS / PROC_UID_ONLY
            ctypes.set_errno(0)
            required = process_library.proc_listpids(selection, selection_information, None, 0)
            if required <= 0:
                raise _system_failure("proc_listpids_size")
            capacity = max(1024, required // ctypes.sizeof(ctypes.c_int) + 1024)
            for _ in range(4):
                if capacity > _MAX_PROCESS_COUNT:
                    raise _InspectionFailure("proc_listpids", "PID enumeration exceeds its bound")
                buffer = (ctypes.c_int * capacity)()
                ctypes.set_errno(0)
                count = process_library.proc_listpids(selection, selection_information, buffer, ctypes.sizeof(buffer))
                if count <= 0:
                    raise _system_failure("proc_listpids")
                if count % ctypes.sizeof(ctypes.c_int) or count > ctypes.sizeof(buffer):
                    raise _InspectionFailure("proc_listpids", "Invalid PID-array extent")
                if count < ctypes.sizeof(buffer):
                    identifiers = list(buffer[:count // ctypes.sizeof(ctypes.c_int)])
                    break
                capacity *= 2
            else:
                raise _InspectionFailure("proc_listpids", "PID enumeration kept changing size")
        elif sys.platform == "linux":
            if uid is not None:
                raise _InspectionFailure("proc_identifiers", "Kernel UID enumeration is available only on Darwin")
            _linux_proc_visibility()
            identifiers = []
            try:
                with os.scandir("/proc") as entries:
                    for entry in entries:
                        if entry.name.isascii() and entry.name.isdecimal():
                            identifiers.append(int(entry.name))
                            if len(identifiers) > _MAX_PROCESS_COUNT:
                                raise _InspectionFailure("proc_identifiers", "PID enumeration exceeds its bound")
            except OSError as problem:
                raise _InspectionFailure("proc_identifiers", str(problem), problem.errno) from problem
        else:
            raise _InspectionFailure("platform", "Only Darwin LP64 and Linux procfs are supported")
        identifiers = sorted(set(identifier for identifier in identifiers if _valid_process_identifier(identifier)))
        return {"state": "present", "pids": identifiers, "error": None}
    except _InspectionFailure as problem:
        return {"state": "unknown", "pids": [], "error": problem.record()}


def identity_matches(record: dict) -> bool | None:
    """Return True, False or None. Callers must preserve resources on None."""
    expected = _identity_key(record)
    if expected is None:
        return None
    observed = process_identity(record["pid"])
    if observed["state"] == "absent":
        return False
    if observed["state"] != "present":
        return None
    return expected == _identity_key(observed)


def _registered_path(value, field: str) -> str:
    if (not isinstance(value, str) or not value or "\0" in value or
            not os.path.isabs(value) or os.path.normpath(value) != value or
            len(os.fsencode(value)) >= _MAX_PATH_BYTES):
        raise ValueError(f"{field} requires an absolute normalized bounded path")
    return value


def _option_values(arguments: list[str], option: str) -> list[str] | None:
    values = []
    for index, argument in enumerate(arguments):
        if argument == option:
            if index + 1 >= len(arguments) or arguments[index + 1].startswith("--"):
                return None
            values.append(arguments[index + 1])
        elif argument.startswith(option + "="):
            value = argument[len(option) + 1:]
            if not value:
                return None
            values.append(value)
    return values


def _browser_process_role(arguments: list[str], registration: dict) -> str | None:
    profiles = _option_values(arguments[1:], "--user-data-dir")
    child_types = _option_values(arguments[1:], "--type")
    if profiles == [registration["profile_path"]] and child_types is not None and len(child_types) <= 1:
        return "browser_child" if child_types else "browser_root"
    database = _option_values(arguments[1:], "--database")
    if profiles == [] and database == [os.path.join(registration["profile_path"], "Crashpad")]:
        return "crashpad"
    return None


def _session_process_role(arguments: list[str], registration: dict) -> str | None:
    if len(arguments) >= 3 and arguments[1] == registration.get("cli_entry_path"):
        sessions = [argument for argument in arguments[2:] if argument.startswith("-s=")]
        aliases = [argument for argument in arguments[2:]
                   if argument in ("-s", "--session") or argument.startswith("--session=")]
        if arguments[2] == "-s=" + registration["session_name"] and sessions == [arguments[2]] and not aliases:
            return "client"
        return None
    if (len(arguments) >= 3 and arguments[1] == registration["cli_daemon_path"] and
            arguments[2] == registration["session_name"]):
        matched = False
        for option, key in (("--profile", "profile_path"), ("--config", "config_path")):
            values = _option_values(arguments[3:], option)
            if values is None or len(values) > 1:
                return None
            if values:
                if key not in registration or values[0] != registration[key]:
                    return None
                matched = True
        if matched:
            return "daemon"
        return None
    if _browser_process_role(arguments, registration) is not None:
        return "browser"
    return None


def _process_kind(record: dict) -> str | None:
    executable = record.get("executable")
    names = [os.path.basename(executable)] if executable else [record.get("process_name")]
    for name in names:
        if not isinstance(name, str):
            continue
        if name.lower() in ("node", "nodejs"):
            return "daemon"
        if name.lower() in ("chrome_crashpad_handler", "crashpad_handler") or (not executable and name.lower().startswith("chrome_crashpad")):
            return "crashpad"
        if name.lower() in ("chrome", "chromium", "chromium-browser") or name.lower().startswith(("google chrome", "chromium")):
            return "browser"
    return None


def _plausible_process(record: dict, observed_pids: set[int]) -> bool:
    return record["pid"] in observed_pids or _process_kind(record) is not None


def owned_session_processes(registration: dict) -> dict:
    """Expose only exact registered CLI/profile processes; report candidate uncertainty.

    ``complete`` describes this scoped observation, not an atomic OS snapshot or
    general proof of machine-wide absence. Unclassified access failures are
    counted without exposing their argv or treating them as owned processes.
    Callers must recheck matching identities and ownership before any action.
    """
    result = {"state": "invalid", "client": [], "daemon": [], "browser": [], "errors": [],
              "unclassified_process_count": 0}
    try:
        if not isinstance(registration, dict):
            raise ValueError("Registration must be a dictionary")
        for key in ("cli_daemon_path", "profile_path"):
            _registered_path(registration.get(key), key)
        for key in ("cli_entry_path", "config_path"):
            if key in registration:
                _registered_path(registration[key], key)
        session = registration.get("session_name")
        uid = registration.get("uid")
        if not isinstance(session, str) or not re.fullmatch(r"[A-Za-z0-9][A-Za-z0-9_-]{0,127}", session):
            raise ValueError("Invalid bounded session name")
        if type(uid) is not int or uid != os.geteuid():
            raise ValueError("Registration UID must equal the inspecting effective UID")
        observed = registration.get("observed_processes", [])
        if not isinstance(observed, list) or len(observed) > 1024 or any(_identity_key(item) is None for item in observed):
            raise ValueError("Invalid observed process identities")
        observed_pids = {item["pid"] for item in observed}
    except ValueError as problem:
        result["errors"].append({"operation": "validate_registration", "errno": None,
                                 "message": str(problem)})
        return result
    inventory = process_identifiers(uid=uid) if sys.platform == "darwin" else process_identifiers()
    if inventory["state"] != "present":
        result.update(state="uncertain", errors=[inventory["error"]])
        return result
    result["state"] = "complete"
    # The kernel UID list excludes a historical PID now reused by another user.
    # Reintroducing every old PID would turn an unrelated protected process into
    # an owned candidate. Every listed candidate still needs identity/argv checks.
    candidates = set(inventory["pids"])
    if sys.platform != "darwin":
        candidates |= observed_pids
    for pid in sorted(candidates):
        observation = process_identity(pid)
        if observation["state"] == "absent" or (observation["uid"] is not None and observation["uid"] != uid):
            continue
        plausible = _plausible_process(observation, observed_pids)
        if observation["state"] != "present":
            if plausible:
                result["errors"].append({"pid": pid, "operation": "candidate_identity",
                                         "error": observation["error"]})
            else:
                result["unclassified_process_count"] += 1
            continue
        if not plausible or observation["status"] in ("zombie", "dead"):
            continue
        arguments = process_arguments(pid)
        if arguments["state"] == "absent":
            continue
        if (arguments["identity"]["state"] == "present" and
                arguments["identity"]["status"] in ("zombie", "dead")):
            continue
        if arguments["state"] != "present":
            # A process may exit between identity and argv reads. Retry once,
            # retaining permission/format failures unless absence is confirmed.
            arguments = process_arguments(pid)
            if arguments["state"] == "absent" or (
                    arguments["identity"]["state"] == "present" and
                    arguments["identity"]["status"] in ("zombie", "dead")):
                continue
        if arguments["state"] != "present":
            result["errors"].append({"pid": pid, "operation": "candidate_arguments",
                                     "error": arguments["error"]})
            continue
        if _identity_key(observation) != _identity_key(arguments["identity"]):
            result["errors"].append({"pid": pid, "operation": "candidate_identity",
                                     "error": {"message": "Candidate birth changed during lookup"}})
            continue
        role = _session_process_role(arguments["arguments"], registration)
        browser_role = _browser_process_role(arguments["arguments"], registration) if role == "browser" else None
        expected_kind = "daemon" if role == "client" else role
        if browser_role == "crashpad":
            expected_kind = "crashpad"
        if expected_kind != _process_kind(arguments["identity"]):
            role = None
        if role is None and any(_identity_key(item) == _identity_key(arguments["identity"])
                                for item in observed):
            result["errors"].append({"pid": pid, "operation": "observed_ownership",
                                     "error": {"message": "Observed process remains but exact ownership is unavailable"}})
        if role is not None:
            # No unrelated argv is exposed. A final reread guards the returned PID.
            final = process_identity(pid)
            if (final["state"] != "present" or
                    _identity_key(final) != _identity_key(arguments["identity"]) or
                    final["executable"] != arguments["identity"]["executable"]):
                result["errors"].append({"pid": pid, "operation": "owned_identity_reread",
                                         "error": final["error"] or {"message": "Owned process changed"}})
                continue
            if browser_role is not None:
                final["process_role"] = browser_role
            result[role].append(final)
    if result["errors"]:
        result["state"] = "uncertain"
    return result
