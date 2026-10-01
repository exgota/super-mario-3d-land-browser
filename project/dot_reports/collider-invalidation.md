# Collider invalidation: direct-field preservation and bounded replay

## Result

The first new source form, cumulative form 5, removes the preservation blocker
in `project/pro_requests/0024C9EC.md`. The existing direct
`mGroundDistance` getter expression stays unchanged while surrounding opaque
storage is expanded to the independently observed 512-byte Collider layout.
All 35 accepted definitions in the three existing dependent translation units
pass ordinary canonical object checks, including the complete 36-byte
`al::isCollidedGround` interval. No accepted regression was observed.

`al::Collider::onInvalidate` remains **NonMatching**, guarded by
`#ifdef NON_MATCHING`. Its complete canonical interval is 88 bytes and differs
in eight bytes. It adds **zero exact functions and zero exact bytes**. No new
semantic type/layout hypothesis justified further cosmetic scheduling trials;
forms 6 through 8 were not compiled. The four earlier forms and their eight
physical target compiles remain documented in the original packet.

The actual retail zero-capacity constructor, including all nested initializers,
and an independent whole-memory layout model agree in 3,200 paired cases.
The retail and canonical candidate invalidation each execute the actual position
provider on all five paths and agree with an independent whole-memory model in
3,200 paired cases, totaling 6,400 provider calls. This supports a bounded
functional proposal, not exact credit or a gameplay/hardware claim.

## Immutable inputs and build

Base: `3de056e0dcb33619c32f8b46ef64f74c90d28934`.
Source commit: `4f90ef56dcf8877265f3dbfc5f20be2ea9eb1cac` on `dot/collider-invalidation`.
Only `lib/al/include/Collision/alCollider.h`, the new
`lib/al/src/Collision/alCollider.cpp`, this report and its packet response change on the branch.
No map, STATE, ledger, tool, configuration, compiler flag, game data or binary
is proposed for intake. All three Pro-reserved intervals remain untouched.

A clean `python make.py eu -ca` compiled 44 Game, 129 al and 1 SDK translation
units, linked and exported successfully. It used configured ARMCC 4.1/791 for
al/Game and 4.0/902 for SDK, with the normal project flags and installed wibo.
The new C++ and header were committed before that build. The ordinary checker
verified the resulting project object and its committed-input provenance.
The target remains U in the restored branch map, so the clean compact link is
separate from the target's canonical isolated check.

The executable SHA-256 is
`e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.
The map was restored byte-for-byte after every checking batch. No temporary
names were needed: the established unnamed provider interval already resolves
the narrow external C address name `fn_003370B4` in the canonical checker.

SHA-256 values from the verified build:

- `lib/al/include/Collision/alCollider.h`: `bab9d41db4e27648f3cdd354eed67a3e4c752d3b137c7c5ca0e1732de5267cd5`
- `lib/al/src/Collision/alCollider.cpp`: `91475d777b132897405e77dbffa3f33bac80b4b21cb40dd2a3da04fd2ccff7b1`
- `build/eu/obj/lib/al/src/Collision/alCollider.o`: `3f7f3d0357c0d8cbe8998f7befa027f0aa23e8469ed2189f33688c80554d8dc0`
- `build/eu/obj/lib/al/src/Collision/alCollisionUtil.o`: `970c3374d8def828ab1d97805d48a032d2e09b5172706e48abbde1b4c4393d61`
- `data/ver/eu/map.csv`: `514c3ec6f2343e8a40fc73996df78fc95b2e33f4897fbdda6bb0311df6cf40fa`

## Target and scheduling difference

Target `_ZN2al8Collider12onInvalidateEv`: `0024C9EC..0024CA44`,
80 code bytes and 8 literal-pool bytes. The sole external call is the unchanged
`003370B4..00337264` provider, passed the Collider and its previous-position
output at offset `0x1F0`. All helpers disappear; the object has one executable
section, containing only the requested function. No helper address is invented.

Ordinary checker output:

```text
U -> m: The linked candidate differs from the unchanged original interval.
O -> O: The complete source-generated function interval matches byte for byte.
```

The second line is the accepted ground predicate. The target's complete
canonical linked-section hash is
`6ac301dadcd4eac8ca50c924d1dd8bc560709f49c9db4e5d8e24382636221486`;
retail is `5a272a14661478a6257f4874ce7a541a6f20993effd102d8dc31cc8a15363f40`.
The complete disassembly difference is:

```diff
 push {r4, lr}
 mov r4, r0
+vldr s0, [pc, #0x40]
 mov r0, #0
-vldr s0, [pc, #0x3c]
 str r0, [r4, #0x24]
```

Bytes from `0024C9FC` through the end of the complete interval agree.
The two pool words are unchanged: `C7C34F80` (-99999.0f) and `00000000`.
No floating-register workaround was applied to the accepted predicate.

## Layout and bounded semantic evidence

Allocation `001EBC64..001EBCEC` requests `0x200` bytes. Constructor
`001EB664..001EB78C` independently establishes radius `+0x14`, count `+0x24`,
displacement `+0x2C`, three result blocks beginning `+0x38/+0xC4/+0x150`,
distances `+0xC0/+0x14C/+0x1D8`, integer `+0x1DC`, previous position
`+0x1F0`, and previous radius `+0x1FC`. The opaque first result remains split
at its direct distance field to preserve accepted source expression structure.
The two other distance categories and the detailed integer semantics stay
unresolved; the header does not label them wall or ceiling.

The replay executes the original constructor and nested routines
`0024CD4C..0024CD9C`, `0026F628..0026F71C`,
`0028EABC..0028EAFC`, and `0039AAE8..0039AAEC`.
The constructor's capacity is zero, so allocation is neither mocked nor claimed.
The ordinary zero-initialized BSS supplies the referenced vector-zero object.
The independent model checks all 64 KiB of fixture memory, including untouched
padding and surroundings; no constructor or provider call is replaced.

Five provider paths are covered equally (640 pairs each): absent offset with
absent matrix, absent offset with matrix, offset with absent matrix, offset with
matrix but transform flag clear, and offset with matrix and transform flag set.
There are 128 deterministic base fixtures, five paths and five FPSCR settings.
Positions, offsets, matrix elements and axial heights use exact dyadic values;
radii include arbitrary bits, signed zeros, subnormals, infinities and NaNs,
which are copied as observed. No arithmetic claim for nonfinite provider inputs
is made. Every run checks return, SP, r4-r11, d8 and the provider's receiver/output
arguments. Maximum observed execution is 513 instructions.

The test is bounded to valid disjoint inputs, finite exact provider arithmetic,
zero-capacity constructors and emulated ARM11 MPCore/VFP. It excludes allocation,
aliased or faulting pointers, concurrent observation, hardware and gameplay.

## Reproduction

Use a checkout of this branch with the private owner-provided `code.bin` and
`exh.bin` at the usual ignored locations, installed compilers/wibo, and the normal
Python environment. Unicorn 2.1.4 and pyelftools are required for replay only.
No executable or game data is distributed in this report. On the dot computer,
the existing `mario-dot/.venv` and `data/compilers` are reusable setup paths;
objects and archives must be built in the new checkout.

Run from the repository root:

```sh
. ./development_environment.sh
export TMP=/tmp
sha256sum data/ver/eu/code.bin
python make.py eu -ca
python tools/check.py _ZN2al8Collider12onInvalidateEv --object build/eu/obj/lib/al/src/Collision/alCollider.o
python tools/check.py _ZN2al16isCollidedGroundEPKNS_9LiveActorE --object build/eu/obj/lib/al/src/Collision/alCollisionUtil.o
```

The first checker is expected to return exit 1 with `U -> m`; it is not a match.
The next script runs all affected accepted checks and restores the original map
from HEAD. Save it as `/tmp/collider-accepted-check.py`, then run
`python /tmp/collider-accepted-check.py`. Expected result: 35 checks, 35 passes.
The recorded verification batch took 23.875 seconds; this is not an estimate for
other hosts and excludes clean building and replay.

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
import csv,json,subprocess,hashlib,time
root=Path.cwd();mp=root/'data/ver/eu/map.csv';original=subprocess.check_output(['git','show','HEAD:data/ver/eu/map.csv']);mp.write_bytes(original)
rows=[{k.strip():v.strip() for k,v in r.items()} for r in csv.DictReader(original.decode().splitlines())]
accepted={r['Symbol']:r for r in rows if r['Rank']=='O'}
objects=[];results=[];start=time.time()
for dep in sorted((root/'build/eu/obj').rglob('*.d')):
 if '/lib/al/include/Collision/alCollider.h' in dep.read_text():objects.append(dep.with_suffix('.o'))
for obj in objects:
 with obj.open('rb') as f:
  elf=ELFFile(f);table=elf.get_section_by_name('.symtab');names=[s.name for s in table.iter_symbols() if s.name in accepted and s['st_info']['type']=='STT_FUNC' and isinstance(s['st_shndx'],int)]
 for name in sorted(set(names)):
  result=subprocess.run([str(root/'.venv/bin/python'),'tools/check.py',name,'--object',str(obj.relative_to(root))],capture_output=True,text=True)
  record={'symbol':name,'object':str(obj.relative_to(root)),'exit':result.returncode,'output':result.stdout+result.stderr};results.append(record)
  if result.returncode: print(json.dumps(record),flush=True)
summary={'objects':[str(o.relative_to(root)) for o in objects],'checks':len(results),'passes':sum(r['exit']==0 for r in results),'seconds':time.time()-start,'results':results,'map_sha256':hashlib.sha256(original).hexdigest()}
mp.write_bytes(original)
(root/'build/collider-accepted-check.json').write_text(json.dumps(summary,indent=2)+'\n')
print(json.dumps({k:v for k,v in summary.items() if k!='results'},indent=2))
assert all(r['exit']==0 for r in results)
```

The canonical checker leaves the isolated candidate under the ignored
`build/exact_checks/eu/` directory. The next script reads that result; it does not
construct, edit or submit an object for acceptance. The only code replacement
is loading those already checked source-generated bytes into a separate emulator
instance for functional comparison. Save as `/tmp/collider-arm-replay.py`, then
run `python /tmp/collider-arm-replay.py`.

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_ARM, UC_MODE_ARM, UC_HOOK_CODE
from unicorn.arm_const import *
import struct, random, hashlib, json

ROOT = Path.cwd()
BLOB = (ROOT / 'data/ver/eu/code.bin').read_bytes()
assert hashlib.sha256(BLOB).hexdigest() == 'e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
START, SIZE, CONSTRUCTOR, PROVIDER = 0x24c9ec, 88, 0x1eb664, 0x3370b4
DB, DS, STACK, SP, END = 0x1000000, 0x10000, 0x70000000, 0x70008000, 0x71000000
POSITION, MATRIX, OFFSET = DB + 0x3000, DB + 0x4000, DB + 0x5000
candidate_path = max((ROOT / 'build/exact_checks/eu').glob('function_0024C9EC_*/candidate.axf'), key=lambda p: p.stat().st_mtime)
with candidate_path.open('rb') as stream:
    elf = ELFFile(stream)
    candidate = next(s.data() for s in elf.iter_sections() if s['sh_addr'] == START and s['sh_size'])
original = BLOB[START - 0x100000:START - 0x100000 + SIZE]
assert len(candidate) == len(original) == SIZE
REGS = [UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2, UC_ARM_REG_R3, UC_ARM_REG_R4, UC_ARM_REG_R5, UC_ARM_REG_R6, UC_ARM_REG_R7, UC_ARM_REG_R8, UC_ARM_REG_R9, UC_ARM_REG_R10, UC_ARM_REG_R11, UC_ARM_REG_R12]

def word(data, address):
    return struct.unpack_from('<I', data, address - DB)[0]

def put(data, address, value):
    struct.pack_into('<I', data, address - DB, value & 0xffffffff)

def bits(value):
    return struct.unpack('<I', struct.pack('<f', value))[0]

def fvalue(value):
    return struct.unpack('<f', struct.pack('<I', value))[0]

def f32(value):
    return fvalue(bits(value))

def constructor_model(initial, obj, matrix, radius, height):
    data = bytearray(initial)
    for off, value in [(0, 0), (4, 0), (8, matrix), (12, POSITION), (16, DB + 0x6000), (20, radius), (24, height), (28, 0), (32, 0), (36, 0), (40, 0), (0x1dc, 0), (0x1e0, 0), (0x1e4, bits(1.0)), (0x1e8, 0), (0x1fc, radius)]:
        put(data, obj + off, value)
    for off in [0x2c, 0x30, 0x34]:
        put(data, obj + off, 0)
    for off in [0x38, 0xc4, 0x150]:
        at = obj - DB + off
        data[at:at + 0x85] = bytes(0x85)
        put(data, obj + off + 0x88, bits(-99999.0))
    at = obj - DB + 0x1ec
    data[at] = (data[at] & ~0x3c) | 3
    at = obj - DB + 0x1f0
    data[at:at + 12] = data[POSITION - DB:POSITION - DB + 12]
    return bytes(data)

def invalidate_model(initial, obj):
    data = bytearray(initial)
    put(data, obj + 0x24, 0)
    for off in [0xc0, 0x14c, 0x1d8]:
        put(data, obj + off, bits(-99999.0))
    for off in [0x2c, 0x30, 0x34]:
        put(data, obj + off, 0)
    put(data, obj + 0x1dc, -1)
    matrix, offset = word(data, obj + 8), word(data, obj + 0x1c)
    pos = [fvalue(word(data, POSITION + 4 * j)) for j in range(3)]
    if offset:
        values = [fvalue(word(data, offset + 4 * j)) for j in range(3)]
        if matrix and data[obj - DB + 0x1ec] & 4:
            for axis in range(3):
                for j in range(3):
                    pos[axis] = f32(pos[axis] + f32(fvalue(word(data, matrix + axis * 16 + j * 4)) * values[j]))
        else:
            pos = [f32(a + b) for a, b in zip(pos, values)]
    else:
        height = fvalue(word(data, obj + 0x18))
        if matrix:
            for axis in range(3):
                pos[axis] = f32(pos[axis] + f32(fvalue(word(data, matrix + axis * 16 + 4)) * height))
        else:
            pos[1] = f32(pos[1] + height)
    for j, value in enumerate(pos):
        put(data, obj + 0x1f0 + j * 4, bits(value))
    put(data, obj + 0x1fc, word(data, obj + 0x14))
    return bytes(data)

class Engine:
    def __init__(self, code):
        self.u = Uc(UC_ARCH_ARM, UC_MODE_ARM)
        self.u.ctl_set_cpu_model(UC_CPU_ARM_11MPCORE)
        self.u.mem_map(0x100000, 0x400000)
        self.u.mem_write(0x100000, BLOB)
        self.u.mem_write(START, code)
        self.u.mem_map(DB, DS)
        self.u.mem_map(STACK, 0x10000)
        self.u.mem_map(END, 0x1000)
        self.u.reg_write(UC_ARM_REG_C1_C0_2, 0xf << 20)
        self.u.reg_write(UC_ARM_REG_FPEXC, 1 << 30)
        self.u.hook_add(UC_HOOK_CODE, self.hook)
    def hook(self, u, pc, size, _):
        self.steps += 1
        if pc == PROVIDER:
            self.calls.append((u.reg_read(UC_ARM_REG_R0), u.reg_read(UC_ARM_REG_R1)))
        permitted = [(CONSTRUCTOR, 0x1eb77c), (0x24cd4c, 0x24cd98), (0x26f628, 0x26f714), (0x28eabc, 0x28eafc), (0x39aae8, 0x39aaec)] if self.constructor else [(START, START + 80), (PROVIDER, 0x337264)]
        assert any(low <= pc < high for low, high in permitted), hex(pc)
    def run(self, initial, obj, matrix, radius, height, fpscr, constructor):
        u = self.u
        u.mem_write(DB, initial)
        u.mem_write(STACK, bytes(0x10000))
        u.reg_write(UC_ARM_REG_CPSR, 0x10)
        u.reg_write(UC_ARM_REG_FPSCR, fpscr)
        for i, reg in enumerate(REGS):
            u.reg_write(reg, 0xa5a50000 + i)
        for reg, value in [(UC_ARM_REG_R0, obj), (UC_ARM_REG_R1, matrix), (UC_ARM_REG_R2, POSITION), (UC_ARM_REG_R3, DB + 0x6000), (UC_ARM_REG_SP, SP), (UC_ARM_REG_LR, END), (UC_ARM_REG_S0, radius), (UC_ARM_REG_S1, height), (UC_ARM_REG_D8, 0x123456789abcdef0)]:
            u.reg_write(reg, value)
        self.steps, self.calls, self.constructor = 0, [], constructor
        u.emu_start(CONSTRUCTOR if constructor else START, END, count=2000)
        assert u.reg_read(UC_ARM_REG_PC) == END
        assert u.reg_read(UC_ARM_REG_SP) == SP
        assert u.reg_read(UC_ARM_REG_D8) == 0x123456789abcdef0
        assert all(u.reg_read(REGS[i]) == 0xa5a50000 + i for i in range(4, 12))
        assert self.calls == ([] if constructor else [(obj, obj + 0x1f0)])
        if constructor:
            assert u.reg_read(UC_ARM_REG_R0) == obj
        return bytes(u.mem_read(DB, DS)), self.steps

rng = random.Random(START)
engines = [Engine(original), Engine(candidate)]
fpscr_values = [0, 1 << 24, 1 << 25, (1 << 24) | (1 << 25), 1 << 22]
max_steps, pairs, constructor_pairs = 0, 0, 0
for case in range(128):
    obj = DB + rng.randrange(0, 0x1000, 4)
    initial = bytearray(rng.randbytes(DS))
    for j in range(3):
        put(initial, POSITION + 4 * j, bits(rng.randrange(-32, 33) / 4))
        put(initial, OFFSET + 4 * j, bits(rng.randrange(-32, 33) / 4))
    for j in range(12):
        put(initial, MATRIX + 4 * j, bits(rng.randrange(-8, 9) / 4))
    radius = [0, 0x80000000, 1, 0x7f800000, 0xff800000, 0x7fc12345, 0x7f812345, rng.getrandbits(32)][case % 8]
    height = bits(rng.randrange(-32, 33) / 4)
    for mode in range(5):
        matrix = MATRIX if mode in (1, 3, 4) else 0
        constructed = constructor_model(initial, obj, matrix, radius, height)
        dirty = bytearray(constructed)
        for off in [0x24, 0x2c, 0x30, 0x34, 0xc0, 0x14c, 0x1d8, 0x1dc, 0x1f0, 0x1f4, 0x1f8, 0x1fc]:
            put(dirty, obj + off, rng.getrandbits(32))
        put(dirty, obj + 0x1c, OFFSET if mode >= 2 else 0)
        at = obj - DB + 0x1ec
        dirty[at] = (dirty[at] & ~4) | (4 if mode == 4 else 0)
        expected = invalidate_model(dirty, obj)
        for fpscr in fpscr_values:
            for engine in engines:
                result, steps = engine.run(bytes(initial), obj, matrix, radius, height, fpscr, True)
                assert result == constructed, ('constructor', case, mode, fpscr)
                max_steps = max(max_steps, steps)
                result, steps = engine.run(bytes(dirty), obj, matrix, radius, height, fpscr, False)
                assert result == expected, ('invalidate', case, mode, fpscr)
                max_steps = max(max_steps, steps)
            constructor_pairs += 1
            pairs += 1
summary = {'constructor_pairs': constructor_pairs, 'invalidation_pairs': pairs, 'provider_calls': 2 * pairs, 'path_pairs': {str(i): pairs // 5 for i in range(5)}, 'candidate_size': len(candidate), 'different_bytes': sum(a != b for a, b in zip(original, candidate)), 'candidate_sha256': hashlib.sha256(candidate).hexdigest(), 'original_sha256': hashlib.sha256(original).hexdigest(), 'maximum_instructions': max_steps, 'cpu': 'Unicorn 2.1.4 ARM11 MPCore with VFP enabled', 'fpscr_initial_values': fpscr_values, 'scope': 'Actual zero-capacity retail constructor and all nested initializers, actual position provider on every defined branch, original and canonical candidate invalidation, independent whole 64 KiB memory models, randomized aligned object placement/padding/prior state and radius bit patterns, exact dyadic provider arithmetic. No mocked imports. Finite bounded disjoint provider inputs; no allocation, fault, alias, concurrency, physical-hardware or gameplay claim. Adds no exact credit.'}
(ROOT / 'build/collider-arm-replay-results.json').write_text(json.dumps(summary, indent=2) + '\n')
print(json.dumps(summary, indent=2))
```

Expected replay result:

```json
{
  "constructor_pairs": 3200,
  "invalidation_pairs": 3200,
  "provider_calls": 6400,
  "path_pairs": {
    "0": 640,
    "1": 640,
    "2": 640,
    "3": 640,
    "4": 640
  },
  "candidate_size": 88,
  "different_bytes": 8,
  "candidate_sha256": "6ac301dadcd4eac8ca50c924d1dd8bc560709f49c9db4e5d8e24382636221486",
  "original_sha256": "5a272a14661478a6257f4874ce7a541a6f20993effd102d8dc31cc8a15363f40",
  "maximum_instructions": 513,
  "cpu": "Unicorn 2.1.4 ARM11 MPCore with VFP enabled",
  "fpscr_initial_values": [
    0,
    16777216,
    33554432,
    50331648,
    4194304
  ],
  "scope": "Actual zero-capacity retail constructor and all nested initializers, actual position provider on every defined branch, original and canonical candidate invalidation, independent whole 64 KiB memory models, randomized aligned object placement/padding/prior state and radius bit patterns, exact dyadic provider arithmetic. No mocked imports. Finite bounded disjoint provider inputs; no allocation, fault, alias, concurrency, physical-hardware or gameplay claim. Adds no exact credit."
}
```

## Intake guidance

Intake can adopt the expanded header and guarded source as a functional proposal,
provided the receiving checkout repeats its affected-root checks. Keep the
function outside exact counts. Future exact work should preserve the direct
ground getter and test a new independently justified structure, rather than
repeat the aggregate getter or alter instruction scheduling through assembly,
volatile fields, compiler flags or fabricated helper symbols.
