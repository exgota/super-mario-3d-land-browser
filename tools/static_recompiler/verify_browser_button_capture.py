#!/usr/bin/env python3
"""Record actual browser A input; stock movie replay remains a separate check."""
import argparse
import json
from pathlib import Path
import re
import subprocess
import time
from urllib.parse import urlsplit

from audit_webassembly_platform import digest
from browser_session_policy import BrowserSession, reject_keep_open
from compare_input_capture import load_input_capture, load_movie, validate_movie_delivery, validate_observation_boundary

ROOT = Path(__file__).resolve().parents[2]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("url")
    parser.add_argument("output", type=Path)
    parser.add_argument("--dump", type=Path, required=True)
    parser.add_argument("--server-output", type=Path, required=True)
    parser.add_argument("--input-method", choices=("pointer", "keyboard", "focus-loss", "neutral"), required=True)
    parser.add_argument("--sampled-frame", type=int, default=360)
    parser.add_argument("--keep-open", action="store_true")
    parser.add_argument("--screenshots", action="store_true")
    parser.add_argument("--headed", action="store_true", help="request the shared visible-preview lease")
    args = parser.parse_args()
    reject_keep_open(getattr(args, "keep_open", False))
    url = urlsplit(args.url)
    output, server = args.output.resolve(), args.server_output.resolve()
    if (url.scheme != "http" or url.hostname != "127.0.0.1" or url.username or url.password or
            url.query or url.fragment or output.exists() or not output.is_relative_to(ROOT / "build") or
            not server.is_relative_to(ROOT / "build") or not server.is_dir()):
        raise ValueError("Fresh owned ignored output and local server required")
    configuration = json.loads((server / "configuration.json").read_text())
    options = configuration["options"]
    if (options.get("live_button_capture") is not True or options.get("input_capture") is not True or
            options.get("audio_capture") is not True or not 0 <= args.sampled_frame < options["presentation_limit"]):
        raise ValueError("Finite live-button recording profile required")
    dump = args.dump.resolve()
    if digest(dump) != configuration["inputs"]["dump"]["expected_sha256"]:
        raise ValueError("Owner dump identity differs")
    output.mkdir(parents=True)
    with BrowserSession(output, headed=args.headed) as browser:
        run = browser.run
        def result(arguments, timeout=60):
            raw = run(arguments, timeout)
            marker = "### Result\n"
            if marker not in raw:
                raise RuntimeError("Actual browser result absent")
            return json.JSONDecoder().raw_decode(raw.split(marker, 1)[1].lstrip())[0]

        run(["open", args.url])
        run(["resize", "1440", "1080"])
        action = "async (page) => { await page.waitForFunction(() => document.body.dataset.captureState === 'ready'); "
        action += "await page.getByLabel('Game file', {exact: true}).setInputFiles(" + json.dumps(str(dump)) + "); "
        action += "await page.getByRole('button', {name:'Run preview',exact:true}).click(); }"
        run(["run-code", action])
        # One browser command waits and operates the real control. External CLI
        # round trips cannot place a short press inside the finite guest window.
        action = "async (page) => { await page.waitForFunction(() => document.body.dataset.captureState === 'failed' || "
        action += "(Number(document.body.dataset.sampledRendererFrame) >= " + str(args.sampled_frame)
        action += " && !document.querySelector('#hold-a').disabled), null, {timeout:240000}); "
        action += "if (await page.evaluate(() => document.body.dataset.captureState === 'failed')) throw new Error('Capture failed'); "
        action += "const target = page.getByRole('button',{name:'Hold A',exact:true}); const before = await page.evaluate(() => ({...document.body.dataset})); "
        if args.screenshots:
            action += "await page.screenshot({path:" + json.dumps(str(output / "desktop_running.png")) + ",fullPage:true}); "
            action += "await page.setViewportSize({width:390,height:1200}); await page.screenshot({path:" + json.dumps(str(output / "mobile_running.png")) + ",fullPage:true}); await page.setViewportSize({width:1440,height:1080}); "
        if args.input_method == "pointer":
            action += "const box = await target.boundingBox(); await page.mouse.move(box.x+box.width/2,box.y+box.height/2); await page.mouse.down(); "
        elif args.input_method in ("keyboard", "focus-loss"):
            action += "await target.focus(); await page.keyboard.down('Space'); "
        if args.input_method != "neutral":
            # Outer-worker filesystem service can delay both messages until a
            # short wall-clock pulse has ended. Hold across observed device polls;
            # actual saved HID edges, never this progress, certify delivery.
            action += "await page.waitForFunction(() => document.body.dataset.buttonRequestSequence === '0' && document.body.dataset.buttonRequestAccepted === 'true',null,{timeout:30000}); "
            action += "const heldPolls = await page.evaluate(() => Number(document.body.dataset.buttonPollCount)); await page.waitForFunction(count => Number(document.body.dataset.buttonPollCount) >= count+8,heldPolls,{timeout:30000}); "
        if args.input_method == "pointer":
            action += "await page.mouse.up(); "
        elif args.input_method in ("keyboard", "focus-loss"):
            if args.input_method == "focus-loss":
                action += "await target.evaluate(element => element.blur()); "
            action += "await page.keyboard.up('Space'); "
        action += "return {before,after:await page.evaluate(() => ({...document.body.dataset})), pressed:await target.getAttribute('aria-pressed')}; }"
        delivery = result(["run-code", action], 270)
        (output / "browser_action.json").write_text(json.dumps(delivery, indent=2) + "\n")
        deadline = time.monotonic() + 240
        states = []
        while True:
            state = result(["eval", "() => ({...document.body.dataset,status:document.querySelector('#capture-status').textContent,error:document.querySelector('#capture-error').textContent,isolated:crossOriginIsolated})"])
            states.append(state)
            (output / "observed_states.json").write_text(json.dumps(states, indent=2) + "\n")
            print(json.dumps(state), flush=True)
            if state.get("captureState") == "complete":
                break
            if state.get("captureState") == "failed" or time.monotonic() > deadline:
                raise RuntimeError("Actual browser did not finish normally")
            time.sleep(5)
        identifier = state["captureIdentifier"]
        if not re.fullmatch(r"capture_[0-9a-f]{32}", identifier) or state["isolated"] is not True:
            raise RuntimeError("Actual browser identity/isolation differs")
        capture = server / identifier
        receipt_path, manifest_path = server / f"{identifier}_receipt.json", server / f"{identifier}_manifest.json"
        receipt, manifest = json.loads(receipt_path.read_text()), json.loads(manifest_path.read_text())
        if (receipt.get("passed") is not True or receipt["completion"]["exit_status"] != 0 or
                any(digest(path) != seal for path, seal in receipt["protected_inputs"].items()) or
                sorted(str(path.relative_to(capture)) for path in capture.rglob("*") if path.is_dir()) != sorted(manifest["directories"])):
            raise RuntimeError("Capture shutdown, seals or directory inventory differs")
        for item in manifest["files"]:
            path = capture / item["relative_path"]
            if path.stat().st_size != item["size"] or digest(path) != receipt["files"][item["relative_path"]]:
                raise RuntimeError("Exported file extent/identity differs")
        snapshot = capture / "initial_user_state"
        expected_files = {item["relative_path"]: item["expected_sha256"] for item in configuration["inputs"]["initial_user_files"]}
        actual_files = {str(path.relative_to(snapshot)): digest(path) for path in snapshot.rglob("*") if path.is_file()}
        host_log = "log/reference_capture.log"
        if (host_log not in expected_files or host_log not in actual_files or
                {name: seal for name, seal in expected_files.items() if name != host_log} !=
                {name: seal for name, seal in actual_files.items() if name != host_log} or
                sorted(str(path.relative_to(snapshot)) for path in snapshot.rglob("*") if path.is_dir()) !=
                sorted(configuration["inputs"]["initial_user_directories"])):
            raise RuntimeError("Initial guest state inventory/identity differs")
        observations = load_input_capture(capture)
        movie = load_movie(capture / "input_movie.ctm")
        validate_observation_boundary(capture, observations)
        validate_movie_delivery(capture, movie, observations)
        polls = observations["polls"]
        pressed = [event for event in polls if event["buttons"] & 1]
        additions = [event for event in polls if event["delta_additions"] & 1]
        removals = [event for event in polls if event["delta_removals"] & 1]
        if args.input_method == "neutral":
            if pressed or additions or removals or manifest["button_requests"]:
                raise RuntimeError("Neutral browser unexpectedly delivered a button")
        elif not pressed or not additions or not removals or polls[-1]["buttons"] & 1:
            raise RuntimeError("Actual HID did not deliver press and release")
        if manifest["button_progress"]["poll_count"] <= 0 or any(counter["fallbacks"] != "0" for counter in manifest["counters"]):
            raise RuntimeError("Device polling/static execution closure differs")
        pixels = result(["eval", "async () => Object.fromEntries(await Promise.all([...document.querySelectorAll('canvas')].map(async c => [c.id,{width:c.width,height:c.height,hidden:c.hidden,sha256:[...new Uint8Array(await crypto.subtle.digest('SHA-256',c.getContext('2d').getImageData(0,0,c.width,c.height).data))].map(v=>v.toString(16).padStart(2,'0')).join('')}])))"])
        for name, screen in pixels.items():
            payload = capture / ("rendered_screen_0.rgba" if name == "top-screen" else "rendered_screen_2.rgba")
            if screen["hidden"] or screen["sha256"] != digest(payload):
                raise RuntimeError("Actual canvas bytes differ from capture")
        viewports = {}
        if args.screenshots:
            for name, width, height in (("desktop", 1440, 1080), ("mobile", 390, 1200)):
                run(["resize", str(width), str(height)])
                run(["run-code", "async page => {await page.evaluate(() => scrollTo(0,0)); await page.waitForTimeout(400);}"])
                run(["screenshot", "--full-page", "--filename=" + str(output / f"{name}.png")])
                raw = (output / f"{name}.png").read_bytes()
                if raw[:8] != b"\x89PNG\r\n\x1a\n" or int.from_bytes(raw[16:20], "big") != width or int.from_bytes(raw[20:24], "big") < height:
                    raise RuntimeError("Invalid full-page screenshot extent")
                viewports[name] = result(["eval", "() => ({inner_width:innerWidth,client_width:document.documentElement.clientWidth,scroll_height:document.documentElement.scrollHeight,fonts_loaded:document.fonts.check('400 16px \"IBM Plex Sans\"') && document.fonts.check('600 44px \"IBM Plex Sans\"')})"])
        if digest(dump) != configuration["inputs"]["dump"]["expected_sha256"]:
            raise RuntimeError("Readonly owner dump changed")
        report = {"passed": True, "capture": str(capture), "input_method": args.input_method, "browser_action": delivery,
            "browser": result(["eval", "() => navigator.userAgent"]), "pixels": pixels, "viewports": viewports,
            "input_polls": len(polls), "held_polls": len(pressed), "addition_polls": len(additions), "removal_polls": len(removals),
            "first_held_poll": pressed[0] if pressed else None, "first_removal_poll": removals[0] if removals else None,
            "movie_sha256": digest(capture / "input_movie.ctm"), "movie_pad_count": len(movie["pads"]),
            "movie_record_counts": movie["record_counts"], "button_requests": manifest["button_requests"],
            "snapshot_guest_files_exact": True, "snapshot_directories_exact": True,
            "raw_host_log_sha256": {"original": expected_files[host_log], "recorded": actual_files[host_log]},
            "manifest_sha256": digest(manifest_path), "receipt_sha256": digest(receipt_path), "source_sha256": digest(Path(__file__)),
            "scope": "Actual finite browser recording, HID press/release, complete export and displayed pixels. Stock replay and causal neutral comparison remain separate checks."}
        cleanup = browser.close()
        report["browser_session_policy"] = {"passed": cleanup["passed"],
                                             "cleanup_sha256": digest(output / "cleanup.json"),
                                             "registration_sha256": digest(output / "registration.json")}
    (output / "result.json").write_text(json.dumps(report, indent=2) + "\n")
    print(json.dumps({"passed": True, "capture": str(capture), "held_polls": len(pressed), "removal_polls": len(removals)}), flush=True)


if __name__ == "__main__":
    main()
