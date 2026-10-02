#!/usr/bin/env python3
"""Link sealed wasm32 translations and compare real address dispatch with original ARM execution."""
import argparse
from collections import Counter
import hashlib
import json
import os
from pathlib import Path
import random
import re
import struct
import subprocess

import capstone
import unicorn
from unicorn import arm_const as arm

from verify_execution import REGISTERS
from verify_floating_point_execution import SAMPLES, arithmetic_fixtures, assign_register, base_fixtures
from verify_floating_point_webassembly import ORIGINAL_SHA256

ROOT = Path(__file__).resolve().parents[2]
REQUEST_BYTES = 208
RESULT_BYTES = 200
FLAGS_MASK = 0xF80F0020
PRIORITY_ADDRESS = 0x0010766C


def digest(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def original_fixtures(requests, expected):
    code = (ROOT / "data/ver/eu/code.bin").read_bytes()
    if hashlib.sha256(code).hexdigest() != ORIGINAL_SHA256:
        raise RuntimeError("original EU executable hash differs")
    reference = unicorn.Uc(unicorn.UC_ARCH_ARM, unicorn.UC_MODE_ARM)
    reference.ctl_set_cpu_model(arm.UC_CPU_ARM_11MPCORE)
    reference.mem_map(0x100000, 0x300000)
    reference.mem_write(0x100000, code)
    reference.reg_write(arm.UC_ARM_REG_FPEXC, 0x40000000)
    disassembler = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_ARM)
    counts = Counter()
    executed_instructions = 0

    def instruction(*unused):
        nonlocal executed_instructions
        executed_instructions += 1

    reference.hook_add(unicorn.UC_HOOK_CODE, instruction)

    def record(request_stream, expected_stream, address, budget, registers, flags, words, fpscr):
        nonlocal executed_instructions
        executed_instructions = 0
        reference.reg_write(arm.UC_ARM_REG_CPSR, 0x10 | flags)
        for register, value in zip(REGISTERS, registers):
            reference.reg_write(register, value)
        for index, value in enumerate(words):
            reference.reg_write(getattr(arm, f"UC_ARM_REG_S{index}"), value)
        reference.reg_write(arm.UC_ARM_REG_FPSCR, fpscr)
        reference.emu_start(address, 0xFFFF0000 if address == PRIORITY_ADDRESS else address + 4, count=budget)
        if executed_instructions != budget:
            raise RuntimeError(f"original instruction extent differs at {address:#x}")
        request_stream.write(struct.pack("<52I", address, budget, *registers, flags, *words, fpscr))
        expected_stream.write(struct.pack("<50I",
            *(reference.reg_read(register) for register in REGISTERS),
            reference.reg_read(arm.UC_ARM_REG_CPSR) & FLAGS_MASK,
            *(reference.reg_read(getattr(arm, f"UC_ARM_REG_S{index}")) for index in range(32)),
            reference.reg_read(arm.UC_ARM_REG_FPSCR)))

    with requests.open("xb") as request_stream, expected.open("xb") as expected_stream:
        for mnemonic, address, operands in SAMPLES:
            decoded = next(disassembler.disasm(code[address - 0x100000:address - 0x100000 + 4], address))
            if decoded.mnemonic != mnemonic or tuple(decoded.op_str.split(", ")) != operands:
                raise RuntimeError(f"original sample identity differs at {address:#x}")
            wide = mnemonic.endswith(".f64")
            operation = mnemonic.split(".")[0]
            fixtures = base_fixtures(wide) if operation in ("vmov", "vabs", "vneg") else arithmetic_fixtures(wide)
            if operation in ("vmla", "vmls", "vnmla", "vnmls"):
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
                            registers = [0x12340000 | index for index in range(16)]
                            registers[13:16] = [0x10000000, 0xFFFF0000, address]
                            record(request_stream, expected_stream, address, 1, registers, 0xA8000000,
                                   words, controls | mode << 22 | sticky)
                            counts[mnemonic] += 1

        generator = random.Random(0x53F00)
        values = [0, 23, 24, 31, 32, 63, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFF]
        values += [generator.getrandbits(32) for _ in range(4096)]
        for value in values:
            registers = [generator.getrandbits(32) for _ in range(16)]
            registers[0], registers[13], registers[14], registers[15] = value, 0x10000000, 0xFFFF0000, PRIORITY_ADDRESS
            budget = 4 if 32 <= value < 0x80000000 else 8
            record(request_stream, expected_stream, PRIORITY_ADDRESS, budget, registers,
                   generator.getrandbits(5) << 27, [0] * 32, 0)
            counts["priority_source_replacement"] += 1
    return dict(counts)


def compare_results(requests, expected, actual, counts):
    total = sum(counts.values())
    if requests.stat().st_size != total * REQUEST_BYTES or expected.stat().st_size != total * RESULT_BYTES:
        raise RuntimeError("reference stream extent differs")
    if actual.stat().st_size != total * RESULT_BYTES:
        raise RuntimeError("actual result stream extent differs")
    with requests.open("rb") as request_stream, expected.open("rb") as expected_stream, actual.open("rb") as actual_stream:
        for index in range(total):
            request = struct.unpack("<52I", request_stream.read(REQUEST_BYTES))
            reference = struct.unpack("<50I", expected_stream.read(RESULT_BYTES))
            result = struct.unpack("<50I", actual_stream.read(RESULT_BYTES))
            # The source replacement follows AAPCS. Its caller-saved registers
            # and arithmetic flags are outside the source function contract.
            compared = [0, *range(4, 14), 15, *range(17, 50)] if request[0] == PRIORITY_ADDRESS else range(50)
            differences = [{"word": word, "expected": reference[word], "actual": result[word]}
                           for word in compared if reference[word] != result[word]]
            if differences:
                return {"case": index, "address": request[0], "differences": differences}
    return None


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("translation", type=Path, help="sealed wasm32 archive output")
    parser.add_argument("output", type=Path)
    parser.add_argument("--emsdk", type=Path, required=True)
    parser.add_argument("--node", type=Path, required=True)
    parser.add_argument("--cmake", default="cmake")
    args = parser.parse_args()
    translation, output = args.translation.resolve(), args.output.resolve()
    if not output.is_relative_to(ROOT / "build") or output.exists():
        raise RuntimeError("output must be an absent child of this checkout's ignored build")
    manifest = json.loads((translation / "build_manifest.json").read_text())
    archive = Path(manifest["archive"])
    if digest(archive) != manifest["archive_sha256"]:
        raise RuntimeError("wasm32 translation archive changed")
    sources = translation / "sources"
    for relative, expected in manifest["sources"].items():
        if digest(sources / Path(relative).name) != expected:
            raise RuntimeError(f"wasm32 translation source changed: {relative}")
    declarations = re.findall(r"\{(0x[0-9A-Fa-f]+)u, [A-Za-z0-9_]+\}", (sources / "entries.c").read_text())
    expected_addresses = b"".join(struct.pack("<I", int(address, 16)) for address in declarations)
    if len(declarations) != manifest["declared_instruction_entries"]:
        raise RuntimeError("sealed address table declaration differs")
    arithmetic_archive = archive.parent / "floating_point/libport_floating_point.a"
    arithmetic_sha256 = digest(arithmetic_archive)
    output.mkdir(parents=True)
    requests, expected, actual = (output / name for name in ("requests.bin", "expected_results.bin", "actual_results.bin"))
    addresses = output / "actual_addresses.bin"
    counts = original_fixtures(requests, expected)
    (output / "CMakeLists.txt").write_text(f'''cmake_minimum_required(VERSION 3.24)
project(WebAssemblyTranslationVerification LANGUAGES C CXX)
add_library(translation STATIC IMPORTED)
set_target_properties(translation PROPERTIES IMPORTED_LOCATION "{archive}")
add_library(arithmetic STATIC IMPORTED)
set_target_properties(arithmetic PROPERTIES IMPORTED_LOCATION "{arithmetic_archive}")
add_executable(verify_webassembly_translation "{ROOT / 'tools/static_recompiler/verify_webassembly_translation.c'}")
set_target_properties(verify_webassembly_translation PROPERTIES C_STANDARD 11 C_STANDARD_REQUIRED YES LINKER_LANGUAGE CXX)
target_include_directories(verify_webassembly_translation PRIVATE "{sources}")
target_compile_options(verify_webassembly_translation PRIVATE -O1 -pthread -Wall -Wextra -Werror)
target_link_libraries(verify_webassembly_translation PRIVATE translation arithmetic)
target_link_options(verify_webassembly_translation PRIVATE -O1 -pthread
    -sENVIRONMENT=node -sNODERAWFS=1 -sPTHREAD_POOL_SIZE=1
    -sPTHREAD_POOL_SIZE_STRICT=2 -sPROXY_TO_PTHREAD=1 -sEXIT_RUNTIME=1
    -sALLOW_MEMORY_GROWTH=1)
''')
    environment = os.environ.copy()
    environment["EMSDK_NODE"] = str(args.node.resolve())
    build = output / "compiled"
    commands = [
        [str(args.emsdk.resolve() / "upstream/emscripten/emcmake"), str(args.cmake),
         "-S", str(output), "-B", str(build), "-DCMAKE_BUILD_TYPE=Release"],
        [str(args.cmake), "--build", str(build), "--parallel", "1"],
        [str(args.node.resolve()), str(build / "verify_webassembly_translation.js"), str(requests), str(actual), str(addresses)],
    ]
    for index, command in enumerate(commands):
        with (output / f"command_{index}.log").open("wb") as log:
            subprocess.run(command, env=environment, stdout=log, stderr=subprocess.STDOUT, check=True)
    execution = json.loads((output / "command_2.log").read_text().strip())
    if (execution["actual_instruction_entries"] != len(declarations)
            or execution["completed_cases"] != sum(counts.values())
            or execution["interpreter_calls"] != 0 or execution["source_binding_matches"] != 1
            or addresses.read_bytes() != expected_addresses):
        raise RuntimeError("actual linked address table or execution extent differs")
    first_difference = compare_results(requests, expected, actual, counts)
    public_sources = [Path(__file__), ROOT / "tools/static_recompiler/verify_webassembly_translation.c",
                      ROOT / "tools/static_recompiler/build_webassembly_translation.py",
                      ROOT / "runtime/port/webassembly_translation/CMakeLists.txt"]
    public_sources += [ROOT / "tools/static_recompiler" / name for name in
                       ("verify_execution.py", "verify_floating_point_execution.py",
                        "verify_floating_point_webassembly.py", "build_port.py")]
    public_sources += [ROOT / "runtime/port" / name for name in
                       ("NativeFloatingPoint.c", "NativeFloatingPoint.h", "NativeTiming.c", "NativeTiming.h")]
    public_sources += sorted(path for path in (ROOT / "runtime/port/floating_point").iterdir() if path.is_file())
    receipt = {"passed": first_difference is None, "original_sha256": ORIGINAL_SHA256,
               "unicorn_version": unicorn.__version__, "frozen_main": manifest["frozen_main"],
               "translation_manifest_sha256": digest(translation / "build_manifest.json"),
               "translation_archive_sha256": digest(archive), "arithmetic_archive_sha256": arithmetic_sha256,
               "case_counts": counts, "total_cases": sum(counts.values()), "result_bytes": actual.stat().st_size,
               "first_difference": first_difference, "commands": commands, "execution": execution,
               "linked_translated_function_bytes": manifest["native_recompiled_bytes"],
               "public_sources": {str(path.relative_to(ROOT)): digest(path) for path in public_sources},
               "requests_sha256": digest(requests), "expected_results_sha256": digest(expected),
               "actual_results_sha256": digest(actual), "actual_addresses_sha256": digest(addresses),
               "wasm_sha256": digest(build / "verify_webassembly_translation.wasm"),
               "scope": "All sealed translated entries linked into wasm32. Selected actual guest entries and one rank-O source replacement execute in Node; compared with original ARM.",
               "gaps": "No browser host/frame, translated startup replay, complete rank-O registry, World 1-1 or semantic state acceptance."}
    (output / "result.json").write_text(json.dumps(receipt, indent=2) + "\n")
    print(json.dumps({key: receipt[key] for key in ("passed", "total_cases", "result_bytes", "first_difference", "execution")}))
    if not receipt["passed"]:
        raise RuntimeError("actual wasm32 guest entries differ from original ARM")


if __name__ == "__main__":
    main()
