#!/usr/bin/env python3
"""Serve the local browser port and preserve bounded capture exports under ignored build/."""
import argparse
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
import json
import mimetypes
from pathlib import Path, PurePosixPath
import re
import time
from threading import Lock
from urllib.parse import urlsplit

from audit_webassembly_platform import digest
from compare_input_capture import load_movie

ROOT = Path(__file__).resolve().parents[2]
MAXIMUM_CHUNK = 65536
MAXIMUM_METADATA = 16 * 1024 * 1024
MAXIMUM_CAPTURE = 2 ** 31


def relative_path(value):
    if not isinstance(value, str) or not value or len(value) > 2048:
        raise ValueError("Invalid relative path")
    if not all(re.fullmatch(r"[A-Za-z0-9_.-]+", part) and part not in (".", "..", "__proto__")
               for part in value.split("/")):
        raise ValueError("Invalid relative path")
    return value


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("module", type=Path, help="successful browser builder output")
    parser.add_argument("output", type=Path, help="absent ignored directory for this server run")
    parser.add_argument("--block-schedule", type=Path, required=True)
    parser.add_argument("--reference", type=Path, required=True, help="capture containing movie and initial_user_state")
    parser.add_argument("--dump-sha256", required=True)
    parser.add_argument("--dump-bytes", type=int, required=True)
    parser.add_argument("--first-swap", action="store_true")
    parser.add_argument("--observe-input", action="store_true", help="capture movie-delivered HID observations")
    parser.add_argument("--observe-audio", action="store_true", help="capture HLE PCM and movie-delivered HID observations")
    parser.add_argument("--stream-audio", action="store_true", help="offer explicit streamed playback of original observed PCM")
    parser.add_argument("--live-button", action="store_true", help="record live browser A input from the reference initial state")
    parser.add_argument("--live-circle-pad", action="store_true", help="record browser circle-pad input from the reference initial state")
    parser.add_argument("--live-touch", action="store_true", help="record bottom-screen pointer touch with frame output")
    parser.add_argument("--frame-output", action="store_true", help="show bounded completed-screen samples during execution")
    parser.add_argument("--presentation-limit", type=int, default=60)
    parser.add_argument("--wall-time-seconds", type=int, default=180)
    parser.add_argument("--port", type=int, default=8765)
    args = parser.parse_args()
    module, output = args.module.resolve(), args.output.resolve()
    if not output.is_relative_to(ROOT / "build") or output.exists():
        raise ValueError("Output must be absent and under this checkout's ignored build")
    if not re.fullmatch(r"[0-9a-f]{64}", args.dump_sha256) or not 0 < args.dump_bytes <= 0xffffffff:
        raise ValueError("Invalid approved dump identity")
    if not 1 <= args.presentation_limit <= 3600 or not 1 <= args.wall_time_seconds <= 3600:
        raise ValueError("Invalid finite capture bound")
    if args.first_swap and (args.observe_input or args.observe_audio or args.stream_audio or args.live_button or args.live_circle_pad or args.live_touch or args.frame_output):
        raise ValueError("Input/audio observation needs a software-presentation boundary")
    if args.live_touch and not args.frame_output:
        raise ValueError("Live touch needs frame output for its visible target")
    manifest = json.loads((module / "build_manifest.json").read_text())
    if manifest.get("passed") is not True:
        raise ValueError("Browser module did not link successfully")
    assets = {"/": ROOT / "runtime/port/browser/index.html"}
    for path in (ROOT / "runtime/port/browser").iterdir():
        if path.suffix in (".html", ".css", ".mjs"):
            assets[f"/{path.name}"] = path
    for name in ("IBMPlexSans-Regular.woff2", "IBMPlexSans-SemiBold.woff2", "LICENSE.txt"):
        assets[f"/fonts/{name}"] = ROOT / "runtime/port/browser/fonts" / name
    for key in ("module", "wasm"):
        path = Path(manifest[key]).resolve()
        if path.parent != module or digest(path) != manifest[f"{key}_sha256"]:
            raise ValueError("Browser module seal differs")
        assets[f"/module/{path.name}"] = path
    sidecars = {}

    def input_record(path):
        path = path.resolve()
        name = f"/inputs/{len(sidecars)}"
        if not path.is_file() or path.stat().st_size > 0xffffffff:
            raise ValueError("Invalid readonly sidecar")
        sidecars[name] = path
        return {"url": name, "expected_bytes": path.stat().st_size, "expected_sha256": digest(path)}

    initial = args.reference.resolve() / "initial_user_state"
    initial_entries = sorted(initial.rglob("*"))
    if any(path.is_symlink() or not (path.is_file() or path.is_dir()) for path in initial_entries):
        raise ValueError("Snapshot contains a symlink or nonregular entry")
    user_directories = [relative_path(str(path.relative_to(initial))) for path in initial_entries if path.is_dir()]
    user_files = [{"relative_path": relative_path(str(path.relative_to(initial))), **input_record(path)}
                  for path in initial_entries if path.is_file()]
    if not user_files:
        raise ValueError("Reference initial user state is empty")
    configuration = {"schema_version": 1, "module_url": f"/module/{Path(manifest['module']).name}",
                     "inputs": {"dump": {"expected_bytes": args.dump_bytes, "expected_sha256": args.dump_sha256},
                                "block_schedule": input_record(args.block_schedule),
                                "movie": input_record(args.reference / "input_movie.ctm"),
                                "initial_user_files": user_files, "initial_user_directories": user_directories},
                     "options": {"presentation_limit": None if args.first_swap else args.presentation_limit,
                                 "wall_time_seconds": args.wall_time_seconds,
                                 "pica_payload_limit_bytes": 256 * 1024 * 1024,
                                 "input_capture": args.observe_input or args.observe_audio or args.stream_audio or args.live_button or args.live_circle_pad or args.live_touch,
                                 "audio_capture": args.observe_audio or args.stream_audio or args.live_button or args.live_circle_pad or args.live_touch}}
    if args.stream_audio:
        configuration["options"]["stream_audio_output"] = True
    if args.frame_output:
        configuration["options"]["frame_output"] = True
    if args.live_button or args.live_circle_pad or args.live_touch:
        movie = load_movie(args.reference / "input_movie.ctm")
        if not 0 <= movie["base_ticks"] <= 2 ** 53 - 1:
            raise ValueError("Recording clock exceeds the exact browser observation range")
        configuration["options"]["record_base_ticks"] = str(movie["base_ticks"])
    if args.live_button:
        configuration["options"]["live_button_capture"] = True
    if args.live_circle_pad:
        configuration["options"]["live_circle_pad_capture"] = True
    if args.live_touch:
        configuration["options"]["live_touch_capture"] = True
    protected = {str(path): digest(path) for path in {*assets.values(), *sidecars.values(),
                                                   module / "build_manifest.json"}}
    output.mkdir(parents=True)
    (output / "configuration.json").write_text(json.dumps(configuration, indent=2) + "\n")
    (output / "server_inputs.json").write_text(json.dumps(protected, indent=2) + "\n")
    sessions = {}
    session_lock = Lock()
    origin = f"http://127.0.0.1:{args.port}"

    class Handler(BaseHTTPRequestHandler):
        protocol_version = "HTTP/1.1"

        def send_headers(self, status, size, content_type="application/json"):
            self.send_response(status)
            self.send_header("Content-Length", str(size))
            self.send_header("Content-Type", content_type)
            self.send_header("Cross-Origin-Opener-Policy", "same-origin")
            self.send_header("Cross-Origin-Embedder-Policy", "require-corp")
            self.send_header("Cross-Origin-Resource-Policy", "same-origin")
            self.send_header("Cache-Control", "no-store")
            self.send_header("X-Content-Type-Options", "nosniff")
            self.end_headers()

        def response(self, value, status=200):
            body = json.dumps(value).encode()
            self.send_headers(status, len(body))
            self.wfile.write(body)

        def do_GET(self):
            if self.headers.get("Host") != f"127.0.0.1:{args.port}":
                return self.response({"error": "Unexpected local host"}, 403)
            path = urlsplit(self.path).path
            if path == "/configuration.json":
                return self.response(configuration)
            source = assets.get(path) or sidecars.get(path)
            if source is None:
                return self.response({"error": "Unknown local asset"}, 404)
            kind = "text/javascript" if source.suffix == ".mjs" else mimetypes.guess_type(source.name)[0]
            self.send_headers(200, source.stat().st_size, kind or "application/octet-stream")
            try:
                with source.open("rb") as stream:
                    while chunk := stream.read(MAXIMUM_CHUNK):
                        self.wfile.write(chunk)
            except (BrokenPipeError, ConnectionResetError):
                # A stopped preview cancels its readonly download.
                self.close_connection = True

        def do_POST(self):
            with session_lock:
                self.receive_post()

        def receive_post(self):
            try:
                if self.headers.get("Origin") != origin or self.headers.get("Host") != f"127.0.0.1:{args.port}":
                    raise ValueError("Unexpected local origin")
                parts = urlsplit(self.path).path.split("/")
                if len(parts) != 4 or parts[1] != "capture" or not re.fullmatch(r"[A-Za-z0-9_-]{1,64}", parts[2]):
                    raise ValueError("Invalid capture endpoint")
                identifier, action = parts[2:]
                length = int(self.headers.get("Content-Length", "-1"))
                maximum = MAXIMUM_CHUNK if action == "chunk" else MAXIMUM_METADATA
                if not 0 <= length <= maximum:
                    raise ValueError("Request exceeds the finite bound")
                body = self.rfile.read(length)
                if len(body) != length:
                    raise ValueError("Short request")
                if action == "manifest":
                    if identifier in sessions:
                        raise ValueError("Capture identifier already exists")
                    record = json.loads(body)
                    files = record["files"]
                    directories = record["directories"]
                    if not isinstance(files, list) or not 0 < len(files) <= 16384:
                        raise ValueError("Invalid capture file manifest")
                    if not isinstance(directories, list) or len(directories) + len(files) > 32768:
                        raise ValueError("Invalid capture directory manifest")
                    directory_names = set()
                    for name in directories:
                        relative_path(name)
                        if name in directory_names or len(name.split("/")) > 32:
                            raise ValueError("Duplicate or excessive capture directory")
                        directory_names.add(name)
                    names = set()
                    total = 0
                    for item in files:
                        name = relative_path(item["relative_path"])
                        size = item["size"]
                        if name in names or name in directory_names or type(size) is not int or not 0 <= size <= MAXIMUM_CAPTURE:
                            raise ValueError("Invalid capture file extent")
                        names.add(name)
                        total += size
                    if total > MAXIMUM_CAPTURE:
                        raise ValueError("Capture exceeds the finite bound")
                    for name in names | directory_names:
                        if any(str(parent) not in directory_names for parent in PurePosixPath(name).parents
                               if str(parent) != "."):
                            raise ValueError("Capture directory parent is absent")
                    directory = output / identifier
                    directory.mkdir()
                    for name in sorted(directory_names, key=lambda value: (value.count("/"), value)):
                        (directory / name).mkdir()
                    for item in files:
                        target = directory / item["relative_path"]
                        target.parent.mkdir(parents=True, exist_ok=True)
                        target.touch(exist_ok=False)
                    (output / f"{identifier}_manifest.json").write_bytes(body)
                    sessions[identifier] = {"files": files, "directories": directories,
                                            "offsets": [0] * len(files), "finished": False,
                                            "directory": directory, "started": time.monotonic()}
                else:
                    session = sessions[identifier]
                    if session["finished"]:
                        raise ValueError("Capture is already closed")
                    if action == "chunk":
                        index = int(self.headers["X-Capture-File"])
                        offset = int(self.headers["X-Capture-Offset"])
                        if not 0 <= index < len(session["files"]) or offset != session["offsets"][index]:
                            raise ValueError("Capture transfer is out of order")
                        item = session["files"][index]
                        if not body or offset + len(body) > item["size"]:
                            raise ValueError("Capture chunk extent disagrees")
                        with (session["directory"] / item["relative_path"]).open("ab") as stream:
                            stream.write(body)
                        session["offsets"][index] += len(body)
                    elif action == "complete":
                        record = json.loads(body)
                        if record.get("exit_status") != 0 or record.get("outcome", {}).get("complete") is not True:
                            raise ValueError("C++ capture did not complete")
                        if any(item["size"] != offset for item, offset in zip(session["files"], session["offsets"])):
                            raise ValueError("Capture transfer is incomplete")
                        if any(digest(path) != expected for path, expected in protected.items()):
                            raise ValueError("Protected server input changed")
                        current_entries = sorted(initial.rglob("*"))
                        if (any(path.is_symlink() for path in current_entries) or
                            [str(path.relative_to(initial)) for path in current_entries if path.is_dir()] != user_directories or
                            [str(path.relative_to(initial)) for path in current_entries if path.is_file()] !=
                                [item["relative_path"] for item in user_files]):
                            raise ValueError("Protected snapshot tree changed")
                        receipt = {"passed": True, "configuration": configuration,
                                   "protected_inputs": protected, "completion": record,
                                   "files": {item["relative_path"]: digest(session["directory"] / item["relative_path"])
                                             for item in session["files"]},
                                   "directories": session["directories"],
                                   "elapsed_export_seconds": time.monotonic() - session["started"],
                                   "scope": "Actual browser transfer and protected local inputs. Differential replay is a separate check."}
                        (output / f"{identifier}_receipt.json").write_text(json.dumps(receipt, indent=2) + "\n")
                        session["finished"] = True
                    else:
                        raise ValueError("Unknown capture action")
                self.response({"accepted": True})
            except (ValueError, KeyError, IndexError, OSError) as error:
                self.response({"error": str(error)}, 400)

    print(json.dumps({"url": origin, "output": str(output), "protected_files": len(protected)}), flush=True)
    ThreadingHTTPServer(("127.0.0.1", args.port), Handler).serve_forever()


if __name__ == "__main__":
    main()
