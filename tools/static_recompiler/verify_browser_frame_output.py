#!/usr/bin/env python3
"""Retain real intermediate canvas samples and require unchanged full movie replay."""
import argparse
import atexit
import base64
import hashlib
import json
from pathlib import Path
import re
import subprocess
import sys
import time
from urllib.parse import urlsplit

from audit_webassembly_platform import digest
from compare_rendered_capture import load_rendered_capture
from verify_browser_execution import CLI_PACKAGE

ROOT = Path(__file__).resolve().parents[2]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("url")
    parser.add_argument("output", type=Path)
    parser.add_argument("--server-output", type=Path, required=True)
    parser.add_argument("--dump", type=Path, required=True)
    parser.add_argument("--reference", type=Path, required=True)
    parser.add_argument("--thresholds", default="161,500", help="two increasing renderer-frame sampling thresholds")
    parser.add_argument("--exercise-stop", action="store_true")
    parser.add_argument("--screenshots", action="store_true")
    parser.add_argument("--keep-open", action="store_true")
    args = parser.parse_args()
    output, server, reference = args.output.resolve(), args.server_output.resolve(), args.reference.resolve()
    url = urlsplit(args.url)
    thresholds = [int(value) for value in args.thresholds.split(",")]
    if (output.exists() or not output.is_relative_to(ROOT / "build") or
            not server.is_relative_to(ROOT / "build") or not server.is_dir() or
            url.scheme != "http" or url.hostname != "127.0.0.1" or url.username or url.password or url.query or url.fragment or
            len(thresholds) != 2 or not 1 <= thresholds[0] < thresholds[1] <= 3600):
        raise ValueError("Fresh ignored output, local server and two finite thresholds required")
    configuration = json.loads((server / "configuration.json").read_text())
    options = configuration["options"]
    if (options.get("frame_output") is not True or options.get("live_button_capture") or
            options.get("input_capture") is not True or options.get("audio_capture") is not True):
        raise ValueError("Read-only movie/input/audio frame-output profile required")
    dump = args.dump.resolve()
    if digest(dump) != configuration["inputs"]["dump"]["expected_sha256"]:
        raise ValueError("Approved dump identity differs")
    output.mkdir(parents=True)
    commands = []
    session = "root-browser-" + output.name.replace("_", "-")
    cli = ["npx", "--yes", "--package", CLI_PACKAGE, "playwright-cli", "-s=" + session]

    def run(arguments, timeout=60):
        command = [*cli, *arguments]
        started = time.monotonic()
        result = subprocess.run(command, cwd=output, capture_output=True, text=True, timeout=timeout)
        index = len(commands)
        stdout, stderr = output / f"command_{index}_stdout.log", output / f"command_{index}_stderr.log"
        stdout.write_text(result.stdout)
        stderr.write_text(result.stderr)
        commands.append({"command": command, "status": result.returncode, "stdout": str(stdout),
                         "stderr": str(stderr), "elapsed_seconds": time.monotonic() - started})
        (output / "commands.json").write_text(json.dumps(commands, indent=2) + "\n")
        if result.returncode or "### Error" in result.stdout:
            raise RuntimeError(f"Actual browser refused; see {stdout}")
        return result.stdout

    def result(arguments, timeout=60):
        raw = run(arguments, timeout)
        marker = "### Result\n"
        if marker not in raw:
            raise RuntimeError("Actual browser result absent")
        return json.JSONDecoder().raw_decode(raw.split(marker, 1)[1].lstrip())[0]

    def begin():
        run(["run-code", "async (page) => { await page.getByLabel('Game file',{exact:true}).setInputFiles(" +
             json.dumps(str(dump)) + "); await page.getByRole('button',{name:'Run preview',exact:true}).click(); }"])

    run(["open", args.url, "--headed"])
    if not args.keep_open:
        atexit.register(lambda: run(["close"]))
    run(["resize", "1440", "1080"])
    run(["run-code", "async (page) => { await page.waitForFunction(() => document.body.dataset.captureState === 'ready'); }"])
    controls = {}
    if args.exercise_stop:
        begin()
        controls["stopped_preview"] = result(["run-code", "async (page) => { await page.waitForFunction(() => "
            "Number(document.body.dataset.previewFrameCount)>2 || document.body.dataset.captureState === 'failed',null,{timeout:240000}); "
            "const before = await page.evaluate(() => ({...document.body.dataset})); "
            "await page.getByRole('button',{name:'Stop preview',exact:true}).click(); "
            "return {before,after:await page.evaluate(() => ({...document.body.dataset,error:document.querySelector('#capture-error').textContent," 
            "input_enabled:!document.querySelector('#game-file').disabled,run_enabled:!document.querySelector('#run-preview').disabled}))}; }"], 270)
        control = controls["stopped_preview"]
        if (control["before"].get("captureState") != "running" or
                control["after"].get("captureState") != "failed" or "stopped" not in control["after"]["error"] or
                not control["after"]["input_enabled"] or not control["after"]["run_enabled"]):
            raise RuntimeError("Actual intermediate stop/recovery control refused")
    # This observer copies the independently displayed canvas, never reference
    # pixels or the shared heap. It retains only two pairs and no timing claims.
    observer = "async (page) => { await page.evaluate(async thresholds => { const bridge = await import('./BrowserCapturePage.mjs'); "
    observer += "window.__rootPortFrameEvidence={samples:[],polls:0}; const retained=window.__rootPortFrameEvidence; "
    observer += "retained.timer=setInterval(() => { ++retained.polls; if(document.body.dataset.captureState!=='running') return; "
    observer += "const metadata=bridge.previewFrameObservation(); const threshold=thresholds[retained.samples.length]; "
    observer += "if(!metadata || threshold===undefined || BigInt(metadata.renderer_frame)<BigInt(threshold)) return; "
    observer += "const screens=['top-screen','bottom-screen'].map(name => {const canvas=document.getElementById(name); "
    observer += "return {name,width:canvas.width,height:canvas.height,hidden:canvas.hidden,rgba:Array.from(canvas.getContext('2d').getImageData(0,0,canvas.width,canvas.height).data)};}); "
    observer += "retained.samples.push({metadata,state:document.body.dataset.captureState,screens}); if(retained.samples.length===thresholds.length)clearInterval(retained.timer); },50); "
    observer += "}," + json.dumps(thresholds) + "); }"
    run(["run-code", observer])
    begin()
    action = "async (page) => { await page.waitForFunction(() => window.__rootPortFrameEvidence.samples.length===2 || document.body.dataset.captureState==='failed',null,{timeout:240000}); "
    if args.screenshots:
        action += "await page.screenshot({path:" + json.dumps(str(output / "desktop_running.png")) + ",fullPage:true}); "
        action += "await page.setViewportSize({width:390,height:1200}); await page.screenshot({path:" + json.dumps(str(output / "mobile_running.png")) + ",fullPage:true}); await page.setViewportSize({width:1440,height:1080}); "
    action += "return await page.evaluate(() => ({...document.body.dataset})); }"
    running = result(["run-code", action], 270)
    deadline = time.monotonic() + 240
    states = []
    while True:
        state = result(["eval", "() => ({...document.body.dataset,status:document.querySelector('#capture-status').textContent,error:document.querySelector('#capture-error').textContent})"])
        states.append(state)
        (output / "observed_states.json").write_text(json.dumps(states, indent=2) + "\n")
        print(json.dumps(state), flush=True)
        if state.get("captureState") == "complete":
            break
        if state.get("captureState") == "failed" or time.monotonic() > deadline:
            raise RuntimeError(f"Actual frame-output capture refused: {state}")
        time.sleep(5)
    identifier = state["captureIdentifier"]
    if not re.fullmatch(r"capture_[0-9a-f]{32}", identifier):
        raise RuntimeError("Capture identity differs")
    capture = server / identifier
    receipt_path, manifest_path = server / f"{identifier}_receipt.json", server / f"{identifier}_manifest.json"
    receipt, manifest = json.loads(receipt_path.read_text()), json.loads(manifest_path.read_text())
    if receipt.get("passed") is not True or any(digest(path) != expected for path, expected in receipt["protected_inputs"].items()):
        raise RuntimeError("Protected actual server input changed")
    for name, expected in receipt["files"].items():
        if digest(capture / name) != expected:
            raise RuntimeError("Actual exported output changed")
    raw = result(["eval", "() => { const evidence=window.__rootPortFrameEvidence; clearInterval(evidence.timer); "
        "return {polls:evidence.polls,samples:evidence.samples.map(sample => ({...sample,screens:sample.screens.map(screen => { "
        "let text='';for(let index=0;index<screen.rgba.length;index+=32768)text+=String.fromCharCode(...screen.rgba.slice(index,index+32768)); "
        "const {rgba,...metadata}=screen; return {...metadata,rgba_base64:btoa(text)};})}))}; }"])
    preview = manifest["observations"]["preview_frames"]
    final = load_rendered_capture(capture)
    final_frame = final["presentation"]["renderer_frame"]
    final_presentation = final["presentation"]["presentation_index"] + 1
    samples = []
    for index, sample in enumerate(raw["samples"]):
        metadata = sample["metadata"]
        frame = int(metadata["renderer_frame"])
        publication = preview["frames"][metadata["sequence"] - 1]
        target = frame - final_frame + final_presentation
        if (sample["state"] != "running" or metadata != publication or not 1 <= target < final_presentation):
            raise RuntimeError("Intermediate frame identity or mapping differs")
        for screen, expected in zip(sample["screens"], metadata["screens"]):
            rgba = base64.b64decode(screen.pop("rgba_base64"), validate=True)
            if (screen["hidden"] or screen["width"] != expected["width"] or screen["height"] != expected["height"] or
                    len(rgba) != expected["bytes"] or hashlib.sha256(rgba).hexdigest() != expected["sha256"]):
                raise RuntimeError("Independent intermediate canvas differs from copied slot")
            path = output / f"sample_{index}_screen_{expected['screen_identifier']}.rgba"
            path.write_bytes(rgba)
            screen.update({"path": str(path), "sha256": digest(path), "bytes": len(rgba)})
        samples.append({**sample, "stock_presentation_target": target})
    if len(samples) != 2 or samples[0]["metadata"]["renderer_frame"] == samples[1]["metadata"]["renderer_frame"]:
        raise RuntimeError("Two distinct actual running frames required")
    if all(left["sha256"] == right["sha256"] for left, right in zip(samples[0]["screens"], samples[1]["screens"])):
        raise RuntimeError("Intermediate images did not change")
    comparison_path = output / "full_movie_comparison.json"
    comparator = ROOT / "tools/static_recompiler/compare_movie_replay.py"
    with (output / "full_movie_comparison.log").open("wb") as stream:
        comparison = subprocess.run([sys.executable, str(comparator), str(reference), str(capture),
            "--movie", str(reference / "input_movie.ctm"), "--report", str(comparison_path)],
            stdout=stream, stderr=subprocess.STDOUT, timeout=90)
    if comparison.returncode or json.loads(comparison_path.read_text()).get("passed") is not True:
        raise RuntimeError("Unchanged full movie differential replay refused")
    final_pixels = result(["eval", "async () => {const result=[];for(const name of ['top-screen','bottom-screen']){const canvas=document.getElementById(name); "
        "const bytes=canvas.getContext('2d').getImageData(0,0,canvas.width,canvas.height).data; result.push({name,width:canvas.width,height:canvas.height," 
        "sha256:[...new Uint8Array(await crypto.subtle.digest('SHA-256',bytes))].map(value=>value.toString(16).padStart(2,'0')).join('')});}return result;}"])
    for screen, screen_identifier in zip(final_pixels, (0, 2)):
        if screen["sha256"] != receipt["files"][f"rendered_screen_{screen_identifier}.rgba"]:
            raise RuntimeError("Separate final canvas differs from completed capture")
    viewports = {}
    if args.screenshots:
        for name, width, height in (("desktop", 1440, 1080), ("mobile", 390, 1200)):
            run(["resize", str(width), str(height)])
            viewports[name] = result(["eval", "async () => {await document.fonts.ready; return {width:innerWidth,height:innerHeight,scroll_width:document.documentElement.scrollWidth," 
                "font:document.fonts.check('16px \"IBM Plex Sans\"'),state:document.body.dataset.captureState};}"])
            run(["screenshot", "--filename=" + str(output / f"{name}.png"), "--full-page"])
    value = {"passed": True, "configuration": configuration, "capture": str(capture), "running": running,
        "preview": preview, "samples": samples, "observer_polls": raw["polls"], "controls": controls,
        "full_movie_comparison": {"path": str(comparison_path), "sha256": digest(comparison_path)},
        "final_pixels": final_pixels, "viewports": viewports, "server_receipt_sha256": digest(receipt_path),
        "manifest_sha256": digest(manifest_path), "verifier_sha256": digest(Path(__file__)),
        "scope": "Actual running canvas equals owned slot bytes; final movie/HID/audio/GPU/ticks/PICA/framebuffer/RGBA parity. Fresh stock intermediate checkpoints remain a separate required check."}
    (output / "result.json").write_text(json.dumps(value, indent=2) + "\n")
    print(json.dumps({"passed": True, "copied_frames": preview["copied_and_acknowledged"],
                      "sample_frames": [sample["metadata"]["renderer_frame"] for sample in samples]}))


if __name__ == "__main__":
    main()
