# Integer-state writer 0x00206474

**Integration qualification:** shared graphics C-symbol declarations differ between the state and shader branches. Do not claim combined integration until they are reconciled and regression checked. See [the exact conflict, common layout evidence, and proposed declaration contract](graphics-shared-declarations.md).

Base: 5025a6cd5ec8531fb1bc40ff4b570a0c01ef201c. Branch: dot/integer-state-root.
The original interval remains 0x00206474..0x0020ACAC, 18,488 bytes, with final
pool marker 0x0020ACA8. Source commit: 49c607d. This is an ordinary-C++
NonMatching proposal with zero exact-byte credit. One complete source form was
compiled; no alternate compiler, flags, function boundaries, or checker edits
were used. The configured ARMCC 4.0/902 section is 15,484 bytes. Compiler
identity for the original root is not established by this placement in CtrSDK.

The canonical checker first stops at the missing BSS data identity. Independently,
15,484 compiled section bytes versus 18,488 target bytes proves that this source
remains size-nonmatching even if main accepts that data identity. Resolving the
import alone cannot make this candidate exact.

The only reconstruction inputs were the owner's retail EU executable and the
clean FloatState proposal from dot/packed-state-root. The executable SHA256 is
e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64. No external SDK
implementation or removed library source was consulted. No game data is in
this branch.

## Identity and ABI

Three incoming direct calls occur at 0x002455AC, 0x002455C8, and 0x00390868.
Their wrapper roots begin at 0x00245594, 0x002455B8, and 0x00390850. They pass
three integers, one integer, and two integers respectively, materializing stack
arrays. The root takes location in r0, integer input pointer in r1, width in r2,
and count in r3. Width is overwritten without being read. The root itself has
no external calls. Original API/library names remain unproven; its address name
is retained.

Location -1 or count zero returns before loading the globals or changing the
scratch record. Other counts, including negative counts, enter the dispatch.
The context pointer comes from +8 of the complete 16-byte map object
0x003E2E40..0x003E2E50. Context+0 points to State. The complete four-byte map
object 0x003E3154..0x003E3158 contains Control*. The same objects, cache words,
byte enables, dirty words, and complementary shadow agree with FloatState.

A newly observed 12-byte scratch record at 0x0042013C receives location first.
When location bit 18 is clear it is overwritten with the selected 12-byte
LocationRecord. The entry LDR at 0x206498 uses the literal 0x0042013C at
0x207370; the store at 0x2064A8 and the three-word copy at 0x2064CC..0x2064D0
establish these accesses. This literal has one aligned occurrence in the
executable. The current map has no row for this BSS address. The header declares
it without defining storage. Twelve bytes are the observed accessed prefix,
not an independent proof of allocation boundaries. Main owns adding any data
identity, in a separate reviewed change.

The other data import, 0x003A480C..0x003A482C, already has a complete eight-word
map row. It is read as an enum conversion table by IDs 50 and 224. No table bytes
or substitute data definition are committed.

## Ordinary locations

Bits 0..6 select an element, bits 7..17 select the LocationRecord. Record word 2
uses bits 13..14 as kind, bit 10 as bank, and bits 24..31 as base register.

- Kind 0 updates one boolean bit for each input. Bank 0 uses cache word 9; bank 1
  uses word 1. It dirties that word even for negative count. Forced shadow writes
  the complemented final word. This path does not change byte-enable masks
- Kind 2 packs three integer low bytes into each word, in component order, and
  updates dirty/shadow state per word. Register index is base+element+i+10 for
  bank 0, or base+element+i+2 for bank 1. Negative count writes no word
- Kinds 1 and 3 return after writing the scratch record
- Kinds 0 and 2 always set control dirty 0x10000 after their loops

The bit helper explicitly reproduces ARM register-shift behavior for large
counts: low-eight-bit shift amount, with zero for amounts 32..255. The source
retains count as signed. Count and width do not bound built-in tuple widths.

## Built-in locations

Location bit 18 chooses ID (location>>2)&0xffff. All active families are present:

- 0,34,36,37: scalar enables and compare-mode encoding
- 3..18 and20: texture choices, color/wrap/filter encodings, packed components,
  and dependent control bits
- 23..30,43,45..50: raw enum/cache replacement and packed state
- 57..64: eight light enables, compact enabled-light ordering and count, and
  conditional lighting invalidation
- 113..160 and177..192: per-light flags and cached enums
- 193..206: seven packed single-bit fields and seven three-bit fields
- 214..232: enum caches and lighting flags, including three aggregate enables
- 233: material-multiplication mode and all eight fourth-color refreshes
- 234..269: six-stage combine modes, source triples, and operand triples
- 289..293,295,296: paired selectors, mode choices, flags, and cached enum
- All remaining IDs return after recording the location

Ordinary writes OR the byte-enable mask on every accepted call. On change or
forced shadow they update the word, its dirty bit, and control dirty 0x80000 plus
any case-specific flags. Forced writes additionally record the complement in
context+0x100C+index*4. Raw enum setters clear their corresponding control cache
only on the forced path. Triple setters reject an invalid component before
writing any cache word.

ID 233 uses the FloatState arithmetic contract: signed-bit upper-one clamping,
unsigned float conversion after 0.5+255*x, and unmasked channel spill into the
BGR ten-bit shifts. It recomputes all eight dependent colors only when the
stored multiplication byte changes. The source preserves the two evaluations
on the non-forced comparison/change path.

## Shared layout additions

FloatState.h is based on the earlier clean header, with previously unknown
padding replaced by observed fields. Every existing field offset is unchanged.
New neutral names avoid claiming original API names:

| Object | Offset | Observation |
| --- | --- | --- |
| State | 0x96D..0x96F | Three bytes whose OR drives an aggregate enable |
| State | 0x970 | Boolean used by IDs 222/223 |
| State | 0x974 | Six raw enums, IDs 214..219 |
| State | 0x98C | Eight-table-value enum, ID 224 |
| LightState | 0x00 | Enable byte, IDs 57..64 |
| LightState | 0x60/0x6C | Raw enums, IDs 145..152 and185..192 |
| State | 0xD70 | Seven raw enums, IDs 23..29 |
| State | 0xD8C/0xDAC/0xDB8 | ID 6, three texture enums, ID 30 |
| State | 0xDE4/0xDF4/0xDF8 | ID 296 enum, ID 43 byte, three ID 47..49 enums |
| Control | 0xF8 | Three texture enum values |
| Control | 0x104..0x107 | Three texture enables and one cube enable |
| Control | 0x10C | Six enum cache words |
| Control | 0x124/0x144 | Two groups of eight light enum cache words |
| Control | 0x164/0x180/0x184 | Seven enum caches, one cache, three caches |

The ARM build asserts the changed structure offsets. These structures describe
an observed prefix, not a proven complete allocation size.

## Undefined saved-enum edge

ID 50's transition from disabled to enabled scans the eight-word table for
State+0x98C. If it finds no entry, the retail path reaches 0x207EB8 without having
assigned r12 in this path, then uses r12 as the nibble encoding at 0x207EC0.
The C++ proposal deliberately retains the analogous uninitialized local
`unsigned encoding`, with ARMCC warning C3017W. There is no invented fallback
value or false claim that absent-table states are supported. The bounded replay
always initializes this saved enum from one of the actual eight table entries.
The source has undefined behavior for this invalid-state path and must not be
called an all-input equivalence proof or a hardened native implementation.

## Canonical build and checker result

Both the first build and a clean rebuild compile, archive, link, and export:

    . ./development_environment.sh
    export DEVKITARM=/usr
    python make.py eu -ca

The final normal build has root rank U unchanged, so this proposed root is not
pulled into the normal compact scaffold. Its canonical SDK object is emitted
by the configured build step. Committed-source provenance succeeds:

    source: lib/CtrSDK/sources/IntegerState.cpp
    compiler: 4.0/902
    object_sha256: e30a64a2dc74afc2b7677b8a0d134bdaa882426ac8272758d9c34878f7e3b551

With only a temporary local fn_00206474 name on the unchanged target row:

    python tools/check.py fn_00206474 --object build/eu/obj/lib/CtrSDK/sources/IntegerState.o

The unchanged checker reports:

    Source closure rejected: An unresolved source helper is referenced by a non-branch relocation.

The missing import is dat_0042013C. The canonical resolver cannot assign an
address to a data symbol absent from the map, and its source-closure fallback
correctly rejects the ABS32 data relocation. This is a resolver rejection,
not a completed byte-diff measurement. The map was restored exactly after both
checks, with no rank change. A diagnostic link using independently observed
symbol addresses is used only for replay, never as a canonical match result.

The complete compiler section and root symbol are both 15,484 bytes, versus
18,488 for the original interval. The compiled local frame is 0x48 bytes, versus
retail's single saved r3 slot. Grouped C++ cases, helper inlining, switch layout,
register lifetimes and literal pools remain code-generation differences. The
missing data identity would need separate main review before a canonical byte
comparison can proceed. No further form was spent merely tuning size around
that prerequisite. Seven of the eight form slots remain unused.

## Bounded behavioral validation

The completed expanded replay passes 67,444 original/candidate fixture pairs,
134,888 call pairs, with zero memory mismatches and zero FPSCR exception-bit
differences. It completes in 106.672 seconds on this machine. ABI and normal
return checks pass for both calls in every fixture.

The harness executes unmodified original ARM instructions and a diagnostic
armlink image made directly from the unchanged canonical compiler object.
Unicorn uses ARM1176 with VFPv2 enabled. No arithmetic or function calls are
hooked, no instructions or objects are patched, and no emulator evidence is
counted as exact matching. State, context shadow, control, input and the BSS
scratch record are compared after each of two repeated calls. Both executions
must preserve r4..r11 and SP and return normally. FPSCR exception bits 0x9F are
compared; final condition flags are not part of the void-function contract.

Coverage comprises 60,140 built-in fixtures (IDs 0..309, 97 input words, both
forced modes), 1,440 ordinary-location fixtures (both banks, all four kinds,
counts -1/0/1/2/5, six register bases and three elements), 840 exceptional-color
refresh fixtures, 4,800 mixed enum triples, 144 rejected-component triples,
32 mixed/rejected two-component selectors, 44 count-boundary fixtures, and four
early returns. Color refresh uses 35 floating bit patterns under six FPSCR
settings: default, FZ, DN and the three nondefault rounding modes. It includes
signed zeros, subnormals, infinities, positive/negative quiet/signaling NaNs,
finite extremes and ordinary values. These explicit color cases force a mode
transition to ensure the full refresh runs.

The claim is bounded to distinct mapped buffers, valid table-backed saved enum,
sufficiently sized register arrays and normal single-threaded state. Invalid
pointers, page-boundary fault timing, concurrency, fully general aliasing,
arbitrary color triples, combined FPSCR flags and all possible input bit
patterns remain unverified. The original enable enumeration performs a
discarded lookahead read beyond the eighth light enable; this C++ loop does not
force that discarded read. Fault behavior at that boundary is not claimed.

The initial replay traversed its 62,424 fixtures without logged memory failures,
but its result serialization failed because a concurrent clean rebuild removed
the object before the final hash read. That incomplete report is not counted.
The reproduction below freezes the object hash before execution and runs after
the completed clean build; its expanded result is the claimed evidence.

Source SHA256: 3e9aa50f4cb1056428e7ef3f1828b671a6095f9e6b36adcb110c0436cae7a0a0.
FloatState.h SHA256: 7585b740ed79b8ceb9a33bf8c63f0ff01b496e5a71acfb82defac12885801417.
IntegerState.h SHA256: e03f4d7eee8f6b35cf62d113f2e36b01cb2c5ae36bbb1f8627bb18e05fadbd94.
The clean rebuild reproduces the same canonical object hash used by replay.

## Reproduction

After the normal build, create build/integer_state_validation and write
original_symbols.sym there:

    #<SYMDEFS>#
    0x003E2E40 D dat_003E2E40
    0x003E3154 D dat_003E3154
    0x0042013C D dat_0042013C
    0x003A480C D dat_003A480C

Create the separate diagnostic image without changing the object:

    TMP=/tmp data/compilers/wibo data/compilers/4.0/902/bin/armlink.exe --cpu=MPCore --fpu=VFPv2 --arm_only --no_exceptions --inline --datacompressor=off --no_debug --no_scanlib --mangled --symbols --map --entry=fn_00206474 --keep=fn_00206474 --ro_base=0x00500000 --output=build/integer_state_validation/candidate.axf --list=build/integer_state_validation/candidate.map build/eu/obj/lib/CtrSDK/sources/IntegerState.o build/integer_state_validation/original_symbols.sym

Save the following as build/integer_state_validation/reproduce.py and run it
from the repository root in the existing Python environment. It writes only to
the ignored build directory and generates synthetic fixtures. Do not rebuild
concurrently with this validation.

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import *
from unicorn.arm_const import *
import struct,random,json,hashlib,sys,time
ROOT=Path.cwd();OUT=ROOT/'build/integer_state_validation'
OBJECT_SHA=hashlib.sha256((ROOT/'build/eu/obj/lib/CtrSDK/sources/IntegerState.o').read_bytes()).hexdigest()
BINARY=(ROOT/'data/ver/eu/code.bin').read_bytes()
assert hashlib.sha256(BINARY).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
S=0x800000;C=0x804000;D=0x808000;T=0x80e000;V=0x820000;SP=0x90f000;END=0x980000;SCRATCH=0x42013c
REGS=[UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3,UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11,UC_ARM_REG_R12]
SNAPS=[('state',S,0x1000),('context',C,0x1400),('control',D,0x600),('input',V,0x400),('scratch',SCRATCH,12)]
TABLE=struct.unpack_from('<8I',BINARY,0x3a480c-0x100000)
def init(candidate):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176)
 u.mem_map(0x100000,0x400000);u.mem_write(0x100000,BINARY)
 u.mem_map(0x500000,0x10000);entry=0x206474
 if candidate:
  with (OUT/'candidate.axf').open('rb') as f:
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
def fixture(seed,values,force,meta,color):
 r=random.Random(seed);blob=bytearray(r.randbytes(0x30000))
 def w(a,x):struct.pack_into('<I',blob,a-S,x&0xffffffff)
 def f(a,x):w(a,fbits(x))
 w(C,S);w(S+0x1c,T)
 for a in range(S+0x990,S+0xe9c,4):f(a,r.choice([0.,.25,.5,.75,1.,2.,-.5]))
 for i in range(8):
  blob[S-S+0x9a0+i*0x70]=r.randrange(2)
  for k in range(4):w(S+0x9d4+i*0x70+k*4,color if color is not None else fbits(r.choice([.25,.5,1.,2.])))
  w(T+i*12,0x12345678+i);w(T+i*12+4,0x89abcdef-i);w(T+i*12+8,meta)
 for i in range(6):blob[0x96c+i]=r.randrange(2)
 if color is not None:blob[0x96c]=values[0]!=0
 blob[D-S+12]=force
 w(S+0x98c,TABLE[seed%8])
 for i,v in enumerate(values):w(V+i*4,v)
 return blob
FAIL=[];TOTAL=0;FPSDIFF=0;START=time.time()
def run(case):
 global TOTAL,FPSDIFF
 name,loc,values,width,count,force,meta,mode,seed,color=case
 blob=fixture(seed,values,force,meta,color);outputs=[]
 for u,entry in (ORIG,CAND):
  u.mem_write(S,bytes(blob));u.mem_write(0x900000,bytes(0x10000));u.mem_write(SCRATCH,bytes.fromhex('123456789abcdef087654321'))
  for j,reg in enumerate(REGS):u.reg_write(reg,0xa0000000+j)
  for j in range(32):u.reg_write(UC_ARM_REG_S0+j,0x3f000000+j)
  u.reg_write(UC_ARM_REG_CPSR,0x10);u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_FPSCR,mode)
  memories=[];fps=[]
  for repeat in range(2):
   for reg,val in zip(REGS,[loc,V,width,count]):u.reg_write(reg,val&0xffffffff)
   u.reg_write(UC_ARM_REG_LR,END)
   try:u.emu_start(entry,END,count=200000)
   except UcError as error:
    FAIL.append({'case':name,'error':str(error),'candidate':entry!=0x206474,'pc':hex(u.reg_read(UC_ARM_REG_PC))});return
   if u.reg_read(UC_ARM_REG_PC)!=END or u.reg_read(UC_ARM_REG_SP)!=SP or [u.reg_read(x) for x in REGS[4:12]]!=[0xa0000000+j for j in range(4,12)]:
    FAIL.append({'case':name,'error':'completion/ABI','candidate':entry!=0x206474});return
   memories += [bytes(u.mem_read(a,n)) for _,a,n in SNAPS];fps.append(u.reg_read(UC_ARM_REG_FPSCR)&0x9f)
  outputs.append((memories,fps))
 TOTAL+=1
 for i,((label,a,n),x,y) in enumerate(zip(SNAPS*2,outputs[0][0],outputs[1][0])):
  if x!=y:
   dif=[j for j,(p,q) in enumerate(zip(x,y)) if p!=q]
   FAIL.append({'case':name,'repeat':i//len(SNAPS),'buffer':label,'differences':len(dif),'first_offset':hex(dif[0]),'original':x[dif[0]//4*4:dif[0]//4*4+4].hex(),'candidate':y[dif[0]//4*4:dif[0]//4*4+4].hex(),'values':[hex(v&0xffffffff) for v in values[:4]]})
 if outputs[0][1] != outputs[1][1]:FPSDIFF+=1

cases=[]
# Every dispatch and default; enum, flag, integer packing and rejected inputs.
values=[0,1,2,3,-1,0x80000000,0x7fffffff,255,256,0x104]+list(range(0x200,0x208))+list(range(0x300,0x304))+[0xb60,0xde1,0x1e01,0x2100,0x2600,0x2601]+list(range(0x2700,0x2704))+[0x6030,0x6048,0x6051,0x6050,0x605e,0x605f,0x6060,0x6061]+list(range(0x609a,0x60a5))+list(range(0x60c0,0x60c3))+list(range(0x60d0,0x60d3))+[0x6210,0x6211]+list(range(0x62b0,0x62b8))+[0x62c0,0x62c8,0x6401,0x6402]+list(range(0x6e00,0x6e04))+[0x812f,0x8370]+list(range(0x84c0,0x84c4))+[0x84e7,0x8513]+list(range(0x8574,0x857a))+list(range(0x8580,0x8586))+[0x86ae,0x86af]
for id in range(310):
 for force in [0,1]:
  for v in values:
   seed=(id*2+force)*1000+(v&0xffffffff)
   cases.append((f'builtin:{id}:{v&0xffffffff:08x}:{force}',0x40000|(id<<2),[v]*8,3,1,force,0,0,seed,None))
# Register writes cover both banks, kinds, counts, and shift-width behavior.
for bank in [0,1]:
 for kind in range(4):
  for count in [-1,0,1,2,5]:
   for base in [0,1,15,31,32,80]:
    for force in [0,1]:
     for element in [0,3,8]:
      meta=(base<<24)|(kind<<13)|(bank<<10)
      vals=[0,1,-1,255,256,0x80000000,0x7fffffff,0x12345678]*16
      cases.append((f'register:{bank}:{kind}:{count}:{base}:{force}:{element}',(2<<7)|element,vals,7,count,force,meta,0,100,None))
# Floating refresh after integer ID233: modes and exceptional color payloads.
edges=[0,0x80000000,1,0x80000001,0x7fffff,0x807fffff,0x800000,0x80800000,0x3f7fffff,0x3f800000,0x3f800001,0xbf800000,0x7f800000,0xff800000,0x7fc00000,0xffc00000,0x7f800001,0xff800001,0x7fffffff,0xffffffff,0x7f7fffff,0xff7fffff]+[fbits(x) for x in [-100,-8,-2,-1.5,-.5,.25,.5,2,4,8,16,32,8192]]
for mode in [0,0x1000000,0x2000000,0x400000,0x800000,0xc00000]:
 for force in [0,1]:
  for v in [0,1]:
   for edge in edges:cases.append((f'color:{mode:x}:{force}:{v}:{edge:08x}',0x40000|(233<<2),[v]*8,1,1,force,0,mode,edge+v,edge))
# Mixed component tuples exercise order and all-or-nothing rejected tuples.
sources=[0x8577,0x6210,0x6211,0x84c0,0x84c1,0x84c2,0x84c3,0x8579,0x8576,0x8578]
colorops=[0x300,0x301,0x302,0x303,0x8580,0x8583,0x8581,0x8584,0x8582,0x8585]
alphaops=[0x302,0x303,0x8580,0x8583,0x8581,0x8584,0x8582,0x8585]
r=random.Random(31337)
for id in range(246,270):
 allowed=sources if id<258 else colorops if id<264 else alphaops
 for force in [0,1]:
  for k in range(100):
   vals=[r.choice(allowed) for j in range(3)]+[0]*5
   cases.append((f'mixed:{id}:{force}:{k}',0x40000|(id<<2),vals,3,1,force,0,0,id*100+k,None))
  for j in range(3):
   vals=[allowed[1]]*8;vals[j]=0xffffffff
   cases.append((f'invalid-component:{id}:{force}:{j}',0x40000|(id<<2),vals,3,1,force,0,0,123,None))
for id in range(289,293):
 for force in [0,1]:
  for pair in [(0x8578,0x8579),(0x8579,0x8578),(0,0x8578),(0x8578,0)]:
   cases.append((f'mixed-pair:{id}:{force}:{pair}',0x40000|(id<<2),list(pair)+[0]*6,2,1,force,0,0,123,None))
for id in [0,37,50,57,193,233,245,269,297,1023,65535]:
 for count in [-1,-2147483648,0,2]:
  cases.append((f'count:{id}:{count}',0x40000|(id<<2),[1]*8,3,count,0,0,0,123,None))
for loc,count in [(0xffffffff,1),(0xffffffff,0),(0,0),(0x40000,0)]:cases.append((f'early:{loc:x}:{count}',loc,[1]*8,1,count,0,0,0,1,None))
if '--quick' in sys.argv:cases=cases[:3000]
for k,case in enumerate(cases):
 run(case)
 if k%1000==0:print('progress',k,'of',len(cases),'failures',len(FAIL),'seconds',round(time.time()-START,2),flush=True)
 if len(FAIL)>30:break
result={'fixture_pairs':TOTAL,'call_pairs':TOTAL*2,'memory_failures':FAIL,'fpscr_exception_differences':FPSDIFF,'seconds':time.time()-START,'object_sha256':OBJECT_SHA}
(OUT/'results.json').write_text(json.dumps(result,indent=2));print(json.dumps(result,indent=2))

```
