#!/usr/bin/env python3
"""Compare exact movie delivery and platform output without channel-coverage policy."""
import argparse
import json
from pathlib import Path
import re
import struct

from compare_audio_capture import (
    BYTES_PER_SAMPLE_FRAME,
    SAMPLE_WIDTH,
    load_audio_capture,
    sample_statistics,
)
from compare_gpu_capture import digest
from compare_input_capture import (
    channel_evidence,
    load_input_capture,
    load_movie,
    validate_movie_delivery,
    validate_observation_boundary,
    validate_script,
)
from compare_rendered_capture import (
    compare as compare_rendered_capture,
    output_path,
    payload_path,
)

VALIDATION_EXCEPTIONS = (OSError, RuntimeError, ValueError, TypeError, KeyError, struct.error)


def first_difference(reference, native):
    difference = next((index for index, pair in enumerate(zip(reference, native))
                       if pair[0] != pair[1]), None)
    if difference is None and len(reference) != len(native):
        difference = min(len(reference), len(native))
    return difference


def rendered_payload_difference(directories, rendered):
    """Locate differing payload bytes after the strict rendered loaders succeed."""
    if rendered["validation_errors"]:
        return None
    names = set(rendered["reference_pica_payload_sha256"]) | set(rendered["native_pica_payload_sha256"])
    for identifier in (0, 2):
        names.update((f"rendered_screen_{identifier}.rgba", f"framebuffer_screen_{identifier}.bin"))
    for name in sorted(names):
        paths = [directory / name for directory in directories.values()]
        if any(not path.exists() for path in paths):
            return {"payload": name, "missing_from": [label for label, directory in directories.items()
                                                       if not (directory / name).exists()]}
        values = [payload_path(directory, name).read_bytes() for directory in directories.values()]
        difference = first_difference(*values)
        if difference is not None:
            return {"payload": name, "byte": difference,
                    "reference_byte": values[0][difference] if difference < len(values[0]) else None,
                    "native_byte": values[1][difference] if difference < len(values[1]) else None}
    return None


def compare(reference, native, movie=None, preview_directory=None):
    directories = {"reference": Path(reference).resolve(), "native": Path(native).resolve()}
    movie_path = Path(movie) if movie is not None else directories["reference"] / "input_movie.ctm"
    if preview_directory is not None:
        output_path(preview_directory, "pixel preview", directory=True)
    input_captures, audio_captures, input_errors, audio_errors, scripts, movie_errors = {}, {}, {}, {}, {}, {}
    result = {label: str(directory) for label, directory in directories.items()}
    result.update({"movie": str(movie_path.resolve()), "passed": False,
                   "scope": "Recorded movie delivery and platform output; no game-state semantics",
                   "channel_evidence_policy": "Diagnostic only; unused channels and exact silent PCM are permitted"})
    try:
        movie_capture = load_movie(movie_path)
        result["movie_diagnostics"] = {
            "sha256": digest(movie_capture["raw"]), "bytes": len(movie_capture["raw"]),
            "program_id": movie_capture["program_id"], "revision": movie_capture["revision"],
            "init_time": movie_capture["init_time"], "base_ticks": movie_capture["base_ticks"],
            "pad_count": len(movie_capture["pads"]), "record_counts": movie_capture["record_counts"],
            "record_type_sha256": movie_capture["record_type_sha256"],
        }
    except VALIDATION_EXCEPTIONS as error:
        movie_capture = None
        input_errors["movie"] = str(error)
    for label, directory in directories.items():
        try:
            capture = load_input_capture(directory)
            input_captures[label] = capture
            result[label + "_input_sha256"] = digest(capture["raw"])
            result[label + "_input_polls"] = len(capture["polls"])
            result[label + "_channel_evidence"] = channel_evidence(capture["polls"])
            validate_observation_boundary(directory, capture)
            if movie_capture is not None:
                validate_movie_delivery(directory, movie_capture, capture)
            scripts[label] = validate_script(directory, capture)
        except VALIDATION_EXCEPTIONS as error:
            input_errors[label] = str(error)
        try:
            capture = load_audio_capture(directory)
            audio_captures[label] = capture
            result[label + "_audio_event_sha256"] = digest(capture["raw"])
            result[label + "_pcm_sha256"] = digest(capture["pcm"])
            result[label + "_audio_blocks"] = capture["blocks"]
            result[label + "_sample_statistics"] = sample_statistics(capture["pcm"])
        except VALIDATION_EXCEPTIONS as error:
            audio_errors[label] = str(error)
        try:
            log = (directory / "user/log/reference_capture.log").read_text()
            movie_errors[label] = bool(re.search(r"\bMovie(?:\.[A-Za-z0-9_.]+)?\s+<(?:Error|Critical)>", log))
        except VALIDATION_EXCEPTIONS as error:
            input_errors[label + "_log"] = str(error)
    result.update({"input_validation_errors": input_errors, "audio_validation_errors": audio_errors,
                   "movie_error_diagnostics": movie_errors,
                   "movie_errors": len(movie_errors) != 2 or any(movie_errors.values()),
                   "script_diagnostics": scripts,
                   "rendered_capture": compare_rendered_capture(reference, native, preview_directory)})
    if len(input_captures) == 2:
        reference_input, native_input = input_captures["reference"], input_captures["native"]
        result.update({
            "input_events_exact": reference_input["raw"] == native_input["raw"],
            "both_input_complete": reference_input["complete"] and native_input["complete"],
            "first_different_input_poll": first_difference(reference_input["polls"], native_input["polls"]),
            "first_different_input_event_byte": first_difference(reference_input["raw"], native_input["raw"]),
        })
    if len(audio_captures) == 2:
        reference_audio, native_audio = audio_captures["reference"], audio_captures["native"]
        difference = first_difference(reference_audio["pcm"], native_audio["pcm"])
        result.update({
            "audio_events_exact": reference_audio["raw"] == native_audio["raw"],
            "pcm_payloads_exact": reference_audio["pcm"] == native_audio["pcm"],
            "both_audio_complete": reference_audio["complete"] and native_audio["complete"],
            "audio_errors": reference_audio["audio_errors"] or native_audio["audio_errors"],
            "first_different_audio_event": first_difference(reference_audio["events"], native_audio["events"]),
            "first_different_audio_event_byte": first_difference(reference_audio["raw"], native_audio["raw"]),
            "first_different_pcm_byte": difference,
            "first_different_stereo_frame": difference // BYTES_PER_SAMPLE_FRAME if difference is not None else None,
            "first_different_channel": (difference % BYTES_PER_SAMPLE_FRAME) // SAMPLE_WIDTH if difference is not None else None,
        })
    if not result["rendered_capture"]["passed"]:
        try:
            if not result["rendered_capture"]["validation_errors"]:
                values = [payload_path(directory, "gpu_events.jsonl").read_bytes()
                          for directory in directories.values()]
                result["first_different_gpu_event_byte"] = first_difference(*values)
            result["first_different_rendered_payload"] = rendered_payload_difference(directories, result["rendered_capture"])
        except VALIDATION_EXCEPTIONS as error:
            result["rendered_payload_diagnostic_error"] = str(error)
    requirements = ("input_events_exact", "both_input_complete", "audio_events_exact",
                    "pcm_payloads_exact", "both_audio_complete")
    result["passed"] = (not input_errors and not audio_errors and
                        all(result.get(name, False) for name in requirements) and
                        not result.get("audio_errors", True) and not result["movie_errors"] and
                        result["rendered_capture"]["passed"])
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("reference", type=Path)
    parser.add_argument("native", type=Path)
    parser.add_argument("--movie", type=Path, help="Original recorded movie; defaults to reference/input_movie.ctm")
    parser.add_argument("--report", type=Path, required=True)
    parser.add_argument("--preview-directory", type=Path, help="Absent ignored directory for exact pixel PNG previews")
    arguments = parser.parse_args()
    report = output_path(arguments.report, "report")
    if arguments.preview_directory is not None:
        preview = output_path(arguments.preview_directory, "pixel preview", directory=True)
        if report == preview or (report.parent == preview and report.name in
                                 ("reference_screen_0.png", "reference_screen_2.png",
                                  "native_screen_0.png", "native_screen_2.png")):
            raise RuntimeError("pixel preview cannot also be the report file")
    result = compare(arguments.reference, arguments.native, arguments.movie, arguments.preview_directory)
    report.parent.mkdir(parents=True, exist_ok=True)
    with report.open("x") as stream:
        stream.write(json.dumps(result, indent=2) + "\n")
    print(json.dumps(result, indent=2))
    raise SystemExit(0 if result["passed"] else 1)


if __name__ == "__main__":
    main()
