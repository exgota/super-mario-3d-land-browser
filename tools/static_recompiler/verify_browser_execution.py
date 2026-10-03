#!/usr/bin/env python3
"""Run the actual local browser capture, compare its exported bytes and inspect displayed pixels."""
import argparse
import json
from pathlib import Path
import re
import subprocess
import struct
import sys
import time
from urllib.parse import urlsplit

from audit_webassembly_platform import digest
from browser_session_policy import BrowserSession, reject_keep_open

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
    parser.add_argument("--observe-input", action="store_true", help="require strict recorded HID parity")
    parser.add_argument("--observe-audio", action="store_true", help="require strict PCM/input parity and real WebAudio play/stop")
    parser.add_argument("--stream-audio", action="store_true", help="verify original streamed source against actual AudioWorklet output")
    parser.add_argument("--stop-stream-after-frames", type=int, help="cancel actual preview sound after this consumed source count")
    parser.add_argument("--movie", type=Path, help="original recorded movie when the reference is a replay directory")
    parser.add_argument("--headed", action="store_true")
    parser.add_argument("--keep-open", action="store_true", help="refused by mandatory browser lifecycle policy")
    parser.add_argument("--exercise-controls", action="store_true", help="verify file rejection, cancellation and recovery before the real capture")
    parser.add_argument("--timeout-seconds", type=int, default=360)
    args = parser.parse_args()
    if args.stream_audio and not args.observe_audio:
        raise ValueError("Stream verification requires original audio observation")
    if args.stop_stream_after_frames is not None and (not args.stream_audio or not 1 <= args.stop_stream_after_frames <= 1048576):
        raise ValueError("Audio cancellation needs a bounded streamed source count")
    reject_keep_open(getattr(args, "keep_open", False))
    url = urlsplit(args.url)
    output, server_output = args.output.resolve(), args.server_output.resolve()
    if url.scheme != "http" or url.hostname != "127.0.0.1" or url.username or url.password or url.query or url.fragment:
        raise ValueError("Verification must use the local preview server")
    if (not output.is_relative_to(ROOT / "build") or output.exists() or
        not server_output.is_relative_to(ROOT / "build") or not server_output.is_dir()):
        raise ValueError("Own ignored output directories are required")
    if not 1 <= args.timeout_seconds <= 7200:
        raise ValueError("Invalid external browser deadline")
    configuration = json.loads((server_output / "configuration.json").read_text())
    if bool(configuration["options"].get("stream_audio_output")) != args.stream_audio:
        raise ValueError("Server stream profile differs from the requested verification")
    if (bool(configuration["options"].get("audio_capture")) != args.observe_audio or
        bool(configuration["options"].get("input_capture")) != (args.observe_input or args.observe_audio) or
        (configuration["options"]["presentation_limit"] is None) != args.first_swap):
        raise ValueError("Server observation profile differs from the requested verification")
    dump = args.dump.resolve()
    protected_dump = {"path": str(dump), "sha256": digest(dump), "bytes": dump.stat().st_size}
    output.mkdir(parents=True)
    with BrowserSession(output, headed=args.headed) as browser:
        run = browser.run
        def evaluate(code):
            text = run(["eval", code])
            marker = "### Result\n"
            if marker not in text:
                raise RuntimeError("Browser evaluation has no result")
            return json.JSONDecoder().raw_decode(text.split(marker, 1)[1].lstrip())[0]

        run(["open", args.url])
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
        stream_start = {}
        stream_stop = {}
        if args.stream_audio:
            run(["run-code", "async (page) => { await page.waitForFunction(() => document.body.dataset.captureState === 'running', null, {timeout: 60000}); }"])
            stream_start = evaluate("async () => { const player = (await import('./BrowserCapturePage.mjs')).streamedAudio(); "
                "if (!player || player.context || player.state !== 'idle') throw new Error('Stream started without gesture'); "
                "player.enableObservation(1048576); window.rootAudioStreamVerifierPlayer = player; return player.statistics(); }")
            run(["run-code", "async (page) => { await page.getByRole('button', {name: 'Play preview sound', exact: true}).click(); "
                 "await page.waitForFunction(() => document.body.dataset.audioState === 'running', null, {timeout: 30000}); }"])
            if args.stop_stream_after_frames is not None:
                run(["run-code", "async (page) => { await page.waitForFunction(() => "
                    "window.rootAudioStreamVerifierPlayer.statistics().consumed_source_frames >= " + str(args.stop_stream_after_frames) + ", null, {timeout: 120000}); "
                    "await page.getByRole('button', {name: 'Stop sound', exact: true}).click(); "
                    "await page.waitForFunction(() => window.rootAudioStreamVerifierPlayer.state === 'stopped' && "
                    "window.rootAudioStreamVerifierPlayer.context.state === 'closed'); }"], timeout=150)
                stream_stop = evaluate("async () => ({capture_state: document.body.dataset.captureState, "
                    "statistics: (await import('./BrowserCapturePage.mjs')).streamedAudio().statistics()})")
                if (stream_stop["capture_state"] != "running" or
                    stream_stop["statistics"]["consumed_source_frames"] < args.stop_stream_after_frames):
                    raise RuntimeError("Sound cancellation did not occur during actual guest capture")
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
        comparator_name = ("compare_audio_capture.py" if args.observe_audio else "compare_input_capture.py" if args.observe_input
                           else "compare_gpu_capture.py" if args.first_swap else "compare_rendered_capture.py")
        comparator = ROOT / "tools/static_recompiler" / comparator_name
        comparison_path = output / "comparison.json"
        comparison_command = [sys.executable, str(comparator), str(args.reference.resolve()), str(capture),
                              "--report", str(comparison_path)]
        if args.observe_input or args.observe_audio:
            movie = args.movie.resolve() if args.movie else args.reference.resolve() / "input_movie.ctm"
            comparison_command.extend(["--movie", str(movie)])
        with (output / "comparison.log").open("wb") as stream:
            comparison = subprocess.run(comparison_command, stdout=stream, stderr=subprocess.STDOUT, timeout=60)
        if comparison.returncode or json.loads(comparison_path.read_text()).get("passed") is not True:
            raise RuntimeError("Actual browser differential comparison refused")
        pixels = {}
        audio = {}
        audio_stream = {}
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
            if args.observe_audio:
                audio = evaluate("async () => { const player = (await import('./BrowserCapturePage.mjs')).capturedAudio(); "
                    "if (!player || !(player.buffer instanceof AudioBuffer)) throw new Error('No actual captured AudioBuffer'); "
                    "const identity = await player.identity(); const offline = new OfflineAudioContext(2, player.buffer.length, player.buffer.sampleRate); "
                    "const source = offline.createBufferSource(); source.buffer = player.buffer; source.connect(offline.destination); source.start(); "
                    "const rendered = await offline.startRendering(); const hashes = []; for (let channel = 0; channel < 2; ++channel) { "
                    "const samples = rendered.getChannelData(channel); const bytes = new ArrayBuffer(samples.length * 4); const view = new DataView(bytes); "
                    "for (let index = 0; index < samples.length; ++index) view.setFloat32(index * 4, samples[index], true); "
                    "hashes.push([...new Uint8Array(await crypto.subtle.digest('SHA-256', bytes))].map(value => value.toString(16).padStart(2, '0')).join('')); } "
                    "return {identity, rendered_rate: rendered.sampleRate, rendered_frames: rendered.length, rendered_channels: rendered.numberOfChannels, "
                    "rendered_float32le_sha256: hashes, initial_context_absent: !player.context, initial_source_absent: !player.source}; }")
                pcm_path = capture / "audio_pcm_s16le.bin"
                pcm = pcm_path.read_bytes()
                from hashlib import sha256
                channel_hashes = [sha256(), sha256()]
                for samples in struct.iter_unpack("<hh", pcm):
                    for channel, sample in enumerate(samples):
                        channel_hashes[channel].update(struct.pack("<f", sample / 32768))
                expected_hashes = [item.hexdigest() for item in channel_hashes]
                if (audio["identity"]["pcm_sha256"] != digest(pcm_path) or
                    audio["identity"]["channel_float32le_sha256"] != expected_hashes or
                    audio["rendered_float32le_sha256"] != expected_hashes or audio["rendered_rate"] != 32728 or
                    audio["rendered_frames"] != len(pcm) // 4 or audio["rendered_channels"] != 2 or
                    not audio["initial_context_absent"] or not audio["initial_source_absent"]):
                    raise RuntimeError("Actual WebAudio conversion/offline output differs from the completed browser PCM")
                if args.stream_audio and args.stop_stream_after_frames is None:
                    audio_stream = evaluate("async () => { const player = (await import('./BrowserCapturePage.mjs')).streamedAudio(); "
                        "const witness = player?.outputObservation; if (!witness || player.state !== 'ended' || player.context.state !== 'closed') "
                        "throw new Error('Original stream did not drain its actual worklet output'); "
                        "const bytes = new ArrayBuffer(witness.interleaved_samples.length * 4); const view = new DataView(bytes); "
                        "for (let index = 0; index < witness.interleaved_samples.length; ++index) view.setFloat32(index * 4, witness.interleaved_samples[index], true); "
                        "const hash = [...new Uint8Array(await crypto.subtle.digest('SHA-256', bytes))].map(value => value.toString(16).padStart(2, '0')).join(''); "
                        "return {statistics: player.statistics(), receipt: player.receipt, output_frames: witness.sample_frames, "
                        "output_float32le_sha256: hash, events: player.events}; }")
                    from hashlib import sha256
                    normalized = sha256()
                    for samples in struct.iter_unpack("<hh", pcm):
                        normalized.update(struct.pack("<ff", *(sample / 32768 for sample in samples)))
                    observation = manifest["observations"]["audio_stream"]
                    if (not observation.get("enabled") or observation.get("state") != "completed" or
                        observation["sample_frames"] != len(pcm) // 4 or observation["payload_bytes"] != len(pcm) or
                        audio_stream["output_frames"] != len(pcm) // 4 or
                        audio_stream["output_float32le_sha256"] != normalized.hexdigest() or
                        observation["consumer"] != audio_stream["receipt"] or
                        audio_stream["statistics"]["consumed_source_frames"] != len(pcm) // 4):
                        raise RuntimeError("Actual worklet output differs from the original PCM source")
                    offset = 0
                    for sequence, packet in enumerate(observation["packets"]):
                        length = packet["sample_frames"] * 4
                        if (packet["sequence"] != sequence or packet["first_sample_frame"] * 4 != offset or
                            not 0 < length <= 8192 or packet["source_extent_bytes"] < offset + length or
                            sha256(pcm[offset:offset + length]).hexdigest() != packet["pcm_sha256"]):
                            raise RuntimeError("Original stream packet identity or ordering differs")
                        offset += length
                    running = [report for report in observation["consumption_reports"] if report["native_phase"] == "running"]
                    running_packets = [packet for packet in observation["packets"]
                                       if packet["native_phase_before_delivery"] == packet["native_phase_after_delivery"] == "running"]
                    if offset != len(pcm) or not running or not running_packets or not any(
                        report["sample_frames"] > 0 and report["source_extent_bytes"] < len(pcm) for report in running):
                        raise RuntimeError("No actual source consumption while the original producer was still growing")
                    audio_stream.update({"initial": stream_start, "observation": observation,
                        "expected_output_float32le_sha256": normalized.hexdigest(),
                        "running_consumption_reports": len(running), "running_packets": len(running_packets),
                        "scope": "Every original stereo pair reaches actual worklet output once and in order; production continues after first consumption. Gaps are counted. Device output and synchronization unverified."})
                elif args.stream_audio:
                    observation = manifest["observations"]["audio_stream"]
                    final = evaluate("async () => { const player = (await import('./BrowserCapturePage.mjs')).streamedAudio(); "
                        "return {statistics: player.statistics(), witness_absent: player.outputObservation === null, node_absent: !player.node}; }")
                    if (not observation.get("enabled") or observation.get("state") != "stopped" or
                        "consumer" in observation or observation["sample_frames"] >= len(pcm) // 4 or
                        final["statistics"]["state"] != "stopped" or final["statistics"]["context_state"] != "closed" or
                        not final["witness_absent"] or not final["node_absent"]):
                        raise RuntimeError("Stopped audio stream claimed completion or retained its graph")
                    audio_stream = {"initial": stream_start, "stopped_during_guest": stream_stop, "final": final,
                        "observation": observation, "scope": "Explicit sound cancellation closes the owned graph; the original guest capture and completed-clip playback still pass unchanged."}
                # The observed menu is dual mono. A separate, channel-distinct
                # signal checks byte order and channel assignment on the real graph.
                fixture_samples = [(0, -32768), (32767, -12345), (-257, 513), (12345, -1)]
                fixture_bytes = b"".join(struct.pack("<hh", *samples) for samples in fixture_samples)
                audio["stereo_fixture"] = evaluate("async () => { const {CapturedAudioPlayback} = await import('./BrowserCapturedAudio.mjs'); "
                    "const bytes = new Uint8Array(" + json.dumps(list(fixture_bytes)) + "); const player = new CapturedAudioPlayback("
                    "{sample_rate: 32728, channels: 2, sample_frames: 4, payload_bytes: 16}, bytes, () => {}); "
                    "const offline = new OfflineAudioContext(2, 4, 32728); const source = offline.createBufferSource(); "
                    "source.buffer = player.buffer; source.connect(offline.destination); source.start(); const rendered = await offline.startRendering(); "
                    "const channels = [Array.from(rendered.getChannelData(0)), Array.from(rendered.getChannelData(1))]; "
                    "const identity = await player.identity(); player.dispose(); return {identity, channels}; }")
                if audio["stereo_fixture"]["channels"] != [[pair[channel] / 32768 for pair in fixture_samples] for channel in range(2)]:
                    raise RuntimeError("Independent channel-distinct signed-16 fixture disagrees with the real WebAudio graph")
                run(["run-code", "async (page) => { await page.getByRole('button', {name: 'Play recorded sound', exact: true}).click(); "
                     "await page.waitForFunction(() => document.body.dataset.audioState === 'playing'); "
                     "await page.waitForTimeout(150); await page.getByRole('button', {name: 'Stop sound', exact: true}).click(); "
                     "await page.waitForFunction(() => document.body.dataset.audioState === 'ready'); }"])
                run(["run-code", "async (page) => { await page.getByRole('button', {name: 'Play recorded sound', exact: true}).click(); "
                     "await page.waitForFunction(() => document.body.dataset.audioState === 'playing'); "
                     "await page.waitForFunction(() => document.body.dataset.audioState === 'ready', null, {timeout: 30000}); }"])
                audio["playback"] = evaluate("async () => { const player = (await import('./BrowserCapturePage.mjs')).capturedAudio(); "
                    "return {events: player.events, context_state: player.context.state, context_time: player.context.currentTime, "
                    "context_rate: player.context.sampleRate, source_absent: !player.source, button: document.querySelector('#play-recorded-sound').textContent}; }")
                if ([item["kind"] for item in audio["playback"]["events"]] != ["started", "stopped", "started", "ended"] or
                    audio["playback"]["context_state"] != "running" or not audio["playback"]["source_absent"] or
                    audio["playback"]["context_time"] <= len(pcm) / 4 / 32728):
                    raise RuntimeError("Actual explicit WebAudio play/stop/natural completion refused")
                audio["scope"] = "Exact browser PCM and normalized Float32 source plus equal-rate offline graph; live AudioContext runs, stops and ends. Speaker fidelity and continuous synchronization unverified."
            for name, width, height in (("desktop", 1440, 1080), ("mobile", 390, 1200)):
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
                  "audio": audio,
                  "audio_stream": audio_stream,
                  "snapshot_identity": snapshot_identity,
                  "capture_receipt_sha256": digest(receipt_path), "capture_manifest_sha256": digest(manifest_path),
                  "comparison_command": comparison_command, "comparison_sha256": digest(comparison_path),
                  "comparator_sha256": digest(comparator), "source_sha256": digest(Path(__file__)),
                  "scope": "Actual bounded browser startup, complete directory and guest-file preservation with raw host logs retained, strict raw differential replay "
                           "and optional displayed frame bytes, recorded HID/audio observations and explicit completed-clip WebAudio playback. "
                           "No continuous gameplay, live HID/audio synchronization or World 1-1 claim."}
        cleanup = browser.close()
        result["browser_session_policy"] = {"passed": cleanup["passed"],
                                             "cleanup_sha256": digest(output / "cleanup.json"),
                                             "registration_sha256": digest(output / "registration.json")}
    (output / "result.json").write_text(json.dumps(result, indent=2) + "\n")
    print(json.dumps({"passed": True, "capture": str(capture), "pixels": pixels, "scope": result["scope"]}), flush=True)


if __name__ == "__main__":
    main()
