# Effect update: genuine math import on current main

The link prerequisite is resolved without an alias bridge or map edit. This is a **252-byte nonexact source proposal with 18 differing bytes**, reproducing the prior best baseline. Zero new exact bytes and zero newly attempted residual source forms. The existing packet and Pro response are unchanged.

Frozen base: `8991bec8cb27ff1a2dac0003f059d270a0b17493`, fetched by the coordinator on 2026-10-02 at 02:07 UTC. Branch: `dot/effect-math-import-8991`. The two source inputs were committed as `cf92b7089e098dba6eb3fc44169a0269a325b610` before the project's clean build and checks. Source patch: `git diff 8991bec8cb27ff1a2dac0003f059d270a0b17493 cf92b7089e098dba6eb3fc44169a0269a325b610 -- lib/al`. The saved identical patch is `build/effect-math-import/source.patch`; `git apply --check` succeeds in the clean frozen `mario-main8991` checkout. No source assembly or shared-header composition is required.

## Source closure and independently recovered interface

The existing row at `0027CB9C..0027CBD8` remains U, Type `fa`, with its existing symbol `_ZN4sead14Vector3CalcCtrIfE3mulERN2nn4math4VEC3ERKNS3_5MTX34ERKS4_`. The row's spelling is a pre-existing symbolic interface, not fresh evidence for an original class identity. No address, name, Type, boundary, flags, checker or binary was changed to obtain closure.

Current main contains no declaration of `Vector3CalcCtr`. Its clean `Math/seadMatrixConversion.cpp` defines `nn::math::MTX34` as `float m[3][4]`. The new private `Effect/alEffectSetUpdateMath.h` keeps that struct definition token-identical, adds only the dump-recovered three-float VEC3 storage, and declares the existing static `Vector3CalcCtr<float>::mul` signature. It contains no implementation, trampoline, source alias or extra helper. The source's packet-local vector/matrix wrappers derive from these plain bases, giving ordinary defined base-reference conversions. These wrappers are source-shape hypotheses, not recovered EffectSet inheritance or original constructors.

Independent retail evidence: at 0027CB9C, `vldmia r1` reads twelve floats, `vldmia r2` reads three, and all reads precede the stores through r0. Rows accumulate translation + x*m0 + y*m1 + z*m2, then store three floats at output offsets 0/4/8. The copier at 00291470 checks identical input/output pointers, loads all twelve words before storing and uses the same 48-byte layout. Both callers 001EA220 and 0023F02C pass result/matrix/vector in r0/r1/r2. This supports the field storage and ABI without consulting removed headers or implementations.

Only `lib/al/src/Effect/alEffectSetUpdateFunction.cpp` includes the new private header. Shared `math/seadVector.h`, `math/seadMatrix.h`, `Math/seadMatrixConversion.cpp`, EffectObj.cpp, config, tools, map, ledger and STATE remain unchanged. The packet baseline's existing `NON_MATCHING` convention is retained. Its separate header for the root declaration is unnecessary because the existing keeper already declares its neutral C identity consistently.

The compiler object defines only `fn_001EA220` (252 bytes); every inline wrapper disappears. It imports exactly five functional symbols: the existing mapped affine-vector method, fn_0023F45C, fn_00291470, fn_0023F294, and fn_002D37D0. These cover six call sites, since the matrix updater is called on either matrix branch. The ordinary ARM library request markers are not source helper bodies. The unchanged strict check resolves all five at their established row starts and reports no source-helper rejection. No external implementation is supplied by this patch.

## Build and unchanged-checker evidence

`. ./development_environment.sh` and `python make.py eu -ca` ran with committed source, an absent build directory, and a pristine base map. The first clean build compiled 45 Game, 130 al and one SDK source, linked and exported successfully without retries. Its root remained U: direct AXF/map inspection shows the 16-byte scaffold definition, not the new 252-byte source function. This unactivated clean build therefore supplies environment/source-compilation evidence, **not** proof of resolution of the earlier normal-link failure. A separate local root-activation check is recorded below. Only the ignored private executable/exheader and the approved compiler/Python installations are shared; objects and archives are local to this worktree. Build launch followed the 02:11:08 UTC source commit; the final exported artifact/log timestamp is 02:11:44.598527 UTC. The exact build wall duration was not separately instrumented.

With the unmodified unnamed target row, `tools/check.py fn_001EA220 --object build/eu/obj/lib/al/src/Effect/alEffectSetUpdateFunction.o` correctly cannot select a unique root. A **local, map-dependent diagnostic** then named only the existing unnamed 001EA220 row `fn_001EA220`, without changing its Type, rank before checking, bounds or any import. The same unchanged canonical command reported:

```text
U -> m: The linked candidate differs from the unchanged original interval.
```

The checker linked all 252 bytes at the original address and measured 18 differing bytes. The compiled raw section has packet pattern C hash `3070ed117843d5426a1ba6025dab579d6a5c305c56a401695c7425aaa4ed03d0`. The full linked candidate hash is `e185d1f107bd8ee4d749c3cdd541cd44d9252dba73c255f9d7ae17efe660cba9`. The original interval hash remains `1b3537fcf97a24a277196374d9acba72cd7e959d3367d199233f1eb1c6b17234`.

The entire map was restored byte-for-byte before the preservation sweep. Its base hash is `2dde00f86cc0bc6f174f3a281853a4faacea1f16b6b628d602bea687b4cdb805`. The direct source-object check establishes the genuine mapped signature and original-address import resolution. Local target enrollment is also needed to activate the previously U root for the normal-link proof. Main owns any eventual enrollment. No alias metadata is required.

The sequential unchanged-checker sweep preserves **717/717 accepted roots and all 736/736 actual canonical definitions**, including all duplicate providers. It selects every STT_FUNC definition for the original accepted set from provenance-recorded Game/lib C++ objects, excludes generated scaffold C, asserts complete root coverage, and requires each literal `O -> O` exact-byte result. The sweep ran 2026-10-02T02:12:24.403134+00:00 through 2026-10-02T02:20:54.708238+00:00 in 510.305217 seconds. It restored the map byte-for-byte. This is the current 8991 acceptance set, not a smaller historical gate; no new root is credited.

After that map-writing sweep finished, the **serialized normal-link diagnostic** temporarily named only the unchanged target row `fn_001EA220` and enrolled it M. `python make.py eu` succeeded, linked and exported in 16.458812 seconds. The normal map states `Selecting member alEffectSetUpdateFunction.o(libActor.a) to define fn_001EA220` and records a **252-byte** source definition at compact address 0x0010A014; this proves the root's archive member was pulled. The existing mapped affine-vector import and all four neutral imports resolve through ordinary project linking. As elsewhere in the compact project, imported routines can remain scaffold bodies; this is import-closure evidence, not reconstructed dependency behavior. The unchanged object checker then reports `M -> m` with the same residual. Frozen activation artifacts/summary are in `build/effect-math-import/activated/`. The diagnostic restored the exact base map, rebuilt successfully again under that map, and verified the candidate object hash stayed unchanged. No alias metadata or source edit was used. A final executed check-baseline.py rerun reproduced `U -> m` and the identical 18-byte result, restoring the map again.

## Full neighboring context and bounded stopping point

The complete 0023F02C body was disassembled directly from the verified dump, not inferred from the packet's excerpt. It saves r4/r5/r6/r7 and reserves 0x44 bytes. Set is r4 throughout; entry arrives in r1 and becomes r7 at function entry, long before the three entry-creation alternatives. After creation/liveness, the matrix path loads configuration into r6 and matrix into r5. Thus its desired vector setup (r0=sp, r1=r5, r2=r6+0x14) occurs with entirely different live-register constraints from 001EA220, which also has the signed loop index and a result/matrix register shared at different lifetimes.

The neighbor uses the entry r7 again after the update paths to dereference its reference and independently validate the identifier; it reloads configuration from set+0xC for further scale/color properties. Neither the matrix update's ignored return nor the affine/copy ignored returns establish a new return-type claim. The liveness routine independently returns exactly zero or one. All used prototypes remain conservative and unqualified by restrict/purity/no-alias assumptions.

The baseline still differs in precisely twelve instruction rows: ten r6/r7 substitutions for entry/configuration and the swapped preparation instructions at +0x60/+0x64. Stack size/slots, every call site, reloads, result/matrix reuse and compiler-generated NOPs agree. The original dump supports no new constructor, receiver or aliasing-contract hypothesis that improves on the prior eight packet forms, the earlier dot value-return form, and Pro's three graded failures. No cosmetic, declaration-order, pointer-only or reference-only permutations were attempted. This pass contains one faithful baseline reproduction, **zero new residual forms** and zero new exact credit.

## Bounded whole-root execution

The committed baseline and retail root pass **2,332 paired executions: 2,304 returning pairs and 28 matching fault pairs** under Unicorn ARM1176 with VFP enabled. All five direct retail callees execute their original instructions. Their original transitive normalization, vector-normalization, matrix-copy and vector-add callees also execute, with no behavioral hooks or modeled external returns. The independent Python model checks only the root's live-entry/flag decisions, direct-call order/arguments and Boolean result, passing 4,608 returning executions; it is not an independent numerical implementation of the math callees.

583 fixtures run at each FPSCR state 0, 0x00400000, 0x01000000 and 0x03C0009F. Inputs cover counts -2/-1/0/1/2/3/4/8, INT_MIN controls, null/stale/dead/live references, raw flags 0/1/128/255, both updater paths, normalization, null matrix on the legal follow branch, repeated entries, output/input matrix aliasing, translation within matrix storage, signed zero, finite values, denormals, infinities and quiet/signaling NaN bit patterns. Seven explicit bad-pointer fixtures fault on array, entry, reference, object, configuration, matrix or translation reads at identical addresses/access widths. These probes do not establish general fault safety.

The comparison includes the complete 64 KiB fixture memory, return, normalized direct-call trace, fault tuple, final FPSCR, SP, r4-r11 and d8-d15. Maximum execution is 1,884 instructions. The two static locations read by the original follow updater receive explicit caller fixture data: zero at 0x004305F8 and a 3x4 identity matrix at 0x00430A88. That is an execution precondition, not a recovery or validation of startup initialization. Writable code/BSS outside these inputs, stack scratch residue, asynchronous activity, callbacks that mutate the set/count, arbitrary aliasing and original C++ object lifetimes are outside the claim. No hardware, full engine, port, or unrestricted functional NonMatching claim follows from this replay. Replay completed in 7.526625 seconds at 02:15:25.690947 UTC.

Observed original helper entries across the 2,332 original executions: liveness 5,400; affine-vector 3,136; matrix constructor/copy 4,716; matrix updater 1,740; follow updater 2,504; matrix normalization 1,584; vector normalization 4,752; raw matrix copy 6,740; vector addition 1,936. All nine routines executed in their original form.

## Frozen inputs and recipes

- Root source SHA256: `033beee30708e29e39b49479ccb28480db1de872ffa4e5b1040d417efe51b0e8`
- Private header SHA256: `6b127e371f2e63e53330334e01d33eb6d28d30f29512a3900fd48f8473451efa`
- Canonical object SHA256: `57ebb903378386c56ebecc8af1dfddd90375af78a335ab306e07d3a4fb824e85`
- Original executable SHA256: `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`
- Checker SHA256: `e177e0abe130e645e75c5f0dc24abb19dbe83310de55f1f099ec8fc385e07317`
- Exact-byte implementation SHA256: `aef81a3a21c49bb17b8a19f139d02607899257e04dbe461ece2992cc726fc2e2`

Ignored evidence resides under `build/effect-math-import/`: baseline-clean-build.log, unnamed-root-check.log, baseline-check.log/json, baseline/ linker outputs, candidate.bin, residual.txt, retail-callees.txt, source-manifest.json, source.patch, replay.py/log/summary and preserve.py/log/JSON, and activated/ normal build, AXF/map, check and summary files. These generated executable bytes are never committed. The following recipes reconstruct the diagnostic and replay from the committed source and the owner's local verified dump; they do not manufacture or edit compiler objects or original bytes.

```sh
. ./development_environment.sh
python make.py eu -ca
python build/effect-math-import/check-baseline.py
python build/effect-math-import/replay.py
python build/effect-math-import/preserve.py
python build/effect-math-import/activate-root.py
```

Recreate each script by saving the corresponding fenced block below to its named ignored path. The baseline recipe temporarily names the existing root and restores the full map in a finally block. Do not run any other map-writing checks concurrently in this worktree. The preservation recipe requires the restored 8991 map and checks every actual canonical definition sequentially.

Report frozen 2026-10-02T02:23:00.874791+00:00. Assignment-to-report wall interval is 866.874791 seconds (14.447913 minutes), including reading, baseline restoration, clean compilation, direct grading, full current-definition checking, original-callee replay, activation proof and report preparation. The source-reproduction attempt count is one; new residual-form attempts are zero. Main-accepted throughput is **0 bytes/hour** and exact added bytes are zero. The useful deliverable is the sound source import closure and current-base verified baseline, plus reusable bounded behavior and live-register evidence. The coordinator publishes; this lane has not pushed.

<!-- replay-file: build/effect-math-import/check-baseline.py -->
```python
from pathlib import Path
import subprocess,json,hashlib
from elftools.elf.elffile import ELFFile
import sys
sys.path.insert(0,str(Path.cwd()))
from tools.low.checkExactBytes import check_exact_bytes
R=Path.cwd();OUT=R/'build/effect-math-import';OUT.mkdir(parents=True,exist_ok=True)
p=R/'data/ver/eu/map.csv';saved=p.read_bytes()
assert hashlib.sha256(saved).hexdigest()=='2dde00f86cc0bc6f174f3a281853a4faacea1f16b6b628d602bea687b4cdb805'
lines=saved.decode().splitlines(keepends=True)
for i,line in enumerate(lines):
 if line.startswith('0x001EA220,'):
  fields=line.split(',');assert fields[6]=='';fields[6]='fn_001EA220';lines[i]=','.join(fields)
try:
 p.write_text(''.join(lines))
 obj='build/eu/obj/lib/al/src/Effect/alEffectSetUpdateFunction.o'
 c=subprocess.run(['.venv/bin/python','tools/check.py','fn_001EA220','--object',obj],capture_output=True,text=True)
 (OUT/'baseline-check.log').write_text(c.stdout+c.stderr)
 print(c.returncode,c.stdout+c.stderr)
 assert 'U -> m: The linked candidate differs from the unchanged original interval.' in c.stdout
 result=check_exact_bytes('fn_001EA220',Path(obj),output_directory=OUT/'baseline')
 (OUT/'baseline-check.json').write_text(json.dumps(result,indent=2)+'\n')
 assert not result['exact'] and result['evidence']['different_bytes']==18
 with (OUT/'baseline/candidate.axf').open('rb') as stream:
  elf=ELFFile(stream);(OUT/'candidate.bin').write_bytes(elf.get_section_by_name('CANDIDATE_CODE').data())
finally:p.write_bytes(saved)
assert p.read_bytes()==saved
```

<!-- replay-file: build/effect-math-import/replay.py -->
```python
"""Bounded whole-root replay using unchanged retail callees; no exact credit."""
from pathlib import Path
from unicorn import Uc,UcError,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE,UC_HOOK_MEM_INVALID
from unicorn.arm_const import *
import hashlib,json,struct,random,time,datetime,collections
R=Path.cwd();OUT=R/'build/effect-math-import'
CODE=(R/'data/ver/eu/code.bin').read_bytes();CAND=(OUT/'candidate.bin').read_bytes()
assert hashlib.sha256(CODE).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
assert hashlib.sha256(CAND).hexdigest()=='e185d1f107bd8ee4d749c3cdd541cd44d9252dba73c255f9d7ae17efe660cba9'
ROOT=0x1ea220;STOP=0x700ff0;DB=0x1000000;DS=0x10000;STACK=0x800000;SP=STACK+0xf000
SET=DB;ARRAY=DB+0x100;CONF=DB+0x200;TRANS=DB+0x300;MATRIX=DB+0x400
ENTRY=DB+0x1000;REF=DB+0x2000;OBJ=DB+0x3000
REGS=[UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3,UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11,UC_ARM_REG_R12]
INTERVALS=[(ROOT,ROOT+252),(0x23f294,0x23f494),(0x27cb9c,0x27cbd8),(0x291470,0x291490),(0x2d37d0,0x2d3a34),(0x1ec610,0x1ec6bc),(0x27ccbc,0x27ccfc),(0x27c18c,0x27c198),(0x27cb48,0x27cb64)]
HELPERS=[a for a,z in INTERVALS if a!=ROOT]+[0x23f45c]
DIRECT=[0x23f45c,0x27cb9c,0x291470,0x23f294,0x2d37d0]
IDENT=[0x3f800000,0,0,0,0,0x3f800000,0,0,0,0,0x3f800000,0]
def words(v):return struct.pack('<'+'I'*len(v),*[x&0xffffffff for x in v])
def put(d,a,v):struct.pack_into('<I',d,a-DB,v&0xffffffff)
def get(d,a):return struct.unpack_from('<I',d,a-DB)[0]
def si(x):return x if x<0x80000000 else x-0x100000000
class Engine:
 def __init__(self,candidate):
  u=self.u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176)
  u.mem_map(0x100000,0x400000);u.mem_write(0x100000,CODE)
  if candidate:u.mem_write(ROOT,CAND)
  u.mem_map(DB,DS);u.mem_map(STACK,0x10000);u.mem_map(0x700000,0x1000)
  u.reg_write(UC_ARM_REG_C1_C0_2,0xf<<20);u.reg_write(UC_ARM_REG_FPEXC,1<<30)
  u.hook_add(UC_HOOK_CODE,self.hook);u.hook_add(UC_HOOK_MEM_INVALID,self.invalid)
 def invalid(self,u,access,address,size,value,data):self.faults.append((access,address,size));return False
 def hook(self,u,pc,size,data):
  self.steps+=1
  assert any(a<=pc<z for a,z in INTERVALS),hex(pc)
  if pc in HELPERS:self.helpers[hex(pc)]+=1
  lr=u.reg_read(UC_ARM_REG_LR)
  if pc in DIRECT and ROOT<=lr<ROOT+252:
   r=[u.reg_read(x) for x in REGS[:4]]
   # The primary control trace is independent of unobservable register allocation.
   if pc==0x23f45c:self.calls.append(('live',r[0]))
   elif pc==0x27cb9c:self.calls.append(('affine',r[1],r[2]))
   elif pc==0x291470:self.calls.append(('copy',r[1]))
   elif pc==0x23f294:self.calls.append(('matrix',r[0],r[1],'local' if STACK<=r[2]<STACK+0x10000 else r[2]))
   elif pc==0x2d37d0:self.calls.append(('follow',*r))
 def run(self,initial,fpscr):
  u=self.u;u.mem_write(DB,initial);u.mem_write(STACK,b'\xa5'*0x10000)
  # Explicit caller-provided static state. Startup initialization is outside this replay.
  u.mem_write(0x4305f8,words([0,0,0]));u.mem_write(0x430a88,words(IDENT))
  u.reg_write(UC_ARM_REG_CPSR,0x10);u.reg_write(UC_ARM_REG_FPSCR,fpscr)
  for i,r in enumerate(REGS):u.reg_write(r,0xb0000000+i*0x1111)
  for i in range(8,16):u.reg_write(UC_ARM_REG_D0+i,0x123456789abc0000+i)
  u.reg_write(UC_ARM_REG_R0,SET);u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,STOP)
  self.faults=[];self.helpers=collections.Counter();self.calls=[];self.steps=0;error=None
  try:u.emu_start(ROOT,STOP,count=200000)
  except UcError as e:error=e.errno
  if error is None:
   assert u.reg_read(UC_ARM_REG_PC)==STOP,'instruction limit'
   assert u.reg_read(UC_ARM_REG_SP)==SP
   assert [u.reg_read(REGS[i]) for i in range(4,12)]==[0xb0000000+i*0x1111 for i in range(4,12)]
   for i in range(8,16):assert u.reg_read(UC_ARM_REG_D0+i)==0x123456789abc0000+i
  return dict(memory=bytes(u.mem_read(DB,DS)),result=u.reg_read(UC_ARM_REG_R0) if error is None else None,calls=self.calls,error=error,faults=self.faults,fpscr=u.reg_read(UC_ARM_REG_FPSCR)),self.helpers,self.steps

def control_model(d):
 calls=[];updated=False;n=si(get(d,SET+8));c=get(d,SET+12);t=get(d,SET+16);m=get(d,SET+20)
 for index in range(max(n,0)):
  e=get(d,get(d,SET+4)+index*4);calls.append(('live',e));ref=get(d,e+4);o=get(d,ref)
  live=o and get(d,ref+4)==get(d,o+12) and si(get(d,o+4))>0
  if not live:continue
  if d[c+0x40-DB]:
   if d[c+0x45-DB]:calls += [('affine',m,c+0x14),('copy',m),('matrix',e,c,'local')]
   else:calls.append(('matrix',e,c,m))
   updated=True
  if d[c+0x3f-DB]:calls.append(('follow',e,c,t,m));updated=True
 return int(updated),calls

rng=random.Random(ROOT)
SPECIAL=[0,0x80000000,1,0x80000001,0x7fffff,0x800000,0x3f800000,0xbf800000,0x7f7fffff,0xff7fffff,0x7f800000,0xff800000,0x7fc12345,0x7fa12345]
FINITE=[0,0x3f000000,0x3f800000,0x40000000,0x40800000,0xbf000000,0xbf800000,0xc0000000]
def case(i):
 d=bytearray(DS);n=[-2,-1,0,1,2,3,4,8][i%8]
 for a,v in [(SET+4,ARRAY),(SET+8,n),(SET+12,CONF),(SET+16,TRANS),(SET+20,MATRIX)]:put(d,a,v)
 flag_values=[0,1,128,255]
 for bit,off in enumerate([0x3d,0x3f,0x40,0x45]):d[CONF+off-DB]=flag_values[(i//(4**bit))%4]
 float_choices=SPECIAL if i%5==0 else FINITE
 for k in range(12):put(d,MATRIX+4*k,rng.choice(float_choices))
 for k in range(3):put(d,TRANS+4*k,rng.choice(float_choices));put(d,CONF+0x14+4*k,rng.choice(float_choices))
 put(d,CONF+0x20,rng.choice(float_choices))
 for k in range(8):
  e=ENTRY+k*0x20;ref=REF+k*0x10;o=OBJ+k*0x200;put(d,ARRAY+4*k,e);put(d,e+4,ref)
  state=(i+k)%5;put(d,ref,0 if state==0 else o);put(d,ref+4,0x1234+k);put(d,o+12,0x1234+k+(1 if state==1 else 0));put(d,o+4,[-1,0,1,2][(i+k)%4] if state==2 else 1)
  for off in range(0xfc,0x180,4):put(d,o+off,rng.choice(float_choices))
 if i%9==0 and not d[CONF+0x40-DB]:put(d,SET+20,0) # legal null-matrix follow path
 return bytes(d)

cases=[('valid',case(i),True) for i in range(512)]
# Repeated entries, aliased transforms, and signed-count extremes stay explicit.
for kind in ['repeat_entry','matrix_output_alias','translation_matrix_alias','negative_count']:
 for i in range(16):
  d=bytearray(case(320+i));put(d,SET+8,4)
  if kind=='repeat_entry':
   for k in range(4):put(d,ARRAY+4*k,ENTRY)
  elif kind=='matrix_output_alias':put(d,SET+20,OBJ+0xfc)
  elif kind=='translation_matrix_alias':put(d,SET+16,MATRIX+12)
  else:put(d,SET+8,0x80000000)
  cases.append((kind,bytes(d),True))
# Fault probes compare actual failing address/access and all pre-fault data writes.
for kind in ['array','entry','reference','object','configuration','matrix','translation']:
 d=bytearray(case(341));put(d,SET+8,1);put(d,REF,OBJ);put(d,REF+4,0x1234);put(d,OBJ+12,0x1234);put(d,OBJ+4,1)
 d[CONF+0x40-DB]=1;d[CONF+0x3f-DB]=1;d[CONF+0x45-DB]=1
 if kind=='array':put(d,SET+4,0)
 elif kind=='entry':put(d,ARRAY,0)
 elif kind=='reference':put(d,ENTRY+4,0)
 elif kind=='object':put(d,REF,0x600000)
 elif kind=='configuration':put(d,SET+12,0)
 elif kind=='matrix':put(d,SET+20,0)
 else:d[CONF+0x40-DB]=0;put(d,SET+20,0);put(d,SET+16,0)
 cases.append(('fault_'+kind,bytes(d),False))

engines=[Engine(False),Engine(True)];started=time.monotonic();summary=dict(return_pairs=0,fault_pairs=0,control_model_checks=0,helpers={},case_kinds=dict(collections.Counter(k for k,d,m in cases)),fpscr_states=[0,0x00400000,0x01000000,0x03c0009f],max_steps=0)
for fpscr in summary['fpscr_states']:
 for i,(kind,data,model) in enumerate(cases):
  a,h,steps=engines[0].run(data,fpscr);b,_,steps2=engines[1].run(data,fpscr)
  assert a==b,(fpscr,i,kind,{k:(a[k],b[k]) for k in a if k!='memory' and a[k]!=b[k]},[hex(DB+j) for j,(x,y) in enumerate(zip(a['memory'],b['memory'])) if x!=y][:20])
  if model:
   expected=control_model(data);assert a['error'] is None and (a['result'],a['calls'])==expected,(fpscr,i,kind,a['error'],a['calls'],expected)
   summary['control_model_checks']+=2
  summary['fault_pairs' if a['error'] else 'return_pairs']+=1
  summary['max_steps']=max(summary['max_steps'],steps,steps2)
  for k,v in h.items():summary['helpers'][k]=summary['helpers'].get(k,0)+v
summary.update(seconds=time.monotonic()-started,finished_utc=datetime.datetime.now(datetime.timezone.utc).isoformat(),candidate_sha256=hashlib.sha256(CAND).hexdigest(),code_sha256=hashlib.sha256(CODE).hexdigest(),emulator='Unicorn ARM1176, VFP enabled; original callees execute without hooks or models',cases_per_fpscr=len(cases))
(OUT/'replay-summary.json').write_text(json.dumps(summary,indent=2)+'\n');print(json.dumps(summary,indent=2))
```

<!-- replay-file: build/effect-math-import/preserve.py -->
```python
from pathlib import Path
import sys,json,subprocess,hashlib,time,datetime
from elftools.elf.elffile import ELFFile
R=Path.cwd();sys.path.insert(0,str(R))
from tools.low.checkExactBytes import _read_map
OUT=R/'build/effect-math-import';OUT.mkdir(exist_ok=True)
p=R/'data/ver/eu/map.csv';baseline=p.read_bytes();rows=_read_map(p)
accepted={(r['Symbol'] or f"fn_{r['Start']:08X}"):r for r in rows if r['Rank']=='O' and 'f' in r['Type']}
assert len(accepted)==717,len(accepted)
records=[]
for obj in sorted((R/'build/eu/obj').rglob('*.o')):
 if not obj.with_suffix('.provenance.json').exists():continue
 provenance=json.loads(obj.with_suffix('.provenance.json').read_text())
 if Path(provenance['source']).suffix not in ('.cpp','.cc','.cxx'):continue
 if not provenance['source'].startswith(('Game/','lib/')):continue
 with obj.open('rb') as stream:
  elf=ELFFile(stream);table=elf.get_section_by_name('.symtab')
  if table is None:continue
  for sym in table.iter_symbols():
   if sym.name in accepted and isinstance(sym['st_shndx'],int) and sym['st_shndx'] and sym['st_info']['type']=='STT_FUNC':
    records.append((sym.name,str(obj.relative_to(R))))
assert len(records)==736,len(records)
found={s for s,o in records};assert found==set(accepted),set(accepted)-found
print('accepted roots',len(accepted),'actual definitions',len(records),flush=True)
checks=[];began=time.monotonic();start=datetime.datetime.now(datetime.timezone.utc).isoformat()
try:
 for i,(sym,obj) in enumerate(records):
  c=subprocess.run([sys.executable,'tools/check.py',sym,'--object',obj],capture_output=True,text=True)
  checks.append(dict(symbol=sym,object=obj,returncode=c.returncode,output=c.stdout+c.stderr))
  if c.returncode or 'O -> O: The complete source-generated function interval matches byte for byte.' not in c.stdout:print('FAIL',sym,obj,c.stdout+c.stderr,flush=True)
  if i%100==99:print('checked',i+1,flush=True)
finally:p.write_bytes(baseline)
summary=dict(base='8991bec8cb27ff1a2dac0003f059d270a0b17493',commit=subprocess.check_output(['git','rev-parse','HEAD'],text=True).strip(),accepted_roots=len(accepted),actual_definitions=len(records),passed=sum(c['returncode']==0 for c in checks),checks=checks,map_sha256=hashlib.sha256(baseline).hexdigest(),map_restored=p.read_bytes()==baseline,started_utc=start,finished_utc=datetime.datetime.now(datetime.timezone.utc).isoformat(),seconds=time.monotonic()-began)
(OUT/'preservation.json').write_text(json.dumps(summary,indent=2)+'\n')
print(json.dumps({k:v for k,v in summary.items() if k!='checks'},indent=2),flush=True)
assert len(checks)==736
assert all(c['returncode']==0 and 'O -> O: The complete source-generated function interval matches byte for byte.' in c['output'] for c in checks)
```

<!-- replay-file: build/effect-math-import/activate-root.py -->
```python
from pathlib import Path
import subprocess,time,datetime,hashlib,json,shutil
from elftools.elf.elffile import ELFFile
R=Path.cwd();OUT=R/'build/effect-math-import/activated';OUT.mkdir(parents=True,exist_ok=True)
p=R/'data/ver/eu/map.csv';saved=p.read_bytes();expected='2dde00f86cc0bc6f174f3a281853a4faacea1f16b6b628d602bea687b4cdb805'
assert hashlib.sha256(saved).hexdigest()==expected
obj=R/'build/eu/obj/lib/al/src/Effect/alEffectSetUpdateFunction.o';obj_hash=hashlib.sha256(obj.read_bytes()).hexdigest()
lines=saved.decode().splitlines(keepends=True)
for i,line in enumerate(lines):
 if line.startswith('0x001EA220,'):
  fields=line.split(',');assert fields[4]=='U' and fields[6]=='';fields[4]='M';fields[6]='fn_001EA220';lines[i]=','.join(fields)
result=dict(started_utc=datetime.datetime.now(datetime.timezone.utc).isoformat(),map_base_sha256=expected)
try:
 p.write_text(''.join(lines));(OUT/'activation-map.csv').write_bytes(p.read_bytes());begin=time.monotonic()
 with (OUT/'normal-build.log').open('w') as log:run=subprocess.run(['.venv/bin/python','make.py','eu'],stdout=log,stderr=subprocess.STDOUT)
 result.update(build_exit=run.returncode,build_seconds=time.monotonic()-begin);assert run.returncode==0
 assert hashlib.sha256(obj.read_bytes()).hexdigest()==obj_hash
 for name in ['RE-Pepper.axf','RE-Pepper.map','RE-Pepper.ld']:
  shutil.copyfile(R/'build/eu'/name,OUT/name)
 with (OUT/'RE-Pepper.axf').open('rb') as stream:
  elf=ELFFile(stream);syms=[s for s in elf.get_section_by_name('.symtab').iter_symbols() if s.name=='fn_001EA220'];assert len(syms)==1
  sym=syms[0];result.update(root_size=sym['st_size'],root_address=sym['st_value']);assert sym['st_size']==252
 text=(OUT/'RE-Pepper.map').read_text();lines=[l.strip() for l in text.splitlines() if 'fn_001EA220' in l and 'alEffectSetUpdateFunction.o' in l]
 assert lines, '252-byte source member must appear in normal linker map'
 result['source_map_lines']=lines
 cmd=['.venv/bin/python','tools/check.py','fn_001EA220','--object',str(obj)]
 c=subprocess.run(cmd,capture_output=True,text=True);(OUT/'object-check.log').write_text(c.stdout+c.stderr)
 result['check_output']=c.stdout+c.stderr;assert 'M -> m: The linked candidate differs from the unchanged original interval.' in c.stdout
finally:
 p.write_bytes(saved);result['map_restored']=p.read_bytes()==saved
 result['finished_utc']=datetime.datetime.now(datetime.timezone.utc).isoformat();(OUT/'summary.json').write_text(json.dumps(result,indent=2)+'\n')
# Put compact artifacts back in agreement with the restored map after freezing the proof.
with (OUT/'restored-base-build.log').open('w') as log:run=subprocess.run(['.venv/bin/python','make.py','eu'],stdout=log,stderr=subprocess.STDOUT)
assert run.returncode==0;assert hashlib.sha256(obj.read_bytes()).hexdigest()==obj_hash;assert p.read_bytes()==saved
result['restored_base_build_exit']=run.returncode;(OUT/'summary.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
```
