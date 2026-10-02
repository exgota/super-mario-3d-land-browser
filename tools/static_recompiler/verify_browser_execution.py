#!/usr/bin/env python3
"""Run the actual local browser capture, compare its exported bytes and inspect displayed pixels."""
import argparse
import atexit
import json
from pathlib import Path
import re
import subprocess
import sys
import time
from urllib.parse import urlsplit

from audit_webassembly_platform import digest

ROOT = Path(__file__).resolve().parents[2]
CLI_PACKAGE = "@playwright/cli@0.1.22"


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("url")
    parser.add_argument("output", type=Path, help="absent ignored directory for commands, screenshots and receipts")
    parser.add_argument("--dump", type=Path, required=True)
    parser.add_argument("--server-output", type=Path, required=True)
    parser.add_argument("--reference", type=Path, required=True)
    parser.add_argument("--first-swap", action="store_true")
    parser.add_argument("--headed", action="store_true")
    parser.add_argument("--keep-open", action="store_true", help="leave only this verification browser open for inspection")
    parser.add_argument("--exercise-controls", action="store_true", help="verify file rejection, cancellation and recovery before the real capture")
    parser.add_argument("--timeout-seconds", type=int, default=360)
    args = parser.parse_args()
    url = urlsplit(args.url)
    output, server_output = args.output.resolve(), args.server_output.resolve()
    if url.scheme != "http" or url.hostname != "127.0.0.1" or url.username or url.password or url.query or url.fragment:
        raise ValueError("Verification must use the local preview server")
    if (not output.is_relative_to(ROOT / "build") or output.exists() or
        not server_output.is_relative_to(ROOT / "build") or not server_output.is_dir()):
        raise ValueError("Own ignored output directories are required")
    if not 1 <= args.timeout_seconds <= 7200:
        raise ValueError("Invalid external browser deadline")
    dump = args.dump.resolve()
    protected_dump = {"path": str(dump), "sha256": digest(dump), "bytes": dump.stat().st_size}
    output.mkdir(parents=True)
    commands = []
    session = "root-browser-" + output.name.replace("_", "-")
    cli = ["npx", "--yes", "--package", CLI_PACKAGE, "playwright-cli", "-s=" + session]

    def run(arguments, timeout=60):
        index = len(commands)
        command = [*cli, *arguments]
        started = time.monotonic()
        timed_out = False
        try:
            result = subprocess.run(command, cwd=output, text=True, capture_output=True, timeout=timeout)
            status_code, standard_output, standard_error = result.returncode, result.stdout, result.stderr
        except subprocess.TimeoutExpired as failure:
            timed_out = True
            status_code = None
            def decode(value):
                return value.decode(errors="replace") if isinstance(value, bytes) else value or ""
            standard_output, standard_error = decode(failure.stdout), decode(failure.stderr)
        stdout, stderr = output / f"command_{index}_stdout.log", output / f"command_{index}_stderr.log"
        stdout.write_text(standard_output)
        stderr.write_text(standard_error)
        commands.append({"command": command, "status": status_code, "timed_out": timed_out, "stdout": str(stdout),
                         "stderr": str(stderr), "elapsed_seconds": time.monotonic() - started})
        (output / "commands.json").write_text(json.dumps(commands, indent=2) + "\n")
        if timed_out or status_code or "### Error" in standard_output:
            raise RuntimeError(f"Actual browser command refused; see {stdout} and {stderr}")
        return standard_output

    def evaluate(code):
        text = run(["eval", code])
        marker = "### Result\n"
        if marker not in text:
            raise RuntimeError("Browser evaluation has no result")
        return json.JSONDecoder().raw_decode(text.split(marker, 1)[1].lstrip())[0]

    opened = ["open", args.url]
    if args.headed:
        opened.append("--headed")
    run(opened)
    if not args.keep_open:
        atexit.register(lambda: run(["close"]))
    run(["snapshot"])
    controls = {}
    if args.exercise_controls:
        fixture = output / "invalid_size.3ds"
        fixture.write_bytes(b"invalid-size-control\n")
        action = "async (page) => { await page.waitForFunction(() => document.body.dataset.captureState === 'ready'); "
        action += "await page.getByLabel('Game file', {exact: true}).setInputFiles(" + json.dumps(str(fixture)) + "); "
        action += "await page.getByRole('button', {name: 'Run preview', exact: true}).click(); }"
        run(["run-code", action])
        controls["invalid_size"] = evaluate("() => ({state: document.body.dataset.captureState, error: document.querySelector('#capture-error').textContent, "
            "input_enabled: !document.querySelector('#game-file').disabled, run_enabled: !document.querySelector('#run-preview').disabled})")
        if (controls["invalid_size"]["state"] != "failed" or "different size" not in controls["invalid_size"]["error"]
                or not controls["invalid_size"]["input_enabled"] or not controls["invalid_size"]["run_enabled"]):
            raise RuntimeError("Actual invalid-file recovery refused")
        action = "async (page) => { await page.getByLabel('Game file', {exact: true}).setInputFiles(" + json.dumps(str(dump)) + "); "
        action += "await page.getByRole('button', {name: 'Run preview', exact: true}).click(); "
        action += "await page.getByRole('button', {name: 'Stop preview', exact: true}).click(); }"
        run(["run-code", action])
        time.sleep(2)
        controls["stopped_download"] = evaluate("() => ({state: document.body.dataset.captureState, error: document.querySelector('#capture-error').textContent, "
            "input_enabled: !document.querySelector('#game-file').disabled, run_enabled: !document.querySelector('#run-preview').disabled, "
            "screens_hidden: [...document.querySelectorAll('canvas')].every(canvas => canvas.hidden)})")
        if (controls["stopped_download"]["state"] != "failed" or "stopped" not in controls["stopped_download"]["error"]
                or not all(controls["stopped_download"][key] for key in ("input_enabled", "run_enabled", "screens_hidden"))):
            raise RuntimeError("Actual stopped-session recovery refused")
        (output / "control_states.json").write_text(json.dumps(controls, indent=2) + "\n")
    action = "async (page) => { await page.waitForFunction(() => ['ready', 'failed'].includes(document.body.dataset.captureState)); "
    action += "await page.getByLabel('Game file', {exact: true}).setInputFiles(" + json.dumps(str(dump)) + "); "
    action += "await page.getByRole('button', {name: 'Run preview', exact: true}).click(); }"
    run(["run-code", action])
    started = time.monotonic()
    states = []
    while True:
        state = evaluate("() => ({state: document.body.dataset.captureState, identifier: document.body.dataset.captureIdentifier, "
                         "status: document.querySelector('#capture-status').textContent, "
                         "error: document.querySelector('#capture-error').textContent, isolated: crossOriginIsolated})")
        states.append({**state, "elapsed_seconds": time.monotonic() - started})
        (output / "observed_states.json").write_text(json.dumps(states, indent=2) + "\n")
        if state.get("state") == "complete":
            break
        if state.get("state") == "failed" or time.monotonic() - started > args.timeout_seconds:
            raise RuntimeError(f"Actual browser did not complete: {state}")
        print(json.dumps(state), flush=True)
        time.sleep(10)
    identifier = state["identifier"]
    if not re.fullmatch(r"capture_[0-9a-f]{32}", identifier) or state["isolated"] is not True:
        raise RuntimeError("Actual browser identity/isolation differs")
    receipt_path = server_output / f"{identifier}_receipt.json"
    receipt = json.loads(receipt_path.read_text())
    capture = server_output / identifier
    if receipt.get("passed") is not True or any(digest(path) != expected for path, expected in receipt["protected_inputs"].items()):
        raise RuntimeError("Actual exported capture input seal differs")
    manifest_path = server_output / f"{identifier}_manifest.json"
    manifest = json.loads(manifest_path.read_text())
    expected_directories = sorted(manifest["directories"])
    if sorted(str(path.relative_to(capture)) for path in capture.rglob("*") if path.is_dir()) != expected_directories:
        raise RuntimeError("Actual exported directory inventory differs")
    for item in manifest["files"]:
        path = capture / item["relative_path"]
        if path.stat().st_size != item["size"] or digest(path) != receipt["files"][item["relative_path"]]:
            raise RuntimeError("Actual exported file identity differs")
    initial = args.reference.resolve() / "initial_user_state"
    recorded = capture / "initial_user_state"
    initial_directories = sorted(str(path.relative_to(initial)) for path in initial.rglob("*") if path.is_dir())
    recorded_directories = sorted(str(path.relative_to(recorded)) for path in recorded.rglob("*") if path.is_dir())
    initial_files = {str(path.relative_to(initial)): digest(path) for path in initial.rglob("*") if path.is_file()}
    recorded_files = {str(path.relative_to(recorded)): digest(path) for path in recorded.rglob("*") if path.is_file()}
    # The unchanged host initializes/truncates this diagnostic log before it
    # records initial_user_state. Preserve its bytes, but do not label host
    # wall times and paths as console state or normalize them for comparison.
    log_path = "log/reference_capture.log"
    if log_path not in initial_files or log_path not in recorded_files:
        raise RuntimeError("Actual initial host diagnostic log is absent")
    guest_initial = {path: value for path, value in initial_files.items() if path != log_path}
    guest_recorded = {path: value for path, value in recorded_files.items() if path != log_path}
    if initial_directories != recorded_directories or guest_initial != guest_recorded:
        raise RuntimeError("Actual recorded initial user tree differs from the oracle")
    snapshot_identity = {"directories_exact": True, "guest_files_exact": True,
                         "directory_count": len(initial_directories), "guest_file_count": len(guest_initial),
                         "preserved_host_log": {"path": log_path, "reference_sha256": initial_files[log_path],
                                                "browser_sha256": recorded_files[log_path],
                                                "reason": "Unchanged host logging initialization precedes snapshot recording."}}
    comparator = ROOT / "tools/static_recompiler" / ("compare_gpu_capture.py" if args.first_swap else "compare_rendered_capture.py")
    comparison_path = output / "comparison.json"
    comparison_command = [sys.executable, str(comparator), str(args.reference.resolve()), str(capture),
                          "--report", str(comparison_path)]
    with (output / "comparison.log").open("wb") as stream:
        comparison = subprocess.run(comparison_command, stdout=stream, stderr=subprocess.STDOUT, timeout=60)
    if comparison.returncode or json.loads(comparison_path.read_text()).get("passed") is not True:
        raise RuntimeError("Actual browser differential comparison refused")
    pixels = {}
    capture_viewports = {}
    if not args.first_swap:
        pixels = evaluate("async () => { const result = {}; for (const name of ['top-screen', 'bottom-screen']) { "
            "const canvas = document.getElementById(name); const rgba = canvas.getContext('2d').getImageData(0, 0, canvas.width, canvas.height).data; "
            "const hash = await crypto.subtle.digest('SHA-256', rgba); result[name] = {width: canvas.width, height: canvas.height, hidden: canvas.hidden, "
            "sha256: [...new Uint8Array(hash)].map(value => value.toString(16).padStart(2, '0')).join('')}; } return result; }")
        for name, screen in pixels.items():
            payload = "rendered_screen_0.rgba" if name == "top-screen" else "rendered_screen_2.rgba"
            if screen["hidden"] or screen["sha256"] != receipt["files"][payload]:
                raise RuntimeError("Displayed browser canvas bytes differ from exported original RGBA")
        for name, width, height in (("desktop", 1440, 1080), ("mobile", 390, 1000)):
            code = "async (page) => { await page.setViewportSize({width: " + str(width) + ", height: " + str(height) + "}); "
            code += "await page.evaluate(() => document.fonts.ready); await page.screenshot({path: " + json.dumps(str(output / f"{name}.png")) + ", fullPage: true, animations: 'disabled'}); }"
            run(["run-code", code])
            screenshot = (output / f"{name}.png").read_bytes()
            if screenshot[:8] != b"\x89PNG\r\n\x1a\n":
                raise RuntimeError("Actual screenshot is not PNG")
            actual_width, actual_height = int.from_bytes(screenshot[16:20], "big"), int.from_bytes(screenshot[20:24], "big")
            if actual_width != width or actual_height < height:
                raise RuntimeError("Actual full-page screenshot dimensions differ from the requested viewport")
            capture_viewports[name] = {"requested_width": width, "requested_height": height,
                                      "screenshot_width": actual_width, "screenshot_height": actual_height,
                                      "document": evaluate("() => ({inner_width: innerWidth, inner_height: innerHeight, "
                                          "client_width: document.documentElement.clientWidth, scroll_height: document.documentElement.scrollHeight, "
                                          "fonts_loaded: document.fonts.check('400 16px \"IBM Plex Sans\"') && document.fonts.check('600 44px \"IBM Plex Sans\"')})")}
    if digest(dump) != protected_dump["sha256"] or dump.stat().st_size != protected_dump["bytes"]:
        raise RuntimeError("Readonly owned dump changed")
    result = {"passed": True, "capture": str(capture), "browser": evaluate("() => navigator.userAgent"),
              "states": states, "pixels": pixels, "capture_viewports": capture_viewports,
              "control_states": controls, "protected_dump": protected_dump,
              "snapshot_identity": snapshot_identity,
              "capture_receipt_sha256": digest(receipt_path), "capture_manifest_sha256": digest(manifest_path),
              "comparison_command": comparison_command, "comparison_sha256": digest(comparison_path),
              "comparator_sha256": digest(comparator), "source_sha256": digest(Path(__file__)),
              "scope": "Actual bounded browser startup, complete directory and guest-file preservation with raw host logs retained, strict raw differential replay "
                       "and optional displayed frame bytes. No continuous gameplay, browser audio/input or World 1-1 claim."}
    (output / "result.json").write_text(json.dumps(result, indent=2) + "\n")
    print(json.dumps({"passed": True, "capture": str(capture), "pixels": pixels, "scope": result["scope"]}), flush=True)


if __name__ == "__main__":
    main()
