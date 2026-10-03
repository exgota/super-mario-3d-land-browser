#!/usr/bin/env python3
"""Exercise owned real-browser cleanup and one-visible admission across worktrees."""
import argparse
import json
import os
from pathlib import Path
import signal
import subprocess
import sys
import time

from browser_process_identity import identity_matches, process_arguments, process_identity
from browser_session_policy import (
    BrowserSession, BrowserSessionError, ROOT, common_directory, digest, owned_path,
    owner_active, read_record, resources_absent, validate_registration, write_record,
)


def wait_for(predicate, deadline=85):
    started = time.monotonic()
    while time.monotonic() - started < deadline:
        result = predicate()
        if result:
            return result
        time.sleep(0.02)
    raise RuntimeError("Actual lifecycle control exceeded its deadline")


def cleanup_receipt(output):
    receipt = wait_for(lambda: read_record(output / "cleanup.json")
                       if (output / "cleanup.json").exists() else None)
    registration = validate_registration(output / "registration.json")
    absent, observation = resources_absent(registration)
    if receipt.get("passed") is not True or not absent:
        raise RuntimeError("Real owned-process cleanup did not pass")
    return {"cleanup": str(output / "cleanup.json"),
            "cleanup_sha256": digest(output / "cleanup.json"),
            "registration_sha256": digest(output / "registration.json"),
            "persistent_metadata_retained": receipt["persistent_metadata_retained"],
            "live_owned_processes_after": sum(len(observation[role]) for role in ("client", "daemon", "browser"))}


def child_owner(arguments):
    output = arguments.output.resolve()
    with BrowserSession(output, headed=arguments.headed, repository=arguments.repository.resolve()) as browser:
        browser.run(["open", "about:blank"])
        browser.run(["eval", "() => ({url: location.href, user_agent: navigator.userAgent})"])
        write_record(output / "owner_ready.json", {"identity": process_identity(os.getpid()),
                                                   "registration_sha256": digest(output / "registration.json")})
        deadline = time.monotonic() + 120
        while not (output / "finish_requested.json").exists():
            if time.monotonic() >= deadline:
                raise RuntimeError("Fixture owner exceeded its bounded lifetime")
            time.sleep(0.02)


class Controls:
    def __init__(self, output, peer):
        self.output = output
        self.peer = peer
        self.children = []
        self.results = []

    def directory(self, name, repository=ROOT):
        output = (self.output / name if repository == ROOT else
                  repository / "build" / (self.output.name + "_" + name))
        output.mkdir()
        return output

    def start_owner(self, name, *, headed=False, ready=True):
        output = self.directory(name)
        command = [sys.executable, str(Path(__file__).resolve()), "--owner", str(output),
                   "--repository", str(ROOT)]
        if headed:
            command.append("--headed")
        with (output.parent / (name + "_owner.log")).open("xb") as log:
            child = subprocess.Popen(command, stdout=log, stderr=log)
        identity = process_identity(child.pid)
        self.children.append((child, identity, output))
        if ready:
            wait_for(lambda: (output / "owner_ready.json").exists())
        return child, identity, output

    def signal_owned(self, identity, *, guardian_registration=None):
        if identity_matches(identity) is not True:
            raise RuntimeError("Fixture process birth identity changed; signal refused")
        arguments = process_arguments(identity["pid"])
        if arguments["state"] != "present":
            raise RuntimeError("Fixture process arguments unavailable; signal refused")
        keys = ("pid", "uid", "platform", "birth", "boot_id")
        if any(arguments["identity"][key] != identity[key] for key in keys):
            raise RuntimeError("Fixture process birth changed during argument lookup; signal refused")
        if guardian_registration:
            registration = validate_registration(guardian_registration)
            expected = [str(Path(registration["policy_source_path"]) / "browser_session_policy.py"),
                        "_guardian", str(guardian_registration)]
            if arguments["arguments"][1:4] != expected:
                raise RuntimeError("Fixture guardian ownership differs; signal refused")
        else:
            outputs = [output for child, expected, output in self.children
                       if all(expected[key] == identity[key] for key in keys)]
            if len(outputs) != 1 or arguments["arguments"][1:6] != [
                    str(Path(__file__).resolve()), "--owner", str(outputs[0]), "--repository", str(ROOT)]:
                raise RuntimeError("Fixture owner and output ownership differ; signal refused")
        if identity_matches(identity) is not True:
            raise RuntimeError("Fixture process birth changed before signal; signal refused")
        os.kill(identity["pid"], signal.SIGKILL)
        self.results.append({"control": "fixture signal", "identity": identity, "signal": "SIGKILL"})

    def normal_cases(self):
        for name in ("success", "original_failure", "command_timeout"):
            output = self.directory(name)
            observed_error = None
            try:
                with BrowserSession(output) as browser:
                    browser.run(["open", "about:blank"])
                    if name == "original_failure":
                        raise RuntimeError("Original fixture failure preserved")
                    if name == "command_timeout":
                        browser.run(["run-code", "async (page) => { await page.waitForTimeout(60000); }"], timeout=0.25)
                    else:
                        text = browser.run(["eval", "() => navigator.userAgent"])
                        if "HeadlessChrome/" not in text:
                            raise RuntimeError("Default test browser was not headless")
            except (RuntimeError, BrowserSessionError) as problem:
                observed_error = str(problem)
                if name == "success" or (name == "original_failure" and observed_error != "Original fixture failure preserved"):
                    raise
            if name != "success" and not observed_error:
                raise RuntimeError("Expected actual failure did not occur")
            if name == "command_timeout":
                commands = json.loads((output / "commands.json").read_text())
                if commands[-1]["command"][3] != "run-code" or commands[-1]["timed_out"] is not True:
                    raise RuntimeError("Actual command receipt does not establish the expected timeout")
            receipt = cleanup_receipt(output)
            if not receipt["persistent_metadata_retained"]:
                raise RuntimeError("Owned persistent metadata was not retained")
            self.results.append({"control": name, "passed": True, "error": observed_error, **receipt})

    def owner_death(self):
        child, identity, output = self.start_owner("dead_owner")
        registration = validate_registration(output / "registration.json")
        absent, before = resources_absent(registration)
        if absent or not before["daemon"] or not before["browser"]:
            raise RuntimeError("Actual persistent context was not live before owner death")
        self.signal_owned(identity)
        child.wait(timeout=10)
        self.results.append({"control": "dead owner, live persistent context", "passed": True,
                             **cleanup_receipt(output)})

    def partial_launch(self):
        child, identity, output = self.start_owner("partial_launch", ready=False)
        registration = wait_for(lambda: validate_registration(output / "registration.json")
                                if (output / "guardian_ready.json").exists() else None)

        def incomplete_context():
            if child.poll() is not None:
                raise RuntimeError("Fixture owner exited before partial-launch observation")
            if Path(registration["session_metadata_path"]).exists():
                raise RuntimeError("Context completed before partial-launch observation")
            absent, observation = resources_absent(registration)
            return observation if not absent and observation["client"] and observation["daemon"] else None

        before = wait_for(incomplete_context, deadline=15)
        write_record(output / "partial_launch_observation.json", before)
        self.signal_owned(identity)
        child.wait(timeout=10)
        self.results.append({"control": "dead owner during actual launch before metadata", "passed": True,
                             "observation_sha256": digest(output / "partial_launch_observation.json"),
                             **cleanup_receipt(output)})

    def abandoned_recovery(self):
        child, identity, output = self.start_owner("dead_supervisor_and_owner")
        guardian = read_record(output / "guardian_ready.json")["identity"]
        self.signal_owned(guardian, guardian_registration=output / "registration.json")
        self.signal_owned(identity)
        child.wait(timeout=10)
        wait_for(lambda: owner_active(guardian) is False)
        registration = validate_registration(output / "registration.json")
        absent, _ = resources_absent(registration)
        if absent or (output / "cleanup.json").exists():
            raise RuntimeError("Abandoned persistent context did not survive for the recovery control")
        next_output = self.directory("abandoned_recovery_next")
        with BrowserSession(next_output) as browser:
            if not resources_absent(registration)[0]:
                raise RuntimeError("New admission did not recover the abandoned owned context")
            browser.run(["open", "about:blank"])
        self.results.append({"control": "next-launch recovery after both processes die", "passed": True,
                             "next_cleanup": cleanup_receipt(next_output), **cleanup_receipt(output)})

    def visible_admission(self):
        child, identity, output = self.start_owner("single_visible_preview", headed=True)
        registration = validate_registration(output / "registration.json")
        _, before = resources_absent(registration)
        original_births = {(item["pid"], json.dumps(item["birth"], sort_keys=True)) for item in before["browser"]}
        refusal = self.directory("second_visible_refusal", self.peer)
        try:
            BrowserSession(refusal, headed=True, repository=self.peer)
        except BrowserSessionError as problem:
            error = str(problem)
            if "already visible" not in error:
                raise
        else:
            raise RuntimeError("A second visible preview was admitted")
        if (refusal / "registration.json").exists() or (refusal / "commands.json").exists():
            raise RuntimeError("Refused visible context reached prospective registration or CLI launch")
        next_output = self.directory("headless_with_visible", self.peer)
        with BrowserSession(next_output, repository=self.peer) as browser:
            browser.run(["open", "about:blank"])
            text = browser.run(["eval", "() => navigator.userAgent"])
            if "HeadlessChrome/" not in text or owner_active(identity) is not True:
                raise RuntimeError("Headless admission changed the active visible owner")
            _, after = resources_absent(registration)
            remaining = {(item["pid"], json.dumps(item["birth"], sort_keys=True)) for item in after["browser"]}
            if not original_births.issubset(remaining):
                raise RuntimeError("Headless admission removed an active visible resource")
        self.results.append({"control": "cross-worktree single-visible refusal and active preservation", "passed": True,
                             "refusal": error, "headless_cleanup": cleanup_receipt(next_output)})
        self.signal_owned(identity)
        child.wait(timeout=10)
        replacement = self.directory("replacement_visible", self.peer)
        with BrowserSession(replacement, headed=True, repository=self.peer) as browser:
            if not resources_absent(registration)[0]:
                raise RuntimeError("Replacement visible preview preceded abandoned-context cleanup")
            browser.run(["open", "about:blank"])
        self.results.append({"control": "dead visible owner cleaned before replacement admission", "passed": True,
                             "replacement_cleanup": cleanup_receipt(replacement), **cleanup_receipt(output)})

    def finish_children(self):
        failures = []
        for child, identity, output in self.children:
            try:
                if child.poll() is None:
                    write_record(output / "finish_requested.json", {"requested": True})
                    try:
                        child.wait(timeout=85)
                    except subprocess.TimeoutExpired:
                        self.signal_owned(identity)
                        child.wait(timeout=10)
                if (output / "registration.json").exists():
                    registration = validate_registration(output / "registration.json")
                    if owner_active(registration["owner"]) is False:
                        from browser_session_policy import recover_abandoned
                        recover_abandoned(Path(registration["registry_path"]))
            except Exception as problem:
                failures.append(problem)
        if failures:
            raise ExceptionGroup("Owned fixture cleanup failed", failures)


def keep_open_refusals(output):
    names = ("execution", "input_identity", "button_capture", "circle_pad_capture", "frame_output", "touch_capture")
    result = []
    for name in names:
        destination = output / ("keep_open_" + name)
        command = [sys.executable, str(ROOT / "tools/static_recompiler" / ("verify_browser_" + name + ".py")),
                   "http://127.0.0.1:1", str(destination), "--keep-open"]
        if name != "input_identity":
            command += ["--dump", "/absent_owned_fixture", "--server-output", "/absent_server_fixture"]
        if name in ("execution", "frame_output"):
            command += ["--reference", "/absent_reference_fixture"]
        if name == "button_capture":
            command += ["--input-method", "neutral"]
        process = subprocess.run(command, capture_output=True, text=True, timeout=15)
        if process.returncode == 0 or destination.exists() or not (
                "--keep-open is refused before launch" in process.stderr or
                (name == "input_identity" and "unrecognized arguments: --keep-open" in process.stderr)):
            raise RuntimeError("Verifier keep-open request did not refuse before launch")
        (output / ("keep_open_" + name + ".stderr")).write_text(process.stderr)
        result.append({"verifier": name, "status": process.returncode, "output_created": destination.exists()})
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("output", type=Path, help="absent ignored proof directory")
    parser.add_argument("--peer-repository", type=Path, help="a second worktree of the same repository")
    parser.add_argument("--owner", action="store_true", help=argparse.SUPPRESS)
    parser.add_argument("--repository", type=Path, default=ROOT, help=argparse.SUPPRESS)
    parser.add_argument("--headed", action="store_true", help=argparse.SUPPRESS)
    arguments = parser.parse_args()
    if arguments.owner:
        child_owner(arguments)
        return
    output = arguments.output.resolve()
    if output.exists() or not output.is_relative_to(ROOT / "build"):
        raise ValueError("An absent ignored proof directory is required")
    if arguments.peer_repository is None:
        raise ValueError("A second worktree is required for the admission control")
    peer = owned_path(arguments.peer_repository.resolve(), directory=True)
    if peer == ROOT or common_directory(peer) != common_directory(ROOT):
        raise ValueError("Admission controls need two different worktrees with one Git common directory")
    output.mkdir()
    sources = {str(Path(__file__).resolve()): digest(__file__)}
    for name in ("browser_session_policy.py", "browser_process_identity.py"):
        source = Path(__file__).with_name(name).resolve()
        sources[str(source)] = digest(source)
    controls = Controls(output, peer)
    failure = None
    try:
        refusals = keep_open_refusals(output)
        controls.normal_cases()
        controls.owner_death()
        controls.partial_launch()
        controls.abandoned_recovery()
        controls.visible_admission()
    except BaseException as problem:
        failure = problem
        raise
    finally:
        try:
            controls.finish_children()
        except BaseException as problem:
            if failure is None:
                raise
            failure.add_note("Fixture cleanup also failed: " + str(problem))
    if any(digest(path) != seal for path, seal in sources.items()):
        raise RuntimeError("Verification sources changed during actual controls")
    result = {"passed": True, "sources": sources, "keep_open_refusals": refusals,
              "controls": controls.results, "platform": sys.platform,
              "scope": "Actual registered owned Chrome contexts on this host. One visible fixture at a time. "
                       "No personal browser attachment, machine-wide cleanup, Linux execution or game replay credit."}
    write_record(output / "result.json", result)
    print(json.dumps({"passed": True, "result": str(output / "result.json"),
                      "result_sha256": digest(output / "result.json")}))


if __name__ == "__main__":
    main()
