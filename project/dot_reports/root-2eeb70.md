# Retail emitter command update: bounded NonMatching proposal

Branch `dot/root-2eeb70`, frozen base `45a4305466a7c4cbd87589169384102e88f8d1b5`. Root interval `002EEB70..002EF5C8`, 2648 complete bytes. This lane began 2026-10-02T01:11:01Z; final evidence was packaged 2026-10-02T01:28:09.044351+00:00. Four source forms were compiled and checked; the working-pass cap is reached. **Zero accepted exact bytes and zero accepted bytes/hour.** This is newly investigated work, with no carried ready inventory.

The final source reconstructs the complete routine as guarded NonMatching C++. It is 2664 compiled bytes, so the canonical checker rejects its extent. The final source passes 901 bounded whole-root original/candidate execution pairs using original callees. None of this changes main coverage, establishes original source identity, or completes gameplay replay. Main advanced to c2e343e/709 roots during the work; preservation below is explicitly only against 45a/707 roots.

## Family and ownership

The coordinator checked STATE, packets, blocked work and reports before selection. The root was U, with no existing report or history. It advances emitter lifetime, fade, random and phase state, prepares twelve four-float parameter blocks, and appends rendering commands. Its direct callees are 002201C4, 002EE878, 0028870C (__aeabi_idivmod), 002F0018, 002EFC44 and 002F00DC. 002EE878 composes two matrices through 0027C79C and emits rendering state; 002EFC44 serializes float parameters and particle commands. These observations support an effect/emitter routine, not an actor lifecycle routine. An aligned immediate-branch/reference scan found no direct caller or absolute pointer to this entry; a concrete public API/class identity is therefore not claimed.

The active 002A451C family owns a 0x1C0 attribute-buffer/ParticleShape factory, called by 00298C58, with separate helpers and data. This root instead uses a state with owner at +0x0C, resource at +0xA8, matrices at +0x18/+0x48, random at +0x7C, fade at +0x84, and lifetime/phase counters at +0x88..+0x98. No translation unit, header, helper or data definition is shared with that lane. The coordinator confirmed disjoint ownership before source work continued.

Only `lib/al/src/Effect/retail_EmitterCommand.cpp` is added. Actor/ARMCC 4.1 build 791 is a **provisional build carrier**, based on the repository's existing effect sources and the observed effect/command call graph. It is not independently proven as this library's original compiler. No 902 comparison or compiler flag experiment was used to improve the score. All four forms use the unchanged project flags, including `--cpu=MPCore --fpmode=fast -O3 -Otime`. No removed library, leaked SDK, external library source, invented function boundary or synthetic object was used.

Existing four-byte data rows 003EF914, 003EF918 and 003EF9C4 are direct literal-load targets for the disposal manager, draw manager and velocity factor respectively. The source imports those unchanged addresses under neutral names; it defines no data there. Function and data names were installed temporarily only on their existing rows for building/checking. The original map bytes were restored exactly; no map, ledger, STATE, tool, configuration or binary is part of the patch.

## Canonical measurements and semantic corrections

| Form | Committed source | Section bytes | Result |
|---|---|---:|---|
| 1 | 6a160c3 | 2644 | Extent rejected; first negative-time replay returned a negative count instead of zero |
| 2 | 81b0c7d | 2672 | Extent rejected; 665 pairs agree before directed rounding exposes two angle bits |
| 3 | b3a5b9f | 2672 | Extent rejected; source parenthesization compiles to the exact same object as form 2 |
| 4 | 258e81a | 2664 | Extent rejected; positive volatile factor prevents constant folding and all 901 bounded pairs agree |

The final canonical command, run after committing C++ and after a fresh `python make.py eu -ca`, was:

```sh
python tools/check.py fn_002EEB70 --object build/eu/obj/lib/al/src/Effect/retail_EmitterCommand.o
```

Its exact text after removing only terminal color controls is:

```text
M -> M: The complete compiled section, including its literal pool, has a different size from the original interval.
```

The normal clean build compiles 45 Game, 130 Actor and one SDK source, links `RE-Pepper.axf` and exports `code.bin`. The strict checker finds one root definition, with all nine imports grounded in existing map rows; there are no emitted source helpers. It rejects by full section size before an exact-byte comparison, so no raw differing-byte count or byte-match percentage is claimed.

Form 1 incorrectly assigned a negative `end - begin` to the output even when no particles were active. The original leaves the initialized output count at zero. Form 2 corrects that behavior.

For angles, ordinary forms 2/3 fold a negative factor into VMUL under configured fast floating-point mode. Retail instead uses VNMUL with a positive factor. Under round-toward-positive-infinity (FPSCR 0x00400000), two negative angle words differ by one bit. Parenthesizing the positive product did not prevent the fold: forms 2 and 3 have the same object hash. Final form 4 materializes the positive factor as a volatile local. **That is an intentional NonMatching semantic workaround; it is not recovered original source and is not byte-match tuning.** It neither changes project flags nor establishes general strict IEEE behavior under fast-mode compilation. The original unmodified ordinary forms and the failing inputs remain reproducible from the commits, hashes and scripts below.

Frozen first counterexample:

```json
{
  "case": 1,
  "group": "finite",
  "params": {
    "frame": -20,
    "time": -20,
    "start": 0
  },
  "fpscr": "0x0",
  "returns": [
    [
      8421424,
      true,
      null,
      16
    ],
    [
      8421424,
      true,
      null,
      16
    ]
  ],
  "differences": [
    [
      "0x804000",
      0,
      238
    ],
    [
      "0x804001",
      0,
      255
    ],
    [
      "0x804002",
      0,
      255
    ],
    [
      "0x804003",
      0,
      255
    ]
  ]
}
```

Frozen directed-rounding counterexample (identical for forms 2 and 3):

```json
{
  "case": 665,
  "group": "fpscr",
  "params": {
    "shape": 3,
    "rotate": 0,
    "override": 0,
    "flags": 256,
    "colorMode": 0
  },
  "fpscr": "0x400000",
  "returns": [
    [
      8421856,
      true,
      null,
      4194320
    ],
    [
      8421856,
      true,
      null,
      4194320
    ]
  ],
  "differences": [
    [
      "0x80813c",
      75,
      74
    ],
    [
      "0x808144",
      198,
      197
    ]
  ]
}
```

## Bounded whole-root execution

Unicorn 2.1.4, ARM1176 model, executes the original EU root and a diagnostic link of the unedited canonical ARMCC object at 00500000. Original image SHA-256 is `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`. The diagnostic relocation is used only for execution, never for canonical acceptance. All reached callees execute original machine code. There are **no modeled function endpoints** or manually assembled replacement instructions.

The final clean-build object passes 901 returning pairs: one base case, 30 finite lifetime, 150 infinite lifetime/phase, 48 fade, 400 parameter combinations, 72 floating-mode controls and 200 deterministic randomized fixtures. Both runs agree on return pointer, output count, the complete 0x100000-byte constructed data arena and 0x110000-byte original global/data region. Every return also preserves SP, r4-r11 and d8-d15. FPSCR is compared with mask 0x0FFFFFFF, preserving controls and cumulative exception status while excluding caller-clobbered comparison NZCV bits.

All 650 instruction addresses in the root execute across the suite. The 12 uncovered words are exactly the seven-word interior literal pool 002EEF3C..002EEF58 and five-word tail pool 002EF5B4..002EF5C8. This is address coverage, not exhaustive path coverage. Direct and nested original callees reached, with invocation counts per side:

```json
{
  "0x2ee878": 884,
  "0x27c79c": 1768,
  "0x28870c": 1968,
  "0x2f0018": 786,
  "0x2efc44": 766,
  "0x220658": 766,
  "0x10b2bc": 766,
  "0x28f0a0": 2352,
  "0x221458": 766,
  "0x2f00dc": 786,
  "0x2201c4": 99
}
```

The fixtures construct valid, aligned, separate state/owner/resource objects, output storage, command buffers and managers. Resource matrices and scalar parameters are finite bounded synthetic values. They vary seeds, frame/time, finite/infinite duration, stop/phase, fade direction, both direction and velocity selectors, flags 0/0x80/0x100/0x180, all three special shape selectors, and alpha thresholds. Infinite cases include stop-derived zero divisors, which execute the original runtime division behavior; this does not claim defined portable C++ division-by-zero semantics. Draw command caches are initialized to a one-word valid entry, so the optional lazy cache generator 0022151C is not exercised. The ordinary texture-cache miss branch is not a tested intake contract. Globals 003EF914/18 and 003EF9C4 are supplied as fixture state rather than inferred live-game values.

The six initial FPSCR settings are 0, FZ, DN and each of the three non-default rounding modes. The suite uses normal finite values, so enabling FZ/DN does not establish subnormal/NaN equivalence. Exceptional floats, arbitrary overflow, out-of-range counts, hostile selectors, aliased input/output objects, invalid pointers, missing lifetime linkage, command-buffer exhaustion and real game initialization are outside the admitted domain. No fault equivalence claim is made. Each run has a 100000-instruction ceiling and all 901 pairs return; that is not a general termination proof. This executes a bounded library routine, not a level, frame replay or renderer.

## Preservation and frozen hashes

The coordinator's pristine 45a baseline clean build and canonical gate passed 707 accepted roots and all 726 actual canonical definitions. Baseline evidence: `build/dot-baseline-45a/report.json` in sibling checkout `mario-main45a`, SHA-256 `42dd622085ca5d98023e5a329bc5d966576fe6c487576bbbc479c60539013dd9`. This lane then compared every one of the 175 pre-existing canonical objects, all 365 recorded inputs, normalized build commands and compiler hashes against that baseline after the final clean build. Every comparison passed. ELF normalization excludes only repository prefixes in non-allocated `.comment`, STT_FILE absolute paths, and resulting raw symbol/string indices; allocated data, other sections, attributes, symbols and resolved relocations all compare equal.

This is **baseline checker plus exhaustive object/input/command equivalence**, not a rerun of the 707/726 canonical checks in the candidate checkout. Main's acceptance gate remains authoritative, including preservation against its newer checkpoint.

Final source SHA-256: `cc6eceeb2c9850da5d6645ee480bb0e56d4622b14b9ef20657bd80b1bde7a557`

Final canonical object SHA-256: `2f4d989dafece3921a0ea16c230a07a8b194f411be8fa8c4d8c9328046c6d019`

Compiler ARMCC 4.1/791 SHA-256: `d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d`

Original/restored map SHA-256: `d517e7aa3eefdbb859e781d7d974f8daf4fbcb33b964edb7d0a5ef6a11db8bd9`

Frozen source and object hashes:

- Form 1: source `a3a29e97632a784642687158d56bb4cafa37836c416274761f4a58b3bdbfa275`, object `dc0eaebb9eeaa169e37550b984ee764a441cf8f9b4148a93dccc455fb22ff6ba`
- Form 2: source `b018f4ed75d1926959a406ad03141b5c31b1b5daf722281b80601d8f35da799b`, object `2045b514e4d3fc4b715efe14a79c94402714a1b02d8f32bf858fc644846a0e74`
- Form 3: source `48e6c33417f3a8952cca5080dbc10974c4122a9be72ce4c896b81cd7f68eae84`, object `2045b514e4d3fc4b715efe14a79c94402714a1b02d8f32bf858fc644846a0e74`
- Form 4: source `cc6eceeb2c9850da5d6645ee480bb0e56d4622b14b9ef20657bd80b1bde7a557`, object `2f4d989dafece3921a0ea16c230a07a8b194f411be8fa8c4d8c9328046c6d019`

## Executed reproduction

Use the owner's approved local binary/toolchain and the project virtual environment. No binary or source from an excluded SDK is part of this report. The source must be committed before the canonical project build/check. The following temporary naming procedure was run from the branch checkout; it changes only names and the root's scratch rank on already existing rows:

```python
from pathlib import Path
p = Path('data/ver/eu/map.csv')
original = p.read_bytes()
Path('build/root2eeb70').mkdir(parents=True, exist_ok=True)
Path('build/root2eeb70/map-original.csv').write_bytes(original)
names = {0x2eeb70:'fn_002EEB70',0x2201c4:'fn_002201C4',
  0x2ee878:'fn_002EE878',0x2efc44:'fn_002EFC44',0x2f0018:'fn_002F0018',
  0x2f00dc:'fn_002F00DC',0x3ef914:'dat_003EF914',0x3ef918:'dat_003EF918',
  0x3ef9c4:'dat_003EF9C4'}
lines = []
for line in original.decode().splitlines(True):
    row = line.rstrip('\n').split(',')
    try: address = int(row[0], 16)
    except ValueError:
        lines.append(line)
        continue
    if address in names:
        assert not row[6].strip()
        row[6] = names[address]
        if address == 0x2eeb70: row[4] = 'M'
        line = ','.join(row) + '\n'
    lines.append(line)
p.write_text(''.join(lines))
```

Executed shell commands, with the Python blocks below saved under their indicated ignored `build/root2eeb70/` filenames:

```sh
. ./development_environment.sh
export DEVKITARM=/usr
sha256sum data/ver/eu/code.bin
python make.py eu -ca
python tools/check.py fn_002EEB70 --object build/eu/obj/lib/al/src/Effect/retail_EmitterCommand.o
python tools/low/checkExactBytes.py fn_002EEB70 build/eu/obj/lib/al/src/Effect/retail_EmitterCommand.o --compiler 4.1/791 > build/root2eeb70/form1-result.json
python build/root2eeb70/link_replay.py
python build/root2eeb70/replay.py
python build/root2eeb70/preserve.py
cp build/root2eeb70/map-original.csv data/ver/eu/map.csv
cmp build/root2eeb70/map-original.csv data/ver/eu/map.csv
```

`form1-result.json` is the import-address input filename used by the link script; all forms have identical import identities. The check result for the selected form remains distinct. Restore the map in a `finally`/shell cleanup step as well if a replay of an earlier form stops at its preserved failure. Forms 1/2/3 are available at 6a160c3/81b0c7d/b3a5b9f respectively, with their exact hashes above. Run each as committed source in its own checkout. The first two failing test locations are deterministic because the fixture RNG is seeded.

### link_replay.py

```python
from pathlib import Path
import json,subprocess,hashlib
out=Path('build/root2eeb70')
imports=json.loads((out/'form1-result.json').read_text())['evidence']['imports']
(out/'imports.sym').write_text('#<SYMDEFS>#\n'+''.join('0x%08X %s %s\n'%(i['address'],i['kind'],i['symbol']) for i in imports))
(out/'behavior.sct').write_text('ROOT_LOAD 0x00500000\n{\n ROOT_CODE 0x00500000 { retail_EmitterCommand.o (i.fn_002EEB70, +FIRST) }\n}\n')
command=['data/compilers/wibo','data/compilers/4.1/791/bin/armlink.exe','--scatter',str(out/'behavior.sct'),'--entry','fn_002EEB70','--no_scanlib','--no_remove','--output',str(out/'behavior.axf'),'--map','--list',str(out/'behavior.map'),'build/eu/obj/lib/al/src/Effect/retail_EmitterCommand.o',str(out/'imports.sym')]
r=subprocess.run(command,text=True,capture_output=True);print(r.stdout,r.stderr);r.check_returncode()
(out/'link-command.json').write_text(json.dumps(command,indent=2))
```

### replay.py

```python
import sys,json,struct,random,time,hashlib
from pathlib import Path
from collections import Counter
from elftools.elf.elffile import ELFFile
from unicorn import *
from unicorn.arm_const import *
ROOT=0x2eeb70;END=0x2ef5c8;CAND=0x500000;STOP=0x700000
DATA=0x800000;SIZE=0x100000;STATE=DATA;OWNER=DATA+0x1000;RES=DATA+0x2000;OUT=DATA+0x4000;CMD=DATA+0x8000;DISPOSE=DATA+0x10000;DRAW=DATA+0x20000;CONTEXT=DATA+0x70000;SP=0xa08000
out=Path('build/root2eeb70');code=Path('data/ver/eu/code.bin').read_bytes();elf=ELFFile(open(out/'behavior.axf','rb'));candidate=elf.get_section_by_name('ROOT_CODE').data()
coverage=set();calls=[Counter(),Counter()];faults=[None,None];machines=[]
callees=[0x2201c4,0x2ee878,0x2efc44,0x2f0018,0x2f00dc,0x27c79c,0x28870c,0x220658,0x10b2bc,0x22151c,0x221458,0x28f0a0]
for j in range(2):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176);u.mem_map(0x100000,0x400000);u.mem_write(0x100000,code);u.mem_map(CAND,0x10000);u.mem_write(CAND,candidate);u.mem_map(STOP,0x1000);u.mem_map(DATA,SIZE);u.mem_map(0xa00000,0x10000)
 def hook(uc,address,size,user,j=j):
  if j==0 and ROOT<=address<END:coverage.add(address)
  if address in callees:calls[j][hex(address)]+=1
 u.hook_add(UC_HOOK_CODE,hook)
 def fault(uc,access,address,size,value,user,j=j):faults[j]=(access,address,size);return False
 u.hook_add(UC_HOOK_MEM_INVALID,fault);machines.append(u)
regs=[UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11]
vregs=[UC_ARM_REG_D8+i for i in range(8)];saved=[0xabc00000+i for i in range(8)];vsaved=[0x7fe1234500000000+i for i in range(8)]
rng=random.Random(0x2eeb70);cases=0;groups=Counter();begin=time.monotonic()
def fixture(seed=0,frame=0,time=0,duration=20,start=0,spacing=2,batch=3,life=8,stop=-1,phase=0,fade=0.5,fadeStep=0.01,fading=0,capture=0,override=0,rotate=0,flags=0,shape=0,colorMode=0,drawMode=2):
 b=bytearray(SIZE)
 def w(addr,off,val):struct.pack_into('<I',b,addr-DATA+off,val&0xffffffff)
 def f(addr,off,val):struct.pack_into('<f',b,addr-DATA+off,val)
 def byte(addr,off,val):b[addr-DATA+off]=val
 w(STATE,0,frame);w(STATE,0xc,OWNER);w(STATE,0xa8,RES);w(STATE,0x7c,seed);f(STATE,0x84,fade);w(STATE,0x88,stop);w(STATE,0x8c,phase);w(STATE,0x90,2);w(STATE,0x94,time);w(STATE,0x98,seed%3)
 w(OWNER,4,1);byte(OWNER,0x1cc,fading);byte(OWNER,0x1cd,override)
 for off in (0x170,0x174,0x178,0x17c,0x18c,0x190,0x194,0x19c,0x1a0,0x1a4):f(OWNER,off,rng.uniform(0.1,2.0))
 for off in range(0x1a8,0x1c0,4):f(OWNER,off,rng.uniform(-3,3))
 for addr,off in [(OWNER,0xfc),(OWNER,0x12c),(RES,0x170),(RES,0x1a0)]:
  for i in range(12):f(addr,off+i*4,rng.uniform(-1,1))
 for off in [0x4c,0x50,0x54,0x58,0x5c,0x60,0x64,0x68,0x6c,0x8c,0x9c,0xa0,0xa4,0xa8,0xac,0xb0,0xb8,0xbc,0xc0,0xc8,0xcc,0xd0,0xd8,0xdc,0xe0,0xf4,0xf8,0xfc,0x108,0x10c,0x110,0x118,0x128]:f(RES,off,rng.uniform(-2,3))
 w(RES,4,flags);w(RES,8,seed);byte(RES,0x2f,rotate);byte(RES,0x33,capture)
 for off,val in [(0x70,start),(0x74,duration),(0x78,spacing),(0x80,batch),(0x84,life),(0x88,3),(0xe4,10),(0xe8,40),(0xec,80),(0xf0,3),(0x100,colorMode),(0x104,80),(0x120,20),(0x124,70),(0x12c,shape),(0x140,rng.getrandbits(32)),(0x14c,rng.getrandbits(32)),(0x158,rng.getrandbits(32)),(0x164,rng.getrandbits(32))]:w(RES,off,val)
 f(RES,0x1d0,fadeStep);w(DISPOSE,0x1c,STATE);w(DISPOSE,0x864,1);w(DRAW,0,CONTEXT);w(CONTEXT,0x60768,drawMode)
 for idx in range(3):
  w(DRAW,0x371e4+idx*1936,1);w(DRAW,0x371e8+idx*1936,0x12340000+idx)
 w(DRAW,0x398a4,0x56789abc);w(DRAW,0x39aa4,4)
 return b

def run(group,params={},fpscr=0,patch=None,expected_fault=False):
 global cases
 b=fixture(**params)
 if patch:patch(b)
 results=[]
 for j,u in enumerate(machines):
  u.mem_write(DATA,bytes(b));u.mem_write(0x3b0000,code[0x2b0000:]+bytes(0x500000-(0x100000+len(code))));u.mem_write(SP-0x2000,b'\x7e'*0x3000);faults[j]=None
  u.mem_write(0x3ef914,struct.pack('<II',DISPOSE,DRAW));u.mem_write(0x3ef9c4,struct.pack('<f',0.75))
  for reg,v in zip(regs+vregs,saved+vsaved):u.reg_write(reg,v)
  u.reg_write(UC_ARM_REG_FPEXC,0x40000000);u.reg_write(UC_ARM_REG_FPSCR,fpscr);u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,STOP)
  for reg,v in [(UC_ARM_REG_R0,0),(UC_ARM_REG_R1,CMD),(UC_ARM_REG_R2,STATE),(UC_ARM_REG_R3,OUT)]:u.reg_write(reg,v)
  error=None
  try:u.emu_start(ROOT if j==0 else CAND,STOP,count=100000)
  except UcError as exc:error=str(exc)
  returned=u.reg_read(UC_ARM_REG_PC)==STOP
  if returned:
   assert u.reg_read(UC_ARM_REG_SP)==SP,(cases,j,'SP')
   assert [u.reg_read(r) for r in regs+vregs]==saved+vsaved,(cases,j,'saved')
  if not returned and not error:raise AssertionError((cases,group,j,'nontermination',hex(u.reg_read(UC_ARM_REG_PC))))
  result=(bytes(u.mem_read(DATA,SIZE)),bytes(u.mem_read(0x3e0000,0x110000)),u.reg_read(UC_ARM_REG_R0) if returned else None,returned,faults[j],u.reg_read(UC_ARM_REG_FPSCR)&0x0fffffff)
  results.append(result)
 if results[0]!=results[1]:
  differences=[]
  for region,start in [(0,DATA),(1,0x3e0000)]:
   differences.extend((hex(start+i),x,y) for i,(x,y) in enumerate(zip(results[0][region],results[1][region])) if x!=y)
  failure={'case':cases,'group':group,'params':params,'fpscr':hex(fpscr),'returns':[r[2:] for r in results],'differences':differences[:50]}
  (out/'failure.json').write_text(json.dumps(failure,indent=2));raise AssertionError(failure)
 if bool(faults[0])!=expected_fault:raise AssertionError(('fault expectation',group,faults[0]))
 cases+=1;groups[group]+=1

run('base')
for frame in [-20,-1,0,1,5,10,20,28,29,100]:
 for start in [0,5,15]:run('finite',{'frame':frame,'time':frame,'start':start})
for phase in range(5):
 for stop in [-1,0,3,20,50]:
  for tm in [0,3,15,25,40,100]:run('infinite',{'duration':0x7fffffff,'phase':phase,'stop':stop,'time':tm,'frame':tm})
for fading in [0,1]:
 for fade in [0.0,0.001,0.5,1.0,1.5,-0.5]:
  for step in [0.0,0.01,-0.01,1.0]:run('fade',{'fade':fade,'fadeStep':step,'fading':fading,'capture':1})
for shape in [0,3,8,13,9]:
 for rotate in [0,1]:
  for override in [0,1]:
   for flags in [0,0x100,0x80,0x180]:
    for alpha in [-100,0,1,2,20]:run('parameters',{'shape':shape,'rotate':rotate,'override':override,'flags':flags,'colorMode':alpha,'seed':rng.getrandbits(32),'drawMode':0})
for mode in [0,1<<24,1<<25,1<<22,2<<22,3<<22]:
 for i in range(12):run('fpscr',{'shape':3,'rotate':i%2,'override':i%2,'flags':0x100,'colorMode':i},fpscr=mode)
for i in range(200):
 tm=rng.randrange(0,100);run('random',{'frame':tm,'time':tm,'duration':rng.choice([10,50,0x7fffffff]),'start':rng.randrange(-10,10),'spacing':rng.randrange(1,10),'batch':rng.randrange(1,12),'life':rng.randrange(1,30),'stop':rng.choice([-1,5,15,40]),'phase':rng.randrange(5),'fade':rng.uniform(-0.1,1.1),'fadeStep':rng.uniform(-0.1,0.1),'fading':i%2,'capture':i%3==0,'override':i%2,'rotate':i%3==0,'flags':rng.choice([0,0x100,0x80,0x180]),'shape':rng.choice([0,3,8,13]),'seed':rng.getrandbits(32)})
result={'all_equal':True,'cases':cases,'groups':dict(groups),'seconds':time.monotonic()-begin,'original_instruction_coverage':len(coverage),'uncovered':[hex(a) for a in range(ROOT,END,4) if a not in coverage],'original_callee_counts':calls[0],'candidate_callee_counts':calls[1],'modeled_endpoints':[],'source_sha256':hashlib.sha256(Path('lib/al/src/Effect/retail_EmitterCommand.cpp').read_bytes()).hexdigest(),'object_sha256':hashlib.sha256(Path('build/eu/obj/lib/al/src/Effect/retail_EmitterCommand.o').read_bytes()).hexdigest()}
(out/'replay-result.json').write_text(json.dumps(result,indent=2));print(json.dumps(result,indent=2))
```

### preserve.py

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection
import json,hashlib
root=Path.cwd();base=root.parent/'mario-main45a';out=root/'build/root2eeb70'
def normalized(p):
 e=ELFFile(open(p,'rb'));sections=[];symbols=[]
 def symrec(s):
  n=s['st_shndx'];sec=e.get_section(n).name if isinstance(n,int) else n
  return [s.name,s['st_info']['type'],s['st_info']['bind'],s['st_other']['visibility'],sec,s['st_value'],s['st_size']]
 for sec in e.iter_sections():
  if isinstance(sec,RelocationSection):
   tab=e.get_section(sec['sh_link']);sections.append([sec.name,[(r['r_offset'],r['r_info_type'],symrec(tab.get_symbol(r['r_info_sym']))) for r in sec.iter_relocations()]])
  elif sec['sh_type'] not in ('SHT_SYMTAB','SHT_STRTAB','SHT_NULL'):
   sections.append([sec.name,sec['sh_type'],sec['sh_flags'],sec['sh_addralign'],sec['sh_entsize'],hashlib.sha256(sec.data().replace(str(root).encode(),b'<repo>').replace(str(base).encode(),b'<repo>') if sec.name=='.comment' else sec.data()).hexdigest()])
 tab=e.get_section_by_name('.symtab')
 for s in tab.iter_symbols():
  if s['st_info']['type']!='STT_FILE':symbols.append(symrec(s))
 return {'sections':sections,'symbols':symbols}
records=[];inputs={}
for p in sorted((root/'build/eu/obj').rglob('*.o')):
 rel=p.relative_to(root/'build/eu/obj')
 if rel.parts[0] not in ('Game','lib') or p.stem=='retail_EmitterCommand':continue
 q=base/'build/eu/obj'/rel;pa=json.loads(p.with_suffix('.provenance.json').read_text());pb=json.loads(q.with_suffix('.provenance.json').read_text())
 assert pa['inputs']==pb['inputs'],str(rel)+' inputs'
 for key,value in pa['inputs'].items():
  assert hashlib.sha256((root/key).read_bytes()).hexdigest()==value
  assert hashlib.sha256((base/key).read_bytes()).hexdigest()==value
  inputs[key]=value
 assert pa['compiler_sha256']==pb['compiler_sha256'],str(rel)+' compiler'
 assert [x.replace(str(root),'<repo>') for x in pa['command']]==[x.replace(str(base),'<repo>') for x in pb['command']],str(rel)+' command'
 a=normalized(p);b=normalized(q)
 if a!=b:
  (out/'preserve-failed.json').write_text(json.dumps({'object':str(rel),'current':a,'baseline':b},indent=2));raise AssertionError(str(rel)+' ELF')
 records.append({'object':str(rel),'current_object_sha256':hashlib.sha256(p.read_bytes()).hexdigest(),'base_object_sha256':hashlib.sha256(q.read_bytes()).hexdigest(),'normalized_elf_sha256':hashlib.sha256(json.dumps(a,sort_keys=True).encode()).hexdigest(),'compiler_sha256':pa['compiler_sha256']})
result={'base':'45a4305466a7c4cbd87589169384102e88f8d1b5','object_count':len(records),'input_count':len(inputs),'all_equal':True,'excluded_difference':'STT_FILE absolute source path, repository prefix in non-allocated .comment, and resulting raw symbol/string table indices; all other symbols, section data, attributes and resolved relocations compare equal','objects':records,'inputs':inputs}
(out/'preservation.json').write_text(json.dumps(result,indent=2));print(json.dumps({k:v for k,v in result.items() if k not in ('objects','inputs')},indent=2))
```

## Intake scope and stopping condition

Apply the one new source plus this report unchanged to the named frozen base. No shared-header composition is needed. The complete proposed root is 2648 original bytes, with zero accepted bytes; all helpers remain original imports. Its raw-state class names are local implementation descriptions and must not be adopted as established external ABI identities. Four unsuccessful canonical forms complete this pass. Further byte work requires a later eligible pass after other work and fresh overlap checking. The final source is useful only as a bounded, explicitly NonMatching reconstruction until the authoritative gate proves otherwise.
