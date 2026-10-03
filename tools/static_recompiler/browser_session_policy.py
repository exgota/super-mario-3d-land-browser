#!/usr/bin/env python3
"""Own project test browsers through process-bound leases and independent cleanup."""
import argparse
import atexit
from contextlib import contextmanager
import fcntl
import hashlib
import json
import os
from pathlib import Path
import re
import secrets
import select
import shutil
import signal
import stat
import subprocess
import sys
import time
from urllib.parse import urlsplit

from browser_process_identity import identity_matches, owned_session_processes, process_identity

ROOT = Path(__file__).resolve().parents[2]
CLI_VERSION = "0.1.22"
CORE_VERSION = "1.64.0-alpha-1790635538000"
MAXIMUM_RECORD_BYTES = 65536
MAXIMUM_HISTORY_BYTES = 1024 * 1024
MAXIMUM_REGISTRATIONS = 512
SOURCE_SEALS = {
    "playwright-cli.js": "fda252270793401d2856530a5f503adf7fc326fc07bed97e413138f67c50662b",
    "package.json": "1c545fd0ee14db9faed985aebfd205f9d6c2b2da5fbc8d142a7af8e26ee9c510",
}
CORE_SEALS = {
    "package.json": "2bd07b7443af06d1b836219e64f7fb40b722ee7764c9e1b1cc11147a601083fb",
    "lib/tools/cli-client/registry.js": "4976cd5b2b3a25572cc1a4b5f055a756dacd4cc1b19d48987ee3253428e759b8",
    "lib/tools/cli-client/session.js": "8c3f9aff3fab537771cb560e1bad3e417e16b3315b7a3dc10a305faeca3f6024",
    "lib/tools/cli-client/program.js": "7f661c00acf56f894a93d32917aa3e2af7d339f60be2cc79fe9d71b6605e51fc",
    "lib/entry/cliDaemon.js": "062f256fb29a7bf91793a014b9c3f1f9cf2dc6b12e40a8a2ebba88a630ae16a0",
    "lib/coreBundle.js": "cd6730be1bbcff00771a0fc1390f200306d28428ce68e305420ca623cb39cdeb",
}


class BrowserSessionError(RuntimeError):
    """A session cannot launch or finish under the mandatory ownership policy."""


def digest(path):
    with Path(path).open("rb") as source:
        return hashlib.file_digest(source, "sha256").hexdigest()


def owned_path(path, *, directory=False):
    path = Path(path)
    if not path.is_absolute() or path != path.resolve():
        raise BrowserSessionError("Session path is not canonical")
    for component in (path, *path.parents):
        if component.is_symlink():
            raise BrowserSessionError("Session path contains a symbolic link")
    information = path.stat()
    if information.st_uid != os.getuid() or (directory and not stat.S_ISDIR(information.st_mode)):
        raise BrowserSessionError("Session path ownership or type differs")
    if not directory and not stat.S_ISREG(information.st_mode):
        raise BrowserSessionError("Session record is not a regular file")
    return path


def read_record(path, *, maximum_bytes=MAXIMUM_RECORD_BYTES):
    path = owned_path(path)
    if path.stat().st_size > maximum_bytes:
        raise BrowserSessionError("Session record exceeds its bound")
    value = json.loads(path.read_text())
    if not isinstance(value, dict):
        raise BrowserSessionError("Session record must be an object")
    return value


def write_record(path, value):
    path = Path(path)
    temporary = path.with_name(path.name + "." + secrets.token_hex(8) + ".temporary")
    with temporary.open("x") as stream:
        os.chmod(temporary, 0o600)
        stream.write(json.dumps(value, indent=2) + "\n")
        stream.flush()
        os.fsync(stream.fileno())
    temporary.replace(path)


def common_directory(repository):
    result = subprocess.run(["git", "rev-parse", "--path-format=absolute", "--git-common-dir"],
                            cwd=repository, capture_output=True, text=True, timeout=10, check=True)
    return owned_path(Path(result.stdout.strip()).resolve(), directory=True)


@contextmanager
def record_lock(path, *, nonblocking=False):
    descriptor = os.open(path, os.O_RDWR | os.O_CREAT | os.O_NOFOLLOW, 0o600)
    try:
        information = os.fstat(descriptor)
        if information.st_uid != os.getuid() or not stat.S_ISREG(information.st_mode):
            raise BrowserSessionError("Session lock ownership differs")
        fcntl.flock(descriptor, fcntl.LOCK_EX | (fcntl.LOCK_NB if nonblocking else 0))
        yield descriptor
    finally:
        os.close(descriptor)


def verified_cli(package_directory=None):
    candidates = ([Path(package_directory)] if package_directory else
                  sorted((Path.home() / ".npm/_npx").glob("*/node_modules/@playwright/cli")))
    if len(candidates) > 512:
        raise BrowserSessionError("Installed CLI candidate inventory exceeds its bound")
    for candidate in candidates:
        candidate = candidate.resolve()
        core = candidate.parent.parent / "playwright-core"
        try:
            if (all(digest(candidate / name) == seal for name, seal in SOURCE_SEALS.items()) and
                    all(digest(core / name) == seal for name, seal in CORE_SEALS.items())):
                node = shutil.which("node")
                if not node:
                    raise BrowserSessionError("Node executable is unavailable")
                return {"entry": str(candidate / "playwright-cli.js"),
                        "daemon": str(core / "lib/entry/cliDaemon.js"),
                        "node": str(Path(node).resolve()), "core_version": CORE_VERSION}
        except OSError:
            continue
    raise BrowserSessionError("Install pinned @playwright/cli@0.1.22 before testing; source closure differs")


def child_environment(registration):
    environment = {key: value for key, value in os.environ.items()
                   if not key.startswith(("PLAYWRIGHT_", "PWTEST_")) and
                   key not in ("NODE_OPTIONS", "NODE_PATH")}
    # This pinned upstream hook avoids reading or changing personal CLI settings.
    environment["PWTEST_CLI_GLOBAL_CONFIG"] = registration["global_configuration_path"]
    environment["NO_UPDATE_NOTIFIER"] = "1"
    return environment


def daemon_directory(output):
    namespace = hashlib.sha1(str(output).encode()).hexdigest()[:16]
    if sys.platform == "darwin":
        base = Path.home() / "Library/Caches/ms-playwright/daemon"
    elif sys.platform == "linux":
        base = Path(os.environ.get("XDG_CACHE_HOME", str(Path.home() / ".cache"))) / "ms-playwright/daemon"
    else:
        raise BrowserSessionError("Browser ownership probes support Darwin and Linux only")
    return base / namespace


def validate_registration(path, registry=None):
    registration = read_record(path)
    required = {"schema_version", "nonce", "session_name", "uid", "repository", "output",
                "common_directory", "registry_path", "profile_path", "config_path",
                "global_configuration_path", "session_metadata_path", "owner", "headed",
                "cli_entry_path", "cli_daemon_path", "node_path", "core_version",
                "source_sha256", "policy_source_path", "config_sha256", "created"}
    if set(registration) != required or registration["schema_version"] != 1:
        raise BrowserSessionError("Session registration schema differs")
    nonce = registration["nonce"]
    if (not isinstance(nonce, str) or not re.fullmatch(r"[0-9a-f]{24}", nonce) or
            registration["session_name"] != "root-browser-" + nonce or
            registration["uid"] != os.getuid() or type(registration["headed"]) is not bool or
            registration["core_version"] != CORE_VERSION):
        raise BrowserSessionError("Session registration identity differs")
    repository = owned_path(registration["repository"], directory=True)
    output = owned_path(registration["output"], directory=True)
    common = common_directory(repository)
    actual_registry = owned_path(common / "browser_session_registry", directory=True)
    if (not output.is_relative_to(repository / "build") or
            registration["common_directory"] != str(common) or
            registration["registry_path"] != str(actual_registry) or
            (registry is not None and Path(registry) != actual_registry) or
            Path(path) != output / "registration.json"):
        raise BrowserSessionError("Session registration project boundary differs")
    expected = {"profile_path": output / "browser_profile", "config_path": output / "browser_configuration.json",
                "global_configuration_path": output / "global_configuration",
                "session_metadata_path": daemon_directory(output) / (registration["session_name"] + ".session")}
    for key, value in expected.items():
        if registration[key] != str(value):
            raise BrowserSessionError("Session registration resource boundary differs")
    for name in ("browser_profile", ".playwright", "global_configuration", "policy_source"):
        owned_path(output / name, directory=True)
    if registration["policy_source_path"] != str(output / "policy_source"):
        raise BrowserSessionError("Frozen policy source path differs")
    expected_sources = {name: digest(output / "policy_source" / name)
                        for name in ("browser_session_policy.py", "browser_process_identity.py")}
    if registration["source_sha256"] != expected_sources:
        raise BrowserSessionError("Frozen cleanup source identity differs")
    installed = verified_cli(Path(registration["cli_entry_path"]).parent)
    if any(registration[key] != installed[field] for key, field in (
            ("cli_entry_path", "entry"), ("cli_daemon_path", "daemon"), ("node_path", "node"))):
        raise BrowserSessionError("Registered browser CLI executable closure differs")
    owned_path(registration["node_path"])
    if not os.access(registration["node_path"], os.X_OK):
        raise BrowserSessionError("Registered Node executable is unavailable")
    owned_path(output / "browser_configuration.json")
    if digest(output / "browser_configuration.json") != registration["config_sha256"]:
        raise BrowserSessionError("Prospective browser configuration identity differs")
    expected_configuration = {"browser": {"browserName": "chromium", "isolated": False,
                                         "userDataDir": registration["profile_path"],
                                         "remoteEndpoint": None, "cdpEndpoint": None,
                                         "launchOptions": {"channel": "chrome", "headless": not registration["headed"]}},
                              "extension": False, "outputDir": str(output / ".playwright-cli")}
    if read_record(output / "browser_configuration.json") != expected_configuration:
        raise BrowserSessionError("Prospective browser configuration contract differs")
    if (not isinstance(registration["owner"], dict) or
            registration["owner"].get("state") != "present" or
            registration["owner"].get("uid") != registration["uid"]):
        raise BrowserSessionError("Session owner identity is incomplete")
    return registration


def owner_active(identity):
    current = process_identity(identity["pid"])
    same = identity_matches(identity)
    if same is None or current["state"] == "unknown":
        return None
    return bool(same and current.get("status") != "zombie")


def session_metadata(registration):
    path = Path(registration["session_metadata_path"])
    if not path.exists():
        return None
    value = read_record(path)
    browser = value.get("browser", {})
    if (value.get("name") != registration["session_name"] or
            value.get("version") != CORE_VERSION or value.get("attached", False) is not False or
            value.get("workspaceDir") != registration["output"] or
            browser.get("userDataDir") != registration["profile_path"] or
            browser.get("browserName") != "chromium" or
            browser.get("launchOptions", {}).get("headless") is not (not registration["headed"]) or
            value.get("cli", {}).get("persistent") is not True):
        raise BrowserSessionError("Actual session metadata does not prove the registered owned profile")
    socket = Path(value.get("socketPath", ""))
    if not socket.is_absolute() or socket.is_symlink():
        raise BrowserSessionError("Actual session socket path differs")
    if socket.exists():
        information = socket.stat()
        if information.st_uid != registration["uid"] or not stat.S_ISSOCK(information.st_mode):
            raise BrowserSessionError("Actual session socket ownership or type differs")
    return value


def observe_processes(registration, *, record_history=True):
    history_path = Path(registration["output"]) / "process_history.json"
    if not record_history:
        history = (read_record(history_path, maximum_bytes=MAXIMUM_HISTORY_BYTES).get("processes", [])
                   if history_path.exists() else [])
        return owned_session_processes({**registration, "observed_processes": history})
    with record_lock(history_path.with_suffix(".lock")):
        history = (read_record(history_path, maximum_bytes=MAXIMUM_HISTORY_BYTES).get("processes", [])
                   if history_path.exists() else [])
        if not isinstance(history, list) or len(history) > 1024:
            raise BrowserSessionError("Owned process history exceeds its bound")
        result = owned_session_processes({**registration, "observed_processes": history})
        for role in ("client", "daemon", "browser"):
            for identity in result[role]:
                if not any((old["pid"], old["birth"], old["boot_id"]) ==
                           (identity["pid"], identity["birth"], identity["boot_id"]) for old in history):
                    history.append(identity)
        if len(history) > 1024:
            raise BrowserSessionError("Owned process history exceeds its bound")
        write_record(history_path, {"processes": history})
        return result


def process_evidence(registration, *, record_history=True):
    result = observe_processes(registration, record_history=record_history)
    if result["state"] != "complete":
        raise BrowserSessionError("Owned browser process identity is uncertain: " +
                                  json.dumps(result.get("errors", [])))
    return result


def resources_absent(registration, *, record_history=False):
    evidence = process_evidence(registration, record_history=record_history)
    live = [item for role in ("client", "daemon", "browser") for item in evidence[role]
            if item.get("status") != "zombie"]
    return not live, evidence


def signal_owned_process(registration, target, requested_signal, receipt):
    keys = ("pid", "uid", "platform", "birth", "boot_id")

    def finished(observation):
        complete = observation["state"] == "absent" or (
            observation["state"] == "present" and observation.get("status") in ("zombie", "dead") and
            all(observation[key] == target[key] for key in keys))
        if complete:
            receipt["finished_without_signal"].append({"identity": target, "observation": observation})
            write_record(Path(registration["output"]) / "cleanup_progress.json", receipt)
        return complete

    observation = process_identity(target["pid"])
    if finished(observation):
        return
    if identity_matches(target) is not True:
        if finished(process_identity(target["pid"])):
            return
        raise BrowserSessionError("Cleanup target birth identity changed or became unknown; signal refused")
    current = process_evidence(registration)
    if not any(all(item[key] == target[key] for key in keys) for role in ("client", "daemon", "browser")
               for item in current[role]):
        if finished(process_identity(target["pid"])):
            return
        raise BrowserSessionError("Cleanup target exact ownership changed; signal refused")
    if identity_matches(target) is not True:
        if finished(process_identity(target["pid"])):
            return
        raise BrowserSessionError("Cleanup target birth changed before signal; signal refused")
    try:
        os.kill(target["pid"], requested_signal)
    except OSError as problem:
        receipt["signal_errors"].append({"identity": target, "signal": requested_signal.name,
                                         "errno": problem.errno, "error": str(problem)})
        write_record(Path(registration["output"]) / "cleanup_progress.json", receipt)
        if isinstance(problem, ProcessLookupError) and finished(process_identity(target["pid"])):
            return
        raise
    receipt["signals"].append({"identity": target, "signal": requested_signal.name})
    write_record(Path(registration["output"]) / "cleanup_progress.json", receipt)


def cleanup_registration(path, *, requested=False):
    """Close only a proven owned inactive session, preserving all profile/evidence files."""
    registration = validate_registration(path)
    output = Path(registration["output"])
    with record_lock(output / "cleanup.lock"):
        active = owner_active(registration["owner"])
        if active is None or (active and not requested):
            raise BrowserSessionError("Session owner is active or unknown; cleanup refused")
        metadata = session_metadata(registration)
        absent, before = resources_absent(registration, record_history=True)
        receipt = {"passed": False, "registration_sha256": digest(path),
                   "requested_by_owner": requested, "owner_active": active,
                   "before": before, "metadata_present": metadata is not None, "signals": [],
                   "signal_errors": [], "finished_without_signal": []}
        commands = []
        for target in before["client"]:
            if target.get("status") == "zombie":
                continue
            signal_owned_process(registration, target, signal.SIGTERM, receipt)
        if not absent and metadata is not None:
            # Named stop cannot replace another session; unique workspace/profile are checked first.
            session_metadata(registration)
            command = [registration["node_path"], registration["cli_entry_path"],
                       "-s=" + registration["session_name"], "close"]
            started = time.monotonic()
            try:
                result = subprocess.run(command, cwd=output, env=child_environment(registration),
                                        capture_output=True, text=True, timeout=45)
                stdout, stderr, status, timed_out = result.stdout, result.stderr, result.returncode, False
            except subprocess.TimeoutExpired as problem:
                def decode(value):
                    return value.decode(errors="replace") if isinstance(value, bytes) else value or ""
                stdout, stderr, status, timed_out = decode(problem.stdout), decode(problem.stderr), None, True
            (output / "cleanup_stdout.log").write_text(stdout)
            (output / "cleanup_stderr.log").write_text(stderr)
            commands.append({"command": command, "status": status, "timed_out": timed_out,
                             "elapsed_seconds": time.monotonic() - started})
        deadline = time.monotonic() + 5
        while time.monotonic() < deadline:
            absent, evidence = resources_absent(registration, record_history=True)
            if absent:
                break
            time.sleep(0.1)
        # Partial launch may never write metadata. Exact prospective daemon/profile argv
        # plus current PID/UID/birth identity authorizes only these owned resources.
        for requested_signal, interval in ((signal.SIGTERM, 10), (signal.SIGKILL, 5)):
            if absent:
                break
            targets = [item for role in ("client", "daemon", "browser") for item in evidence[role]
                       if item.get("status") != "zombie"]
            for target in targets:
                signal_owned_process(registration, target, requested_signal, receipt)
            deadline = time.monotonic() + interval
            while time.monotonic() < deadline:
                absent, evidence = resources_absent(registration, record_history=True)
                if absent:
                    break
                time.sleep(0.1)
        quiet_started = time.monotonic()
        quiet_deadline = quiet_started + 5
        while absent and time.monotonic() - quiet_started < 1:
            time.sleep(0.1)
            absent, evidence = resources_absent(registration, record_history=True)
            if time.monotonic() >= quiet_deadline:
                absent = False
                break
        receipt.update({"passed": absent, "after": evidence, "commands": commands,
                        "persistent_metadata_retained": Path(registration["session_metadata_path"]).exists(),
                        "completed": time.time()})
        write_record(output / "cleanup.json", receipt)
        if not absent:
            raise BrowserSessionError("Owned detached browser resources remain after bounded cleanup")
        return receipt


def registry_entries(registry):
    paths = sorted(Path(registry).glob("*.registration.json"))
    if len(paths) > MAXIMUM_REGISTRATIONS:
        raise BrowserSessionError("Browser registration inventory exceeds its bound")
    result = []
    for path in paths:
        entry = read_record(path)
        if set(entry) != {"registration", "sha256"}:
            raise BrowserSessionError("Registry reference schema differs")
        source = Path(entry["registration"])
        if digest(owned_path(source)) != entry["sha256"]:
            raise BrowserSessionError("Registry reference identity differs")
        result.append((source, validate_registration(source, registry)))
    return result


def recover_abandoned(registry):
    receipts = []
    for path, registration in registry_entries(registry):
        output = Path(registration["output"])
        active = owner_active(registration["owner"])
        if active is None:
            raise BrowserSessionError("Existing browser owner identity is unknown")
        requested = False
        close_request = output / "close_requested.json"
        if close_request.exists():
            token = read_record(close_request)
            if token != {"nonce": registration["nonce"], "registration_sha256": digest(path)}:
                raise BrowserSessionError("Explicit close request identity differs")
            requested = True
        if active and not requested:
            receipts.append({"registration": str(path), "state": "active, preserved"})
            continue
        guardian_path = output / "guardian_ready.json"
        guardian = read_record(guardian_path).get("identity") if guardian_path.exists() else None
        deadline = time.monotonic() + 70
        while guardian and owner_active(guardian) is True and not (output / "cleanup.json").exists():
            if time.monotonic() >= deadline:
                raise BrowserSessionError("Abandoned-session cleanup supervisor has not completed")
            time.sleep(0.1)
        previous = output / "cleanup.json"
        if previous.exists():
            receipt = read_record(previous)
            absent, observation = resources_absent(registration)
            if (receipt.get("passed") is True and receipt.get("registration_sha256") == digest(path)
                    and absent and (not guardian or owner_active(guardian) is False)):
                receipts.append({"registration": str(path), "state": "closed, absence rechecked",
                                 "cleanup_sha256": digest(previous), "observation": observation})
                continue
        receipt = cleanup_registration(path, requested=requested)
        receipts.append({"registration": str(path), "state": "closed", "cleanup": receipt})
    return receipts


def guard(registration_path, owner_pipe, visible_descriptor):
    registration = validate_registration(registration_path)
    output = Path(registration["output"])
    identity = process_identity(os.getpid())
    if identity["state"] != "present":
        raise BrowserSessionError("Cleanup supervisor identity unavailable")
    write_record(output / "guardian_ready.json", {"identity": identity, "registration_sha256": digest(registration_path)})
    requested = False
    try:
        while True:
            readable, _, _ = select.select([owner_pipe], [], [], 0.25)
            observation = observe_processes(registration)
            write_record(output / "observed_processes.json", observation)
            if readable:
                value = os.read(owner_pipe, 1)
                if value == b"C":
                    requested = True
                    break
                if value == b"":
                    break
                raise BrowserSessionError("Invalid private supervisor request")
        cleanup_registration(registration_path, requested=requested)
    except BaseException as problem:
        write_record(output / "cleanup_failure.json",
                     {"passed": False, "error": str(problem), "type": type(problem).__name__,
                      "registration_sha256": digest(registration_path), "time": time.time()})
        raise
    finally:
        os.close(owner_pipe)
        if visible_descriptor >= 0:
            os.close(visible_descriptor)


def reject_keep_open(requested):
    if requested:
        raise BrowserSessionError("BRIEF browser lifecycle policy requires test cleanup; --keep-open is refused before launch")


class BrowserSession:
    """A single explicitly owned test session; use as a context manager."""

    def __init__(self, output, *, headed=False, repository=ROOT, package_directory=None):
        self.output = owned_path(Path(output).resolve(), directory=True)
        self.repository = owned_path(Path(repository).resolve(), directory=True)
        if not self.output.is_relative_to(self.repository / "build") or type(headed) is not bool:
            raise BrowserSessionError("Browser test needs an owned ignored output and boolean visibility")
        self.closed = False
        self.opened = False
        self.commands = []
        self.visible_descriptor = -1
        self.owner_pipe = -1
        self.supervisor = None
        self.registration_path = None
        self.registration = None
        if any(self.output.iterdir()):
            raise BrowserSessionError("Browser policy needs a fresh empty test directory")
        try:
            self._initialize(headed, package_directory)
        except BaseException as problem:
            if self.owner_pipe >= 0:
                try:
                    os.write(self.owner_pipe, b"C")
                except BrokenPipeError:
                    pass
                os.close(self.owner_pipe)
                self.owner_pipe = -1
            if self.supervisor is not None:
                try:
                    self.supervisor.wait(timeout=75)
                except subprocess.TimeoutExpired:
                    problem.add_note("Pre-launch cleanup supervisor did not exit")
            if self.visible_descriptor >= 0:
                os.close(self.visible_descriptor)
                self.visible_descriptor = -1
            if self.registration and self.registration_path and self.registration_path.exists():
                write_record(self.output / "close_requested.json",
                             {"nonce": self.registration["nonce"], "registration_sha256": digest(self.registration_path)})
            write_record(self.output / "initialization_failure.json",
                         {"error": str(problem), "type": type(problem).__name__, "browser_launch_attempted": False})
            raise

    def _initialize(self, headed, package_directory):
        common = common_directory(self.repository)
        self.registry = common / "browser_session_registry"
        self.registry.mkdir(mode=0o700, exist_ok=True)
        owned_path(self.registry, directory=True)
        cli = verified_cli(package_directory)
        owner = process_identity(os.getpid())
        if owner["state"] != "present":
            raise BrowserSessionError("Browser test owner identity unavailable")
        # Short transaction protects prospective registrations; recovery never closes a live owner.
        with record_lock(self.registry / "transaction.lock"):
            recovery = recover_abandoned(self.registry)
            # Recheck active ownership and resources after obtaining the transaction.
            for path, existing in registry_entries(self.registry):
                active = owner_active(existing["owner"])
                if active is None:
                    raise BrowserSessionError("Existing owner identity became uncertain")
                if not active:
                    absent, _ = resources_absent(existing)
                    if not absent:
                        cleanup_registration(path)
            if headed:
                self.visible_descriptor = os.open(self.registry / "visible.lock",
                                                  os.O_RDWR | os.O_CREAT | os.O_NOFOLLOW, 0o600)
                information = os.fstat(self.visible_descriptor)
                if information.st_uid != os.getuid() or not stat.S_ISREG(information.st_mode):
                    raise BrowserSessionError("Shared visible-preview lock ownership or type differs")
                try:
                    fcntl.flock(self.visible_descriptor, fcntl.LOCK_EX | fcntl.LOCK_NB)
                except BlockingIOError:
                    os.close(self.visible_descriptor)
                    self.visible_descriptor = -1
                    raise BrowserSessionError("One project preview is already visible; active preview preserved")
                # A dead owner may have released the OS lease while its detached browser survives.
                for old_path, old_registration in registry_entries(self.registry):
                    if old_registration["headed"]:
                        absent, _ = resources_absent(old_registration)
                        if not absent:
                            os.close(self.visible_descriptor)
                            self.visible_descriptor = -1
                            raise BrowserSessionError("An unresolved owned visible preview prevents launch")
            nonce = secrets.token_hex(12)
            for name in (".playwright", "browser_profile", "global_configuration", "policy_source"):
                (self.output / name).mkdir(mode=0o700)
            for name in ("browser_session_policy.py", "browser_process_identity.py"):
                shutil.copyfile(Path(__file__).with_name(name), self.output / "policy_source" / name)
                os.chmod(self.output / "policy_source" / name, 0o600)
            profile = self.output / "browser_profile"
            config_path = self.output / "browser_configuration.json"
            write_record(config_path, {"browser": {"browserName": "chromium", "isolated": False,
                                                  "userDataDir": str(profile), "remoteEndpoint": None,
                                                  "cdpEndpoint": None,
                                                  "launchOptions": {"channel": "chrome", "headless": not headed}},
                                       "extension": False, "outputDir": str(self.output / ".playwright-cli")})
            registration = {"schema_version": 1, "nonce": nonce, "session_name": "root-browser-" + nonce,
                            "uid": os.getuid(), "repository": str(self.repository), "output": str(self.output),
                            "common_directory": str(common), "registry_path": str(self.registry),
                            "profile_path": str(profile), "config_path": str(config_path),
                            "global_configuration_path": str(self.output / "global_configuration"),
                            "session_metadata_path": str(daemon_directory(self.output) / ("root-browser-" + nonce + ".session")),
                            "owner": owner, "headed": headed, "cli_entry_path": cli["entry"],
                            "cli_daemon_path": cli["daemon"], "node_path": cli["node"],
                            "core_version": CORE_VERSION,
                            "source_sha256": {name: digest(self.output / "policy_source" / name)
                                              for name in ("browser_session_policy.py", "browser_process_identity.py")},
                            "policy_source_path": str(self.output / "policy_source"),
                            "config_sha256": digest(config_path),
                            "created": time.time()}
            self.registration = registration
            self.registration_path = self.output / "registration.json"
            write_record(self.registration_path, registration)
            write_record(self.registry / (nonce + ".registration.json"),
                         {"registration": str(self.registration_path), "sha256": digest(self.registration_path)})
            write_record(self.output / "recovery.json", {"registrations": recovery})
            self.registration = validate_registration(self.registration_path, self.registry)
            reader, self.owner_pipe = os.pipe()
            command = [sys.executable, str(self.output / "policy_source/browser_session_policy.py"), "_guardian",
                       str(self.registration_path), str(reader), str(self.visible_descriptor)]
            descriptors = (reader,) if self.visible_descriptor < 0 else (reader, self.visible_descriptor)
            log = (self.output / "guardian.log").open("xb")
            try:
                self.supervisor = subprocess.Popen(command, pass_fds=descriptors, start_new_session=True,
                                                   stdout=log, stderr=log)
            finally:
                log.close()
                os.close(reader)
            deadline = time.monotonic() + 10
            while not (self.output / "guardian_ready.json").exists():
                if self.supervisor.poll() is not None or time.monotonic() >= deadline:
                    raise BrowserSessionError("Cleanup supervisor did not register before launch")
                time.sleep(0.05)
        atexit.register(self.close)

    def __enter__(self):
        return self

    def __exit__(self, error_type, error, traceback):
        try:
            self.close()
        except BaseException as cleanup_error:
            if error is None:
                raise
            error.add_note("Browser cleanup also failed: " + str(cleanup_error))
        return False

    def run(self, arguments, timeout=60):
        if self.closed or not arguments or arguments[0] not in {
                "open", "eval", "run-code", "resize", "snapshot", "screenshot", "tab-list"}:
            raise BrowserSessionError("Browser command is outside the owned test session")
        if not 0 < timeout <= 7200:
            raise BrowserSessionError("Browser command deadline differs")
        if (self.supervisor is None or self.supervisor.poll() is not None or
                owner_active(read_record(self.output / "guardian_ready.json")["identity"]) is not True):
            raise BrowserSessionError("Independent cleanup supervisor is unavailable; browser command refused")
        validate_registration(self.registration_path, self.registry)
        if arguments[0] == "open":
            if self.opened or len(arguments) != 2:
                raise BrowserSessionError("A test can open only its one fresh registered context")
            url = urlsplit(arguments[1])
            if arguments[1] != "about:blank" and (
                    url.scheme != "http" or url.hostname != "127.0.0.1" or
                    url.username or url.password or url.query or url.fragment):
                raise BrowserSessionError("Owned browser tests use local project URLs")
            self.opened = True
            arguments = [*arguments, "--profile=" + self.registration["profile_path"],
                         "--config=" + self.registration["config_path"], "--browser=chrome"]
            if self.registration["headed"]:
                arguments.append("--headed")
        else:
            if not self.opened:
                raise BrowserSessionError("Browser context has not opened")
            if session_metadata(self.registration) is None:
                raise BrowserSessionError("Actual owned browser metadata is absent before command")
        command = [self.registration["node_path"], self.registration["cli_entry_path"],
                   "-s=" + self.registration["session_name"], *arguments]
        started = time.monotonic()
        timed_out = False
        try:
            child = subprocess.Popen(command, cwd=self.output, env=child_environment(self.registration),
                                     stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
            write_record(self.output / "active_command.json",
                         {"identity": process_identity(child.pid), "command": command,
                          "nonce": self.registration["nonce"], "registration_sha256": digest(self.registration_path)})
            try:
                stdout, stderr = child.communicate(timeout=timeout)
                status = child.returncode
            except subprocess.TimeoutExpired:
                child.kill()
                stdout, stderr = child.communicate()
                status, timed_out = None, True
        except subprocess.TimeoutExpired as problem:
            timed_out = True
            def decode(value):
                return value.decode(errors="replace") if isinstance(value, bytes) else value or ""
            stdout, stderr, status = decode(problem.stdout), decode(problem.stderr), None
        index = len(self.commands)
        stdout_path, stderr_path = self.output / f"command_{index}_stdout.log", self.output / f"command_{index}_stderr.log"
        stdout_path.write_text(stdout)
        stderr_path.write_text(stderr)
        self.commands.append({"command": command, "status": status, "timed_out": timed_out,
                              "stdout": str(stdout_path), "stderr": str(stderr_path),
                              "elapsed_seconds": time.monotonic() - started})
        write_record(self.output / "commands.json", self.commands)
        if timed_out or status or "### Error" in stdout:
            raise BrowserSessionError(f"Actual browser command refused; see {stdout_path}")
        metadata = session_metadata(self.registration)
        if metadata is None:
            raise BrowserSessionError("Actual owned browser metadata is absent")
        write_record(self.output / "session_metadata.json", metadata)
        process_evidence(self.registration)
        return stdout

    def close(self):
        if self.closed:
            return read_record(self.output / "cleanup.json")
        write_record(self.output / "close_requested.json",
                     {"nonce": self.registration["nonce"], "registration_sha256": digest(self.registration_path)})
        self.closed = True
        try:
            if self.owner_pipe >= 0:
                try:
                    os.write(self.owner_pipe, b"C")
                except BrokenPipeError:
                    pass
                os.close(self.owner_pipe)
                self.owner_pipe = -1
            if self.supervisor is not None:
                try:
                    self.supervisor.wait(timeout=75)
                except subprocess.TimeoutExpired as problem:
                    raise BrowserSessionError("Owned browser cleanup supervisor exceeded its deadline") from problem
            path = self.output / "cleanup.json"
            if not path.exists() or read_record(path).get("passed") is not True:
                raise BrowserSessionError("Browser test cleanup lacks verified process absence")
            return read_record(path)
        finally:
            if self.visible_descriptor >= 0:
                os.close(self.visible_descriptor)
                self.visible_descriptor = -1
            atexit.unregister(self.close)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("mode", choices=["_guardian"])
    parser.add_argument("registration", type=Path)
    parser.add_argument("owner_pipe", type=int)
    parser.add_argument("visible_descriptor", type=int)
    arguments = parser.parse_args()
    guard(arguments.registration, arguments.owner_pipe, arguments.visible_descriptor)


if __name__ == "__main__":
    main()
