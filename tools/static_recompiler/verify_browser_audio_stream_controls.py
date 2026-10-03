#!/usr/bin/env python3
"""Exercise actual AudioWorklet output, ring wrap, rejection and owned cancellation."""
import argparse
from hashlib import sha256
import json
from pathlib import Path
import struct
from urllib.parse import urlsplit

from audit_webassembly_platform import digest
from browser_session_policy import BrowserSession

ROOT = Path(__file__).resolve().parents[2]

CONTROL_CODE = """async (page) => {
    await page.waitForFunction(() => document.body.dataset.captureState === 'ready');
    await page.evaluate(async () => {
        const {StreamedAudioPlayback} = await import('./BrowserStreamedAudio.mjs');
        const insist = (condition, message) => { if (!condition) throw new Error(message); };
        const packet = (identifier, sequence, first, count) => {
            const pcm = new Uint8Array(count * 4), view = new DataView(pcm.buffer);
            for (let index = 0; index < count; ++index) {
                view.setInt16(index * 4, ((first + index) * 257 + 32768) % 65536 - 32768, true);
                view.setInt16(index * 4 + 2, ((first + index) * 911 + 12345) % 65536 - 32768, true);
            }
            return {capture_identifier: identifier, sequence, first_sample_frame: first,
                sample_frames: count, sample_rate: 32728, channels: 2, pcm};
        };
        const hash = async samples => {
            const bytes = new ArrayBuffer(samples.length * 4), view = new DataView(bytes);
            for (let index = 0; index < samples.length; ++index) view.setFloat32(index * 4, samples[index], true);
            return [...new Uint8Array(await crypto.subtle.digest('SHA-256', bytes))].map(value => value.toString(16).padStart(2, '0')).join('');
        };
        window.audioStreamControlTask = async () => {
            const full = new StreamedAudioPlayback({capture_identifier: 'independent_stereo_signal', sample_rate: 32728, channels: 2});
            const total = 131077;
            full.enableObservation(total);
            insist(full.context === null, 'Context exists before gesture');
            let cancelled, draining;
            try {
                await full.start();
                insist(full.context.state === 'running' && full.context.sampleRate === 32728, 'Actual context refused');
                const base = packet(full.metadata.capture_identifier, 0, 0, 1);
                const invalid = [{...base, capture_identifier: 'other_capture'}, {...base, sequence: 1},
                    {...base, first_sample_frame: 1}, {...base, sample_rate: 48000}, {...base, channels: 1},
                    {...base, pcm: new Uint8Array(3)}, {...base, sample_frames: 0}, {...base, extra_field: true}];
                const refusals = [];
                for (const item of invalid) {
                    let refused = false;
                    try { await full.append(item); } catch { refused = true; }
                    insist(refused && full.statistics().accepted_source_frames === 0 && base.pcm.byteLength === 4,
                        'Malformed packet mutated source acceptance');
                    refusals.push(refused);
                }
                let invalidFinish = false;
                try { await full.finish(1); } catch { invalidFinish = true; }
                insist(invalidFinish && full.state === 'running', 'Wrong final count changed the graph');
                let first = 0, sequence = 0, longestAppendMilliseconds = 0;
                while (first < total) {
                    const count = Math.min(2048, total - first), source = packet(full.metadata.capture_identifier, sequence, first, count);
                    const started = performance.now();
                    await full.append(source);
                    longestAppendMilliseconds = Math.max(longestAppendMilliseconds, performance.now() - started);
                    insist(source.pcm.byteLength === count * 4, 'Caller buffer detached');
                    first += count; ++sequence;
                }
                const receipt = await full.finish(total);
                insist(receipt.consumed_source_frames === total && receipt.context_state === 'closed' &&
                    receipt.high_water_mark_frames <= 65536 && receipt.output_observation_frames === total, 'Actual drain differs');
                const output = full.outputObservation;
                insist(output.sample_frames === total, 'Output witness truncated');
                const fullResult = {receipt, output_float32le_sha256: await hash(output.interleaved_samples),
                    malformed_packet_refusals: refusals.length, invalid_finish_refused: invalidFinish,
                    longest_append_milliseconds: longestAppendMilliseconds, caller_buffers_retained: true};
                cancelled = new StreamedAudioPlayback({capture_identifier: 'pending_append_cancellation', sample_rate: 32728, channels: 2});
                await cancelled.start();
                const source = packet(cancelled.metadata.capture_identifier, 0, 0, 2048);
                const pending = cancelled.append(source).then(() => false, () => true);
                let concurrentRefused = false;
                try { await cancelled.append(source); } catch { concurrentRefused = true; }
                await cancelled.stop();
                insist(await pending && concurrentRefused && cancelled.context.state === 'closed', 'Pending append did not cancel');
                let terminalRefused = false;
                try { await cancelled.start(); } catch { terminalRefused = true; }
                await cancelled.dispose(); await cancelled.dispose();
                insist(terminalRefused && !cancelled.node && !cancelled.pendingAppend, 'Terminal graph retained work');
                draining = new StreamedAudioPlayback({capture_identifier: 'pending_drain_cancellation', sample_rate: 32728, channels: 2});
                await draining.start();
                for (let index = 0; index < 32; ++index) await draining.append(packet(draining.metadata.capture_identifier, index, index * 2048, 2048));
                insist(draining.statistics().buffered_source_frames > 2048, 'No actual queued source remains for drain cancellation');
                const pendingDrain = draining.finish(65536).then(() => false, () => true);
                await draining.stop();
                insist(await pendingDrain && draining.context.state === 'closed' && !draining.outputObservation, 'Drain cancellation manufactured a witness');
                return {full: fullResult, pending_append_cancelled: true, concurrent_append_refused: true,
                    terminal_start_refused: true, repeated_disposal_closed: true, pending_drain_cancelled: true,
                    cancellation_statistics: cancelled.statistics(), drain_statistics: draining.statistics()};
            } finally {
                await full.dispose();
                if (cancelled) await cancelled.dispose();
                if (draining) await draining.dispose();
            }
        };
        const button = document.createElement('button'); button.id = 'audio-stream-control';
        button.textContent = 'Start audio control';
        button.onclick = () => { window.audioStreamControlResult = window.audioStreamControlTask(); };
        document.body.append(button);
    });
    await page.getByRole('button', {name: 'Start audio control', exact: true}).click();
    return await page.evaluate(async () => {
        try { return await window.audioStreamControlResult; }
        finally { document.querySelector('#audio-stream-control').remove(); }
    });
}"""


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("url")
    parser.add_argument("output", type=Path)
    args = parser.parse_args()
    url = urlsplit(args.url)
    output = args.output.resolve()
    if (url.scheme != "http" or url.hostname != "127.0.0.1" or url.username or url.password or url.query or url.fragment
            or not output.is_relative_to(ROOT / "build") or output.exists()):
        raise ValueError("Use a local preview and absent ignored output")
    sources = {str(path): digest(path) for path in [Path(__file__),
        ROOT / "runtime/port/browser/BrowserStreamedAudio.mjs", ROOT / "runtime/port/browser/BrowserAudioWorklet.mjs"]}
    output.mkdir(parents=True)
    with BrowserSession(output, headed=False) as browser:
        browser.run(["open", args.url])
        text = browser.run(["run-code", CONTROL_CODE], timeout=60)
        marker = "### Result\n"
        if marker not in text:
            raise RuntimeError("Actual audio controls returned no result")
        result = json.JSONDecoder().raw_decode(text.split(marker, 1)[1].lstrip())[0]
        expected = sha256()
        for index in range(131077):
            pair = ((index * 257 + 32768) % 65536 - 32768, (index * 911 + 12345) % 65536 - 32768)
            expected.update(struct.pack("<ff", *(sample / 32768 for sample in pair)))
        if (result["full"]["output_float32le_sha256"] != expected.hexdigest()
                or result["full"]["receipt"]["high_water_mark_frames"] < 60000
                or result["full"]["longest_append_milliseconds"] < 20
                or any(digest(path) != value for path, value in sources.items())):
            raise RuntimeError("Actual independent stereo/ring/backpressure source proof differs")
        cleanup = browser.close()
        result.update({"passed": True, "source_sha256": sources, "expected_float32le_sha256": expected.hexdigest(),
            "browser_session_policy": {"passed": cleanup["passed"], "cleanup_sha256": digest(output / "cleanup.json")},
            "scope": "Actual requested-rate stereo worklet output and bounded queue cancellation. No game execution, speaker or device fidelity claim."})
    (output / "result.json").write_text(json.dumps(result, indent=2) + "\n")
    print(json.dumps({"passed": True, "output": str(output), "source_frames": 131077}), flush=True)


if __name__ == "__main__":
    main()
