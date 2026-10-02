#!/usr/bin/env python3
"""Compare the actual wasm32 guest arithmetic target with selected original ARM instructions."""
import argparse
from collections import Counter
import hashlib
import json
import os
from pathlib import Path
import struct
import subprocess

import capstone
import unicorn
from unicorn import arm_const as arm

from verify_floating_point_execution import (
    SAMPLES, arithmetic_fixtures, assign_register, base_fixtures,
)

ROOT = Path(__file__).resolve().parents[2]
ORIGINAL_SHA256 = "e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64"
OPERATIONS = {
    "vmla": 0, "vmls": 1, "vnmls": 2, "vnmla": 3, "vmul": 4,
    "vnmul": 5, "vadd": 6, "vsub": 7, "vdiv": 8,
    "vmov": 9, "vabs": 10, "vneg": 11, "vsqrt": 12,
}


def digest(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def original_fixtures(path):
    code = (ROOT / "data/ver/eu/code.bin").read_bytes()
    if hashlib.sha256(code).hexdigest() != ORIGINAL_SHA256:
        raise RuntimeError("original EU executable hash differs")
    reference = unicorn.Uc(unicorn.UC_ARCH_ARM, unicorn.UC_MODE_ARM)
    reference.mem_map(0x100000, 0x300000)
    reference.mem_write(0x100000, code)
    reference.reg_write(arm.UC_ARM_REG_FPEXC, 0x40000000)
    disassembler = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_ARM)
    counts = Counter()
    with path.open("xb") as stream:
        for mnemonic, address, operands in SAMPLES:
            instruction = next(disassembler.disasm(code[address - 0x100000:address - 0x100000 + 4], address))
            if instruction.mnemonic != mnemonic or tuple(instruction.op_str.split(", ")) != operands:
                raise RuntimeError(f"original sample identity differs at {address:#x}")
            wide = mnemonic.endswith(".f64")
            operation = OPERATIONS[mnemonic.split(".")[0]]
            fixtures = base_fixtures(wide) if operation in (9, 10, 11) else arithmetic_fixtures(wide)
            if operation <= 3:
                fixtures += [("separate_rounding",
                              0x3FF0000000000001 if wide else 0x3F800001,
                              0x3FEFFFFFFFFFFFFE if wide else 0x3F7FFFFE,
                              0xBFF0000000000000 if wide else 0xBF800000)]
            for mode in range(4):
                for controls in (0, 1 << 24, 1 << 25, 3 << 24):
                    for sticky in (0, 0x9F):
                        for _, left, right, accumulator in fixtures:
                            words = [0xA5A50000 | index for index in range(32)]
                            assign_register(words, operands[0], accumulator)
                            if len(operands) == 3:
                                assign_register(words, operands[1], left)
                                assign_register(words, operands[2], right)
                            else:
                                assign_register(words, operands[1], right)

                            def operand(name):
                                index = int(name[1:])
                                return (words[2 * index] | words[2 * index + 1] << 32
                                        if name[0] == "d" else words[index])

                            accumulator, right = operand(operands[0]), operand(operands[-1])
                            left = operand(operands[1]) if len(operands) == 3 else 0
                            initial = controls | mode << 22 | sticky
                            reference.reg_write(arm.UC_ARM_REG_CPSR, 0x10)
                            for index, value in enumerate(words):
                                reference.reg_write(getattr(arm, f"UC_ARM_REG_S{index}"), value)
                            reference.reg_write(arm.UC_ARM_REG_FPSCR, initial)
                            reference.emu_start(address, address + 4, count=1)
                            expected = reference.reg_read(getattr(arm, "UC_ARM_REG_" + operands[0].upper()))
                            status = reference.reg_read(arm.UC_ARM_REG_FPSCR)
                            stream.write(struct.pack("<IIIQQQQI", operation, wide, initial,
                                                     left, right, accumulator, expected, status))
                            counts[mnemonic] += 1
    return dict(counts)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("softfloat_source", type=Path)
    parser.add_argument("output", type=Path)
    parser.add_argument("--emsdk", type=Path, required=True)
    parser.add_argument("--node", type=Path, required=True)
    parser.add_argument("--cmake", default="cmake")
    args = parser.parse_args()
    output = args.output.resolve()
    if not output.is_relative_to(ROOT / "build") or output.exists():
        raise RuntimeError("verification output must be an absent child of this checkout's ignored build")
    output.mkdir(parents=True)
    fixtures, results = output / "fixtures.bin", output / "actual_results.bin"
    counts = original_fixtures(fixtures)
    # Generated build glue contains paths to public tools, never guest code.
    (output / "CMakeLists.txt").write_text(f'''cmake_minimum_required(VERSION 3.24)
project(FloatingPointVerification LANGUAGES C)
add_subdirectory("{ROOT / 'runtime/port/floating_point'}" arithmetic)
add_executable(verify_floating_point
    "{ROOT / 'tools/static_recompiler/verify_floating_point_webassembly.c'}"
    "{ROOT / 'tools/static_recompiler/verify_floating_point_environment.c'}")
set_target_properties(verify_floating_point PROPERTIES C_STANDARD 11 C_STANDARD_REQUIRED YES)
target_compile_definitions(verify_floating_point PRIVATE THREAD_LOCAL=_Thread_local SOFTFLOAT_FAST_INT64=1)
target_include_directories(verify_floating_point PRIVATE "{args.softfloat_source.resolve() / 'include'}")
target_compile_options(verify_floating_point PRIVATE -Wall -Wextra -Werror)
target_link_libraries(verify_floating_point PRIVATE port_floating_point)
target_link_options(verify_floating_point PRIVATE
    -sENVIRONMENT=node -sNODERAWFS=1 -sPTHREAD_POOL_SIZE=3
    -sPTHREAD_POOL_SIZE_STRICT=2 -sPROXY_TO_PTHREAD=1 -sEXIT_RUNTIME=1)
''')
    build = output / "compiled"
    environment = os.environ.copy()
    environment["EMSDK_NODE"] = str(args.node.resolve())
    commands = [
        [str(args.emsdk.resolve() / "upstream/emscripten/emcmake"), str(args.cmake),
         "-S", str(output), "-B", str(build), "-DCMAKE_BUILD_TYPE=Release",
         f"-DROOT_PORT_SOFTFLOAT_SOURCE_DIRECTORY={args.softfloat_source.resolve()}"],
        [str(args.cmake), "--build", str(build), "--parallel", "1"],
        [str(args.node.resolve()), str(build / "verify_floating_point.js"), str(fixtures), str(results)],
    ]
    for index, command in enumerate(commands):
        with (output / f"command_{index}.log").open("wb") as log:
            subprocess.run(command, env=environment, stdout=log, stderr=subprocess.STDOUT, check=True)
    expected_raw, actual_raw = fixtures.read_bytes(), results.read_bytes()
    if len(actual_raw) != sum(counts.values()) * 12:
        raise RuntimeError("actual result stream extent differs")
    first_difference = None
    for index in range(sum(counts.values())):
        expected = struct.unpack_from("<QI", expected_raw, index * 48 + 36)
        actual = struct.unpack_from("<QI", actual_raw, index * 12)
        if expected != actual:
            first_difference = {"index": index, "expected": expected, "actual": actual}
            break
    source_paths = [ROOT / "runtime/port" / name for name in ("NativeFloatingPoint.c", "NativeFloatingPoint.h")]
    source_paths += sorted((ROOT / "runtime/port/floating_point").iterdir())
    source_paths += [Path(__file__), ROOT / "tools/static_recompiler/verify_floating_point_webassembly.c",
                     ROOT / "tools/static_recompiler/verify_floating_point_environment.c"]
    receipt = {"passed": first_difference is None, "original_sha256": ORIGINAL_SHA256,
               "unicorn_version": unicorn.__version__, "case_counts": counts,
               "total_cases": sum(counts.values()), "result_bytes": len(actual_raw),
               "first_difference": first_difference, "commands": commands,
               "public_sources": {str(path.relative_to(ROOT)): digest(path) for path in source_paths if path.is_file()},
               "fixture_sha256": digest(fixtures), "result_sha256": digest(results),
               "wasm_sha256": digest(build / "verify_floating_point.wasm"),
               "archive_sha256": digest(build / "arithmetic/libport_floating_point.a"),
               "execution_output": (output / "command_2.log").read_text(),
               "scope": "Selected original instructions versus the public arithmetic API in Node wasm32. No translated guest entries or browser game execution.",
               "gaps": "Original double samples cover add/subtract/multiply/divide/move. Exception delivery, conversions/comparisons, short vectors and browser host remain unverified."}
    (output / "result.json").write_text(json.dumps(receipt, indent=2) + "\n")
    print(json.dumps({key: receipt[key] for key in ("passed", "total_cases", "result_bytes", "first_difference", "execution_output")}))
    if not receipt["passed"]:
        raise RuntimeError("wasm arithmetic differs from original execution")


if __name__ == "__main__":
    main()
