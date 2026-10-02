#!/usr/bin/env python3
"""Compare generated one-instruction native VFP entries with original ARM execution."""
import argparse
import ctypes
import hashlib
import json
from pathlib import Path
import random
import struct

import capstone
import unicorn
from unicorn import arm_const as arm

from verify_execution import CODE, Context, Execution, REGISTERS, ROOT


# Addresses and architectural operand names only. The original instruction words
# are read from the owner's locally hashed executable, never embedded here.
SAMPLES = (
    ("vabs.f32", 0x00215E2C, ("s0", "s0")),
    ("vadd.f32", 0x00109FEC, ("s1", "s0", "s1")),
    ("vadd.f64", 0x0023BAE0, ("d0", "d0", "d8")),
    ("vdiv.f32", 0x00109E04, ("s1", "s1", "s2")),
    ("vdiv.f64", 0x001F402C, ("d1", "d0", "d8")),
    ("vmla.f32", 0x0010C524, ("s3", "s0", "s12")),
    ("vmls.f32", 0x0010C3B0, ("s10", "s13", "s2")),
    ("vmov.f32", 0x001002AC, ("s3", "s16")),
    ("vmov.f64", 0x0012A1E8, ("d8", "d0")),
    ("vmul.f32", 0x00109DF8, ("s1", "s1", "s2")),
    ("vmul.f64", 0x001F3FA4, ("d8", "d1", "d0")),
    ("vneg.f32", 0x00109E68, ("s1", "s1")),
    ("vnmla.f32", 0x0010EDA8, ("s12", "s8", "s5")),
    ("vnmls.f32", 0x0011E5AC, ("s0", "s1", "s17")),
    ("vnmul.f32", 0x0010C430, ("s1", "s17", "s15")),
    ("vsqrt.f32", 0x0010ED24, ("s3", "s3")),
    ("vsub.f32", 0x00109D8C, ("s0", "s0", "s1")),
    ("vsub.f64", 0x0023BAB4, ("d1", "d1", "d2")),
)


def floating_bits(value, wide):
    return int.from_bytes(struct.pack("<d" if wide else "<f", value), "little")


def base_fixtures(wide):
    fraction_width = 52 if wide else 23
    sign = 1 << (63 if wide else 31)
    minimum_normal = 1 << fraction_width
    maximum = 0x7FEFFFFFFFFFFFFF if wide else 0x7F7FFFFF
    quiet_nan = 0x7FF8123456789ABC if wide else 0x7FC12345
    signaling_nan = 0x7FF0123456789ABC if wide else 0x7FA12345
    bits = lambda value: floating_bits(value, wide)
    return [
        ("ordinary", bits(2), bits(3), bits(1)),
        ("positive_rounding", bits(1), bits(3 * 2 ** (-fraction_width - 2)), bits(1)),
        ("negative_rounding", bits(-1), bits(-3 * 2 ** (-fraction_width - 2)), bits(-1)),
        ("input_subnormal", minimum_normal, 1, 0),
        ("negative_input_subnormal", minimum_normal, sign | 1, 0),
        ("output_subnormal", minimum_normal, bits(0.5), 0),
        ("positive_overflow", maximum, bits(2), 0),
        ("negative_overflow", sign | maximum, bits(2), 0),
        ("quiet_nan", quiet_nan, bits(1), bits(1)),
        ("signaling_nan", signaling_nan, bits(1), bits(1)),
        ("quiet_nan_right", bits(1), quiet_nan, bits(1)),
        ("signaling_nan_right", bits(1), signaling_nan, bits(1)),
        ("both_nan", quiet_nan, signaling_nan, bits(1)),
        ("zero_division", 0, 0, 0),
        ("divide_by_zero", bits(1), 0, 0),
        ("cancellation", bits(1), bits(-1), 0),
    ]


def arithmetic_fixtures(wide):
    sign = 1 << (63 if wide else 31)
    minimum = 0x0010000000000000 if wide else 0x00800000
    below_one = 0x3FEFFFFFFFFFFFFF if wide else 0x3F7FFFFF
    infinity = 0x7FF0000000000000 if wide else 0x7F800000
    quiet = 0x7FF8123456789ABC if wide else 0x7FC12345
    second_quiet = 0x7FF856789ABCDEF0 if wide else 0x7FC56789
    signaling = 0x7FF0123456789ABC if wide else 0x7FA12345
    one = floating_bits(1, wide)
    cases = base_fixtures(wide) + [
        ("before_rounding_tiny", minimum, below_one, 0),
        ("negative_before_rounding_tiny", sign | minimum, below_one, 0),
        ("opposite_zero", 0, sign, 0),
        ("negative_zeros", sign, sign, sign),
        ("infinity_minus_infinity", infinity, infinity, infinity),
        ("opposite_infinities", infinity, sign | infinity, infinity),
        ("quiet_accumulator", one, one, quiet),
        ("signaling_accumulator", one, one, signaling),
        ("quiet_accumulator_signaling_product", signaling, one, quiet),
        ("both_quiet", quiet, second_quiet, second_quiet),
        ("tiny_accumulator", one, one, 1),
        ("maximum_subnormal_input", minimum - 1, one, 0),
        ("negative_signaling_nan", sign | signaling, one, 0),
        ("negative_quiet_nan", sign | quiet, one, 0),
        ("all_negative_quiet", sign | quiet, sign | second_quiet, sign | second_quiet),
        ("all_signaling", signaling, sign | signaling, signaling),
    ]
    generator = random.Random(32064 if wide else 32032)
    for index in range(160):
        cases.append(("seeded_bits_" + str(index),
                      *(generator.getrandbits(64 if wide else 32) for _ in range(3))))
    return cases


def assign_register(words, name, value):
    index = int(name[1:])
    if name[0] == "d":
        words[2 * index] = value & 0xFFFFFFFF
        words[2 * index + 1] = value >> 32
    else:
        words[index] = value


def compare_instruction(execution, entry, operands, values, initial_fpscr):
    floating_words = [0xA5A50000 | index for index in range(32)]
    left, right, accumulator = values
    assign_register(floating_words, operands[0], accumulator)
    if len(operands) == 3:
        assign_register(floating_words, operands[1], left)
        assign_register(floating_words, operands[2], right)
    else:
        assign_register(floating_words, operands[1], right)
    registers = [0x12340000 | index for index in range(16)]
    registers[13:16] = [0x10000000, 0xFFFF0000, entry]
    flags = 0xA8000000
    context = Context()
    context.r[:] = registers
    for name, bit in (("n", 31), ("z", 30), ("c", 29), ("v", 28), ("q", 27)):
        setattr(context, name, (flags >> bit) & 1)
    native_words = (ctypes.c_uint32 * 32)(*floating_words)
    status = ctypes.c_uint32(initial_fpscr)
    context.budget = 1
    context.tls = 0x1FF82000
    context.vfp, context.fpscr = native_words, ctypes.pointer(status)
    context.read_pages, context.write_pages = execution.read_pages, execution.write_pages
    context.host = ctypes.pointer(execution.host)
    execution.errors.clear()
    execution.last_svc = None
    execution.original_instructions = 0

    reference = execution.reference
    reference.reg_write(arm.UC_ARM_REG_CPSR, 0x10 | flags)
    for register, value in zip(REGISTERS, registers):
        reference.reg_write(register, value)
    for index, value in enumerate(floating_words):
        reference.reg_write(getattr(arm, f"UC_ARM_REG_S{index}"), value)
    reference.reg_write(arm.UC_ARM_REG_FPSCR, initial_fpscr)
    reference.emu_start(entry, entry + 4, count=1)
    CODE(execution.lookup[entry])(ctypes.byref(context))
    if execution.errors:
        raise RuntimeError(f"native host callback used at 0x{entry:08X}: {execution.errors[0]}")
    if execution.last_svc is not None or execution.original_instructions != 1 or context.budget != 0:
        raise RuntimeError(f"instruction boundary disagrees at 0x{entry:08X}")
    expected_words = [reference.reg_read(getattr(arm, f"UC_ARM_REG_S{index}")) for index in range(32)]
    expected_status = reference.reg_read(arm.UC_ARM_REG_FPSCR)
    expected_registers = [reference.reg_read(register) for register in REGISTERS]
    actual_flags = sum(getattr(context, name) << bit for name, bit in
                       (("n", 31), ("z", 30), ("c", 29), ("v", 28), ("q", 27)))
    equal = (list(native_words) == expected_words and status.value == expected_status
             and list(context.r) == expected_registers
             and actual_flags == reference.reg_read(arm.UC_ARM_REG_CPSR) & 0xF8000000)
    if equal:
        return None
    return {"vfp_register_differences": [
                {"register": f"s{index}", "expected": expected, "actual": actual}
                for index, (expected, actual) in enumerate(zip(expected_words, native_words)) if expected != actual],
            "expected_fpscr": expected_status, "actual_fpscr": status.value,
            "expected_registers": expected_registers, "actual_registers": list(context.r),
            "expected_flags": reference.reg_read(arm.UC_ARM_REG_CPSR) & 0xF8000000,
            "actual_flags": actual_flags}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("directory", type=Path, help="ignored native build directory with sealed manifest")
    args = parser.parse_args()
    directory = args.directory.resolve()
    if not directory.is_relative_to(ROOT / "build"):
        raise RuntimeError("native verification reports must remain under ignored build")
    execution = Execution(directory)
    callback = ctypes.c_void_p.in_dll(execution.library, "native_block_timing_callback")
    if callback.value is not None:
        raise RuntimeError("standalone instruction checks require an unset timing callback")
    if execution.manifest.get("floating_point_operations_rewritten", 0) == 0:
        raise RuntimeError("native build does not record floating-point generation rewrites")
    for relative, expected in execution.manifest["sources"].items():
        if hashlib.sha256((ROOT / relative).read_bytes()).hexdigest() != expected:
            raise RuntimeError(f"sealed native source changed: {relative}")
    disassembler = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_ARM)
    for mnemonic, entry, operands in SAMPLES:
        if entry not in execution.lookup:
            raise RuntimeError(f"missing one-instruction native entry 0x{entry:08X}")
        instruction = next(disassembler.disasm(execution.code[entry - 0x100000:entry - 0x100000 + 4], entry))
        if instruction.mnemonic != mnemonic or tuple(instruction.op_str.split(", ")) != operands:
            raise RuntimeError(f"original sample no longer decodes as {mnemonic} at 0x{entry:08X}")
    counts = {"arithmetic": 0, "raw_bit_operations": 0}
    mismatches = []
    for mnemonic, entry, operands in SAMPLES:
        kind = "raw_bit_operations" if mnemonic.split(".")[0] in ("vmov", "vabs", "vneg") else "arithmetic"
        wide = mnemonic.endswith(".f64")
        cases = base_fixtures(wide) if kind == "raw_bit_operations" else arithmetic_fixtures(wide)
        for mode in range(4):
            for controls in (0, 0x01000000, 0x02000000, 0x03000000):
                for sticky in (0, 0x9F):
                    initial_fpscr = controls | mode << 22 | sticky
                    for fixture, left, right, accumulator in cases:
                        difference = compare_instruction(execution, entry, operands,
                                                         (left, right, accumulator), initial_fpscr)
                        counts[kind] += 1
                        if difference is not None:
                            mismatches.append({"mnemonic": mnemonic, "instruction_address": entry,
                                               "fixture": fixture, "initial_fpscr": initial_fpscr,
                                               "left": left, "right": right, "accumulator": accumulator,
                                               **difference})
    report = {"original_sha256": execution.manifest["original_sha256"],
              "library_sha256": execution.manifest["library_sha256"],
              "verifier_sha256": hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
              "unicorn_version": unicorn.__version__, "counts": counts,
              "sample_addresses": [entry for _, entry, _ in SAMPLES],
              "native_entry_budget": 1, "timing_callback": "unset",
              "compared_state": "all 32 single words, FPSCR, all integer registers including next PC, NZCVQ",
              "interpreter_instructions": 0, "gpu_frame_verified": False,
              "mismatch_count": len(mismatches), "mismatches": mismatches}
    (directory / "floating_point_execution_report.json").write_text(json.dumps(report, indent=2) + "\n")
    print(json.dumps({key: value for key, value in report.items() if key != "mismatches"}))
    if mismatches:
        raise RuntimeError(f"{len(mismatches)} generated native floating-point cases differ; see ignored report")


if __name__ == "__main__":
    main()
