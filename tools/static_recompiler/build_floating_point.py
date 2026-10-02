#!/usr/bin/env python3
"""Build the pinned portable guest-arithmetic archive without downloading sources."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[2]
TARGET = ROOT / "runtime/port/floating_point"


def digest(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def build(source, output, cmake="cmake"):
    source, output = Path(source).resolve(), Path(output).resolve()
    if not output.is_relative_to(ROOT / "build") or output.exists():
        raise RuntimeError("arithmetic output must be an absent child of this checkout's ignored build")
    output.mkdir(parents=True)
    commands = [
        [str(cmake), "-S", str(TARGET), "-B", str(output),
         f"-DROOT_PORT_SOFTFLOAT_SOURCE_DIRECTORY={source}",
         "-DCMAKE_BUILD_TYPE=Release", "-DCMAKE_C_COMPILER=clang"],
        [str(cmake), "--build", str(output), "--target", "port_floating_point", "-j", "1"],
    ]
    for index, arguments in enumerate(commands):
        with (output / f"build_{index}.log").open("wb") as log:
            subprocess.run(arguments, stdout=log, stderr=subprocess.STDOUT, check=True)
    archive = output / "libport_floating_point.a"
    manifest = json.loads((TARGET / "softfloat_sources.json").read_text())
    manifest.update({
        "source_directory": str(source), "commands": commands,
        "archive": str(archive), "archive_sha256": digest(archive),
        "adapter_sha256": digest(ROOT / "runtime/port/NativeFloatingPoint.c"),
        "header_sha256": digest(ROOT / "runtime/port/NativeFloatingPoint.h"),
        "platform_sha256": digest(TARGET / "platform.h"),
        "cmake_sha256": digest(TARGET / "CMakeLists.txt"),
        "license_copy_sha256": digest(output / "SoftFloat-COPYING.txt"),
        "definitions": ["SOFTFLOAT_FAST_INT64=1", "THREAD_LOCAL=_Thread_local", "INLINE_LEVEL=0"],
        "host_arithmetic": False, "thread_local_state": True,
    })
    (output / "build_manifest.json").write_text(json.dumps(manifest, indent=2) + "\n")
    return archive, manifest


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("source", type=Path, help="unmodified official SoftFloat 3e source directory")
    parser.add_argument("output", type=Path)
    parser.add_argument("--cmake", default="cmake")
    arguments = parser.parse_args()
    _, manifest = build(arguments.source, arguments.output, arguments.cmake)
    print(json.dumps({key: manifest[key] for key in ("archive", "archive_sha256")}))


if __name__ == "__main__":
    main()
