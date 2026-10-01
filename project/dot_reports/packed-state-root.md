# Float-state root 0x0020ACAC

**Integration qualification:** shared graphics C-symbol declarations differ between the state and shader branches. Do not claim combined integration until they are reconciled and regression checked. See [the exact conflict, common layout evidence, and proposed declaration contract](graphics-shared-declarations.md).

Base: e2cbf8db72566da78fc16b77a5b90026bfd9ea0a. Branch: dot/packed-state-root.
Target interval stays 0x0020ACAC..0x0020F690, 18,916 bytes. The map's final
pool marker is 0x0020F688; this is not the first pool. Final result: a complete,
guarded ordinary-C++ NonMatching proposal; zero exact-byte credit. The final
committed source is f3dc426 and the canonical 902 section is 19,040 bytes versus
18,916 original bytes. The unchanged checker rejects the size. Five source
forms and one alternate-compiler diagnostic were explored, below the eight-form
cap. This branch does not modify ranks, boundaries, tools, flags, or shared state.

The source is reconstructed exclusively from the owner's retail EU executable,
whose SHA256 is e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64.
No SDK source or external implementation was consulted.

## Identity and ABI

The root accepts a packed location, float input pointer, width, count, transpose
flag, and matrix flag. All arguments are core-register/stack arguments. The
three incoming direct call sites are 0x00245604, 0x00283E44, and 0x00390844.
Their containing roots are 0x002455D4, 0x00283E20, and 0x00390820. These wrappers
respectively write a four-float scalar tuple, an array of four-float tuples, and
one float, and pass zero for transpose/matrix. The scalar wrappers receive the
floats in VFP registers and materialize stack arrays before calling this root.
The original public/API name and exact library source ownership remain unproven;
the address name is retained. The body has no external calls.

Location -1 returns before allocating a stack frame. Count zero returns without
dereferencing either global. With location bit18 clear, bits0..6 select an array
element, bits7..17 select a 12-byte LocationRecord. Record word2 has bank bit10,
component bits11..12, matrix register stride bits13..14 plus one, and register
base bits24..31. Bank0's register pointer/dirty words are state+0x2c/+0x1b4;
bank1's are +0x1c0/+0x348. Each register is four words, and components are stored
in descending order. The path sets control dirty bit0x10000. Matrix transpose
changes whether the inner loop advances by a register or by a component.

Location bit18 selects the built-in state switch with ID (location>>2)&0xffff.
This branch repeatedly packs floating values into cached words, including
colors, truncated short floats, and fixed point. The calls and the field
operations support a graphics float-state/uniform-family identity; assigning an
exact API name is deferred.

## Global and cache ownership

The complete 16-byte map row 0x003E2E40..0x003E2E50 contains current Context* at
+8. Its context+0 is State*. The complete four-byte row 0x003E3154..0x003E3158
holds Control*. There are no vtables or fabricated data definitions in this
proposal. The header declares these whole map objects; their storage stays
owned by main.

Cached word j is state+0x4b4+4*j; its byte-enable mask is state+0x3f6+j; its dirty
bit lives at state+0x7a8+4*(j/32). The complementary shadow is context+0x100c+4*j.
Every setter ORs the byte-enable mask. A changed or forced word sets its dirty
bit and control flag0x80000; forceShadow additionally writes the complement to
the context shadow. This relationship is independently repeated by fixed and
indexed setters, and is also present in adjacent root0x00206474.

The header records a minimum observed State prefix, not a proven allocation
size. Eight records beginning at+0x9a0 have stride0x70. Four float4 material
fields begin at+0xd20. Original semantic field names remain descriptive only.

## Complete reconstruction and checked outcome

The implementation is in lib/CtrSDK/sources/FloatState.cpp under NON_MATCHING.
The existing SDK module supplies ARMCC4.0/902; this placement is a build choice,
not a claim of a recovered original filename. Both whole global imports resolve
through the existing map rows, with no local data stubs. The final object has
one root and its compiler switch labels, with no external function calls.

The completed State prefix reaches +0xe9c. The reconstructed built-in cases are:

- 1,2,19,21,22: raw float state plus short-float/fixed-point packed fields
- 31,32,33: paired range values and derived packed scale/offset, including bias
- 35,38,39,40,41,42,44: float tuples, scalar fields, and color packing
- 51,52: global color and offset; 53–56: four material tuples and eight dependent records
- 65–96: four groups of eight per-record colors, including the +0x96c mode byte
- 97–104: four-float record fields, two packed words, and a zero/nonzero flag
- 105–112: three-component signed fixed-point fields
- 161–176: two groups of eight truncated 20-bit float fields
- 207–213: six accepted quarter-scale values encoded into seven nibbles
- 270–281: two three-choice fields in each of six words
- 282–288,294: six color tuples, one further tuple, and one RGB tuple
- Every other ID returns unchanged; 289–293 are explicit retail no-ops

Short-float conversions preserve the observed signed-zero/underflow behavior,
truncate mantissas, and do not invent saturation for large exponents. Colors
retain unmasked channel spill. Upper-one clamping compares signed float bit
patterns, including negative NaNs. Copies preserve raw float bits. The four-value
copy retains the observed one-element lookahead instead of substituting memmove.

Source checkpoints and unchanged canonical outcomes:

| Form | Commit | Measurement | Outcome |
| --- | --- | --- | --- |
| Layout | 059c342 | No compiled body | ABI, callers, and offsets established |
| 1 | f4fdae2 | Root symbol 12,816 bytes | Source closure rejects generated __aeabi_memmove4 |
| 2 | e65b9f2 | Full section 12,808 bytes | U -> M, complete-section size mismatch |
| 3 | 63c3009 | Full section 14,768 bytes | M -> M, complete-section size mismatch |
| 4 | 0600a92 | Full section 23,156 bytes | M -> M, complete-section size mismatch |
| 5 | f3dc426 | Full section 19,040 bytes | M -> M, complete-section size mismatch |
| 6 diagnostic | Same source, ARMCC4.1/791 | Full section 18,588 bytes | Provenance correctly rejects nonconfigured compiler |

The temporary ignored module override for the diagnostic was removed. An initial
make with the override reused the old902 object; touching the source forced the
actual791 diagnostic compilation. No result from the stale first invocation was
attributed to791. Neither compiler identity nor exact matching follows from this
diagnostic. Retail's register-homing entry resembles the902 form;791 merges both
entry guards and has a different save set and frame.

Final normal build, after restoring map and compiler configuration:

    . ./development_environment.sh
    export DEVKITARM=/usr
    python make.py eu -ca

This clean build compiles, archives, links, and exports successfully. With only a
temporary local fn_0020ACAC name on the unchanged target row, the final command

    python tools/check.py fn_0020ACAC --object build/eu/obj/lib/CtrSDK/sources/FloatState.o

prints:

    U -> M: The complete compiled section, including its literal pool, has a different size from the original interval.

The map is then restored exactly. Canonical provenance succeeds with source
lib/CtrSDK/sources/FloatState.cpp and compiler4.0/902. The final function symbol
size is19,036; its full compiler section is19,040. The latter is authoritative.

## Bounded behavioral validation

Forms2 and3 each pass27,524 original/candidate whole-root ARM11 emulator pairs.
Final form5 additionally passes55,048 call pairs across27,524 fixtures: every
fixture invokes each root twice, checking both the first and repeated write.
All compared state, context shadow, control, register banks, and input bytes
agree. R4–R11 and SP are preserved. FPSCR exception bits0x9f agree; the condition
flags at return are not compared because this is a void function.

The suite uses Unicorn's ARM1176 CPU model with VFPv2 enabled. It executes the
unaltered original root and a diagnostic armlink image made directly from the
canonical source object at another code address. It does not hook arithmetic,
replace external calls, fabricate instructions, patch either code image, or use
the emulator result as an exact-match oracle.

Coverage includes:

- All300 built-in IDs in both force modes under six FPSCR settings
- 112 active built-in IDs against35 floating bit patterns under those settings
- Both signed zeros, subnormals, normal boundaries, infinities, positive and negative quiet/signaling NaNs, finite extremes, and ordinary enum/scale values
- FPSCR0, FZ, DN, and the three nondefault rounding modes individually
- 400 ordinary register/matrix cases: both banks, transpose modes, dimensions1–4, valid component offsets, counts0/1/2/3/-1
- Four location/count early-return cases
- Repeated writes, unchanged packed-word comparisons, and unchanged material early returns

This is bounded emulator evidence, not a proof for arbitrary inputs or native
hardware. It uses distinct, mapped input/state/control buffers with sufficient
padding and valid ordinary register ranges. Invalid pointers, memory faults,
concurrent mutation, fully general aliasing, combined FPSCR mode bits, and every
possible floating value are outside the claim. Retail's pipelined loops perform
some discarded reads beyond the last used source element; the C++ source does
not force those reads, so page-boundary fault behavior is explicitly unverified.

The final canonical object SHA256 is
b2f9451a3cd16d508b570c292eab5aa874e0402b8ed7eca86a05ce9fe6ca61be.
Source SHA256 is b5520ff0972084d90e578f424f6b4d6c48d769c2fb6d603a716119f23f44364f.
Header SHA256 is 7b75baaeb7826fec6f1957fe1ce3c37880f3c5484c2aba6d21adb8fa444d1a79.
The clean rebuild reproduces the same object hash used by the validation.

## Remaining exact-match blocker

The current root has a0xa0-byte local frame, versus retail's0x0c, and a19,040-byte
full section versus18,916. The surviving differences include temporary lifetimes,
register allocation, loop unrolling, switch layout, arithmetic common-subexpression
reuse, and consequent literal-pool placement. No guessed ABI or missing dispatch
case is being concealed behind this codegen blocker. Matching requires recovering
more of the original source expression/lifetime shape and validating any further
form through the canonical checker. Neither adjusting boundaries nor embedding
instruction bytes is an acceptable remedy. The two remaining experiment slots
are retained rather than spent on cosmetic size tuning.

Adjacent0x00206474 was read only as supporting evidence and was not implemented
or claimed by this branch. Parent owns publication and main intake. No game data,
map edits, compiler override, tools, ledger, STATE, or flags are committed here.

## Reproducing the bounded validation

After the normal project build, create build/float_state_validation and write
original_symbols.sym there with these three lines:

    #<SYMDEFS>#
    0x003E2E40 D dat_003E2E40
    0x003E3154 D dat_003E3154

Create the separate diagnostic executable from the unchanged compiler object:

    TMP=/tmp data/compilers/wibo data/compilers/4.0/902/bin/armlink.exe --cpu=MPCore --fpu=VFPv2 --arm_only --no_exceptions --inline --datacompressor=off --no_debug --no_scanlib --mangled --symbols --map --entry=fn_0020ACAC --keep=fn_0020ACAC --ro_base=0x00500000 --output=build/float_state_validation/candidate.axf --list=build/float_state_validation/candidate.map build/eu/obj/lib/CtrSDK/sources/FloatState.o build/float_state_validation/original_symbols.sym

Run the following harness from the repository root in the existing environment.
It writes results only under the ignored build directory. The fixture data is
synthetic; no original executable data is included in this report.

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import *
from unicorn.arm_const import *
import struct,random,json,hashlib,sys,time
ROOT=Path.cwd()
BINARY=(ROOT/'data/ver/eu/code.bin').read_bytes()
assert hashlib.sha256(BINARY).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
S=0x800000;C=0x804000;D=0x808000;A=0x80a000;B=0x80c000;T=0x80e000;V=0x820000;SP=0x90f000;END=0x980000
REGS=[UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3,UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11,UC_ARM_REG_R12]
SNAPS=[('state',S,0x1000),('context',C,0x1400),('control',D,0x600),('bank0',A,0x1800),('bank1',B,0x1800),('input',V,0x1000)]
def init(candidate):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176)
 u.mem_map(0x100000,0x400000);u.mem_write(0x100000,BINARY)
 u.mem_map(0x500000,0x10000)
 entry=0x20acac
 if candidate:
  with (ROOT/'build/float_state_validation/candidate.axf').open('rb') as f:
   e=ELFFile(f)
   for sec in e.iter_sections():
    if sec['sh_flags']&2 and sec['sh_size'] and sec['sh_type']=='SHT_PROGBITS':u.mem_write(sec['sh_addr'],sec.data())
   entry=e['e_entry']
 u.mem_map(S,0x30000);u.mem_map(0x900000,0x10000);u.mem_map(END,0x1000)
 u.reg_write(UC_ARM_REG_C1_C0_2,0xf00000);u.reg_write(UC_ARM_REG_FPEXC,0x40000000)
 u.mem_write(0x3e2e48,struct.pack('<I',C));u.mem_write(0x3e3154,struct.pack('<I',D))
 return u,entry
ORIG=init(False);CAND=init(True)
def fbits(x):return struct.unpack('<I',struct.pack('<f',x))[0]
def fixture(seed,values,force,multiply,meta):
 r=random.Random(seed);blob=bytearray(r.randbytes(0x30000))
 def w(a,x):struct.pack_into('<I',blob,a-S,x&0xffffffff)
 def f(a,x):w(a,fbits(x))
 w(C,S);w(S+0x1c,T);w(S+0x2c,A);w(S+0x1c0,B)
 for a in range(S+0x990,S+0xe9c,4):f(a,r.choice([0.,.1,.25,.5,1.,1.5,-.5,2.]))
 blob[D-S+12]=force;blob[S-S+0x96c]=multiply
 f(D+0x44,0.25);f(D+0x4c,0.2);f(D+0x50,0.8);blob[D-S+0x54]=seed%2;w(D+0x5bc,seed%3)
 for j in range(128):
  w(T+j*12,0x1234);w(T+j*12+4,0x5678);w(T+j*12+8,meta)
 for j,v in enumerate(values):w(V+j*4,v)
 return blob
FAIL=[];TOTAL=0;FPSDIFF=0

def run(case):
 global TOTAL,FPSDIFF
 name,loc,values,width,count,transpose,matrix,force,multiply,meta,mode,seed=case
 blob=fixture(seed,values,force,multiply,meta)
 outputs=[]
 for u,entry in (ORIG,CAND):
  u.mem_write(S,bytes(blob));u.mem_write(0x900000,bytes(0x10000))
  for j,reg in enumerate(REGS):u.reg_write(reg,0xa0000000+j)
  for j in range(32):u.reg_write(UC_ARM_REG_S0+j,0x3f000000+j)
  for reg,val in zip(REGS,[loc,V,width,count]):u.reg_write(reg,val&0xffffffff)
  u.reg_write(UC_ARM_REG_CPSR,0x10);u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,END);u.reg_write(UC_ARM_REG_FPSCR,mode)
  u.mem_write(SP,struct.pack('<II',transpose,matrix))
  try:u.emu_start(entry,END,count=200000)
  except UcError as error:
   FAIL.append({'case':name,'error':str(error),'candidate':entry!=0x20acac,'pc':hex(u.reg_read(UC_ARM_REG_PC))});return
  if u.reg_read(UC_ARM_REG_PC)!=END:
   FAIL.append({'case':name,'error':'instruction cap','candidate':entry!=0x20acac});return
  saved=[u.reg_read(x) for x in REGS[4:12]]
  if saved!=[0xa0000000+j for j in range(4,12)] or u.reg_read(UC_ARM_REG_SP)!=SP:
   FAIL.append({'case':name,'error':'ABI','candidate':entry!=0x20acac});return
  first=[bytes(u.mem_read(a,n)) for _,a,n in SNAPS]
  for reg,val in zip(REGS,[loc,V,width,count]):u.reg_write(reg,val&0xffffffff)
  u.reg_write(UC_ARM_REG_LR,END)
  try:u.emu_start(entry,END,count=200000)
  except UcError as error:
   FAIL.append({'case':name,'error':'repeat: '+str(error),'candidate':entry!=0x20acac});return
  if u.reg_read(UC_ARM_REG_PC)!=END or u.reg_read(UC_ARM_REG_SP)!=SP or [u.reg_read(x) for x in REGS[4:12]]!=[0xa0000000+j for j in range(4,12)]:
   FAIL.append({'case':name,'error':'repeat completion/ABI','candidate':entry!=0x20acac});return
  outputs.append((first+[bytes(u.mem_read(a,n)) for _,a,n in SNAPS],u.reg_read(UC_ARM_REG_FPSCR)))
 TOTAL+=1
 for (label,a,n),x,y in zip(SNAPS*2,outputs[0][0],outputs[1][0]):
  if x!=y:
   dif=[i for i,(p,q) in enumerate(zip(x,y)) if p!=q]
   FAIL.append({'case':name,'buffer':label,'differences':len(dif),'first_offset':hex(dif[0]),'original':x[dif[0]//4*4:dif[0]//4*4+4].hex(),'candidate':y[dif[0]//4*4:dif[0]//4*4+4].hex(),'mode':hex(mode),'values':[hex(v) for v in values[:4]]})
 if outputs[0][1]&0x9f != outputs[1][1]&0x9f:FPSDIFF+=1

cases=[]
basevals=[fbits(x) for x in [.25,.5,.75,1.,2.,3.,4.,5.]]
for mode in [0,0x1000000,0x2000000,0x400000,0x800000,0xc00000]:
 for id in range(300):
  for force in [0,1]:
   cases.append((f'builtin{id}:force{force}:mode{mode:x}',0x40000|(id<<2),basevals,4,1,0,0,force,id%2,0,mode,id*2+force))

active=[1,2,19,21,22,31,32,33,35,38,39,40,41,42,44,51,52,53,54,55,56]+list(range(65,113))+list(range(161,177))+list(range(207,214))+list(range(270,289))+[294]
edges=[0,0x80000000,1,0x80000001,0x7fffff,0x807fffff,0x800000,0x80800000,0x3f7fffff,0x3f800000,0x3f800001,0xbf800000,0x7f800000,0xff800000,0x7fc00000,0xffc00000,0x7f800001,0xff800001,0x7fffffff,0xffffffff,0x7f7fffff,0xff7fffff]+[fbits(x) for x in [-100,-8,-2,-1.5,-.5,.25,.5,2,4,8,16,32,8192]]
for mode in [0,0x1000000,0x2000000,0x400000,0x800000,0xc00000]:
 for id in active:
  for edge in edges:
   cases.append((f'edge:{id}:{edge:08x}:mode{mode:x}',0x40000|(id<<2),[edge]*8,4,1,0,0,edge&1,(edge>>1)&1,0,mode,id+edge))
for bank in [0,1]:
 for matrix in [0,1]:
  for trans in [0,1]:
   for width in range(1,5):
    for count in [0,1,2,3,-1]:
     for component in range(5-width):
      stride=width if matrix else 1
      meta=(5<<24)|((stride-1)<<13)|(component<<11)|(bank<<10)
      vals=(edges*8)[:128]
      cases.append((f'copy:{bank}:{matrix}:{trans}:{width}:{count}:{component}',(2<<7)|3,vals,width,count,trans,matrix,0,0,meta,0,100))
for loc,count in [(0xffffffff,1),(0xffffffff,0),(0,0),(0x40000,0)]:
 cases.append((f'early:{loc:x}:{count}',loc,basevals,4,count,0,0,0,0,0,0,1))

if '--quick' in sys.argv:cases=cases[:600]
for k,case in enumerate(cases):
 run(case)
 if k%500==0:print('progress',k,'failures',len(FAIL),flush=True)
 if len(FAIL)>30:break
result={'fixture_pairs':TOTAL,'call_pairs':TOTAL*2,'memory_failures':FAIL,'fpscr_exception_differences':FPSDIFF,'object_sha256':hashlib.sha256((ROOT/'build/eu/obj/lib/CtrSDK/sources/FloatState.o').read_bytes()).hexdigest()}
(ROOT/'build/float_state_validation/results.json').write_text(json.dumps(result,indent=2))
print(json.dumps(result,indent=2))
```
