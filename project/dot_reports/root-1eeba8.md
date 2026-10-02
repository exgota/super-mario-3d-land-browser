# Shader output command builder 001EEBA8

Frozen base: `54d39734c4edffeeac012157ab71fb11c40d815b`. Lane branch:
`dot/root-1eeba8`. New isolated source:
`lib/CtrSDK/sources/gr_ShaderOutput_1EEBA8.cpp`. No shared header/TU, runtime,
map, ledger, STATE, compiler flag, configuration, tool or binary is submitted.
The complete existing anonymous U interval is 001EEBA8..001EF5AC (2564 bytes),
with pool 001EF560. Original class and public function names remain unknown.

## Role, source ownership and ABI

The only direct caller in the verified EU image is 00220CBC, within the root
00220BD0..00220CE4. That caller parses a DVLB-style program-offset directory,
forms absolute program pointers at object+0C, reads the shared instruction
stream and operand descriptor data, stores incoming vertex/geometry indices at
object+0/+4, passes object+2A4 to this root, and stores the returned command word
count at object+364. This establishes all four argument registers and the
returned write pointer: r0 context, r1 command destination, r2 vertex index,
r3 geometry index, r0 result. Geometry is enabled by the sign of stored
context+4, independently of the passed geometry index. The source preserves
that distinction rather than normalizing either argument.

The root reads eight-byte output descriptors from each program's relative
output table at program+28 with count at +2C. It emits PICA command headers for
shader entrypoints, vertex/geometry output masks and counts, geometry primitive
configuration, seven rasterizer output maps, attribute mode and attribute clock.
There are no game actor, effect, allocation, scene or higher-level rendering
calls. The sole real callee at 00284680..002846A4 copies the four descriptor
halfwords in order and returns its unchanged first argument. It remains an
address-named import on an existing function row; no invented helper identity
or map boundary is introduced.

Independent clean public corroboration is limited to format/register meaning:
[libctru shbin.h](https://github.com/devkitPro/libctru/blob/master/libctru/include/3ds/gpu/shbin.h)
and [registers.h](https://github.com/devkitPro/libctru/blob/master/libctru/include/3ds/gpu/registers.h).
These describe the same eight-byte output descriptor family, output types and
PICA register destinations. The [libctru README](https://github.com/devkitPro/libctru)
identifies a homebrew development library. No implementation from it was copied;
this source was reconstructed from the verified EU dump. No removed SDK,
NintendoWare or sead submodule or leak-derived input was read.

This low-level binary/command serialization role and the independent parser
caller motivate the existing `lib/CtrSDK` module, whose configured ARMCC is
4.0 build902. This does not prove the original library membership, original
class name or original compiler. No compiler choice was made by comparing
function sizes. Original-compiler uncertainty remains explicit.

## Recovered edge behavior

Geometry merge first pairs equal valid descriptor types, then appends unused
geometry and vertex entries. It assigns consecutive output register indices
and retains the selected entry's component mask. The pair loop contains an
observable asymmetric test: it checks vertex entry `i` for validity while
comparing geometry entry `i` with vertex entry `j`. The proposal preserves
that original behavior. It does not replace this test with the apparently
more natural vertex entry `j` test.

Each descriptor independently restarts its component counter. Unsupported
rasterizer types retain 1F bytes; the geometry-input path treats type9 as FF.
After compacting nonempty rasterizer maps, the root fills the remaining slots
from the original uncompressed array indices beginning at the number written.
It does not uniformly replace them with 1F1F1F1F. The command count and mask
underflow behavior for empty outputs is retained. The original performs no
bounds or null checks; caller-valid table storage and destination capacity are
preconditions, not newly inserted policy.

## Final measured result

The retained source is commit `0c529a5c9a461bcdbf53cd5e8df649ba730db911`.
Three meaningful forms were committed before their normal project builds.
All three normal builds link; all three canonical object checks reject the
complete section size. This lane adds **zero exact roots and zero exact bytes**.
The final source remains guarded by `NON_MATCHING`. Main still owns intake,
enrollment and grading. No original compiler, public class identity or general
malformed-input equivalence is claimed.

| Form | Source commit | Complete section | Canonical outcome |
| --- | --- | ---: | --- |
| Direct descriptor copy | c7599c80a6546056ff94aec9b43d7611e0158bfc | 2444 | U -> M, size mismatch |
| Value-copy staging with captured table extent | 163393b088b5936482c7f8a250c47c5a93222f7d | 2468 | U -> M, size mismatch |
| Original odd-prefix and paired read-ahead staging | 0c529a5c9a461bcdbf53cd5e8df649ba730db911 | 2528 | U -> M, size mismatch |

Original complete interval: 2564 bytes. The checker stops at unequal complete
section sizes; no positional differing-byte number or percentage is claimed.
The retained candidate allocates 0x27C stack bytes after its register save,
versus original0x254. Register scheduling, stack placement and copy code remain
different. No fourth form was attempted: no new semantic hypothesis justified
another attempt. This is a three-form handoff, not a falsely reported four-form
cap. No per-function flag, pragma, padding, volatile/register tweak, edited
object, or invented helper address was used.

The actual canonical invocation for each committed form was:

```sh
. ./development_environment.sh
export DEVKITARM=/usr
python make.py eu
python tools/check.py fn_001EEBA8 --object build/eu/obj/lib/CtrSDK/sources/gr_ShaderOutput_1EEBA8.o
```

Only the existing001EEBA8 row's blank Symbol field was temporarily named
`fn_001EEBA8` for the object check. Its boundary, Type and original U rank were
not altered to enroll the candidate before checking. Checker output was:
`U -> M: The complete compiled section, including its literal pool, has a different size from the original interval.`
The exact original map bytes were restored in `finally` after every check.
The existing00284680 row already resolves the address-named import; it needed
no metadata edit. No data/import identity proposal remains pending.

## Full-root bounded replay and limits

Final original-versus-candidate replay executes the entire original root,
including all603 executable instructions outside its two jump tables and
literal pool. The compiled root lives at a separate diagnostic address500000;
the original EU image is never patched and remains read/execute-only during
emulation. Both paths execute the unchanged original00284680 callee. There
are **no modeled callees**. Unicorn ARM1176 is the execution engine; these are
synthetic CPU fixtures, not a hardware, renderer, scene or gameplay test.

1823 returning fixtures compare the complete128KiB fixture memory, full command
buffer and untouched surrounding bytes, return pointer, caller stack above
entry SP, r4-r11, d8-d15, SP and FPSCR. All pass. Every one of the603 original
instructions is reached by the suite. Eighteen additional returning fixtures
vary the stored geometry sign independently of passed vertex/geometry indices
5/9 and preserve an unrelated stored vertex value; all18 pass. These are1841
returning cases total. Coverage of every instruction is not all-input or
all-path proof.

Four negative controls agree on fault access/address/width and retained
fixture state: null context (+4 read), null selected program (+28 read),
out-of-range relative table offset, and the descriptor read-ahead guard page.
A fifth control with countFFFFFFFF has not returned after200 instructions on
either path and retains equal fixture state. This is a bounded execution
observation, not proof of infinite nontermination. The main suite's ordinary
instruction bound is200000; every valid fixture returns before it. Dead callee
stack bytes and permitted caller-saved registers are deliberately excluded.

**The copy-count difference was real and is now resolved.** Forms1/2 each passed
the original1823 returning fixtures and four initial controls. However, the
original entered00284680 exactly5519 times, compared with2687 and5373 for
forms1 and2. A new guard-page control demonstrated why this matters: original
execution reads one descriptor beyond the logical table end and faults, while
form2 returns and writes output. Form1's direct copy likewise lacks the
original read-ahead. Form3 reconstructs the odd first copy, two temporary
records and paired preloading algorithm. With the added boundary control,
**both original and form3 enter the real copy callee5521 times**. The matching
fault is an unmapped two-byte read at01101000. The different instruction counts
(2002996 original,2042133 candidate across1828 main/control cases) are reported,
not used as an equivalence condition.

Bounded inputs are explicit. Returning fixtures use descriptor counts0..16,
merge-loop indices strictly below32, at most49 merged records (the source
array capacity is64), valid aligned program/context/table pointers and ample
non-overlapping command storage. The source uses unchecked scalar `1u << i`
expressions; C++ shifts at or above32 and writes beyond merged[64] would be
undefined. These cases are not established as equivalent to ARM register-shift
or overflow behavior. The merge's vertex[i] validity read can extend beyond
the vertex logical count, and non-merge copying reads one descriptor past its
logical end, so mapped readable backing storage is part of the tested domain.
Descriptor types0..11, register indices0..17, all16 low component masks, varied
high mask bits, modes0/1/2/3/255, duplicate/holey maps, empty outputs, odd/even
counts, and selected FPSCR values are exercised. The three deliberately
malformed pointer controls plus one guard-page and one bounded-count control
cannot imply general malformed-input equivalence. No gameplay or all-input
claim follows.

## Closure and preservation

The final object contains one executable source root. Its two zero-size
`__switch$$` symbols mark jump tables inside that root and are not independent
helpers. Source-local `outputs` is fully eliminated. The sole real function
import is `fn_00284680`; compiler library-request marker symbols do not resolve
additional executable code. Diagnostic linking uses `--no_scanlib` and loads
only the compiled root plus the untouched original callee. The original root
contains its jump tables and command-header literal pool; no new static data,
shared header, global storage or library implementation is submitted. The
object's normal eight-byte .ARM.exidx metadata is not executable root code.

A fresh normal clean baseline build completed at02:44:52 UTC, before the new
source was created. The local canonical gate then checked all731 accepted
roots/all751 actual Game/lib C++ definitions from those frozen objects:
751/751 passed,02:47:38.509952..02:56:27.407773 UTC,528.897854 seconds.
The exact baseline map was restored. This is current54d/731 evidence, not an
older717 gate. The coordinator also independently completed its own pristine
54d baseline; this report relies on the recorded local gate.

The final `python make.py eu -ca` clean build links and finished at03:02:56 UTC.
Exhaustive preservation then finds all177 baseline canonical Game/lib objects
byte-identical and all467 tracked baseline build-input files byte-identical,
with valid project build provenance for all177 objects and the new source
object. Final object bytes equal the form3 object already checked and replayed.
This is **exhaustive object/input equivalence against an actually verified
731-root/all751-definition baseline**, not a second full canonical gate.
No repeated broad gate was run after nothing in those old inputs changed.

The first final-inventory assertion accidentally classified generated
`build/eu/split/stubs.o` as an extra canonical source object, although the
baseline deliberately covered only Game/lib C++ objects. All177 old objects
and467 inputs had already compared equal. The final inventory query now uses
that same Game/lib scope; no real prior-object mismatch was discarded.
Generated stubs remain separately recorded in `generated_stub.json`, with
source/object/provenance hashes and their normal-build compiler identity.
They are compact-link scaffolding and carry no canonical source coverage or
claimed historical byte equality.

Final source/object/checker/toolchain hashes:

```json
{
  "lib/CtrSDK/sources/gr_ShaderOutput_1EEBA8.cpp": "4eba5c2e3f1b926d629a27782608b91189ca419937bbe647266ea38ff9d509f3",
  "build/eu/obj/lib/CtrSDK/sources/gr_ShaderOutput_1EEBA8.o": "15d96733a1b131308227fb617864ce8fbbd65d9604c5f11ed46f5f2be9e8d6f8",
  "tools/check.py": "e177e0abe130e645e75c5f0dc24abb19dbe83310de55f1f099ec8fc385e07317",
  "tools/low/checkExactBytes.py": "aef81a3a21c49bb17b8a19f139d02607899257e04dbe461ece2992cc726fc2e2",
  "tools/low/buildProvenance.py": "343d1a0954aba9a4805e71420be9a99d91185db4c8b329d1363d11efcee53529",
  "data/config.json": "5d41e617eb5b26374ed60b5383a0940ce0ce7d046344f7f815b7564144822c45",
  "data/ver/eu/map.csv": "434ad589b05ee299e967025dc3bc9ace954e547db7f83911fba2dc72daf8eb90",
  "data/ver/eu/code.bin": "e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64",
  "data/compilers/4.0/902/bin/armcc.exe": "e6aca4ebc280a54065406bcc3a0ae3d9f4d0ac5a4032c3a193adb836c72e70fe",
  "data/compilers/4.0/902/bin/armlink.exe": "f511d2d087fe0cad6f604ff774b4d75476a7d4949c100ea082eac03a368e967d",
  "data/compilers/4.1/791/bin/armcc.exe": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d",
  "data/compilers/wibo": "aee836280b3d7c80c2031410219c907c9824c11be6b128d0744655de0f22280b"
}
```

Closure inventory:

```json
{
  "definitions": [
    {
      "name": "__switch$$",
      "size": 0,
      "value": 808,
      "section": "i.fn_001EEBA8"
    },
    {
      "name": "__switch$$",
      "size": 0,
      "value": 1252,
      "section": "i.fn_001EEBA8"
    },
    {
      "name": "fn_001EEBA8",
      "size": 2456,
      "value": 0,
      "section": "i.fn_001EEBA8"
    }
  ],
  "imports": [
    {
      "name": "fn_00284680",
      "kind": "STT_FUNC"
    },
    {
      "name": "Lib$$Request$$armlib",
      "kind": "STT_FUNC"
    },
    {
      "name": "Lib$$Request$$cpplib",
      "kind": "STT_FUNC"
    }
  ],
  "allocated_sections": [
    {
      "name": "i.fn_001EEBA8",
      "size": 2528,
      "flags": 6
    },
    {
      "name": ".ARM.exidx",
      "size": 8,
      "flags": 130
    }
  ]
}
```

## Timing and handoff

Task start02:41:52 UTC. Read/classification/implementation and baseline work
precede first measurement02:56:45.358959..02:56:45.931932 UTC. Form2 measurement
02:58:51.763428..02:58:52.289250; form3 measurement03:00:51.209691..03:00:51.952139.
The final clean build finished03:02:56.086156, and final preservation verification
finished03:04:10.839693 UTC. Report/packet preparation follows those measurements.
The measured task-to-final-preservation interval is22m18.840s, including the
8m48.898s local baseline gate. Attempts:3; accepted complete bytes:0;
accepted throughput:0bytes/hour. No investigation-only size metric is presented
as accepted throughput.

The patch contains only the isolated C++ TU, this report and its follow-up
packet. The branch is based on exact54d and is prepared for coordinator
publication/intake. No function boundary, Type, map, ledger, STATE, build tool,
configuration, binary or shared-source change is included. Final commit/patch
identity and apply-check results are returned in the handoff message, avoiding
a self-referential hash in this report.

## Executed reproduction

The scripts below are report notes, not modifications to project tools. Save
them under the named ignored `build/root_1eeba8/` paths in a worktree at the
frozen54d base. Use the owner's existing private EU files, approved compilers
and venv; verify the stated EU hash. First run a clean baseline build and
`preserve.py` before applying the source proposal. Apply and commit the
intended proposal source, run the normal build, then `measure.py form3`,
`link_replay.py`, `replay.py` and `selector_cases.py`. Finish with a clean
`python make.py eu -ca` and `final_preservation.py`. The reproduction measure
requires the map's original anonymous root row and restores it exactly.
The archived first/second forms are the named commits, not rewritten attempts.

```sh
. ./development_environment.sh
export DEVKITARM=/usr
sha256sum data/ver/eu/code.bin
python make.py eu -ca
python build/root_1eeba8/preserve.py
# Apply the source/report proposal and commit intended source at this point.
python make.py eu
python build/root_1eeba8/measure.py form3
python build/root_1eeba8/link_replay.py
python build/root_1eeba8/replay.py
python build/root_1eeba8/selector_cases.py
python make.py eu -ca
python build/root_1eeba8/final_preservation.py
```

Observed environment: Python3.12.14, Unicorn2.1.4, Capstone5.0.7,
pyelftools0.33. Normal compile flags are the unchanged module/configuration
flags and are captured verbatim in the object's `.provenance.json`.

### build/root_1eeba8/preserve.py

```python
from pathlib import Path
import sys,json,subprocess,hashlib,time,datetime
from elftools.elf.elffile import ELFFile
R=Path.cwd();sys.path.insert(0,str(R))
from tools.low.checkExactBytes import _read_map
OUT=R/'build/root_1eeba8';p=R/'data/ver/eu/map.csv';baseline=p.read_bytes();rows=_read_map(p)
accepted={(r['Symbol'] or f"fn_{r['Start']:08X}"):r for r in rows if r['Rank']=='O' and 'f' in r['Type']}
assert len(accepted)==731,len(accepted)
records=[]; objects={};inputs={}
for obj in sorted((R/'build/eu/obj').rglob('*.o')):
 if not obj.with_suffix('.provenance.json').exists():continue
 provenance=json.loads(obj.with_suffix('.provenance.json').read_text())
 if Path(provenance['source']).suffix not in ('.cpp','.cc','.cxx'):continue
 if not provenance['source'].startswith(('Game/','lib/')):continue
 objects[str(obj.relative_to(R))]=hashlib.sha256(obj.read_bytes()).hexdigest()
 for field in ['source','preinclude']:
  if field in provenance and isinstance(provenance[field],str):
   q=R/provenance[field]
   if q.is_file():inputs[str(q.relative_to(R))]=hashlib.sha256(q.read_bytes()).hexdigest()
 with obj.open('rb') as stream:
  elf=ELFFile(stream);table=elf.get_section_by_name('.symtab')
  if table is None:continue
  for sym in table.iter_symbols():
   if sym.name in accepted and isinstance(sym['st_shndx'],int) and sym['st_shndx'] and sym['st_info']['type']=='STT_FUNC':records.append((sym.name,str(obj.relative_to(R))))
found={s for s,o in records};assert found==set(accepted),set(accepted)-found
print('accepted roots',len(accepted),'actual definitions',len(records),flush=True)
(OUT/'baseline_objects.json').write_text(json.dumps(objects,indent=2))
# All tracked build inputs form a conservative superset of effective inputs.
for path in subprocess.check_output(['git','ls-files','Game','lib','data/config.json','data/ver/eu/config.json','tools','make.py'],text=True).splitlines():
 q=R/path
 if q.is_file():inputs[path]=hashlib.sha256(q.read_bytes()).hexdigest()
(OUT/'baseline_inputs.json').write_text(json.dumps(inputs,indent=2))
checks=[];began=time.monotonic();start=datetime.datetime.now(datetime.timezone.utc).isoformat()
try:
 for i,(sym,obj) in enumerate(records):
  c=subprocess.run([sys.executable,'tools/check.py',sym,'--object',obj],capture_output=True,text=True)
  checks.append(dict(symbol=sym,object=obj,returncode=c.returncode,output=c.stdout+c.stderr))
  if c.returncode or 'O -> O: The complete source-generated function interval matches byte for byte.' not in c.stdout:print('FAIL',sym,obj,c.stdout+c.stderr,flush=True)
  if i%100==99:print('checked',i+1,flush=True)
finally:p.write_bytes(baseline)
summary=dict(base='54d39734c4edffeeac012157ab71fb11c40d815b',accepted_roots=len(accepted),actual_definitions=len(records),passed=sum(c['returncode']==0 for c in checks),checks=checks,map_sha256=hashlib.sha256(baseline).hexdigest(),map_restored=p.read_bytes()==baseline,started_utc=start,finished_utc=datetime.datetime.now(datetime.timezone.utc).isoformat(),seconds=time.monotonic()-began)
(OUT/'baseline_preservation.json').write_text(json.dumps(summary,indent=2)+'\n')
print(json.dumps({k:v for k,v in summary.items() if k!='checks'},indent=2),flush=True)
assert all(c['returncode']==0 and 'O -> O: The complete source-generated function interval matches byte for byte.' in c['output'] for c in checks)
```

### build/root_1eeba8/measure.py

```python
from pathlib import Path
import subprocess,sys,json,datetime,hashlib
from elftools.elf.elffile import ELFFile
R=Path.cwd();O=R/'build/root_1eeba8';tag=sys.argv[1];m=R/'data/ver/eu/map.csv';before=m.read_bytes()
lines=before.decode().splitlines(True)
for i,l in enumerate(lines):
 if l.startswith('0x001EEBA8,'):
  cells=l.rstrip('\n').split(',');assert cells[6]=='';cells[6]='fn_001EEBA8';lines[i]=','.join(cells)+'\n'
try:
 m.write_text(''.join(lines));start=datetime.datetime.now(datetime.timezone.utc).isoformat()
 command=[sys.executable,'tools/check.py','fn_001EEBA8','--object','build/eu/obj/lib/CtrSDK/sources/gr_ShaderOutput_1EEBA8.o']
 c=subprocess.run(command,capture_output=True,text=True)
 obj=R/command[-1]
 with obj.open('rb') as f:
  elf=ELFFile(f);table=elf.get_section_by_name('.symtab');sym=next(s for s in table.iter_symbols() if s.name=='fn_001EEBA8');section=elf.get_section(sym['st_shndx']);size=section['sh_size']
  definitions=[s.name for s in table.iter_symbols() if s['st_info']['type']=='STT_FUNC' and isinstance(s['st_shndx'],int) and s['st_shndx']]
 report=dict(commit=subprocess.check_output(['git','rev-parse','HEAD'],text=True).strip(),source_sha256=hashlib.sha256(Path('lib/CtrSDK/sources/gr_ShaderOutput_1EEBA8.cpp').read_bytes()).hexdigest(),object_sha256=hashlib.sha256(obj.read_bytes()).hexdigest(),canonical_output=c.stdout+c.stderr,returncode=c.returncode,compiled_section_size=size,definitions=definitions,command=command,started_utc=start,finished_utc=datetime.datetime.now(datetime.timezone.utc).isoformat())
 (O/f'{tag}.json').write_text(json.dumps(report,indent=2));print(json.dumps(report,indent=2))
finally:m.write_bytes(before)
```

### build/root_1eeba8/link_replay.py

```python
from pathlib import Path
import subprocess,json,os,sys
R=Path.cwd();O=R/'build/root_1eeba8'
(O/'replay.sym').write_text('#<SYMDEFS>#\n0x00284680 A fn_00284680\n')
(O/'replay.sct').write_text('REPLAY 0x00500000\n{\n CODE 0x00500000\n {\n  * (i.fn_001EEBA8, +FIRST)\n  * (+RO)\n }\n}\n')
command=[str(R/'data/compilers/wibo'),str(R/'data/compilers/4.0/902/bin/armlink.exe'),'--cpu=MPCore','--fpu=VFPv2','--arm_only','--no_exceptions','--inline','--datacompressor=off','--no_debug','--no_scanlib','--mangled','--symbols','--map','--entry=fn_001EEBA8','--keep=fn_001EEBA8',f'--scatter={O}/replay.sct',f'--output={O}/replay.axf',f'--list={O}/replay.map',str(R/'build/eu/obj/lib/CtrSDK/sources/gr_ShaderOutput_1EEBA8.o'),str(O/'replay.sym')]
env=os.environ.copy();env['TMP']='/tmp';c=subprocess.run(command,env=env,capture_output=True,text=True)
(O/'replay_link.json').write_text(json.dumps(dict(command=command,returncode=c.returncode,output=c.stdout+c.stderr),indent=2));print(c.stdout+c.stderr);sys.exit(c.returncode)
```

### build/root_1eeba8/replay.py

```python
from pathlib import Path
import struct,json,hashlib,random,sys
from unicorn import *
from unicorn.arm_const import *
from elftools.elf.elffile import ELFFile
R=Path.cwd(); OUT=R/'build/root_1eeba8'; ROOT=0x1eeba8; CAND=0x500000; STOP=0x600000
MEM=0x1000000; SIZE=0x20000; CTX=MEM; VP=MEM+0x1000; GP=MEM+0x2000; CMD=MEM+0x10000; STACK=0x2010000
binary=Path('data/ver/eu/code.bin').read_bytes()
assert hashlib.sha256(binary).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
with (OUT/'replay.axf').open('rb') as f:
 elf=ELFFile(f); sections=[(s['sh_addr'],s.data()) for s in elf.iter_sections() if s['sh_flags']&2 and s['sh_size']]

def run(case,candidate):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176)
 u.reg_write(UC_ARM_REG_C1_C0_2,0xf00000);u.reg_write(UC_ARM_REG_FPEXC,0x40000000);u.reg_write(UC_ARM_REG_FPSCR,case.get('fpscr',0))
 u.mem_map(0x100000,0x300000,UC_PROT_READ|UC_PROT_EXEC);u.mem_write(0x100000,binary)
 u.mem_map(CAND,0x10000,UC_PROT_READ|UC_PROT_EXEC)
 for a,b in sections:u.mem_write(a,b)
 u.mem_map(STOP,0x1000,UC_PROT_READ|UC_PROT_EXEC);u.mem_map(MEM,SIZE);u.mem_map(0x2000000,0x20000)
 fill=bytes([(case.get('seed',0)*13+0x55)&255])*SIZE
 u.mem_write(MEM,fill);u.mem_write(0x2000000,bytes([0xcc])*0x20000)
 if case.get('guarded_table'):u.mem_map(0x1100000,0x1000)
 def w(a,*x):u.mem_write(a,struct.pack('<'+'I'*len(x),*(i&0xffffffff for i in x)))
 def program(a,entries,geo=False):
  u.mem_write(a,b'\0'*0x30);w(a,0x454c5644);w(a+8,case.get('entry',0x135) + (32 if geo else 0));w(a+0x28,0x100,case.get('bad_count',len(entries)))
  u.mem_write(a+7,bytes([case.get('merge',0) if geo else 0]));u.mem_write(a+0x14,bytes([case.get('mode',0),case.get('start',4),case.get('varcount',2),case.get('count',3)]))
  for i,e in enumerate(entries):u.mem_write(a+0x100+8*i,struct.pack('<4H',*e))
  if case.get('guarded_table') and not geo:
   w(a+0x28,0x1100ff8-a);u.mem_write(0x1100ff8,struct.pack('<4H',*entries[0]))
  if case.get('bad_offset'):w(a+0x28,case['bad_offset'])
 program(VP,case['vertex']);program(GP,case.get('geometry',[]),True)
 vi=case.get('vertex_index',0);gi=case.get('geometry_index',1)
 w(CTX,case.get('stored_vertex',0),case.get('stored_geometry',1 if case.get('geometry_on') else -1),2)
 w(CTX+12+4*vi,VP);w(CTX+12+4*gi,GP)
 if case.get('null_program'):w(CTX+12,0)
 u.reg_write(UC_ARM_REG_R0,0 if case.get('null_context') else CTX);u.reg_write(UC_ARM_REG_R1,CMD)
 u.reg_write(UC_ARM_REG_R2,vi);u.reg_write(UC_ARM_REG_R3,gi)
 u.reg_write(UC_ARM_REG_SP,STACK);u.reg_write(UC_ARM_REG_LR,STOP)
 saved={r:(0xabcdef00+r) for r in [UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11]}
 saved.update({r:0xaabbccdd11220000+r for r in range(UC_ARM_REG_D8,UC_ARM_REG_D15+1)})
 for r,v in saved.items():u.reg_write(r,v)
 pcs=set();calls=0;steps=0;fault=None
 def hook(u,a,s,d):
  nonlocal calls,steps
  steps+=1;pcs.add(a)
  if a==0x284680:calls+=1
 def invalid(u,access,a,s,v,d):
  nonlocal fault
  fault=[access,a,s];return False
 u.hook_add(UC_HOOK_CODE,hook);u.hook_add(UC_HOOK_MEM_INVALID,invalid)
 status='returned'
 try:u.emu_start(CAND if candidate else ROOT,STOP,count=case.get('limit',200000))
 except UcError:status='fault'
 if status=='returned' and u.reg_read(UC_ARM_REG_PC)!=STOP:status='bounded-nontermination'
 result={'status':status,'fault':fault,'fpscr':u.reg_read(UC_ARM_REG_FPSCR),'memory':bytes(u.mem_read(MEM,SIZE)).hex()}
 if status=='returned':
  result.update(returned=u.reg_read(UC_ARM_REG_R0),abi={str(r):u.reg_read(r) for r in saved},sp=u.reg_read(UC_ARM_REG_SP),retained_stack=bytes(u.mem_read(STACK,0x100)).hex())
  assert result['sp']==STACK and all(u.reg_read(r)==v for r,v in saved.items())
 return result,pcs,steps,calls

def fixtures():
 out=[]
 for geo in [False,True]:
  for merge in [0,1]:
   for mode in [0,1,2,3,255]:
    for n in [0,1,2,3,4,7,8,16]:
     entries=[(i%10,i%16,15,0x1234) for i in range(n)]
     out.append(dict(vertex=entries,geometry=entries[:7],geometry_on=geo,merge=merge,mode=mode,count=n,start=13))
 for typ in range(12):
  for mask in range(16):
   for reg in [0,3,6,7,15,16]:
    out.append(dict(vertex=[(typ,reg,mask,0xfefe)],geometry=[],geometry_on=False))
 rng=random.Random(0x1eeba8)
 for i in range(500):
  def desc(n):return [(rng.randrange(12),rng.randrange(18),rng.randrange(65536),rng.randrange(65536)) for _ in range(n)]
  out.append(dict(vertex=desc(rng.randrange(8)),geometry=desc(rng.randrange(8)),geometry_on=True,merge=i%2,mode=rng.choice([0,1,2,3,255]),start=rng.randrange(256),count=rng.randrange(256),entry=rng.randrange(2**32),fpscr=rng.choice([0,0x4000000,0x03000000]),seed=i))
 # The independent merge index discrepancy is exercised with i != j and a type-9 entry at i.
 out += [dict(vertex=[(0,0,15,0),(9,1,15,0)],geometry=[(1,0,15,0),(0,1,15,0)],geometry_on=True,merge=1,mode=2)]
 # Exercise output-map duplication, holes, all component counters and near-capacity merge.
 out += [dict(vertex=[(t,0,m,0) for t in range(10)],geometry_on=False) for m in (0,1,3,5,7,15)]
 out += [dict(vertex=[(0,i,15,0) for i in range(7)],geometry=[(0,i,15,0) for i in range(7)],geometry_on=True,merge=1,mode=m,count=0) for m in (0,1,2)]
 out += [dict(vertex=[(0,6,15,0),(2,2,15,0)],geometry_on=False)]
 controls=[dict(vertex=[],null_context=True),dict(vertex=[],null_program=True),dict(vertex=[(0,0,15,0)],bad_offset=0x7000000),dict(vertex=[(0,0,15,0)],bad_count=0xffffffff,geometry_on=True,merge=1,limit=200)]
 controls.append(dict(vertex=[(0,0,15,0)],guarded_table=True))
 return out,controls

normal,controls=fixtures();fail=[];control_results=[];visited=set();stats={'cases':0,'returned':0,'faults':0,'bounded':0,'actual_copy_calls_original':0,'actual_copy_calls_candidate':0,'original_steps':0,'candidate_steps':0}
for i,c in enumerate(normal+controls):
 a,ap,ast,ac=run(c,False);z,zp,zst,zc=run(c,True);visited|=ap
 stats['cases']+=1;stats['original_steps']+=ast;stats['candidate_steps']+=zst;stats['actual_copy_calls_original']+=ac;stats['actual_copy_calls_candidate']+=zc
 stats[{'returned':'returned','fault':'faults','bounded-nontermination':'bounded'}[a['status']]]+=1
 if i >= len(normal):
  control_results.append({'case':i,'original_status':a['status'],'candidate_status':z['status'],'original_fault':a['fault'],'candidate_fault':z['fault'],'original_steps':ast,'candidate_steps':zst,'equal_retained_state':a['memory']==z['memory'] and a['fpscr']==z['fpscr']})
 if a!=z and i < len(normal):
  keys=[k for k in set(a)|set(z) if a.get(k)!=z.get(k)]
  fail.append({'case':i,'keys':keys,'original_status':a['status'],'candidate_status':z['status']})
  (OUT/f'failure_{i}.json').write_text(json.dumps({'case':c,'original':a,'candidate':z},indent=2))
  print('FAIL',i,keys,flush=True)
  if len(fail)>=5:break
 if i%200==199:print('checked',i+1,flush=True)
summary=dict(stats=stats,normal_fixture_count=len(normal),control_count=len(controls),controls=control_results,failures=fail,original_pcs=sorted(visited),models='none; both roots execute unchanged original 00284680',limits='Synthetic inputs; full fixture memory and retained caller stack compared. Dead callee stack is excluded. Invalid-state paths are bounded, not universal equivalence.')
(OUT/'replay.json').write_text(json.dumps(summary,indent=2));print(json.dumps({k:v for k,v in summary.items() if k!='original_pcs'},indent=2));sys.exit(bool(fail))
```

### build/root_1eeba8/selector_cases.py

```python
from pathlib import Path
import json
ns={};exec(Path('build/root_1eeba8/replay.py').read_text().split('normal,controls=fixtures();')[0],ns)
records=[]
for state in [-2147483648,-1,0,1,31,2147483647]:
 for mode in [0,1,2]:
  c=dict(vertex=[(0,0,15,0),(2,5,15,0)],geometry=[(3,2,3,0)],stored_geometry=state,stored_vertex=-124,vertex_index=5,geometry_index=9,merge=1,mode=mode)
  a,ap,ast,ac=ns['run'](c,False);z,zp,zst,zc=ns['run'](c,True)
  records.append(dict(case=c,equal=a==z,status=a['status'],original_steps=ast,candidate_steps=zst,original_calls=ac,candidate_calls=zc))
assert all(x['equal'] and x['status']=='returned' for x in records)
Path('build/root_1eeba8/selector_cases.json').write_text(json.dumps(records,indent=2));print('selector cases',len(records),'pass')
```

### build/root_1eeba8/final_preservation.py

```python
from pathlib import Path
import hashlib,json,sys,datetime,subprocess
R=Path.cwd();O=R/'build/root_1eeba8';sys.path.insert(0,str(R))
from tools.low.buildProvenance import verify_build_output
h=lambda p:hashlib.sha256(Path(p).read_bytes()).hexdigest()
base=json.loads((O/'baseline_preservation.json').read_text());assert base['passed']==751 and base['map_restored']
assert all(c['returncode']==0 and 'O -> O:' in c['output'] for c in base['checks'])
objects=json.loads((O/'baseline_objects.json').read_text());inputs=json.loads((O/'baseline_inputs.json').read_text())
object_changes=[p for p,d in objects.items() if h(R/p)!=d];input_changes=[p for p,d in inputs.items() if h(R/p)!=d]
assert not object_changes and not input_changes,(object_changes,input_changes)
for p in objects:verify_build_output(R/p)
obj='build/eu/obj/lib/CtrSDK/sources/gr_ShaderOutput_1EEBA8.o';provenance=verify_build_output(R/obj)
measured=json.loads((O/'form3.json').read_text());assert h(R/obj)==measured['object_sha256']
assert h(R/'lib/CtrSDK/sources/gr_ShaderOutput_1EEBA8.cpp')==measured['source_sha256']
assert h(R/'data/ver/eu/map.csv')==base['map_sha256']
new_objects=sorted(str(p.relative_to(R)) for p in (R/'build/eu/obj').rglob('*.o') if p.with_suffix('.provenance.json').exists() and json.loads(p.with_suffix('.provenance.json').read_text())['source'].startswith(('Game/','lib/')) and str(p.relative_to(R)) not in objects)
assert new_objects==[obj],new_objects
hashes={p:h(R/p) for p in ['lib/CtrSDK/sources/gr_ShaderOutput_1EEBA8.cpp',obj,'tools/check.py','tools/low/checkExactBytes.py','tools/low/buildProvenance.py','data/config.json','data/ver/eu/map.csv','data/ver/eu/code.bin','data/compilers/4.0/902/bin/armcc.exe','data/compilers/4.0/902/bin/armlink.exe','data/compilers/4.1/791/bin/armcc.exe','data/compilers/wibo']}
result=dict(base=base['base'],source_commit=subprocess.check_output(['git','rev-parse','HEAD'],text=True).strip(),accepted_roots=731,actual_definitions=751,baseline_canonical_checks=751,unchanged_objects=len(objects),unchanged_tracked_input_files=len(inputs),all_object_provenance_verified=True,new_objects=new_objects,final_object_equals_measured_form3=True,map_restored=True,hashes=hashes,finished_utc=datetime.datetime.now(datetime.timezone.utc).isoformat(),method='Exhaustive object and tracked-input equality against the fresh local 731-root/all751-definition canonical baseline. Not a second full canonical gate.')
(O/'final_preservation.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
```
