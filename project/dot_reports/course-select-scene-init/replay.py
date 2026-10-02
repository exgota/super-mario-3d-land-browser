#!/usr/bin/env python3
"""Bounded whole-root ARM replay; system/resource callees have explicit models."""
import argparse, hashlib, json, random, struct, sys, time
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_ARM, UC_MODE_ARM, UC_HOOK_CODE, UC_HOOK_MEM_INVALID, UcError
from unicorn.arm_const import *
ROOT=Path(__file__).resolve().parents[3]
START=0x17cb1c
REGS=[UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3]
SCENE=0x600000; SOURCE=0x601000; KIT=0x602000; APP=0x603000
GLOBAL=0x604000; RESOURCES=0x605000; ALT_CAMERA=0x606000
PLAYERDATA=0x607000; POSITION=0x608000; SOURCEVT=0x609000
STRINGS=0x610000; STACK=0xa10000; STOP=0x700100
REAL={0x27654c,0x25bd08,0x257ff8,0x243ff8,0x27b31c,0x2910e8,
      0x276c3c,0x2765d0,0x275514,0x275048,0x32bd64,0x276774}
# The wrappers below also execute their unchanged tail and direct helpers:
# 25BD08->2568CC(model)->25BD18; 243FF8->244004;
# 2765D0->1C32A8->24E638(model); 32BD64->28028C;
# 276774->28E688(model) then real VFP subtraction/division;
# 257FF8->28E1E4(format model); fixed-string termination 39E0E4.
ADDITIONAL_MODELS={0x2568cc,0x24e638,0x28e688,0x700000,0x700004,0x700008,0x70000c}
P=lambda *x:struct.pack('<'+'I'*len(x),*(v&0xffffffff for v in x))
def u32(b):return struct.unpack('<I',b)[0]
def signed(v):return v-(1<<32) if v&0x80000000 else v
class ModeledFault(Exception):pass

class Runner:
 def __init__(self,original,body,imports,case):
  self.u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u=self.u
  u.mem_map(0x100000,0x340000);u.mem_write(0x100000,original)
  if body:u.mem_write(START,body)
  u.mem_map(0x600000,0x100000);u.mem_map(0x700000,0x1000)
  u.mem_map(0x800000,0x100000);u.mem_map(0xa00000,0x20000)
  u.reg_write(UC_ARM_REG_C1_C0_2,0xf00000);u.reg_write(UC_ARM_REG_FPEXC,0x40000000)
  u.reg_write(UC_ARM_REG_FPSCR,0)
  self.case=case;self.trace=[];self.calls={};self.labels={};self.alloc=[];self.next=0x800000
  self.cycles=0;self.root_pcs=set();self.status=None;self.fault=None
  u.mem_write(SCENE,bytes([0xa5])*0x6c);u.mem_write(KIT,bytes([0xb6])*0x60)
  u.mem_write(SOURCE,P(SOURCEVT));u.mem_write(SOURCEVT,P(0x700000,0x700004,0x700008,0x70000c))
  self.put(SCENE+0x34,SOURCE);self.put(SCENE+0x38,0x612000)
  self.put(SCENE+0x10,KIT);self.put(GLOBAL+0x1c,0x613000)
  self.put(KIT+4,0x614000);self.put(KIT+8,0x615000)
  u.mem_write(APP+0x20,struct.pack('<4f',*case['rectangle']))
  self.put(RESOURCES,RESOURCES+0x20);self.put(RESOURCES+0x20,0x616000,0x617000)
  self.put(PLAYERDATA+0x14,POSITION);self.put(POSITION+4,case['position_bits'])
  for i in range(64):u.mem_write(STRINGS+i*64,('World%dCourse%d'%(case['world'],i)).encode()+b'\0')
  for i in range(4,12):u.reg_write(UC_ARM_REG_R0+i,0xb0000000+i)
  u.reg_write(UC_ARM_REG_S16,0x3f765432);u.reg_write(UC_ARM_REG_S17,0xbf123456)
  u.reg_write(UC_ARM_REG_SP,STACK);u.reg_write(UC_ARM_REG_LR,STOP);u.reg_write(UC_ARM_REG_R0,SCENE)
  self.models={i['address'] for i in imports if i['kind']=='A'}-REAL|ADDITIONAL_MODELS
  u.hook_add(UC_HOOK_CODE,self.hook);u.hook_add(UC_HOOK_MEM_INVALID,self.invalid)
 def read(self,p,n):return bytes(self.u.mem_read(p,n))
 def word(self,p):return u32(self.read(p,4))
 def words(self,p,n):return [self.word(p+i*4) for i in range(n)]
 def put(self,p,*v):self.u.mem_write(p,P(*v))
 def string(self,p):
  if not p:return None
  out=bytearray()
  for i in range(2048):
   b=self.read(p+i,1)
   if b==b'\0':return out.decode('cp932')
   out.extend(b)
  raise ModeledFault('unterminated modeled string')
 def norm(self,p):
  if p in self.labels:return self.labels[p]
  if 0xa00000<=p<0xa20000:return 'unlabelled_stack'
  return p
 def obj(self,p,n):return [self.norm(x) for x in self.words(p,n)]
 def log(self,a,*values):self.trace.append([hex(a),*values])
 def done(self,value=0):
  u=self.u
  for reg in [UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3,UC_ARM_REG_R12]:u.reg_write(reg,0xcc000000+reg)
  for i in range(16):u.reg_write(UC_ARM_REG_S0+i,0x3f000000+i)
  u.reg_write(UC_ARM_REG_R0,value&0xffffffff);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 def require(self,p,what):
  if not p:raise ModeledFault('null '+what)
 def invalid(self,u,access,address,size,value,data):
  self.fault=['memory',access,address,size];return False
 def hook(self,u,a,size,data):
  self.cycles+=1
  if START<=a<0x17d398:self.root_pcs.add(a)
  if a==STOP:self.status='returned';u.emu_stop();return
  if self.cycles>200000:self.status='cycle_limit';u.emu_stop();return
  if a==0x32bd64 and self.case['camera_alias']:
   self.put(SCENE+0x58,ALT_CAMERA);self.log(a,'modeled callback changes scene camera pointer')
  if a in REAL:
   # Name stack objects at their first real initialization for normalized traces.
   p=u.reg_read(UC_ARM_REG_R0)
   if a==0x27b31c:self.labels[p]='layout'
   if a==0x2910e8:self.labels[p]='placement'
   if a==0x276c3c:self.labels[p]='actor_info'
   return
  if a not in self.models:return
  r=[u.reg_read(x) for x in REGS];sp=u.reg_read(UC_ARM_REG_SP);ret=0
  self.calls[a]=self.calls.get(a,0)+1
  try:
   if a==0x700000:ret=STRINGS
   elif a==0x700004:ret=self.case['scenario']
   elif a==0x700008:ret=self.case['world']
   elif a==0x70000c:ret=self.case['course']
   elif a==0x2932b0:
    n=len(self.alloc)+1;ret=0 if self.case['fail_allocation']==n else self.next
    self.alloc.append([ret,r[0]]);self.log(a,r[0],ret)
    if ret:u.mem_write(ret,bytes([0xc7])*r[0]);self.next+=(r[0]+255)&~255
   elif a==0x276858:ret=0x618000;self.log(a)
   elif a==0x1de86c:self.log(a,r[0])
   elif a==0x26ad40:ret=self.case['special'];self.log(a,self.words(r[0],2))
   elif a==0x216f1c:ret=self.case['special_number'];self.log(a,self.words(r[0],2))
   elif a==0x28e1e4:
    fmt=self.string(r[1]);args=[r[2],r[3],self.word(sp)];types=[fmt[i+1] for i,c in enumerate(fmt[:-1]) if c=='%']
    values=[self.string(v) if t=='s' else signed(v) for v,t in zip(args,types)]
    result=(fmt%tuple(values)).encode('cp932');ptr=self.word(r[0]+4);capacity=self.word(r[0]+8)
    result=result[:capacity-1];u.mem_write(ptr,result+b'\0');ret=len(result)
    self.log(a,fmt,values,result.decode('cp932'))
   elif a==0x257f74:ret=0x619000;self.log(a)
   elif a==0x1dcba8:
    self.log(a,r[0],self.string(r[1]),r[2],r[3]);self.put(SCENE+0x24,RESOURCES)
   elif a==0x26afbc:
    ret=self.case['counts'][min(self.calls[a]-1,1)];self.log(a,r[0],r[1],ret)
   elif a==0x2568cc:
    pair=self.words(r[0],2);ret=self.case['types'][pair[1]%len(self.case['types'])];self.log(a,pair,ret)
   elif a==0x2527d8:ret=self.case['scenario']+r[2];self.log(a,*r[:3],ret)
   elif a==0x26af80:ret=STRINGS+r[2]*64;self.log(a,*r[:3],self.string(ret))
   elif a==0x257f88:self.log(a,self.string(self.word(r[0]+4)),r[1])
   elif a==0x257ed0:self.put(r[0],0x620000,0x620100,0x620200);ret=r[0];self.log(a,r[0])
   elif a==0x16b044:self.log(a)
   elif a==0x27baac:ret=GLOBAL;self.log(a)
   elif a==0x16b6c8:self.log(a,r[0])
   elif a==0x1891a8:self.put(r[0]+0x18,0x621000);self.log(a,r[0])
   elif a==0x276810:self.put(r[0]+0x1c,0x622000);self.log(a,r[0])
   elif a==0x2767a0:self.put(r[0]+0x10,KIT);self.log(a,r[0])
   elif a==0x28e688:ret=APP;self.log(a)
   elif a==0x1e6e5c:
    self.log(a,r[0],self.string(r[1]),self.words(r[2],9),u.reg_read(UC_ARM_REG_S0))
    self.put(r[0]+4,0x623000,0x624000);self.put(r[0]+0x2c,0x625000);ret=r[0]
   elif a==0x268fe8:self.log(a,*r[:3])
   elif a==0x276660:self.put(r[0]+0x14,0x626000);self.log(a,r[0])
   elif a==0x24e638:ret=self.case['view_id'];self.log(a,self.obj(r[0],2))
   elif a==0x275748:
    stack=self.words(sp,3);self.log(a,r[0],self.string(r[1]),r[2],r[3],self.string(self.word(stack[0]+4)),self.string(stack[1]),stack[2])
   elif a==0x2765a8:self.log(a,r[0])
   elif a==0x1a69e0:self.put(r[0]+4,r[1]);self.log(a,*r[:2]);ret=r[0]
   elif a==0x275638:self.log(a,r[0]);ret=r[0]
   elif a==0x1cb48c:self.log(a,*r[:2])
   elif a==0x26ee44:self.log(a)
   elif a in [0x2753e8,0x275054]:self.log(a,*r[:3])
   elif a==0x17bfac:
    self.put(r[0],r[1],0x627000,self.case['selection']);self.log(a,*r[:2]);ret=r[0]
   elif a==0x188f20:
    self.put(r[0]+8,PLAYERDATA);self.log(a,r[0],self.string(r[1]),r[2],r[3]);ret=r[0]
   elif a==0x188db0:self.require(r[0],'player');self.log(a,r[0],self.obj(r[1],6))
   elif a==0x17da74:
    self.log(a,*r[:3],self.obj(r[3],6),self.obj(self.word(sp),3));ret=r[0]
   elif a==0x27ee64:self.require(r[0],'map');self.log(a,r[0],self.obj(r[1],6))
   elif a==0x17d9a0:self.log(a,*r[:2])
   elif a==0x18899c:self.log(a,r[0],self.string(r[1]),r[2],self.obj(r[3],6),self.obj(self.word(sp),3));ret=r[0]
   elif a==0x266bac:self.log(a,r[0],self.string(r[1]),self.string(r[2]),self.obj(r[3],3),self.word(sp));ret=r[0]
   elif a==0x13dccc:self.log(a,r[0],self.string(r[1]),self.obj(r[2],3));ret=r[0]
   elif a==0x27361c:self.log(a,r[0],*[self.string(v) for v in r[1:]],self.obj(self.word(sp),3),self.word(sp+4));ret=r[0]
   elif a==0x274ef0:self.log(a,*r[:2],self.obj(r[2],6),self.string(r[3]))
   elif a==0x277658:ret=0x628000;self.log(a)
   elif a==0x274aa8:self.log(a,r[0],self.obj(r[1],3))
   elif a==0x274a7c:self.log(a,r[0],self.obj(r[1],6))
   elif a==0x27b364:self.put(r[0]+4,0x629000);self.log(a,*r[:3])
   elif a==0x1b979c:self.log(a,*r,self.obj(self.word(sp),6),self.obj(self.word(sp+4),3));ret=r[0]
   elif a==0x1b899c:self.log(a,*r[:3],self.obj(r[3],3));ret=r[0]
   elif a==0x1cacb4:self.log(a,*r[:3],self.string(r[3]))
   elif a==0x274990:self.log(a,r[0])
   elif a==0x243260:self.log(a,self.string(self.word(r[0]+4)));ret=0x630000
   else:raise RuntimeError('Missing model '+hex(a))
   self.done(ret)
  except ModeledFault as exc:
   self.status='modeled_fault';self.fault=str(exc);u.emu_stop()
 def run(self):
  try:self.u.emu_start(START,0,count=200010)
  except UcError as exc:
   self.status='fault';self.fault=self.fault or str(exc)
  if not self.status:self.status='unexpected_stop'
  return {'status':self.status,'fault':self.fault,'trace':self.trace,'scene':self.read(SCENE,0x6c).hex(),
          'heap':[(p,n,self.read(p,n).hex()) for p,n in self.alloc if p],
          'alt_camera':self.read(ALT_CAMERA,0x38).hex(),
          'preserved_registers':[self.u.reg_read(UC_ARM_REG_R0+i) for i in range(4,12)] if self.status=='returned' else None,
          'preserved_d8':[self.u.reg_read(UC_ARM_REG_S16),self.u.reg_read(UC_ARM_REG_S17)] if self.status=='returned' else None,
          'fpscr':self.u.reg_read(UC_ARM_REG_FPSCR),
          'sp_restored':self.u.reg_read(UC_ARM_REG_SP)==STACK if self.status=='returned' else None}

def cases():
 rng=random.Random(0x17cb1c)
 for i in range(240):
  yield dict(id=i,world=rng.randrange(16),course=rng.randrange(16),scenario=rng.randrange(1,8),
   counts=[rng.choice([-2147483648,-1,0,1,2,7,8,16,32]),rng.choice([-1,0,1,2,7,16,32])],
   types=[rng.choice([0,1,2,3,4,5,6,10,0xffffffff]) for _ in range(16)],
   special=rng.choice([0,1,255]),special_number=rng.randrange(1,9),selection=rng.randrange(32),
   position_bits=rng.choice([0,0x80000000,0x3f800000,0xc2480000,0x3eaaaaab]),
   rectangle=rng.choice([[0,0,400,240],[5,7,5,247],[-20,-10,320,170],[1,2,13,5]]),
   view_id=rng.choice([0,1,3,-1]),camera_alias=(i%13==0),fail_allocation=0)
 base=dict(id=240,world=1,course=2,scenario=1,counts=[0,0],types=[0],special=0,
           special_number=1,selection=4,position_bits=0x3f800000,rectangle=[0,0,400,240],
           view_id=-1,camera_alias=False,fail_allocation=0)
 for n in range(1,15):yield dict(base,id=240+n,fail_allocation=n)

if __name__=='__main__':
 ap=argparse.ArgumentParser();ap.add_argument('--directory',default='build/course-form2');a=ap.parse_args()
 directory=ROOT/a.directory;original=(ROOT/'data/ver/eu/code.bin').read_bytes()
 assert hashlib.sha256(original).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
 with (directory/'diagnostic.axf').open('rb') as stream:
  elf=ELFFile(stream);section=elf.get_section_by_name('BODY');assert section['sh_addr']==START;body=section.data()
 imports=json.loads((directory/'imports.json').read_text());started=time.time();passed=[];failures=[];statuses={};original_pcs=set();compiled_pcs=set();instructions=[0,0]
 fixtures=json.loads((Path(__file__).parent/'fixtures.json').read_text());assert fixtures==list(cases())
 for case in fixtures:
  reference=Runner(original,None,imports,case);compiled=Runner(original,body,imports,case)
  ref=reference.run();cur=compiled.run()
  original_pcs.update(reference.root_pcs);compiled_pcs.update(compiled.root_pcs)
  instructions[0]+=reference.cycles;instructions[1]+=compiled.cycles
  if ref!=cur:
   failure={'case':case,'reference':ref,'compiled':cur};failures.append(failure)
   (directory/'replay-first-failure.json').write_text(json.dumps(failure,indent=2));break
  passed.append(case['id']);statuses[ref['status']]=statuses.get(ref['status'],0)+1
 report={'fixture_entries':len(fixtures),'executed_pairs':len(passed)+len(failures),'fixture_ids':'0-239 and 241-254; 240 is only an uninstantiated generator template label, not an excluded case','original_root_instructions_visited':len(original_pcs),'compiled_root_instructions_visited':len(compiled_pcs),'total_instructions_original_compiled':instructions,'cases':len(passed),'failures':len(failures),'statuses':statuses,'elapsed_seconds':time.time()-started,
         'real_direct_entries':[hex(x) for x in sorted(REAL)],'modeled_entries':[hex(x) for x in sorted(Runner(original,None,imports,next(cases())).models)],
         'body_sha256':hashlib.sha256(body).hexdigest(),'all_passed':not failures,
         'limits':'200000 instructions/case; at most 32 courses/pass; original globals/data, isolated non-overlapping scene/heap/stack; modeled heavy subsystem effects and allocations; no full scene/render/gameplay or exact-byte claim'}
 (directory/'replay.json').write_text(json.dumps(report,indent=2));print(json.dumps(report,indent=2));sys.exit(bool(failures))
