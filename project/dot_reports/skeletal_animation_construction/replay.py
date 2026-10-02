from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import *
from unicorn.arm_const import *
import struct,hashlib,json,random,time,sys
D=Path('build/skeletal_animation_validation');B=Path('data/ver/eu/code.bin').read_bytes()
assert hashlib.sha256(B).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
ROOT=0x1c440c;STOP=0x600000;ALLOCATE=0x600100;RELEASE=0x600104;HEAP_ALLOCATE=0x600108
BASE=0x800000;SIZE=0x80000;SELF=BASE;ARCH=BASE+0x100;MODEL=BASE+0x300;ALLOC=BASE+0x800;AVT=BASE+0x840;HEAP=BASE+0x880;HVT=BASE+0x900
SKEL=BASE+0xa00;MRES=BASE+0xb00;CONTEXT=BASE+0xd00;CRES=BASE+0xe00;OWNER=BASE+0xf00;BIND=BASE+0x1000;ENTRIES=BASE+0x1200
ARPTR=BASE+0x1500;ARES=BASE+0x1600;DICT=BASE+0x1800;RECORDS=BASE+0x2000;NAMES=BASE+0x3000;VEC=BASE+0x4000;VECBUF=BASE+0x4100
SP=0xa10000
REGS=[UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3];CALLEE=[UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11]
CANARIES=[0x12345600+i for i in range(8)]
segments=[]
with open(D/'candidate.axf','rb') as f:
 e=ELFFile(f);ENTRY=e.header.e_entry
 for s in e.iter_segments():
  if s['p_type']=='PT_LOAD':segments.append((s['p_vaddr'],s.data()))

def put(u,a,*values):u.mem_write(a,struct.pack('<'+'I'*len(values),*[v&0xffffffff for v in values]))
def get(u,a):return struct.unpack('<I',u.mem_read(a,4))[0]
def string(u,a):
 if not a:return None
 out=b''
 for _ in range(1024):
  c=bytes(u.mem_read(a+len(out),1))
  if c==b'\0':return out.decode('ascii')
  out+=c
 raise AssertionError('unterminated string')
coverage=set();callee_counts={};failures=[]

def run(case,candidate):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176)
 u.mem_map(0x100000,0x400000);u.mem_write(0x100000,B)
 u.mem_map(0x500000,0x10000)
 for a,b in segments:u.mem_write(a,b)
 u.mem_map(0x600000,0x1000);u.mem_map(BASE,SIZE);u.mem_write(BASE,b'\xa5'*SIZE)
 u.mem_map(0xa00000,0x20000);u.mem_write(0xa00000,b'\x69'*0x20000)
 u.reg_write(UC_ARM_REG_C1_C0_2,0xf00000);u.reg_write(UC_ARM_REG_FPEXC,0x40000000);u.reg_write(UC_ARM_REG_FPSCR,case.get('fpscr',0))
 put(u,ALLOC,AVT);put(u,AVT,0,0,ALLOCATE,RELEASE);put(u,HEAP,HVT);put(u,HVT,0,0,0,0,0,HEAP_ALLOCATE)
 put(u,ARCH+0x98,ARPTR);put(u,ARPTR,ARES);put(u,ARES+0x64,len(case['tracks']),DICT-(ARES+0x68))
 for i,tracks in enumerate(case['tracks']):
  rec=RECORDS+i*0x40;name=NAMES+i*0x40
  put(u,DICT+0x28+i*0x10,rec-(DICT+0x28+i*0x10));put(u,rec+8,name-(rec+8));put(u,rec+0x18,tracks)
  u.mem_write(name,('animation_%d'%i).encode()+b'\0')
 put(u,MODEL+8,MRES);put(u,MRES,0x40000092);put(u,MRES+0xe0,SKEL-(MRES+0xe0));put(u,SKEL+0x18,case['joints']);put(u,SKEL+0x28,case['flags'])
 put(u,MODEL+0x28,BIND);put(u,MODEL+0x220,OWNER);put(u,MODEL+0x228,CONTEXT,case['index'])
 put(u,CONTEXT+8,CRES);put(u,CRES+0x10,case['models']);put(u,CRES+0x14,0)
 put(u,BIND+0xc,ENTRIES,ENTRIES+case['bindings']*4);put(u,BIND+0x1c,ENTRIES+0x100);put(u,BIND+0x28,case['stride'])
 put(u,OWNER+0x10,VEC);put(u,VEC+4,ALLOC,VECBUF,VECBUF,0x40000008)
 if case.get('alias')=='records' and case['tracks']:
  for i in range(len(case['tracks'])):put(u,DICT+0x28+i*0x10,RECORDS-(DICT+0x28+i*0x10))
 if case.get('alias')=='binding_self':put(u,BIND+0x1c,SELF)
 if case.get('bad')=='resource_null':put(u,MODEL+8,0)
 if case.get('bad')=='resource_type':put(u,MRES,0)
 if case.get('bad')=='skeleton_null':put(u,MRES+0xe0,0)
 if case.get('bad')=='dictionary_null':put(u,ARES+0x68,0)
 if case.get('bad')=='record_null':put(u,DICT+0x28,0)
 if case.get('bad')=='name_null':put(u,RECORDS+8,0)
 for reg,val in zip(CALLEE,CANARIES):u.reg_write(reg,val)
 for i in range(8,16):u.reg_write(UC_ARM_REG_D0+i,0x7654321000000000+i)
 state={'next':BASE+0x10000,'allocs':[],'frees':[],'fault':None,'instructions':0,'calls':{},'strings':[],'writes':[],'resources':[]}
 def ret(value=0):u.reg_write(UC_ARM_REG_R0,value&0xffffffff);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 def hook(uc,a,size,unused):
  state['instructions']+=1
  if not candidate and ROOT<=a<0x1c4c80:coverage.add(a)
  if a==ROOT and candidate:u.reg_write(UC_ARM_REG_PC,ENTRY);return
  if a==STOP:u.emu_stop();return
  if a==0x293088:
   state['calls'][str(a)]=state['calls'].get(str(a),0)+1;ret(HEAP);return
  if a in (ALLOCATE,HEAP_ALLOCATE):
   size=u.reg_read(UC_ARM_REG_R1);align=u.reg_read(UC_ARM_REG_R2)
   n=len(state['allocs']);fail=(case.get('fail')==n)
   assert 0<=size<0x30000,(case,size,a)
   result=0 if fail else (state['next']+max(align,1)-1)&-max(align,1)
   state['allocs'].append([a,size,align,result])
   if u.reg_read(UC_ARM_REG_LR)==0x24e4b8 and result:state['resources'].append(result)
   if result:state['next']=result+max(size,4)
   ret(result);return
  if a==RELEASE:state['frees'].append(u.reg_read(UC_ARM_REG_R1));ret();return
  if a in (0x24e47c,0x1d956c,0x24e598,0x24e568,0x24e49c,0x26ac60,0x2a63d8,0x298878,0x2a69e8):
   state['calls'][str(a)]=state['calls'].get(str(a),0)+1
   if a==0x24e49c:state['strings'].append(string(u,u.reg_read(UC_ARM_REG_R1)))
 def fault(uc,access,address,size,value,unused):
  state['fault']=[access,address,size];return False
 u.hook_add(UC_HOOK_CODE,hook);u.hook_add(UC_HOOK_MEM_INVALID,fault)
 for iteration in range(case.get('repeats',1)):
  u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,STOP)
  if case.get('caller'):
   for r,v in zip(REGS,[ARCH,MODEL,ALLOC,case['count']]):u.reg_write(r,v)
   start=0x1c43b8
  else:
   for r,v in zip(REGS,[SELF,ARCH,MODEL,ALLOC]):u.reg_write(r,v)
   put(u,SP,case['count']);start=ROOT
  try:u.emu_start(start,STOP+4,count=200000)
  except UcError:
   if state['fault'] is None:raise
  if state['fault'] or u.reg_read(UC_ARM_REG_PC)!=STOP:break
 pc=u.reg_read(UC_ARM_REG_PC);state['return']=u.reg_read(UC_ARM_REG_R0)
 state['stopped']='fault' if state['fault'] else 'return' if pc==STOP else 'budget'
 # The generated resource stores a relative reference to the caller's identical
 # source string. Compare its resolved content, then normalize that one relocation.
 mem=bytearray(u.mem_read(BASE,SIZE))
 for ptr in state['resources']:
  if ptr:
   target=(ptr+0xc+get(u,ptr+0xc))&0xffffffff
   assert string(u,target)=='SkeletalAnimation'
   struct.pack_into('<I',mem,ptr+0xc-BASE,(0x1c4908-(ptr+0xc))&0xffffffff)
 state['memory']=bytes(mem);state['globals']=bytes(u.mem_read(0x3e0000,0x120000));state['fpscr']=u.reg_read(UC_ARM_REG_FPSCR)&0x9f
 if state['stopped']=='return':
  assert u.reg_read(UC_ARM_REG_SP)==SP,(case,'sp')
  assert [u.reg_read(r) for r in CALLEE]==CANARIES,(case,'registers')
  assert [u.reg_read(UC_ARM_REG_D0+i) for i in range(8,16)]==[0x7654321000000000+i for i in range(8,16)],(case,'vfp')
 return state

def compare(case):
 a=run(case,False);b=run(case,True)
 for key in ['stopped','fault','allocs','frees','strings','memory','globals','fpscr','calls']+(['return'] if a['stopped']=='return' else []):
  if a[key]!=b[key]:
   info={'case':case,'key':key,'original':str(a[key])[:1000],'candidate':str(b[key])[:1000]}
   if key in ('memory','globals'):
    ds=[i for i,(x,y) in enumerate(zip(a[key],b[key])) if x!=y];info['differences']=ds[:100];info['values']=[[hex(i+(BASE if key=='memory' else 0x3e0000)),a[key][i:i+4].hex(),b[key][i:i+4].hex()]for i in ds[:10]]
   (D/'failure.json').write_text(json.dumps(info,indent=2));raise AssertionError(info)
 return {k:a[k] for k in ['stopped','fault','instructions','calls','allocs']}

if __name__=='__main__':
 cases=[dict(count=1,tracks=[1],joints=1,models=1,flags=2,index=0,bindings=2,stride=1)]
 if len(sys.argv)>1 and sys.argv[1]=='full':
  r=random.Random(0x1c440c)
  cases=[]
  for i in range(240):
   cases.append(dict(count=r.choice([-3,0,1,2,3,4,8]),tracks=[r.randint(-2,6) for _ in range(r.randint(0,6))],joints=r.randint(0,6),models=r.randint(0,6),flags=r.choice([0,1,2,3,0xffffffff]),index=r.choice([-2,-1,0,1,2,5]),bindings=r.randint(0,5),stride=r.choice([-1,0,1,2,3]),fpscr=r.choice([0,0x01000000,0x02000000,0x00400000,0x00800000,0x00c00000])))
 start=time.time();results=[]
 for c in cases:
  results.append(compare(c))
 print(json.dumps({'cases':len(cases),'seconds':time.time()-start,'coverage':len(coverage),'returns':sum(x['stopped']=='return' for x in results),'faults':sum(x['stopped']=='fault' for x in results)}))
 (D/'result.json').write_text(json.dumps({'cases':cases,'results':results,'seconds':time.time()-start,'coverage':sorted(coverage)},indent=2))
