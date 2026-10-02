#!/usr/bin/env python3
"""Compare passive raw RAM observations and exact movie/platform output."""
import argparse
import json
from pathlib import Path
import re

from compare_gpu_capture import digest
from compare_movie_replay import compare as compare_movie, first_difference
from compare_rendered_capture import output_path, payload_path


def integer(value, maximum=(1 << 64) - 1):
    if type(value) is not int or not 0 <= value <= maximum:
        raise ValueError("Observation integer is outside its exact unsigned range")
    return value


def unique_object(pairs):
    result = {}
    for name, value in pairs:
        if name in result:
            raise ValueError("Duplicate observation JSON member")
        result[name] = value
    return result


def keys(value, expected):
    if type(value) is not dict or set(value) != set(expected.split()):
        raise ValueError("Observation members differ from the supported schema")


def hexadecimal_bytes(value, count):
    if type(value) is not str or not re.fullmatch(r"[0-9a-f]{%d}" % (count * 2), value):
        raise ValueError("Observation bytes need the exact lowercase hexadecimal extent")
    return bytes.fromhex(value)


def load_plan(raw):
    if not 0 < len(raw) <= 256 * 1024 or not raw.endswith(b"\n"):
        raise ValueError("Observation plan extent or final LF is invalid")
    lines = raw.decode("ascii").splitlines()
    if len(lines) < 3 or lines[0] != "version 1":
        raise ValueError("Observation plan version or field list is missing")
    if not re.fullmatch(r"frames [0-9]+(?: [0-9]+)*", lines[1]):
        raise ValueError("Observation frames are not canonical")
    frames = [integer(int(token)) for token in lines[1].split()[1:]]
    if not 1 <= len(frames) <= 1024 or frames != sorted(set(frames)):
        raise ValueError("Observation frames must be bounded and strictly increasing")
    fields = []
    names = set()
    for line in lines[2:]:
        match = re.fullmatch(r"field ([a-z][a-z0-9]*(?:_[a-z0-9]+)*) ([0-9]+) "
                             r"(0x[0-9a-f]{8}) ((?:0x[0-9a-f]{8} ?)+)", line)
        if match is None:
            raise ValueError("Observation field plan is not canonical")
        name, count, base, offsets = match.groups()
        count = integer(int(count), 256)
        offsets = [int(token, 16) for token in offsets.split()]
        if not count or len(name) > 64 or name in names or not 1 <= len(offsets) <= 16:
            raise ValueError("Observation field extent, name or pointer depth is invalid")
        names.add(name)
        fields.append({"name": name, "byte_count": count, "base_address": int(base, 16),
                       "offsets": offsets})
    if not 1 <= len(fields) <= 128:
        raise ValueError("Observation field count is outside the supported bound")
    canonical = "version 1\nframes " + " ".join(map(str, frames)) + "\n"
    for field in fields:
        canonical += "field %s %d 0x%08x %s\n" % (
            field["name"], field["byte_count"], field["base_address"],
            " ".join("0x%08x" % value for value in field["offsets"]))
    if canonical.encode("ascii") != raw:
        raise ValueError("Observation plan differs from its canonical receipt")
    return frames, fields


def load_capture(directory):
    plan_raw = payload_path(directory, "state_plan.txt").read_bytes()
    raw = payload_path(directory, "state_events.jsonl").read_bytes()
    if not raw or not raw.endswith(b"\n") or len(raw) + len(plan_raw) > 16 * 1024 * 1024:
        raise ValueError("Observation output exceeds its bound or is truncated")
    frames, fields = load_plan(plan_raw)
    events = [json.loads(line, object_pairs_hook=unique_object) for line in raw.splitlines()]
    if len(events) != len(frames) + 1:
        raise ValueError("Every requested frame needs one observation and one final outcome")
    previous_ticks = previous_vblank = 0
    for sequence, (event, frame) in enumerate(zip(events[:-1], frames)):
        keys(event, "sequence kind renderer_frame ticks vblank_index fields")
        if integer(event["sequence"]) != sequence or event["kind"] != "state_observation":
            raise ValueError("Missing or reordered state observation")
        if integer(event["renderer_frame"]) != frame:
            raise ValueError("State observation substitutes another requested frame")
        ticks, vblank = integer(event["ticks"]), integer(event["vblank_index"])
        if ticks < previous_ticks or vblank < previous_vblank:
            raise ValueError("State observation clock moved backwards")
        previous_ticks, previous_vblank = ticks, vblank
        if type(event["fields"]) is not list or len(event["fields"]) != len(fields):
            raise ValueError("Observation field coverage is incomplete")
        for value, field in zip(event["fields"], fields):
            keys(value, "name byte_count base_address address status bytes pointer_reads")
            if (value["name"] != field["name"] or value["status"] != "available" or
                    integer(value["byte_count"], 256) != field["byte_count"] or
                    integer(value["base_address"], 0xffffffff) != field["base_address"]):
                raise ValueError("Observation field is unavailable or differs from its plan")
            current = field["base_address"]
            traces = value["pointer_reads"]
            if type(traces) is not list or len(traces) != len(field["offsets"]) - 1:
                raise ValueError("Observation pointer-chain coverage is incomplete")
            for trace, offset in zip(traces, field["offsets"][:-1]):
                keys(trace, "offset read_address status value bytes")
                address = current + offset
                if (not current or address + 4 > 1 << 32 or trace["status"] != "available" or
                        integer(trace["offset"], 0xffffffff) != offset or
                        integer(trace["read_address"], 0xffffffff) != address):
                    raise ValueError("Observation pointer read is unavailable or inconsistent")
                current = integer(trace["value"], 0xffffffff)
                if not current or int.from_bytes(hexadecimal_bytes(trace["bytes"], 4), "little") != current:
                    raise ValueError("Observation pointer value differs from its original bytes")
            address = current + field["offsets"][-1]
            if (not current or address + field["byte_count"] > 1 << 32 or
                    integer(value["address"], 0xffffffff) != address):
                raise ValueError("Observation final address differs from its pointer trace")
            hexadecimal_bytes(value["bytes"], field["byte_count"])
    outcome = events[-1]
    keys(outcome, "sequence kind requested_frames fields_per_frame observed_frames missing_frames "
         "observed_fields unavailable_fields capture_complete all_requested_frames_observed "
         "all_observed_fields_available complete")
    expected = {"sequence": len(frames), "requested_frames": len(frames), "fields_per_frame": len(fields),
                "observed_frames": len(frames), "missing_frames": 0,
                "observed_fields": len(frames) * len(fields), "unavailable_fields": 0}
    if outcome["kind"] != "state_outcome" or any(integer(outcome[name]) != value
                                                for name, value in expected.items()):
        raise ValueError("Observation outcome counts are inconsistent")
    if any(outcome[name] is not True for name in ("capture_complete", "all_requested_frames_observed",
                                                 "all_observed_fields_available", "complete")):
        raise ValueError("Missing or unavailable observation cannot receive complete credit")
    gpu_events = [json.loads(line, object_pairs_hook=unique_object) for line in
                  payload_path(directory, "gpu_events.jsonl").read_bytes().splitlines()]
    if any(type(event) is not dict for event in gpu_events):
        raise ValueError("GPU event is not an observation anchor object")
    vblanks = {}
    for event in gpu_events:
        if event.get("kind") == "vblank":
            index, ticks = integer(event["vblank_index"]), integer(event["ticks"])
            if index in vblanks:
                raise ValueError("Observation anchor has a duplicate VBlank index")
            vblanks[index] = ticks
    presentations = [event for event in gpu_events if event.get("kind") == "frame_presentation"]
    if len(presentations) != 1:
        raise ValueError("Observation requires the selected natural presentation capture")
    boundary = presentations[0]
    for name in ("renderer_frame", "ticks", "vblank_index"):
        integer(boundary[name])
    for event in events[:-1]:
        if (vblanks.get(event["vblank_index"]) != event["ticks"] or
                event["renderer_frame"] > boundary["renderer_frame"] or
                event["ticks"] > boundary["ticks"]):
            raise ValueError("Observation does not lie at its recorded natural VBlank edge")
        if event["renderer_frame"] == boundary["renderer_frame"] and any(
                event[name] != boundary[name] for name in ("ticks", "vblank_index")):
            raise ValueError("Final observation differs from its presentation boundary")
    return {"plan": plan_raw, "raw": raw, "events": events, "frames": frames,
            "field_count": len(fields)}


def compare(reference, native, movie=None):
    directories = {"reference": Path(reference).resolve(), "native": Path(native).resolve()}
    captures, errors = {}, {}
    result = {"passed": False, "scope": "Raw guest RAM at natural presentations; field semantics and "
              "completed gameplay-update phase remain unverified"}
    for name, directory in directories.items():
        try:
            capture = load_capture(directory)
            captures[name] = capture
            result[name] = {"plan_sha256": digest(capture["plan"]), "events_sha256": digest(capture["raw"]),
                            "frames": capture["frames"], "field_count": capture["field_count"]}
        except (OSError, ValueError, TypeError, KeyError, RuntimeError) as error:
            errors[name] = str(error)
    result["validation_errors"] = errors
    if not errors:
        first, second = captures["reference"], captures["native"]
        result["plans_exact"] = first["plan"] == second["plan"]
        result["state_events_exact"] = first["raw"] == second["raw"]
        difference = first_difference(first["events"], second["events"])
        result["first_different_event"] = difference
        result["first_different_renderer_frame"] = (first["events"][difference].get("renderer_frame")
                                                   if difference is not None else None)
        result["first_different_event_byte"] = first_difference(first["raw"], second["raw"])
    result["movie_platform"] = compare_movie(reference, native, movie)
    result["passed"] = (not errors and result["plans_exact"] and result["state_events_exact"] and
                        result["movie_platform"]["passed"])
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("reference", type=Path)
    parser.add_argument("native", type=Path)
    parser.add_argument("--movie", type=Path)
    parser.add_argument("--report", type=Path, required=True)
    arguments = parser.parse_args()
    report = output_path(arguments.report, "report")
    result = compare(arguments.reference, arguments.native, arguments.movie)
    report.parent.mkdir(parents=True, exist_ok=True)
    with report.open("x") as stream:
        stream.write(json.dumps(result, indent=2) + "\n")
    print(json.dumps(result, indent=2))
    raise SystemExit(0 if result["passed"] else 1)


if __name__ == "__main__":
    main()
