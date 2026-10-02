#!/usr/bin/env python3
"""Check two bounded public Dynarmic probes against the owner's original ARM instructions."""
import argparse
import hashlib
import itertools
import json
from pathlib import Path
import struct
import subprocess

import unicorn
from unicorn import arm_const as arm

ROOT = Path(__file__).resolve().parents[2]
ORIGINAL_DIGEST = "e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64"
INSTRUCTION_ADDRESSES = (0x00105208, 0x0038CFF0, 0x0038CFF4)


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def original_execution(code, initial_mode, requested_mode, dimension):
    processor = unicorn.Uc(unicorn.UC_ARCH_ARM, unicorn.UC_MODE_ARM)
    processor.ctl_set_cpu_model(arm.UC_CPU_ARM_11MPCORE)
    processor.mem_map(0x1000, 0x1000)
    # Locally read register-only instructions. No original instruction bytes are embedded here.
    processor.mem_write(0x1000, b"".join(code[address - 0x100000:address - 0x100000 + 4]
                                       for address in INSTRUCTION_ADDRESSES))
    processor.reg_write(arm.UC_ARM_REG_CPSR, 0x10)
    processor.reg_write(arm.UC_ARM_REG_FPEXC, 1 << 30)
    processor.reg_write(arm.UC_ARM_REG_FPSCR, 0x03000000 | initial_mode << 22)
    processor.reg_write(arm.UC_ARM_REG_R0, 0x03000000 | requested_mode << 22)
    processor.reg_write(arm.UC_ARM_REG_S1, int.from_bytes(struct.pack("<f", dimension), "little"))
    processor.reg_write(arm.UC_ARM_REG_S17, int.from_bytes(struct.pack("<f", 2.0), "little"))
    processor.emu_start(0x1000, 0x100C, count=3)
    return processor.reg_read(arm.UC_ARM_REG_R0), processor.reg_read(arm.UC_ARM_REG_FPSCR)


def compare(executable, code_path, code):
    raw = subprocess.check_output([str(executable), str(code_path)], text=True)
    rows = [json.loads(line) for line in raw.splitlines()]
    expected = set(itertools.product(range(4), range(4), (240, 320, 400), (False, True)))
    observed = {(row["initial_mode"], row["requested_mode"], row["dimension"], row["split_run"])
                for row in rows}
    if len(rows) != len(expected) or observed != expected:
        raise RuntimeError("probe case matrix differs from the reviewed 96 cases")
    result_mismatches = status_mismatches = host_mismatches = fallback_count = 0
    continuous_mismatches = split_mismatches = 0
    for row in rows:
        result, fpscr = original_execution(code, row["initial_mode"], row["requested_mode"], row["dimension"])
        result_equal = int(row["result"], 16) == result
        status_equal = int(row["fpscr"], 16) == fpscr
        host_equal = int(row["host_fpcr_at_svc"], 16) == 0x03000000 | row["requested_mode"] << 22
        result_mismatches += not result_equal
        status_mismatches += not status_equal
        host_mismatches += not host_equal
        fallback_count += row["fallbacks"]
        continuous_mismatches += not result_equal and not row["split_run"]
        split_mismatches += not result_equal and row["split_run"]
        row.update({"original_result": f"0x{result:08X}", "original_fpscr": f"0x{fpscr:08X}",
                    "result_equal": result_equal, "status_equal": status_equal,
                    "host_control_matches_requested": host_equal})
    return {"executable": str(executable), "executable_sha256": digest(executable),
            "case_count": len(rows), "result_mismatches": result_mismatches,
            "status_mismatches": status_mismatches, "host_control_mismatches": host_mismatches,
            "continuous_result_mismatches": continuous_mismatches,
            "split_result_mismatches": split_mismatches, "fallback_count": fallback_count, "cases": rows}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("original_executable", type=Path)
    parser.add_argument("corrected_executable", type=Path)
    parser.add_argument("--report", type=Path, required=True)
    args = parser.parse_args()
    report = args.report.resolve()
    if not report.is_relative_to(ROOT / "build") or report.exists():
        raise RuntimeError("report must be an absent path in this checkout's ignored build")
    code_path = ROOT / "data/ver/eu/code.bin"
    code = code_path.read_bytes()
    if hashlib.sha256(code).hexdigest() != ORIGINAL_DIGEST:
        raise RuntimeError("original executable differs from the approved EU oracle")
    original = compare(args.original_executable.resolve(), code_path, code)
    corrected = compare(args.corrected_executable.resolve(), code_path, code)
    passed = (original["continuous_result_mismatches"] == 22 and original["split_result_mismatches"] == 0
              and original["status_mismatches"] == 0 and original["fallback_count"] == 0
              and all(corrected[key] == 0 for key in ("result_mismatches", "status_mismatches",
                                                     "host_control_mismatches", "fallback_count")))
    result = {"original_code_sha256": ORIGINAL_DIGEST, "unicorn_version": unicorn.__version__,
              "probe_source_sha256": digest(ROOT / "tools/static_recompiler/verify_azahar_floating_point_control.cpp"),
              "verifier_sha256": digest(Path(__file__).resolve()), "original": original,
              "corrected": corrected, "passed": passed,
              "scope": "96 bounded rounding transitions, macOS arm64 host, original ARM11MPCore instructions"}
    report.parent.mkdir(parents=True, exist_ok=True)
    report.write_text(json.dumps(result, indent=2) + "\n")
    print(json.dumps({"passed": passed,
                      "original": {k: v for k, v in original.items() if k != "cases"},
                      "corrected": {k: v for k, v in corrected.items() if k != "cases"}}, indent=2))
    raise SystemExit(0 if passed else 1)


if __name__ == "__main__":
    main()
