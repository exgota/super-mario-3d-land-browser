"""Whole-root ARM replay; only fixture allocator callbacks are modeled."""
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import *
from unicorn.arm_const import *
import struct,hashlib,json,random,time,sys
D=Path('build/transform_state_validation');B=Path('data/ver/eu/code.bin').read_bytes()
assert hashlib.sha256(B).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
ROOT=0x2a2c9c;STOP=0x600000;ALLOCATE=0x600100;RELEASE=0x600104
BASE=0x800000;SIZE=0x80000;RESOURCE=BASE;DONOR=BASE+0x100;DONORBUF=BASE+0x400
ALLOC=BASE+0x1000;AVT=BASE+0x1040;SP=0xa10000
REGS=[UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3]
CALLEE=[UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11]
CANARIES=[0x12345600+i for i in range(8)]
segments=[]
with open(D/'candidate.axf','rb') as f:
 e=ELFFile(f);ENTRY=e.header.e_entry
 for s in e.iter_segments():
  if s['p_type']=='PT_LOAD':segments.append((s['p_vaddr'],s.data()))
def put(u,a,*values):u.mem_write(a,struct.pack('<'+'I'*len(values),*[v&0xffffffff for v in values]))
def get(u,a):return struct.unpack('<I',u.mem_read(a,4))[0]
coverage=set();original_callees=set();candidate_callees=set()

def run(case,candidate):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176)
 u.mem_map(0x100000,0x400000);u.mem_write(0x100000,B)
 u.mem_map(0x500000,0x10000)
 for a,b in segments:u.mem_write(a,b)
 u.mem_map(0x600000,0x1000);u.mem_map(BASE,SIZE);u.mem_write(BASE,bytes([case.get('fill',0xa5)])*SIZE)
 u.mem_map(0xa00000,0x20000);u.mem_write(0xa00000,b'\x69'*0x20000)
 u.reg_write(UC_ARM_REG_C1_C0_2,0xf00000);u.reg_write(UC_ARM_REG_FPEXC,0x40000000);u.reg_write(UC_ARM_REG_FPSCR,case.get('fpscr',0))
 put(u,ALLOC,AVT);put(u,AVT,0,0,ALLOCATE,RELEASE)
 put(u,RESOURCE+0x18,case['count'])
 donor=case.get('donor_address',DONOR)
 donorbuf=case.get('donor_buffer',DONORBUF)
 put(u,donor,case.get('donor_allocator',ALLOC),donorbuf,donorbuf+case.get('donor_count',2)*64,case.get('flags',0x40000008))
 if case.get('bad')=='donor_null':donor=0
 guards=case.get('guards',[0,0]);put(u,0x3f389c,guards[0]);put(u,0x3f38a8,guards[1])
 # Deliberately non-default values show whether initialization was actually run.
 patterns=[0x80000000,0x3f800000,0xbf800000,0x7f800001,0x7fc01234,0x41200000,0xff800000]
 for a,z in [(0x430c68,0x430c98),(0x430cf0,0x430d30)]:
  for i,p in enumerate(range(a,z,4)):put(u,p,patterns[(i+case.get('seed',0))%len(patterns)])
 for reg,val in zip(CALLEE,CANARIES):u.reg_write(reg,val)
 for i in range(8,16):u.reg_write(UC_ARM_REG_D0+i,0x7654321000000000+i)
 state={'next':BASE+0x10000,'allocs':[],'frees':[],'fault':None,'instructions':0,'calls':{}}
 def ret(value=0):u.reg_write(UC_ARM_REG_R0,value&0xffffffff);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 def hook(uc,a,size,unused):
  state['instructions']+=1
  if not candidate and ROOT<=a<0x2a34d4:coverage.add(a)
  if a==ROOT and candidate:u.reg_write(UC_ARM_REG_PC,ENTRY);return
  if a==STOP:u.emu_stop();return
  if a==ALLOCATE:
   size=u.reg_read(UC_ARM_REG_R1);align=u.reg_read(UC_ARM_REG_R2);n=len(state['allocs'])
   fail=case.get('fail')==n or n in case.get('fails',[])
   # Huge wrapped requests are observable, then this bounded allocator rejects.
   if size>0x30000:fail=True
   result=0 if fail else (state['next']+max(align,1)-1)&-max(align,1)
   if n==0 and 'self_address' in case and not fail:result=case['self_address']
   state['allocs'].append([size,align,result])
   if result and not(n==0 and 'self_address' in case):state['next']=result+max(size,4)
   if n in case.get('mutate_counts',{}):put(u,RESOURCE+0x18,case['mutate_counts'][n])
   ret(result);return
  if a==RELEASE:state['frees'].append(u.reg_read(UC_ARM_REG_R1));ret();return
  if a in (0x254890,0x28a998,0x291470,0x2b349c):
   state['calls'][str(a)]=state['calls'].get(str(a),0)+1
   (candidate_callees if candidate else original_callees).add(a)
 def fault(uc,access,address,size,value,unused):state['fault']=[access,address,size];return False
 u.hook_add(UC_HOOK_CODE,hook);u.hook_add(UC_HOOK_MEM_INVALID,fault)
 for iteration in range(case.get('repeats',1)):
  u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,STOP)
  resource=0 if case.get('bad')=='resource_null' else RESOURCE
  allocator=0 if case.get('bad')=='allocator_null' else ALLOC
  for r,v in zip(REGS,[resource,case['word'],case['mode']&0xffffffff,donor]):u.reg_write(r,v)
  put(u,SP,allocator)
  try:u.emu_start(ROOT,STOP+4,count=200000)
  except UcError:
   if state['fault'] is None:raise
  if state['fault'] or u.reg_read(UC_ARM_REG_PC)!=STOP:break
 pc=u.reg_read(UC_ARM_REG_PC);state['return']=u.reg_read(UC_ARM_REG_R0)
 state['stopped']='fault' if state['fault'] else 'return' if pc==STOP else 'budget'
 state['memory']=bytes(u.mem_read(BASE,SIZE));state['globals']=bytes(u.mem_read(0x3e0000,0x120000));state['fpscr']=u.reg_read(UC_ARM_REG_FPSCR)&0x9f
 if state['stopped']=='return':
  assert u.reg_read(UC_ARM_REG_SP)==SP,(case,'sp')
  assert [u.reg_read(r) for r in CALLEE]==CANARIES,(case,'registers')
  assert [u.reg_read(UC_ARM_REG_D0+i) for i in range(8,16)]==[0x7654321000000000+i for i in range(8,16)],(case,'vfp')
 return state

def compare(case):
 a=run(case,False);b=run(case,True)
 differences=[]
 for key in ['stopped','fault','allocs','frees','memory','globals','fpscr','calls']+(['return'] if a['stopped']=='return' else []):
  if a[key]!=b[key]:
   info={'case':case,'key':key,'original':str(a[key])[:400],'candidate':str(b[key])[:400]}
   if key in ('memory','globals'):
    ds=[i for i,(x,y) in enumerate(zip(a[key],b[key])) if x!=y];info['differences']=ds[:100];info['values']=[[hex(i+(BASE if key=='memory' else 0x3e0000)),a[key][i:i+4].hex(),b[key][i:i+4].hex()]for i in ds[:10]]
   differences.append(info)
 return {'case':case,'original':{k:a[k] for k in ['stopped','fault','instructions','calls','allocs','frees']},'candidate':{k:b[k] for k in ['stopped','fault','instructions','calls','allocs','frees']},'differences':differences}

def main():
 basic=dict(count=1,word=1,mode=1)
 cases=[basic]
 if len(sys.argv)>1 and sys.argv[1]=='full':
  r=random.Random(ROOT);cases=[]
  for i in range(240):
   cases.append(dict(count=r.choice([0,1,2,3,5,8]),word=r.choice([0,1,2,5]),mode=r.choice([-128,-1,0,1,2,127]),guards=[r.choice([0,1,2,3,0xffffffff]),r.choice([0,1,2,3,0xffffffff])],flags=r.choice([0,1,0x3fffffff,0x40000008,0x80000004,0xc0000002]),donor_count=r.choice([0,1,2,4]),fill=r.choice([0,0xa5,0xff]),seed=i,fpscr=r.choice([0,0x01000000,0x02000000,0x00400000,0x00800000,0x00c00000])))
  for count in [0,1,2,5]:
   for word in [0,2]:
    for mode in [0,1]:
     for fail in range(8):cases.append(dict(count=count,word=word,mode=mode,fail=fail))
  for bad in ['resource_null','allocator_null','donor_null']:
   for count in [0,1,2]:cases.append(dict(count=count,word=2,mode=1,bad=bad))
  for count in [-3,-1]:cases.append(dict(count=count,word=2,mode=1))
  for count in [0,1,3]:cases.append(dict(count=count,word=2,mode=1,repeats=2))
 start=time.time();results=[compare(c)for c in cases]
 report={'results':results,'seconds':time.time()-start,'coverage':sorted(coverage),'original_callees':sorted(original_callees),'candidate_callees':sorted(candidate_callees),'cases':len(cases),'matched':sum(not x['differences'] for x in results),'divergences':sum(bool(x['differences']) for x in results),'returns':sum(x['original']['stopped']=='return' for x in results),'faults':sum(x['original']['stopped']=='fault' for x in results),'budgets':sum(x['original']['stopped']=='budget' for x in results)}
 (D/'replay-result.json').write_text(json.dumps(report,indent=2));print(json.dumps({k:report[k]for k in ['cases','matched','divergences','returns','faults','budgets','seconds']}))
 for x in results:
  if x['differences']:print(json.dumps(x['differences']));break
 if report['divergences']:sys.exit(1)
if __name__=='__main__':main()
