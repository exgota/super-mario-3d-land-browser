#!/usr/bin/env python3
"""Compare exact HLE PCM output and timing with complete input/render replay."""
import argparse
from collections import deque
import json
import math
from pathlib import Path
import re
import struct
import wave

from compare_gpu_capture import digest
from compare_input_capture import compare as compare_input_capture
from compare_rendered_capture import payload_path, unique_object, unsigned_integer

ROOT = Path(__file__).resolve().parents[2]
MAXIMUM_AUDIO_BYTES = 64 * 1024 * 1024
SAMPLE_RATE = 32728
CHANNELS = 2
SAMPLE_WIDTH = 2
SAMPLE_FRAMES_PER_BLOCK = 160
BYTES_PER_SAMPLE_FRAME = CHANNELS * SAMPLE_WIDTH
BYTES_PER_BLOCK = SAMPLE_FRAMES_PER_BLOCK * BYTES_PER_SAMPLE_FRAME
SAMPLE_FIELDS = {"sequence", "kind", "ticks", "renderer_frame", "sample_rate", "channels",
                 "sample_frames", "first_sample_frame", "payload_offset", "payload_bytes"}
OUTCOME_FIELDS = {"sequence", "kind", "blocks", "sample_frames", "payload_bytes", "complete"}


def output_path(path, description, directory=False):
    requested = Path(path)
    resolved = requested.resolve()
    if (requested.exists() or requested.is_symlink() or
            not resolved.is_relative_to(ROOT / "build") or resolved == ROOT / "build"):
        raise RuntimeError(f"{description} must be an absent {'directory' if directory else 'path'} in this checkout's ignored build")
    return resolved


def presentation_boundary(directory):
    with payload_path(directory, "gpu_events.jsonl").open("rb") as stream:
        lines = deque(stream, maxlen=2)
    if len(lines) != 2:
        raise RuntimeError("audio observation has no software presentation boundary")
    presentation, outcome = [json.loads(line, object_pairs_hook=unique_object) for line in lines]
    if (not isinstance(presentation, dict) or presentation.get("kind") != "frame_presentation" or
            not isinstance(outcome, dict) or outcome.get("kind") != "capture_outcome"):
        raise RuntimeError("audio observation has no final software presentation and outcome")
    ticks = unsigned_integer(presentation.get("ticks"), "presentation ticks", 0xFFFFFFFFFFFFFFFF)
    frame = unsigned_integer(presentation.get("renderer_frame"), "presentation frame", 0xFFFFFFFFFFFFFFFF, True)
    complete = outcome.get("complete") is True and outcome.get("outcome") == "software_presentation"
    return ticks, frame, complete


def load_audio_capture(directory):
    paths = {name: payload_path(directory, name) for name in ("audio_events.jsonl", "audio_pcm_s16le.bin")}
    if any(path.stat().st_size > MAXIMUM_AUDIO_BYTES for path in paths.values()):
        raise RuntimeError("audio observation exceeds its bounded extent")
    raw = paths["audio_events.jsonl"].read_bytes()
    pcm = paths["audio_pcm_s16le.bin"].read_bytes()
    if not raw or not raw.endswith(b"\n") or not pcm or len(pcm) % BYTES_PER_BLOCK:
        raise RuntimeError("audio observation needs complete event lines and 160-pair PCM blocks")
    events = [json.loads(line, object_pairs_hook=unique_object) for line in raw.splitlines()]
    if len(events) < 2:
        raise RuntimeError("audio observation needs sample blocks and a final outcome")
    boundary_ticks, boundary_frame, gpu_complete = presentation_boundary(directory)
    configuration = json.loads(payload_path(directory, "capture_configuration.json").read_bytes(),
                               object_pairs_hook=unique_object)
    if not isinstance(configuration, dict) or configuration.get("audio_sink") != "null":
        raise RuntimeError("audio capture must preserve the configured null sink")
    base_ticks = unsigned_integer(configuration.get("base_ticks"), "audio base ticks", 0x7FFFFFFFFFFFFFFF)
    previous_ticks, previous_frame = base_ticks, 0
    for sequence, event in enumerate(events[:-1]):
        if not isinstance(event, dict) or set(event) != SAMPLE_FIELDS or event["kind"] != "audio_samples":
            raise RuntimeError("invalid audio sample event schema")
        if unsigned_integer(event["sequence"], "audio sequence", 0xFFFFFFFFFFFFFFFF) != sequence:
            raise RuntimeError("audio event sequence is incomplete")
        ticks = unsigned_integer(event["ticks"], "audio ticks", 0xFFFFFFFFFFFFFFFF)
        frame = unsigned_integer(event["renderer_frame"], "audio renderer frame", 0xFFFFFFFFFFFFFFFF)
        if ticks < previous_ticks or frame < previous_frame:
            raise RuntimeError("audio timing is not monotonic")
        if ticks > boundary_ticks or frame > boundary_frame:
            raise RuntimeError("audio observation extends past the selected presentation")
        previous_ticks, previous_frame = ticks, frame
        expected = {"sample_rate": SAMPLE_RATE, "channels": CHANNELS,
                    "sample_frames": SAMPLE_FRAMES_PER_BLOCK,
                    "first_sample_frame": sequence * SAMPLE_FRAMES_PER_BLOCK,
                    "payload_offset": sequence * BYTES_PER_BLOCK, "payload_bytes": BYTES_PER_BLOCK}
        for name, value in expected.items():
            if unsigned_integer(event[name], name, 0xFFFFFFFFFFFFFFFF) != value:
                raise RuntimeError(f"audio {name} disagrees with contiguous stereo PCM blocks")
    blocks = len(events) - 1
    outcome = events[-1]
    if not isinstance(outcome, dict) or set(outcome) != OUTCOME_FIELDS or outcome["kind"] != "audio_outcome":
        raise RuntimeError("audio observation needs one final outcome")
    expected = {"sequence": blocks, "blocks": blocks,
                "sample_frames": blocks * SAMPLE_FRAMES_PER_BLOCK, "payload_bytes": blocks * BYTES_PER_BLOCK}
    for name, value in expected.items():
        if unsigned_integer(outcome[name], name, 0xFFFFFFFFFFFFFFFF) != value:
            raise RuntimeError(f"audio outcome {name} disagrees with observed sample blocks")
    if type(outcome["complete"]) is not bool or len(pcm) != expected["payload_bytes"]:
        raise RuntimeError("audio completion type or PCM payload extent disagrees")
    log = (directory / "user/log/reference_capture.log").read_text()
    audio_errors = bool(re.search(r"\b(?:Audio(?:\.[A-Za-z0-9_.]+)?|Service\.DSP)\s+<(?:Error|Critical)>", log))
    return {"raw": raw, "events": events, "pcm": pcm, "blocks": blocks,
            "complete": outcome["complete"] and gpu_complete, "audio_errors": audio_errors}


def sample_statistics(pcm):
    channels = [{"minimum": 32767, "maximum": -32768, "nonzero_samples": 0,
                 "positive_samples": 0, "negative_samples": 0, "sum": 0, "sum_squares": 0}
                for _ in range(CHANNELS)]
    distinct_values = [set() for _ in range(CHANNELS)]
    different_stereo_pairs = 0
    for stereo in struct.iter_unpack("<hh", pcm):
        different_stereo_pairs += stereo[0] != stereo[1]
        for index, sample in enumerate(stereo):
            channel = channels[index]
            channel["minimum"] = min(channel["minimum"], sample)
            channel["maximum"] = max(channel["maximum"], sample)
            channel["nonzero_samples"] += sample != 0
            channel["positive_samples"] += sample > 0
            channel["negative_samples"] += sample < 0
            channel["sum"] += sample
            channel["sum_squares"] += sample * sample
            distinct_values[index].add(sample)
    sample_frames = len(pcm) // BYTES_PER_SAMPLE_FRAME
    for index, channel in enumerate(channels):
        channel["channel"] = "left" if index == 0 else "right"
        channel["peak_absolute"] = max(abs(channel["minimum"]), abs(channel["maximum"]))
        channel["mean"] = channel.pop("sum") / sample_frames
        channel["root_mean_square"] = math.sqrt(channel.pop("sum_squares") / sample_frames)
        channel["distinct_values"] = len(distinct_values[index])
        channel["nonzero_and_varying"] = channel["nonzero_samples"] > 0 and channel["minimum"] < channel["maximum"]
    return {"sample_rate": SAMPLE_RATE, "channels": channels, "sample_frames": sample_frames,
            "nominal_duration_seconds": sample_frames / SAMPLE_RATE,
            "different_left_right_pairs": different_stereo_pairs,
            "both_channels_nonzero_and_varying": all(channel["nonzero_and_varying"] for channel in channels)}


def write_wav(path, pcm):
    if len(pcm) % BYTES_PER_SAMPLE_FRAME:
        raise RuntimeError("WAV payload contains an incomplete stereo sample frame")
    with path.open("xb") as stream:
        with wave.open(stream, "wb") as output:
            output.setnchannels(CHANNELS)
            output.setsampwidth(SAMPLE_WIDTH)
            output.setframerate(SAMPLE_RATE)
            output.writeframes(pcm)


def compare(reference, native, movie=None, preview_directory=None):
    directories = {"reference": Path(reference).resolve(), "native": Path(native).resolve()}
    preview = output_path(preview_directory, "audio preview", directory=True) if preview_directory is not None else None
    captures, errors = {}, {}
    result = {label: str(directory) for label, directory in directories.items()}
    result.update({"passed": False, "audio_validation_errors": errors,
                   "capture_stage": "HLE stereo PCM before host FIFO and volume/stretching callback"})
    for label, directory in directories.items():
        try:
            capture = load_audio_capture(directory)
            captures[label] = capture
            result[label + "_audio_event_sha256"] = digest(capture["raw"])
            result[label + "_pcm_sha256"] = digest(capture["pcm"])
            result[label + "_audio_blocks"] = capture["blocks"]
            result[label + "_sample_statistics"] = sample_statistics(capture["pcm"])
        except (OSError, RuntimeError, ValueError, TypeError, KeyError, struct.error) as error:
            errors[label] = str(error)
    result["input_render_capture"] = compare_input_capture(reference, native, movie=movie)
    if len(captures) == 2:
        reference_capture, native_capture = captures["reference"], captures["native"]
        first_event_difference = next((index for index, pair in enumerate(zip(reference_capture["events"], native_capture["events"]))
                                       if pair[0] != pair[1]), None)
        if first_event_difference is None and len(reference_capture["events"]) != len(native_capture["events"]):
            first_event_difference = min(len(reference_capture["events"]), len(native_capture["events"]))
        first_byte_difference = next((index for index, pair in enumerate(zip(reference_capture["pcm"], native_capture["pcm"]))
                                      if pair[0] != pair[1]), None)
        if first_byte_difference is None and len(reference_capture["pcm"]) != len(native_capture["pcm"]):
            first_byte_difference = min(len(reference_capture["pcm"]), len(native_capture["pcm"]))
        result.update({"audio_events_exact": reference_capture["raw"] == native_capture["raw"],
                       "pcm_payloads_exact": reference_capture["pcm"] == native_capture["pcm"],
                       "both_audio_complete": reference_capture["complete"] and native_capture["complete"],
                       "audio_errors": reference_capture["audio_errors"] or native_capture["audio_errors"],
                       "first_different_audio_event": first_event_difference,
                       "first_different_pcm_byte": first_byte_difference,
                       "first_different_stereo_frame": first_byte_difference // BYTES_PER_SAMPLE_FRAME if first_byte_difference is not None else None,
                       "first_different_channel": (first_byte_difference % BYTES_PER_SAMPLE_FRAME) // SAMPLE_WIDTH if first_byte_difference is not None else None})
        result["passed"] = (not errors and result["audio_events_exact"] and result["pcm_payloads_exact"] and
                            result["both_audio_complete"] and not result["audio_errors"] and
                            result["input_render_capture"]["passed"] and
                            all(result[label + "_sample_statistics"]["both_channels_nonzero_and_varying"] for label in directories))
    if preview is not None and captures:
        preview.mkdir(parents=True, exist_ok=False)
        result["audio_previews"] = []
        for label, capture in captures.items():
            path = preview / f"{label}_audio.wav"
            write_wav(path, capture["pcm"])
            result["audio_previews"].append(str(path))
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("reference", type=Path)
    parser.add_argument("native", type=Path)
    parser.add_argument("--movie", type=Path, help="Original recorded movie; defaults to reference/input_movie.ctm")
    parser.add_argument("--report", type=Path, required=True)
    parser.add_argument("--preview-directory", type=Path, help="Absent ignored directory for unchanged PCM WAV previews")
    arguments = parser.parse_args()
    report = output_path(arguments.report, "report")
    if arguments.preview_directory is not None:
        preview = output_path(arguments.preview_directory, "audio preview", directory=True)
        if report == preview or (report.parent == preview and report.name in ("reference_audio.wav", "native_audio.wav")):
            raise RuntimeError("audio preview cannot also be the report file")
    result = compare(arguments.reference, arguments.native, arguments.movie, arguments.preview_directory)
    report.parent.mkdir(parents=True, exist_ok=True)
    with report.open("x") as stream:
        stream.write(json.dumps(result, indent=2) + "\n")
    print(json.dumps(result, indent=2))
    raise SystemExit(0 if result["passed"] else 1)


if __name__ == "__main__":
    main()
