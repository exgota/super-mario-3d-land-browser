# Sound start: executable verification recipe

This recipe tests the unchanged source at commit `626fc34b6c73011db1f53ca1309ff82b53fe5fdd` against frozen base `754f99a30a337756df5aa01c28b4ed6a977fecd5`. Locally supply the ignored EU dump/exheader, project environment and installed ARMCC 791/wibo. Never upload the binaries. The original EU SHA and final source/object/replay hashes are in the main report. The recorded emulator is Unicorn 2.1.4, CPU model ARM1176 with VFP enabled. No hardware-cycle result is claimed.

Extract only the named ignored diagnostic scripts:

```sh
. ./development_environment.sh
python - <<'PYTHON'
from pathlib import Path
import re
report=Path('project/dot_reports/sound-start-22b6e8-replay.md').read_text()
blocks=re.findall(r'<!-- file: ([^ ]+) -->\n```python\n(.*?)\n```',report,re.S)
assert {name for name,_ in blocks}=={'strict.py','prepare.py','replay.py','extended.py','snapshot.py','preservation.py'}
out=Path('build/sound-start');out.mkdir(parents=True,exist_ok=True)
for name,code in blocks:(out/name).write_text(code+'\n')
PYTHON
python make.py eu -ca
python build/sound-start/strict.py
python build/sound-start/prepare.py
python build/sound-start/replay.py
python build/sound-start/extended.py
```

Expected strict output is the complete-size mismatch, not acceptance. `strict.py` invokes unchanged check.py and verifies that its exit code is 1, then restores the complete original map. The wrapper exits successfully only when that expected rejection is reproduced. `prepare.py` checks project provenance and constructs a separate diagnostic link of compiler-produced bytes. The replay scripts must report 711 and 151 returning passes respectively, zero failures, and three excluded fault/cycle controls. They deliberately do not count those controls as passing state equivalence. Source hashes, linked entry/length and replay-image digest are pinned; no edited object, invented import address or alternate compiler flags are used.

Preservation reuses the verified coordinator baseline locally. Run `snapshot.py` from the pristine 754 checkout, with an absolute output filename under this lane's ignored build folder, to write baseline-snapshot.json. Run it from this lane's final clean build to write final-snapshot.json. Then run preservation.py. The recorded baseline checkout path is `/workspace/scratch/73cdb2c524af/mario-main754`; when reproducing elsewhere, change only that diagnostic checkout path/prefix. The baseline gate report hash is pinned. The comparison includes all canonical Game/lib C++ objects, allocated bytes, symbols, relocations, ARM attributes and provenance. It normalizes only the known STT_FILE workspace prefix after verifying the rest of each record, and normalizes workspace roots in compiler commands. This is not another 753-function checker run.

The source, input geometry/layout and boundary limitations are described in [the report](sound-start-22b6e8.md). The actual preparation hooks and synthetic virtual callbacks are implemented explicitly below; they must not be mistaken for original callee execution. The write guard permits original-code stores only in the compared fixture and manager regions or the excluded bounded stack mapping. It does not assert equivalence for invalid execution after faults or budget exhaustion.

## strict.py

SHA256 `17ccdbfdd3590effc5bc329e35652f23b54fe348fee9de68ae478c004aa574c3`.

<!-- file: strict.py -->
```python
from pathlib import Path
import sys,subprocess,json,hashlib
sys.path.insert(0,str(Path.cwd()))
from tools.low.checkExactBytes import check_exact_bytes
out=Path('build/sound-start');p=Path('data/ver/eu/map.csv');base=p.read_bytes();line=b'0x0022B6E8,0x0022C020,0x0022C024,          ,U,f,,';assert base.count(line)==1
source=Path('lib/al/src/Audio/SoundArchiveStart.cpp');assert hashlib.sha256(source.read_bytes()).hexdigest()=='50a808b2c293d6d398a4f19a338b9762a715057e014d811db62a45dcb322d775'
try:
 p.write_bytes(base.replace(line,line.replace(b',U,f,,',b',U,f,fn_0022B6E8,')))
 obj=Path('build/eu/obj/lib/al/src/Audio/SoundArchiveStart.o');r=subprocess.run([sys.executable,'tools/check.py','fn_0022B6E8','--object',str(obj)],capture_output=True,text=True);log=r.stdout+r.stderr;(out/'strict-final.log').write_text(log);print(log);assert r.returncode==1
 assert 'U -> M: The complete compiled section, including its literal pool, has a different size from the original interval.' in log
 z=check_exact_bytes('fn_0022B6E8',obj,output_directory=out/'final-exact');(out/'final-exact.json').write_text(json.dumps(z,indent=2));assert not z['exact'] and z['evidence']['compiled_section_size']==2224 and z['evidence']['original_size']==2364
finally:p.write_bytes(base)
assert p.read_bytes()==base
```

## prepare.py

SHA256 `59d91ce1d28f8af707fb838846bd051dd7d395c2a85e14c3019a08bb28f96175`.

<!-- file: prepare.py -->
```python
from pathlib import Path
import sys,json,subprocess,hashlib,os
sys.path.insert(0,str(Path.cwd()))
from tools.low.checkExactBytes import _read_map,_isolate_function
from tools.low.buildProvenance import verify_build_output
from elftools.elf.elffile import ELFFile
out=Path('build/sound-start');obj=Path('build/eu/obj/lib/al/src/Audio/SoundArchiveStart.o')
p=verify_build_output(obj);assert p['compiler']=='4.1/791'
rows=_read_map(Path('data/ver/eu/map.csv'))
sec,raw,imports=_isolate_function(obj,'fn_0022B6E8',rows,out/'candidate.o')
(out/'imports.json').write_text(json.dumps(imports,indent=2))
(out/'original.sym').write_text('#<SYMDEFS>#\n'+''.join('0x%08X %s %s\n'%(i['address'],i['kind'],i['symbol']) for i in imports))
(out/'candidate.sct').write_text('REPLAY 0x0022B6E8 { CODE 0x0022B6E8 { candidate.o (i.fn_0022B6E8, +FIRST) } }\n')
cmd=['data/compilers/wibo','data/compilers/4.1/791/bin/armlink.exe','--cpu=MPCore','--fpu=VFPv2','--arm_only','--no_exceptions','--inline','--datacompressor=off','--no_debug','--no_scanlib','--entry=fn_0022B6E8','--keep=fn_0022B6E8','--scatter='+str(out/'candidate.sct'),'--output='+str(out/'candidate.axf'),str(out/'candidate.o'),str(out/'original.sym')]
r=subprocess.run(cmd,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
with (out/'candidate.axf').open('rb') as f:
 elf=ELFFile(f);ss=[s for s in elf.iter_sections() if s['sh_flags']&2 and s['sh_size']];assert len(ss)==1;assert ss[0]['sh_addr']==0x22b6e8
 linked=ss[0].data();(out/'candidate.bin').write_bytes(linked)
assert len(linked)==len(raw)==2224
assert hashlib.sha256(linked).hexdigest()=='95baab7363a066ac708f3af9bb0d0c7b025293fc85635bbd2431144772abab93'
print(json.dumps({'source_sha':hashlib.sha256(Path(p['source']).read_bytes()).hexdigest(),'object_sha':hashlib.sha256(obj.read_bytes()).hexdigest(),'replay_bytes':len(linked),'replay_sha':hashlib.sha256(linked).hexdigest(),'imports':len(imports)}))
```

## replay.py

SHA256 `2c152b7aa2d0791ac76ddbd7a13c9eb673f9c6568fe5aad78430a88b2ae586ad`.

<!-- file: replay.py -->
```python
import struct,json,hashlib,random,sys
from pathlib import Path
from collections import Counter
from unicorn import Uc,UcError,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE,UC_HOOK_MEM_WRITE
from unicorn.arm_const import *
ROOT=0x22b6e8;STOP=0x700000;INIT=STOP+4;RELEASE=STOP+8;ENGINE=STOP+12;REORDER=STOP+16;PRIORITY=STOP+20;ALLOC=STOP+24
MEM=0x600000;PLAYER=MEM;ARCHIVE=MEM+0x200;READER=MEM+0x300;INFO=MEM+0x400;TABLE=MEM+0x500;RAW=MEM+0x600;SPEC=MEM+0x700;BANKS=MEM+0x780
VT=MEM+0x800;AVT=MEM+0x900;PROVIDER=MEM+0xa00;AMBIENT=MEM+0xb00;USERDATA=MEM+0xc00;AMBBUFFER=MEM+0xd00
SLOTS=MEM+0x1000;ACTOR=MEM+0x2000;HANDLE=MEM+0x2100;OPTIONS=MEM+0x2200;OLDHANDLE=MEM+0x2300;CMD=MEM+0x40000;MANAGER=0x4258c8;STACK=0x820000
REGS=[UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3]
def pack(*v):return struct.pack('<'+'I'*len(v),*[x&0xffffffff for x in v])
def run(binary,candidate,c):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176);u.reg_write(UC_ARM_REG_C1_C0_2,0xf00000);u.reg_write(UC_ARM_REG_FPEXC,0x40000000);u.reg_write(UC_ARM_REG_FPSCR,c.get('fpscr',0))
 u.mem_map(0x100000,0x400000);u.mem_write(0x100000,binary)
 if candidate:u.mem_write(ROOT,candidate)
 u.mem_map(MEM,0x60000);u.mem_map(STOP,0x1000);u.mem_map(0x800000,0x40000)
 def w(a,*v):u.mem_write(a,pack(*v))
 def rd(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def byt(a,v):u.mem_write(a,bytes([v&255]))
 def half(a,v):u.mem_write(a,struct.pack('<H',v&65535))
 def args():return [u.reg_read(r) for r in REGS]
 def ret(v=0):u.reg_write(UC_ARM_REG_R0,v&0xffffffff);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 owners={};soundpool={};all_sounds=[]
 def listinit(a,items):
  w(a,len(items),items[0] if items else a+4,items[-1] if items else a+4)
  for i,n in enumerate(items):w(n,items[i+1] if i+1<len(items) else a+4,items[i-1] if i else a+4);owners[n]=a
 def unlink(n):
  if rd(n)==0:return
  a=owners.pop(n);nxt,prev=rd(n),rd(n+4);w(nxt+4,prev);w(prev,nxt);w(a,rd(a)-1);w(n,0,0)
 def append(a,n):
  prev=rd(a+8);w(n,a+4,prev);w(prev,n);w(a+8,n);w(a,rd(a)+1);owners[n]=a
 w(PLAYER+4,0 if c.get('no_archive') else ARCHIVE);w(PLAYER+0x24,SLOTS);w(ARCHIVE+4,0 if c.get('not_ready') else READER);w(READER+0x3c,INFO)
 w(INFO+4,TABLE-INFO);w(TABLE,0 if c.get('missing') else 1,0,RAW-TABLE)
 kind=c.get('kind',1);rawtype={0:0,1:0x2203,2:0x2201,3:0x2202}.get(kind,0)
 w(RAW,0x123,c.get('player_id',0));byt(RAW+8,c.get('volume',127));half(RAW+0xc,rawtype);w(RAW+0x10,SPEC-RAW)
 # Optional bit 1: two byte parameters; bit 2: priority and actor player; bit 17: boolean.
 flags=c.get('info_flags', (1<<1)|(1<<2)|(1<<17));w(RAW+0x14,flags)
 vals={1:c.get('p20',3)|(c.get('p21',4)<<8),2:(c.get('priority',64)&255)|((c.get('actor_id',0)&255)<<8),17:c.get('p22',1)}
 for j,k in enumerate(sorted(vals)):
  if flags&(1<<k):w(RAW+0x18+4*sum(bool(flags&(1<<x)) for x in range(k)),vals[k])
 if kind==1:
  w(SPEC,0,BANKS-SPEC,0x789,3,0x12,0x120);w(BANKS,c.get('bank_count',4),10,20,30,40)
 elif kind==2:half(SPEC,c.get('stream0',2));half(SPEC+2,c.get('stream2',5))
 else:w(SPEC,0x456,0x789,1,0x120)
 w(VT+0xc,INIT,RELEASE);w(VT+0x20,ENGINE,REORDER)
 w(AVT+8,ALLOC,PRIORITY);w(PROVIDER,AVT);w(AMBIENT,PROVIDER if c.get('ambient_provider',True) else 0,0x4141,PROVIDER,USERDATA,20);u.mem_write(USERDATA,b'ambient-state-1234567')
 # Every type has its own manager pool, one reusable entry, and sorted active entries.
 for k,off in [(1,0x28),(2,0x58),(3,0x40)]:
  pool=PLAYER+off;base=MEM+0x10000+(k-1)*0xc000;sounds=[base+i*0x3000 for i in range(4)];all_sounds+=sounds
  for i,s in enumerate(sounds):
   soundpool[s]=pool;w(s,VT);byt(s+0x98,c.get('active_priority',40)+i*10);w(s+0x50,c.get('active_offset',0));w(s+0x9c,0x1001000+i)
  n=c.get('active',0) if k==kind else 0;free=c.get('free',1) if k==kind else 1
  listinit(pool,[s+0xd4 for s in sounds[:n]]);listinit(pool+12,[sounds[3]+0xd4] if free else [])
  if c.get('cycle') and k==kind:
   n0=sounds[0]+0xd4;w(n0,n0,n0);w(pool,1,n0,n0)
  if c.get('null_node') and k==kind:w(pool+12,1,0,0)
 for i in range(3):
  a=SLOTS+i*0x48;listinit(a,[]);listinit(a+12,[]);listinit(a+24,[]);w(a+0x24,c.get('player_limit',4))
 for i in range(4):listinit(ACTOR+8+i*16,[]);w(ACTOR+8+i*16+12,c.get('actor_limit',4))
 # Optional existing handle, including both matching and nonmatching reverse links.
 old=MEM+0x38000;w(old,VT);w(HANDLE,old if c.get('attached') else 0);w(old+8,HANDLE if c.get('attached') else 0);w(old+12,HANDLE if c.get('dual_handle') else OLDHANDLE);w(OLDHANDLE,old)
 opt=c.get('options',0);w(OPTIONS,opt);byt(OPTIONS+4,c.get('offset_type',0));w(OPTIONS+8,c.get('offset',321),c.get('override_player',0),c.get('override_priority',80),c.get('override_actor',0));w(OPTIONS+0x18,0,0,*c.get('banks',[0xffffffff,99,0xffffffff,101]))
 w(0x3ef848,1);u.mem_write(MANAGER,bytes(0x200));w(MANAGER+0x178,CMD,0x4000,0,0)
 actor=ACTOR if c.get('actor') else 0;amb=AMBIENT if c.get('ambient') else 0;sid=c.get('id',0x01000000)
 for r,v in zip(REGS,[PLAYER,HANDLE,sid,amb]):u.reg_write(r,v)
 if c.get('alias_handle_options'):
  u.reg_write(UC_ARM_REG_R1,OPTIONS+0x18)
 if c.get('alias_handle_actor'):
  w(ACTOR+0x48,0);u.reg_write(UC_ARM_REG_R1,ACTOR+0x48)
 u.reg_write(UC_ARM_REG_SP,STACK);w(STACK,actor,c.get('hold',0),OPTIONS if c.get('options_present',bool(opt)) else 0)
 u.reg_write(UC_ARM_REG_LR,STOP)
 saved={r:0xa0000000+r for r in [UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11]+list(range(UC_ARM_REG_D8,UC_ARM_REG_D15+1))}
 for r,v in saved.items():u.reg_write(r,v)
 trace=[];visited=set();allvisited=set();steps=0;maxstack=STACK;lookups=0
 def info_values(p):return list(struct.unpack('<5I',u.mem_read(p,20)))+list(u.mem_read(p+20,3))
 def hook(u,a,size,data):
  nonlocal steps,maxstack,lookups
  steps+=1;allvisited.add(a);maxstack=min(maxstack,u.reg_read(UC_ARM_REG_SP))
  if ROOT<=a<0x22c020:visited.add(a)
  r0,r1,r2,r3=args()
  if a==STOP:u.emu_stop();return
  if a==0x2323e8:owners[r2]=r0
  if a==0x22c024:
   # The original erase executes; update only bookkeeping for the later virtual fixture.
   owners.pop(r1,None)
  if a==0x232390:owners.pop(r1,None)
  if a==0x214a2c:
   lookups+=1
   if c.get('detail_missing') and lookups>=4:w(TABLE,0)
   if c.get('second_type_invalid') and lookups>=3:half(RAW+0xc,0)
  if a in (INIT,RELEASE,ENGINE,REORDER,PRIORITY,ALLOC):
   if a==INIT:
    trace.append(['initialize',r0]);u.mem_write(r0+4,bytes(0xd0));
    if c.get('admission_failure'):w(SLOTS+0x24,0)
   elif a==RELEASE:
    trace.append(['release',r0])
    for off in [0xd4,0xdc,0xe4,0xec]:unlink(r0+off)
    if r0 in soundpool:append(soundpool[r0]+12,r0+0xd4)
    w(r0+0x10,0);w(r0+0x18,0)
   elif a==ENGINE:ret(r0+0xf4);return
   elif a==REORDER:trace.append(['reorder',r0])
   elif a==PRIORITY:trace.append(['ambient_priority',r0,r1,r2]);ret(c.get('ambient_priority',0));return
   elif a==ALLOC:trace.append(['ambient_allocate',r0,r1]);ret(AMBBUFFER if c.get('ambient_alloc') else 0);return
   ret();return
  if a in (0x2bcaf8,0x2bc8c0):
   sp=u.reg_read(UC_ARM_REG_SP);extra=[rd(sp),rd(sp+4)]
   if a==0x2bcaf8:detail=list(struct.unpack('<6I',u.mem_read(r3,24)))+list(u.mem_read(r3+24,2));extra+=[rd(sp+8)]
   else:detail=list(struct.unpack('<2I',u.mem_read(r3,8)))+list(u.mem_read(r3+8,2))
   trace.append(['prepare',a,r0,r1,info_values(r2),detail,extra]);w(r1+0xa4,0xface1234)
   ret(c.get('prepare_status',0));return
  if a==0x2c0e28 and c.get('actor_admission_failure'):w(r0+12,0)
 def write_guard(u,access,address,size,value,data):
  assert any(low<=address and address+size<=high for low,high in [(MEM,MEM+0x60000),(MANAGER,MANAGER+0x200),(0x800000,0x840000)]),('unobserved write',hex(address),size)
 error=None
 try:u.hook_add(UC_HOOK_CODE,hook);u.hook_add(UC_HOOK_MEM_WRITE,write_guard);u.emu_start(ROOT,STOP,count=c.get('limit',20000))
 except Exception as e:error=str(e)
 if not error and u.reg_read(UC_ARM_REG_PC)!=STOP:error='instruction bound'
 result={'return':u.reg_read(UC_ARM_REG_R0),'trace':trace,'memory':bytes(u.mem_read(MEM,0x60000)).hex(),'manager':bytes(u.mem_read(MANAGER,0x200)).hex(),'fpscr':u.reg_read(UC_ARM_REG_FPSCR),'restored_sp':u.reg_read(UC_ARM_REG_SP)==STACK,'preserved_registers':all(u.reg_read(r)==v for r,v in saved.items())}
 if error:result={'error':error,'pc':hex(u.reg_read(UC_ARM_REG_PC)),'trace':trace}
 return result,{'steps':steps,'stack_bytes':STACK-maxstack,'visited':sorted(visited),'allvisited':sorted(allvisited)}
def fixtures():
 out=[{'no_archive':1},{'not_ready':1},{'missing':1},{'id':0x02000000},{'id':0x01000001},{'kind':0}]
 for k in (1,2,3):
  for extra in [{},{'attached':1,'dual_handle':1},{'actor':1},{'actor':1,'actor_id':4},{'player_limit':0},{'actor':1,'actor_limit':0},{'free':0},{'free':0,'active':1,'active_priority':100},{'free':0,'active':2,'active_priority':10},{'active':3},{'ambient':1},{'ambient':1,'ambient_alloc':1},{'ambient':1,'ambient_provider':0},{'detail_missing':1},{'second_type_invalid':1},{'admission_failure':1},{'actor':1,'actor_admission_failure':1}]:out.append({'kind':k,**extra})
  for offset_type in (0,1,2,3,255):
   for hold in (0,1):out.append({'kind':k,'options':31,'actor':1,'ambient':1,'ambient_priority':-17,'offset_type':offset_type,'hold':hold,'override_player':0xab000001,'override_actor':3})
  if k!=2:
   for status in (1,2,3,4,5,7,8,9,15,255,256,0x1234):out.append({'kind':k,'prepare_status':status})
 rng=random.Random(0x22b6e8)
 for i in range(600):
  out.append({'kind':rng.choice([1,2,3]),'priority':rng.randrange(256),'volume':rng.choice([0,1,63,127,128,255]),'options':rng.randrange(32),'options_present':rng.choice([0,1]),'override_priority':rng.choice([0,1,127,128,255,-1,-255,0x7fffffff,-0x80000000]),'override_player':rng.choice([0,1,2,0xff000002]),'override_actor':rng.choice([0,1,3,4,0xffffffff]),'actor_id':rng.choice([0,1,2,3,4,255]),'actor':rng.randrange(2),'ambient':rng.randrange(2),'ambient_provider':rng.randrange(2),'ambient_alloc':rng.randrange(2),'ambient_priority':rng.choice([-1000,-128,-1,0,1,128,1000,0x7fffffff,-0x80000000]),'free':rng.randrange(2),'active':rng.randrange(4),'active_priority':rng.randrange(180),'active_offset':rng.choice([-200,0,100]),'hold':rng.randrange(2),'offset_type':rng.choice([0,1,2,3,255]),'offset':rng.getrandbits(32),'attached':rng.randrange(2),'prepare_status':rng.choice([0,0,0,1,255,256]),'bank_count':rng.randrange(6),'fpscr':rng.choice([0,0x01000000,0x02000000])})
 return out
if __name__=='__main__':
 b=Path('data/ver/eu/code.bin').read_bytes();assert hashlib.sha256(b).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64';candidate=Path('build/sound-start/candidate.bin').read_bytes();cases=fixtures();fail=[];errors=[];counts=Counter();visited=set();allvisited=set();steps=[0,0];maxsteps=[0,0];maxstack=[0,0]
 for i,c in enumerate(cases):
  x,a=run(b,None,c);y,z=run(b,candidate,c);visited.update(a['visited']);allvisited.update(a['allvisited'])
  for j,t in enumerate([a,z]):steps[j]+=t['steps'];maxsteps[j]=max(maxsteps[j],t['steps']);maxstack[j]=max(maxstack[j],t['stack_bytes'])
  if 'error' in x or 'error' in y:errors.append(i)
  if x!=y or 'error' in x or 'error' in y:
   fail.append(i);Path('build/sound-start/failure.json').write_text(json.dumps({'index':i,'case':c,'original':x,'candidate':y},indent=2));print('FAIL',i,c,[k for k in x if x.get(k)!=y.get(k)],x.get('error'),y.get('error'));break
  assert x['restored_sp'] and x['preserved_registers'];counts[x['return']]+=1
 result={'requested':len(cases),'completed':i+1,'passed':i+1-len(fail),'failures':fail,'errors':errors,'returns':dict(counts),'total_instructions':steps,'max_instructions':maxsteps,'max_stack':maxstack,'original_root_pcs':sorted(visited),'all_original_pcs':sorted(allvisited)}
 Path('build/sound-start/replay.json').write_text(json.dumps(result,indent=2));print({k:v for k,v in result.items() if not k.endswith('_pcs')});sys.exit(bool(fail))
```

## extended.py

SHA256 `507e6786528aedf8735a06f3f26603ecf92ec3a983bd67841088dab112154c15`.

<!-- file: extended.py -->
```python
from replay import *
b=Path('data/ver/eu/code.bin').read_bytes();candidate=Path('build/sound-start/candidate.bin').read_bytes();cases=[]
for kind in (1,2,3):
 for flags in (0,1,2,4,8,16,31):
  for alias in ('alias_handle_options','alias_handle_actor'):
   cases.append({'kind':kind,'options':flags,'options_present':1,'actor':1,'override_actor':0,alias:1})
for volume in (0,1,2,3,63,126,127,128,254,255):
 for fpscr in (0,0x00400000,0x00800000,0x00c00000,0x01000000,0x02000000,0x0300009f):
  cases.append({'kind':2,'volume':volume,'fpscr':fpscr})
for kind in (1,2,3):
 for flags in (0,2,4,1<<17,6,(1<<17)|2):
  cases.append({'kind':kind,'info_flags':flags,'actor':1,'options':31,'offset_type':2})
for kind in (1,2,3):
 for hold in (2,127,128,255,256,0xffffffff,0x80000000):
  cases.append({'kind':kind,'hold':hold,'options':31,'override_priority':0,'ambient':1,'ambient_priority':-1})
returning=[];fail=[];excluded=[];steps=[0,0];maxstack=[0,0];pcs=set()
for i,c in enumerate(cases):
 a,x=run(b,None,c);z,y=run(b,candidate,c);pcs.update(x['visited'])
 for j,v in enumerate([x,y]):steps[j]+=v['steps'];maxstack[j]=max(maxstack[j],v['stack_bytes'])
 if a!=z or 'error'in a or 'error'in z:
  fail.append(i);Path('build/sound-start/extended-failure.json').write_text(json.dumps({'case':c,'original':a,'candidate':z},indent=2));break
 returning.append(a['return'])
controls=[{'kind':1,'active':1,'active_priority':1,'cycle':1,'limit':3000},{'kind':2,'null_node':1},{'kind':3,'null_node':1}]
for c in controls:
 a,x=run(b,None,c);z,y=run(b,candidate,c);excluded.append({'case':c,'original':a,'candidate':z,'original_instructions':x['steps'],'candidate_instructions':y['steps']})
 assert 'error'in a and 'error'in z
r={'requested':len(cases),'passed':len(returning),'failures':fail,'returns':dict(Counter(returning)),'total_instructions':steps,'max_stack':maxstack,'excluded_controls':excluded};Path('build/sound-start/extended.json').write_text(json.dumps(r,indent=2));print({k:v for k,v in r.items() if k!='excluded_controls'});print('Excluded controls',[(v['original']['error'],v['candidate']['error']) for v in excluded]);sys.exit(bool(fail))
```

## snapshot.py

SHA256 `2d17d888a9502300154011a90cf4e16e7dd4fb11bdc8a88d550c23225d4d8e2a`.

<!-- file: snapshot.py -->
```python
from pathlib import Path
import sys,json,hashlib,io
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection
repo=Path.cwd().resolve();sys.path.insert(0,str(repo))
from tools.low.buildProvenance import verify_build_output
records={};all_inputs={}
for p in sorted((repo/'build/eu/obj').rglob('*.o')):
 rel=p.relative_to(repo).as_posix()
 if not rel.startswith(('build/eu/obj/Game/','build/eu/obj/lib/')):continue
 prov=verify_build_output(p)
 original=json.loads(p.with_suffix('.provenance.json').read_text());assert original['language']=='C++'
 all_inputs.update(original['inputs'])
 elf=ELFFile(io.BytesIO(p.read_bytes()));ss=list(elf.iter_sections());table=elf.get_section_by_name('.symtab')
 def sym(x):
  ix=x['st_shndx'];return [x.name,x['st_value'],x['st_size'],dict(x['st_info']),dict(x['st_other']),ss[ix].name if isinstance(ix,int) else ix]
 alloc=[[s.name,s['sh_type'],s['sh_flags'],s['sh_addralign'],s['sh_size'],hashlib.sha256(s.data()).hexdigest()] for s in ss if s['sh_flags']&2]
 symbols=[sym(s) for s in table.iter_symbols()]
 relocs=[]
 for s in ss:
  if isinstance(s,RelocationSection):
   relocs.append([s.name,ss[s['sh_info']].name,[[r['r_offset'],r['r_info_type'],sym(table.get_symbol(r['r_info_sym']))] for r in s.iter_relocations()]])
 attrs=[s.data().hex() for s in ss if s.name=='.ARM.attributes']
 provenance={k:v for k,v in original.items() if k not in ['command','object_sha256']};provenance['command']=[s.replace(str(repo),'<REPO>') for s in original['command']]
 records[rel]={'allocated':alloc,'symbols':symbols,'relocations':relocs,'arm_attributes':attrs,'provenance':provenance}
result={'objects':records,'inputs':all_inputs,'verified_objects':len(records)}
Path(sys.argv[1]).write_text(json.dumps(result,sort_keys=True,indent=2));print(json.dumps({'objects':len(records),'inputs':len(all_inputs)}))
```

## preservation.py

SHA256 `027d5bcd088b798aa93e067ba71ea9db986dc024efde5b8162e51df0317f9d40`.

<!-- file: preservation.py -->
```python
import json,hashlib,subprocess
from pathlib import Path
out=Path('build/sound-start');a=json.loads((out/'baseline-snapshot.json').read_text());b=json.loads((out/'final-snapshot.json').read_text());differences=[];normalized_file_paths=0
for p,v in a['objects'].items():
 for left,right in zip(v['symbols'],b['objects'][p]['symbols']):
  if left!=right and left[3]['type']==right[3]['type']=='STT_FILE':
   assert left[1:]==right[1:]
   assert left[0].replace('/workspace/scratch/73cdb2c524af/mario-main754/','<REPO>/')==right[0].replace(str(Path.cwd())+'/','<REPO>/')
   right[0]=left[0];normalized_file_paths+=1
 if b['objects'].get(p)!=v:differences.append({'object':p,'different_fields':[k for k,x in v.items() if b['objects'].get(p,{}).get(k)!=x]})
input_diff=[p for p,v in a['inputs'].items() if b['inputs'].get(p)!=v];added=sorted(set(b['objects'])-set(a['objects']));report=Path('/workspace/scratch/73cdb2c524af/mario-main754/build/dot-baseline-754/report.json');digest=hashlib.sha256(report.read_bytes()).hexdigest();assert digest=='c208f39fadf49e66c5c98c97b445881ba2e51ade07b5724610cf0309175ec028';r=json.loads(report.read_text());assert r['clean_build_returncode']==0 and len(r['prior_checks'])==753 and all(x['returncode']==0 for x in r['prior_checks'])
tracked=subprocess.check_output(['git','ls-tree','-r','754f99a','--','Game','lib','tools','data/config.json','data/ver/eu/map.csv'],text=True).splitlines();changed=[];gitlinks=[];blobs=0
for line in tracked:
 meta,p=line.split('\t');mode,kind,digest=meta.split()
 if kind=='commit':
  current=subprocess.check_output(['git','ls-tree','HEAD','--',p],text=True).split()[2];assert current==digest;gitlinks.append(p);continue
 assert kind=='blob';blobs+=1;base=subprocess.check_output(['git','show','754f99a:'+p]);now=Path(p).read_bytes()
 if base!=now:changed.append(p)
s={'base':'754f99a30a337756df5aa01c28b4ed6a977fecd5','baseline_report_sha256':hashlib.sha256(report.read_bytes()).hexdigest(),'baseline_roots':733,'baseline_actual_definitions':len(r['prior_checks']),'baseline_clean_build_returncode':r['clean_build_returncode'],'baseline_elapsed_seconds':r['elapsed_seconds'],'canonical_objects_compared':len(a['objects']),'normalized_STT_FILE_workspace_prefixes':normalized_file_paths,'input_hashes_compared':len(a['inputs']),'tracked_blob_files_compared':blobs,'unchanged_gitlinks':gitlinks,'object_differences':differences,'input_differences':input_diff,'tracked_differences':changed,'added_objects':added,'second_753_check_run':False};(out/'preservation.json').write_text(json.dumps(s,indent=2));print(json.dumps(s,indent=2));assert not differences and not input_diff and not changed;assert len(added)==1
```
