# Skeletal transform blend root 0033BA3C

Branch `dot/root-33ba3c` starts at main `3de056e0dcb33619c32f8b46ef64f74c90d28934`. This is a guarded ordinary-C++ **NonMatching proposal with zero exact-byte credit**. Its existing U interval is `[0033BA3C,0033C7C0)`, 3,460 bytes. No map, rank, ledger, STATE, configuration, tool or binary change is included.

## Measured status

The project build compiles the committed source, archives, links and exports. The compact build keeps the root U, so that successful link does not establish inclusion of this candidate in the compact game image. The canonical check cannot grade it: the original identity-matrix storage at 00430C68 has no map row, and the unchanged checker rejects its unresolved non-branch data relocation. No diagnostic import is presented as canonical metadata or acceptance.

Two meaningful source forms were tried. Form 1 (`8519586`, with declaration assertions added in `f2f25e3`) emitted one 3,276-byte function section. Its first 2,445 fixtures/4,890 repeated-call pairs passed, but expanded testing found a real floating-exception difference: with cached weight bits 00000001, FZ enabled, mode 0 and normalization disabled, retail sets FPSCR.IDC while the candidate does not. The 902 compiler had eliminated subtraction by zero in the inline zero-vector predicate. Memory and return values agreed; that did not make the floating state equal.

Form 2 (`6db5ac3`) calls the independently verified original near-zero helper 00215E20 for that predicate. The helper performs the required subtraction and absolute-bit comparison. This is an ordinary call to a real existing callee, not a new helper address, volatile access, padding, assembly or compiler flag change. It emits one 3,256-byte function section, 204 bytes shorter than the complete retail interval. All source helpers inline into that section; no candidate helper address is added. Final committed-source canonical checking still rejects the missing data import. The complete bounded replay passes **2,685 returning fixtures / 5,370 repeated-call pairs**, plus five separately labeled bounded nonreturning pairs. The corrected source is `6db5ac3`; its header assertions are `f2f25e3`.

## Placement and compiler assumption

The binary establishes a skeletal-animation library role. Constructor 001C440C contains the string `SkeletalAnimation`, allocates the blend object, installs its vtable, and connects its model context. The root operates on calculated transforms and calls the existing named `nw::gfx::CalculatedTransform::UpdateTranslateFlags` and `UpdateCompositeFlags` rows. None of that establishes which compiler emitted this root.

The source is placed in the currently configured `lib/CtrSDK` source module, which builds with ARMCC 4.0/902. This is an explicit build-placement assumption, not a claim that the root belongs to nn::CtrSDK or that NintendoWare used 902. There is no configured clean NintendoWare module in this snapshot. The configured al/game 791 module is an **untested, evidence-dependent alternative**. No alternate compiler, module, version or flags were probed to improve this candidate's bytes.

## Executable ranges and independent object evidence

Addresses and object offsets in the evidence below are hexadecimal unless an explicit byte count is given.

The old pool marker at 0033BE3C is internal. The complete interval contains 853 executable ARM instructions/3,412 bytes and 48 bytes of constants:

- Code 0033BA3C..0033BE3C, constants 0033BE3C..0033BE58
- Code 0033BE58..0033C6AC, constants 0033C6AC..0033C6C0
- Code 0033C6C0..0033C7C0

The only word equal to the root entry in the owner image occurs at 003D8518, slot 18 of the table starting 003D8500. Constructor 001C440C independently requests 0x44 (68) bytes at 001C4594, initializes the arrays, writes the mode bytes at 001C462C/34/38, and installs that exact vtable at 001C463C. Its defaults are componentMode 0, dirty 0, normalize 1. The table's slot 0C is 00298878, which writes its argument to object+8, independently establishing the model-context field. The proposal's 68-byte root view therefore has allocation evidence beyond its own field reads.

Descriptive views in `lib/CtrSDK/include/clean/TransformBlendRoot.h` preserve unknown bytes. They are not asserted original class names. The root observes children at 14..18, raw weights 24..28, cached weights 34 and control bytes 40..42. Model+8 is a resource pointer, +10 a policy-pointer array and +54 a bind-pose-pointer array. The resource's self-relative 14 field leads to a dictionary; its entry at 0x28+0x10*index has another self-relative pointer. Record+0C selects the policy. Tests construct valid records; invalid-resource fault timing is not claimed.

The 64-byte calculated-transform view is a 48-byte row-major 3x4 matrix, three scale floats at 30..38, then flags at 3C. Child evaluator slots 8/18/1C are type/evaluate/availability; a type-specific evaluator supplies three disable bytes at 5B..5D. The root's linked type identity is the existing four-byte object 003F151C. No original class name is invented for it.

The actual child table 003D8414 independently supplies type 0033B6D4, evaluate 0033B99C and availability 0033B44C. The evaluate thunk sets r3=1 and branches to the existing 0033B4AC body. Its cached path copies a 64-byte transform from child+64 using the index table at+14; its default-pose path uses child+8's model and existing math/flag helpers. Both paths run unchanged in the replay.

Actual policy tables 003D84C4 and 003D8550 pair blend/finish functions 0033B9D0/0033BA20 and 0033C7D0/0033C820. Both pairs, including every math callee they reach, run from the unchanged owner executable. A constructed nested blend object uses the actual 003D8500 table: the original outer root acts as the caller, and only the nested receiver's 0033BA3C entry is redirected. This is a bounded real virtual-dispatch caller test, not evidence of a game scene or full animation initialization.

## Data identity and canonical blocker

The root imports existing mapped objects 003F151C,003F389C,003B6694 and 003A48F4. The component-disable constant at 003B6694 is **minus one**, a sentinel understood by the original blend helpers, not zero. The consumed trigonometric table prefix is 256 four-float records/4,096 bytes, while its existing whole map interval is 4,160 bytes. The declaration remains unsized; the extra 64 bytes are not identified, reconstructed or claimed by this work.

The identity-matrix storage 00430C68 is absent from the map. Its guard is the existing four-byte 003F389C object. An independent constructor at 001E4788 uses the guard at 001E48B0, initializes 12 matrix words at 001E48D8..001E4908, and passes the matrix to the independent 48-byte copy 00291470 at 001E491C. It repeats the same matrix use for a second field at 001E4988. The loaded constants at 001E47B4/B8 establish the diagonal ones and off-diagonal zeros. Other consumers refer to separate nearby matrix storage 00430C38 and 00430C98; for example 002F3Axx accesses 00430C98 as a 64-byte matrix. These observations establish the 48-byte identity-matrix extent **used by these consumers**, not a complete original allocation boundary or symbol identity. No new BSS row is proposed as accepted evidence.

Consequently the diagnostic linker binds `dat_00430C68` explicitly to that observed address, but the canonical checker is allowed to reject it. A future whole-data identity decision belongs to the main lane. Nothing in the diagnostic binding bypasses or changes that gate.

## Recovered behavior

Component mode scans available nonnull children with absolute weight bits above the 0.001f tolerance. It sums weights independently for enabled scale/rotation/translation components. If all sums are near zero, it returns null before changing the output. Each remaining sum gets reciprocal normalization unless it is near zero or one. Children are then visited in reverse order; masked components receive the minus-one sentinel. A child can return no transform, the scratch transform, or another transform pointer. Flag intersection and first-matrix capture use the scratch transform even when the child returns a different pointer; the blend consumes the returned pointer.

Shared-weight mode first requires at least one child reporting the index available, independently of its weight. When both dirty and normalize are set, it normalizes the entire raw-weight span into cached weights and clears dirty. Reverse traversal uses cached weights. A missing child or a null evaluation result produces a transform from the model's scale/rotation/translation bind pose, using the owner-local trigonometric table and original flag-update helpers. Near-zero cached weights skip evaluation and fallback entirely.

Both modes temporarily set output flags 4000001C, preserve the caller's 40000000 state, fold transform flags 7E0 and optionally finalize through the policy. Component mode additionally restores the original 1C bits and returns null if no child evaluation succeeds. Shared mode can return the output even when every cached weight was skipped. The first eligible type-specific rotation matrix is retained for the original failed-finalization repair 002A52EC.

Rotation uses signed angle scaling by the binary32 factor 4222F983, absolute magnitude, repeated subtraction of 65536, low 16-bit truncation, low 8-bit table selection and multiply/add interpolation. It does not impose a new validity clamp. Exceptional angles can remain in the original reduction loop; these are measured separately from returning fixtures.

## Final measurements

- Source SHA256: `a274a0145af79ae056ae084404f7f3bf9b1ec85b354ed9955fb686ce17f0cbb6`
- Header SHA256: `2fb0536fe909e796fa0243e981fbb38ef7bc30883f91518897c514fd0f1ef75c`
- Project canonical object SHA256: `31571f740901efbf5a57685cadb76bd9030e1e763a4303a1852fbea420cac524`
- Original executable SHA256: `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`

Returning fixture groups, each run twice:

- 500 modeled child-result/null/external-result and flag cases
- 1,000 actual cached-child cases across the two observed policy pairs
- 200 actual default-pose child cases
- 200 nested blend cases with the original outer root as the caller
- Four degenerate-matrix finalization controls and one explicit wrapped-angle control
- 360 exceptional weight/tolerance cases under default, FZ, DN and each nondefault rounding mode, including normalized and unchanged cached weights
- 120 exceptional scale/translation field cases
- 300 deterministic fallback-rotation cases with finite angles up to magnitude 50,000

Actual-child fixtures total 1,760; modeled-child fixtures total 925. All 2,685 compare complete root returns. Policy blend calls total 11,084 per machine; original near-zero helper calls intentionally differ, 8,865 versus 27,142, because the corrected candidate calls it for the formerly inline predicate. The other recorded original callee counts agree. Counts include the five bounded nonreturn probes where applicable.

Five separate angle probes use +infinity, -infinity, qNaN, sNaN and maximum finite binary32 in the first Euler component. Both machines remain in their reduction loops after 50,000 instructions. Observed exception bits are respectively 0, 0, 1, 1 and 0x14; output flags are 4000001C. These are bounded nonreturn observations, not completed replay pairs or a general exceptional-angle equivalence proof.

## Verification scope

The replay starts at the whole root entry and compares after normal return, not at isolated slices. Both machines use ARM1176/VFPv2 and the verified entire EU executable. The candidate is a diagnostic armlink of the unchanged project-built canonical object at 00500000. Only entry dispatch changes; no target or object instruction byte is patched.

Each returning fixture runs twice without resetting its working memory or globals between calls. It compares all 64KiB of constructed working memory, output status, the 48-byte matrix global, its guard, callback-visible arguments/results and cumulative floating exception bits 9F. It checks r4..r11, d8..d15 and SP canaries. The second call covers dirty-cache clearing, repeated accumulation and one-time matrix initialization.

The actual-child groups execute every callee unmodified. In the modeled-child groups, only entry 0033B4AC is intercepted: the bounded evaluator contract writes a supplied transform or returns null, optionally returns an external transform, and preserves the incoming 40000000 bit when writing scratch output. Type queries, availability getters, policy blending, finalization and math/flag helpers still execute original instructions. That modeled child behavior is not validation of 0033B4AC itself.

Coverage reaches 851 of 853 executable root instructions. The two remaining addresses 0033BF7C and 0033C6A8 are redundant child-null branches after earlier nonnull checks; the child pointer is held in a preserved register and cannot become null under the tested ABI contract. The script asserts the complete expected address set minus exactly these two words, and asserts no literal-pool word is executed. This is control-flow coverage, not all-input equivalence.

The fixtures do not establish general pointer overlap, invalid-pointer/fault timing, concurrent mutation, floating trap-enabled execution, arbitrary custom evaluators, all angle magnitudes, full game initialization, animation asset correctness, rendering, gameplay or native-port replay. Finite fallback angles are bounded to the generated values, including magnitude 50000. Weight and scale/translation exceptional controls are separate from nonreturning angle probes.

## Reproduction

Provide the owner-local code.bin/exh.bin, approved wibo/ARMCC 902 and the repository's normal Python dependencies, plus Unicorn. No game data or generated object is embedded below. Run from this branch's repository root. The initial measured build started with an absent build directory; the normal build compiled 44 Game, 128 al and 2 SDK sources and linked/exported. A final `python make.py eu -ca` also compiles, links and exports successfully; the candidate object SHA remains identical. Later source/header revisions used the same unmodified project build step.

```sh
. ./development_environment.sh
export DEVKITARM=/usr
TMP=/tmp python make.py eu
mkdir -p build/transform_blend_validation
```

Save this canonical probe as `build/transform_blend_validation/canonical_probe.py` and run it with the activated Python. It temporarily names only already-existing rows, then restores every original map byte even if checking fails. It does not add the missing BSS row or change an interval, class, flag or target. The expected checker message is:

    Source closure rejected: An unresolved source helper is referenced by a non-branch relocation.

The original and restored map SHA256 is `514c3ec6f2343e8a40fc73996df78fc95b2e33f4897fbdda6bb0311df6cf40fa` on this snapshot.

```python
from pathlib import Path
import subprocess,hashlib,os
p=Path('data/ver/eu/map.csv');before=p.read_bytes()
imports={0x33ba3c:'fn_0033BA3C',0x215e20:'fn_00215E20',0x28a998:'fn_0028A998',0x2a52ec:'fn_002A52EC',0x2a55d4:'fn_002A55D4',0x3a48f4:'dat_003A48F4',0x3b6694:'dat_003B6694',0x3f151c:'dat_003F151C',0x3f389c:'dat_003F389C'}
try:
 lines=[];found=set()
 for line in before.decode().splitlines(True):
  fields=line.split(',')
  try:address=int(fields[0],16)
  except ValueError:lines.append(line);continue
  if address in imports:
   assert fields[6].strip() in ('',imports[address]);fields[6]=imports[address];found.add(address)
  lines.append(','.join(fields))
 assert found==set(imports)
 p.write_bytes(''.join(lines).encode())
 result=subprocess.run(['python','tools/check.py','fn_0033BA3C','--object','build/eu/obj/lib/CtrSDK/sources/TransformBlendRoot.o'],capture_output=True,text=True,env=dict(os.environ,TMP='/tmp'))
 output=result.stdout+result.stderr
 Path('build/transform_blend_validation/canonical_check.log').write_text(output)
 print(output)
finally:
 p.write_bytes(before)
 assert p.read_bytes()==before
 print('Restored map SHA256:',hashlib.sha256(before).hexdigest())
```

Run the probe:

```sh
python build/transform_blend_validation/canonical_probe.py
```

For behavioral replay only, create `build/transform_blend_validation/original_symbols.sym` with these diagnostic import bindings. The missing BSS binding remains diagnostic and does not enter the map:

```text
#<SYMDEFS>#
0x003F389C D dat_003F389C
0x003F151C D dat_003F151C
0x00430C68 D dat_00430C68
0x003B6694 D dat_003B6694
0x003A48F4 D dat_003A48F4
0x00215E20 A fn_00215E20
0x0028A998 A fn_0028A998
0x00291470 A fn_00291470
0x002A52EC A fn_002A52EC
0x002A55D4 A fn_002A55D4
0x0022F278 A _ZN2nw3gfx19CalculatedTransform20UpdateTranslateFlagsEv
0x0022F244 A _ZN2nw3gfx19CalculatedTransform20UpdateCompositeFlagsEv
```

Link directly from the canonical object:

```sh
TMP=/tmp data/compilers/wibo data/compilers/4.0/902/bin/armlink.exe --cpu=MPCore --fpu=VFPv2 --arm_only --no_exceptions --inline --datacompressor=off --no_debug --no_scanlib --mangled --symbols --map --entry=fn_0033BA3C --keep=fn_0033BA3C --ro_base=0x00500000 --output=build/transform_blend_validation/candidate.axf --list=build/transform_blend_validation/candidate.map build/eu/obj/lib/CtrSDK/sources/TransformBlendRoot.o build/transform_blend_validation/original_symbols.sym
```

Save the following complete script as `build/transform_blend_validation/reproduce.py`. It constructs every fixture, reads the owner-local table from the fingerprinted executable, and writes result.json, coverage.json, nonreturn.json and a failure.json only if comparison fails. It does not read any uncommitted fixture or script. Run after building/linking, without a concurrent rebuild:

```sh
python build/transform_blend_validation/reproduce.py
```

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import *
from unicorn.arm_const import *
import struct, hashlib, random, json, time
D=Path('build/transform_blend_validation'); ROOT=0x33ba3c; STOP=0x600000; BASE=0x800000; SIZE=0x10000; SP=0x910000
R=BASE; MODEL=BASE+0x100; RESOURCE=BASE+0x200; DICT=BASE+0x300; RECORD=BASE+0x400
POLICY=BASE+0x500; PVT=BASE+0x580; ARRAY=BASE+0x600; WEIGHTS=BASE+0x700; CACHED=BASE+0x800; POSES=BASE+0x900
OUT=BASE+0x4000; SOURCE=BASE+0x4200; POSE=BASE+0x5000
D.mkdir(parents=True,exist_ok=True)
(D/'failure.json').unlink(missing_ok=True)
object_hash=hashlib.sha256(Path('build/eu/obj/lib/CtrSDK/sources/TransformBlendRoot.o').read_bytes()).hexdigest()
original=Path('data/ver/eu/code.bin').read_bytes()
assert hashlib.sha256(original).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
with (D/'candidate.axf').open('rb') as f:
 elf=ELFFile(f); entry=elf.header.e_entry
 segments=[(s['p_vaddr'],s.data()) for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
regs=list(range(UC_ARM_REG_R4,UC_ARM_REG_R11+1)); can=[0xa0000000+i*0x1111 for i in range(8)]
vregs=list(range(UC_ARM_REG_D8,UC_ARM_REG_D15+1)); vcan=[0x1122334455667788+i*0x1111 for i in range(8)]
coverage=set(); calls=[{},{}]; current={}; logs=[[],[]]
def ui(b,a=0):return struct.unpack_from('<I',b,a)[0]
def word(x):return struct.pack('<I',x&0xffffffff)
def fb(x):return struct.unpack('<I',struct.pack('<f',x))[0]
def put(b,a,x):struct.pack_into('<I',b,a-BASE,x&0xffffffff)
def floats(*x):return struct.pack('<'+'f'*len(x),*x)
def machine(which):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176)
 u.mem_map(0x100000,0x600000);u.mem_write(0x100000,original)
 for a,b in segments:u.mem_write(a,b)
 u.mem_map(BASE,SIZE);u.mem_map(0x900000,0x20000)
 u.reg_write(UC_ARM_REG_C1_C0_2,0xf00000);u.reg_write(UC_ARM_REG_FPEXC,0x40000000)
 def hook(uc,a,n,_):
  if a==STOP:uc.emu_stop();return
  if which==1 and a==ROOT and (not current['kw'].get('nested') or uc.reg_read(UC_ARM_REG_R0)==BASE+0x3000):uc.reg_write(UC_ARM_REG_PC,entry);return
  if which==0 and ROOT<=a<0x33c7c0:coverage.add(a)
  if a in (0x215e20,0x28a998,0x291470,0x2a52ec,0x2a55d4,0x22f278,0x22f244,0x33b9d0,0x33c7d0,0x33ba20,0x33b6d4,0x33b9b4,0x33b9ac,0x33c7c8,0x33b99c,0x33b44c,0x33c820):calls[which][hex(a)]=calls[which].get(hex(a),0)+1
  if a==0x33b4ac:
   child=uc.reg_read(UC_ARM_REG_R0);out=uc.reg_read(UC_ARM_REG_R1);index=uc.reg_read(UC_ARM_REG_R2)
   slot=(child-(BASE+0x1000))//0x100; cfg=current['children'][slot]
   logs[which].append(('evaluate',slot,index,bytes(uc.mem_read(out,64)).hex()))
   if cfg.get('actual'):return
   result=0
   if cfg['result']:
    old=ui(uc.mem_read(out+60,4));data=bytearray(cfg['transform']);struct.pack_into('<I',data,60,ui(data,60)|(old&0x40000000))
    uc.mem_write(out,bytes(data))
    result=out if cfg['result']==1 else SOURCE
   uc.reg_write(UC_ARM_REG_R0,result);uc.reg_write(UC_ARM_REG_PC,uc.reg_read(UC_ARM_REG_LR));return
  if a in (0x33b9d0,0x33c7d0):
   wp=ui(uc.mem_read(uc.reg_read(UC_ARM_REG_SP),4));tp=uc.reg_read(UC_ARM_REG_R3)
   logs[which].append(('blend',bytes(uc.mem_read(wp,12)).hex(),bytes(uc.mem_read(tp,64)).hex()))
  if a in (0x33ba20,0x33c820):logs[which].append(('finish',bytes(uc.mem_read(uc.reg_read(UC_ARM_REG_R1),64)).hex()))
 u.hook_add(UC_HOOK_CODE,hook)
 return u
machines=[machine(0),machine(1)]
def fixture(seed,**kw):
 rng=random.Random(seed);b=bytearray(rng.randbytes(SIZE)); n=kw.get('n',rng.randrange(1,6));idx=kw.get('index',rng.randrange(3))
 def p(a,v):put(b,a,v)
 p(R+8,MODEL);p(MODEL+8,RESOURCE);p(RESOURCE+0x14,DICT-RESOURCE-0x14);p(RESOURCE+0x10,3)
 for i in range(3):
  p(DICT+0x28+i*16,RECORD+i*32-DICT-0x28-i*16);p(RECORD+i*32+12,i)
 p(MODEL+16,BASE+0x980)
 for i in range(3):p(BASE+0x980+4*i,POLICY)
 p(POLICY,PVT);b[POLICY+5-BASE]=kw.get('finish',rng.randrange(2));p(PVT+8,kw.get('policy',0x33b9d0));p(PVT+12,0x33c820 if kw.get('policy')==0x33c7d0 else 0x33ba20)
 p(R+0x14,ARRAY);p(R+0x18,ARRAY+4*n);p(R+0x24,WEIGHTS);p(R+0x28,WEIGHTS+4*n);p(R+0x34,CACHED)
 b[0x40]=kw.get('mode',seed%2);b[0x41]=kw.get('dirty',rng.randrange(2));b[0x42]=kw.get('normalize',rng.randrange(2))
 p(MODEL+0x54,POSES)
 for i in range(3):p(POSES+4*i,POSE)
 scales=kw.get('scales',[rng.choice([0.,1.,.5,2.]) for _ in range(3)])
 angles=kw.get('angles',[rng.choice([0.,.1,-.4,1.,3.14,-1.7]) for _ in range(3)])
 translations=kw.get('translations',[rng.choice([0.,1.,-1.]) for _ in range(3)])
 b[POSE-BASE:POSE-BASE+36]=floats(*(scales+angles+translations))
 def transform(flags):
  x=bytearray(floats(1.,.2,.3,.4,.5,1.,.6,.7,.8,.9,1.,1.1,1.,2.,3.))
  if kw.get('zero_matrix'):x[:48]=bytes(48)
  x+=word(flags);return bytes(x)
 outflags=kw.get('outflags',rng.choice([0,0x40000000,0x7e0,0x1c,0xffffffff]));b[OUT-BASE:OUT-BASE+64]=transform(outflags)
 b[SOURCE-BASE:SOURCE-BASE+64]=transform(0x801)
 children=[]
 for i in range(n):
  c=BASE+0x1000+i*0x100;vt=BASE+0x2000+i*0x100;null=rng.randrange(5)==0
  if i==0:null=False
  if 'nulls' in kw:null=i in kw['nulls']
  p(ARRAY+i*4,0 if null else c);p(c,vt)
  p(vt+8,0x33b6d4 if rng.randrange(2) else 0x33b9b4)
  p(vt+0x1c,0x33b9ac if rng.randrange(4) else 0x33c7c8);p(vt+0x18,0x33b4ac)
  if 'has' in kw:p(vt+0x1c,0x33b9ac if kw['has'] else 0x33c7c8)
  if 'special' in kw:p(vt+8,0x33b6d4 if kw['special'] else 0x33b9b4)
  for j in range(3):b[c+0x5b+j-BASE]=kw.get('mask',rng.randrange(2))
  w=kw.get('weightbits',fb(rng.choice([0.,.0001,.001,.25,.5,1.,2.,-1.,-2.])))
  p(WEIGHTS+4*i,w);p(CACHED+4*i,w)
  p(c+0x64,BASE+0x6000+i*0x100);p(c+0x68,BASE+0x6040+i*0x100);b[c+0x58-BASE]=0;p(c+0x54,1);p(c+0x14,BASE+0x7000+i*0x20)
  for j in range(3):p(BASE+0x7000+i*0x20+4*j,0)
  b[0x6000+i*0x100:0x6040+i*0x100]=transform(kw.get('childflags',0x801))
  if kw.get('actual'):
   p(c,0x3d8414);p(c+8,MODEL)
   if kw.get('actual_pose'):p(c+0x54,0)
  children.append({'actual':kw.get('actual',False),'result':kw.get('result',rng.randrange(3)),'transform':transform(kw.get('childflags',rng.choice([0,0x801,0x810,0x8,0x7e0,0xffffffff])))})
 if kw.get('nested'):
  root=BASE+0x3000;p(root,0x3d8500);p(root+8,MODEL);p(root+0x14,BASE+0xa00);p(root+0x18,BASE+0xa04);p(BASE+0xa00,BASE+0x1000)
  p(root+0x24,BASE+0xa80);p(root+0x28,BASE+0xa84);p(root+0x34,BASE+0xa90);p(BASE+0xa80,fb(1.));p(BASE+0xa90,fb(1.))
  b[root+0x40-BASE]=kw.get('nested_mode',0);b[root+0x41-BASE]=1;b[root+0x42-BASE]=1;p(ARRAY,root)
 return {'seed':seed,'bytes':bytes(b),'children':children,'index':idx,'fpscr':kw.get('fpscr',0),'guard':kw.get('guard',seed%3),'chain':kw.get('chain',0),'kw':kw}
def run(cfg):
 global current
 current=cfg;results=[]
 for which,u in enumerate(machines):
  logs[which].clear();u.mem_write(BASE,cfg['bytes']);u.mem_write(0x900000,bytes(0x20000))
  u.mem_write(0x3f389c,word(cfg['guard']));u.mem_write(0x430c68,floats(1.,0.,0.,0.,0.,1.,0.,0.,0.,0.,1.,0.))
  u.mem_write(0x3f150c,word(0x3f151c if cfg['chain'] else 0));u.mem_write(0x3f151c,word(0))
  snapshots=[]
  for repetition in range(2):
   u.reg_write(UC_ARM_REG_CPSR,0x10);u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,STOP);u.reg_write(UC_ARM_REG_R0,R);u.reg_write(UC_ARM_REG_R1,OUT);u.reg_write(UC_ARM_REG_R2,cfg['index'])
   u.reg_write(UC_ARM_REG_FPSCR,cfg['fpscr'])
   for reg,v in zip(regs,can):u.reg_write(reg,v)
   for reg,v in zip(vregs,vcan):u.reg_write(reg,v)
   u.emu_start(ROOT,STOP+4,count=300000)
   if u.reg_read(UC_ARM_REG_PC)!=STOP:raise AssertionError(('no return',which,hex(u.reg_read(UC_ARM_REG_PC)),cfg['seed']))
   assert [u.reg_read(reg) for reg in regs]==can
   assert [u.reg_read(reg) for reg in vregs]==vcan
   assert u.reg_read(UC_ARM_REG_SP)==SP
   snapshots.append((u.reg_read(UC_ARM_REG_R0),bytes(u.mem_read(BASE,SIZE)),bytes(u.mem_read(0x430c68,48)),bytes(u.mem_read(0x3f389c,4)),u.reg_read(UC_ARM_REG_FPSCR)&0x9f,list(logs[which])))
  results.append(snapshots)
 if results[0]!=results[1]:
  diff=[]
  for repetition,(x,y) in enumerate(zip(*results)):
   for k,(a,b) in enumerate(zip(x,y)):
    if a!=b:
     if isinstance(a,bytes):diff.append((repetition,k,[(hex(BASE+j),hex(ui(a,j//4*4)),hex(ui(b,j//4*4))) for j in range(len(a)) if a[j]!=b[j]][:20]))
     else:diff.append((repetition,k,a,b))
  failure={'seed':cfg['seed'],'kw':cfg['kw'],'diff':diff};(D/'failure.json').write_text(json.dumps(failure,indent=2));raise AssertionError(failure)
 return len(logs[0])
def bounded_nonreturn(angle_word):
 global current
 cfg=fixture(999,mode=0,n=1,has=True,result=0,weightbits=fb(1.),outflags=0,dirty=0,angles=[0.,0.,0.],guard=0)
 b=bytearray(cfg['bytes']);put(b,POSE+12,angle_word);cfg['bytes']=bytes(b);current=cfg
 observations=[]
 for which,u in enumerate(machines):
  logs[which].clear();u.mem_write(BASE,cfg['bytes']);u.mem_write(0x900000,bytes(0x20000));u.mem_write(0x3f389c,word(0));u.mem_write(0x430c68,bytes(48))
  u.reg_write(UC_ARM_REG_CPSR,0x10);u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,STOP);u.reg_write(UC_ARM_REG_R0,R);u.reg_write(UC_ARM_REG_R1,OUT);u.reg_write(UC_ARM_REG_R2,cfg['index']);u.reg_write(UC_ARM_REG_FPSCR,0)
  u.emu_start(ROOT,STOP+4,count=50000)
  assert u.reg_read(UC_ARM_REG_PC)!=STOP
  observations.append({'pc':hex(u.reg_read(UC_ARM_REG_PC)),'fpscr_exceptions':u.reg_read(UC_ARM_REG_FPSCR)&0x9f,'output_flags':hex(ui(u.mem_read(OUT+60,4)))})
 return {'angle_word':hex(angle_word),'observations':observations}
if __name__=='__main__':
 start=time.time();total=0
 for i in range(500):run(fixture(i));total+=1
 for i in range(500):run(fixture(i,actual=True));total+=1
 for i in range(500):run(fixture(i,actual=True,policy=0x33c7d0));total+=1
 for i in range(200):run(fixture(i,actual=True,actual_pose=True));total+=1
 for i in range(200):run(fixture(i,actual=True,nested=True,nested_mode=i%2,policy=0x33c7d0 if i%3 else 0x33b9d0));total+=1
 for mode in [0,1]:
  for special in [0,1]:
   run(fixture(500,mode=mode,n=3,outflags=0,childflags=0x801,zero_matrix=True,policy=0x33c7d0,special=special,mask=0,has=True,result=1,weightbits=fb(1.),finish=1,guard=0));total+=1
 run(fixture(501,mode=0,n=3,result=0,has=True,angles=[-20000.,15000.,-11000.],outflags=0,dirty=0,weightbits=fb(1.)));total+=1
 # Weight exceptional values, tolerance boundaries and floating modes.
 words=[0,0x80000000,1,0x80000001,0x7fffff,0x800000,0x3a83126e,0x3a83126f,0x3a831270,0x3f7fffff,0x3f800000,0x3f800001,0xbf800000,0x7f7fffff,0xff7fffff,0x7f800000,0xff800000,0x7fc12345,0x7f812345,0xffc12345]
 for mode in [0,1]:
  for fpscr in [0,1<<24,1<<25,1<<22,2<<22,3<<22]:
   for w in words:
    run(fixture(600,mode=mode,actual=True,n=3,mask=0,dirty=1,normalize=1,outflags=0,weightbits=w,fpscr=fpscr));total+=1
 for fpscr in [0,1<<24,1<<25,1<<22,2<<22,3<<22]:
  for w in words:
   run(fixture(601,mode=0,actual=True,n=3,mask=0,dirty=0,normalize=0,outflags=0,weightbits=w,fpscr=fpscr));total+=1
 for offset in [0,4,8,24,28,32]:
  for w in words:
   cfg=fixture(602,mode=0,n=1,result=0,has=True,dirty=0,weightbits=fb(1.),outflags=0,angles=[0.,0.,0.])
   b=bytearray(cfg['bytes']);put(b,POSE+offset,w);cfg['bytes']=bytes(b)
   run(cfg);total+=1
 for i in range(300):
  rng=random.Random(i);angles=[rng.uniform(-50000,50000) for _ in range(3)]
  run(fixture(700+i,mode=0,n=3,angles=angles,result=0,has=True,dirty=0,weightbits=fb(1.),fpscr=[0,1<<24,1<<25,1<<22,2<<22,3<<22][i%6]));total+=1
 nonreturn=[bounded_nonreturn(w) for w in [0x7f800000,0xff800000,0x7fc12345,0x7f812345,0x7f7fffff]]
 expected=set(range(ROOT,0x33be3c,4))|set(range(0x33be58,0x33c6ac,4))|set(range(0x33c6c0,0x33c7c0,4))
 assert coverage==expected-{0x33bf7c,0x33c6a8}
 (D/'nonreturn.json').write_text(json.dumps(nonreturn,indent=2))
 (D/'result.json').write_text(json.dumps({'fixtures':total,'call_pairs':total*2,'nonreturn_pairs':len(nonreturn),'object_sha256':object_hash,'seconds':time.time()-start,'coverage':len(coverage),'calls':calls},indent=2))
 (D/'coverage.json').write_text(json.dumps(sorted(coverage)))
 print((D/'result.json').read_text())
```
