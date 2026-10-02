#!/usr/bin/env python3
"""Derive sealed native scheduling metadata with the pinned public Dynarmic frontend."""
import argparse
import collections
import hashlib
import json
from pathlib import Path
import re
import struct
import subprocess

ROOT = Path(__file__).resolve().parents[2]
AZAHAR_REVISION = "662d412123305a9f4be94dd3dc73ddf91a18c55e"
DYNARMIC_REVISION = "e77b1ba0b7da7cbe93021b01a663acfe7c4dd516"
ORIGINAL_HASH = "e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64"


def digest(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def execute(arguments, *, input_bytes=None):
    result = subprocess.run([str(value) for value in arguments], input=input_bytes,
                            stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=True)
    return result.stdout


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("translated_directory", type=Path)
    parser.add_argument("azahar_directory", type=Path)
    parser.add_argument("azahar_build", type=Path)
    parser.add_argument("output", type=Path)
    parser.add_argument("--dependency-prefix", type=Path, default=Path("/opt/homebrew"))
    parser.add_argument("--mode", type=lambda value: int(value, 0), action="append")
    args = parser.parse_args()
    source, azahar, build, output = (value.resolve() for value in
        (args.translated_directory, args.azahar_directory, args.azahar_build, args.output))
    if not output.is_relative_to(ROOT / "build") or output.exists():
        raise RuntimeError("schedule output must be absent and inside this checkout's ignored build directory")
    modes = sorted(set(args.mode or [0x03000000, 0x03C00000]))
    if any(mode & ~0x07F70000 or mode & 0x00370000 for mode in modes):
        raise RuntimeError("only explicitly requested scalar FPSCR modes are supported")
    original = ROOT / "data/ver/eu/code.bin"
    if digest(original) != ORIGINAL_HASH:
        raise RuntimeError("approved original code.bin SHA256 differs")
    manifest = json.loads((source / "build_manifest.json").read_text())
    if digest(manifest["library"]) != manifest["library_sha256"]:
        raise RuntimeError("translated library seal differs")
    for relative, expected in manifest["sources"].items():
        if digest(source / Path(relative).name) != expected:
            raise RuntimeError(f"translated source seal differs: {relative}")
    dynarmic = azahar / "externals/dynarmic"
    for directory, revision in ((azahar, AZAHAR_REVISION), (dynarmic, DYNARMIC_REVISION)):
        if execute(["git", "-C", directory, "rev-parse", "HEAD"]).decode().strip() != revision:
            raise RuntimeError("public frontend source revision differs")
    tick_source = azahar / "src/core/arm/dynarmic/arm_tick_counts.cpp"
    if tick_source.read_bytes() != execute(["git", "-C", azahar, "show",
            AZAHAR_REVISION + ":src/core/arm/dynarmic/arm_tick_counts.cpp"]):
        raise RuntimeError("stock cost model source differs")
    if execute(["git", "-C", dynarmic, "diff", "HEAD", "--", "src/dynarmic/frontend/A32", "src/dynarmic/ir"]):
        raise RuntimeError("public frontend or IR source is modified")
    # Original instruction comments supply the address inventory, never runtime decoding.
    addresses = set()
    instruction = re.compile(r"/\* ([0-9A-F]{8}) ([0-9A-F]{4,8}) \*/")
    for path in sorted(source.glob("code*.c")):
        for address, opcode in instruction.findall(path.read_text()):
            addresses.add(int(address, 16) | (len(opcode) == 4))
    if not addresses:
        raise RuntimeError("generated resumable instruction inventory is empty")
    output.parent.mkdir(parents=True, exist_ok=True)
    helper = output.parent / "derive_native_block_schedule"
    frontend_archive = build / "externals/dynarmic/src/dynarmic/libdynarmic.a"
    support_archive = build / "externals/dynarmic/externals/mcl/src/libmcl.a"
    arguments = ["clang++", "-std=c++20", "-O2", "-Wall", "-Wextra", "-Werror",
        "-I", azahar / "src", "-I", dynarmic / "src",
        "-I", dynarmic / "externals/mcl/include", "-I", azahar / "externals/boost",
        "-I", args.dependency_prefix / "include",
        ROOT / "tools/static_recompiler/derive_native_block_schedule.cpp", tick_source,
        frontend_archive, support_archive, "-L", args.dependency_prefix / "lib", "-lfmt", "-o", helper]
    execute(arguments)
    (output.parent / "schedule_helper_compile_arguments.json").write_text(json.dumps([str(v) for v in arguments], indent=2) + "\n")
    record_bytes = bytearray()
    node_bytes = bytearray()
    kinds = collections.Counter()
    unsupported = 0
    for mode in modes:
        ordered = sorted(addresses)
        request = b"".join(struct.pack("<II", address, mode) for address in ordered)
        response = execute([helper, original], input_bytes=request)
        cursor = 0
        for address in ordered:
            if cursor + 72 > len(response):
                raise RuntimeError("incomplete frontend schedule record")
            fields = struct.unpack_from("<10I3Q2I", response, cursor)
            cursor += 72
            count = fields[-1]
            if fields[0:2] != (address, mode) or not 0 < count <= 64 or cursor + 32 * count > len(response):
                raise RuntimeError("frontend record identity or terminal extent differs")
            node_offset = len(node_bytes) // 32
            record_bytes.extend(struct.pack("<10I3Q4I", *fields[:14], node_offset, count, 0))
            node_bytes.extend(response[cursor:cursor + 32 * count])
            cursor += 32 * count
            kinds[fields[8] & 0x3FFFFFFF] += 1
            unsupported += bool(fields[8] & 0x80000000)
        if cursor != len(response):
            raise RuntimeError("trailing frontend schedule output")
        print(f"derived {len(ordered)} scheduling records for FPSCR {mode:08X}", flush=True)
    header = b"SM3DBS02" + struct.pack("<5I", 2, len(record_bytes) // 80, len(node_bytes) // 32, 80, 32)
    header += ORIGINAL_HASH.encode("ascii") + struct.pack("<I", 0)
    output.write_bytes(header + record_bytes + node_bytes)
    receipt = {
        "version": 2, "original_sha256": ORIGINAL_HASH, "azahar_revision": AZAHAR_REVISION,
        "dynarmic_revision": DYNARMIC_REVISION, "modes": modes,
        "unique_instruction_addresses": len(addresses), "records": len(record_bytes) // 80,
        "terminal_nodes": len(node_bytes) // 32, "unsupported_records": unsupported,
        "terminal_kinds": dict(kinds), "schedule_sha256": digest(output), "schedule": str(output),
        "translated_library_sha256": manifest["library_sha256"],
        "frontend_archive_sha256": digest(frontend_archive), "support_archive_sha256": digest(support_archive),
        "helper_source_sha256": digest(ROOT / "tools/static_recompiler/derive_native_block_schedule.cpp"),
        "helper_sha256": digest(helper), "stock_cost_source_sha256": digest(tick_source),
        "runtime_verified": False,
    }
    output.with_suffix(".json").write_text(json.dumps(receipt, indent=2) + "\n")
    print(json.dumps(receipt, indent=2))


if __name__ == "__main__":
    main()
