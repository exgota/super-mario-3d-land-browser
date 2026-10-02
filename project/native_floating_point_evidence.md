# Native floating-point arithmetic checkpoint

Root owns `runtime/port/NativeFloatingPoint.h` and `.c`. These are new source-only runtime helpers. The helper lane changed no matching source, oracle source, ranks, ledger, parent CPU/timing files or STATE. No game instruction payload, generated game C, captures or user state is included.

## Observed correction

The generated arithmetic used the host rounding mode, did not flush input subnormals, retained NaN payloads under DN, and did not update guest exception flags. At the initial thread FPSCR `0x03C00010`, original ARM instructions independently executed by Unicorn 2.1.4 ARM11MPCore give these discriminating cases:

| Original instruction | Generic input | Original result | Unchanged generated result | Original new flag |
|---|---|---|---|---|
| VADD.F32 at `0x00109FEC` | 1 + 0.75 ulp | `0x3F800000` | `0x3F800001` | IXC already set |
| VMUL.F32 at `0x00109DF8` | largest finite * 2 | `0x7F7FFFFF` | infinity | OFC |
| VADD.F32 at `0x00109FEC` | smallest normal + smallest subnormal | `0x00800000` | `0x00800001` | IDC |
| VADD.F32 at `0x00109FEC` | quiet NaN + 1 | default NaN | input NaN payload | none |
| VMUL.F32 at `0x00109DF8` | smallest normal * 0.5 | zero | zero | UFC |

The unchanged generated header SHA256 was `5b6dcd3d07931c4cf23214e785ff4c1c2431779773c69cbc1589fc3a7206a6d6`. Its bounded baseline comparison had 1,792 cases, 579 result mismatches and 696 FPSCR mismatches. The corrected helper passed the same 1,792 cases.

A broader final check passed 86,016 arithmetic cases with zero result or FPSCR mismatches. It uses 14 original instruction forms, four rounding modes, DN/FZ independently off/on, initially empty or prepopulated sticky flags, 32 selected operand triples and 160 deterministic random triples per width. Cases include signaling/quiet NaN priority, signed zero, before-rounding tiny results, exact and inexact underflow, overflow, divide by zero, invalid operations and non-fused multiply-accumulate stages. It also passed 2,048 original VMOV/VABS/VNEG bit/status cases. A separate host-environment check preserved host rounding and preexisting FE_INVALID/FE_INEXACT in all 16 host/guest rounding pairs. Raw bit transforms preserved signaling-NaN bits and FPSCR with enabled trap bits.

The tested C SHA256 is `53f9af37f8552c71623f919ff64205635596cd8600f480aeee1924f2ea340239`; the header SHA256 is `633d3ef3cdd1e5355eaa1136a3dc35429fe656f4711cabc2dafac7a24eeb1924`. Build: clang 21 on macOS arm64, `-O2 -frounding-math -ffp-contract=off -fno-fast-math -fno-math-errno`, with warnings treated as errors. Local experimental reports remain in ignored `build/root_port_floating_point/`.

## Semantics and integration

The source uses raw IEEE bits and an explicit guest FPSCR pointer. It selects rounding, prepares arithmetic input subnormals, selects NaNs before host arithmetic can quiet them, rounds non-fused multiply and add separately, merges cumulative flags, and restores the host floating-point environment. Enabled arithmetic exception traps are an explicit refusal. Move, absolute and negate operate on bits and leave FPSCR unchanged, including signaling-NaN payloads and subnormals.

The [pinned Azahar thread initializer](https://github.com/azahar-emu/azahar/blob/662d412123305a9f4be94dd3dc73ddf91a18c55e/src/core/hle/kernel/thread.cpp#L501) supplies DN, FZ, toward-zero and IXC initially. Its pinned Dynarmic revision is `e77b1ba0b7da7cbe93021b01a663acfe7c4dd516`. The public [VFP translator](https://github.com/azahar-emu/dynarmic/blob/e77b1ba0b7da7cbe93021b01a663acfe7c4dd516/src/dynarmic/frontend/A32/translate/impl/vfp.cpp#L145) represents multiply-accumulate as separate multiply/add operations, and [NaN processing](https://github.com/azahar-emu/dynarmic/blob/e77b1ba0b7da7cbe93021b01a663acfe7c4dd516/src/dynarmic/common/fp/process_nan.cpp) gives signaling operands priority. These public sources were inspected for semantics; no emulator implementation was copied into the new helper. The game operands and original instruction bytes stayed local.

The parent owns generation-time integration. `codegen/vfp.rs` in the ignored pinned generator (`83e6920784baff6ebc5888ed5cfdbf9735dfbf1c`) currently emits scalar arithmetic expressions directly, so changing `vfp_apply_s/d` alone is insufficient. Replace each scalar arithmetic body with the corresponding compile-time operation selector and raw register reads/writes:

```c
ctx->vfp[destination] = NativeFloatingPointApplySingle(
    NativeFloatingPointAdd, ctx->vfp[left], ctx->vfp[right],
    ctx->vfp[destination], ctx->fpscr);
```

For doubles, assemble each raw operand from its two single words, evaluate to a local `uint64_t`, then store low/high words. For square root, pass source bits as `right`, with zero `left` and `accumulator`. The short-vector loop can call the same arithmetic helper per lane, retaining register sequencing. Its bit operations must also preserve raw bits. Do not prepare raw loads/stores/register moves, and do not globally flush `vfp_s/d` reads. No guest instruction decoder or interpreter fallback is used by these helpers.

## Limits

This checks original instructions with independent Unicorn execution, not a completed native/Azahar frame replay. It establishes a bounded arithmetic checkpoint, not full VFP parity. Conversions, VCMP versus VCMPE exception behavior, short-vector register sequencing, enabled exception delivery, all reachable game operands, and other native/WebAssembly hosts remain open. Double multiply-accumulate, double negated multiply and double square root share the implementation but were not present in the 14 sampled original arithmetic forms. Original double VABS/VNEG forms were also not found in the selected scan. Cross-platform before-rounding tininess and host fenv behavior still need confirmation. No port milestone, rendering or gameplay credit follows from this checkpoint.

## Actual generated instruction integration

The source-only `tools/static_recompiler/verify_floating_point_execution.py` checks generated native entry pointers, not the arithmetic helper API. It starts each native context at the sampled original instruction address with budget 1 and an unset timing callback. Independent Unicorn executes the unchanged original with `count=1`. The comparison includes all 32 single-register words, FPSCR, all integer registers including next PC, and NZCVQ. It also checks the linked library seal, generated source seals and original sample decoding.

The parent integrated scalar arithmetic/square-root and short-vector arithmetic rewrites in `build/floating_point_timed_native`. Its linked library SHA256 `e0f53fa6edd3bb52a5f0a60f39f2d18b7efa6b2655c6d181782ae5a64dcc4fe8` passes the same 86,016 scalar arithmetic and 2,048 raw bit-operation cases, with zero state differences and zero interpreter instructions. The verifier SHA256 is `d5eac9c5b8adae13cd04f4a6b06488b501f71a5d81b5bcac72b7ef58f69e224e`. Its receipt is the ignored `build/floating_point_timed_native/floating_point_execution_report.json`.

Run from the checkout with the final native build and project Python environment:

```sh
python tools/static_recompiler/verify_floating_point_execution.py build/floating_point_timed_native
```

This receipt tests scalar FPSCR length/stride fields at zero. A separate ignored probe tried lengths 2/3/4 with stride 1 and length 2 with stride 2 across the same 18 sampled forms and 16 base fixtures, 1,152 cases. It found 27 differences, all single-precision stride field `0b11` for VMLS or VNMLA. For length 2 VMLS at `0x0010C3B0`, Unicorn writes the second result to s14 while the native/Dynarmic stride maps it to s12. All sampled stride-1 and double stride-2 cases agree. This remains an oracle disagreement requiring Azahar or hardware confirmation, not grounds to change the native helper.

The pinned [Dynarmic FPSCR definition](https://github.com/azahar-emu/dynarmic/blob/e77b1ba0b7da7cbe93021b01a663acfe7c4dd516/src/dynarmic/frontend/A32/FPSCR.h#L81) maps stride field `0b11` to 2. [Unicorn 2.1.4 stores that field unchanged](https://github.com/unicorn-engine/unicorn/blob/2.1.4/qemu/target/arm/vfp_helper.c#L183), and its [single-precision vector translator](https://github.com/unicorn-engine/unicorn/blob/2.1.4/qemu/target/arm/translate-vfp.inc.c#L1185) increments by the raw field plus 1. The double translator uses the raw field shifted right before adding 1. The experimental report remains in `build/root_port_floating_point/generated_short_vector_comparison.json` outside the submitted source tree.

## Rerun from source

The owner's approved `data/ver/eu/code.bin` must be present locally. The following reads instruction bytes from that hashed file at the listed addresses; it embeds no original opcode payload in the public source. It requires the project Python environment with Capstone and Unicorn 2.1.4 installed. On macOS run from the checkout containing the helper:

```sh
. ./development_environment.sh
mkdir -p build/native_floating_point_verification
clang -std=c11 -O2 -frounding-math -ffp-contract=off -fno-fast-math -fno-math-errno -Wall -Wextra -Werror -shared -fPIC runtime/port/NativeFloatingPoint.c -o build/native_floating_point_verification/native_floating_point.dylib
python - <<'PY'
from pathlib import Path
import ctypes, hashlib, json, random, struct
import unicorn
from unicorn import arm_const as arm
from capstone import Cs, CS_ARCH_ARM, CS_MODE_ARM

binary_path = Path('data/ver/eu/code.bin')
binary = binary_path.read_bytes()
original_hash = hashlib.sha256(binary).hexdigest()
assert original_hash == 'e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
samples = [
    ('vabs.f32', 10, 0x00215E2C, ('s0', 's0')),
    ('vadd.f32', 6, 0x00109FEC, ('s1', 's0', 's1')),
    ('vadd.f64', 6, 0x0023BAE0, ('d0', 'd0', 'd8')),
    ('vdiv.f32', 8, 0x00109E04, ('s1', 's1', 's2')),
    ('vdiv.f64', 8, 0x001F402C, ('d1', 'd0', 'd8')),
    ('vmla.f32', 0, 0x0010C524, ('s3', 's0', 's12')),
    ('vmls.f32', 1, 0x0010C3B0, ('s10', 's13', 's2')),
    ('vmov.f32', 9, 0x001002AC, ('s3', 's16')),
    ('vmov.f64', 9, 0x0012A1E8, ('d8', 'd0')),
    ('vmul.f32', 4, 0x00109DF8, ('s1', 's1', 's2')),
    ('vmul.f64', 4, 0x001F3FA4, ('d8', 'd1', 'd0')),
    ('vneg.f32', 11, 0x00109E68, ('s1', 's1')),
    ('vnmla.f32', 3, 0x0010EDA8, ('s12', 's8', 's5')),
    ('vnmls.f32', 2, 0x0011E5AC, ('s0', 's1', 's17')),
    ('vnmul.f32', 5, 0x0010C430, ('s1', 's17', 's15')),
    ('vsqrt.f32', 12, 0x0010ED24, ('s3', 's3')),
    ('vsub.f32', 7, 0x00109D8C, ('s0', 's0', 's1')),
    ('vsub.f64', 7, 0x0023BAB4, ('d1', 'd1', 'd2')),
]
library = ctypes.CDLL(str(Path('build/native_floating_point_verification/native_floating_point.dylib').resolve()))
functions = {}
for wide, suffix in ((False, 'Single'), (True, 'Double')):
    function = getattr(library, 'NativeFloatingPointApply' + suffix)
    integer = ctypes.c_uint64 if wide else ctypes.c_uint32
    function.argtypes = [ctypes.c_int, integer, integer, integer, ctypes.POINTER(ctypes.c_uint32)]
    function.restype = integer
    functions[wide] = function
reference = unicorn.Uc(unicorn.UC_ARCH_ARM, unicorn.UC_MODE_ARM)
reference.ctl_set_cpu_model(arm.UC_CPU_ARM_11MPCORE)
reference.mem_map(0x1000, 0x1000)
reference.reg_write(arm.UC_ARM_REG_FPEXC, 0x40000000)
disassembler = Cs(CS_ARCH_ARM, CS_MODE_ARM)

def floating_bits(value, wide):
    return int.from_bytes(struct.pack('<d' if wide else '<f', value), 'little')

def base_fixtures(wide):
    exponent = 52 if wide else 23
    sign = 1 << (63 if wide else 31)
    minimum_normal = 1 << exponent
    maximum = 0x7fefffffffffffff if wide else 0x7f7fffff
    quiet_nan = 0x7ff8123456789abc if wide else 0x7fc12345
    signaling_nan = 0x7ff0123456789abc if wide else 0x7fa12345
    b = lambda value: floating_bits(value, wide)
    return [('ordinary', b(2), b(3), b(1)),
            ('positive_rounding', b(1), b(3 * 2 ** (-exponent - 2)), b(1)),
            ('negative_rounding', b(-1), b(-3 * 2 ** (-exponent - 2)), b(-1)),
            ('input_subnormal', minimum_normal, 1, 0),
            ('negative_input_subnormal', minimum_normal, sign | 1, 0),
            ('output_subnormal', minimum_normal, b(0.5), 0),
            ('positive_overflow', maximum, b(2), 0),
            ('negative_overflow', sign | maximum, b(2), 0),
            ('quiet_nan', quiet_nan, b(1), b(1)),
            ('signaling_nan', signaling_nan, b(1), b(1)),
            ('quiet_nan_right', b(1), quiet_nan, b(1)),
            ('signaling_nan_right', b(1), signaling_nan, b(1)),
            ('both_nan', quiet_nan, signaling_nan, b(1)),
            ('zero_division', 0, 0, 0),
            ('divide_by_zero', b(1), 0, 0),
            ('cancellation', b(1), b(-1), 0)]

def fixtures(wide):
    sign = 1 << (63 if wide else 31)
    minimum = 0x0010000000000000 if wide else 0x00800000
    below_one = 0x3fefffffffffffff if wide else 0x3f7fffff
    infinity = 0x7ff0000000000000 if wide else 0x7f800000
    quiet = 0x7ff8123456789abc if wide else 0x7fc12345
    second_quiet = 0x7ff856789abcdef0 if wide else 0x7fc56789
    signaling = 0x7ff0123456789abc if wide else 0x7fa12345
    one = floating_bits(1, wide)
    cases = base_fixtures(wide) + [
        ('before_rounding_tiny', minimum, below_one, 0),
        ('negative_before_rounding_tiny', sign | minimum, below_one, 0),
        ('opposite_zero', 0, sign, 0),
        ('negative_zeros', sign, sign, sign),
        ('infinity_minus_infinity', infinity, infinity, infinity),
        ('opposite_infinities', infinity, sign | infinity, infinity),
        ('quiet_accumulator', one, one, quiet),
        ('signaling_accumulator', one, one, signaling),
        ('quiet_accumulator_signaling_product', signaling, one, quiet),
        ('both_quiet', quiet, second_quiet, second_quiet),
        ('tiny_accumulator', one, one, 1),
        ('maximum_subnormal_input', minimum - 1, one, 0),
        ('negative_signaling_nan', sign | signaling, one, 0),
        ('negative_quiet_nan', sign | quiet, one, 0),
        ('all_negative_quiet', sign | quiet, sign | second_quiet, sign | second_quiet),
        ('all_signaling', signaling, sign | signaling, signaling)]
    generator = random.Random(32064 if wide else 32032)
    width = 64 if wide else 32
    for index in range(160):
        cases.append(('seeded_bits_' + str(index), *(generator.getrandbits(width) for _ in range(3))))
    return cases

counts = {'arithmetic': 0, 'raw_bit_operations': 0}
mismatches = []
for mnemonic, operation, address, operands in samples:
    instruction = binary[address - 0x100000:address - 0x100000 + 4]
    decoded = next(disassembler.disasm(instruction, address))
    assert decoded.mnemonic == mnemonic and tuple(decoded.op_str.split(', ')) == operands
    wide = mnemonic.endswith('.f64')
    kind = 'raw_bit_operations' if operation in (9, 10, 11) else 'arithmetic'
    cases = base_fixtures(wide) if kind == 'raw_bit_operations' else fixtures(wide)
    for mode in range(4):
        for controls in (0, 0x01000000, 0x02000000, 0x03000000):
            for sticky in (0, 0x9f):
                fpscr = controls | mode << 22 | sticky
                for fixture, left, right, accumulator in cases:
                    reference.mem_write(0x1000, instruction)
                    reference.ctl_remove_cache(0x1000, 0x1004)
                    reference.reg_write(arm.UC_ARM_REG_FPSCR, fpscr)
                    register = lambda name: getattr(arm, 'UC_ARM_REG_' + name.upper())
                    reference.reg_write(register(operands[0]), accumulator)
                    if len(operands) == 3:
                        reference.reg_write(register(operands[1]), left)
                        reference.reg_write(register(operands[2]), right)
                    else:
                        reference.reg_write(register(operands[1]), right)
                    reference.emu_start(0x1000, 0x1004, count=1)
                    expected = reference.reg_read(register(operands[0]))
                    expected_status = reference.reg_read(arm.UC_ARM_REG_FPSCR)
                    status = ctypes.c_uint32(fpscr)
                    actual = functions[wide](operation, left, right, accumulator, ctypes.byref(status))
                    counts[kind] += 1
                    if (actual, status.value) != (expected, expected_status):
                        mismatches.append({'instruction_address': hex(address), 'mnemonic': mnemonic,
                            'fixture': fixture, 'initial_fpscr': hex(fpscr),
                            'expected': hex(expected), 'actual': hex(actual),
                            'expected_fpscr': hex(expected_status), 'actual_fpscr': hex(status.value)})
assert hashlib.sha256(binary_path.read_bytes()).hexdigest() == original_hash
report = {'unicorn_version': unicorn.__version__, 'counts': counts,
          'mismatch_count': len(mismatches), 'mismatches': mismatches}
Path('build/native_floating_point_verification/comparison.json').write_text(json.dumps(report, indent=2)+'\n')
print(json.dumps({key:value for key,value in report.items() if key != 'mismatches'}))
assert not mismatches
PY
```

Expected counts are 86,016 arithmetic and 2,048 raw bit operations, with zero mismatches. The comparison JSON remains under ignored `build/`. This rerun does not exercise a whole function or game frame.

The host-state check is also rerunnable without any game data:

```sh
clang -std=c11 -O2 -frounding-math -ffp-contract=off -fno-fast-math -fno-math-errno -Wall -Wextra -Werror -Iruntime/port -x c - runtime/port/NativeFloatingPoint.c -o build/native_floating_point_verification/verify_host_environment <<'C'
#include "NativeFloatingPoint.h"
#include <fenv.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#pragma STDC FENV_ACCESS ON

int main(void) {
    const int modes[4] = {FE_TONEAREST, FE_UPWARD, FE_DOWNWARD, FE_TOWARDZERO};
    unsigned checked = 0;
    for (int host_mode = 0; host_mode < 4; ++host_mode) {
        for (int guest_mode = 0; guest_mode < 4; ++guest_mode) {
            fesetenv(FE_DFL_ENV);
            fesetround(modes[host_mode]);
            feraiseexcept(FE_INVALID | FE_INEXACT);
            int exceptions = fetestexcept(FE_ALL_EXCEPT);
            uint32_t fpscr = UINT32_C(0x03000000) | (uint32_t)guest_mode << 22;
            uint32_t bits = NativeFloatingPointApplySingle(NativeFloatingPointAdd,
                UINT32_C(0x3F800000), UINT32_C(0x33C00000), 0, &fpscr);
            if (bits != (guest_mode == 0 || guest_mode == 1 ? UINT32_C(0x3F800001) : UINT32_C(0x3F800000))) return 1;
            if (fpscr != (UINT32_C(0x03000010) | (uint32_t)guest_mode << 22)) return 2;
            if (fegetround() != modes[host_mode] || fetestexcept(FE_ALL_EXCEPT) != exceptions) return 3;
            uint64_t double_bits = NativeFloatingPointApplyDouble(NativeFloatingPointDivide,
                UINT64_C(0x3FF0000000000000), 0, 0, &fpscr);
            if (double_bits != UINT64_C(0x7FF0000000000000) || !(fpscr & 2)) return 4;
            if (fegetround() != modes[host_mode] || fetestexcept(FE_ALL_EXCEPT) != exceptions) return 5;
            ++checked;
        }
    }
    printf("host floating-point environment preserved in %u combinations\n", checked);
    uint32_t fpscr = UINT32_C(0x03C09F9F);
    uint32_t signaling_nan = UINT32_C(0xFFA12345);
    if (NativeFloatingPointApplySingle(NativeFloatingPointMove, 0, signaling_nan, 0, &fpscr) != signaling_nan) return 6;
    if (NativeFloatingPointApplySingle(NativeFloatingPointAbsolute, 0, signaling_nan, 0, &fpscr) != (signaling_nan & UINT32_C(0x7FFFFFFF))) return 7;
    if (NativeFloatingPointApplySingle(NativeFloatingPointNegate, 0, signaling_nan, 0, &fpscr) != (signaling_nan ^ UINT32_C(0x80000000))) return 8;
    if (fpscr != UINT32_C(0x03C09F9F)) return 9;
    puts("raw bit operations preserve signaling NaN and enabled trap state");
    return 0;
}
C
build/native_floating_point_verification/verify_host_environment
```

Expected output confirms 16 preserved rounding combinations and unchanged raw signaling-NaN/trap state. Enabled arithmetic trap delivery remains deliberately unsupported.
