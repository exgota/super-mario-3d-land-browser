# Reproduce graphics binding root 00282D20

These are the executed local verification scripts for the frozen proposal. They
contain generated fixtures and metadata checks, with no executable/resource bytes
copied from the private input. The only root source is the committed C++ TU built
by the unchanged project build. The separate diagnostic AXF is never submitted to
the canonical checker.

Use the approved local code.bin/exh.bin and already approved compiler/wibo/Python
inputs. Source the project environment, with DEVKITARM pointing to installed ARM
binutils. The observed root uses the existing 4.0/902 module. No configuration or
compiler flag change is needed. Capstone, Unicorn and pyelftools are required.

The preservation script expects sibling pristine `mario-main861` at
86104d96a7f570383bbfefc5fffad998e134496c, independently clean-built and already gated.
Its build/dot-baseline-861/report.json has SHA-256
a692f9a29d92c3f8021b62a2254b06fb44776f34cfa2f51849364bde66dee374. To independently
recreate the baseline, run its unchanged acceptance_batch.py with an empty candidate
manifest and a new ignored output directory. That is a fresh full 767/787 gate;
this proposal instead proves old-object/input equivalence against the supplied
baseline. Do not conflate those two checks.

Materialize the fenced files below from this note, then run from the proposal root:

```sh
. ./development_environment.sh
export DEVKITARM=/usr
python - <<'EXTRACT_REPRODUCTION_861'
from pathlib import Path
import re
note=Path('project/dot_reports/texture-binding-state-reproduction.md').read_text()
for name,language,body in re.findall(r'^## (build/texture-binding-state/[^\n]+)\n\n```(python|cpp)\n(.*?)\n```',note,re.M|re.S):
 p=Path(name);p.parent.mkdir(parents=True,exist_ok=True);p.write_text(body+'\n')
EXTRACT_REPRODUCTION_861
python make.py eu -ca > build/texture-binding-state/clean-build.log 2>&1
python build/texture-binding-state/check.py
python build/texture-binding-state/link.py
python build/texture-binding-state/replay.py
python build/texture-binding-state/extended.py
python build/texture-binding-state/cycles.py
python build/texture-binding-state/preserve.py
python build/texture-binding-state/audit.py
```

The canonical checker must reject the 1,924/2,164 extent. Its subprocess exit code
is 1; the wrapper succeeds only at restoring the exact map and recording the
result. Do not interpret the wrapper's shell status as a match. The diagnostic
link's removed-unused-RO warning is observed and does not add helpers to the root.
The base suite reports 314 returning pairs plus nine separate fault diagnostics;
the extension reports 310 returning pairs plus one fault diagnostic. Two of the
ten fault cases differ at original write address 4 versus candidate address 8.
All fault cases are excluded from the 624 passing returning-pair claim.

The cycle script reports eight nonreturning budget-limited runs. They are separate
negative diagnostics. The preservation script checks 179 canonical objects/372
inputs/all 625 baseline tracked files, counts all 181 physical outputs, and audits
the generated stub separately for exactly six function and three data aliases.
Every prior CFI byte and relocation is associated with its function; only generated
frame-CIE ordinal identities normalize. The compact image must equal the baseline.

The optional declaration inventory and syntax check use the local historical
proposal worktrees. Their exact heads/hashes are recorded by declarations.py.
They are not prerequisites for the ARM replay. With the completed graphics
proposal at sibling mario-graphics-integration, the final signature test is:

```sh
python build/texture-binding-state/declarations.py
c++ -fsyntax-only -Ilib/CtrSDK/include -I../mario-graphics-integration/lib/CtrSDK/include build/texture-binding-state/signature-compatibility.cpp
```

The passing syntax test proves only the stated shared declarations are compatible.
It does not combine source families or resolve remaining historical conflicts.

A packaging heredoc collision at 05:53 UTC accidentally invoked command examples
outside the venv, producing import failures and overwriting current wrapper logs.
It caused no C++ changes and the map wrapper restored its bytes. The clean build,
checker and all execution/preservation scripts were rerun from the sourced venv.
The failure remains recorded locally in packaging-failure.txt and its saved logs.

## build/texture-binding-state/check.py

```python
from pathlib import Path
import subprocess,sys,hashlib,json
p=Path('data/ver/eu/map.csv'); original=p.read_bytes(); old=b'0x00282D20,0x00283578,0x00283594,          ,U,f,,'; assert original.count(old)==1
try:
 p.write_bytes(original.replace(old,b'0x00282D20,0x00283578,0x00283594,          ,U,f,fn_00282D20,'))
 c=subprocess.run([sys.executable,'tools/check.py','fn_00282D20','--object','build/eu/obj/lib/CtrSDK/sources/gx_BindingState.o'],capture_output=True,text=True)
 print(c.stdout,c.stderr);Path('build/texture-binding-state/check-current.log').write_text(c.stdout+c.stderr)
finally:p.write_bytes(original)
assert p.read_bytes()==original
Path('build/texture-binding-state/check-result.json').write_text(json.dumps({'returncode':c.returncode,'stdout':c.stdout,'stderr':c.stderr,'map_restored':True,'map_sha256':hashlib.sha256(original).hexdigest()},indent=2))
```

## build/texture-binding-state/link.py

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
import csv,subprocess,os,json,hashlib
root=Path.cwd();out=root/'build/texture-binding-state';out.mkdir(exist_ok=True)
obj=root/'build/eu/obj/lib/CtrSDK/sources/gx_BindingState.o'
rows=[{k.strip():v.strip() for k,v in x.items()} for x in csv.DictReader((root/'data/ver/eu/map.csv').open())]
syms={x['Symbol']:int(x['Start'],16) for x in rows if x['Symbol']}
with obj.open('rb') as f:
 e=ELFFile(f);names=[s.name for s in e.get_section_by_name('.symtab').iter_symbols() if s['st_shndx']=='SHN_UNDEF' and s.name and not s.name.startswith('Lib$$')]
 lines=[]
 for n in names:
  a=int(n.split('_')[1],16) if n.startswith(('fn_','dat_')) else syms[n]
  kind = 'd' if n.startswith('dat_') else 'f'
  assert any(int(row['Start'],16)==a and kind in row['Type'] for row in rows), (n,hex(a))
  lines.append(f'0x{a:08X} {"D" if n.startswith("dat_") else "A"} {n}')
(out/'symbols.sym').write_text('#<SYMDEFS>#\n'+'\n'.join(lines)+'\n')
(out/'candidate.sct').write_text('CANDIDATE_LOAD 0x00282D20\n{\n CANDIDATE_CODE 0x00282D20\n {\n  gx_BindingState.o (i.fn_00282D20, +FIRST)\n  gx_BindingState.o (+RO)\n }\n}\n')
cmd=[str(root/'data/compilers/wibo'),str(root/'data/compilers/4.0/902/bin/armlink.exe'),'--cpu=MPCore','--fpu=VFPv2','--arm_only','--no_exceptions','--inline','--datacompressor=off','--no_debug','--no_scanlib','--mangled','--symbols','--map','--entry=fn_00282D20','--keep=fn_00282D20',f'--scatter={out}/candidate.sct',f'--output={out}/candidate.axf',f'--list={out}/candidate.map',str(obj),str(out/'symbols.sym')]
p=subprocess.run(cmd,env={**os.environ,'TMP':'/tmp'},stdout=subprocess.PIPE,stderr=subprocess.STDOUT,text=True);print(p.stdout);assert p.returncode==0
print('Diagnostic root link only; canonical checker ran separately.')
```

## build/texture-binding-state/replay.py

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import *
from unicorn.arm_const import *
import struct,json,random,hashlib,itertools,sys,time
ROOT=Path.cwd();OUT=ROOT/'build/texture-binding-state';BINARY=(ROOT/'data/ver/eu/code.bin').read_bytes()
RESULT_NAME='replay-results.json'
BASE=0x282d20;END=0x71000000;DB=0x1000000;STACK=0x70000000;SP=STACK+0x18000
STATE=DB;REGISTRY=DB+0x1000;DEFAULT=DB+0x2000;FIXED=DB+0x3000;HEAP=DB+0x10000;ALLOC=0x40000000;FREE=ALLOC+4
assert hashlib.sha256(BINARY).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
with (OUT/'candidate.axf').open('rb') as f:
 e=ELFFile(f);CANDIDATE=next(s.data() for s in e.iter_sections() if s['sh_addr']==BASE and s['sh_size'])
R=[UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3,UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11,UC_ARM_REG_R12]
U=lambda *v:struct.pack('<'+'I'*len(v),*[x&0xffffffff for x in v])
CALLEES={0x28b290:'make2D',0x28b1e0:'makeCube',0x210850:'insert',0x28d1f0:'zero',0x282650:'delete'}
ALLOWED=[(0x28b1e0,0x28b330),(0x210850,0x2108d4),(0x28d1f0,0x28d244),(0x282650,0x282b84)]
class Engine:
 def __init__(self,case,candidate):
  self.case=case;self.events=[];self.allocs=[];self.free=[];self.cursor=HEAP;self.fixed=FIXED;self.instructions=0;self.returned=False;self.fault=None;self.coverage=set();self.entries={};self.allocno=0
  u=self.u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176);u.mem_map(0x100000,0x400000);u.mem_write(0x100000,BINARY)
  if candidate:u.mem_write(BASE,CANDIDATE)
  self.extent=len(CANDIDATE) if candidate else 2164
  u.mem_map(DB,0x200000);u.mem_map(STACK,0x20000);u.mem_map(END,0x1000);u.mem_map(ALLOC,0x1000)
  u.reg_write(UC_ARM_REG_C1_C0_2,0xf<<20);u.reg_write(UC_ARM_REG_FPEXC,1<<30)
  u.hook_add(UC_HOOK_CODE,self.hook);u.hook_add(UC_HOOK_MEM_INVALID,self.invalid)
  self.w(0x3e3154,STATE);self.w(0x3e3180,REGISTRY,0xffffffff);self.w(0x3e2654,0 if case.get('null_allocator') else ALLOC,FREE)
  self.w(REGISTRY+8,DEFAULT);self.w(STATE+0x58,case.get('unit',0));self.w(STATE,case.get('dirty',0x810001));self.w(STATE+0xf4,case.get('old_set',900))
  self.nodes={};self.payloads={}
  for spec in case.get('nodes',[]):self.add_node(**spec)
  for kind,off,count in [('t2',0x5c,3),('cube',0x68,3),('aux',0x74,32)]:
   vals=case.get(kind,[0]*count);assert len(vals)==count;self.w(STATE+off,*vals)
  self.fill_set(DEFAULT,case.get('defaults',{}))
  for name,values in case.get('set_values',{}).items():self.fill_set(self.payloads[int(name)],values)
  active=case.get('selected',0)
  if active:self.w(REGISTRY+0x8a8,self.nodes[active])
  self.w(REGISTRY+0xc,case.get('pending',0))
  self.w(STATE+0x104,0x01010101,0x0101)
  # Initialize cached node arrays consistently with existing current bindings.
  for off,stateoff,count in [(0x810,0x5c,3),(0x81c,0x68,3),(0x828,0x74,32)]:
   for i in range(count):self.w(REGISTRY+off+4*i,self.nodes.get(self.r(STATE+stateoff+4*i),0))
  self.saved=[0x44440000+i for i in range(8)]
 def r(self,a):return struct.unpack('<I',self.u.mem_read(a,4))[0]
 def w(self,a,*v):self.u.mem_write(a,U(*v))
 def reserve(self,n):
  p=self.fixed;self.fixed+=(n+15)&~15;assert self.fixed<HEAP;return p
 def add_node(self,name,kind=3,payload=True,next=None):
  p=self.reserve(16);q=self.reserve([0x78,0x1cc,0x9c,0x820][kind]) if payload else 0
  if q:
   if kind in (2,3):self.w(q,name)
   if kind==3:self.w(q+0x81c,0xffffffff)
  self.nodes[name]=p;self.payloads[name]=q;self.w(p,q,kind,name,0)
  bucket=REGISTRY+0x10+4*(name&511);first=self.r(bucket)
  if not first or self.r(first+8)>name:self.w(p+12,first);self.w(bucket,p)
  else:
   prev=first;cur=self.r(prev+12)
   while cur and self.r(cur+8)<name:prev=cur;cur=self.r(cur+12)
   self.w(p+12,cur);self.w(prev+12,p)
  if next is not None:self.w(p+12,self.nodes[next])
 def fill_set(self,p,values):
  for key,off,n in [('t2',4,3),('cube',16,3),('aux',28,32)]:
   v=values.get(key,[0]*n);assert len(v)==n;self.w(p+off,*v)
 def poison(self,result=0):
  u=self.u
  for i in [0,1,2,3,12]:u.reg_write(R[i],0xdead0000+i)
  for i in range(16):u.reg_write(UC_ARM_REG_S0+i,0x7fc01000+i)
  u.reg_write(R[0],result);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 def invalid(self,u,access,address,size,value,user):
  self.fault=(access,address,size);return False
 def hook(self,u,pc,size,user):
  self.instructions+=1
  if BASE<=pc<BASE+self.extent:self.coverage.add(pc-BASE)
  elif pc==END:self.returned=True;u.emu_stop();return
  elif pc==ALLOC:
   args=[u.reg_read(r) for r in R[:4]];assert args[:3]==[0x10000,0x100,0],args
   self.allocno+=1;size=args[3];failed=self.allocno==self.case.get('fail_alloc')
   ptr=0 if failed else self.cursor
   if ptr:
    self.cursor+=(size+15)&~15;assert self.cursor<DB+0x200000;u.mem_write(ptr,bytes([0xa5])*size);self.allocs.append((ptr,size))
   self.events.append(('allocate',*args,ptr))
   if self.allocno==1:
    if self.case.get('callback_unit') is not None:self.w(STATE+0x58,self.case['callback_unit'])
    if self.case.get('callback_clear'):self.w(0x3e2654,0)
    if self.case.get('callback_registry'):
     u.mem_write(DB+0x20000,bytes(u.mem_read(REGISTRY,0x8ac)));self.w(0x3e3180,DB+0x20000)
   self.poison(ptr);return
  elif pc==FREE:
   args=[u.reg_read(r) for r in R[:4]];assert args[:3]==[0x10000,0x100,0],args
   self.events.append(('free',*args));self.free.append(args[3]);self.poison();return
  elif not any(a<=pc<b for a,b in ALLOWED):raise RuntimeError('unmodeled execution %08x'%pc)
  if pc in CALLEES:
   self.entries[CALLEES[pc]]=self.entries.get(CALLEES[pc],0)+1
   a,b=[u.reg_read(r) for r in R[:2]]
   self.events.append((CALLEES[pc],a,self.r(b) if pc==0x282650 else b if pc in (0x210850,0x28d1f0) else None))
 def run(self):
  summaries=[]
  for target,name in self.case['calls']:
   u=self.u;self.returned=False;self.fault=None
   for i,v in enumerate(self.saved):u.reg_write(R[i+4],v)
   for i in range(8,16):u.reg_write(UC_ARM_REG_D0+i,0x5eed000000000000+i)
   u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,END);u.reg_write(R[0],target);u.reg_write(R[1],name);u.reg_write(UC_ARM_REG_FPSCR,self.case.get('fpscr',0))
   try:u.emu_start(BASE,END+4,count=200000)
   except UcError:
    if not self.fault:raise
   if self.fault:break
   assert self.returned,'instruction limit';assert u.reg_read(UC_ARM_REG_SP)==SP,'sp'
   assert [u.reg_read(R[i+4]) for i in range(8)]==self.saved,'callee-saved integer registers'
   assert [u.reg_read(UC_ARM_REG_D0+i) for i in range(8,16)]==[0x5eed000000000000+i for i in range(8,16)],'callee-saved VFP registers'
   summaries.append(u.reg_read(UC_ARM_REG_FPSCR))
  mem=bytes(u.mem_read(DB,self.cursor-DB))+(bytes(u.mem_read(DB+0x20000,0x8ac)) if self.case.get('callback_registry') else b'');globals=bytes(u.mem_read(0x3e3180,8))+bytes(u.mem_read(0x3e3154,4))+bytes(u.mem_read(0x3e2654,8))
  return {'memory':mem,'globals':globals,'events':self.events,'fault':self.fault,'returned':self.returned,'fpscr':summaries}
def node(name,kind=3,payload=True):return dict(name=name,kind=kind,payload=payload)
def cases():
 c=[]
 targets=[0xde1,0x8513,0x6600]+list(range(0x6610,0x6630))
 for target in targets:
  kind=0 if target==0xde1 else 1 if target==0x8513 else 2 if target==0x6600 else 3
  for situation in ['new','existing','empty','unbind','same','collision_new','collision_existing','selected']:
   case={'label':f'{target:x}-{situation}','calls':[[target,1025]],'unit':2,'nodes':[]}
   if situation in ('existing','same','collision_existing'):case['nodes']=[node(1025,kind)]
   if situation=='empty':case['nodes']=[node(1025,kind,False)]
   if situation=='unbind':case['calls']=[[target,0]]
   if situation.startswith('collision'):case['nodes']=[node(1),*case['nodes'],node(1537)]
   if situation=='selected':case['nodes']=[node(19,2)];case['selected']=19
   if situation in ('unbind','same'):
    val=1025
    if target==0xde1:case['t2']=[0,0,val]
    elif target==0x8513:case['cube']=[0,0,val]
    elif target==0x6600:case['old_set']=val
    else:case['aux']=[val if i==target-0x6610 else 0 for i in range(32)]
   c.append(case)
 for target in [0,1,0xde0,0xde2,0x65ff,0x6601,0x660f,0x6630,0x8512,0x8514,0x7fffffff,0x80000000,0xffffffff]:c.append({'label':f'invalid-{target:x}','calls':[[target,1025]]})
 for unit,target in itertools.product(range(3),[0xde1,0x8513]):
  c.append({'label':f'unit-{unit}-{target:x}','unit':unit,'calls':[[target,511],[target,0],[target,512],[target,512],[target,0xffffffff],[target,0]],'nodes':[node(511,0 if target==0xde1 else 1,False)]})
 # Binding-set switching exercises all 38 cached names, with found, absent, and colliding names.
 for mode in range(8):
  rng=random.Random(mode)
  vals=[rng.choice([0,1,513,1025,1555,88]) for _ in range(38)]
  current=[rng.choice([0,1,513,1025,1555,88]) for _ in range(38)] if mode&1 else [0]*38
  data={'t2':vals[:3],'cube':vals[3:6],'aux':vals[6:]}
  old={'t2':current[:3],'cube':current[3:6],'aux':current[6:]}
  case={'label':f'set-switch-{mode}','calls':[[0x6600,23 if mode&2 else 0]],'nodes':[node(1),node(513),node(1025),node(23,2)],'set_values':{'23':data},'defaults':data,**old}
  if mode&4:case['pending']=71;case['nodes']+=[node(71,2)]
  c.append(case)
 for pendingkind,payload in itertools.product([2,3],[False,True]):
  c.append({'label':f'pending-{pendingkind}-{payload}','calls':[[0x6600,23]],'nodes':[node(23,2),node(71,pendingkind,payload)],'pending':71})
 for target in [0xde1,0x8513,0x6600,0x6610]:
  for fail in [1,2]:c.append({'label':f'failure-{target:x}-{fail}','calls':[[target,11]],'fail_alloc':fail})
  c.append({'label':f'null-callback-{target:x}','calls':[[target,11]],'null_allocator':True})
 return c
def main():
 start=time.monotonic();records=[];maxins=0;events=0;compared=0;coverage=set();entries={};faults=0;diagnostics=[]
 cs=cases()
 for index,case in enumerate(cs):
  engines=[Engine(case,x) for x in [False,True]];results=[e.run() for e in engines]
  a,b=results
  if a['fault'] or b['fault']:
   assert a['fault'] and b['fault'], ('one-sided fault',case,a['fault'],b['fault'])
   diagnostics.append({'case':case,'faults':[a['fault'],b['fault']],'same_fault_address':a['fault']==b['fault'],'memory_equal':a['memory']==b['memory'],'events_equal':a['events']==b['events'],'memory_differences':[(hex(DB+i),x,y) for i,(x,y) in enumerate(zip(a['memory'],b['memory'])) if x!=y][:32],'globals_equal':a['globals']==b['globals']})
  for key in ([] if a['fault'] else a):
   if a[key]!=b[key]:
    if key=='memory':print('first memory differences',[(hex(DB+i),x,y) for i,(x,y) in enumerate(zip(a[key],b[key])) if x!=y][:20])
    else:print('different',key,a[key],b[key])
    (OUT/'failed-case.json').write_text(json.dumps(case,indent=2));raise AssertionError((index,case['label'],key))
  if a['fault']:faults+=1
  else:compared+=len(a['memory'])+len(a['globals'])
  events+=len(a['events']);maxins=max(maxins,*[e.instructions for e in engines]);coverage.update(engines[0].coverage)
  for key,val in engines[0].entries.items():entries[key]=entries.get(key,0)+val
  records.append({'case':case,'fault':a['fault'],'returned':a['returned'],'instructions':[e.instructions for e in engines],'events':len(a['events']),'memory_sha256':hashlib.sha256(a['memory']).hexdigest()})
 result={'pairs':len(cs),'returning_pairs':len(cs)-faults,'fault_diagnostic_pairs':faults,'maximum_instructions':maxins,'events':events,'compared_return_bytes':compared,'retail_instruction_offsets':sorted(coverage),'original_callee_entries':entries,'candidate_bytes':len(CANDIDATE),'candidate_sha256':hashlib.sha256(CANDIDATE).hexdigest(),'seconds':time.monotonic()-start,'fault_diagnostics':diagnostics,'records':records}
 (OUT/RESULT_NAME).write_text(json.dumps(result,indent=2));print(json.dumps({k:v for k,v in result.items() if k not in ['records','retail_instruction_offsets','fault_diagnostics']},indent=2))
if __name__=='__main__':main()
```

## build/texture-binding-state/extended.py

```python
import replay as r
import itertools,random,json,hashlib,time

def cases():
 out=[]
 # Random finite lookup graphs and all 38 restored fields, including present/missing collisions.
 for seed in range(200):
  rng=random.Random(seed+8134);names=[1,513,1025,1537,7,519,1031,88,600,1112]
  chosen=[rng.choice(names+[0,0,55,1079]) for _ in range(38)]
  prior=[rng.choice(names+[0,55,1079]) for _ in range(38)]
  data={'t2':chosen[:3],'cube':chosen[3:6],'aux':chosen[6:]}
  old={'t2':prior[:3],'cube':prior[3:6],'aux':prior[6:]}
  out.append({'label':f'random-set-{seed}','calls':[[0x6600,23 if seed%2 else 0]],'nodes':[r.node(n) for n in names]+[r.node(23,2)],'defaults':data,'set_values':{'23':data},'dirty':rng.getrandbits(32),**old})
 # Name hashing wraps only to the low nine bits; unsigned names sort around 2^31.
 for target,name,position in itertools.product([0xde1,0x8513,0x6610,0x662f], [0x7fffffff,0x80000000,0xfffffffe,0xffffffff],['existing','new']):
  kind=0 if target==0xde1 else 1 if target==0x8513 else 3
  samebucket=(name&511) or 512
  nodes=[r.node(samebucket),r.node(samebucket+512),r.node(samebucket+1024)]
  if position=='existing':nodes.append(r.node(name,kind,False))
  out.append({'label':f'high-{target:x}-{name:x}-{position}','nodes':nodes,'unit':1,'calls':[[target,name],[target,0],[target,name]]})
 # Each target is unchanged while the allocator is absent; no fallback allocation is required.
 for target in [0xde1,0x8513,0x6600,*range(0x6610,0x6630)]:
  kind=0 if target==0xde1 else 1 if target==0x8513 else 2 if target==0x6600 else 3
  c={'label':f'no-allocator-{target:x}','calls':[[target,11]],'null_allocator':True,'nodes':[r.node(11,kind)]}
  if target==0xde1:c['t2']=[11,0,0]
  elif target==0x8513:c['cube']=[11,0,0]
  elif target==0x6600:c['old_set']=11
  else:c['aux']=[11 if i==target-0x6610 else 0 for i in range(32)]
  out.append(c)
 # No arithmetic in this root consumes FPSCR; real constructors use VFP moves only.
 for fpscr,target in itertools.product([0,0x1000000,0x2000000,0x3000000,0x400000,0x800000,0xc00000],[0xde1,0x8513,0x6600,0x6610]):out.append({'label':f'fpscr-{fpscr:x}-{target:x}','fpscr':fpscr,'calls':[[target,37]]})
 # Empty-chain insertion, last-node match and mismatch, and selected-set writes.
 for target in [0xde1,0x8513,0x6610,0x662f]:
  kind=0 if target==0xde1 else 1 if target==0x8513 else 3
  out.append({'label':f'chain-tail-{target:x}','nodes':[r.node(1),r.node(513),r.node(1025,kind)],'calls':[[target,1025],[target,2049]]})
 for target,mutation in itertools.product([0xde1,0x8513,0x6600,0x6610],['callback_unit','callback_clear','callback_registry']):
  c={'label':f'callback-{target:x}-{mutation}','calls':[[target,37]],mutation:1}
  out.append(c)
 return out
if __name__=='__main__':
 r.cases=cases;r.RESULT_NAME='extended-results.json';r.main()
```

## build/texture-binding-state/cycles.py

```python
import replay as r
import json
results=[]
for target in [0xde1,0x8513,0x6600,0x6610]:
 case={'label':f'cyclic-lookup-{target:x}','calls':[[target,1025]],'nodes':[r.node(1)]}
 for candidate in [False,True]:
  e=r.Engine(case,candidate);e.w(e.nodes[1]+12,e.nodes[1]);before=bytes(e.u.mem_read(r.DB,0x20000))
  try:e.run();raise AssertionError('cycle unexpectedly returned')
  except AssertionError as error:assert str(error)=='instruction limit',str(error)
  assert not e.returned and e.fault is None
  assert before==bytes(e.u.mem_read(r.DB,0x20000)) and not e.events
  results.append({'target':hex(target),'candidate':candidate,'instruction_limit':200000,'instructions':e.instructions,'returned':e.returned,'fault':e.fault,'data_unchanged':True})
(r.OUT/'cycle-diagnostics.json').write_text(json.dumps(results,indent=2));print(json.dumps(results,indent=2))
```

## build/texture-binding-state/preserve.py

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection
import json,hashlib,subprocess,collections
root=Path.cwd();base=root.parent/'mario-main861';out=root/'build/texture-binding-state'
def normalized(p):
 e=ELFFile(open(p,'rb'));sections=[];symbols=[]
 def symrec(s):
  n=s['st_shndx'];sec=e.get_section(n).name if isinstance(n,int) else n
  name=s.name.replace(str(root),'<repo>').replace(str(base),'<repo>') if s['st_info']['type']=='STT_FILE' else s.name
  return [name,s['st_info']['type'],s['st_info']['bind'],s['st_other']['visibility'],sec,s['st_value'],s['st_size']]
 for sec in e.iter_sections():
  if isinstance(sec,RelocationSection):
   tab=e.get_section(sec['sh_link']);sections.append([sec.name,[(r['r_offset'],r['r_info_type'],symrec(tab.get_symbol(r['r_info_sym']))) for r in sec.iter_relocations()]])
  elif sec['sh_type'] not in ('SHT_SYMTAB','SHT_STRTAB','SHT_NULL'):
   sections.append([sec.name,sec['sh_type'],sec['sh_flags'],sec['sh_addralign'],sec['sh_entsize'],hashlib.sha256(sec.data().replace(str(root).encode(),b'<repo>').replace(str(base).encode(),b'<repo>') if sec.name=='.comment' else sec.data()).hexdigest()])
 tab=e.get_section_by_name('.symtab')
 for s in tab.iter_symbols():
  symbols.append(symrec(s))
 return {'sections':sections,'symbols':symbols}
records=[];inputs={}
old_objects={p.relative_to(base/'build/eu/obj') for p in (base/'build/eu/obj').rglob('*.o')}
new_objects={p.relative_to(root/'build/eu/obj') for p in (root/'build/eu/obj').rglob('*.o')}
assert new_objects-old_objects=={Path('lib/CtrSDK/sources/gx_BindingState.o')}
assert not old_objects-new_objects
sealed={}
for name in subprocess.check_output(['git','ls-files'],cwd=base,text=True).splitlines():
 p=base/name;q=root/name
 if p.is_file():
  assert q.is_file(),name
  a=hashlib.sha256(p.read_bytes()).hexdigest();b=hashlib.sha256(q.read_bytes()).hexdigest();assert a==b,name;sealed[name]=a
(out/'baseline-tracked-inputs.json').write_text(json.dumps(sealed,indent=2,sort_keys=True))
for p in sorted((root/'build/eu/obj').rglob('*.o')):
 rel=p.relative_to(root/'build/eu/obj')
 if rel.parts[0] not in ('Game','lib') or p.stem=='gx_BindingState':continue
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
assert len(records)==179
# The generated scaffold gains only the six expected address aliases.
stub=Path('build/eu/obj/build/eu/split/stubs.o');a=normalized(root/stub);b=normalized(base/stub)
sa={x[0]:x for x in a['sections'] if x[0] not in ('.debug_frame','.rel.debug_frame')};sb={x[0]:x for x in b['sections'] if x[0] not in ('.debug_frame','.rel.debug_frame')}
expected_functions={'i.'+x for x in ['fn_00210850','fn_00282650','fn_00282D20','fn_0028B1E0','fn_0028B290','fn_0028D1F0']}
expected_data={'.sdata_'+x for x in ['dat_003E2654','dat_003E3154','dat_003E3180']}
expected=expected_functions|expected_data
assert set(sa)-set(sb)==expected
assert not set(sb)-set(sa)
for n in sb:assert sa[n]==sb[n],n
def debug_frames(path):
 elf=ELFFile(open(path,'rb'));frames={}
 for sec in elf.iter_sections():
  if isinstance(sec,RelocationSection) and sec.name=='.rel.debug_frame':
   tab=elf.get_section(sec['sh_link']);rels=[];owner=None
   for rel in sec.iter_relocations():
    sym=tab.get_symbol(rel['r_info_sym']);name=sym.name
    if name.startswith('i.'):owner=name
    if name.startswith('__ARM_grp_.debug_frame$'):name='<frame-CIE>'
    ndx=sym['st_shndx'];target=elf.get_section(ndx).name if isinstance(ndx,int) else ndx
    rels.append([rel['r_offset'],rel['r_info_type'],name,target,sym['st_value'],sym['st_size']])
   assert owner and owner not in frames
   frame=elf.get_section(sec['sh_info']);frames[owner]=[hashlib.sha256(frame.data()).hexdigest(),rels]
 return frames
fa=debug_frames(root/stub);fb=debug_frames(base/stub)
assert set(fa)-set(fb)==expected_functions
assert not set(fb)-set(fa)
for n in fb:assert fa[n]==fb[n],n+' debug frame'
ca=collections.Counter(json.dumps(x) for x in a['symbols']);cb=collections.Counter(json.dumps(x) for x in b['symbols']);assert not cb-ca
added=[json.loads(x) for x in (ca-cb).elements()]
debug_symbols={f'__ARM_grp_.debug_frame${x}' for x in [5807,5810,5813,5816,5819,5822]}
assert all(x[4] in expected or x[0] in debug_symbols for x in added),added
assert {x[0] for x in added if x[4] not in expected}==debug_symbols
assert {x[0] for x in added if x[1]=='STT_FUNC'}=={x[2:] for x in expected_functions}
oldsource=(base/'build/eu/split/stubs.c').read_text();newsource=(root/'build/eu/split/stubs.c').read_text()
for n in sorted(expected_functions):
 sym=n[2:];address=sym.split('_')[1];kind='STUB' if sym=='fn_0028D1F0' else 'STUB_G'
 addition=f'/* Scaffold alias for unnamed function at 0x{address}. */\n{kind}({sym});\n';assert newsource.count(addition)==1;newsource=newsource.replace(addition,'')
for symbol,size in [('dat_003E2654',4),('dat_003E3154',4),('dat_003E3180',8)]:
 address=symbol.split('_')[1];addition=f'/* Zero-filled scaffold data at 0x{address}, not reconstructed data. */\n__weak __attribute__((section(".sdata_{symbol}"), aligned(4))) unsigned char {symbol}[{size}] = {{0}};\n';assert newsource.count(addition)==1;newsource=newsource.replace(addition,'')
assert oldsource==newsource
psa=json.loads((root/stub).with_suffix('.provenance.json').read_text());psb=json.loads((base/stub).with_suffix('.provenance.json').read_text())
assert psa['compiler_sha256']==psb['compiler_sha256']
assert [x.replace(str(root),'<repo>') for x in psa['command']]==[x.replace(str(base),'<repo>') for x in psb['command']]
assert set(psa['inputs'])==set(psb['inputs'])
for key in psa['inputs']:
 assert hashlib.sha256((root/key).read_bytes()).hexdigest()==psa['inputs'][key]
 assert hashlib.sha256((base/key).read_bytes()).hexdigest()==psb['inputs'][key]
 if key!='build/eu/split/stubs.c':assert psa['inputs'][key]==psb['inputs'][key]
stub_delta={'all_prior_sections_symbols_unchanged':True,'old_debug_frame_count':len(fb),'new_debug_frame_count':len(fa),'debug_normalization':'Compare every CFI byte and resolved relocation per function; compiler-generated frame-CIE ordinal names and section-table indices normalize only to that per-function association','added_sections':sorted(expected),'added_symbols':added,'old_generated_source_sha256':psb['inputs']['build/eu/split/stubs.c'],'new_generated_source_sha256':psa['inputs']['build/eu/split/stubs.c']}
(out/'generated-stub-delta.json').write_text(json.dumps(stub_delta,indent=2))
assert (root/'build/eu/code.bin').read_bytes()==(base/'build/eu/code.bin').read_bytes()
result={'base':'86104d96a7f570383bbfefc5fffad998e134496c','object_count':len(records),'input_count':len(inputs),'all_baseline_tracked_files':len(sealed),'total_objects_with_addition':len(new_objects),'prior_canonical_cpp_objects':sum(1 for x in old_objects if x.parts[0] in ('Game','lib')),'all_equal':True,'generated_stub_delta_verified':True,'compact_image_equal':True,'excluded_difference':'Only repository prefix in STT_FILE absolute source path and in non-allocated .comment, and resulting raw symbol/string table indices; all other symbols, section data, attributes and resolved relocations compare equal','objects':records,'inputs':inputs}
(out/'preservation.json').write_text(json.dumps(result,indent=2));print(json.dumps({k:v for k,v in result.items() if k not in ('objects','inputs')},indent=2))
```

## build/texture-binding-state/audit.py

```python
from pathlib import Path
from capstone import *
from elftools.elf.elffile import ELFFile
import csv,struct,json,hashlib,subprocess
root=Path.cwd();out=root/'build/texture-binding-state';raw=(root/'data/ver/eu/code.bin').read_bytes();md=Cs(CS_ARCH_ARM,CS_MODE_ARM);md.detail=True
assert hashlib.sha256(raw).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
start,end=0x282d20,0x283594;data=[(0x282df0,0x282e14),(0x282e30,0x282e54),(0x283578,0x283594)]
ins=[]
for a,b in [(start,data[0][0]),(data[0][1],data[1][0]),(data[1][1],data[2][0])]:ins+=list(md.disasm(raw[a-0x100000:b-0x100000],a))
assert len(ins)==516
rows=[{k.strip():v.strip() for k,v in r.items()} for r in csv.DictReader((root/'data/ver/eu/map.csv').open())]
callers=[]
for off in range(0,len(raw)-4,4):
 word=struct.unpack_from('<I',raw,off)[0]
 if word&0x0f000000==0x0b000000:
  delta=word&0xffffff;delta=delta-(1<<24) if delta&0x800000 else delta
  if off+0x100008+4*delta==start:
   address=off+0x100000;row=next(r for r in rows if int(r['Start'],16)<=address<int(r['End'],16));callers.append({'callsite':hex(address),'row':row})
literals=[(hex(a),hex(struct.unpack_from('<I',raw,a-0x100000)[0])) for a in range(0x283578,end,4)]
obj=root/'build/eu/obj/lib/CtrSDK/sources/gx_BindingState.o'
with obj.open('rb') as f:
 elf=ELFFile(f);sections=[(s.name,s['sh_size']) for s in elf.iter_sections() if s.name.startswith('i.')];functions=[(s.name,s['st_size']) for s in elf.get_section_by_name('.symtab').iter_symbols() if s['st_info']['type']=='STT_FUNC' and s['st_size']]
manifest={}
for p in [root/'lib/CtrSDK/include/retail/GraphicsBindingState.h',root/'lib/CtrSDK/sources/gx_BindingState.cpp',root/'data/ver/eu/map.csv',root/'data/config.json',root/'data/ver/eu/config.json',root/'tools/check.py',root/'tools/pypstem/stepBuild.py',obj,obj.with_suffix('.provenance.json'),root/'build/eu/code.bin']:
 if p.exists():manifest[str(p.relative_to(root))]=hashlib.sha256(p.read_bytes()).hexdigest()
result={'base':'86104d96a7f570383bbfefc5fffad998e134496c','source_commit':subprocess.check_output(['git','log','-1','--format=%H','--','lib/CtrSDK/sources/gx_BindingState.cpp'],text=True).strip(),'root':hex(start),'end':hex(end),'bytes':end-start,'root_sha256':hashlib.sha256(raw[start-0x100000:end-0x100000]).hexdigest(),'instructions':len(ins),'data_intervals':data,'direct_calls':[(i.address,i.op_str) for i in ins if i.mnemonic=='bl'],'indirect_calls':[(i.address,i.op_str) for i in ins if i.mnemonic=='blx'],'callers':callers,'literals':literals,'object_sections':sections,'nonempty_function_definitions':functions,'hashes':manifest}
(out/'audit.json').write_text(json.dumps(result,indent=2));print(json.dumps({k:v for k,v in result.items() if k not in ['callers','direct_calls','indirect_calls','hashes','literals']},indent=2))
```

## build/texture-binding-state/declarations.py

```python
from pathlib import Path
import subprocess,hashlib,json
root=Path.cwd();out=root/'build/texture-binding-state'
needle=r'dat_003E3154|dat_003E3180|dat_003E2654|fn_00210850|fn_0028D1F0|fn_00282D20|fn_00282650|fn_0028B1E0|fn_0028B290'
roots=[p for p in root.parent.glob('mario-*') if p.is_dir() and (p/'.git').exists()]
seen={};records=[];heads={}
for checkout in roots:
 heads[str(checkout)]=subprocess.check_output(['git','rev-parse','HEAD'],cwd=checkout,text=True).strip()
 dirs=[str(checkout/x) for x in ['Game','lib'] if (checkout/x).is_dir()]
 if not dirs:continue
 proc=subprocess.run(['rg','-i','-n','--glob','*.h','--glob','*.cpp',needle,*dirs],capture_output=True,text=True);assert proc.returncode in (0,1)
 for line in proc.stdout.splitlines():
  path,lineno,content=line.split(':',2);file=Path(path);digest=hashlib.sha256(file.read_bytes()).hexdigest();key=(digest,content.strip())
  if key in seen:seen[key]['also_seen'].append(str(checkout));continue
  record={'path':path,'line':lineno,'text':content.strip(),'file_sha256':digest,'also_seen':[]};seen[key]=record;records.append(record)
(out/'declaration-audit.json').write_text(json.dumps({'worktrees':len(roots),'heads':heads,'matches':records},indent=2));print('Worktrees:',len(roots),'Distinct matching lines:',len(records))
```

## build/texture-binding-state/signature-compatibility.cpp

```cpp
#include <retail/GraphicsGlobals.h>
#include <retail/ShaderBinary.h>
#include <retail/ProgramLink.h>
#include <shv_ValidatorAccess.h>
#include <retail/GraphicsBindingState.h>
extern "C" {
extern retail_graphics::RenderControl* dat_003E3154;
extern retail_graphics_binding::Allocate dat_003E2654;
void fn_0028D1F0(void*, unsigned);
void fn_00282D20(unsigned, unsigned);
void fn_00282650(int, unsigned*);
}
```

## Verification script hashes

- `check.py`: `e0dc3582618fe4fbf1a408a5659b2b41118a6d5d0ed2f35e35810f9e99102978`
- `link.py`: `f5f17a1e671e5bec5849ebe0f517ecb79fec80b1edd5fe0aadd7f7f2fc6a6a21`
- `replay.py`: `70df8a3358a896bd639e3a2502d94161b2aab584297eec2cbac990debca29e6e`
- `extended.py`: `954f24308b779235e260a83b540b89250be3e23ba227104833fa5221e7466b46`
- `cycles.py`: `78654124acae2636e1627155ac5da12ccec50bc3c5c1d189c751d693e6bab3c3`
- `preserve.py`: `f2127ca942cce70086707294c4e73b9aa226439855c47848b9ba023947fd8253`
- `audit.py`: `a7a1cca3bd3a62b0ab6066dd7570f152c1e7e31cb6479aee82a95da054ea685c`
- `declarations.py`: `bfa646eb86f2572186447a40e867bd184fdbb881d0d1fe4e28ccdcbffaca4fc8`
- `signature-compatibility.cpp`: `ba447b9c29d958ccc73d682b4940ec5ba57bdd674251bb37f7f7231a0d9950fd`
