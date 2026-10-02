#!/usr/bin/env python3
"""Compare native translated execution with the unchanged original ARM code."""
import argparse
import ctypes as c
import csv
import hashlib
import json
from pathlib import Path
import random
import struct

import capstone
import unicorn
from unicorn import arm_const as arm

ROOT = Path(__file__).resolve().parents[2]
REGISTERS = [getattr(arm, f"UC_ARM_REG_R{i}") for i in range(13)] + [arm.UC_ARM_REG_SP, arm.UC_ARM_REG_LR, arm.UC_ARM_REG_PC]
RETURN_ADDRESS = 0xFFFF0000


class Context(c.Structure):
    pass


READ8 = c.CFUNCTYPE(c.c_uint8, c.POINTER(Context), c.c_uint32)
READ16 = c.CFUNCTYPE(c.c_uint16, c.POINTER(Context), c.c_uint32)
READ32 = c.CFUNCTYPE(c.c_uint32, c.POINTER(Context), c.c_uint32)
WRITE8 = c.CFUNCTYPE(None, c.POINTER(Context), c.c_uint32, c.c_uint8)
WRITE16 = c.CFUNCTYPE(None, c.POINTER(Context), c.c_uint32, c.c_uint16)
WRITE32 = c.CFUNCTYPE(None, c.POINTER(Context), c.c_uint32, c.c_uint32)
INTERPRET = c.CFUNCTYPE(None, c.POINTER(Context), c.c_uint32, c.c_uint32)
LOOKUP = c.CFUNCTYPE(c.c_void_p, c.POINTER(Context), c.c_uint32)
CODE = c.CFUNCTYPE(None, c.POINTER(Context))


class Host(c.Structure):
    _fields_ = [("read8", READ8), ("read16", READ16), ("read32", READ32),
                ("write8", WRITE8), ("write16", WRITE16), ("write32", WRITE32),
                ("interpret", INTERPRET), ("lookup", LOOKUP)]


Context._fields_ = [("r", c.c_uint32 * 16)] + [(name, c.c_uint8) for name in
    ("n", "z", "c", "v", "q", "thumb", "ge", "exclusive")] + [
    ("budget", c.c_int32), ("exit", c.c_uint32), ("svc", c.c_uint32),
    ("depth", c.c_uint32), ("exclusive_address", c.c_uint32), ("tls", c.c_uint32),
    ("read_pages", c.POINTER(c.c_void_p)), ("write_pages", c.POINTER(c.c_void_p)),
    ("vfp", c.POINTER(c.c_uint32)), ("fpscr", c.POINTER(c.c_uint32)),
    ("host", c.POINTER(Host)), ("user", c.c_void_p)]


class Entry(c.Structure):
    _fields_ = [("address", c.c_uint32), ("code", c.c_void_p)]


class Execution:
    def __init__(self, directory):
        self.directory = directory
        self.manifest = json.loads((directory / "build_manifest.json").read_text())
        library = Path(self.manifest["library"])
        if hashlib.sha256(library.read_bytes()).hexdigest() != self.manifest["library_sha256"]:
            raise RuntimeError("linked library changed after build")
        self.library = c.CDLL(str(library))
        if c.c_uint32.in_dll(self.library, "recomp_abi").value != 4:
            raise RuntimeError("unsupported native ABI")
        count = c.c_uint32.in_dll(self.library, "recomp_entry_count").value
        self.entries = (Entry * count).in_dll(self.library, "recomp_entries")
        self.lookup = {e.address: e.code for e in self.entries}
        if list(self.lookup) != sorted(self.lookup) or len(self.lookup) != count:
            raise RuntimeError("bad linked entry table")
        self.code = (ROOT / "data/ver/eu/code.bin").read_bytes()
        if hashlib.sha256(self.code).hexdigest() != self.manifest["original_sha256"]:
            raise RuntimeError("original binary changed after build")
        header = (ROOT / "data/ver/eu/exh.bin").read_bytes()
        if hashlib.sha256(header).hexdigest() != self.manifest["header_sha256"]:
            raise RuntimeError("executable header changed after build")
        self.regions = []
        offset = 0
        for at in (0x10, 0x20, 0x30):
            base, pages, size = struct.unpack_from("<III", header, at)
            extent = max(pages * 4096, size + (struct.unpack_from("<I", header, 0x3C)[0] if at == 0x30 else 0))
            initial = self.code[offset:offset + size].ljust((extent + 4095) & ~4095, b"\0")
            self.regions.append((base, initial, at == 0x30))
            offset += pages * 4096
        self.regions.extend([(0x0FF00000, bytes(0x100000), True), (0x1FF82000, bytes(4096), True)])
        self.arrays = [(base, (c.c_uint8 * len(initial)).from_buffer_copy(initial), writable)
                       for base, initial, writable in self.regions]
        self.read_pages, self.write_pages = (c.c_void_p * (1 << 20))(), (c.c_void_p * (1 << 20))()
        for base, array, writable in self.arrays:
            for offset in range(0, len(array), 4096):
                self.read_pages[(base + offset) >> 12] = c.addressof(array) + offset
                if writable:
                    self.write_pages[(base + offset) >> 12] = c.addressof(array) + offset
        self.errors = []

        def access(address, size, write=False):
            for base, array, writable in self.arrays:
                if base <= address and address + size <= base + len(array) and (not write or writable):
                    return array, address - base
            raise RuntimeError(f"invalid native memory access 0x{address:08x}")

        def read(size):
            def callback(context, address):
                try:
                    array, offset = access(address, size)
                    return int.from_bytes(array[offset:offset + size], "little")
                except Exception as error:
                    self.errors.append(str(error)); context.contents.exit = 3
                    return 0
            return callback

        def write(size):
            def callback(context, address, value):
                try:
                    array, offset = access(address, size, True)
                    array[offset:offset + size] = value.to_bytes(size, "little")
                except Exception as error:
                    self.errors.append(str(error)); context.contents.exit = 3
            return callback

        def interpret(context, address, opcode):
            self.errors.append(f"interpreter refused 0x{address:08x} opcode {opcode:08x}")
            context.contents.exit = 3

        self.host = Host(READ8(read(1)), READ16(read(2)), READ32(read(4)),
                         WRITE8(write(1)), WRITE16(write(2)), WRITE32(write(4)),
                         INTERPRET(interpret), LOOKUP(lambda context, address: self.lookup.get(address)))
        self.reference = unicorn.Uc(unicorn.UC_ARCH_ARM, unicorn.UC_MODE_ARM)
        self.reference.ctl_set_cpu_model(arm.UC_CPU_ARM_11MPCORE)
        for base, initial, writable in self.regions:
            self.reference.mem_map(base, len(initial))
            self.reference.mem_write(base, initial)
        self.reference.reg_write(arm.UC_ARM_REG_C13_C0_3, 0x1FF82000)
        self.reference.reg_write(arm.UC_ARM_REG_FPEXC, 0x40000000)
        self.last_svc = None
        self.original_instructions = 0

        def instruction(reference, address, size, user):
            self.original_instructions += 1
        self.reference.hook_add(unicorn.UC_HOOK_CODE, instruction)

        def interrupt(reference, number, user):
            pc = reference.reg_read(arm.UC_ARM_REG_PC)
            opcode = int.from_bytes(reference.mem_read(pc - 4, 4), "little")
            if opcode & 0x0F000000 != 0x0F000000:
                raise RuntimeError(f"original exception {number} at {pc:08x}")
            self.last_svc = opcode & 0xFFFFFF
            reference.emu_stop()
        self.reference.hook_add(unicorn.UC_HOOK_INTR, interrupt)

    def reset_memory(self):
        for (base, initial, writable), (_, array, _) in zip(self.regions, self.arrays):
            if writable:
                c.memmove(array, initial, len(initial))
                self.reference.mem_write(base, initial)

    def compare(self, entry, registers, flags=0, *, replacement=False):
        self.errors.clear()
        self.last_svc = None
        self.original_instructions = 0
        vfp, fpscr = (c.c_uint32 * 32)(), c.c_uint32(0)
        context = Context()
        context.r[:] = registers
        for name, bit in (("n", 31), ("z", 30), ("c", 29), ("v", 28), ("q", 27)):
            setattr(context, name, (flags >> bit) & 1)
        context.budget = 1000000
        context.tls = 0x1FF82000
        context.vfp, context.fpscr = vfp, c.pointer(fpscr)
        context.read_pages, context.write_pages, context.host = self.read_pages, self.write_pages, c.pointer(self.host)
        for register, value in zip(REGISTERS, registers):
            self.reference.reg_write(register, value)
        self.reference.reg_write(arm.UC_ARM_REG_CPSR, 0x10 | flags)
        # CPSR mode changes banked registers. Set SP/LR again after selecting user mode.
        self.reference.reg_write(arm.UC_ARM_REG_SP, registers[13])
        self.reference.reg_write(arm.UC_ARM_REG_LR, registers[14])
        for i in range(32):
            self.reference.reg_write(getattr(arm, f"UC_ARM_REG_S{i}"), 0)
        self.reference.reg_write(arm.UC_ARM_REG_FPSCR, 0)
        self.reference.emu_start(entry, RETURN_ADDRESS, count=1000000)
        while context.budget > 0 and context.r[15] != RETURN_ADDRESS:
            pointer = self.lookup.get(context.r[15] | context.thumb)
            if not pointer:
                raise RuntimeError(f"missing translated entry {context.r[15]:08x}")
            context.exit, context.depth = 0, 0
            previous = (context.r[15], context.budget)
            CODE(pointer)(c.byref(context))
            if self.errors:
                raise RuntimeError(self.errors[0])
            if context.exit in (1, 2):
                break
            if previous == (context.r[15], context.budget):
                raise RuntimeError("native execution made no progress")
        expected = [self.reference.reg_read(register) for register in REGISTERS]
        actual = list(context.r)
        compared = [0, *range(4, 14), 15] if replacement else list(range(16))
        if any(actual[i] != expected[i] for i in compared):
            raise RuntimeError(f"register divergence at {entry:08x}: native {actual}, original {expected}")
        native_flags = sum(getattr(context, name) << bit for name, bit in (("n", 31), ("z", 30), ("c", 29), ("v", 28), ("q", 27)))
        if not replacement and native_flags != self.reference.reg_read(arm.UC_ARM_REG_CPSR) & 0xF8000000:
            raise RuntimeError(f"flag divergence at {entry:08x}")
        if not replacement and context.ge != (self.reference.reg_read(arm.UC_ARM_REG_CPSR) >> 16) & 15:
            raise RuntimeError(f"GE flag divergence at {entry:08x}")
        if (context.exit == 1) != (self.last_svc is not None) or (self.last_svc is not None and context.svc != self.last_svc):
            raise RuntimeError("supervisor-call boundary divergence")
        return context


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("directory", type=Path)
    args = parser.parse_args()
    directory = args.directory.resolve()
    execution = Execution(directory)
    generator = random.Random(0x53F00)
    registers = [0] * 16
    registers[13:16] = [0x10000000, RETURN_ADDRESS, 0x100000]
    context = execution.compare(0x100000, registers)
    memory_checks = 0
    for base, array, writable in execution.arrays:
        if writable:
            if bytes(array) != bytes(execution.reference.mem_read(base, len(array))):
                raise RuntimeError(f"startup memory divergence at {base:08x}")
            memory_checks += len(array)
    startup = {"native_budget_units": 1000000 - context.budget,
               "original_instructions": execution.original_instructions,
               "budget_units_equal_original_instructions": 1000000 - context.budget == execution.original_instructions,
               "svc": context.svc,
               "next_pc": context.r[15], "writable_bytes_compared": memory_checks}
    execution.reset_memory()
    priority_cases = [0, 23, 24, 31, 32, 63, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFF]
    priority_cases += [generator.getrandbits(32) for _ in range(4096)]
    for value in priority_cases:
        registers = [generator.getrandbits(32) for _ in range(16)]
        registers[0] = value
        registers[13:16] = [0x10000000, RETURN_ADDRESS, 0x10766C]
        execution.compare(0x10766C, registers, generator.getrandbits(5) << 27, replacement=True)
    # Sample bounded retail leaf functions across the executable. They may read literal
    # pools, but cannot require class layouts, pointer arguments, services, or other functions.
    decoder = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_ARM)
    decoder.detail = True
    candidates = []
    with (directory / "function_map.csv").open() as source:
        rows = [{k.strip(): v.strip() for k, v in row.items()} for row in csv.DictReader(source)]
    for row in rows:
        if "f" not in row["Type"]:
            continue
        start, end = int(row["Start"], 16), int(row["Pool"] or row["End"], 16)
        if start == 0x10766C or not 4 <= end - start <= 48 or start not in execution.lookup:
            continue
        instructions = list(decoder.disasm(execution.code[start - 0x100000:end - 0x100000], start))
        if len(instructions) * 4 != end - start or instructions[-1].id != capstone.arm.ARM_INS_BX:
            continue
        permitted = True
        for instruction in instructions:
            if instruction.mnemonic.startswith(("v", "str", "stm", "ldm", "push", "pop", "mcr", "mrc", "svc", "ldrex", "strex", "swp", "bkpt")):
                permitted = False
            if instruction.id in (capstone.arm.ARM_INS_BL, capstone.arm.ARM_INS_BLX):
                permitted = False
            if instruction.id == capstone.arm.ARM_INS_BX and instruction.op_str != "lr":
                permitted = False
            if instruction.id == capstone.arm.ARM_INS_B:
                target = instruction.operands[0].imm
                if not instruction.address < target < end:
                    permitted = False
            for operand in instruction.operands:
                if operand.type == capstone.arm.ARM_OP_MEM and operand.mem.base != capstone.arm.ARM_REG_PC:
                    permitted = False
        if permitted:
            candidates.append(start)
    sampled = sorted(generator.sample(candidates, min(64, len(candidates))))
    leaf_cases = 0
    for entry in sampled:
        for _ in range(32):
            registers = [generator.getrandbits(32) for _ in range(16)]
            registers[13:16] = [0x10000000, RETURN_ADDRESS, entry]
            execution.compare(entry, registers, generator.getrandbits(5) << 27)
            leaf_cases += 1
    report = {"original_sha256": execution.manifest["original_sha256"],
              "library_sha256": execution.manifest["library_sha256"], "unicorn_version": unicorn.__version__,
              "startup": startup, "priority_replacement_cases": len(priority_cases),
              "bounded_integer_leaf_addresses": sampled, "bounded_integer_leaf_cases": leaf_cases,
              "replacement_contract": "AAPCS return r0, callee-saved r4-r11, SP and return PC; caller-saved registers and flags excluded",
              "interpreter_instructions": 0, "gpu_frame_verified": False}
    (directory / "execution_report.json").write_text(json.dumps(report, indent=2) + "\n")
    print(json.dumps(report))


if __name__ == "__main__":
    main()
