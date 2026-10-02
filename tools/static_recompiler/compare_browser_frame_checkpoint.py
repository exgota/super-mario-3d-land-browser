#!/usr/bin/env python3
"""Compare a retained running browser canvas with its natural stock presentation."""
import argparse
import json
from pathlib import Path

from audit_webassembly_platform import digest
from compare_rendered_capture import load_rendered_capture, output_path, unique_object


def read_record(path):
    if path.is_symlink() or not path.is_file():
        raise RuntimeError("Evidence must be a regular local file")
    return json.loads(path.read_text(), object_pairs_hook=unique_object)


def compare(sample_result, sample_index, stock_capture):
    result = read_record(sample_result)
    if result.get("passed") is not True or type(sample_index) is not int or not 0 <= sample_index < 2:
        raise RuntimeError("Successful browser receipt and bounded sample index required")
    capture = Path(result["capture"]).resolve(strict=True)
    manifest_path = capture.parent / f"{capture.name}_manifest.json"
    receipt_path = capture.parent / f"{capture.name}_receipt.json"
    if digest(manifest_path) != result["manifest_sha256"] or digest(receipt_path) != result["server_receipt_sha256"]:
        raise RuntimeError("Bound browser manifest or server receipt changed")
    manifest, receipt = read_record(manifest_path), read_record(receipt_path)
    if receipt.get("passed") is not True:
        raise RuntimeError("Browser export did not complete")
    final = load_rendered_capture(capture)
    stock = load_rendered_capture(stock_capture.resolve(strict=True))
    if final["complete"] is not True or stock["complete"] is not True:
        raise RuntimeError("Browser or stock software presentation is incomplete")
    sample = result["samples"][sample_index]
    metadata = sample["metadata"]
    sequence = metadata["sequence"]
    if type(sequence) is not int or not 1 <= sequence <= 8192:
        raise RuntimeError("Invalid preview publication sequence")
    published = manifest["observations"]["preview_frames"]["frames"][sequence - 1]
    if metadata != published or sample["state"] != "running":
        raise RuntimeError("Sample is not the original running preview publication")
    frame = int(metadata["renderer_frame"])
    final_frame = final["presentation"]["renderer_frame"]
    final_presentation = final["presentation"]["presentation_index"] + 1
    target = frame - final_frame + final_presentation
    if not 1 <= target < final_presentation or target != sample["stock_presentation_target"]:
        raise RuntimeError("Intermediate presentation derivation disagrees")
    if (stock["presentation"]["renderer_frame"] != frame or
            stock["presentation"]["presentation_index"] + 1 != target):
        raise RuntimeError("Stock checkpoint renderer frame or presentation disagrees")
    comparison = Path(result["full_movie_comparison"]["path"])
    if digest(comparison) != result["full_movie_comparison"]["sha256"]:
        raise RuntimeError("Full movie comparison changed")
    full = read_record(comparison)
    if full.get("passed") is not True or Path(full["native"]).resolve() != capture:
        raise RuntimeError("Full final movie comparison does not bind this browser capture")
    screens = []
    if len(sample["screens"]) != 2 or len(metadata["screens"]) != 2:
        raise RuntimeError("Both paired screens are required")
    for retained, original, identifier in zip(sample["screens"], metadata["screens"], (0, 2)):
        path = Path(retained["path"])
        if path.is_symlink() or not path.is_file() or path.resolve().parent != sample_result.resolve().parent:
            raise RuntimeError("Retained sample is outside its local evidence directory")
        pixels = path.read_bytes()
        screen = next(item for item in stock["presentation"]["screens"] if item["screen_id"] == identifier)
        if (original["screen_identifier"] != identifier or retained["hidden"] is not False or
                type(original["width"]) is not int or type(original["height"]) is not int or
                not 1 <= original["width"] <= 4096 or not 1 <= original["height"] <= 4096 or
                original["width"] * original["height"] * 4 != len(pixels) or len(pixels) > 1024 * 1024 or
                len(pixels) != original["bytes"] or retained["bytes"] != len(pixels) or
                retained["width"] != original["width"] or retained["height"] != original["height"] or
                digest(path) != original["sha256"] or retained["sha256"] != original["sha256"] or
                screen["width"] != original["width"] or screen["height"] != original["height"]):
            raise RuntimeError("Original slot/canvas/dimensions or payload seal disagrees")
        if pixels != stock["rgba"][identifier]:
            offset = next(index for index, pair in enumerate(zip(pixels, stock["rgba"][identifier])) if pair[0] != pair[1])
            raise RuntimeError(f"Screen {identifier} original RGBA differs at byte {offset}")
        screens.append({"screen_identifier": identifier, "width": original["width"], "height": original["height"],
                        "bytes": len(pixels), "sha256": digest(path), "direct_bytes_equal": True})
    return {"passed": True, "sample_result": str(sample_result.resolve()), "sample_result_sha256": digest(sample_result),
            "sample_index": sample_index, "stock_capture": str(stock_capture.resolve()),
            "stock_events_sha256": digest(stock_capture / "gpu_events.jsonl"), "screens": screens,
            "derivation": {"sample_renderer_frame": frame, "final_renderer_frame": final_frame,
                           "final_presentation": final_presentation, "target_presentation": target},
            "full_movie_comparison_sha256": digest(comparison),
            "scope": "Two original running canvas buffers equal copied slot hashes and fresh stock RGBA bytes. No intermediate raw GPU/input/audio or game-state assertion."}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("sample_result", type=Path)
    parser.add_argument("sample_index", type=int)
    parser.add_argument("stock_capture", type=Path)
    parser.add_argument("--report", type=Path, required=True)
    args = parser.parse_args()
    report = output_path(args.report, "report")
    value = compare(args.sample_result, args.sample_index, args.stock_capture)
    report.parent.mkdir(parents=True, exist_ok=True)
    with report.open("x") as stream:
        json.dump(value, stream, indent=2)
        stream.write("\n")
    print(json.dumps(value))


if __name__ == "__main__":
    main()
