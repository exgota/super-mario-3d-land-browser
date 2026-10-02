#!/usr/bin/env python3
"""Compare recorded HID delivery, movie structure and complete rendered captures."""
import argparse
from collections import Counter
import json
from pathlib import Path
import struct

from compare_gpu_capture import digest
from compare_rendered_capture import (
    compare as compare_rendered_capture,
    output_path,
    payload_path,
    unique_object,
    unsigned_integer,
)

MAXIMUM_OBSERVATION_BYTES = 64 * 1024 * 1024
MAXIMUM_MOVIE_BYTES = 16 * 1024 * 1024
MOVIE_HEADER_BYTES = 256
MOVIE_RECORD_BYTES = 7
MOVIE_RECORD_TYPES = ("pad_and_circle", "touch", "accelerometer", "gyroscope",
                      "infrared_reset", "extra_hid_response")
BUTTON_MASK = 0x3FFF
DELIVERED_BUTTON_MASK = 0xF0003FFF
INPUT_FIELDS = {"sequence", "kind", "ticks", "renderer_frame", "pad_index", "touch_index",
                "buttons", "delta_additions", "delta_removals", "circle_pad_x", "circle_pad_y",
                "touch_x", "touch_y", "touch_valid"}
OUTCOME_FIELDS = {"sequence", "kind", "polls", "complete"}


def signed_integer(value, description, minimum, maximum):
    if type(value) is not int or not minimum <= value <= maximum:
        raise RuntimeError(f"invalid {description}")
    return value


def circle_direction_bits(x, y):
    # For integer axes bounded by 154, no nonzero rational axis ratio lies on
    # either 30/60-degree threshold. Squared comparisons avoid host rounding.
    if x * x + y * y <= 40 * 40:
        return 0
    bits = 0
    if x and y * y < 3 * x * x:
        bits |= 1 << (28 if x > 0 else 29)
    if not x or 3 * y * y > x * x:
        bits |= 1 << (30 if y > 0 else 31)
    return bits


def read_json(directory, name):
    return json.loads(payload_path(directory, name).read_bytes(), object_pairs_hook=unique_object)


def load_input_capture(directory):
    path = payload_path(directory, "input_events.jsonl")
    if path.stat().st_size > MAXIMUM_OBSERVATION_BYTES:
        raise RuntimeError("input observation exceeds its bounded extent")
    raw = path.read_bytes()
    if not raw or not raw.endswith(b"\n"):
        raise RuntimeError("input observation needs complete JSON lines")
    events = [json.loads(line, object_pairs_hook=unique_object) for line in raw.splitlines()]
    if len(events) < 2:
        raise RuntimeError("input observation needs delivered polls and a final outcome")
    previous_buttons = 0
    previous_ticks, previous_frame = -1, 0
    for sequence, event in enumerate(events[:-1]):
        if not isinstance(event, dict) or set(event) != INPUT_FIELDS or event["kind"] != "hid_input":
            raise RuntimeError("invalid delivered input schema")
        if unsigned_integer(event["sequence"], "input sequence", 0xFFFFFFFFFFFFFFFF) != sequence:
            raise RuntimeError("input sequence is incomplete")
        ticks = unsigned_integer(event["ticks"], "input ticks", 0xFFFFFFFFFFFFFFFF)
        frame = unsigned_integer(event["renderer_frame"], "input renderer frame", 0xFFFFFFFFFFFFFFFF)
        if ticks <= previous_ticks or frame < previous_frame:
            raise RuntimeError("input timing is not monotonic")
        previous_ticks, previous_frame = ticks, frame
        for field in ("pad_index", "touch_index"):
            if unsigned_integer(event[field], field, 7) != sequence % 8:
                raise RuntimeError("input ring index disagrees with poll order")
        buttons = unsigned_integer(event["buttons"], "delivered buttons")
        if buttons & ~DELIVERED_BUTTON_MASK:
            raise RuntimeError("input includes unsupported button bits")
        additions = unsigned_integer(event["delta_additions"], "button additions")
        removals = unsigned_integer(event["delta_removals"], "button removals")
        if additions != (buttons & ~previous_buttons) or removals != (previous_buttons & ~buttons):
            raise RuntimeError("button edges disagree with the delivered ring history")
        previous_buttons = buttons
        for field in ("circle_pad_x", "circle_pad_y"):
            signed_integer(event[field], field, -154, 154)
        if buttons & 0xF0000000 != circle_direction_bits(event["circle_pad_x"], event["circle_pad_y"]):
            raise RuntimeError("circle direction bits disagree with delivered axes")
        unsigned_integer(event["touch_x"], "touch X", 319)
        unsigned_integer(event["touch_y"], "touch Y", 239)
        unsigned_integer(event["touch_valid"], "touch validity", 1)
    outcome = events[-1]
    if not isinstance(outcome, dict) or set(outcome) != OUTCOME_FIELDS or outcome["kind"] != "input_outcome":
        raise RuntimeError("input observation needs one final outcome")
    polls = len(events) - 1
    if (unsigned_integer(outcome["sequence"], "outcome sequence", 0xFFFFFFFFFFFFFFFF) != polls or
            unsigned_integer(outcome["polls"], "outcome poll count", 0xFFFFFFFFFFFFFFFF) != polls or
            type(outcome["complete"]) is not bool):
        raise RuntimeError("input outcome disagrees with delivered polls")
    return {"raw": raw, "polls": events[:-1], "complete": outcome["complete"]}


def load_movie(path):
    path = Path(path)
    if path.is_symlink() or not path.is_file() or not MOVIE_HEADER_BYTES < path.stat().st_size <= MAXIMUM_MOVIE_BYTES:
        raise RuntimeError("movie must be a bounded local regular file with input records")
    raw = path.read_bytes()
    if (len(raw) - MOVIE_HEADER_BYTES) % MOVIE_RECORD_BYTES or raw[:4] != b"CTM\x1b":
        raise RuntimeError("movie magic or seven-byte record extent disagrees")
    program_id = struct.unpack_from("<Q", raw, 4)[0]
    revision = raw[12:32].hex()
    init_time = struct.unpack_from("<Q", raw, 32)[0]
    pad_count = struct.unpack_from("<Q", raw, 84)[0]
    base_ticks = struct.unpack_from("<q", raw, 92)[0]
    if not program_id or not pad_count or base_ticks < 0 or any(raw[100:MOVIE_HEADER_BYTES]):
        raise RuntimeError("invalid pinned movie header metadata")
    pads, touches, types = [], [], []
    awaiting_touch = False
    for offset in range(MOVIE_HEADER_BYTES, len(raw), MOVIE_RECORD_BYTES):
        record = raw[offset:offset + MOVIE_RECORD_BYTES]
        kind = record[0]
        if kind >= len(MOVIE_RECORD_TYPES) or (awaiting_touch and kind != 1) or (kind == 1 and not awaiting_touch):
            raise RuntimeError("movie record type or pad/touch call order disagrees")
        types.append(kind)
        if kind == 0:
            buttons, x, y = struct.unpack_from("<Hhh", record, 1)
            if buttons & ~BUTTON_MASK or not (-154 <= x <= 154 and -154 <= y <= 154):
                raise RuntimeError("movie pad record exceeds supported input bounds")
            pads.append((buttons, x, y))
            awaiting_touch = True
        elif kind == 1:
            x, y, valid = struct.unpack_from("<HHB", record, 1)
            if x >= 320 or y >= 240 or valid > 1 or record[6] != 0:
                raise RuntimeError("invalid movie touch record")
            touches.append((x, y, valid))
            awaiting_touch = False
        elif kind == 4 and (record[5] > 1 or record[6] > 1):
            raise RuntimeError("invalid movie infrared button record")
        elif kind == 5 and any(record[5:7]):
            raise RuntimeError("invalid movie extra-HID padding")
    if awaiting_touch or len(pads) != pad_count or len(touches) != pad_count:
        raise RuntimeError("movie header pad count or final poll completeness disagrees")
    return {"raw": raw, "program_id": program_id, "revision": revision, "init_time": init_time,
            "base_ticks": base_ticks, "pads": pads, "touches": touches,
            "record_counts": dict(Counter(MOVIE_RECORD_TYPES[kind] for kind in types)),
            "record_type_sha256": digest(bytes(types))}


def validate_movie_delivery(directory, movie, capture):
    identity = read_json(directory, "program_identity.json")
    configuration = read_json(directory, "capture_configuration.json")
    if not isinstance(identity, dict) or set(identity) != {"program_id"} or not isinstance(configuration, dict):
        raise RuntimeError("invalid movie provenance metadata")
    program_id = unsigned_integer(identity["program_id"], "program identifier", 0xFFFFFFFFFFFFFFFF, True)
    init_time = unsigned_integer(configuration.get("init_time"), "initial clock", 0xFFFFFFFFFFFFFFFF)
    base_ticks = unsigned_integer(configuration.get("base_ticks"), "base ticks", 0x7FFFFFFFFFFFFFFF)
    if (program_id != movie["program_id"] or configuration.get("source_commit") != movie["revision"] or
            init_time != movie["init_time"] or base_ticks != movie["base_ticks"]):
        raise RuntimeError("movie identity, revision or initial timing disagrees with capture")
    if len(capture["polls"]) != len(movie["pads"]):
        raise RuntimeError("movie pad count disagrees with observed input polls")
    if capture["polls"][0]["ticks"] < base_ticks:
        raise RuntimeError("input poll precedes the movie base tick")
    for event, pad, touch in zip(capture["polls"], movie["pads"], movie["touches"]):
        delivered_pad = (event["buttons"] & BUTTON_MASK, event["circle_pad_x"], event["circle_pad_y"])
        delivered_touch = (event["touch_x"], event["touch_y"], event["touch_valid"])
        if delivered_pad != pad or delivered_touch != touch:
            raise RuntimeError(f"movie disagrees with delivered input at poll {event['sequence']}")


def validate_observation_boundary(directory, capture):
    from collections import deque

    with payload_path(directory, "gpu_events.jsonl").open("rb") as stream:
        final_lines = deque(stream, maxlen=2)
    if len(final_lines) != 2:
        raise RuntimeError("input observation has no rendered capture boundary")
    presentation = json.loads(final_lines[0], object_pairs_hook=unique_object)
    if not isinstance(presentation, dict) or presentation.get("kind") != "frame_presentation":
        raise RuntimeError("input observation has no final software presentation")
    ticks = unsigned_integer(presentation.get("ticks"), "presentation ticks", 0xFFFFFFFFFFFFFFFF)
    frame = unsigned_integer(presentation.get("renderer_frame"), "presentation frame", 0xFFFFFFFFFFFFFFFF)
    if capture["polls"][-1]["ticks"] > ticks or capture["polls"][-1]["renderer_frame"] > frame:
        raise RuntimeError("input observation extends past its rendered boundary")


def validate_script(directory, capture):
    requested = directory / "input_script.txt"
    if not requested.exists() and not requested.is_symlink():
        return {"present": False}
    raw = payload_path(directory, "input_script.txt").read_bytes()
    if len(raw) > 1024 * 1024:
        raise RuntimeError("input script exceeds its bounded receipt extent")
    states = []
    for line in raw.decode("ascii").splitlines():
        values = line.split()
        if len(values) != 7:
            raise RuntimeError("invalid canonical input script receipt")
        frame, buttons, x, y, touch_x, touch_y, valid = map(int, values)
        if (not 0 <= frame <= 1000000 or not 0 <= buttons <= 0xFFF or
                not (-154 <= x <= 154 and -154 <= y <= 154) or
                not (0 <= touch_x < 320 and 0 <= touch_y < 240 and valid in (0, 1)) or
                (states and frame <= states[-1][0]) or len(states) >= 4096):
            raise RuntimeError("invalid or unordered input script receipt state")
        states.append((frame, buttons, x, y, touch_x, touch_y, valid))
    if not states or states[0][0] != 0:
        raise RuntimeError("input script receipt needs an initial frame zero state")
    index, observed_states = 0, set()
    for event in capture["polls"]:
        while index + 1 < len(states) and states[index + 1][0] <= event["renderer_frame"]:
            index += 1
        observed_states.add(index)
        delivered = (event["buttons"] & BUTTON_MASK, event["circle_pad_x"], event["circle_pad_y"],
                     event["touch_x"], event["touch_y"], event["touch_valid"])
        if delivered != states[index][1:]:
            raise RuntimeError(f"script disagrees with delivered input at poll {event['sequence']}")
    if len(observed_states) != len(states):
        raise RuntimeError("input script includes changes without an observed poll")
    return {"present": True, "sha256": digest(raw), "state_count": len(states),
            "all_states_delivered": True}


def channel_evidence(polls):
    predicates = {"buttons": lambda event: bool(event["buttons"] & 0xFFF),
                  "circle_pad": lambda event: bool(event["circle_pad_x"] or event["circle_pad_y"]),
                  "touch": lambda event: event["touch_valid"] == 1}
    result = {}
    for channel, active in predicates.items():
        first = next((index for index, event in enumerate(polls) if active(event)), None)
        released = next((index for index in range(first + 1, len(polls)) if not active(polls[index])), None) if first is not None else None
        result[channel] = {"nonzero_observed": first is not None, "release_observed": released is not None,
                           "first_nonzero_poll": first, "first_release_poll": released,
                           "nonzero_polls": sum(active(event) for event in polls),
                           "inactive_at_boundary": not active(polls[-1])}
    result["all_channels_pressed_and_released"] = all(
        result[channel][field] for channel in predicates
        for field in ("nonzero_observed", "release_observed", "inactive_at_boundary"))
    return result


def compare(reference, native, movie=None, preview_directory=None):
    directories = {"reference": Path(reference).resolve(), "native": Path(native).resolve()}
    movie_path = Path(movie) if movie is not None else directories["reference"] / "input_movie.ctm"
    if preview_directory is not None:
        output_path(preview_directory, "preview", directory=True)
    errors, captures, scripts = {}, {}, {}
    result = {label: str(directory) for label, directory in directories.items()}
    result.update({"movie": str(movie_path.resolve()), "passed": False})
    try:
        movie_capture = load_movie(movie_path)
        result["movie_diagnostics"] = {
            "sha256": digest(movie_capture["raw"]), "bytes": len(movie_capture["raw"]),
            "program_id": movie_capture["program_id"], "revision": movie_capture["revision"],
            "pad_count": len(movie_capture["pads"]), "record_counts": movie_capture["record_counts"],
            "record_type_sha256": movie_capture["record_type_sha256"],
        }
    except (OSError, RuntimeError, ValueError, TypeError, KeyError, struct.error) as error:
        movie_capture = None
        errors["movie"] = str(error)
    for label, directory in directories.items():
        try:
            capture = load_input_capture(directory)
            validate_observation_boundary(directory, capture)
            if movie_capture is not None:
                validate_movie_delivery(directory, movie_capture, capture)
            scripts[label] = validate_script(directory, capture)
            captures[label] = capture
            result[label + "_input_sha256"] = digest(capture["raw"])
            result[label + "_input_polls"] = len(capture["polls"])
            result[label + "_channel_evidence"] = channel_evidence(capture["polls"])
        except (OSError, RuntimeError, ValueError, TypeError, KeyError) as error:
            errors[label] = str(error)
    result["input_validation_errors"] = errors
    result["script_diagnostics"] = scripts
    result["rendered_capture"] = compare_rendered_capture(reference, native, preview_directory)
    if len(captures) == 2:
        reference_capture, native_capture = captures["reference"], captures["native"]
        first_difference = next((index for index, pair in enumerate(zip(reference_capture["polls"], native_capture["polls"]))
                                 if pair[0] != pair[1]), None)
        if first_difference is None and len(reference_capture["polls"]) != len(native_capture["polls"]):
            first_difference = min(len(reference_capture["polls"]), len(native_capture["polls"]))
        result.update({"input_events_exact": reference_capture["raw"] == native_capture["raw"],
                       "both_input_complete": reference_capture["complete"] and native_capture["complete"],
                       "first_different_input_poll": first_difference})
        result["passed"] = (not errors and result["input_events_exact"] and result["both_input_complete"] and
                            result["rendered_capture"]["passed"] and
                            all(result[label + "_channel_evidence"]["all_channels_pressed_and_released"] for label in directories))
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("reference", type=Path)
    parser.add_argument("native", type=Path)
    parser.add_argument("--movie", type=Path, help="Original recorded movie; defaults to reference/input_movie.ctm")
    parser.add_argument("--report", type=Path, required=True)
    parser.add_argument("--preview-directory", type=Path)
    arguments = parser.parse_args()
    report = output_path(arguments.report, "report")
    if arguments.preview_directory is not None and arguments.preview_directory.resolve() == report:
        raise RuntimeError("preview directory cannot also be the report file")
    result = compare(arguments.reference, arguments.native, arguments.movie, arguments.preview_directory)
    report.parent.mkdir(parents=True, exist_ok=True)
    with report.open("x") as stream:
        stream.write(json.dumps(result, indent=2) + "\n")
    print(json.dumps(result, indent=2))
    raise SystemExit(0 if result["passed"] else 1)


if __name__ == "__main__":
    main()
