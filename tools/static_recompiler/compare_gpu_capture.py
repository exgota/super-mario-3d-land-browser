#!/usr/bin/env python3
"""Compare complete first-swap captures without normalizing acceptance evidence."""
import argparse
from collections import Counter
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]


def digest(data):
    return hashlib.sha256(data).hexdigest()


def load(directory):
    raw = (directory / "gpu_events.jsonl").read_bytes()
    events = [json.loads(line) for line in raw.splitlines()]
    if not events or any(event["sequence"] != index for index, event in enumerate(events)):
        raise RuntimeError("capture event sequence is incomplete")
    complete = events[-1].get("kind") == "capture_outcome" and events[-1].get("complete") is True
    payloads = {}
    for event in events:
        if event["kind"] == "pica_command_list":
            name = event["payload"]
            if Path(name).name != name or name in payloads:
                raise RuntimeError("invalid or duplicate payload path")
            data = (directory / name).read_bytes()
            if len(data) != event["size"] or len(data) % 4:
                raise RuntimeError("PICA payload extent disagrees")
            payloads[name] = data
    if set(payloads) != {p.name for p in directory.glob("pica_command_list_*.bin")}:
        raise RuntimeError("unreferenced PICA payload remains")
    log = (directory / "user/log/reference_capture.log").read_text()
    return raw, events, payloads, complete, "Movie <Error>" in log


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("reference", type=Path)
    parser.add_argument("native", type=Path)
    parser.add_argument("--report", type=Path, required=True)
    args = parser.parse_args()
    report = args.report.resolve()
    if not report.is_relative_to(ROOT / "build") or report.exists():
        raise RuntimeError("report must be an absent path in this checkout's ignored build")
    reference = load(args.reference.resolve())
    native = load(args.native.resolve())
    first_difference = next((index for index, pair in enumerate(zip(reference[1], native[1]))
                             if pair[0] != pair[1]), None)
    if first_difference is None and len(reference[1]) != len(native[1]):
        first_difference = min(len(reference[1]), len(native[1]))
    # This diagnostic helps distinguish timing from command divergence. It never
    # changes the strict acceptance result or overwrites either original capture.
    def commands(events):
        return [{key: value for key, value in event.items() if key not in ("sequence", "ticks")}
                for event in events if event["kind"] not in ("vblank", "capture_outcome")]
    result = {
        "reference": str(args.reference.resolve()), "native": str(args.native.resolve()),
        "events_exact": reference[0] == native[0],
        "payloads_exact": reference[2] == native[2],
        "both_complete": reference[3] and native[3],
        "movie_errors": reference[4] or native[4],
        "first_different_event": first_difference,
        "command_metadata_without_timing_equal": commands(reference[1]) == commands(native[1]),
        "reference_event_sha256": digest(reference[0]), "native_event_sha256": digest(native[0]),
        "reference_event_counts": dict(Counter(event["kind"] for event in reference[1])),
        "native_event_counts": dict(Counter(event["kind"] for event in native[1])),
        "reference_payload_sha256": {name: digest(data) for name, data in reference[2].items()},
        "native_payload_sha256": {name: digest(data) for name, data in native[2].items()},
    }
    result["passed"] = all(result[name] for name in ("events_exact", "payloads_exact", "both_complete")) and not result["movie_errors"]
    report.parent.mkdir(parents=True, exist_ok=True)
    report.write_text(json.dumps(result, indent=2) + "\n")
    print(json.dumps(result, indent=2))
    raise SystemExit(0 if result["passed"] else 1)


if __name__ == "__main__":
    main()
