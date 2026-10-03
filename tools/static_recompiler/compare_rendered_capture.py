#!/usr/bin/env python3
"""Compare complete software presentations, retaining exact events and pixels."""
import argparse
from collections import Counter
import json
from pathlib import Path
import struct
import zlib

from compare_gpu_capture import digest, load as load_gpu_capture

ROOT = Path(__file__).resolve().parents[2]
MAXIMUM_SCREEN_DIMENSION = 4096
MAXIMUM_FRAMEBUFFER_BYTES = 16 * 1024 * 1024
BYTES_PER_PIXEL = {0: 4, 1: 3, 2: 2, 3: 2, 4: 2}
SCREEN_IDENTIFIERS = (0, 2)


def unsigned_integer(value, description, maximum=0xFFFFFFFF, positive=False):
    if type(value) is not int or not (int(positive) <= value <= maximum):
        raise RuntimeError(f"invalid {description}")
    return value


def unique_object(pairs):
    result = {}
    for key, value in pairs:
        if key in result:
            raise RuntimeError(f"duplicate JSON field: {key}")
        result[key] = value
    return result


def payload_path(directory, name):
    if not isinstance(name, str) or not name or Path(name).name != name or name in (".", ".."):
        raise RuntimeError("invalid capture payload path")
    path = directory / name
    if path.is_symlink() or not path.is_file() or path.resolve().parent != directory:
        raise RuntimeError(f"capture payload is not a local regular file: {name}")
    return path


def read_payload(directory, name, expected_bytes):
    path = payload_path(directory, name)
    if path.stat().st_size != expected_bytes:
        raise RuntimeError(f"capture payload extent disagrees: {name}")
    data = path.read_bytes()
    if len(data) != expected_bytes:
        raise RuntimeError(f"capture payload changed while reading: {name}")
    return data


def load_rendered_capture(directory):
    if not directory.is_dir():
        raise RuntimeError("capture directory is absent")
    event_path = payload_path(directory, "gpu_events.jsonl")
    raw = event_path.read_bytes()
    events = [json.loads(line, object_pairs_hook=unique_object) for line in raw.splitlines()]
    for index, event in enumerate(events):
        if not isinstance(event, dict) or unsigned_integer(event.get("sequence"), "event sequence") != index:
            raise RuntimeError("capture event sequence is incomplete")
        unsigned_integer(event.get("ticks"), "event ticks", maximum=0xFFFFFFFFFFFFFFFF)
        if not isinstance(event.get("kind"), str):
            raise RuntimeError("invalid capture event kind")
        if event["kind"] == "pica_command_list":
            payload_path(directory, event.get("payload"))
    capture = load_gpu_capture(directory)
    if capture[0] != raw or capture[1] != events:
        raise RuntimeError("capture events changed while reading")
    presentations = [event for event in events if event["kind"] == "frame_presentation"]
    if len(presentations) != 1 or len(events) < 2 or presentations[0] != events[-2]:
        raise RuntimeError("capture needs one final software presentation before its outcome")
    presentation = presentations[0]
    unsigned_integer(presentation.get("renderer_frame"), "renderer frame", maximum=0xFFFFFFFFFFFFFFFF, positive=True)
    unsigned_integer(presentation.get("presentation_index"), "presentation index", maximum=7199)
    submission_ticks = unsigned_integer(presentation.get("submission_ticks"), "submission ticks", maximum=0xFFFFFFFFFFFFFFFF)
    vblank_index = unsigned_integer(presentation.get("vblank_index"), "presentation VBlank index", maximum=0xFFFFFFFFFFFFFFFF)
    submissions = [event for event in events[:-2] if event["kind"] == "buffer_swap" and event.get("screen_id") == 0]
    vblanks = [event for event in events[:-2] if event["kind"] == "vblank"]
    if not submissions or submissions[0]["ticks"] != submission_ticks or submission_ticks > presentation["ticks"]:
        raise RuntimeError("presentation does not identify its first top-screen submission")
    if not vblanks or vblanks[-1].get("vblank_index") != vblank_index or vblanks[-1]["ticks"] != presentation["ticks"]:
        raise RuntimeError("presentation does not identify its natural VBlank")
    screens = presentation.get("screens")
    if not isinstance(screens, list) or len(screens) != len(SCREEN_IDENTIFIERS):
        raise RuntimeError("presentation needs top-left and bottom screen metadata")
    rgba_payloads, framebuffer_payloads, summaries = {}, {}, []
    identifiers = []
    for screen in screens:
        if not isinstance(screen, dict):
            raise RuntimeError("invalid screen metadata")
        identifier = unsigned_integer(screen.get("screen_id"), "screen identifier")
        if identifier not in SCREEN_IDENTIFIERS or identifier in identifiers:
            raise RuntimeError("invalid or repeated screen identifier")
        identifiers.append(identifier)
        dimensions = {name: unsigned_integer(screen.get(name), name, MAXIMUM_SCREEN_DIMENSION, True)
                      for name in ("storage_width", "storage_height", "width", "height")}
        if dimensions["width"] != dimensions["storage_height"] or dimensions["height"] != dimensions["storage_width"]:
            raise RuntimeError("landscape dimensions disagree with software storage")
        rgba_bytes = unsigned_integer(screen.get("rgba_bytes"), "RGBA byte count", 4 * MAXIMUM_SCREEN_DIMENSION ** 2, True)
        if rgba_bytes != dimensions["width"] * dimensions["height"] * 4:
            raise RuntimeError("RGBA extent disagrees with landscape dimensions")
        framebuffer_width = unsigned_integer(screen.get("framebuffer_width"), "framebuffer width", 0xFFFF, True)
        framebuffer_height = unsigned_integer(screen.get("framebuffer_height"), "framebuffer height", 0xFFFF, True)
        stride = unsigned_integer(screen.get("stride"), "framebuffer stride", positive=True)
        framebuffer_bytes = unsigned_integer(screen.get("framebuffer_bytes"), "framebuffer byte count", MAXIMUM_FRAMEBUFFER_BYTES, True)
        if framebuffer_bytes != stride * framebuffer_height:
            raise RuntimeError("raw framebuffer extent disagrees with stride and hardware height")
        color_format = unsigned_integer(screen.get("color_format"), "framebuffer color format", 4)
        raw_format = unsigned_integer(screen.get("format"), "raw framebuffer format")
        bytes_per_pixel = BYTES_PER_PIXEL[color_format]
        if color_format != (raw_format & 7) or stride % bytes_per_pixel or stride < framebuffer_width * bytes_per_pixel:
            raise RuntimeError("framebuffer format or pixel stride disagrees")
        unsigned_integer(screen.get("active_fb"), "active framebuffer register")
        address = unsigned_integer(screen.get("physical_address"), "framebuffer physical address", positive=True)
        unsigned_integer(screen.get("right_physical_address"), "right framebuffer physical address")
        color_fill = unsigned_integer(screen.get("color_fill"), "LCD color fill")
        if address + framebuffer_bytes > 0xFFFFFFFF:
            raise RuntimeError("framebuffer physical extent overflows")
        rgba_name = screen.get("rgba_payload")
        framebuffer_name = screen.get("framebuffer_payload")
        if rgba_name != f"rendered_screen_{identifier}.rgba" or framebuffer_name != f"framebuffer_screen_{identifier}.bin":
            raise RuntimeError("screen payload filename disagrees with its identifier")
        rgba = read_payload(directory, rgba_name, rgba_bytes)
        framebuffer = read_payload(directory, framebuffer_name, framebuffer_bytes)
        rgba_payloads[identifier] = rgba
        framebuffer_payloads[identifier] = framebuffer
        colors = Counter(value for value, in struct.iter_unpack(">I", rgba))
        distinct_colors = len(colors)
        distinct_rgb_colors = len({value >> 8 for value in colors})
        dominant_color, dominant_pixels = max(colors.items(), key=lambda pair: (pair[1], -pair[0]))
        summaries.append({
            "screen_id": identifier, "width": dimensions["width"], "height": dimensions["height"],
            "rgba_bytes": rgba_bytes, "framebuffer_bytes": framebuffer_bytes,
            "rgba_sha256": digest(rgba), "framebuffer_sha256": digest(framebuffer),
            "distinct_rgba_colors": distinct_colors, "nonuniform": distinct_colors > 1,
            "distinct_rgb_colors": distinct_rgb_colors, "rgb_nonuniform": distinct_rgb_colors > 1,
            "alpha_only_variation": distinct_colors > 1 and distinct_rgb_colors == 1,
            "dominant_rgba_color": [(dominant_color >> shift) & 255 for shift in (24, 16, 8, 0)],
            "dominant_color_pixels": dominant_pixels,
            "dominant_color_share": dominant_pixels / (rgba_bytes // 4),
            "lcd_color_fill_enabled": bool(color_fill & 0x01000000),
        })
    if set(identifiers) != set(SCREEN_IDENTIFIERS):
        raise RuntimeError("presentation screen identifiers are incomplete")
    for pattern, expected in (("rendered_screen_*.rgba", {f"rendered_screen_{i}.rgba" for i in identifiers}),
                              ("framebuffer_screen_*.bin", {f"framebuffer_screen_{i}.bin" for i in identifiers})):
        if {path.name for path in directory.glob(pattern)} != expected:
            raise RuntimeError("unreferenced rendered payload remains")
    complete = capture[3] and events[-1].get("outcome") == "software_presentation"
    return {"gpu": capture, "presentation": presentation, "rgba": rgba_payloads,
            "framebuffers": framebuffer_payloads, "screens": summaries, "complete": complete}


def output_path(path, description, directory=False):
    requested = Path(path)
    resolved = requested.resolve()
    if requested.exists() or requested.is_symlink() or not resolved.is_relative_to(ROOT / "build") or resolved == ROOT / "build":
        raise RuntimeError(f"{description} must be an absent {'directory' if directory else 'path'} in this checkout's ignored build")
    return resolved


def write_png(path, width, height, rgba):
    def chunk(kind, data):
        return struct.pack(">I", len(data)) + kind + data + struct.pack(">I", zlib.crc32(kind + data) & 0xFFFFFFFF)
    row_bytes = width * 4
    if len(rgba) != row_bytes * height:
        raise RuntimeError("PNG pixel extent disagrees")
    compressor = zlib.compressobj()
    compressed = bytearray()
    for row in range(height):
        compressed.extend(compressor.compress(b"\x00" + rgba[row * row_bytes:(row + 1) * row_bytes]))
    compressed.extend(compressor.flush())
    header = struct.pack(">2I5B", width, height, 8, 6, 0, 0, 0)
    data = b"\x89PNG\r\n\x1a\n" + chunk(b"IHDR", header) + chunk(b"IDAT", bytes(compressed)) + chunk(b"IEND", b"")
    with path.open("xb") as stream:
        stream.write(data)


def compare(reference, native, preview_directory=None):
    directories = {"reference": Path(reference).resolve(), "native": Path(native).resolve()}
    preview = output_path(preview_directory, "preview", directory=True) if preview_directory is not None else None
    captures, errors = {}, {}
    for label, directory in directories.items():
        try:
            captures[label] = load_rendered_capture(directory)
        except (OSError, RuntimeError, ValueError, TypeError, KeyError) as error:
            errors[label] = str(error)
    result = {label: str(directory) for label, directory in directories.items()}
    result.update({"validation_errors": errors, "passed": False})
    for label, capture in captures.items():
        result[label + "_screens"] = capture["screens"]
        result[label + "_event_sha256"] = digest(capture["gpu"][0])
        result[label + "_event_counts"] = dict(Counter(event["kind"] for event in capture["gpu"][1]))
        result[label + "_pica_payload_sha256"] = {name: digest(data) for name, data in capture["gpu"][2].items()}
    if len(captures) == 2:
        reference_capture, native_capture = captures["reference"], captures["native"]
        reference_gpu, native_gpu = reference_capture["gpu"], native_capture["gpu"]
        first_difference = next((index for index, pair in enumerate(zip(reference_gpu[1], native_gpu[1]))
                                 if pair[0] != pair[1]), None)
        if first_difference is None and len(reference_gpu[1]) != len(native_gpu[1]):
            first_difference = min(len(reference_gpu[1]), len(native_gpu[1]))
        result.update({
            "events_exact": reference_gpu[0] == native_gpu[0],
            "pica_payloads_exact": reference_gpu[2] == native_gpu[2],
            "screen_metadata_exact": reference_capture["presentation"]["screens"] == native_capture["presentation"]["screens"],
            "rgba_payloads_exact": reference_capture["rgba"] == native_capture["rgba"],
            "framebuffer_payloads_exact": reference_capture["framebuffers"] == native_capture["framebuffers"],
            "both_complete": reference_capture["complete"] and native_capture["complete"],
            "movie_errors": reference_gpu[4] or native_gpu[4],
            "first_different_event": first_difference,
        })
        result["passed"] = all(result[name] for name in ("events_exact", "pica_payloads_exact", "screen_metadata_exact",
                                                         "rgba_payloads_exact", "framebuffer_payloads_exact", "both_complete")) and not result["movie_errors"]
    if preview is not None and captures:
        preview.mkdir(parents=True, exist_ok=False)
        result["previews"] = []
        for label, capture in captures.items():
            for screen in capture["screens"]:
                identifier = screen["screen_id"]
                path = preview / f"{label}_screen_{identifier}.png"
                write_png(path, screen["width"], screen["height"], capture["rgba"][identifier])
                result["previews"].append(str(path))
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("reference", type=Path)
    parser.add_argument("native", type=Path)
    parser.add_argument("--report", type=Path, required=True)
    parser.add_argument("--preview-directory", type=Path)
    args = parser.parse_args()
    report = output_path(args.report, "report")
    if args.preview_directory is not None and args.preview_directory.resolve() == report:
        raise RuntimeError("preview directory cannot also be the report file")
    result = compare(args.reference, args.native, args.preview_directory)
    report.parent.mkdir(parents=True, exist_ok=True)
    with report.open("x") as stream:
        stream.write(json.dumps(result, indent=2) + "\n")
    print(json.dumps(result, indent=2))
    raise SystemExit(0 if result["passed"] else 1)


if __name__ == "__main__":
    main()
