# Root 00269A60: executable verification recipe

This is a complete reproduction recipe for the four-path proposal on top of main `3de056e`. Use source commit `63c7cf6` or the final report descendant with identical source/header bytes. The original EU executable, exheader, configured compilers and dependencies remain locally supplied/ignored. Python needs Unicorn 2.1.4 and pyelftools, in addition to the project's documented environment.

The canonical checker remains unchanged. It rejects the root because the independently evidenced static filter has no canonical map row. The diagnostic link below resolves that one data identity independently, never writes it to map.csv, and never supplies it to the checker. The candidate object remains untouched compiler output. This is semantic replay of a NonMatching proposal, with zero exact credit.

## Run

The following extraction command writes only the three explicitly named ignored scripts. Run from the repository root:

```sh
. ./development_environment.sh
export DEVKITARM=/usr
python - <<'PY'
from pathlib import Path
import re
report=Path('project/dot_reports/root-269a60-replay.md').read_text()
blocks=re.findall(r'<!-- file: ([^ ]+) -->\n```python\n(.*?)\n```',report,re.S)
assert {name for name,_ in blocks}=={'prepare.py','replay.py','extended.py'}
output=Path('build/dot_269a60');output.mkdir(parents=True,exist_ok=True)
for name,code in blocks:(output/name).write_text(code+'\n')
PY
python make.py eu -ca
python build/dot_269a60/prepare.py
python build/dot_269a60/replay.py --output build/dot_269a60/replay_final.json
python - <<'PY'
import json,subprocess,sys
from pathlib import Path
result=subprocess.run([sys.executable,'build/dot_269a60/extended.py'])
# The extended diagnostic exits 1 because it reports seven bounded runs.
assert result.returncode==1
ordinary=json.loads(Path('build/dot_269a60/replay_final.json').read_text())
assert ordinary['cases']==ordinary['passed']==260 and ordinary['failures']==[]
extended=json.loads(Path('build/dot_269a60/extended_final.json').read_text())
assert extended['cases']==2044 and extended['passed']==2037 and extended['mismatches']==[]
assert [x['index'] for x in extended['faults']]==[49,119,189,259,329,399,469]
assert all(x['original_error']==x['candidate_error']=='query bound' for x in extended['faults'])
print('2297 returning whole-root pairs agree; seven paired query-bound exhaustions are excluded.')
PY
```

Expected canonical output is the source-closure rejection recorded in the main report, not U->O or U->M. The wrapper's assertions keep every unexpected mismatch, exception, missing result, changed input/hash, or new bound failure fatal. The seven deliberately retained negative-infinity displacement probes remain outside equivalence evidence. Their paired bound exhaustion is not a termination proof.

The driver's 196608 fixture bytes include inputs, output capacity, result pool and contact records. It separately compares the complete 1048-byte filter, return, trace, full FPSCR, restored stack and preserved registers. The original filter initializer runs before every pair. The scene-provider and optional delegated-filter hooks are the only modeled game boundaries. Synthetic hook addresses are emulator fixtures, not source imports or claimed retail symbols. All other reached original callees execute from the owner's verified executable.

The one-million-instruction default and 1024-query limits fail when reached. Large ring-saturation controls explicitly use five million instructions. The original root's internal literal pools are not counted as executable coverage. The source's ordinary helpers are completely inlined; the diagnostic image has one 3288-byte root section.

## Canonical preparation and separate diagnostic link

<!-- file: prepare.py -->
```python
from pathlib import Path
import json,subprocess,sys,hashlib
sys.path.insert(0,str(Path.cwd()))
from tools.low.checkExactBytes import _read_map,_isolate_function
from tools.low.buildProvenance import verify_build_output
from elftools.elf.elffile import ELFFile
out=Path('build/dot_269a60');obj=Path('build/eu/obj/lib/al/src/Collision/SweptSphere269A60.o')
expected={
 'data/ver/eu/code.bin':'e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64',
 'data/compilers/4.1/791/bin/armcc.exe':'d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d',
 'lib/al/src/Collision/SweptSphere269A60.cpp':'2a44755684f4cf375ae7d56e4e5098f33fd2f01383cf7db1528e199c7d6c8a2e',
 'lib/al/include/Collision/SweptSphere269A60.h':'f6d33ffd15db1e4bf2479973e938fec340170b0cd2f4a00e70cd7fe6ef1e7983',
}
for name,digest in expected.items():assert hashlib.sha256(Path(name).read_bytes()).hexdigest()==digest,name
p=Path('data/ver/eu/map.csv');original=p.read_bytes()
assert hashlib.sha256(original).hexdigest()=='514c3ec6f2343e8a40fc73996df78fc95b2e33f4897fbdda6bb0311df6cf40fa'
line=next(x for x in original.splitlines() if x.startswith(b'0x00269A60,'))
assert line==b'0x00269A60,0x00269E48,0x0026A7E8,          ,U,f,,'
try:
 p.write_bytes(original.replace(line,line.replace(b',U,f,,',b',U,f,fn_00269A60,')))
 result=subprocess.run([sys.executable,'tools/check.py','fn_00269A60','--object',str(obj)],capture_output=True,text=True)
 output=result.stdout+result.stderr;print(output);(out/'strict_final.txt').write_text(output)
 assert result.returncode==1
 assert 'Source closure rejected: An unresolved source helper is referenced by a non-branch relocation.' in output
finally:p.write_bytes(original)
assert p.read_bytes()==original
assert hashlib.sha256(obj.read_bytes()).hexdigest()=='ca416282ec7575abdb1ba3fa09661955f0bfe7cbbfc3544e1c1e8212e8dcd1b4'

print(verify_build_output(obj))
rows=_read_map(Path('data/ver/eu/map.csv'))
# Diagnostic-only binding independently proven by initializer00387DEC and callback00361114.
# Never supplied to check.py, never written to canonical map, and no matching credit.
rows.append({'Start':0x424b14,'End':0x424f2c,'Type':'db','Symbol':'dat_00424B14','SectionName':''})
sec,compiled,imports=_isolate_function(obj,'fn_00269A60',rows,out/'candidate.o')
assert sec=='i.fn_00269A60' and len(compiled)==3288
print(sec,len(compiled));(out/'imports.json').write_text(json.dumps(imports,indent=2))
(out/'original.sym').write_text('#<SYMDEFS>#\n'+''.join('0x%08X %s %s\n'%(v['address'],v['kind'],v['symbol']) for v in imports))
(out/'candidate.sct').write_text('REPLAY 0x00269A60 { CODE 0x00269A60 { candidate.o (i.fn_00269A60, +FIRST) } }\n')
cmd=['data/compilers/wibo','data/compilers/4.1/791/bin/armlink.exe','--cpu=MPCore','--fpu=VFPv2','--arm_only','--no_exceptions','--inline','--datacompressor=off','--no_debug','--no_scanlib','--entry=fn_00269A60','--keep=fn_00269A60','--scatter='+str(out/'candidate.sct'),'--output='+str(out/'candidate.axf'),str(out/'candidate.o'),str(out/'original.sym')]
r=subprocess.run(cmd,capture_output=True,text=True);print(r.stdout+r.stderr);assert r.returncode==0
with (out/'candidate.axf').open('rb') as f:
 s=ELFFile(f).get_section_by_name('CODE');(out/'candidate.bin').write_bytes(s.data());print(s['sh_size'],hashlib.sha256(s.data()).hexdigest())
assert hashlib.sha256((out/'candidate.bin').read_bytes()).hexdigest()=='c25247b5afd55b2fe4b50d5cc54defe5c8671dc581c2afbe45bb64feb22089ac'
print('Strict rejection reproduced, canonical map restored, separate diagnostic image verified.')
```

## Whole-root driver and initial fixtures

<!-- file: replay.py -->
```python
import argparse,struct,json,hashlib,random
from pathlib import Path
from unicorn import Uc,UcError,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE
from unicorn.arm_const import *
ROOT=0x269a60; FILTER=0x424b14; STOP=0x700000; WORLD=STOP+4; RESUME=STOP+8; DELEGATE=STOP+12
MEM=0x600000; START=MEM; DIRECTION=MEM+0x20; OUTPUT=MEM+0x1000; DIRECTOR=MEM+0x100; POOL=MEM+0x200; SLOTS=MEM+0x300
KIT=MEM+0x500; SYSTEM=MEM+0x600; PROVIDER=MEM+0x800; VT=MEM+0x900; DELOBJ=MEM+0xa00; CONTACTS=MEM+0x10000; STACK=0x820000
REGS=[UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3]
def pack(*x):return struct.pack('<'+'I'*len(x),*[v&0xffffffff for v in x])
def flt(*x):return struct.pack('<'+'f'*len(x),*x)
def run(binary,candidate,case):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176);u.reg_write(UC_ARM_REG_C1_C0_2,0xf00000);u.reg_write(UC_ARM_REG_FPEXC,0x40000000);u.reg_write(UC_ARM_REG_FPSCR,case.get('fpscr',0))
 u.mem_map(0x100000,0x400000);u.mem_write(0x100000,binary)
 if candidate is not None:u.mem_write(ROOT,candidate)
 u.mem_map(MEM,0x30000);u.mem_map(STOP,0x1000);u.mem_map(0x800000,0x40000)
 def w(a,*x):u.mem_write(a,pack(*x))
 def word(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def setargs(*x):
  for r,v in zip(REGS,x):u.reg_write(r,v)
 singleton=struct.unpack_from('<I',binary,0x28e694-0x100000)[0]
 w(singleton,SYSTEM);w(SYSTEM+0x54,KIT);w(KIT+0x28,DIRECTOR)
 w(DIRECTOR,0,PROVIDER,0,0,0,POOL);w(POOL,0,64,SLOTS,0);w(PROVIDER,VT);w(VT+0x14,WORLD);w(DELOBJ,VT+0x40);w(VT+0x40,DELEGATE)
 # Execute the independently identified original initializer before root entry.
 u.reg_write(UC_ARM_REG_SP,STACK);u.reg_write(UC_ARM_REG_LR,STOP);u.emu_start(0x387dec,STOP,count=100)
 assert word(FILTER)==0x3a8fa0 and word(FILTER+4)==FILTER+0x14 and word(FILTER+8)==256
 u.mem_write(START,flt(*case['start']));u.mem_write(DIRECTION,flt(*case['direction']));u.mem_write(OUTPUT,bytes([0xa5])*0x6000)
 for label,base in [('start',START),('direction',DIRECTION)]:
  for i,value in case.get(label+'_bits',{}).items():w(base+int(i)*4,value)
 setargs(OUTPUT,case['capacity'],START,DIRECTION);u.reg_write(UC_ARM_REG_S0,case.get('radius_bits',struct.unpack('<I',flt(case['radius']))[0]));w(STACK,0x12340,DELOBJ if case.get('delegate') else 0)
 u.reg_write(UC_ARM_REG_LR,STOP);saved={r:0xa0000000+r for r in [UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11]+list(range(UC_ARM_REG_D8,UC_ARM_REG_D15+1))}
 for r,v in saved.items():u.reg_write(r,v)
 trace=[];visited=set();allvisited=set();nquery=0;steps=0;pending=None
 def finish(value):
  u.reg_write(UC_ARM_REG_R0,value);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 def dispatch():
  nonlocal pending
  if pending['index']==len(pending['pointers']):
   for i,p in enumerate(pending['accepted']):w(SLOTS+4*i,p)
   w(POOL,len(pending['accepted']))
   trace.append(['accepted',len(pending['accepted'])])
   u.reg_write(UC_ARM_REG_R0,len(pending['accepted']));u.reg_write(UC_ARM_REG_PC,pending['return']);pending=None
  else:
   p=pending['pointers'][pending['index']];setargs(FILTER,p);u.reg_write(UC_ARM_REG_LR,RESUME);u.reg_write(UC_ARM_REG_PC,0x361114)
 def hook(u,a,size,data):
  nonlocal steps,pending,nquery
  steps+=1;allvisited.add(a)
  if ROOT<=a<0x26a7dc and not 0x269e48<=a<0x269e68:visited.add(a)
  if a==STOP:u.emu_stop();return
  if a==WORLD:
   assert pending is None
   args=[u.reg_read(r) for r in REGS];q=args[2];center=struct.unpack('<3f',u.mem_read(word(q),12));rad=struct.unpack('<I',u.mem_read(q+12,4))[0]
   assert args[0]==PROVIDER and args[1]==POOL and args[3]==1 and word(q+4)==0x12340 and word(q+8)==FILTER
   trace.append(['query',*[struct.unpack('<I',flt(x))[0] for x in center],rad])
   pointers=[]
   for i,c in enumerate(case['contacts']):
    p=CONTACTS+i*0x100;key=0x900000+c['key']*4+(nquery*0x10000 if case.get('new_keys') else 0)
    # The modeled scene provider supplies synthetic contacts, not world geometry.
    u.mem_write(p,bytes([i&255])*0x88);w(p,0x910000+i*4,key)
    u.mem_write(p+8,flt(*c['normal']));u.mem_write(p+0x14,flt(*sum(c['sides'],[])));u.mem_write(p+0x38,flt(*sum(c['vertices'],[])))
    position=[center[j]+c['position'][j] for j in range(3)] if c.get('relative',True) else c['position']
    u.mem_write(p+0x5c,flt(c['distance'],*position));u.mem_write(p+0x84,bytes([c['triangle']]))
    for offset,value in c.get('bits',{}).items():w(p+int(offset),value)
    pointers.append(p)
   nquery+=1
   if nquery>1024:raise RuntimeError('query bound')
   pending={'return':u.reg_read(UC_ARM_REG_LR),'pointers':pointers,'accepted':[],'index':0};dispatch();return
  if a==RESUME:
   p=pending['pointers'][pending['index']];rejected=u.reg_read(UC_ARM_REG_R0)
   trace.append(['filter',word(p+4),rejected])
   if not rejected:pending['accepted'].append(p)
   pending['index']+=1;dispatch();return
  if a==DELEGATE:
   p=u.reg_read(UC_ARM_REG_R1);reject=(word(p+4)//4)%3==0
   trace.append(['delegate',word(p+4),int(reject)]);finish(int(reject));return
 try:u.hook_add(UC_HOOK_CODE,hook);u.emu_start(ROOT,STOP,count=case.get('instruction_limit',1000000))
 except Exception as error:return {'error':str(error),'pc':hex(u.reg_read(UC_ARM_REG_PC)),'steps':steps,'trace':trace},visited,allvisited
 if u.reg_read(UC_ARM_REG_PC)!=STOP:return {'error':'instruction bound','pc':hex(u.reg_read(UC_ARM_REG_PC)),'steps':steps,'trace':trace},visited,allvisited
 assert u.reg_read(UC_ARM_REG_SP)==STACK
 assert all(u.reg_read(r)==v for r,v in saved.items())
 # Ignore stack scratch/padding; all observable fixture objects and full filter count.
 return {'return':u.reg_read(UC_ARM_REG_R0),'fpscr':u.reg_read(UC_ARM_REG_FPSCR),'memory':bytes(u.mem_read(MEM,0x30000)).hex(),'filter':bytes(u.mem_read(FILTER,0x418)).hex(),'trace':trace},visited,allvisited

def contact(kind=0,index=0,rng=None):
 if rng is None:rng=random.Random(index)
 return {'key':index,'triangle':kind,'normal':[-1,0,0],'distance':rng.choice([0,.25,.5,1,2]),'position':[rng.choice([-.5,0,.5]),rng.choice([-2,-.5,0,.5,2]),rng.choice([-1,0,1])],
         'sides':[[0,1,0],[0,-.5,1],[0,-.5,-1]] if kind==1 else [[0,0,0]]*3,
         'vertices':[[0,-2,-2],[0,2,-2],[0,0,2]]}
def fixtures():
 out=[]
 for length in [0,1,10,36,100]:
  for radius in [.5,2,35,36,100]:
   for capacity in [-1,0,1,4]:
    out.append({'start':[1,2,3],'direction':[length,0,0],'radius':radius,'capacity':capacity,'contacts':[]})
 rng=random.Random(26960)
 for j in range(160):
  n=rng.choice([1,2,3,5,16,17,20,32]);kind=j%4
  cs=[contact(kind if kind<3 else i%3,i,rng) for i in range(n)]
  if kind==2:
   for c in cs:c['triangle']=1;c['sides']=[[0,0,0]]*3
  if j%5==0:
   for c in cs:c['normal']=[1,0,0]
  if j%7==0:
   for c in cs:c['key']%=2
  out.append({'start':[rng.choice([0,1,-10]),rng.choice([0,2,-5]),rng.choice([0,3,-8])],
              'direction':[rng.choice([1,10,100]),rng.choice([0,2,-3]),rng.choice([0,1,-2])],
              'radius':rng.choice([.5,2,35,36,100]),'capacity':rng.choice([0,1,2,16,17,32]),'contacts':cs,
              'delegate':j%3==0,'new_keys':j%4==0})
 return out
if __name__=='__main__':
 parser=argparse.ArgumentParser();parser.add_argument('--limit',type=int);parser.add_argument('--output',default='build/dot_269a60/replay1.json');args=parser.parse_args()
 b=Path('data/ver/eu/code.bin').read_bytes();assert hashlib.sha256(b).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64';candidate=Path('build/dot_269a60/candidate.bin').read_bytes()
 cases=fixtures();cases=cases[:args.limit] if args.limit else cases;visited=set();allvisited=set();fail=[]
 for i,c in enumerate(cases):
  a,pcs,allpcs=run(b,None,c);z,_,_=run(b,candidate,c);visited|=pcs;allvisited|=allpcs
  if a!=z or 'error' in a or 'error' in z:
   fail.append(i);Path('build/dot_269a60/failure.json').write_text(json.dumps({'index':i,'case':c,'original':a,'candidate':z},indent=2));print('FAIL',i,a.get('return'),z.get('return'),a.get('error'),z.get('error'))
   for k in a:
    if a.get(k)!=z.get(k):print('different',k)
   break
 result={'cases':len(cases),'passed':i+1-len(fail),'failures':fail,'original_visited':sorted(visited),'all_original_visited':sorted(allvisited)}
 Path(args.output).write_text(json.dumps(result,indent=2));print({k:v for k,v in result.items() if 'visited' not in k},'root PCs',len(visited));raise SystemExit(bool(fail))
```

## Exceptional arithmetic and saturation fixtures

<!-- file: extended.py -->
```python
import sys,json,copy,hashlib
from pathlib import Path
sys.path.insert(0,str(Path('build/dot_269a60').resolve()))
from replay import *
base={'start':[0,0,0],'direction':[10,2,1],'radius':2,'capacity':1,'contacts':[contact(0,1)]}
base['contacts'][0]['position']=[0,.5,.25]
cases=[]
modes=[0,0x1000000,0x2000000,0x3000000,0x400000,0x800000,0xc00000]
values=[0,0x80000000,1,0x80000001,0x007fffff,0x807fffff,0x00800000,0x80800000,0x7f800000,0xff800000,0x7fc12345,0xffc12345,0x7fa12345,0xffa12345]
for mode in modes:
 for value in values:
  for field in ['radius','normal','position','distance','direction']:
   case=copy.deepcopy(base);case['fpscr']=mode;case['label']=[hex(mode),hex(value),field]
   if field=='radius':case['radius_bits']=value
   elif field=='direction':case['direction_bits']={0:value}
   else:case['contacts'][0]['bits']={ {'normal':8,'position':0x60,'distance':0x5c}[field]:value }
   cases.append(case)
# The distance is used on the triangle path, so repeat raw exceptional
# controls with both interior and exterior triangle projections.
for kind in ['interior','exterior']:
 for mode in modes:
  for value in values:
   for field in ['normal','position','distance']:
    case=copy.deepcopy(base);case['fpscr']=mode;case['label']=[kind,hex(mode),hex(value),field]
    case['contacts']=[contact(1,1)];case['contacts'][0]['position']=[0,.5,.25]
    if kind=='interior':case['contacts'][0]['sides']=[[0,0,0]]*3
    case['contacts'][0]['bits']={{'normal':8,'position':0x60,'distance':0x5c}[field]:value}
    cases.append(case)
# Full ring excludes many back-facing records without producing output.
for delegated in [False,True]:
 for n in [16,32,64]:
  cs=[contact(0,i) for i in range(n)]
  for c in cs:c['normal']=[1,0,0]
  cases.append({'label':['fullring',delegated,n],'fpscr':0,'start':[1,2,3],'direction':[20,1,0],
   'radius':2,'capacity':32,'instruction_limit':5000000,'contacts':cs,'delegate':delegated,'new_keys':True})
# Repeat ordinary cases under each supported FP control mode.
for mode in modes[1:]:
 for case in fixtures()[100:]:
  c=copy.deepcopy(case);c['fpscr']=mode;c['label']=['regular',mode];cases.append(c)
binary=Path('data/ver/eu/code.bin').read_bytes();candidate=Path('build/dot_269a60/candidate.bin').read_bytes();failed=[];faults=[];passed=0;visited=set();diffs={}
for i,c in enumerate(cases):
 a,pcs,_=run(binary,None,c);b,_,_=run(binary,candidate,c);visited|=pcs
 if a!=b or 'error' in a or 'error' in b:
  keys=[k for k in set(a)|set(b) if a.get(k)!=b.get(k)];entry={'index':i,'label':c.get('label'),'different':keys,'original_error':a.get('error'),'candidate_error':b.get('error'),'original_fpscr':hex(a.get('fpscr',0)),'candidate_fpscr':hex(b.get('fpscr',0))}
  if 'error' in a or 'error' in b:faults.append(entry)
  else:failed.append(entry)
  if len(failed)+len(faults)<=20:Path('build/dot_269a60/extended_failure_%d.json'%i).write_text(json.dumps({'case':c,'original':a,'candidate':b},indent=2))
  print(entry,flush=True)
 else:passed+=1
out={'cases':len(cases),'passed':passed,'mismatches':failed,'faults':faults,'visited':sorted(visited),'fpscr_modes':modes}
Path('build/dot_269a60/extended_final.json').write_text(json.dumps(out,indent=2));print('DONE',len(cases),passed,len(failed),len(faults));raise SystemExit(bool(failed or faults))
```
