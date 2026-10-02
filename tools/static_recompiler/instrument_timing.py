#!/usr/bin/env python3
"""Copy sealed generated code, add stock Azahar block costs, and link a timed library."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import shutil
import struct
import subprocess
import sys

from rewrite_floating_point import rewrite as rewrite_floating_point
from build_floating_point import build as build_floating_point

ROOT = Path(__file__).resolve().parents[2]
AZAHAR_REVISION = "662d412123305a9f4be94dd3dc73ddf91a18c55e"


def run(arguments, log=None, input_bytes=None):
    result = subprocess.run([str(v) for v in arguments], input=input_bytes, stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE)
    if log:
        Path(log).write_bytes(result.stdout + result.stderr)
    if result.returncode:
        raise RuntimeError(f"command failed: {arguments}\n{result.stderr.decode(errors='replace')}")
    return result.stdout


def digest(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("source", type=Path)
    parser.add_argument("azahar", type=Path)
    parser.add_argument("output", type=Path)
    parser.add_argument("--softfloat-source", required=True, type=Path,
                        help="unmodified official SoftFloat 3e source directory")
    parser.add_argument("--cmake", default="cmake")
    args = parser.parse_args()
    source, azahar, output = args.source.resolve(), args.azahar.resolve(), args.output.resolve()
    if not output.is_relative_to(ROOT / "build") or output.exists():
        raise RuntimeError("timed output must be an absent child of this checkout's ignored build directory")
    manifest = json.loads((source / "build_manifest.json").read_text())
    if digest(manifest["library"]) != manifest["library_sha256"]:
        raise RuntimeError("sealed source library changed")
    if run(["git", "-C", azahar, "rev-parse", "HEAD"]).decode().strip() != AZAHAR_REVISION:
        raise RuntimeError("Azahar cost-model revision differs")
    tick_source = "src/core/arm/dynarmic/arm_tick_counts.cpp"
    original = run(["git", "-C", azahar, "show", f"{AZAHAR_REVISION}:{tick_source}"])
    if (azahar / tick_source).read_bytes() != original:
        raise RuntimeError("Azahar instruction timing source was modified")
    output.mkdir(parents=True)
    for path in source.iterdir():
        if path.suffix in (".c", ".h", ".cpp"):
            shutil.copy2(path, output / path.name)
    for relative, expected in manifest["sources"].items():
        copied = output / Path(relative).name
        if digest(copied) != expected:
            raise RuntimeError(f"generated source seal failed: {copied}")
    for name in ("NativeTiming.h", "NativeTiming.c", "NativeFloatingPoint.h", "NativeFloatingPoint.c"):
        shutil.copy2(ROOT / "runtime/port" / name, output / name)
    # Compile the pinned stock cost function as a generation-time helper. It never executes
    # guest instructions and does not decode at runtime in the port.
    helper = output / "instruction_cost.cpp"
    helper.write_text('''#include <cstdint>
#include <iostream>
#include "core/arm/dynarmic/arm_tick_counts.h"
int main() { std::uint32_t pair[2]; while (std::cin.read(reinterpret_cast<char*>(pair), 8)) {
    std::uint64_t cost = Core::TicksForInstruction(pair[0] != 0, pair[1]);
    std::cout.write(reinterpret_cast<const char*>(&cost), 8);
} }
''')
    run(["clang++", "-std=c++20", "-O1", "-I", azahar / "src", helper,
         azahar / tick_source, "-o", output / "instruction_cost"], output / "instruction_cost_build.log")
    instruction = re.compile(r"/\* ([0-9A-F]{8}) ([0-9A-F]{4,8}) \*/")
    block = re.compile(r"BUDGET\(0x([0-9A-F]{8})u, (\d+)\);(.*?)(?=\nL_[0-9A-F]{8}:|\n\})", re.S)
    sources = sorted(output.glob("code*.c"))
    pairs = sorted({(len(opcode) == 4, int(opcode, 16)) for path in sources
                    for _, opcode in instruction.findall(path.read_text())})
    data = b"".join(struct.pack("<II", thumb, opcode) for thumb, opcode in pairs)
    costs = run([output / "instruction_cost"], input_bytes=data)
    if len(costs) != len(pairs) * 8:
        raise RuntimeError("cost helper output extent disagrees")
    lookup = {pair: struct.unpack_from("<Q", costs, index * 8)[0] for index, pair in enumerate(pairs)}
    blocks = 0
    for path in sources:
        def replace(match):
            nonlocal blocks
            words = instruction.findall(match[3])
            if len(words) != int(match[2]):
                raise RuntimeError(f"block instruction extent disagrees at {match[1]}")
            if len(words) != 1:
                raise RuntimeError("timed generation requires one resumable instruction per budget")
            _, opcode = words[0]
            thumb, operation = len(opcode) == 4, int(opcode, 16)
            ticks = lookup[(thumb, operation)]
            condition = ((operation >> 8) & 15) if thumb and operation & 0xF000 == 0xD000 else (14 if thumb else operation >> 28)
            expression = f"NativeConditionalTicks(ctx, {condition}, {ticks}ull)"
            supervisor = (thumb and operation & 0xFF00 == 0xDF00) or (not thumb and operation & 0x0F000000 == 0x0F000000)
            if supervisor:
                expression = f"NativeConditionalTicks(ctx, {condition}, NATIVE_SUPERVISOR_TICKS({ticks}ull))"
            blocks += 1
            return f"BUDGET(0x{match[1]}u, {match[2]}, {expression});" + match[3]
        text = path.read_text().replace('#include "recomp.h"', '#include "NativeTiming.h"', 1)
        text = block.sub(replace, text)
        if re.search(r"BUDGET\(0x[0-9A-F]+u, \d+\);", text):
            raise RuntimeError("uninstrumented block remains")
        path.write_text(text)
    floating_point_operations = rewrite_floating_point(output)
    # The only registered source replacement uses four original instructions for >=32 and
    # eight otherwise, each one cycle in the pinned stock model. This is port timing metadata.
    replacement = output / "replacement.cpp"
    text = replacement.read_text().replace('#include "recomp.h"', '#include "NativeTiming.h"')
    marker = 'extern "C" RECOMP_OVERRIDE(0x0010766C) {'
    if marker not in text:
        raise RuntimeError("reviewed priority replacement is absent")
    text = text.replace(marker, marker + '\n    const uint32_t count = static_cast<int32_t>(ctx->r[0]) >= 32 ? 4 : 8;\n    BUDGET(0x0010766Cu, count, count);')
    replacement.write_text(text)
    original_source = manifest["replacement_sources"]["lib/CtrSDK/sources/os_Priority.cpp"]
    if digest(ROOT / "lib/CtrSDK/sources/os_Priority.cpp") != original_source:
        raise RuntimeError("exact source differs from frozen native build")
    objects = []
    arithmetic_archive, arithmetic_manifest = build_floating_point(
        args.softfloat_source, output / "floating_point", args.cmake)
    for name, key in (("NativeFloatingPoint.c", "adapter_sha256"),
                      ("NativeFloatingPoint.h", "header_sha256")):
        if digest(output / name) != arithmetic_manifest[key]:
            raise RuntimeError(f"copied arithmetic source differs from the linked target: {name}")
    for index, path in enumerate(sorted(output.glob("*.c"))):
        if path.name == "NativeFloatingPoint.c":
            continue
        object_file = path.with_suffix(".o")
        run(["clang", "-O1", "-ffp-contract=off", "-fno-math-errno", "-frounding-math", "-fno-fast-math", "-fPIC", "-fvisibility=hidden",
             "-I", output, "-c", path, "-o", object_file], output / f"compile_{index:03}.log")
        objects.append(object_file)
        print(f"compiled timed source {index + 1}", flush=True)
    object_file = output / "replacement.o"
    run(["clang++", "-std=c++20", "-O1", "-ffp-contract=off", "-fPIC", "-I", ROOT, "-I", output,
         "-c", replacement, "-o", object_file], output / "replacement_build.log")
    objects.append(object_file)
    library = output / ("translated.dylib" if sys.platform == "darwin" else "translated.so")
    run(["clang", "-shared", *objects, arithmetic_archive, "-lm", "-o", library], output / "link.log")
    manifest.update({"azahar_timing_revision": AZAHAR_REVISION, "timing_blocks": blocks,
                     "library": str(library), "library_sha256": digest(library),
                     "sources": {str(path.relative_to(ROOT)): digest(path) for path in sorted(output.iterdir()) if path.suffix in (".c", ".h", ".cpp")},
                     "timing_callback": "native_block_timing_callback",
                     "native_timing_revision": 2,
                     "floating_point_operations_rewritten": floating_point_operations,
                     "floating_point_library": arithmetic_manifest,
                     "runtime_verified": False,
                     "gpu_frame_verified": False})
    (output / "build_manifest.json").write_text(json.dumps(manifest, indent=2) + "\n")
    shutil.copy2(source / "function_map.csv", output / "function_map.csv")
    print(json.dumps({"timing_blocks": blocks, "library_sha256": manifest["library_sha256"]}))


if __name__ == "__main__":
    main()
