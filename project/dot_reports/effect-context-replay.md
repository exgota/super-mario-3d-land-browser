# Reproduce bounded effect context replay

Requires the local authorized EU code.bin, canonical committed-source object, approved ARMCC791+wibo, pyelftools and Unicorn2.1.4. Run both extracted scripts from the repository root. The first performs a diagnostic original-address link, not exact acceptance. The second compares512 bounded states with the original and independent model. Scope is in effect-context-creation.md. Neither script contains game instruction bytes or modifies a checker, input object, map or original.

## Diagnostic link
```python
from pathlib import Path
import json,subprocess,hashlib,os
from elftools.elf.elffile import ELFFile
r=Path.cwd();o=r/'build/eu/obj/lib/al/src/Effect/alEffectCreationFunction.o';out=r/'build/dot_effect_context_replay';out.mkdir(exist_ok=True)
imports={'_ZN4sead15Matrix34CalcCtrIfE4copyERN2nn4math5MTX34ERKS4_':(0x27c18c,'A'),'_ZN4sead6Random6getU32Ev':(0x24b12c,'A'),'fn_002200FC':(0x2200fc,'A'),'dat_003E26DC':(0x3e26dc,'D'),'_ZN4sead7Vector3IfE4onesE':(0x4305ec,'D')}
with o.open('rb') as f:
 e=ELFFile(f);s=e.get_section_by_name('i.fn_002E54AC');size=s['sh_size'];symbols=e.get_section_by_name('.symtab');undef={x.name for x in symbols.iter_symbols() if x.name and x['st_shndx']=='SHN_UNDEF'};assert undef-{n for n in undef if n.startswith('Lib$$Request$$')}<=imports.keys(),undef
(out/'original_symbols.sym').write_text('#<SYMDEFS>#\n'+''.join(f'0x{a:08X} {k} {n}\n' for n,(a,k) in imports.items()))
(out/'candidate.sct').write_text('CANDIDATE_LOAD 0x002E54AC\n{\n CANDIDATE_CODE 0x002E54AC\n {\n alEffectCreationFunction.o (i.fn_002E54AC, +FIRST)\n }\n}\n')
cmd=[str(r/'data/compilers/wibo'),str(r/'data/compilers/4.1/791/bin/armlink.exe'),'--cpu=MPCore','--fpu=VFPv2','--arm_only','--no_exceptions','--inline','--datacompressor=off','--no_debug','--no_scanlib','--mangled','--symbols','--map','--entry=fn_002E54AC','--keep=fn_002E54AC',f'--scatter={out}/candidate.sct',f'--output={out}/candidate.axf',f'--list={out}/candidate.map',str(o),str(out/'original_symbols.sym')]
p=subprocess.run(cmd,env=dict(os.environ,TMP='/tmp'),capture_output=True,text=True);(out/'link.log').write_text(p.stdout+p.stderr);assert p.returncode==0,p.stdout+p.stderr
with (out/'candidate.axf').open('rb') as f:
 e=ELFFile(f);ss=[s for s in e.iter_sections() if s['sh_flags']&2 and s['sh_size']];assert len(ss)==1 and ss[0]['sh_addr']==0x2e54ac and ss[0]['sh_size']==size;linked=ss[0].data()
evidence={'object_sha256':hashlib.sha256(o.read_bytes()).hexdigest(),'size':size,'linked_sha256':hashlib.sha256(linked).hexdigest(),'command':cmd,'imports':imports,'diagnostic_only':'Genuine unchanged project-built full ARMCC object linked at original address without size assertion. No object/target/checker/map changes. Canonical checker remains M on size. Not acceptance evidence.'};(out/'link-evidence.json').write_text(json.dumps(evidence,indent=2));print(json.dumps(evidence,indent=2))
```

## ARM1176 replay
```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE
from unicorn.arm_const import *
import struct,random,json,hashlib
ROOT=Path.cwd();BINARY=(ROOT/'data/ver/eu/code.bin').read_bytes();BASE=0x2e54ac;END=0x71000000;DB=0x1000000;DS=0x10000;STACK=0x70000000;SP=STACK+0x8000
assert hashlib.sha256(BINARY).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
with (ROOT/'build/dot_effect_context_replay/candidate.axf').open('rb') as f:
 e=ELFFile(f);CANDIDATE=next(s.data() for s in e.iter_sections() if s['sh_addr']==BASE and s['sh_size'])
REGS=[UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3,UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11,UC_ARM_REG_R12]
CTX=DB;SETS=DB+0x1000;EMITTERS=DB+0x3000;BANKS=DB+0x4000;REF=DB+0x9000;MATRIX=DB+0xa000;RNG=DB+0xb000
class Engine:
 def __init__(self,candidate):
  u=self.u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176)
  u.mem_map(0x100000,0x400000);u.mem_write(0x100000,BINARY)
  if candidate:u.mem_write(BASE,CANDIDATE)
  self.extent=len(CANDIDATE) if candidate else 1240
  u.mem_map(DB,DS);u.mem_map(STACK,0x10000);u.mem_map(END,0x1000)
  u.reg_write(UC_ARM_REG_C1_C0_2,0xf<<20);u.reg_write(UC_ARM_REG_FPEXC,1<<30)
  u.mem_write(0x4305ec,struct.pack('<III',*[0x3f800000]*3));u.mem_write(0x3e26dc,struct.pack('<I',RNG))
  u.hook_add(UC_HOOK_CODE,self.hook)
 def hook(self,u,pc,size,_):
  self.steps+=1
  if pc==0x2200fc:
   assert u.reg_read(UC_ARM_REG_R0)==CTX;ptr=u.reg_read(UC_ARM_REG_R1);msg=bytearray()
   while True:
    x=bytes(u.mem_read(ptr+len(msg),1))[0]
    if not x:break
    msg.append(x);assert len(msg)<1000
   self.events.append(('report',msg.hex()))
   for i in [0,1,2,3,12]:u.reg_write(REGS[i],0xcc000000+i)
   u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR));return
  if pc==0x27c18c:self.events.append(('matrix',u.reg_read(UC_ARM_REG_R0),u.reg_read(UC_ARM_REG_R1)))
  if pc==0x24b12c:self.events.append(('random',))
  assert BASE<=pc<BASE+self.extent or 0x27c18c<=pc<0x27c198 or 0x24b12c<=pc<0x24b168,hex(pc)
 def run(self,data,resource,bank,group,mask):
  u=self.u;u.mem_write(DB,bytes(data));u.mem_write(STACK,bytes(0x10000));u.reg_write(UC_ARM_REG_CPSR,0x10)
  for i,r in enumerate(REGS):u.reg_write(r,0xaa000000+i)
  for i,v in enumerate([CTX,REF,MATRIX,resource]):u.reg_write(REGS[i],v)
  u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,END);u.reg_write(UC_ARM_REG_D8,0x123456789abcdef0)
  u.mem_write(SP,struct.pack('<III',bank,group,mask));self.steps=0;self.events=[]
  u.emu_start(BASE,END,count=20000);assert u.reg_read(UC_ARM_REG_PC)==END,('did not return',self.steps)
  assert u.reg_read(UC_ARM_REG_SP)==SP and u.reg_read(UC_ARM_REG_D8)==0x123456789abcdef0
  assert [u.reg_read(REGS[i]) for i in range(4,12)]==[0xaa000000+i for i in range(4,12)]
  return bytes(u.mem_read(DB,DS)),u.reg_read(UC_ARM_REG_R0),self.events,self.steps

def get(data,a):return struct.unpack_from('<I',data,a-DB)[0]
def put(data,a,x):struct.pack_into('<I',data,a-DB,x&0xffffffff)
def signed(x):return x if x<0x80000000 else x-0x100000000
MSG_EMITTER=bytes.fromhex('8347837e8362835e82aa8ccd8a8982b582dc82b582bd0a').hex();MSG_SET=bytes.fromhex('8347837e8362835e835a8362836782aa8ccd8a8982b582dc82b582bd0a').hex()
def model(initial,resource,bank,group,mask):
 d=bytearray(initial);events=[]
 def g(a):return get(d,a)
 def p(a,x):put(d,a,x)
 def pb(a,x):d[a-DB]=x
 def rng():
  events.append(('random',));x,y,z,w=[g(RNG+4*i) for i in range(4)];t=(x^(x<<11))&0xffffffff;n=(t^(t>>8)^w^(w>>19))&0xffffffff
  for i,v in enumerate([y,z,w,n]):p(RNG+4*i,v)
  return n
 record=g(g(BANKS+4*bank)+0x18)+40*resource;count=signed(g(record+8))
 if signed(g(CTX+0x868))<count:return bytes(d),0,[('report',MSG_EMITTER)]
 probes=0;chosen=None
 while True:
  cursor=(g(CTX+0x834)+1)&g(CTX+0x18);p(CTX+0x834,cursor);slot=SETS+cursor*0x1d0
  if not g(slot+4):chosen=slot;break
  probes+=1
  if probes>=signed(g(CTX+0x14)):break
 p(REF,chosen or 0)
 if chosen is None:return bytes(d),0,[('report',MSG_SET)]
 s=chosen
 for off in [0xfc,0x12c]:
  events.append(('matrix',s+off,MATRIX));d[s+off-DB:s+off-DB+48]=d[MATRIX-DB:MATRIX-DB+48]
 for off in [0x15c,0x160,0x164,0x180,0x184,0x188,0x170,0x174,0x178,0x17c,0x16c,0x168,0x198,0x194,0x190,0x18c,0x1a4,0x1a0,0x19c]:p(s+off,0x3f800000)
 for off in [0x1c0,0x1b0,0x1ac,0x1a8,0x1c4]:p(s+off,0)
 for off in [0x1cc,0x1cd,0x1ce,0x1cf]:pb(s+off,0)
 p(s+0xc,g(CTX+0x8ac));p(s+0xf0,bank);p(s+0xf4,resource);p(REF+4,g(CTX+0x8ac))
 for index in range(count-1,-1,-1):
  if not(mask&(1<<index)):continue
  emitter=None
  while True:
   cursor=(g(CTX+0x82c)+1)&g(CTX+0x848);p(CTX+0x82c,cursor);slot=EMITTERS+cursor*0xb4
   if not g(slot+0xa4):emitter=slot;break
   probes+=1
   if probes>=signed(g(CTX+0x83c)):break
  if emitter is None:events.append(('report',MSG_EMITTER));break
  n=g(s+4);p(s+4,n+1);p(s+0x10+4*n,emitter);par=s+0x30+24*index;p(emitter+0x10,par);p(emitter+0xc,s)
  for i,v in enumerate([0x100,0x100,0x100,0,0,0]):p(par+4*i,v)
  p(emitter+0x14,g(CTX+0x8ac));p(emitter+0x84,0x3f800000);head=g(CTX+0x1c+4*group)
  if head:p(head+0x9c,emitter)
  p(emitter+0xa0,head);p(CTX+0x1c+4*group,emitter);p(emitter+0x9c,0);p(CTX+0x868,g(CTX+0x868)-1)
  res=g(g(record+4)+index*8+4);p(emitter+0xa8,res)
  for off,v in [(0,0),(0x88,0xffffffff),(0x8c,0),(0x90,0),(0x94,0)]:p(emitter+off,v)
  seed=g(res+8) or rng();p(emitter+0x78,seed);p(emitter+0x7c,seed)
  if g(res+0x84)==0x7fffffff:pb(s+0x1cf,1)
  p(emitter+0xac,0);p(emitter+0xb0,0);p(emitter+8,group);p(emitter+0xa4,g(CTX+0x8c0+g(res)*4))
  scale=(seed*signed(g(res+0x7c)))>>32;p(emitter+0x7c,seed*0x41c64e6d+0x3039);p(emitter+4,0x7fffffff);p(emitter+0x80,g(res+0x78)-scale)
 p(s+8,g(s+4));p(CTX+0x864,g(CTX+0x864)+1);p(CTX+0x8ac,g(CTX+0x8ac)+1)
 return bytes(d),1,events

def case(rng,i):
 data=bytearray(DS)
 def p(a,x):put(data,a,x)
 occupied_sets=[rng.randrange(2) for _ in range(8)];occupied_emitters=[rng.randrange(2) for _ in range(8)]
 if i%7==0:occupied_sets=[1]*8
 if i%7==1:occupied_emitters=[0]*8
 count=rng.randrange(9);resource=rng.randrange(2);bank=rng.randrange(2);group=rng.choice([0,1,127,128,254,255]);mask=rng.choice([0,255,rng.randrange(256)])
 for off,v in [(4,BANKS),(0x10,SETS),(0x14,8),(0x18,7),(0x41c,EMITTERS),(0x82c,rng.randrange(8)),(0x834,rng.randrange(8)),(0x83c,8),(0x848,7),(0x864,rng.randrange(9)),(0x868,8-sum(occupied_emitters)),(0x8ac,rng.getrandbits(32))]:p(CTX+off,v)
 for k in range(8):
  p(SETS+k*0x1d0,CTX);p(SETS+k*0x1d0+4,occupied_sets[k]);p(EMITTERS+k*0xb4+0xa4,0x22000000 if occupied_emitters[k] else 0)
 for k in range(3):p(CTX+0x8c0+4*k,0x23000000+k*0x100)
 if any(occupied_emitters):p(CTX+0x1c+4*group,EMITTERS+occupied_emitters.index(1)*0xb4)
 for bankidx in range(2):
  bh=DB+0x4100+bankidx*0x100;records=DB+0x5000+bankidx*0x100;p(BANKS+4*bankidx,bh);p(bh+0x18,records)
  for recordidx in range(2):
   members=DB+0x6000+(bankidx*2+recordidx)*0x100;p(records+recordidx*40+4,members);p(records+recordidx*40+8,count)
   for k in range(8):p(members+8*k+4,DB+0x7000+k*0x100)
 for k in range(8):
  res=DB+0x7000+k*0x100
  for off,v in [(0,rng.randrange(3)),(8,rng.choice([0,rng.getrandbits(32)])),(0x78,rng.randrange(-1000,1001)),(0x7c,rng.randrange(-1000,1001)),(0x84,rng.choice([0,0x7fffffff]))]:p(res+off,v)
 for k in range(12):p(MATRIX+4*k,rng.getrandbits(32))
 for k in range(4):p(RNG+4*k,rng.getrandbits(32))
 p(REF,0xdead0000);p(REF+4,0xabcdef00)
 return data,resource,bank,group,mask
engines=[Engine(False),Engine(True)];r=random.Random(BASE);maximum=0;counts={0:0,1:0};reports=0;random_calls=0
for i in range(512):
 args=case(r,i);expected=model(*args);runs=[engine.run(*args) for engine in engines]
 for j,run in enumerate(runs):
  if run[:3]!=expected:
   diffs=[hex(DB+k) for k,(a,b) in enumerate(zip(run[0],expected[0])) if a!=b];raise AssertionError((i,j,diffs[:20],run[1:3],expected[1:3]))
 counts[runs[0][1]]+=1;maximum=max(maximum,*[x[3] for x in runs]);reports+=sum(x[0]=='report' for x in expected[2]);random_calls+=sum(x[0]=='random' for x in expected[2])
summary={'pairs_passed':512,'engines':'Unicorn2.1.4 ARM1176, VFP enabled','max_instructions':maximum,'returns':counts,'report_calls':reports,'random_calls':random_calls,'candidate_size':len(CANDIDATE),'candidate_sha256':hashlib.sha256(CANDIDATE).hexdigest(),'retail_sha256':hashlib.sha256(BINARY[BASE-0x100000:BASE-0x100000+1240]).hexdigest(),'scope':'Bounded valid 8-slot pools, 0..8 resources, two banks/records, masks and group endpoints, occupied/free slots, explicit/random seeds and signed ranges. Actual retail matrix-copy/RNG execute; report callback mocked non-mutating. Full modeled memory, result, traces and preserved registers compared against independent model. No hardware/fault/concurrency proof or exact credit.'}
print(json.dumps(summary,indent=2));(ROOT/'build/dot_effect_context_replay/results.json').write_text(json.dumps(summary,indent=2))
```
