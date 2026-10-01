# Camera matrix refresh

## Scope and current status

The selected unchanged EU interval is `0x00261190–0x00261594`, 1,028 bytes, with
two trailing floats at pool `0x0026158C`: zero and one. The function was U in the
main refresh at `a059251507b899d48a6ab58b564cfa8a6d3d360f`. The game binary SHA256
is `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.

The original owning class, namespace, and method spelling are unproven. The
complete C++ reconstruction therefore retains `fn_00261190(void*)`, with private
descriptive layout types, in `lib/al/src/Camera/alCameraMatrixUpdate.cpp` and a
minimal declaration in `lib/al/include/Camera/alCameraMatrixUpdate.h`. Its location
in lib/al is an integration home, not a claim that this is an al-owned method.
The body is guarded `NON_MATCHING`. The retained source is exactly candidate 4,
SHA256 `4c4efac5d74a48c7a7a77ebd9d44e199e34846098eb44b8038916cd25760b152`.
Six source candidates were tested; the eight-candidate cap was not exhausted.
The complete guarded reconstruction is nonmatching. No accepted match or exact
coverage increase is claimed. The instruction stream differs substantially even
though the candidate's complete section is only four bytes longer than target.

## Independent binary evidence

- Constructor `0x002B17EC–0x002B18C4` first calls `0x00231660`, installs vtable
  `0x003D8660`, initializes 3x4 matrices at +0x148/+0x178, 4x4 matrices at
  +0x1A8/+0x1E8, and another 3x4 matrix at +0x228. It moves updater pointers into
  +0x258 and +0x260 and ownership bytes into +0x25C/+0x264
- The base constructor zeros a parent-like pointer at +0x0C and initializes a
  transform at +0x8C. The target extracts its fourth column at +0x98/+0xA8/+0xB8
  as a three-float position. The owning vtable's +0x14 slot is `0x00338D60`, whose
  complete body returns this+0x8C
- The target calls the +0x258 object's +0x0C virtual slot with output +0x148,
  the parent's +0x14 matrix result (or identity), and the extracted position.
  It then inverses +0x148 into +0x178. This supports view/inverse-view roles
- The target calls the distinct +0x260 object's +0x0C slot with output +0x1A8
  and the 3x4 matrix at +0x228. It then computes a general 4x4 inverse into +0x1E8.
  Renderer helper `0x0024D530` independently uploads +0x148 and +0x1A8 to GPU
  uniform streams. The +0x228 post-projection label remains descriptive
- The two view-resource getters at `0x002610EC` and `0x0026111C` use the +0x258
  object's +0x14 virtual slot and resource-kind bits 0x40000000/0x80000000.
  Perspective parameter getter `0x0033CA2C` similarly uses +0x260 and kind bit
  0x20000000. This independently distinguishes the updater responsibilities
- Five direct call sites are `0x0015943C`, `0x001595D0`, `0x001C74FC`,
  `0x0024D0C8`, and `0x00273584`. The first two refresh after world-transform
  function `0x00261594`; the last three pass the same camera to rendering setup

Three external address identities require only names on existing function rows:

- `fn_00254890`, interval `0x00254890–0x00254910`, returns a 3x4 identity matrix.
  Its body guards static initialization with `0x003F389C`, writes the twelve
  identity floats at `0x00430C68`, and returns that address
- `fn_0022E130`, interval `0x0022E130–0x0022E204`, returns a 4x4 identity matrix.
  Its body uses guard `0x003F38A0` and writes sixteen identity floats at
  `0x00430C98`. The camera constructor independently calls both getters
- `fn_00287284`, interval `0x00287284–0x002873A0`, inverses a 3x4 affine matrix
  from r1 to r0, returning zero for either signed-zero determinant and one on
  success. Other callers include `CFL_MakeModelIcon` and `0x00272420`

The source does not import any of those guards, identity objects, vtables, or
literal-pool locations. It neither invents data aliases nor changes boundaries.
No removed NintendoWare/sead implementation or other external SDK code was read.

## Reconstructed semantics

The method always refreshes the view and projection matrices. A null parent uses
the identity parent transform. The view and projection updater pointers are
required, matching the target's unchecked calls. Each failed inverse is replaced
with identity. The projection inverse fails only when its computed determinant
compares equal to zero; it does not use an epsilon or attempt to reject NaNs.

The determinant sums four positive groups before subtracting four negative
groups. The cofactor numerators preserve the observed operation grouping,
including the leading negative third-row terms. Those details matter for float
rounding. No iteration, matrix library, approximation, or generic elimination
algorithm substitutes for this target arithmetic.

## Canonical attempts

1. Source committed as `9c88886`, normal project ARMCC 4.1/791 build succeeded.
   The unchanged canonical checker reports:
   `U -> M: The complete compiled section, including its literal pool, has a different size from the original interval.`
   The generated section is 1,324 bytes, stack allocation 0x94. Target is 1,028
   bytes with 0x14 allocation. Source SHA256 is
   `54dfd467e8748c83639613b688e39be5347c8159f1fa05b93450f349fbd6f918`.
   Read-only inspection shows repeated 2x2 minor calculations and many spills;
   output stores occur before remaining cofactors are calculated
2. Candidate committed as `aa62f8b`, with eighteen shared minor locals and sixteen result
   locals before output stores. Determinant, position, copies, bool return,
   and interfaces are unchanged. Source SHA256 is
   `0fb4dbb6e8f50c4546bdf7f0ae0d83ab9c1dd35dff72642c6cf3d3224dd2ceec`.
   Normal project build succeeded. Canonical result:
   `M -> M: The complete compiled section, including its literal pool, has a different size from the original interval.`
   The generated section shrank to 1,040 bytes with a 0x2C stack allocation.
   Repeated minors disappeared, but determinant groups still overlap heavily,
   causing spills. Inlining folds the output pointer into state+offset and
   moves the singular fallback before the inverse arithmetic; the target
   retains a separate output pointer and success flag and leaves fallback last
3. Source committed as `466b9f0`, removing only `inline` from the private inverse
   helper. Project build succeeded, but canonical checking rejects source closure:
   `Residual helper code or another allocated extent remains after inlining.`
   The object contains a 288-byte driver and an 804-byte helper. This experiment
   did not establish a match or an improvement and is not retained
4. Source committed as `2a42dd0`. Restored `inline`; changed only position construction to an explicit
   three-float constructor. Source SHA256 is
   `4c4efac5d74a48c7a7a77ebd9d44e199e34846098eb44b8038916cd25760b152`.
   Normal project build succeeded; checker remains `M -> M` for section size.
   Section is 1,032 bytes with 0x2C frame. Position construction uses three GPR
   loads and STM, still unlike the target's VFP loads and stores
5. Source committed as `7697dcb`. Changed only the matrix declarations from structs to unions to test opaque
   representation copying without adding a memcpy import. Source SHA256 is
   `9dffdd213646b712a038b1ca9139422b2015a65303d061bf2c3b61287c44f7ed`.
   Normal project build succeeded; checker remains `M -> M` for section size.
   Section is still 1,032 bytes/0x2C frame. Representation copying did not change;
   only some final output scheduling differed. The union experiment was reverted
6. Source committed as `71463df`, using candidate 4 as the controlled baseline.
   Reverted the union experiment, then changed only determinant spelling to an
   assignment followed by three += and four -= statements, preserving operation
   order. Source SHA256 was
   `2735a15835d5f76be4f2fbdde5a722c9901e49c3c9ab126872d14144903217c6`.
   Build succeeded and checker again reported `M -> M` for section size.
   Section stayed 1,032 bytes/0x2C frame and retained the overlapping determinant
   schedule. Its canonical object SHA256 was
   `f9c531339cc13c1900283e7a99a6e7cb1a3786f8ee1abc7b4258f1afc4b42d98`.
   The ineffective experiment was reverted to the exact candidate 4 source

The parent serializes commits, builds, and checker calls. This worker never
compiled or edited map, ranks, flags, tools, ledger, or STATE. A size comparison,
arithmetic test, or disassembly inspection cannot establish a byte-exact match.

Canonical reproduction after naming the four existing function rows:

```sh
. ./development_environment.sh
export DEVKITARM=/usr
python make.py eu
python tools/check.py fn_00261190 --object build/eu/obj/lib/al/src/Camera/alCameraMatrixUpdate.o
```

The remaining substantive blocker is source-level code-generation shape:
matrix position/copies use different register classes, determinant scheduling
spills many more values, and the early-inlined inverse folds the output pointer
into state-relative stores while moving the failure block. The target keeps the
output pointer in r0 and a success flag in r1, with identity fallback last. A
concrete original math-type or inline-helper boundary is needed before more
experimentation; this pass does not consume attempts on arbitrary register-order
permutations. The packet is `project/pro_requests/00261190.md`, below 600 lines.

Actual `tools/diff.py --no_check fn_00261190 --format json` invocation exits zero
but prints `can't open file '.../tools/asm-differ/diff.py': [Errno 2] No such file or directory`.
This wrapper exit is not a successful diff. Strict checks and read-only complete
canonical-object listings supplied the measured differences above. No missing
dependency, boundary, or checker was altered to improve the result.

## Arithmetic validation, separate from compiler acceptance

The first candidate's formula expressions were evaluated directly from the C++
text and compared against an independent interpreter of target instructions
`0x00261268–0x00261558`. Each multiplication, addition, subtraction, division,
and negation was rounded with Python's `struct.pack('<f', value)` followed by
`struct.unpack('<f', ...)`; VMLA/VMLS were two rounded operations, consistent with
the target's scalar VFP sequence. The interpreter tracked its two float stack
temporaries and all sixteen output stores. Literal loads came from the verified
binary. This was a read-only inspection, not an emitted or checked ARM object.

Inputs: 4x4 identity, all +0, all -0, then 1,000 matrices using Python
`random.seed(261190)` and sixteen `random.uniform(-2, 2)` draws per matrix,
each first rounded to binary32. All 1,003 determinant bit comparisons passed;
the two zero matrices were singular; all 16,016 inverse-element bit comparisons
passed. For cases with |determinant| > 0.1, the largest absolute residual in
double-precision A * inverse(A) - I was `8.873016497545905e-06`. This finite-input
test does not establish every floating-point edge case or a compiled match.

Reproduce the arithmetic-only check from the repository root with `.venv/bin/python`:

```python
import random, re, struct
from pathlib import Path
from capstone import Cs, CS_ARCH_ARM, CS_MODE_ARM
binary = Path("data/ver/eu/code.bin").read_bytes()
source = Path("lib/al/src/Camera/alCameraMatrixUpdate.cpp").read_text()
code = list(Cs(CS_ARCH_ARM, CS_MODE_ARM).disasm(
    binary[0x161268:0x161558], 0x261268))
class F:
    def __init__(self, value):
        self.value = struct.unpack("<f", struct.pack("<f", float(value)))[0]
    def __add__(a, b): return F(a.value + b.value)
    def __sub__(a, b): return F(a.value - b.value)
    def __mul__(a, b): return F(a.value * b.value)
    def __truediv__(a, b): return F(a.value / b.value)
    def __neg__(a): return F(-a.value)
    def bits(a): return struct.pack("<f", a.value)
def target(values):
    regs, stack, output = {}, {}, {}
    for ins in code:
        op, args = ins.mnemonic, ins.op_str.split(", ")
        dst = args[0]
        if op == "vldr":
            if "r4" in ins.op_str:
                offset = int(re.search(r"#0x([a-f0-9]+)", ins.op_str)[1], 16)
                regs[dst] = values[(offset - 0x1a8) // 4]
            elif "pc" in ins.op_str:
                offset = int(re.search(r"#0x([a-f0-9]+)", ins.op_str)[1], 16)
                regs[dst] = F(struct.unpack_from("<f", binary,
                    ins.address + 8 + offset - 0x100000)[0])
            elif "sp" in ins.op_str:
                regs[dst] = stack[4 if "#4" in ins.op_str else 0]
        elif op == "vstr":
            if "sp" in ins.op_str:
                stack[4 if "#4" in ins.op_str else 0] = regs[dst]
            elif "r0" in ins.op_str:
                offset = re.search(r"#(0x[a-f0-9]+|[0-9]+)", ins.op_str)
                output[(int(offset[1], 0) if offset else 0) // 4] = regs[dst]
        elif op == "vmul.f32": regs[dst] = regs[args[1]] * regs[args[2]]
        elif op == "vmla.f32": regs[dst] = regs[dst] + regs[args[1]] * regs[args[2]]
        elif op == "vmls.f32": regs[dst] = regs[dst] - regs[args[1]] * regs[args[2]]
        elif op == "vdiv.f32": regs[dst] = regs[args[1]] / regs[args[2]]
        elif op == "vneg.f32": regs[dst] = -regs[args[1]]
        elif op == "vmov.f32": regs[dst] = regs[args[1]]
        elif op == "vcmp.f32":
            determinant = regs["s0"]
            if determinant.value == 0: return determinant, None
    return determinant, [output[index] for index in range(16)]
body = source[source.index("float determinant ="):source.index("        return true;")]
det_initial = re.search(r"float determinant =\s*(.*?);", body, re.S)[1]
det_updates = re.findall(r"determinant ([+-])= (.*?);", body, re.S)
assignments = re.findall(r"const float (\w+) =\s*(.*?);", body, re.S)
outputs = re.findall(r"result->m\[ (\d) \]\[ (\d) \] = (.*?);", body, re.S)
def expression(text):
    return " ".join(re.sub(r"(?<=\d)f\b", "", text).split())
random.seed(261190)
cases = [[F(i // 4 == i % 4) for i in range(16)], [F(0) for _ in range(16)],
         [F(-0.0) for _ in range(16)]]
cases += [[F(random.uniform(-2, 2)) for _ in range(16)] for _ in range(1000)]
count, singular = 0, 0
for values in cases:
    env = dict(zip("abcdefghijklmnop", values))
    determinant, expected = target(values)
    env["determinant"] = eval(expression(det_initial), {}, env)
    for operation, text in det_updates:
        value = eval(expression(text), {}, env)
        env["determinant"] = (env["determinant"] + value if operation == "+"
                              else env["determinant"] - value)
    assert env["determinant"].bits() == determinant.bits()
    if determinant.value == 0:
        assert expected is None
        singular += 1
        continue
    for name, text in assignments:
        if name == "inverseDeterminant":
            env[name] = F(1) / env["determinant"]
        else: env[name] = eval(expression(text), {}, env)
    actual = [eval(expression(text), {}, env) for row, col, text in outputs]
    assert len(actual) == 16
    assert all(a.bits() == b.bits() for a, b in zip(actual, expected))
    count += len(actual)
print(len(cases), "determinants;", count, "inverse values;", singular, "singular")
```

## Fresh current-main verification

Final retained source SHA2564c4efac5d74a48c7a7a77ebd9d44e199e34846098eb44b8038916cd25760b152 was applied to exact main21b861d6462f323be08b31b9996abe38603606e6, committed locally as ef1416e, and rebuilt with the unchanged canonical project command. Strict command `.venv/bin/python tools/check.py fn_00261190 --object build/eu/obj/lib/al/src/Camera/alCameraMatrixUpdate.o` again returned `U -> M: The complete compiled section, including its literal pool, has a different size from the original interval.` It remains1032 versus1028 bytes; no exact coverage. Main's scaffold needed a second ordinary make.py run after fresh object provenance became available; that retry linked/exported successfully without tool changes. The canonical checker files are identical to the earlier a504 base. Only the target's existing row needed a local address name; the three existing-boundary callee aliases resolve under the canonical checker without renaming them. No map, tools, flags, ledger or STATE changes are published.
