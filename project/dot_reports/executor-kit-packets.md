# Holder initialization and kit callback completion

Source checkpoint: `bd313f6`. Base: `e87d556cd4d993606a03e205b4604d863d718bd4`. Branch: `dot/executor-kit-packets`. Reservation: `7a4aed2`. This branch changes neither main's executor-list roots `001E36EC`/`001E2044` nor their bodies.

## Result

Both roots are complete guarded NonMatching proposals. The unchanged project checker rejects both for complete-section size. Exact additions: **0 functions, 0 bytes**. Nothing here advances M2, rendering, or gameplay replay.

| Root | Original complete bytes | Current compiled bytes | Whole interval differing bytes, including unequal tail |
|---|---:|---:|---:|
| ExecuteTableHolderUpdate::init, 001E37D8 | 908 | 912 | 322 |
| LiveActorKit::endInit, 00274990 | 236 | 228 | 133 |

The last column is a diagnostic raw byte comparison, including literal pools, and is not an exactness score or semantic claim. Both canonical outputs say:

```text
U -> M: The complete compiled section, including its literal pool, has a different size from the original interval.
```

The local map was restored after every canonical check batch. Its original bounds, pools, classifications, ranks and symbols are unchanged in the offered branch. The report's diagnostic linker uses the generated section's actual length only to run that C++ output. It does not change the canonical checker, original interval, executable or compiled object, and cannot grant exact credit.

## Source and evidence

`lib/al/src/Execute/alExecuteTableHolderUpdateInit.cpp` supplies the packet's preserved best fourth holder form behind `NON_MATCHING`, in a separate translation unit. All constructors remain external. Their semantic class spellings are proposals; the six original constructor rows, their storage sizes and ABI evidence remain as documented in the packet. The ordinary 16-byte input record, all six category branches, null scalar allocation paths, per-category writes and common insertion are represented. No main-owned helper body is changed.

`lib/al/src/LiveActor/alLiveActorKit.cpp` keeps the existing registration strings and helper calls. It now captures the current group at each loop condition and uses that same group for the current element. `lib/al/include/LiveActor/alLiveActorGroup.h` adds inline count/element accessors over its existing storage. This preserves the original reload of kit+3C, group+C and group+10 after every callback, while keeping accepted layouts and virtual entries unchanged.

The final kit loop has retail's register flow and field offsets, shifted eight bytes earlier. Retail's omitted bytes are the load of kit+30 and `MOV r0,r0`, immediately followed by a load replacing r0. For valid ordinary kit storage, those instructions produce no callback or memory mutation. Their original C++ construct and pointee identity remain unresolved. No empty method, intrinsic, helper address, instruction substitute or flag change was invented to emit them.

## Bounded attempts

The four earlier holder forms and one earlier kit form remain recorded in `project/pro_requests/001E37D8.md` and `project/pro_requests/00274990.md`. Their historical compiler/header evidence limitations remain unchanged.

| Holder distinct form | Change | ARMCC791 complete bytes |
|---|---|---:|
| 1–4, prior packet | Preserved prior forms | 924 / 900 / 912 / 912 |
| 5, 6fd29bd | Grounded PtrArray views and typed common result | 928 |
| 6, 9926a57 | Inline append returning the inserted object | 912 |
| 7, 6be78d1 | Buffer reference to preserve post-constructor reload | 928 |
| 8, eada9b6 | Shared void-pointer append implementation | 928 |

Form6 also moved buffer reads before constructors, an undesirable callback-alias contract. Form7 corrected that but duplicated branch append code. The pass stopped at eight distinct holder forms. The final commit repeats the already preserved form4, rather than adding a ninth form. No exact claim follows from any form. Earlier forms' raw objects/source remain under ignored `build/executor-kit/holder-form*` directories. Only the final canonical output is proposed for intake.

Kit form2 directly re-read the array and emitted244 bytes; form3 captured the group in the condition and emitted232; form4 added direct inline accessors and emits228. Together with the prior cached-array form, this is four distinct kit forms. Source was committed before each normal project build. Only791 was used in this pass; no alternative flags or compiler rescue was attempted.

## Whole-root execution

The final committed C++ root sections were linked by ARMCC's linker, loaded into independent ARM1176 Unicorn machines, and compared against execution of the original root. The original executable is hash checked first. No target instructions are copied into a candidate body.

Holder: **173 returning pairs** and **10 separate allocation-fault pairs**, across183 cases. Returning comparisons cover complete holder storage, every allocated region, deterministic allocation requests/results and explicit alias-mutation storage. Callee-saved general registers and SP are checked on normal returns. Inputs include every recognized kind, unknown/case-mismatched/empty kinds, zero and positive capacities, mixed/random order lists, negative/zero signed counts, six scalar-allocation failure positions, buffer replacement during allocation and count shrinkage during allocation. The ten failed-array cases compare fault kind/address/width and partial state; they are not counted as successful executions.

The original string comparison, kind/count providers and all six actual constructors run without behavioral stubs in both machines. Across the original side, the count helper ran549 times, string comparison8842, kind helper3474, actor constructors411 combined, layout129, execute131, functor128 and the shared name base799. Only the two external allocation services are controlled hooks for this holder replay. This validates bounded caller/real-helper composition, not the allocator implementation, arbitrary corrupt objects, reinitialization of populated holders, or a complete game scene.

Kit: **43 returning pairs**. Its six external helper addresses and actor virtual callback are controlled hooks that record calls/arguments, clobber caller-saved registers, and apply declared state changes. Cases include signed empty counts, zero/eight actors, callback growth/shrink, next-actor replacement, storage replacement and whole-group replacement, plus changes before the loop. Unknown kit+30 is tested with zero and a non-dereferenced nonzero value. Final memory and callback traces agree against original execution and a separate collection model. This does not validate the six kit helper bodies or real actor callback implementations.

Examples: replacing the four-entry group's storage after actor0 visits `[0,5,6,7]`; replacing the group itself has the same trace with the selected replacement group's count; shrinking after actor0 visits `[0]`; growing after actor0 visits `[0,1,2,3,4,5]`. A cached pointer/count loop would violate these cases.

## Accepted preservation and build

The final serialized `python make.py eu -ca` compiled, linked and exported successfully in28.232 seconds. An early duplicate launch hit the provenance projection guard; that output is not used as clean-build evidence. All final checks and replay were repeated after the successful clean build.

The affected current canonical objects are holder update, LiveActor, LiveActorGroup and LiveActorKit. All **35 accepted definitions** in those objects pass the unchanged canonical `--object` checker after the header/source changes, including the holder constructor and registration methods, kit constructor/accessors, group operations and accepted actor callbacks/thunks. This is an affected-source preservation check, not a new full651-root audit.

## Immutable measurements

holder source SHA256: `347a1402f73bd0888bdded6f8718a7a1c9ba3dc06755213b80da5f16685391f8`. Canonical object SHA256: `65006ef2b3afefb1b83d2501e63522da399154f84ace983a7a137aba6b00797b`. Diagnostic linked section SHA256: `67f04ee207cf709a28b9735dc8573da38c377ea986850b273203945925987cb5`.

kit source SHA256: `b86092213b42c1732eaa554e04d8a9e806364c6bea26d7d1553d666a103678f6`. Canonical object SHA256: `3e2013e31d8190829f06ce799ddf72a96536e70cd99f359db566cdbd5726e476`. Diagnostic linked section SHA256: `81ff90e3eb614da1bf335e36a72fb3358a10e2c98f4de9c39ba369ce302ea1dc`.

Restored map SHA256: `a8cd649c372ae92c33ae26d4c4a90dc84f257e549e6465b292e419a2a2ce9b97`. Original executable SHA256: `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.

## Replay appendix

All paths below are repository relative. The worktree needs the approved ARMCC791/wibo toolchain, private `data/ver/eu/code.bin` and `exh.bin`, project Python dependencies and Unicorn2.1.4. `development_environment.sh` must activate the local environment first. None of the original data, generated binary, executable disassembly, tool changes, maps or acceptance bookkeeping is committed.

Extract the three complete scripts embedded below into ignored build storage, then rebuild, check, replay and verify accepted preservation:

```sh
. ./development_environment.sh
python - <<'PYEXTRACT'
from pathlib import Path
import re
report = Path('project/dot_reports/executor-kit-packets.md').read_text()
for name, code in re.findall(r'<!-- replay-file: ([^>]+) -->\n```python\n(.*?)\n```', report, re.S):
    path = Path(name)
    assert path.parts[:2] == ('build', 'executor-kit')
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(code + '\n')
PYEXTRACT
python make.py eu -ca
python build/executor-kit/prepare.py
python build/executor-kit/replay.py
python build/executor-kit/preserve.py
git diff --exit-code -- data/ver/eu/map.csv
```

`prepare.py` changes only existing-row symbol names transiently, saves/restores the full map in a finally block, calls official canonical checks, and produces clearly separated diagnostic links. Its manifest records every imported original address, complete sizes and hashes. `replay.py` contains the complete controlled service/callback behavior and deterministic case generation. `preserve.py` discovers all accepted definitions in the affected canonical objects and restores checker bookkeeping.

Expected results are173 holder returning pairs,10 holder fault pairs,43 kit pairs, and35/35 accepted checks. No exact new match is expected. JSON and logs are written under `build/executor-kit/final/`.

<!-- replay-file: build/executor-kit/prepare.py -->
```python
from pathlib import Path
import hashlib,json,os,subprocess,sys
ROOT=Path.cwd(); sys.path.insert(0,str(ROOT))
from tools.low.checkExactBytes import _read_map,_isolate_function,check_exact_bytes
from tools.low.buildProvenance import verify_build_output
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
OUT=ROOT/'build/executor-kit/final';OUT.mkdir(parents=True,exist_ok=True)
ALIASES={0x1e43d4:'_ZN2al21ExecutorActorMovementC1EPKci',0x1e43bc:'_ZN2al21ExecutorActorCalcAnimC1EPKci',0x1e85a4:'_ZN2al29ExecutorActorMovementCalcAnimC1EPKci',0x1e3bc8:'_ZN2al20ExecutorLayoutUpdateC1EPKci',0x1e7618:'_ZN2al15ExecutorExecuteC1EPKci',0x1dc890:'_ZN2al15ExecutorFunctorC1EPKc',0x1bfd70:'_ZN2al12EffectSystem10startSceneEv',0x1d44e0:'_ZN2al17CollisionDirector7endInitEv',0x1bd91c:'_ZN2al11FogDirector7endInitEv',0x1c2bc0:'_ZN2al16ClippingDirector7endInitEv'}
TARGETS=[('holder','Execute/alExecuteTableHolderUpdateInit','_ZN2al24ExecuteTableHolderUpdate4initEPKNS_12ExecuteOrderEi',0x1e37d8,908),('kit','LiveActor/alLiveActorKit','_ZN2al12LiveActorKit7endInitEv',0x274990,236)]
def sha(b):return hashlib.sha256(b).hexdigest()
p=ROOT/'data/ver/eu/map.csv'; baseline=p.read_bytes();manifest={}
try:
 lines=baseline.decode().splitlines()
 for n,line in enumerate(lines):
  f=line.split(',')
  try:a=int(f[0],16)
  except ValueError:continue
  if a in ALIASES:
   assert len(f)>6
   f[6]=ALIASES[a];lines[n]=','.join(f)
 p.write_text('\n'.join(lines)+'\n'); rows=_read_map(p)
 for label,src,sym,start,size in TARGETS:
  obj=ROOT/('build/eu/obj/lib/al/src/'+src+'.o');directory=OUT/label;directory.mkdir(exist_ok=True)
  provenance=verify_build_output(obj)
  check=subprocess.run([sys.executable,'tools/check.py',sym,'--object',str(obj)],capture_output=True,text=True)
  (directory/'canonical-check.txt').write_text(check.stdout+check.stderr)
  result=check_exact_bytes(sym,obj,output_directory=directory/'canonical')
  (directory/'canonical-result.json').write_text(json.dumps(result,indent=2)+'\n')
  section,raw,imports=_isolate_function(obj,sym,rows,directory/'candidate.o')
  # Diagnostic execution only: actual generated extent, never target/map changes.
  (directory/'symbols.sym').write_text('#<SYMDEFS>#\n'+''.join(f'0x{i["address"]:08X} {i["kind"]} {i["symbol"]}\n' for i in imports))
  (directory/'candidate.sct').write_text(f'LOAD 0x{start:X}\n{{\n CODE 0x{start:X} 0x{len(raw):X}\n {{\n candidate.o ({section}, +FIRST)\n }}\n}}\n')
  command=[str(ROOT/'data/compilers/wibo'),str(ROOT/'data/compilers/4.1/791/bin/armlink.exe'),'--cpu=MPCore','--fpu=VFPv2','--arm_only','--no_exceptions','--inline','--datacompressor=off','--no_debug','--no_scanlib','--mangled','--symbols','--map',f'--entry={sym}',f'--keep={sym}',f'--scatter={directory}/candidate.sct',f'--output={directory}/candidate.axf',f'--list={directory}/candidate.map',str(directory/'candidate.o'),str(directory/'symbols.sym')]
  env=dict(os.environ,TMP='/tmp');r=subprocess.run(command,capture_output=True,text=True,env=env);(directory/'link.log').write_text(r.stdout+r.stderr);assert r.returncode==0,r.stdout+r.stderr
  with (directory/'candidate.axf').open('rb') as f:
   elf=ELFFile(f);secs=[s for s in elf.iter_sections() if s['sh_flags']&2 and s['sh_size']];assert len(secs)==1;linked=secs[0].data();assert secs[0]['sh_addr']==start
  (directory/'candidate.bin').write_bytes(linked)
  original=(ROOT/'data/ver/eu/code.bin').read_bytes()[start-0x100000:start-0x100000+size]
  info=dict(source=src,source_sha256=sha((ROOT/('lib/al/src/'+src+'.cpp')).read_bytes()),provenance=provenance,original_size=size,generated_size=len(linked),canonical_result=result['reason'],exact=result['exact'],linked_sha256=sha(linked),whole_difference_count=sum(a!=b for a,b in zip(original,linked))+abs(len(original)-len(linked)),imports=imports,link_command=command)
  manifest[label]=info
  cap=Cs(CS_ARCH_ARM,CS_MODE_ARM)
  (directory/'generated.txt').write_text('\n'.join(f'{i.address:08X}: {i.mnemonic} {i.op_str}' for i in cap.disasm(linked,start))+'\n')
  print(label,len(linked),info['canonical_result'])
finally:p.write_bytes(baseline)
assert p.read_bytes()==baseline
manifest['map_restored_sha256']=sha(baseline)
(OUT/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
```

<!-- replay-file: build/executor-kit/replay.py -->
```python
"""Bounded whole-root ARM execution, with explicit external hooks; zero exact credit."""
from pathlib import Path
import hashlib,json,random,struct
from unicorn import Uc,UcError,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE,UC_HOOK_MEM_INVALID
from unicorn.arm_const import *
ROOT=Path.cwd(); OUT=ROOT/'build/executor-kit/final'
CODE=(ROOT/'data/ver/eu/code.bin').read_bytes()
assert hashlib.sha256(CODE).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
BASE=0x1000000;HEAP=BASE+0x100000;STACK=0x800000;STOP=0x700ff0;CALLBACK=0x700100
REGS=[UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3,UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11,UC_ARM_REG_R12]
HELPERS=[0x242cc4,0x242ccc,0x292308,0x1e43d4,0x1e43bc,0x1e85a4,0x2415dc,0x243b94,0x1e3bc8,0x1e7618,0x1dc890]
KHELPERS=[0x1cc9b0,0x1bfd70,0x1cca1c,0x1d44e0,0x1bd91c,0x1c2bc0]
def word(v):return struct.pack('<I',v&0xffffffff)
def setup(label,candidate):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176)
 u.mem_map(0x100000,0x400000);u.mem_write(0x100000,CODE)
 if candidate:u.mem_write(0x1e37d8 if label=='holder' else 0x274990,(OUT/label/'candidate.bin').read_bytes())
 u.mem_map(BASE,0x200000);u.mem_map(STACK,0x10000);u.mem_map(0x700000,0x1000)
 for i,r in enumerate(REGS):u.reg_write(r,0xB0000000+i*0x1111)
 u.reg_write(UC_ARM_REG_SP,STACK+0xff00);u.reg_write(UC_ARM_REG_LR,STOP)
 return u
def read(u,a):return struct.unpack('<I',u.mem_read(a,4))[0]
def put(u,a,v):u.mem_write(a,word(v))
def string(u,a):
 b=bytearray()
 for n in range(512):
  x=bytes(u.mem_read(a+n,1))
  if x==b'\0':return bytes(b)
  b+=x
 raise AssertionError('unterminated string')
def ret(u,value=0):
 for i,r in enumerate([UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3,UC_ARM_REG_R12]):u.reg_write(r,0xD1234000+i)
 u.reg_write(UC_ARM_REG_R0,value);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
def finish(u,start):
 faults=[]
 def invalid(u,access,address,size,value,data):faults.append((access,address,size));return False
 u.hook_add(UC_HOOK_MEM_INVALID,invalid)
 error=None
 try:u.emu_start(start,STOP,count=1000000)
 except UcError as exc:error=exc.errno
 if not error:assert u.reg_read(UC_ARM_REG_PC)==STOP,'instruction bound'
 if not error:
  assert u.reg_read(UC_ARM_REG_SP)==STACK+0xff00
  for i in range(4,12):assert u.reg_read(REGS[i])==0xB0000000+i*0x1111
 return error,faults
KINDS=['ActorMovement','ActorCalcAnim','ActorMovementCalcAnim','LayoutUpdate','Execute','Functor','Unknown']
def holder(case,candidate):
 u=setup('holder',candidate); count=case.get('count',len(case['entries']))
 for i,(kind,cap) in enumerate(case['entries']):
  name=BASE+0x2000+i*0x80;typ=name+0x30
  u.mem_write(name,f'actor_{i}'.encode()+b'\0');u.mem_write(typ,kind.encode()+b'\0')
  u.mem_write(BASE+0x1000+i*16,word(name)+word(typ)+word(cap)+word(0xABCD0000+i))
 next_alloc=HEAP;allocs=[];seen={};scalar=0;array=0;changed=False
 def hook(u,address,size,data):
  nonlocal next_alloc,scalar,array,changed
  if address in HELPERS:seen[address]=seen.get(address,0)+1
  if address not in (0x292a78,0x2932b0):return
  is_scalar=address==0x2932b0
  if is_scalar:scalar+=1
  else:array+=1
  amount=u.reg_read(UC_ARM_REG_R0)
  fail=(is_scalar and scalar==case.get('fail_scalar')) or (not is_scalar and array==case.get('fail_array')) or amount>0x10000
  result=0
  if not fail:
   result=next_alloc;extent=max(16,(amount+15)&~15);next_alloc+=extent
   u.mem_write(result,b'\xA5'*extent)
  allocs.append(('scalar' if is_scalar else 'array',amount,result))
  if is_scalar and not changed and case.get('mutation'):
   changed=True
   if case['mutation']=='shrink':put(u,BASE+0xC,1)
   elif case['mutation']=='actor_buffer':
    source=read(u,BASE+0x1c);target=BASE+0x9000
    u.mem_write(target,bytes(u.mem_read(source,read(u,BASE+0x14)*4)));put(u,BASE+0x1c,target)
  ret(u,result)
 u.hook_add(UC_HOOK_CODE,hook)
 u.reg_write(UC_ARM_REG_R0,BASE);u.reg_write(UC_ARM_REG_R1,BASE+0x1000);u.reg_write(UC_ARM_REG_R2,count&0xffffffff)
 error,faults=finish(u,0x1e37d8)
 return dict(error=error,faults=faults,allocations=allocs,holder=bytes(u.mem_read(BASE,0x44)).hex(),heap=bytes(u.mem_read(HEAP,next_alloc-HEAP)).hex(),mutation=bytes(u.mem_read(BASE+0x9000,128)).hex()),seen

def kit(case,candidate):
 u=setup('kit',candidate);a=BASE+0x1000;b=BASE+0x1100;ba=BASE+0x2000;bb=BASE+0x2100;vtable=BASE+0x5000
 put(u,BASE+0x3c,a);put(u,BASE+0x30,case.get('unknown',0))
 for off in [4,8,0x14,0x24,0x28]:put(u,BASE+off,BASE+0x6000+off*16)
 put(u,a+8,8);put(u,a+12,case.get('count',4));put(u,a+16,ba)
 put(u,b+8,8);put(u,b+12,case.get('second_count',4));put(u,b+16,bb)
 actors=[BASE+0x3000+i*0x80 for i in range(8)]
 for i,p in enumerate(actors):put(u,p,vtable);put(u,p+4,i)
 for i in range(8):put(u,ba+i*4,actors[i]);put(u,bb+i*4,actors[(i+4)%8])
 put(u,vtable+8,CALLBACK)
 events=[];visits=[]
 def mutate():
  action=case.get('action')
  if action=='grow':put(u,a+12,6)
  elif action=='shrink':put(u,a+12,0)
  elif action=='switch_group':put(u,BASE+0x3c,b)
  elif action=='switch_buffer':put(u,a+16,bb)
  elif action=='replace_next':put(u,ba+len(visits)*4,actors[7])
 def hook(u,address,size,data):
  if address in KHELPERS:
   r0=u.reg_read(UC_ARM_REG_R0)
   if address==0x1cc9b0:
    f=u.reg_read(UC_ARM_REG_R1);events.append((address,string(u,r0).hex(),read(u,f),read(u,f+4)))
   else:events.append((address,r0))
   if address==0x1c2bc0 and case.get('before_loop'):mutate()
   ret(u,0xdead0000+len(events))
  elif address==CALLBACK:
   visits.append(read(u,u.reg_read(UC_ARM_REG_R0)+4))
   if len(visits)==case.get('after',1) and not case.get('before_loop'):mutate()
   ret(u,0xabcdef00+len(visits))
 u.hook_add(UC_HOOK_CODE,hook);u.reg_write(UC_ARM_REG_R0,BASE)
 error,faults=finish(u,0x274990)
 return dict(error=error,faults=faults,events=events,visits=visits,state=bytes(u.mem_read(BASE,0x7000)).hex())

def expected_visits(case):
 # Independent collection model, not a transcription of ARM instructions.
 a=list(range(8));b=[(i+4)%8 for i in range(8)];storage=a;size=case.get('count',4);index=0;visits=[]
 def mutate():
  nonlocal storage,size
  action=case.get('action')
  if action=='grow':size=6
  elif action=='shrink':size=0
  elif action=='switch_group':storage=b;size=case.get('second_count',4)
  elif action=='switch_buffer':storage=b
  elif action=='replace_next':a[len(visits)]=7
 if case.get('before_loop'):mutate()
 while index<size:
  visits.append(storage[index]);index+=1
  if len(visits)==case.get('after',1) and not case.get('before_loop'):mutate()
  assert len(visits)<=8
 return visits
cases=[dict(entries=[],count=n) for n in [-2,-1,0]]
for kind in KINDS+['actorMovement','']:
 for cap in [0,1,2,3,7]:cases.append(dict(entries=[(kind,cap)]))
for one in KINDS:
 for two in KINDS:cases.append(dict(entries=[(one,1),(two,3)]))
for cap in [0,1,3,5]:
 for order in [KINDS,list(reversed(KINDS))]:cases.append(dict(entries=[(k,cap) for k in order]))
rng=random.Random(0x1e37d8)
for _ in range(60):cases.append(dict(entries=[(rng.choice(KINDS),rng.randrange(9)) for i in range(rng.randrange(1,21))]))
for failure in range(1,7):cases.append(dict(entries=[(k,3) for k in KINDS],fail_scalar=failure))
for failure in range(1,11):cases.append(dict(entries=[(k,3) for k in KINDS],fail_array=failure))
for mutation in ['actor_buffer','shrink']:cases.append(dict(entries=[(k,3) for k in KINDS],mutation=mutation))
summary={'holder_returning_pairs':0,'holder_fault_pairs':0,'holder_case_count':len(cases),'helper_entries':{}}
for n,c in enumerate(cases):
 original,seen=holder(c,False);generated,_=holder(c,True)
 assert original==generated,('holder',n,c,{k:(original[k],generated[k]) for k in original if original[k]!=generated[k]})
 summary['holder_fault_pairs' if original['error'] else 'holder_returning_pairs']+=1
 for a,count in seen.items():summary['helper_entries'][f'{a:08X}']=summary['helper_entries'].get(f'{a:08X}',0)+count
kit_cases=[dict(count=n) for n in [-4,-1,0,1,2,3,4,6,8]]
for action in ['grow','shrink','switch_group','switch_buffer','replace_next']:
 for after in [1,2,3]:
  for unknown in [0,0xfeedbeef]:kit_cases.append(dict(action=action,after=after,unknown=unknown))
for action in ['grow','shrink','switch_group','switch_buffer']:kit_cases.append(dict(action=action,before_loop=True))
summary['kit_returning_pairs']=0;summary['kit_mutation_traces']=[]
for n,c in enumerate(kit_cases):
 original=kit(c,False);generated=kit(c,True)
 assert original==generated,('kit',n,c,{k:(original[k],generated[k]) for k in original if original[k]!=generated[k]})
 assert original['error'] is None
 assert original['visits']==expected_visits(c),(c,original['visits'],expected_visits(c))
 summary['kit_returning_pairs']+=1
 if c.get('action'):summary['kit_mutation_traces'].append(dict(case=c,visits=original['visits']))
summary['executable_sha256']=hashlib.sha256(CODE).hexdigest()
summary['candidate_sha256']={k:hashlib.sha256((OUT/k/'candidate.bin').read_bytes()).hexdigest() for k in ['holder','kit']}
(OUT/'replay-summary.json').write_text(json.dumps(summary,indent=2)+'\n')
print(json.dumps({k:v for k,v in summary.items() if k!='kit_mutation_traces'},indent=2))
```

<!-- replay-file: build/executor-kit/preserve.py -->
```python
from pathlib import Path
import json,sys,subprocess
from elftools.elf.elffile import ELFFile
ROOT=Path.cwd();sys.path.insert(0,str(ROOT))
from tools.low.checkExactBytes import _read_map
rows=_read_map(ROOT/'data/ver/eu/map.csv');accepted={r['Symbol']:r for r in rows if r['Rank']=='O' and 'f' in r['Type'] and r['Symbol']}
paths=set()
for p in (ROOT/'build/eu/obj').rglob('*.provenance.json'):
 j=json.loads(p.read_text())
 if 'LiveActor/alLiveActorGroup.h' in json.dumps(j['inputs']) or j['source'].endswith('Execute/alExecuteTableHolderUpdate.cpp'):
  paths.add(p.with_suffix('').with_suffix('.o'))
checks=[];baseline=(ROOT/'data/ver/eu/map.csv').read_bytes()
try:
 for p in sorted(paths):
  with p.open('rb') as f:
   e=ELFFile(f);symbols=[s.name for s in e.get_section_by_name('.symtab').iter_symbols() if isinstance(s['st_shndx'],int) and s.name in accepted]
  for symbol in symbols:
   r=subprocess.run([sys.executable,'tools/check.py',symbol,'--object',str(p)],text=True,capture_output=True)
   checks.append(dict(symbol=symbol,object=str(p.relative_to(ROOT)),exit=r.returncode,output=r.stdout+r.stderr))
finally:(ROOT/'data/ver/eu/map.csv').write_bytes(baseline)
(ROOT/'build/executor-kit/final/preservation.json').write_text(json.dumps(checks,indent=2)+'\n')
print('affected objects',len(paths),'accepted checks',len(checks),'passed',sum(c['exit']==0 for c in checks))
for c in checks:
 if c['exit']:print(c)
assert checks and all(c['exit']==0 for c in checks)
```
