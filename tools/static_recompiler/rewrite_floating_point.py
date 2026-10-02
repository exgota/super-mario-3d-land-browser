"""Route generated arithmetic through checked bit-oriented native math helpers."""
from pathlib import Path
import re

EXPRESSIONS = ["acc + a * b", "acc - a * b", "-acc + a * b", "-acc - a * b",
               "a * b", "-(a * b)", "a + b", "a - b", "a / b"]
ARITHMETIC = re.compile(
    r"\{ (float|double) a = vfp_([sd])\(ctx, (\d+)\), b = vfp_\2\(ctx, (\d+)\), "
    r"acc = vfp_\2\(ctx, (\d+)\); vfp_set_\2\(ctx, \5, ([^;]+)\); \}")
SINGLE_SQUARE_ROOT = re.compile(r"vfp_set_s\(ctx, (\d+), sqrtf\(vfp_s\(ctx, (\d+)\)\)\);")
DOUBLE_SQUARE_ROOT = re.compile(r"\{ double v = vfp_d\(ctx, (\d+)\); vfp_set_d\(ctx, (\d+), sqrt\(v\)\); \}")


def arithmetic(wide, operation, destination, left, right, accumulator):
    if wide:
        return ("{ uint64_t result = NativeFloatingPointApplyDouble("
                f"(NativeFloatingPointOperation){operation}, {left}, {right}, {accumulator}, ctx->fpscr); "
                f"native_floating_point_store_double(ctx, {destination}, result); }}")
    return (f"ctx->vfp[{destination}] = NativeFloatingPointApplySingle("
            f"(NativeFloatingPointOperation){operation}, {left}, {right}, {accumulator}, ctx->fpscr);")


def rewrite(directory):
    directory = Path(directory)
    header = directory / "recomp.h"
    text = header.read_text()
    marker = "static void vfp_vector(Context *ctx, int op, int wide, int d, int n, int m) {"
    if text.count(marker) != 1:
        raise RuntimeError("pinned vector helper changed")
    support = '''static inline uint64_t native_floating_point_load_double(const Context* context, unsigned reg) {
    reg = (reg & 15) * 2;
    return (uint64_t)context->vfp[reg] | (uint64_t)context->vfp[reg + 1] << 32;
}
static inline void native_floating_point_store_double(Context* context, unsigned reg, uint64_t bits) {
    reg = (reg & 15) * 2;
    context->vfp[reg] = (uint32_t)bits;
    context->vfp[reg + 1] = (uint32_t)(bits >> 32);
}
'''
    text = '#include "NativeFloatingPoint.h"\n' + text.replace(marker, support + marker)
    original_double = "vfp_set_d(ctx, dd & 15, vfp_apply_d(op, vfp_d(ctx, nn & 15), vfp_d(ctx, mm & 15), vfp_d(ctx, dd & 15)));"
    original_single = "vfp_set_s(ctx, dd, vfp_apply_s(op, vfp_s(ctx, nn), vfp_s(ctx, mm), vfp_s(ctx, dd)));"
    for original in (original_single, original_double):
        if text.count(original) != 1:
            raise RuntimeError("pinned vector arithmetic changed")
    text = text.replace(original_double, arithmetic(True, "op", "dd", "native_floating_point_load_double(ctx, nn)",
                        "native_floating_point_load_double(ctx, mm)", "native_floating_point_load_double(ctx, dd)"))
    text = text.replace(original_single, arithmetic(False, "op", "dd", "ctx->vfp[nn]", "ctx->vfp[mm]", "ctx->vfp[dd]"))
    header.write_text(text)
    count = 0
    for path in sorted(directory.glob("code*.c")):
        def replace(match):
            nonlocal count
            wide = match[2] == "d"
            if (match[1] == "double") != wide or match[6] not in EXPRESSIONS:
                raise RuntimeError("unknown scalar arithmetic expression")
            count += 1
            def operand(register):
                return f"native_floating_point_load_double(ctx, {register})" if wide else f"ctx->vfp[{register}]"
            return arithmetic(wide, EXPRESSIONS.index(match[6]), match[5], operand(match[3]), operand(match[4]), operand(match[5]))
        text = ARITHMETIC.sub(replace, path.read_text())
        text, single_count = SINGLE_SQUARE_ROOT.subn(
            lambda match: arithmetic(False, 12, match[1], "0", f"ctx->vfp[{match[2]}]", "0"), text)
        text, double_count = DOUBLE_SQUARE_ROOT.subn(
            lambda match: arithmetic(True, 12, match[2], "0", f"native_floating_point_load_double(ctx, {match[1]})", "0"), text)
        count += single_count + double_count
        if re.search(r"(?:float|double) a = vfp_|sqrtf?\(", text):
            raise RuntimeError("unconverted scalar arithmetic remains")
        path.write_text(text)
    return count
