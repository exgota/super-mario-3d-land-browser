# NoteObjGenerator initialization packet response

## Result

Zero exact functions and zero exact bytes. The complete `NoteObjGenerator::init` is retained under `NON_MATCHING` as an ordinary C++ reconstruction with bounded behavior evidence. It compiles through committed-source `make.py eu` and through the final `make.py eu -ca`; both normal builds link and export. There is no gameplay or full initialization equivalence claim.

The unchanged canonical command

```sh
python tools/check.py _ZN16NoteObjGenerator4initERKN2al13ActorInitInfoE --object build/eu/obj/Game/backup/src/MapObj/NoteObjGenerator.o -s
```

returns exit 1 with:

```text
Source closure rejected: An unresolved source helper is referenced by a non-branch relocation.
```

The wrapper temporarily supplies only names on the existing anonymous function rows and restores every map byte in a `finally` block. It does not add or change any function/data boundary, classification, pool, rank or compiler flag. No checker, tool, ledger, STATE, configuration, binary or generated object is committed.

A separate, explicitly noncanonical link uses independently observed whole-table identities omitted by the current split data rows. It links the exact project-generated root section without editing its bytes. This diagnostic measures **644 bytes versus 644 retail bytes, with 8 differing bytes**. The complete 36-byte literal pool is equal. These measurements cannot confer canonical exact credit.

## Source forms and remaining differences

Baseline is immutable main `e87d556cd4d993606a03e205b4604d863d718bd4`. The four prior meaningful forms remain recorded in `project/pro_requests/00171334.md`.

Form 5 replaces the prior shared mode assignment with explicit constant stores: absent/-1 resets to 0, explicit 1 stores 1, explicit 2 stores 2, and other values retain the old byte. The resulting 644-byte section closes the packet's four-byte size gap. Form 6 tests a concretely typed group field and NoteObj loop pointer. Form 7 adds a typed inline group-registration operation. Both produce the same 644 bytes and the same eight diagnostic differences. The final source restores the smallest form 5; that restoration is not a new structural attempt. Seven distinct forms total, including the packet's four, were sufficient to locate the remaining limitation.

The remaining three instructions differ only in register choice and scheduling around the virtual group-registration call at 0x171520..0x171528. Retail moves the actor into r1 before loading the vtable/function through r2; the candidate loads through r1/r2 and moves the actor into r1 immediately afterward. The complete following code and pool remain at their original offsets. No arbitrary padding, volatile field or compiler flag was introduced to force the schedule.

`_80` changes from void pointer to signed integer and `_84` from bool to u8. The initializer uses `_80` as an eight-byte table index and preserves/stores mode values 0/1/2 in `_84`, so these are grounded layout corrections. Offsets and object extent do not change. The accepted constructor's initializer spellings become integer zero; its complete 112-byte canonical body still matches.

Eighteen existing canonical accepted definitions pass again: the constructor, four action-name wrappers and inherited executeOnEnd in NoteObjGenerator.o; NoteObj::initAfterPlacement; and all eleven accepted definitions emitted by alActorFactory.o, including both relevant actor creator templates. This is a targeted preservation check, not a claim that the initializer is exact.

## Independent data identities and acceptance prerequisites

The diagnostic importer validates both whole three-word group tables directly from the unchanged EU input. Each has two zero ABI words followed by the already accepted `LiveActorGroup::registerActor` entry 0x1CAB14. NoteObjGroup's whole table is 0x3D66C8..0x3D66D4, address point 0x3D66D0; BeatBlockGroup's is 0x3D66E0..0x3D66EC, address point 0x3D66E8. Existing data rows split these whole ABI objects, so the project map cannot currently express their actual vtable symbols without a separate reviewed data-ownership repair. These descriptive class names are source hypotheses, not recovered original symbols.

The Note collector's independent initializer at 0x388B08 loads the literal at 0x388B58 (0x42FD7C), then calls the 12-byte `NerveActionCollector` constructor at 0x211C28. Its complete BSS identity has no production row. The diagnostic maps the source collector section there only for this explicitly bounded link. No BSS row is added to the canonical map. The six sequence definitions stay in the existing 48-byte row at 0x3BEAC4.

The actor slots at 0x3CC200 and 0x3C9998 independently identify these roots. The connector constructor alias is tied to the existing 0x270F00..0x270F40 function interval by its observed 0x18-byte layout and writes. Existing function extents are asserted unchanged before diagnostic linking. All additional diagnostic data identities are disclosed in the replay code and local JSON, never silently substituted into canonical checking.

Integration still requires separately reviewed data identities, then another unchanged canonical check. Even after those data repairs, the retained source differences would remain nonexact.

## Bounded ARM11 replay

The unchanged complete original root and the retained canonical C++ compiler root agree on **750 returning pairs and 30 separately reported fault pairs**, over 156 constructed fixtures and five FPSCR modes. All 152 statically reachable root instructions are observed. This measures instruction coverage, not every control-flow edge or all possible game states. Five one-sided input mutations are rejected as unequal, confirming sensitivity to relevant state changes.

The replay executes the original group constructor and registerActor body. It also executes original makeActorDeadAll, the original NoteObj constructor, and the original connector constructor. Calls to the base actor constructor, allocation, placement/argument retrieval, rail operations, resource setup, scene lookup, nerve setup and actor virtual callbacks are explicit controlled boundaries. The function list is reproduced in the local JSON generated by the appendix. These boundary models are not claimed as reconstructed game implementations.

Fixtures cover all six unchanged retail sequence definitions, both rail states, absent and signed boundary duration values, every supported mode plus retained unexpected bytes, signed zero/subnormal/infinite/NaN and rounding-sensitive float words, and failures of the note-pointer array, group, group storage, child allocations and connector allocations. Connector nulls can return normally and are retained in the pointer array; child/group/storage failures are separately reported as original-equivalent faults. The startup-populated unit quaternion is a controlled input at its independently observed address; complete global startup is not replayed.

Comparison includes ordered modeled-call events with normalized borrowed local out-pointer identity, their input/output words, full actor bytes, allocated object/array bytes, group-label content and final FPSCR. Callee-saved integer/VFP registers and SP must be preserved on return. Only the borrowed local pointer identity and the group label pointer are normalized; the latter is separately compared by complete label content. No game data or generated ARM binary is present in this report's committed artifact.

## Reproducibility

Place the authorized private EU code/exheader and approved ARMCC/wibo in their normal ignored project paths. Use the project's Python dependencies plus Unicorn 2.1.4. Save the following four blocks under `build/analysis/` with the shown filenames. They work for either class using an explicit argument, including after both sources are integrated. The checker is expected to reject on this frozen baseline; the diagnostic link is separate and never grants exact credit.

```sh
mkdir -p build/analysis
. ./development_environment.sh
export DEVKITARM=/usr # Linux installation only
python make.py eu -ca
python build/analysis/check_actor.py NoteObjGenerator
python build/analysis/link_actor.py NoteObjGenerator
python build/analysis/replay_actor.py NoteObjGenerator
python build/analysis/preserve_actor.py NoteObjGenerator
```

The final clean build links and exports; the original executable SHA256 is `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`. The restored map SHA256 is `a8cd649c372ae92c33ae26d4c4a90dc84f257e549e6465b292e419a2a2ce9b97`.

- Source SHA256: `702613571e11fa1a5582c2ae9b513b153bfca4bd321617734ca22124400485a1`
- Header SHA256: `eb176af61e94bdc605502c4c8f2b098c535f94c73f12f434c039e57c9b77d4b8`
- Canonical object SHA256: `d30fb2837fa7102ba8d6906876fd4aaa7d67ddb957fbea866b146d3b386e936d`
- Noncanonical linked root SHA256: `ad8e47c3642bfe6534ec65cffe9cca0b8f5a41dfaa7f85dfae0c59a2ea640703`
- Equal full pool SHA256: `a4186dfdfde6f7b82683563a01bc82134627d187992b70af13c45036c1bb098b`

No per-form labor or CPU timing claim is made; source reasoning and builds shared the cloud host with other lanes.

### check_actor.py

```python
from pathlib import Path
import subprocess,hashlib,json,sys
name=sys.argv[1];assert name in ('NoteObjGenerator','BeatBlockHolder')
out=Path('build/analysis')/name;out.mkdir(parents=True,exist_ok=True)
root=0x171334 if name=='NoteObjGenerator' else 0x156758
names={root:'_ZN'+str(len(name))+name+'4initERKN2al13ActorInitInfoE'}
if name=='NoteObjGenerator':names[0x270f00]='_ZN20ActorMatrixConnectorC1EPN2al9LiveActorEbb'
else:names[0x31a454]='_ZN9BeatBlockC1ERKN4sead14SafeStringBaseIcEE'
p=Path('data/ver/eu/map.csv');original=p.read_bytes();lines=original.decode().splitlines(keepends=True);found=set()
for i,line in enumerate(lines):
 if not line.startswith('0x'):continue
 cells=line.split(',');address=int(cells[0],16)
 if address in names:
  assert not cells[6].strip(),(address,cells[6]);cells[6]=names[address];lines[i]=','.join(cells);found.add(address)
assert found==set(names)
try:
 p.write_text(''.join(lines));r=subprocess.run(['python','tools/check.py',names[root],'--object',f'build/eu/obj/Game/backup/src/MapObj/{name}.o','-s'],text=True,capture_output=True)
 (out/'canonical-check.log').write_text(r.stdout+r.stderr);print(r.stdout+r.stderr);print('exit',r.returncode)
 assert r.returncode==1 and 'An unresolved source helper is referenced by a non-branch relocation' in r.stdout
finally:p.write_bytes(original)
assert p.read_bytes()==original
print('restored map',hashlib.sha256(original).hexdigest())
```

### link_actor.py

```python
from pathlib import Path
import sys,subprocess,struct,hashlib,json
from elftools.elf.elffile import ELFFile
sys.path.insert(0,str(Path.cwd()))
from tools.low.checkExactBytes import _isolate_function,_read_map
from tools.low.buildProvenance import verify_build_output
name=sys.argv[1];assert name in ('NoteObjGenerator','BeatBlockHolder')
address=0x171334 if name=='NoteObjGenerator' else 0x156758
symbol='_ZN'+str(len(name))+name+'4initERKN2al13ActorInitInfoE'
out=Path('build/analysis')/name;out.mkdir(parents=True,exist_ok=True)
p=Path(f'build/eu/obj/Game/backup/src/MapObj/{name}.o');verify_build_output(p)
rows=_read_map(Path('data/ver/eu/map.csv'))
def mapped(start,end,name,typ='dc',section=''):
 return {'Start':start,'End':end,'Symbol':name,'Type':typ,'SectionName':section}
# These diagnostic identities come only from independent retail constructor/table
# evidence in the packet; the repository map and canonical oracle are unchanged.
if name=='NoteObjGenerator':
 extra=[mapped(0x270f00,0x270f40,'_ZN20ActorMatrixConnectorC1EPN2al9LiveActorEbb','f'),mapped(0x3d66c8,0x3d66d4,'_ZTV12NoteObjGroup'),mapped(0x42fd7c,0x42fd88,'','db','.bss.NoteObjGenerator.cpp')]
else:
 extra=[mapped(0x31a454,0x31a490,'_ZN9BeatBlockC1ERKN4sead14SafeStringBaseIcEE','f'),mapped(0x3d66e0,0x3d66ec,'_ZTV14BeatBlockGroup')]
binary=Path('data/ver/eu/code.bin').read_bytes()
assert hashlib.sha256(binary).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
def word(address):return struct.unpack_from('<I',binary,address-0x100000)[0]
for entry in extra:
 if entry['Type']=='f':
  matches=[row for row in rows if row['Start']==entry['Start'] and 'f' in row['Type']]
  assert len(matches)==1 and matches[0]['End']==entry['End']
assert [word(a) for a in (0x3d66c8,0x3d66cc,0x3d66d0)]==[0,0,0x1cab14]
assert [word(a) for a in (0x3d66e0,0x3d66e4,0x3d66e8)]==[0,0,0x1cab14]
assert word(0x388b58)==0x42fd7c
assert word(0x3cc200)==0x171334 and word(0x3c9998)==0x156758
section,raw,imports=_isolate_function(p,symbol,rows+extra,out/'root.o')
(out/'imports.sym').write_text('#<SYMDEFS>#\n'+''.join(f'0x{x["address"]:08X} {x["kind"]} {x["symbol"]}\n' for x in imports))
(out/'root.sct').write_text(f'LOAD 0x{address:X} {{\n ROOT 0x{address:X} 0x10000 {{ root.o ({section},+FIRST) }}\n}}\n')
cmd=['data/compilers/wibo','data/compilers/4.1/791/bin/armlink.exe','--cpu=MPCore','--fpu=VFPv2','--arm_only','--no_exceptions','--inline','--datacompressor=off','--no_debug','--no_scanlib','--mangled','--symbols','--map',f'--entry={symbol}',f'--keep={symbol}',f'--scatter={out}/root.sct',f'--output={out}/root.axf',f'--list={out}/root.map',str(out/'root.o'),str(out/'imports.sym')]
r=subprocess.run(cmd,text=True,capture_output=True);assert r.returncode==0,r.stdout+r.stderr
with (out/'root.axf').open('rb') as f:
 e=ELFFile(f);allocated=[sec for sec in e.iter_sections() if sec['sh_flags']&2 and sec['sh_size']]
 assert len(allocated)==1 and allocated[0].name=='ROOT' and allocated[0]['sh_addr']==address
 body=allocated[0].data()
binary=Path('data/ver/eu/code.bin').read_bytes();assert hashlib.sha256(binary).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
row=next(r for r in rows if r['Start']==address);target=binary[address-0x100000:row['End']-0x100000]
report={'canonical':False,'source':verify_build_output(p),'diagnostic_data':extra,'imports':imports,'size':len(body),'retail_size':len(target),'different_bytes_over_common_extent':sum(x!=y for x,y in zip(body,target)),'linked_sha256':hashlib.sha256(body).hexdigest()}
pool=int(row['Pool'],16)-address
report['pool_size']=len(target)-pool
report['pool_equal']=len(body)==len(target) and body[pool:]==target[pool:]
report['pool_sha256']=hashlib.sha256(target[pool:]).hexdigest()
(out/'link-report.json').write_text(json.dumps(report,indent=2));print(json.dumps({k:report[k] for k in ['size','retail_size','different_bytes_over_common_extent','linked_sha256']},indent=2))
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
md=Cs(CS_ARCH_ARM,CS_MODE_ARM)
for nm,data in [('retail',target),('source',body)]:
 (out/(nm+'.txt')).write_text('\n'.join(f'{i.address:08x} {i.mnemonic} {i.op_str}' for i in md.disasm(data,address)))
```

### replay_actor.py

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import *
from unicorn.arm_const import *
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM,CS_GRP_JUMP
import hashlib,json,struct,random,sys
NAME=sys.argv[1];assert NAME in ('NoteObjGenerator','BeatBlockHolder')
OUT=Path('build/analysis')/NAME
NOTE=NAME=='NoteObjGenerator';ROOT=0x171334 if NOTE else 0x156758;END_CODE=0x171594 if NOTE else 0x15692c
BINARY=Path('data/ver/eu/code.bin').read_bytes();assert hashlib.sha256(BINARY).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
with (OUT/'root.axf').open('rb') as f:SOURCE=ELFFile(f).get_section_by_name('ROOT').data()
K=0x800000;HOST=K+0x100;INFO=K+0x300;AVT=K+0x500;STRINGS=K+0x1000;HEAP=K+0x10000;SP=0x90f000;STOP=0x980000;APPEAR=STOP+4;DEAD=STOP+8
REG=[UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3,UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11,UC_ARM_REG_R12]
def p32(n):return struct.pack('<I',n&0xffffffff)
def read(u,a):return struct.unpack('<I',u.mem_read(a,4))[0]
def put(u,a,n):u.mem_write(a,p32(n))
def signed(n):return n if n<0x80000000 else n-0x100000000
def string(u,a):
 out=bytearray()
 for i in range(2048):
  x=bytes(u.mem_read(a+i,1))
  if x==b'\0':return out.hex()
  out+=x
 raise AssertionError('unterminated fixture string')
def word(a):return struct.unpack_from('<I',BINARY,a-0x100000)[0]
# Vtable slot identities are read from the unchanged original data, not invented.
NOTE_VT=word(0x312864);BEAT_VT=word(0x31a48c)
NOTE_INIT=word(NOTE_VT+4);NOTE_DEAD=word(NOTE_VT+0x18)
REAL={0x277da0,0x1cab14,0x1cab2c,0x31a454,0x3127c0,0x270f00}
COMMON={0x2932b0,0x292a78,0x280428,APPEAR,DEAD}
NOTE_MODELED={0x27fb44,0x27b690,0x27fcbc,0x27b650,0x27b638,0x27b60c,0x27d238,0x27d180,0x27aff8,0x2693d8,0x227988,0x22735c,0x225ce8,0x277e5c,0x260f9c,0x276588,0x27ae84,NOTE_INIT,NOTE_DEAD}
BEAT_MODELED={0x277de0,0x268144,0x2794f8,0x27d1dc,0x27d180,0x1ebd94,0x27ab64,0x27aaec,0x27a9d4,0x280538,0x27fab8}
class Runner:
 def __init__(self,candidate):
  self.u=u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176)
  for a,n in [(0x100000,0x400000),(K,0x80000),(0x900000,0x10000),(STOP,0x1000)]:u.mem_map(a,n)
  u.mem_write(0x100000,BINARY)
  if candidate:u.mem_write(ROOT,SOURCE)
  u.reg_write(UC_ARM_REG_C1_C0_2,0xf00000);u.reg_write(UC_ARM_REG_FPEXC,0x40000000)
  u.hook_add(UC_HOOK_CODE,self.hook);u.hook_add(UC_HOOK_MEM_INVALID,self.bad);self.cover=set();self.real=set();self.modeled=set()
 def bad(self,u,kind,a,size,value,data):self.fault=[kind,a,size];return False
 def hook(self,u,a,size,data):
  if ROOT<=a<END_CODE:self.cover.add(a)
  if a in REAL:self.real.add(a)
  if a not in COMMON|(NOTE_MODELED if NOTE else BEAT_MODELED):return
  self.modeled.add(a);r=[u.reg_read(x) for x in REG[:4]];f=self.f;ev=[hex(a)];ret=0
  if a in (0x2932b0,0x292a78):
   n=r[0];assert n<=0x10000,('large allocation',n)
   ret=0 if self.alloc_index in f.get('fail',[]) else self.next
   if ret:self.next+=max(0x100,(n+0xff)&~0xff);u.mem_write(ret,bytes([0xa5])*n);self.allocations.append((ret,n))
   ev += [n,ret];self.alloc_index+=1
  elif a==0x280428:
   ev += [r[0],string(u,read(u,r[1]+4))]
   # Model the base constructor only. Child and connector constructors execute retail code.
   u.mem_write(r[0],bytes(0x60));ret=r[0]
  elif a in (APPEAR,DEAD,NOTE_DEAD):ev += [r[0]]
  elif a==NOTE_INIT:ev += r[:2]
  elif a==0x27fb44:ev += [r[0],string(u,r[1]),r[2],r[3]]
  elif a==0x268144:ev += [r[0],string(u,read(u,r[1]+4)),r[2]]
  elif a in (0x27b690,0x27fcbc,0x27b638,0x277de0,0x280538):ev += r[:2]
  elif a==0x27b650:ev += r[:1];ret=f['rail']
  elif a in (0x27b60c,0x27ae84,0x27fab8):ev += r[:1]
  elif a in (0x227988,0x22735c,0x225ce8):ev += r[:2]
  elif a==0x277e5c:ev += r[:3]
  elif a==0x260f9c:ev += r[:1];u.reg_write(UC_ARM_REG_S0,f['length'])
  elif a in (0x27d238,0x27d180,0x27aff8,0x2693d8,0x2794f8,0x27d1dc):
   key={0x27d238:'spacing',0x27d180:'duration' if NOTE else 'nerve',0x27aff8:'sequence',0x2693d8:'mode',0x2794f8:'interval',0x27d1dc:'enable'}[a]
   # Normalize only borrowed local out-pointer identity; retain initial and resulting data.
   before=read(u,r[0]);v=f.get(key)
   if v is not None:put(u,r[0],v);ret=1
   ev += [key,r[1],before,read(u,r[0]),ret]
  elif a==0x1ebd94:ev += r[:3]
  elif a==0x27ab64:ev += r[:1];ret=f['count']
  elif a==0x27aaec:
   ev += r[:2];ret=STRINGS+r[1]*32;u.mem_write(ret,('Beat'+str(r[1])).encode()+b'\0')
  elif a==0x27a9d4:
   ev += r[:3]
   if r[0]:put(u,r[0]+0x60,f['indices'][r[2]])
   if 'callback_maximum' in f:put(u,HOST+0x64,f['callback_maximum'])
  elif a==0x276588:pass # Coin-rotater scene lookup modeled at its published call boundary.
  else:raise AssertionError(('unmodeled boundary',hex(a)))
  self.events.append(ev);u.reg_write(UC_ARM_REG_R0,ret);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 def run(self,f,fpscr):
  u=self.u;self.f=f;self.fault=None;self.events=[];self.allocations=[];self.next=HEAP;self.alloc_index=0
  u.mem_write(K,bytes(0x80000));u.mem_write(0x900000,bytes(0x10000));u.mem_write(HOST,bytes([0xcc])*0x100)
  put(u,HOST,AVT);put(u,AVT+0x10,APPEAR);put(u,AVT+0x18,DEAD)
  if NOTE:
   # Mode starts at arbitrary observed byte values; all six real sequence counts are used.
   put(u,HOST+0x60,0);put(u,HOST+0x64,0);put(u,HOST+0x68,0x41a00000);put(u,HOST+0x6c,0x43480000);put(u,HOST+0x74,180);put(u,HOST+0x80,f.get('initial_sequence',0));u.mem_write(HOST+0x84,bytes([f.get('initial_mode',0)]))
   # Runtime-initialized global unit quaternion, controlled independently of root.
   q=word(0x312868);put(u,q,0);put(u,q+4,0);put(u,q+8,0);put(u,q+12,0x3f800000)
  else:
   for off,v in [(0x60,0),(0x64,0),(0x68,-1),(0x6c,0),(0x70,20),(0x78,0)]:put(u,HOST+off,v)
   u.mem_write(HOST+0x74,b'\0\1\0\0')
  for j,r in enumerate(REG):u.reg_write(r,0xa0000000+j)
  for j in range(16):u.reg_write(UC_ARM_REG_D0+j,0x3ff0000000000000+j)
  u.reg_write(UC_ARM_REG_R0,HOST);u.reg_write(UC_ARM_REG_R1,INFO);u.reg_write(UC_ARM_REG_CPSR,0x10);u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,STOP);u.reg_write(UC_ARM_REG_FPSCR,fpscr)
  try:u.emu_start(ROOT,STOP,count=2000000)
  except UcError:
   if self.fault and f.get('expect_fault'):return {'fault':self.fault,'events':self.events}
   raise AssertionError(('unexpected fault',hex(u.reg_read(UC_ARM_REG_PC)),self.fault,f))
  assert not f.get('expect_fault'),('expected a fault',f)
  assert u.reg_read(UC_ARM_REG_PC)==STOP,('budget',hex(u.reg_read(UC_ARM_REG_PC)))
  assert u.reg_read(UC_ARM_REG_SP)==SP
  assert all(u.reg_read(REG[i])==0xa0000000+i for i in range(4,12))
  assert all(u.reg_read(UC_ARM_REG_D0+i)==0x3ff0000000000000+i for i in range(8,16))
  # Constructors store borrowed string pointers; their bytes are stable because root
  # pools coincide in this retained source. Stack addresses are excluded from snapshots.
  allocations=[]
  for p,n in self.allocations:
   v=bytearray(u.mem_read(p,n))
   if n==0x14:v[4:8]=bytes(4) # compare group label content separately
   allocations.append([p,n,v.hex()])
  group=read(u,HOST+0x60);label=string(u,read(u,group+4)) if group else None
  return {'events':self.events,'actor':bytes(u.mem_read(HOST,0x88 if NOTE else 0x7c)).hex(),'allocations':allocations,'group_label':label,'fpscr':u.reg_read(UC_ARM_REG_FPSCR)}
def fixtures():
 out=[]
 if NOTE:
  for sequence in range(6):
   for rail in (0,1):
    for mode in (None,-1,0,1,2,3,255,-2):out.append({'rail':rail,'sequence':sequence,'mode':mode,'duration':None,'length':0x43c80000})
  for value in (None,-0x80000000,-1,0,1,180,0x7fffffff):out.append({'rail':1,'sequence':5,'mode':2,'duration':value,'length':0x3f800000})
  for initial in (0,1,2,3,255):
   for mode in (None,-1,0,1,2,3,-2):out.append({'rail':0,'sequence':4,'mode':mode,'initial_mode':initial,'length':0x43c80000})
  for bits in (0,0x80000000,1,0x807fffff,0x7f800000,0xff800000,0x7fc12345,0x7f812345,0x3f800001,0x7f7fffff):out.append({'rail':1,'sequence':3,'mode':1,'spacing':bits,'length':bits})
  for fail in (0,1,2,3,4,5,17,18):out.append({'rail':1,'sequence':3,'mode':0,'length':0x43c80000,'fail':[fail],'expect_fault':fail not in (4,18)})
 else:
  for count in (-2,-1,0,1,2,8):
   for enable in (None,-1,0,1,7):
    for nerve in (None,-1,0,1,2):out.append({'count':count,'enable':enable,'nerve':nerve,'indices':list(range(max(0,count)))})
  for indices in ([0],[-1],[-0x80000000],[0x7fffffff],[5,3,7,0],[3,3,3],[-1,-2,-3]):out.append({'count':len(indices),'indices':indices,'interval':-1,'enable':1,'nerve':0})
  for val in (-0x80000000,-1,0,1,2147483647):out.append({'count':3,'indices':[0,7,-3],'callback_maximum':val})
  for fail in range(6):out.append({'count':4,'indices':[1,2,3,4],'fail':[fail],'expect_fault':True})
 return out
if __name__=='__main__':
 a,b=Runner(False),Runner(True);fs=fixtures();faults=0;pairs=0
 for mode in (0,0x1000000,0x2000000,0x3000000,0x400000):
  for i,f in enumerate(fs):
   x,y=a.run(f,mode),b.run(f,mode);pairs+=1;faults+=int('fault' in x)
   if x!=y:
    (OUT/'replay-failure.json').write_text(json.dumps({'fixture_index':i,'fpscr':mode,'fixture':f,'original':x,'source':y},indent=2));raise AssertionError(('mismatch',i,f))
 controls=[]
 baseline=fs[0] if NOTE else {'count':3,'indices':[0,1,2],'enable':0,'nerve':0}
 changes=[{'duration':1},{'mode':1},{'rail':1},{'length':0x3f800000},{'sequence':5}] if NOTE else [{'indices':[7,1,2]},{'enable':1},{'nerve':1},{'interval':1},{'count':1,'indices':[0]}]
 for change in changes:
  altered=dict(baseline);altered.update(change);assert a.run(baseline,0)!=b.run(altered,0),change;controls.append(change)
 md=Cs(CS_ARCH_ARM,CS_MODE_ARM);md.detail=True;pending=[ROOT];reachable=set()
 while pending:
  pc=pending.pop()
  if pc in reachable or not ROOT<=pc<END_CODE:continue
  ins=next(md.disasm(BINARY[pc-0x100000:pc-0x100000+4],pc));reachable.add(pc)
  if ins.group(CS_GRP_JUMP) and ins.operands[0].type==2:pending.append(ins.operands[0].imm)
  ret=('pc' in ins.op_str and ins.mnemonic in ('pop','ldm','ldmia')) or ins.mnemonic=='bx'
  if ins.mnemonic!='b' and not ret:pending.append(pc+4)
 report={'name':NAME,'negative_input_controls':len(controls),'fixtures':len(fs),'pairs':pairs,'returning_pairs':pairs-faults,'fault_pairs':faults,'reachable_root_instructions':len(reachable),'covered_root_instructions':len(a.cover),'uncovered_root':[hex(x) for x in sorted(reachable-a.cover)],'actual_original_callees':[hex(x) for x in sorted(a.real)],'modeled_boundaries':[hex(x) for x in sorted(a.modeled)],'candidate_sha256':hashlib.sha256(SOURCE).hexdigest()}
 (OUT/'replay-results.json').write_text(json.dumps(report,indent=2));print(json.dumps(report,indent=2))
```

### preserve_actor.py

```python
from pathlib import Path
import csv,json,subprocess,sys,hashlib
from elftools.elf.elffile import ELFFile
name=sys.argv[1];assert name in ('NoteObjGenerator','BeatBlockHolder')
out=Path('build/analysis')/name;out.mkdir(parents=True,exist_ok=True)
map_path=Path('data/ver/eu/map.csv');before=map_path.read_bytes();rows={r['Symbol'].strip():r for r in csv.DictReader(before.decode().splitlines()) if r['Rank'].strip()=='O'}
paths=['Game/backup/src/MapObj/NoteObjGenerator.o','Game/backup/src/MapObj/NoteObj.o','lib/al/src/Factory/alActorFactory.o'] if name=='NoteObjGenerator' else ['Game/backup/src/MapObj/BeatBlockHolder.o']
results=[]
for path in paths:
 p=Path('build/eu/obj')/path
 with p.open('rb') as stream:
  e=ELFFile(stream);symbols=[s.name for s in e.get_section_by_name('.symtab').iter_symbols() if s.name in rows and isinstance(s['st_shndx'],int) and s['st_info']['type']=='STT_FUNC']
 for symbol in symbols:
  r=subprocess.run(['python','tools/check.py',symbol,'--object',str(p),'-s'],capture_output=True,text=True)
  results.append({'symbol':symbol,'object':str(p),'exit':r.returncode,'output':r.stdout+r.stderr})
  assert r.returncode==0,(symbol,r.stdout,r.stderr)
assert map_path.read_bytes()==before
(out/'preservation.json').write_text(json.dumps(results,indent=2));print(name,'preserved',len(results),'canonical accepted definitions; map unchanged',hashlib.sha256(before).hexdigest())
```
