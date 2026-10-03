#!/usr/bin/env python3
"""Compare one declared GPU window and the complete original movie/input/audio run."""
import argparse
from collections import Counter
import hashlib
import json
import os
from pathlib import Path
import re
import struct

from compare_audio_capture import (
    BYTES_PER_BLOCK, CHANNELS, MAXIMUM_AUDIO_BYTES, OUTCOME_FIELDS as AUDIO_OUTCOME_FIELDS,
    SAMPLE_FIELDS, SAMPLE_FRAMES_PER_BLOCK, SAMPLE_RATE,
)
from compare_input_capture import load_input_capture, load_movie, validate_movie_delivery, validate_script
from compare_rendered_capture import (
    BYTES_PER_PIXEL, MAXIMUM_FRAMEBUFFER_BYTES, MAXIMUM_SCREEN_DIMENSION, SCREEN_IDENTIFIERS,
    output_path, payload_path, read_payload, unique_object, unsigned_integer, write_png,
)

MAXIMUM_GPU_EVENT_BYTES = 256 * 1024 * 1024
MAXIMUM_GPU_LINE_BYTES = 64 * 1024
MAXIMUM_GPU_EVENTS = 2000000
MAXIMUM_PICA_FILES = 1000000
MAXIMUM_PICA_BYTES = 3 * 1024 ** 3
UINT64_MAXIMUM = 0xFFFFFFFFFFFFFFFF
BASE_FIELDS = {"sequence", "kind", "ticks", "frame"}
COUNTER_FIELDS = {"gsp_commands", "pica_lists", "pica_payload_bytes", "buffer_swaps", "vblanks",
                  "hardware_register_writes", "color_fills"}
CALLBACK_FIELDS = {
    "gsp_command": {"command_index", "command_id", "words"},
    "pica_command_list": {"list_index", "physical_address", "size", "chained", "ignored",
                          "payload", "gsp_command_index"},
    "buffer_swap": {"screen_id", "words"},
    "vblank": {"vblank_index"},
    "hardware_register_write": {"address", "value"},
    "color_fill": {"value"},
}
HEADER_FIELDS = {"schema_version", "begin_presentation", "end_presentation", "unobserved_prefix",
                 "gpu_scope", "indices", "payload_scope", "input_audio_scope"}
ARM_FIELDS = {"after_presentation", "before_guest_load", "arming_ticks", "renderer_frame",
              "unexported_prefix_observed"}
PRESENTATION_FIELDS = {"renderer_frame", "presentation_index", "submission_ticks", "vblank_index", "screens"}
CLOSE_FIELDS = {"after_presentation", "closing_ticks", "recorded", "total_observed"}
FOOTER_FIELDS = {"outcome", "complete", "gsp_commands", "pica_lists", "payload_bytes", "vblanks",
                 "gpu_scope", "begin_presentation", "end_presentation", "presentations_observed",
                 "unobserved_prefix", "window_armed", "window_closed", "arming_ticks", "closing_ticks",
                 "first_top_screen_submission_ticks", "unexported_prefix_observed", "recorded", "total_observed"}
GPU_SCOPE = "declared presentation window only"
INPUT_AUDIO_SCOPE = "full execution to final presentation when enabled"
VALIDATION_EXCEPTIONS = (OSError, RuntimeError, ValueError, TypeError, KeyError, struct.error)


def file_identity(path, expected_bytes=None):
    path = Path(path)
    if path.is_symlink() or not path.is_file():
        raise RuntimeError(f"payload is not an ordinary file: {path}")
    before = path.stat()
    if expected_bytes is not None and before.st_size != expected_bytes:
        raise RuntimeError(f"payload extent disagrees: {path.name}")
    digest = hashlib.sha256()
    count = 0
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(4 * 1024 * 1024), b""):
            count += len(block)
            digest.update(block)
    after = path.stat()
    if count != before.st_size or (before.st_dev, before.st_ino, before.st_size, before.st_mtime_ns) != \
            (after.st_dev, after.st_ino, after.st_size, after.st_mtime_ns):
        raise RuntimeError(f"payload changed while reading: {path.name}")
    return {"path": str(path), "bytes": count, "sha256": digest.hexdigest()}


def counters(value):
    if not isinstance(value, dict) or set(value) != COUNTER_FIELDS:
        raise RuntimeError("invalid GPU counter schema")
    return {name: unsigned_integer(value[name], name, UINT64_MAXIMUM) for name in COUNTER_FIELDS}


def words(value):
    if not isinstance(value, list) or not 1 <= len(value) <= 64:
        raise RuntimeError("invalid GPU command word extent")
    for word in value:
        unsigned_integer(word, "GPU command word")


def load_screens(directory, presentation):
    screens = presentation["screens"]
    if not isinstance(screens, list) or len(screens) != len(SCREEN_IDENTIFIERS):
        raise RuntimeError("presentation needs top-left and bottom screen metadata")
    payloads, summaries, identifiers = {}, [], []
    fields = {"screen_id", "storage_width", "storage_height", "width", "height", "rgba_payload", "rgba_bytes",
              "framebuffer_payload", "framebuffer_bytes", "framebuffer_width", "framebuffer_height", "stride",
              "format", "color_format", "active_fb", "physical_address", "right_physical_address", "color_fill"}
    for screen in screens:
        if not isinstance(screen, dict) or set(screen) != fields:
            raise RuntimeError("invalid screen metadata schema")
        identifier = unsigned_integer(screen["screen_id"], "screen identifier")
        if identifier not in SCREEN_IDENTIFIERS or identifier in identifiers:
            raise RuntimeError("invalid or repeated screen identifier")
        identifiers.append(identifier)
        dimensions = {name: unsigned_integer(screen[name], name, MAXIMUM_SCREEN_DIMENSION, True)
                      for name in ("storage_width", "storage_height", "width", "height")}
        if dimensions["width"] != dimensions["storage_height"] or dimensions["height"] != dimensions["storage_width"]:
            raise RuntimeError("landscape dimensions disagree with software storage")
        rgba_bytes = unsigned_integer(screen["rgba_bytes"], "RGBA bytes", 4 * MAXIMUM_SCREEN_DIMENSION ** 2, True)
        if rgba_bytes != dimensions["width"] * dimensions["height"] * 4:
            raise RuntimeError("RGBA extent disagrees with dimensions")
        width = unsigned_integer(screen["framebuffer_width"], "framebuffer width", 0xFFFF, True)
        height = unsigned_integer(screen["framebuffer_height"], "framebuffer height", 0xFFFF, True)
        stride = unsigned_integer(screen["stride"], "framebuffer stride", positive=True)
        extent = unsigned_integer(screen["framebuffer_bytes"], "framebuffer bytes", MAXIMUM_FRAMEBUFFER_BYTES, True)
        color_format = unsigned_integer(screen["color_format"], "framebuffer color format", 4)
        raw_format = unsigned_integer(screen["format"], "raw framebuffer format")
        pixel_bytes = BYTES_PER_PIXEL[color_format]
        if extent != stride * height or color_format != (raw_format & 7) or \
                stride % pixel_bytes or stride < width * pixel_bytes:
            raise RuntimeError("framebuffer format, stride or extent disagrees")
        unsigned_integer(screen["active_fb"], "active framebuffer")
        address = unsigned_integer(screen["physical_address"], "physical framebuffer address", positive=True)
        unsigned_integer(screen["right_physical_address"], "right framebuffer address")
        unsigned_integer(screen["color_fill"], "LCD color fill")
        if address + extent > 0xFFFFFFFF:
            raise RuntimeError("framebuffer extent overflows")
        expected_names = (f"rendered_screen_{identifier}.rgba", f"framebuffer_screen_{identifier}.bin")
        if (screen["rgba_payload"], screen["framebuffer_payload"]) != expected_names:
            raise RuntimeError("screen payload filenames disagree")
        for name, size in zip(expected_names, (rgba_bytes, extent)):
            payloads[name] = file_identity(payload_path(directory, name), size)
        rgba = read_payload(directory, expected_names[0], rgba_bytes)
        colors = Counter(value for value, in struct.iter_unpack(">I", rgba))
        summaries.append({"screen_id": identifier, "width": dimensions["width"], "height": dimensions["height"],
                          "rgba_sha256": payloads[expected_names[0]]["sha256"],
                          "framebuffer_sha256": payloads[expected_names[1]]["sha256"],
                          "distinct_rgba_colors": len(colors), "distinct_rgb_colors": len({value >> 8 for value in colors})})
    if set(identifiers) != set(SCREEN_IDENTIFIERS):
        raise RuntimeError("presentation screen identifiers are incomplete")
    for pattern, expected in (("rendered_screen_*.rgba", {f"rendered_screen_{i}.rgba" for i in identifiers}),
                              ("framebuffer_screen_*.bin", {f"framebuffer_screen_{i}.bin" for i in identifiers})):
        if {path.name for path in directory.glob(pattern)} != expected:
            raise RuntimeError("unreferenced rendered payload remains")
    return payloads, summaries


def load_window_capture(directory):
    if not directory.is_dir() or os.path.lexists(directory / "gpu_events.jsonl"):
        raise RuntimeError("window capture must be an ordinary window dataset without a whole-prefix GPU stream")
    path = payload_path(directory, "gpu_window_events.jsonl")
    if not 1 <= path.stat().st_size <= MAXIMUM_GPU_EVENT_BYTES:
        raise RuntimeError("GPU window event stream exceeds its bounded extent")
    configuration = json.loads(payload_path(directory, "capture_configuration.json").read_bytes(),
                               object_pairs_hook=unique_object)
    if not isinstance(configuration, dict) or configuration.get("gpu_observation_scope") != "presentation_window" or \
            configuration.get("gpu_event_file") != "gpu_window_events.jsonl" or \
            configuration.get("input_audio_scope") != INPUT_AUDIO_SCOPE:
        raise RuntimeError("capture configuration does not declare the GPU window")
    beginning = unsigned_integer(configuration.get("gpu_window_begin_presentation"), "window beginning", 18000, True)
    end = unsigned_integer(configuration.get("gpu_window_end_presentation"), "window end", 18000, True)
    if beginning > end or unsigned_integer(configuration.get("presentation_limit"), "presentation limit", 18000, True) != end:
        raise RuntimeError("invalid or inconsistent GPU window range")
    unsigned_integer(configuration.get("wall_time_seconds"), "wall bound", 3600, True)
    pica_limit = unsigned_integer(configuration.get("pica_payload_limit_bytes"), "PICA bound", MAXIMUM_PICA_BYTES, True)
    for name, expected in {"renderer": "software", "console": "old 3DS", "audio_sink": "null",
                           "deterministic_async_operations": True, "async_fs_operations": False,
                           "deterministic_file_read_delay_correction": True}.items():
        if configuration.get(name) != expected or type(configuration.get(name)) is not type(expected):
            raise RuntimeError(f"invalid window configuration: {name}")
    counts = dict.fromkeys(COUNTER_FIELDS, 0)
    payloads, event_counts = {}, Counter()
    header = arm = presentation = close = outcome = last_vblank = None
    phase, previous_ticks, byte_count, event_count = "header", 0, 0, 0
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        while line := stream.readline(MAXIMUM_GPU_LINE_BYTES + 1):
            if len(line) > MAXIMUM_GPU_LINE_BYTES or not line.endswith(b"\n") or event_count >= MAXIMUM_GPU_EVENTS:
                raise RuntimeError("GPU window needs bounded complete JSON lines")
            digest.update(line)
            byte_count += len(line)
            event = json.loads(line, object_pairs_hook=unique_object)
            if not isinstance(event, dict) or unsigned_integer(event.get("sequence"), "GPU sequence", UINT64_MAXIMUM) != event_count or \
                    event.get("frame") != 0 or type(event.get("frame")) is not int:
                raise RuntimeError("GPU event sequence/frame is invalid")
            ticks = unsigned_integer(event.get("ticks"), "GPU ticks", UINT64_MAXIMUM)
            if ticks < previous_ticks:
                raise RuntimeError("GPU window ticks are not monotonic")
            previous_ticks = ticks
            event_count += 1
            kind = event.get("kind")
            if not isinstance(kind, str):
                raise RuntimeError("invalid GPU event kind")
            event_counts[kind] += 1
            if phase == "header":
                if kind != "gpu_observation_window" or set(event) != BASE_FIELDS | HEADER_FIELDS or ticks != 0:
                    raise RuntimeError("window has no initial coverage declaration")
                expected = {"schema_version": 1, "begin_presentation": beginning, "end_presentation": end,
                            "unobserved_prefix": beginning > 1, "gpu_scope": GPU_SCOPE,
                            "indices": "absolute command/list/VBlank counters; local window event sequence", "payload_scope": "exported window only",
                            "input_audio_scope": INPUT_AUDIO_SCOPE}
                if any(event[name] != value or type(event[name]) is not type(value) for name, value in expected.items()):
                    raise RuntimeError("window coverage declaration disagrees")
                header, phase = event, "arm"
            elif phase == "arm":
                if kind != "gpu_observation_window_arm" or set(event) != BASE_FIELDS | ARM_FIELDS or \
                        unsigned_integer(event["after_presentation"], "arming presentation", 17999) != beginning - 1 or \
                        type(event["before_guest_load"]) is not bool or event["before_guest_load"] != (beginning == 1):
                    raise RuntimeError("window has no precise declared arming boundary")
                prefix = counters(event["unexported_prefix_observed"])
                if beginning == 1:
                    if ticks != 0 or event["arming_ticks"] is not None or event["renderer_frame"] is not None or any(prefix.values()):
                        raise RuntimeError("initial arming must have unavailable ticks/frame and no prefix")
                else:
                    if unsigned_integer(event["arming_ticks"], "arming ticks", UINT64_MAXIMUM) != ticks:
                        raise RuntimeError("arming ticks disagree")
                    unsigned_integer(event["renderer_frame"], "arming renderer frame", UINT64_MAXIMUM, True)
                    if prefix["vblanks"] < beginning - 1:
                        raise RuntimeError("arming lacks the declared observed presentation prefix")
                arm, phase = event, "callbacks"
            elif phase == "callbacks" and kind in CALLBACK_FIELDS:
                if set(event) != BASE_FIELDS | CALLBACK_FIELDS[kind]:
                    raise RuntimeError(f"invalid GPU callback schema: {kind}")
                if kind == "gsp_command":
                    if unsigned_integer(event["command_index"], "command index", UINT64_MAXIMUM) != prefix["gsp_commands"] + counts["gsp_commands"]:
                        raise RuntimeError("absolute GSP command indices are discontinuous")
                    unsigned_integer(event["command_id"], "GSP command identifier")
                    words(event["words"])
                    counts["gsp_commands"] += 1
                elif kind == "pica_command_list":
                    index = unsigned_integer(event["list_index"], "PICA list index", UINT64_MAXIMUM)
                    if index != prefix["pica_lists"] + counts["pica_lists"] or counts["pica_lists"] >= MAXIMUM_PICA_FILES:
                        raise RuntimeError("absolute PICA indices or file bound disagree")
                    unsigned_integer(event["physical_address"], "PICA physical address")
                    size = unsigned_integer(event["size"], "PICA size", pica_limit)
                    if size % 4 or counts["pica_payload_bytes"] + size > pica_limit or \
                            type(event["chained"]) is not bool or type(event["ignored"]) is not bool:
                        raise RuntimeError("invalid or oversized PICA payload")
                    name = f"pica_command_list_{index:06d}.bin"
                    if event["payload"] != name or name in payloads or \
                            unsigned_integer(event["gsp_command_index"], "PICA GSP index", UINT64_MAXIMUM) != \
                            max(prefix["gsp_commands"] + counts["gsp_commands"] - 1, 0):
                        raise RuntimeError("PICA path or absolute GSP association disagrees")
                    payloads[name] = file_identity(payload_path(directory, name), size)
                    counts["pica_lists"] += 1
                    counts["pica_payload_bytes"] += size
                elif kind == "vblank":
                    if unsigned_integer(event["vblank_index"], "VBlank index", UINT64_MAXIMUM) != prefix["vblanks"] + counts["vblanks"]:
                        raise RuntimeError("absolute VBlank indices are discontinuous")
                    counts["vblanks"] += 1
                    last_vblank = event
                elif kind == "buffer_swap":
                    unsigned_integer(event["screen_id"], "buffer swap screen")
                    words(event["words"])
                    counts["buffer_swaps"] += 1
                elif kind == "hardware_register_write":
                    unsigned_integer(event["address"], "hardware register address")
                    unsigned_integer(event["value"], "hardware register value")
                    counts["hardware_register_writes"] += 1
                else:
                    unsigned_integer(event["value"], "color fill value")
                    counts["color_fills"] += 1
            elif phase == "callbacks" and kind == "frame_presentation":
                if set(event) != BASE_FIELDS | PRESENTATION_FIELDS or \
                        unsigned_integer(event["presentation_index"], "final presentation index", 17999) != end - 1:
                    raise RuntimeError("window has no selected final presentation")
                unsigned_integer(event["renderer_frame"], "final renderer frame", UINT64_MAXIMUM, True)
                unsigned_integer(event["submission_ticks"], "first top submission ticks", UINT64_MAXIMUM)
                unsigned_integer(event["vblank_index"], "final VBlank index", UINT64_MAXIMUM)
                if last_vblank is None or event["vblank_index"] != last_vblank["vblank_index"] or ticks != last_vblank["ticks"]:
                    raise RuntimeError("final presentation does not identify the actual final VBlank")
                if beginning > 1 and (counts["vblanks"] != end - beginning + 1 or event["renderer_frame"] <= arm["renderer_frame"]):
                    raise RuntimeError("window presentation extent or subsequent boundary disagrees")
                presentation, phase = event, "close"
            elif phase == "close":
                if kind != "gpu_observation_window_close" or set(event) != BASE_FIELDS | CLOSE_FIELDS or \
                        unsigned_integer(event["after_presentation"], "closing presentation", 18000) != end or \
                        unsigned_integer(event["closing_ticks"], "closing ticks", UINT64_MAXIMUM) != presentation["ticks"] or ticks != presentation["ticks"]:
                    raise RuntimeError("window has no complete declared closing boundary")
                if counters(event["recorded"]) != counts or counters(event["total_observed"]) != \
                        {name: prefix[name] + counts[name] for name in COUNTER_FIELDS}:
                    raise RuntimeError("closing counters disagree with recorded callbacks and prefix")
                close, phase = event, "outcome"
            elif phase == "outcome":
                if kind != "capture_outcome" or set(event) != BASE_FIELDS | FOOTER_FIELDS:
                    raise RuntimeError("window has no final capture outcome")
                expected = {"outcome": "software_presentation", "complete": True, "gpu_scope": GPU_SCOPE,
                            "begin_presentation": beginning, "end_presentation": end, "presentations_observed": end,
                            "unobserved_prefix": beginning > 1, "window_armed": True, "window_closed": True,
                            "arming_ticks": arm["arming_ticks"], "closing_ticks": presentation["ticks"],
                            "first_top_screen_submission_ticks": presentation["submission_ticks"],
                            "gsp_commands": close["total_observed"]["gsp_commands"],
                            "pica_lists": close["total_observed"]["pica_lists"], "payload_bytes": counts["pica_payload_bytes"],
                            "vblanks": close["total_observed"]["vblanks"]}
                if any(event[name] != value or type(event[name]) is not type(value) for name, value in expected.items()) or \
                        counters(event["unexported_prefix_observed"]) != prefix or counters(event["recorded"]) != counts or \
                        counters(event["total_observed"]) != counters(close["total_observed"]):
                    raise RuntimeError("window outcome, coverage or counters are incomplete/inconsistent")
                outcome, phase = event, "finished"
            else:
                raise RuntimeError("GPU event occurs outside the declared ordered window")
    if phase != "finished" or byte_count != path.stat().st_size:
        raise RuntimeError("GPU window is incomplete or changed while reading")
    event_identity = {"path": str(path), "bytes": byte_count, "sha256": digest.hexdigest()}
    if file_identity(path) != event_identity:
        raise RuntimeError("GPU window bytes changed during validation")
    if set(payloads) != {item.name for item in directory.glob("pica_command_list_*.bin")}:
        raise RuntimeError("unreferenced PICA payload remains")
    screen_payloads, screens = load_screens(directory, presentation)
    payloads.update(screen_payloads)
    return {"event_identity": event_identity, "payloads": payloads, "configuration": configuration,
            "header": header, "arm": arm, "presentation": presentation, "close": close, "outcome": outcome,
            "event_counts": dict(event_counts), "screens": screens}


def load_full_audio(directory, presentation):
    paths = {name: payload_path(directory, name) for name in ("audio_events.jsonl", "audio_pcm_s16le.bin")}
    if any(path.stat().st_size > MAXIMUM_AUDIO_BYTES for path in paths.values()):
        raise RuntimeError("full-run audio exceeds its existing bounded extent")
    raw, pcm = paths["audio_events.jsonl"].read_bytes(), paths["audio_pcm_s16le.bin"].read_bytes()
    if not raw or not raw.endswith(b"\n") or not pcm or len(pcm) % BYTES_PER_BLOCK:
        raise RuntimeError("full-run audio needs complete JSON lines and stereo PCM blocks")
    events = [json.loads(line, object_pairs_hook=unique_object) for line in raw.splitlines()]
    if len(events) < 2:
        raise RuntimeError("full-run audio needs sample blocks and a final outcome")
    configuration = json.loads(payload_path(directory, "capture_configuration.json").read_bytes(), object_pairs_hook=unique_object)
    previous_ticks = unsigned_integer(configuration.get("base_ticks"), "audio base ticks", 0x7FFFFFFFFFFFFFFF)
    previous_frame = 0
    for sequence, event in enumerate(events[:-1]):
        if not isinstance(event, dict) or set(event) != SAMPLE_FIELDS or event["kind"] != "audio_samples" or \
                unsigned_integer(event["sequence"], "audio sequence", UINT64_MAXIMUM) != sequence:
            raise RuntimeError("invalid or discontinuous audio sample schema")
        ticks = unsigned_integer(event["ticks"], "audio ticks", UINT64_MAXIMUM)
        frame = unsigned_integer(event["renderer_frame"], "audio renderer frame", UINT64_MAXIMUM)
        if ticks < previous_ticks or frame < previous_frame or ticks > presentation["ticks"] or frame > presentation["renderer_frame"]:
            raise RuntimeError("full-run audio timing disagrees with its final boundary")
        previous_ticks, previous_frame = ticks, frame
        expected = {"sample_rate": SAMPLE_RATE, "channels": CHANNELS, "sample_frames": SAMPLE_FRAMES_PER_BLOCK,
                    "first_sample_frame": sequence * SAMPLE_FRAMES_PER_BLOCK,
                    "payload_offset": sequence * BYTES_PER_BLOCK, "payload_bytes": BYTES_PER_BLOCK}
        if any(unsigned_integer(event[name], name, UINT64_MAXIMUM) != value for name, value in expected.items()):
            raise RuntimeError("audio sample extent/rate/channel metadata disagrees")
    blocks, outcome = len(events) - 1, events[-1]
    if not isinstance(outcome, dict) or set(outcome) != AUDIO_OUTCOME_FIELDS or outcome["kind"] != "audio_outcome":
        raise RuntimeError("audio has no final outcome")
    expected = {"sequence": blocks, "blocks": blocks, "sample_frames": blocks * SAMPLE_FRAMES_PER_BLOCK,
                "payload_bytes": blocks * BYTES_PER_BLOCK}
    if any(unsigned_integer(outcome[name], name, UINT64_MAXIMUM) != value for name, value in expected.items()) or \
            outcome["complete"] is not True or len(pcm) != expected["payload_bytes"]:
        raise RuntimeError("audio outcome or payload is incomplete")
    return {"raw": raw, "pcm": pcm, "blocks": blocks,
            "event_sha256": hashlib.sha256(raw).hexdigest(), "pcm_sha256": hashlib.sha256(pcm).hexdigest()}


def files_exact(first, second):
    paths = [Path(record["path"]) for record in (first, second)]
    if any(path.is_symlink() or not path.is_file() for path in paths):
        raise RuntimeError("validated payload is no longer an ordinary file")
    before = [path.stat() for path in paths]
    digests = [hashlib.sha256(), hashlib.sha256()]
    counts = [0, 0]
    equal = True
    with paths[0].open("rb") as a, paths[1].open("rb") as b:
        while True:
            left, right = a.read(4 * 1024 * 1024), b.read(4 * 1024 * 1024)
            equal = equal and left == right
            for index, block in enumerate((left, right)):
                counts[index] += len(block)
                digests[index].update(block)
            if not left and not right:
                break
    after = [path.stat() for path in paths]
    for index, record in enumerate((first, second)):
        if counts[index] != record["bytes"] or digests[index].hexdigest() != record["sha256"] or \
                (before[index].st_dev, before[index].st_ino, before[index].st_size, before[index].st_mtime_ns) != \
                (after[index].st_dev, after[index].st_ino, after[index].st_size, after[index].st_mtime_ns):
            raise RuntimeError("validated payload changed before/during byte comparison")
    return equal


def compare(reference, native, movie, preview_directory=None):
    directories = {"reference": Path(reference).resolve(), "native": Path(native).resolve()}
    result = {label: str(directory) for label, directory in directories.items()}
    result.update({"passed": False, "scope": "Declared GPU window and complete original movie/input/audio run",
                   "full_gpu_prefix_compared": False, "game_state_semantics_compared": False,
                   "validation_errors": {}})
    captures, inputs, audio, scripts = {}, {}, {}, {}
    try:
        movie_capture = load_movie(movie)
        result["movie"] = file_identity(movie)
    except VALIDATION_EXCEPTIONS as exception:
        movie_capture = None
        result["validation_errors"]["movie"] = str(exception)
    for label, directory in directories.items():
        try:
            capture = load_window_capture(directory)
            if capture["configuration"].get("backend") != ("stock Dynarmic" if label == "reference" else "static recompiler"):
                raise RuntimeError("window has the wrong original/native backend")
            input_capture = load_input_capture(directory)
            if input_capture["complete"] is not True or input_capture["polls"][-1]["ticks"] > capture["presentation"]["ticks"] or \
                    input_capture["polls"][-1]["renderer_frame"] > capture["presentation"]["renderer_frame"]:
                raise RuntimeError("full-run input is incomplete or extends past the final boundary")
            if movie_capture is not None:
                validate_movie_delivery(directory, movie_capture, input_capture)
            scripts[label] = validate_script(directory, input_capture)
            audio_capture = load_full_audio(directory, capture["presentation"])
            log_path = payload_path(directory / "user/log", "reference_capture.log")
            if log_path.stat().st_size > 64 * 1024 * 1024:
                raise RuntimeError("reference log exceeds its bounded extent")
            log = log_path.read_text()
            if re.search(r"\b(?:Movie(?:\.[A-Za-z0-9_.]+)?|Audio(?:\.[A-Za-z0-9_.]+)?|Service\.DSP)\s+<(?:Error|Critical)>", log):
                raise RuntimeError("movie or audio log reports an error")
            captures[label], inputs[label], audio[label] = capture, input_capture, audio_capture
            result[label + "_coverage"] = {"header": capture["header"], "arm": capture["arm"], "close": capture["close"], "outcome": capture["outcome"]}
            result[label + "_gpu_events"] = capture["event_identity"]
            result[label + "_gpu_event_counts"] = capture["event_counts"]
            result[label + "_screens"] = capture["screens"]
            result[label + "_payloads"] = capture["payloads"]
            result[label + "_input_polls"] = len(input_capture["polls"])
            result[label + "_input_sha256"] = hashlib.sha256(input_capture["raw"]).hexdigest()
            result[label + "_audio_blocks"] = audio_capture["blocks"]
            result[label + "_audio_event_sha256"] = audio_capture["event_sha256"]
            result[label + "_pcm_sha256"] = audio_capture["pcm_sha256"]
        except VALIDATION_EXCEPTIONS as exception:
            result["validation_errors"][label] = str(exception)
    result["script_diagnostics"] = scripts
    if len(captures) == 2 and movie_capture is not None:
        try:
            first, second = captures["reference"], captures["native"]
            config_first, config_second = first["configuration"], second["configuration"]
            # Backend identity differs by design. Every other configuration field
            # remains equal; GPU, input, audio and payload bytes are never normalized.
            configuration_exact = set(config_first) == set(config_second) and all(
                config_first[name] == config_second[name] and type(config_first[name]) is type(config_second[name])
                for name in config_first if name != "backend")
            events_exact = files_exact(first["event_identity"], second["event_identity"])
            payloads_exact = set(first["payloads"]) == set(second["payloads"])
            if payloads_exact:
                for name in sorted(first["payloads"]):
                    payloads_exact = files_exact(first["payloads"][name], second["payloads"][name]) and payloads_exact
            result.update({"configuration_exact_except_declared_backend": configuration_exact,
                           "gpu_window_events_exact": events_exact, "gpu_window_payloads_exact": payloads_exact,
                           "input_events_exact": inputs["reference"]["raw"] == inputs["native"]["raw"],
                           "audio_events_exact": audio["reference"]["raw"] == audio["native"]["raw"],
                           "pcm_payloads_exact": audio["reference"]["pcm"] == audio["native"]["pcm"]})
            result["passed"] = not result["validation_errors"] and all(result[name] for name in
                ("configuration_exact_except_declared_backend", "gpu_window_events_exact", "gpu_window_payloads_exact",
                 "input_events_exact", "audio_events_exact", "pcm_payloads_exact"))
        except VALIDATION_EXCEPTIONS as exception:
            result["validation_errors"]["comparison"] = str(exception)
    if preview_directory is not None and captures:
        preview = output_path(preview_directory, "window pixel preview", directory=True)
        preview.mkdir(parents=True, exist_ok=False)
        result["previews"] = []
        for label, capture in captures.items():
            for screen in capture["screens"]:
                name = f"rendered_screen_{screen['screen_id']}.rgba"
                data = read_payload(directories[label], name, capture["payloads"][name]["bytes"])
                target = preview / f"{label}_screen_{screen['screen_id']}.png"
                write_png(target, screen["width"], screen["height"], data)
                result["previews"].append(str(target))
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("reference", type=Path)
    parser.add_argument("native", type=Path)
    parser.add_argument("--movie", type=Path, required=True, help="The original window recording's own unchanged movie")
    parser.add_argument("--report", type=Path, required=True)
    parser.add_argument("--preview-directory", type=Path)
    arguments = parser.parse_args()
    report = output_path(arguments.report, "window report")
    if arguments.preview_directory is not None:
        preview = output_path(arguments.preview_directory, "window pixel preview", directory=True)
        if report == preview or report.is_relative_to(preview):
            raise RuntimeError("window preview cannot contain its report")
    result = compare(arguments.reference, arguments.native, arguments.movie, arguments.preview_directory)
    report.parent.mkdir(parents=True, exist_ok=True)
    with report.open("x") as stream:
        stream.write(json.dumps(result, indent=2) + "\n")
    print(json.dumps(result, indent=2))
    raise SystemExit(0 if result["passed"] else 1)


if __name__ == "__main__":
    main()
