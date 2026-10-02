from pathlib import Path
from collections import Counter
import json,struct,time,hashlib,csv,random
from elftools.elf.elffile import ELFFile
from unicorn import *
from unicorn.arm_const import *
OUT=Path('build/primitive-draw-setup');ROOT=0x2e0d2c;END=0x2e1544;CAND=0x500000;STOP=0x700000
DATA=0x800000;SIZE=0x100000;SELF=DATA+0x100;HEAP=DATA+0x10000;VT=HEAP+0x100;THREAD=HEAP+0x200;SHADER=DATA+0x20000;ARENA=0x14000000;ASIZE=0x200000;SP=0xa08000
ALLOC=STOP+0x100;FREE=STOP+0x104;RESIZE=STOP+0x108;LARGEST=STOP+0x10c
CODE=Path('data/ver/eu/code.bin').read_bytes();assert hashlib.sha256(CODE).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
elf=ELFFile(open(OUT/'behavior.axf','rb'));sections=[s for s in elf.iter_sections() if s['sh_flags']&2 and s['sh_size']]
entries={int(r[0],16) for r in csv.reader(open('data/ver/eu/map.csv')) if r and r[0].startswith('0x') and r[5].strip().startswith('f')}
regs=[UC_ARM_REG_R4+i for i in range(8)];vregs=[UC_ARM_REG_D8+i for i in range(8)];saved=[0xabcd0000+i for i in range(8)];vsaved=[0x7fe1234512340000+i for i in range(8)]
def rd(u,a):return struct.unpack('<I',u.mem_read(a,4))[0]
def wr(u,a,*v):u.mem_write(a,struct.pack('<'+'I'*len(v),*(x&0xffffffff for x in v)))
def string(u,a):
 b=bytearray()
 for i in range(256):
  c=u.mem_read(a+i,1)[0]
  if not c:return b.decode('ascii')
  b.append(c)
 raise RuntimeError('string limit')
def shader(u,missing=False,commands=1):
 u.mem_write(SHADER,bytes(0x1000));wr(u,SHADER,0x424c5644,2,0x100,0x500)
 wr(u,SHADER+16,0x504c5644,0,0x800,commands,0x900,1)
 for i in range(commands):wr(u,SHADER+16+0x800+4*i,0x10000000+i)
 names=['wvp','user','color0','color1','uv_src','uv_size','Vertex','TexCoord0','ColorRate','dmp_Line.width']
 for k in range(2):
  p=SHADER+0x100+k*0x400;wr(u,p,0x454c5644);u.mem_write(p+6,bytes([k,0]));wr(u,p+8,0);wr(u,p+0x18,0x40,0);wr(u,p+0x28,0x40,0);wr(u,p+0x30,0x60,len(names),0x100)
  offset=0
  for i,name in enumerate(names):
   # Attribute index 0..2; float uniform registers 16 and up. A prefix boundary is also exercised.
   actual=name+'.x' if missing and i<6 else name
   b=actual.encode()+b'\0';u.mem_write(p+0x100+offset,b)
   register=i-6 if i in (6,7,8) else 16+i
   wr(u,p+0x60+i*8,offset,register|(register<<16));offset+=len(b)
coverage=set();all_calls=[Counter(),Counter()];all_models=[Counter(),Counter()];cases=[];failures=[];machines=[];state=[{},{}];trace=[[],[]]
for j in range(2):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176)
 for a,n in [(0x100000,0x400000),(CAND,0x10000),(STOP,0x1000),(DATA,SIZE),(0xa00000,0x10000),(ARENA,ASIZE)]:u.mem_map(a,n)
 u.mem_write(0x100000,CODE)
 for s in sections:u.mem_write(s['sh_addr'],s.data())
 def hook(uc,a,size,arg,j=j):
  st=state[j]
  if not st.get('active'):return
  st['steps']+=1
  if j==0 and ROOT<=a<END:coverage.add(a)
  if a in entries:
   st['calls'][a]+=1;trace[j].append(a)
   if len(trace[j])>30:trace[j].pop(0)
  if a==0x220a6c:
   r=[uc.reg_read(UC_ARM_REG_R0+k) for k in range(3)];st['semantic'].append(['attribute',r[1]-st['self'],string(uc,r[2])])
  if a in [0x220f04,0x2206e0]:
   r=[uc.reg_read(UC_ARM_REG_R0+k) for k in range(4)];st['semantic'].append(['shape_allocate' if a==0x220f04 else 'shape_copy',r[2]-st['self'] if a==0x220f04 else r[1]-st['self'],r[3],rd(uc,uc.reg_read(UC_ARM_REG_SP))])
  if a not in [ALLOC,FREE,RESIZE,LARGEST,0x28cf4c,0x28e8f8,0x296048]:return
  r=[uc.reg_read(UC_ARM_REG_R0+k) for k in range(4)];ret=0
  if a==ALLOC:
   size=r[1];alignment=abs(struct.unpack('<i',struct.pack('<I',r[2]))[0]);assert alignment and alignment&(alignment-1)==0
   if st['fail_alloc']==len(st['allocs']):ret=0
   else:
    ret=(st['next']+alignment-1)&-alignment;st['next']=ret+size+0x80;assert st['next']<ARENA+ASIZE;st['allocs'][ret]=size
   st['events'].append(['allocate',size,r[2],ret])
  elif a==LARGEST:ret=st['largest'];st['events'].append(['largest',r[1],ret])
  elif a==RESIZE:
   old=r[1];n=r[2];assert n<=st['allocs'][old]
   if st['move']:
    ret=(st['next']+31)&-32;st['next']=ret+n+0x80;uc.mem_write(ret,bytes(uc.mem_read(old,n)));st['allocs'][ret]=n
   else:ret=old
   st['events'].append(['resize',old,n,ret])
  elif a==FREE:st['events'].append(['free',r[1]]);ret=0
  elif a==0x28cf4c:ret=THREAD;st['events'].append(['thread_local'])
  elif a==0x28e8f8:ret=HEAP;st['events'].append(['find_heap',r[1]])
  elif a==0x296048:ret=0;st['events'].append(['cache_flush',r[0],r[1]])
  st['models'][a]+=1
  if st['clobber']:
   for reg in [UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3,UC_ARM_REG_R12]:uc.reg_write(reg,0xface0000+reg)
   for k in range(8):uc.reg_write(UC_ARM_REG_D0+k,0x7feabc0012340000+k)
  uc.reg_write(UC_ARM_REG_R0,ret);uc.reg_write(UC_ARM_REG_PC,uc.reg_read(UC_ARM_REG_LR))
 u.hook_add(UC_HOOK_CODE,hook)
 def fault(uc,access,a,size,value,arg,j=j):state[j]['fault']=[access,a,size];return False
 u.hook_add(UC_HOOK_MEM_INVALID,fault);machines.append(u)
def run(case):
 out=[]
 for j,u in enumerate(machines):
  fill=case.get('fill',0x55);self=SELF+case.get('shift',0)
  u.mem_write(DATA,bytes([fill])*SIZE);u.mem_write(ARENA,bytes([fill])*ASIZE);u.mem_write(0xa00000,bytes([fill])*0x10000);u.mem_write(0x3e0000,CODE[0x2e0000:]+bytes(0x500000-(0x100000+len(CODE))))
  wr(u,HEAP,VT);wr(u,VT+0x14,ALLOC);wr(u,VT+0x18,FREE);wr(u,VT+0x20,RESIZE);wr(u,VT+0x38,LARGEST);wr(u,THREAD+0x64,HEAP);wr(u,0x3e23b8,HEAP);wr(u,0x3e2614,HEAP)
  shader(u,case.get('suffix',False),case.get('commands',1))
  state[j]={'active':False}
  u.reg_write(UC_ARM_REG_C1_C0_2,0xf00000);u.reg_write(UC_ARM_REG_FPEXC,0x40000000);u.reg_write(UC_ARM_REG_FPSCR,case.get('fpscr',0));u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,STOP);u.reg_write(UC_ARM_REG_R0,self)
  u.emu_start(0x10349c,STOP,count=200000)
  assert u.reg_read(UC_ARM_REG_PC)==STOP
  st={'active':True,'self':self,'steps':0,'calls':Counter(),'models':Counter(),'events':[],'semantic':[],'allocs':{},'next':ARENA+case.get('arena_shift',0),'largest':case.get('largest',0x4000),'move':case.get('move',False),'clobber':case.get('clobber',False),'fail_alloc':case.get('fail_alloc',-1),'fault':None};state[j]=st;trace[j]=[]
  for reg,v in zip(regs+vregs,saved+vsaved):u.reg_write(reg,v)
  u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,STOP)
  for k,v in enumerate([0 if case.get('null_receiver') else self,0 if case.get('null_heap') else HEAP,0 if case.get('null_shader') else SHADER,case.get('unused_r3',0)]):u.reg_write(UC_ARM_REG_R0+k,v)
  error=None
  try:u.emu_start(ROOT if j==0 else CAND,STOP,count=2000000)
  except UcError as e:error=str(e)
  returned=u.reg_read(UC_ARM_REG_PC)==STOP;pc=u.reg_read(UC_ARM_REG_PC)
  if returned:
   assert u.reg_read(UC_ARM_REG_SP)==SP
   assert [u.reg_read(r) for r in regs+vregs]==saved+vsaved
  st['calls'].pop(ROOT,None);all_calls[j].update(st['calls']);all_models[j].update(st['models'])
  out.append({'returned':returned,'error':error,'fault':st['fault'],'pc':pc,'data':bytes(u.mem_read(DATA,SIZE)),'arena':bytes(u.mem_read(ARENA,ASIZE)),'global':bytes(u.mem_read(0x3e0000,0x110000)),'fpscr':u.reg_read(UC_ARM_REG_FPSCR)&0xfffffff,'events':st['events'],'semantic':st['semantic'],'calls':st['calls'],'steps':st['steps'],'trace':[hex(x) for x in trace[j]]})
 comparable=['returned','error','fault','data','arena','global','fpscr','events','semantic','calls']
 mismatch=[k for k in comparable if out[0][k]!=out[1][k]]
 if mismatch or (not out[0]['returned'] and not case.get('expected_fault')):
  differences={k:[(hex((DATA if k=='data' else ARENA if k=='arena' else 0x3e0000)+i),x,y) for i,(x,y) in enumerate(zip(out[0][k],out[1][k])) if x!=y][:80] for k in ['data','arena','global'] if k in mismatch}
  failure={'case':case,'mismatch':mismatch,'differences':differences,'machines':[{k:v for k,v in o.items() if k not in ['data','arena','global']} for o in out]};(OUT/'replay-failure.json').write_text(json.dumps(failure,indent=2));print(json.dumps(failure,indent=2));raise AssertionError('replay divergence')
 cases.append({'fixture':case,'returned':out[0]['returned'],'fault':out[0]['fault'],'steps':[o['steps'] for o in out],'allocation_count':len(state[0]['allocs']),'call_count':sum(out[0]['calls'].values())})
if __name__=='__main__':
 start=time.monotonic()
 run({'group':'baseline'})
 for null in [False,True]:
  for move in [False,True]:
   for clobber in [False,True]:run({'group':'allocator_modes','null_heap':null,'move':move,'clobber':clobber,'fill':0x96})
 for n in [1,3,8,16,31]:run({'group':'shader_commands','commands':n,'suffix':True,'fill':n})
 for fpscr in [1<<22,2<<22,3<<22,1<<24,1<<25,(3<<22)|(3<<24)]:run({'group':'floating_modes','fpscr':fpscr,'fill':0xCC})
 for shift in [4,0xF00,0x1F00,0x8000]:run({'group':'receiver_alignment','shift':shift,'arena_shift':shift,'move':True,'clobber':True})
 for bad in ['null_receiver','null_shader']:run({'group':'fault_control',bad:True,'expected_fault':True})
 for alloc in [0,1,2,3,7,18,35,39,40]:run({'group':'allocation_failure','fail_alloc':alloc,'expected_fault':True})
 result={'cases':cases,'fixture_count':len(cases),'returned':sum(c['returned'] for c in cases),'equal_faults':sum(not c['returned'] for c in cases),'all_equal':True,'seconds':time.monotonic()-start,'root_instruction_addresses':[hex(a) for a in sorted(coverage)],'calls':all_calls,'models':all_models,'models_description':['Heap allocate,free,resize and largest-block vtable dispatch','TLS read 0028CF4C','heap-owner query 0028E8F8','cache flush 00296048'],'instruction_limit':2000000}
 (OUT/'replay-result.json').write_text(json.dumps(result,indent=2));print(json.dumps({k:v for k,v in result.items() if k not in ['cases','root_instruction_addresses','calls','models']},indent=2))
