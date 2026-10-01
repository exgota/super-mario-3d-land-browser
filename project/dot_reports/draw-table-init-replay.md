# Draw table ARM1176 replay

Extract and run from the repository root after canonical check.py creates candidate.axf. Requires pyelftools and Unicorn2.1.4. It reads only the authorized local EU binary and untouched linked candidate. See draw-table-init.md for the exact scope and limits.

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE
from unicorn.arm_const import *
import struct,random,hashlib,json
ROOT=Path.cwd();BASE=0x24b168;END=0x71000000;DB=0x1000000;DS=0x40000;TABLE=DB;ORDERS=DB+0x1000;HEAP=DB+0x20000;SP=0x70008000
binary=(ROOT/'data/ver/eu/code.bin').read_bytes();assert hashlib.sha256(binary).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
image=max((ROOT/'build/exact_checks/eu').glob('function_0024B168_*/candidate.axf'),key=lambda p:p.stat().st_mtime)
with image.open('rb') as f:e=ELFFile(f);candidate=next(s.data() for s in e.iter_sections() if s['sh_addr']==BASE and s['sh_size'])
assert len(candidate)==1028
regs=[UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3,UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11,UC_ARM_REG_R12]
types=['ActorModelDraw','ActorModelDrawModelCache','LayoutDraw','LayoutDrawBottom','Draw','Functor','Unknown','draw','']
ctors={0x1e7e00:0,0x1e8aa8:1,0x1e6078:2,0x1e5fcc:3,0x1e5f24:4,0x1dc890:5}
ranges=[(BASE,BASE+1028),(0x1e7e00,0x1e7e18),(0x1e8aa8,0x1e8ac0),(0x1e7518,0x1e75bc),(0x1e6078,0x1e6124),(0x1e5fcc,0x1e6078),(0x1e5f24,0x1e5fcc),(0x1dc890,0x1dc8b0),(0x243b94,0x243ba8),(0x242cc4,0x242d14),(0x292308,0x292354)]
def get(data,a):return struct.unpack_from('<I',data,a-DB)[0]
def put(data,a,v):struct.pack_into('<I',data,a-DB,v&0xffffffff)
class Engine:
 def __init__(self,is_candidate):
  self.u=u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176);u.mem_map(0x100000,0x400000);u.mem_write(0x100000,binary)
  if is_candidate:u.mem_write(BASE,candidate)
  u.mem_map(DB,DS);u.mem_map(0x70000000,0x10000);u.mem_map(END,0x1000);u.reg_write(UC_ARM_REG_C1_C0_2,0xf<<20);u.reg_write(UC_ARM_REG_FPEXC,1<<30);u.hook_add(UC_HOOK_CODE,self.hook)
 def cstr(self,a):
  assert a;out=bytearray()
  while len(out)<128:
   c=bytes(self.u.mem_read(a+len(out),1))[0]
   if not c:return bytes(out)
   out.append(c)
  raise AssertionError('string length')
 def hook(self,u,pc,size,_):
  self.steps+=1
  if pc in (0x292a78,0x2932b0):
   size=u.reg_read(UC_ARM_REG_R0);assert size<0x10000;ptr=self.cursor;self.cursor+=max(8,(size+7)&~7);assert self.cursor<DB+DS
   self.trace.append(('allocate','array' if pc==0x292a78 else 'object',size));self.allocations.append((ptr,size))
   for i in [1,2,3,12]:u.reg_write(regs[i],0xcc000000+i)
   u.reg_write(UC_ARM_REG_R0,ptr);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR));return
  if pc in ctors:
   kind=ctors[pc];ptr=u.reg_read(UC_ARM_REG_R0);name=u.reg_read(UC_ARM_REG_R1);capacity=u.reg_read(UC_ARM_REG_R2) if kind!=5 else None
   self.trace.append(('construct',kind,ptr,name,capacity));self.created[ptr]=(kind,name,capacity)
  if pc==0x292308:self.trace.append(('equal',self.cstr(u.reg_read(UC_ARM_REG_R0)).hex(),self.cstr(u.reg_read(UC_ARM_REG_R1)).hex()))
  assert any(a<=pc<z for a,z in ranges),hex(pc)
 def run(self,data,amount):
  u=self.u;u.mem_write(DB,bytes(data));u.mem_write(0x70000000,bytes(0x10000));u.reg_write(UC_ARM_REG_CPSR,0x10)
  for i,r in enumerate(regs):u.reg_write(r,0xaa000000+i)
  for i,v in enumerate([TABLE,DB+0x800,ORDERS,amount]):u.reg_write(regs[i],v)
  u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,END);self.trace=[];self.created={};self.allocations=[];self.steps=0;self.cursor=HEAP
  u.emu_start(BASE,END,count=50000);assert u.reg_read(UC_ARM_REG_PC)==END
  assert u.reg_read(UC_ARM_REG_SP)==SP and all(u.reg_read(regs[i])==0xaa000000+i for i in range(4,12))
  return bytes(u.mem_read(DB,DS)),self.trace,self.created,self.allocations,self.steps
rng=random.Random(BASE);engines=[Engine(False),Engine(True)];maxsteps=0;total_created=0
for case in range(512):
 n=case if case<10 else rng.randrange(13);data=bytearray(DS);data[HEAP-DB:]=b'\xcd'*(DS-(HEAP-DB));data[0x800:0x806]=b'table\0';kindids=[];cap=[]
 modelinfo=rng.choice([0,0x30000010]);layoutinfo=rng.choice([0,0x30000020]);put(data,TABLE+0x48,modelinfo);put(data,TABLE+0x4c,layoutinfo)
 for i in range(n):
  typ=i%len(types) if case<10 else rng.randrange(len(types));kindids.append(typ);capacity=rng.randrange(9);cap.append(capacity);name=DB+0x4000+i*0x80;typeptr=name+0x30
  nm=f'entry{i}'.encode()+b'\0';ty=types[typ].encode()+b'\0';data[name-DB:name-DB+len(nm)]=nm;data[typeptr-DB:typeptr-DB+len(ty)]=ty
  for off,v in [(0,name),(4,typeptr),(8,capacity),(12,rng.getrandbits(32))]:put(data,ORDERS+16*i+off,v)
 runs=[e.run(data,n) for e in engines];assert runs[0][:4]==runs[1][:4],('pair mismatch',case)
 state,trace,created,allocations,_=runs[0];counts=[sum(t in group for t in kindids) for group in [(0,1),(2,3),(4,),(5,)]]
 assert get(state,TABLE)==DB+0x800 and get(state,TABLE+0xc)==n and get(state,TABLE+0x10)==n
 assert [get(state,TABLE+off) for off in [0x18,0x24,0x30,0x3c]]==counts
 assert [get(state,TABLE+off) for off in [0x1c,0x28,0x34,0x40]]==counts
 master=get(state,TABLE+0x14);expected_groups=[[],[],[],[]];expected_alloc=[('array',n*4)]+[('array',c*4) for c in counts]
 for i,t in enumerate(kindids):
  entry=get(state,master+4*i)
  if t>=6:assert entry==0;continue
  assert created[entry]==(t,DB+0x4000+i*0x80,None if t==5 else cap[i]);assert get(state,entry+4)==DB+0x4000+i*0x80
  g=0 if t<2 else 1 if t<4 else t-2;expected_groups[g].append(entry)
  expected_alloc.append(('object',[28,28,24,24,20,12][t]))
  if t!=5:
   expected_alloc.append(('array',cap[i]*4));assert get(state,entry+8)==cap[i] and get(state,entry+12)==0
   inner=get(state,entry+16);assert all(get(state,inner+4*j)==0 for j in range(cap[i]))
  if t<2:assert get(state,entry+24)==modelinfo
  if 2<=t<4:assert get(state,entry+20)==layoutinfo
  if t==5:assert get(state,entry+8)==0
 for off,expected in zip([0x20,0x2c,0x38,0x44],expected_groups):
  array=get(state,TABLE+off);assert [get(state,array+4*i) for i in range(len(expected))]==expected
 actual_alloc=[(e[1],e[2]) for e in trace if e[0]=='allocate'];assert actual_alloc==expected_alloc
 maxsteps=max(maxsteps,*[r[4] for r in runs]);total_created+=len(created)
summary={'pairs_passed':512,'max_instructions':maxsteps,'objects_created':total_created,'candidate_size':len(candidate),'candidate_sha256':hashlib.sha256(candidate).hexdigest(),'original_sha256':hashlib.sha256(binary[BASE-0x100000:BASE-0x100000+1028]).hexdigest(),'engine':'Unicorn2.1.4 ARM1176','scope':'0..12 valid orders, all six recognized kinds plus unknown/case-mismatched/empty kinds, capacities0..8, optional draw-info pointers. Actual retail constructor/type-count/string helpers execute; only ordinary allocation is a deterministic always-successful model. Full data/heap memory and call traces compare; independent category/capacity/append/initialization model also checks outputs. Excludes allocation failure, malformed/reentrant/aliased inputs and hardware behavior. No exact credit.'}
print(json.dumps(summary,indent=2));(ROOT/'build/draw-table-replay-results.json').write_text(json.dumps(summary,indent=2))
```
