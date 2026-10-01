# Shader binary ingestion bounded reproducer

This file contains only a synthetic diagnostic; it includes no owner binary, assets, copied game-function bytes or assembly stubs. Run from the repository root with the owner's verified ignored code.bin and the approved compiler present. Install the already approved Unicorn dependency in the local venv if it is absent. Build the committed source with the project, then save the Python block as `build/shader_binary_validation/reproduce.py` and run it. Outputs stay ignored.

```sh
. ./development_environment.sh
export DEVKITARM=/usr
python make.py eu -ca
mkdir -p build/shader_binary_validation
cat > build/shader_binary_validation/original_symbols.sym <<'SYMBOLS'
#<SYMDEFS>#
0x003E2E40 D dat_003E2E40
0x003E2654 D dat_003E2654
0x0028D1F0 A fn_0028D1F0
0x0028BA44 A __rt_memcpy
0x0020F690 A fn_0020F690
SYMBOLS
data/compilers/wibo data/compilers/4.0/902/bin/armlink.exe --no_scanlib --entry=fn_002478D8 --ro_base=0x600000 --inline --mangled --symbols --map --list=build/shader_binary_validation/candidate.map --output=build/shader_binary_validation/candidate.axf build/eu/obj/lib/CtrSDK/sources/ShaderBinary.o build/shader_binary_validation/original_symbols.sym
python build/shader_binary_validation/reproduce.py
```

Expected: 439 fixtures, no failures, 3382 callback events per executable, 1087/1087 visited retail code instruction addresses. Exact timings vary. The diagnostic object hash must be compared with the accompanying report; a different build is a new test result.

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import Uc, UcError, UC_ARCH_ARM, UC_MODE_ARM, UC_HOOK_CODE
from unicorn.arm_const import *
import struct, random, json, hashlib, time
ROOT=Path.cwd(); OUT=ROOT/'build/shader_binary_validation'
BINARY=(ROOT/'data/ver/eu/code.bin').read_bytes()
assert hashlib.sha256(BINARY).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
BASE=0x800000; C=0x800000; HANDLES=0x802000; SHADERS=0x803000; B=0x810000; OLD=0x840000; ALLOC=0x880000
SP=0xa1f000; END=0x980000; CALL=0x990000
REGS=[UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3,UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11,UC_ARM_REG_R12]
SNAPS=[('context',C,0x2000),('handles',HANDLES,0x1000),('shaders',SHADERS,0x3000),('binary',B,0x20000),('old',OLD,0x30000),('allocated',ALLOC,0x40000)]
def word(u,a,v): u.mem_write(a,struct.pack('<I',v&0xffffffff))
def init(candidate):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176)
 u.mem_map(0x100000,0x400000);u.mem_write(0x100000,BINARY);u.mem_map(0x600000,0x20000)
 entry=0x2478d8
 if candidate:
  with (OUT/'candidate.axf').open('rb') as f:
   e=ELFFile(f)
   for s in e.iter_sections():
    if s['sh_flags']&2 and s['sh_size'] and s['sh_type']=='SHT_PROGBITS':u.mem_write(s['sh_addr'],s.data())
   entry=e['e_entry']
 u.mem_map(BASE,0x100000);u.mem_map(0xa00000,0x20000);u.mem_map(END,0x1000);u.mem_map(CALL,0x1000)
 state={}
 if not candidate:
  u.hook_add(UC_HOOK_CODE, lambda uc,a,n,user: COVERED.add(a), begin=0x2478d8,end=0x248a3c)
 def hook(uc,a,n,user):
  args=[uc.reg_read(x) for x in REGS[:4]]
  if a==CALL:
   ix=state['next'];state['next']+=1
   result=0 if state.get('fail')==ix else ALLOC+ix*0x1000
   state['calls'].append(['allocate',args,result]);
   if state.get('removeCallback')==ix:word(uc,0x3e2654,0)
   uc.reg_write(UC_ARM_REG_R0,result)
  else: state['calls'].append(['release',args])
  for j in [1,2,3,12]:uc.reg_write(REGS[j],0xcc000000+j)
  uc.reg_write(UC_ARM_REG_PC,uc.reg_read(UC_ARM_REG_LR))
 u.hook_add(UC_HOOK_CODE,hook,begin=CALL,end=CALL+4)
 return u,entry,state
COVERED=set();ORIG=init(False);CAND=init(True)
def fixture(case):
 rng=random.Random(case['seed']);blob=bytearray(rng.randbytes(0x100000))
 def w(a,x):struct.pack_into('<I',blob,a-BASE,x&0xffffffff)
 def h(a,x):struct.pack_into('<H',blob,a-BASE,x&65535)
 def by(a,x):blob[a-BASE]=x&255
 def data(a,x):blob[a-BASE:a-BASE+len(x)]=x
 # Deterministic shader chains, one shared old resource, and a list predecessor.
 n=case.get('stages',1);count=case.get('count',n);handles=[0x23456+512*i for i in range(max(1,count))]
 data(C,bytes(0x1100));w(C+0x1008,OLD if case.get('old') else (OLD+0x80 if case.get('list') else 0))
 for i,handle in enumerate(handles):
  sh=SHADERS+i*0x40;w(HANDLES+i*4,handle);data(sh,bytes(0x1c));w(sh,OLD if case.get('old') else 0);w(sh+4,9);w(sh+8,handle);w(sh+12,0x8b31);w(sh+24,sh+0x40 if i+1<len(handles) else 0)
 w(C+0x808+(handles[0]&511)*4,SHADERS)
 if case.get('old'):
  data(OLD,bytes(0x24));w(OLD,OLD+0x1000);w(OLD+8,OLD+0x2000);w(OLD+16,OLD+0x3000);w(OLD+20,1);w(OLD+24,count+case.get('extraRefs',0));w(OLD+32,OLD+0x80)
  data(OLD+0x3000,bytes(0xe8));w(OLD+0x3030,OLD+0x4000);w(OLD+0x3058,OLD+0x5000);w(OLD+0x30e0,OLD+0x6000);w(OLD+0x80+28,OLD)
 else:w(OLD+0x80+28,0)
 if case.get('middle'):
  w(C+0x1008,OLD+0x80);w(OLD+28,OLD+0x80);w(OLD+32,OLD+0xc0);w(OLD+0x80+32,OLD);w(OLD+0xc0+28,OLD)
 if case.get('lastOld'):
  w(OLD+32,0)
 # DVLB/DVLP-like tables: offsets are relative to their containing header.
 data(B,bytes(0x20000));w(B,0x424c5644);w(B+4,n)
 prog=B+8+n*4;nc=case.get('codeCount',rng.randrange(1,10));no=case.get('operandCount',rng.randrange(1,10))
 w(prog,0x504c5644);w(prog+8,0x100);w(prog+12,nc);w(prog+16,0x200);w(prog+20,no)
 for i in range(nc):w(prog+0x100+i*4,rng.getrandbits(32))
 for i in range(no*2):w(prog+0x200+i*4,rng.getrandbits(32))
 for k in range(n):
  st=B+0x1000+k*0x7000;w(B+8+k*4,st-B);w(st,0x454c5644);h(st+4,1);by(st+6,k%2);by(st+7,rng.randrange(256));w(st+8,rng.randrange(1024));w(st+12,rng.randrange(1024,4096));h(st+16,rng.getrandbits(16));h(st+18,0x7f)
  for j in range(4):by(st+20+j,rng.randrange(256))
  cs=case.get('constants',[(0,0,[1,0,0,0]),(0,15,[0,0,0,0]),(1,0,[rng.getrandbits(32),0,0,0]),(1,3,[rng.getrandbits(32),0,0,0]),(1,4,[9,0,0,0]),(2,7,[rng.getrandbits(24) for _ in range(4)]),(2,95,[rng.getrandbits(24) for _ in range(4)]),(3,0,[0,0,0,0])])
  w(st+24,0x100);w(st+28,len(cs))
  for j,(typ,ix,values) in enumerate(cs):
   h(st+0x100+20*j,typ);h(st+0x102+20*j,ix)
   for l,v in enumerate(values):w(st+0x104+20*j+4*l,v)
  outs=case.get('outputs',[(x,x%7,rng.randrange(16)) for x in range(10)])
  w(st+40,0x800);w(st+44,len(outs))
  for j,(sem,ix,mask) in enumerate(outs):h(st+0x800+8*j,sem);h(st+0x802+8*j,ix);h(st+0x804+8*j,mask)
  vs=case.get('variables',[(0,0,'attr.x'),(1,1,'attr.yz'),(2,3,'mat.xy'),(4,6,'mat3'),(8,11,'mat4'),(12,12,'v.wzyx'),(16,16,'float.x'),(17,18,'vec.xy'),(19,21,'vec.xyz'),(22,25,'vec.xyzw'),(26,26,'offset.y'),(27,28,'offset.zw'),(29,29,'bad.xyq'),(30,30,'trail.'),(31,32,'multi.xy.z'),(112,115,'int.z'),(120,127,'bool.w')])
  w(st+48,0x1000);w(st+52,len(vs));names=bytearray()
  for j,(first,last,name) in enumerate(vs):
   w(st+0x1000+8*j,len(names));h(st+0x1004+8*j,first);h(st+0x1006+8*j,last);names.extend(name.encode()+b'\0')
  w(st+56,0x2000);w(st+60,len(names));data(st+0x2000,names)
 return blob,count
FAIL=[];TOTAL=0;START=time.time();CALL_COUNTS=[]
def run(case):
 global TOTAL
 blob,count=fixture(case);results=[]
 for u,entry,state in [ORIG,CAND]:
  u.mem_write(BASE,bytes(blob));u.mem_write(0xa00000,bytes(0x20000));word(u,0x3e2e48,C);word(u,0x3e2654,0 if case.get('noAllocate') else CALL);word(u,0x3e2658,0 if case.get('noRelease') else CALL+4)
  for j,r in enumerate(REGS):u.reg_write(r,0xa0000000+j)
  u.reg_write(UC_ARM_REG_CPSR,0x10);u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,END)
  u.reg_write(REGS[0],count&0xffffffff);u.reg_write(REGS[1],HANDLES);u.reg_write(REGS[2],case.get('format',0x6000));u.reg_write(REGS[3],B);word(u,SP,case.get('length',0x20000))
  state.clear();state.update(next=0,fail=case.get('fail'),removeCallback=case.get('removeCallback'),calls=[]);fault=None
  try:u.emu_start(entry,END,count=2000000)
  except UcError as ex:fault=[str(ex),hex(u.reg_read(UC_ARM_REG_PC))]
  if not fault and (u.reg_read(UC_ARM_REG_PC)!=END or u.reg_read(UC_ARM_REG_SP)!=SP or [u.reg_read(r) for r in REGS[4:12]]!=[0xa0000000+j for j in range(4,12)]):fault=['completion/ABI',hex(u.reg_read(UC_ARM_REG_PC))]
  results.append(([bytes(u.mem_read(a,n)) for _,a,n in SNAPS],state['calls'][:],fault))
 TOTAL+=1;CALL_COUNTS.append(len(results[0][1]))
 if results[0][2] or results[1][2]:FAIL.append({'case':case,'faults':[r[2] for r in results]});return
 if results[0][1]!=results[1][1]:FAIL.append({'case':case,'calls':[r[1] for r in results]})
 for (label,a,n),x,y in zip(SNAPS,results[0][0],results[1][0]):
  if x!=y:
   ds=[j for j,(p,q) in enumerate(zip(x,y)) if p!=q];off=ds[0]
   FAIL.append({'case':case,'buffer':label,'differences':len(ds),'first':[hex(j) for j in ds[:10]],'original':x[off//4*4:off//4*4+4].hex(),'candidate':y[off//4*4:off//4*4+4].hex()})
cases=[]
for seed in range(8):
 for n in range(4):
  for old in [False,True]:cases.append(dict(seed=seed,stages=n,old=old and n>0,list=seed%2,extraRefs=seed%2))
for seed in range(4):
 for fail in range(14):cases.append(dict(seed=seed,stages=3,fail=fail,old=True))
for count in [-1,0,1,3]:cases.append(dict(seed=10,stages=1,count=count))
for opt in [dict(noAllocate=True),dict(codeCount=0),dict(operandCount=0),dict(constants=[]),dict(outputs=[]),dict(variables=[]),dict(noRelease=True,old=True),dict(format=0,length=-1)]:cases.append(dict(seed=11,stages=1,**opt))
for sem in list(range(12))+[0xffff]:
 for mask in range(16):cases.append(dict(seed=12,stages=1,outputs=[(sem,0,mask)]))
for suffix in ['','x','y','z','w','xy','xyz','xyzw','wzyx','xyq','.xy','xy.z','x.y.z','q.x','xxxxx']:
 for width in range(1,6):cases.append(dict(seed=13,stages=1,variables=[(16,15+width,'f.'+suffix),(0,0,'a.'+suffix)]))
for ix in range(13):cases.append(dict(seed=14,stages=3,removeCallback=ix,old=True))
for extra in [0,1]:
 for middle in [False,True]:
  for last in [False,True]:cases.append(dict(seed=15,stages=3,old=True,extraRefs=extra,middle=middle,lastOld=last))
cases.append(dict(seed=16,stages=1,constants=[(2,4,[0x123456,0xabcdef,0x998877,0x667788])]))
cases.append(dict(seed=17,stages=1,constants=[(0,4,[1,0,0,0])]))
cases.append(dict(seed=18,stages=1,variables=[(16,16,'singleton.z')]))
for case in cases:
 run(case)
 if len(FAIL)>40:break
code_addresses=set(range(0x2478d8,0x247fa8,4))|set(range(0x247fd0,0x24814c,4))|set(range(0x248164,0x248844,4))|set(range(0x248870,0x248a40,4))
result={'retail_executed_instructions':len(COVERED&code_addresses),'retail_code_instructions':len(code_addresses),'retail_uncovered_instructions':[hex(a) for a in sorted(code_addresses-COVERED)],'fixtures':TOTAL,'failures':FAIL,'seconds':time.time()-START,'call_count_total':sum(CALL_COUNTS),'object_sha256':hashlib.sha256((ROOT/'build/eu/obj/lib/CtrSDK/sources/ShaderBinary.o').read_bytes()).hexdigest()}
(OUT/'results.json').write_text(json.dumps(result,indent=2));print(json.dumps(result,indent=2))

```
