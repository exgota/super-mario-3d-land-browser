# Reproduce joint aim root verification

Run from this branch's root with only the owner's ignored verified EU inputs, installed 791/902/wibo and the existing Python environment (Unicorn, Capstone, pyelftools). The root is deliberately U. The pristine base754 comparison checkout must have passed the named canonical baseline independently; absence of that evidence requires a new main-owned preservation gate.

Extract the scripts below, then run:

```sh
. ./development_environment.sh
export DEVKITARM=/usr TMP=/tmp
python build/root-1dfb44/clean.py
python build/root-1dfb44/probe.py final
python build/root-1dfb44/link.py
python build/root-1dfb44/distance.py
python build/root-1dfb44/replay.py final
python build/root-1dfb44/preserve.py /absolute/path/to/pristine754
python build/root-1dfb44/seal.py
```

The probe's checker rejection is expected and is saved in JSON; it restores the exact map. The link scripts use the unedited compiler-produced object. Their ex/ey definitions are explicitly diagnostic original-address bindings, not canonical enrollment or fabricated replacement data. The distance is diagnostic only. The replay creates synthetic fixture memory and executes the actual original static initializers and math implementations; it never patches original instructions. No binary output or private input is uploaded.

```python
from pathlib import Path
import re
text=Path('project/dot_reports/joint-aim-1dfb44-replay.md').read_text()
out=Path('build/root-1dfb44');out.mkdir(parents=True,exist_ok=True)
for name,body in re.findall(r'<!-- script: ([^ ]+) -->\n```python\n(.*?)```',text,re.S):
    (out/name).write_text(body)
```

<!-- script: clean.py -->
```python
from pathlib import Path
import subprocess,time,json,os
D=Path('build/root-1dfb44');t=time.monotonic()
r=subprocess.run(['python','make.py','eu','-ca'],stdout=(D/'clean-build.log').open('w'),stderr=subprocess.STDOUT,env=dict(os.environ,TMP='/tmp'))
result={'returncode':r.returncode,'seconds':time.monotonic()-t,'source_commit':subprocess.check_output(['git','rev-parse','HEAD'],text=True).strip()};(D/'clean-build.json').write_text(json.dumps(result,indent=2));print(result)
raise SystemExit(r.returncode)
```

<!-- script: probe.py -->
```python
from pathlib import Path
import subprocess,os,json,time,sys
p=Path('data/ver/eu/map.csv');b=p.read_bytes();lines=[]
for l in b.decode().splitlines(True):
 f=l.split(',')
 if f[0]=='0x001DFB44':assert not f[6].strip();f[6]='fn_001DFB44'
 lines.append(','.join(f))
try:
 p.write_text(''.join(lines));t=time.monotonic()
 r=subprocess.run(['python','tools/check.py','fn_001DFB44','--object','build/eu/obj/lib/al/src/Model/alJointAim1DFB44.o'],capture_output=True,text=True,env=dict(os.environ,TMP='/tmp'))
 result={'source_commit':subprocess.check_output(['git','rev-parse','HEAD'],text=True).strip(),'seconds':time.monotonic()-t,'returncode':r.returncode,'output':r.stdout+r.stderr};print(result);Path('build/root-1dfb44/check-'+sys.argv[1]+'.json').write_text(json.dumps(result,indent=2))
finally:p.write_bytes(b)
```

<!-- script: link.py -->
```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
import re,csv,subprocess,os,json
out=Path('build/root-1dfb44');obj='build/eu/obj/lib/al/src/Model/alJointAim1DFB44.o';rows=[{k.strip():v.strip() for k,v in r.items()} for r in csv.DictReader(open('data/ver/eu/map.csv'))];byname={r['Symbol']:r for r in rows if r['Symbol']};lines=['#<SYMDEFS>#'];imports=[]
# Existing clean header/README identities; independently verified initializer.
missing={'_ZN4sead7Vector3IfE2exE':(0x4305c8,0x4305d4),'_ZN4sead7Vector3IfE2eyE':(0x4305d4,0x4305e0)}
with open(obj,'rb') as f:
 e=ELFFile(f);st=e.get_section_by_name('.symtab');root='fn_001DFB44'
 defs=[{'symbol':s.name,'size':s['st_size'],'kind':s['st_info']['bind']} for s in st.iter_symbols() if s['st_info']['type']=='STT_FUNC' and s['st_shndx']!='SHN_UNDEF']
 for s in st.iter_symbols():
  if s['st_shndx']!='SHN_UNDEF' or not s.name or '$$' in s.name or s['st_info']['bind']=='STB_WEAK':continue
  n=s.name;m=re.fullmatch('(fn|dat)_([0-9A-F]{8})',n)
  if n in missing:a,end=missing[n];kind='D';enrolled=False
  else:
   r=next(x for x in rows if int(x['Start'],16)==int(m[2],16)) if m else byname[n]
   a=int(r['Start'],16);end=int(r['End'],16);kind='A' if 'f' in r['Type'] else 'D';enrolled=True
  lines.append(f'0x{a:08X} {kind} {n}');imports.append({'symbol':n,'address':a,'kind':kind,'end':end,'canonical_enrolled':enrolled})
 (out/'imports.json').write_text(json.dumps(imports,indent=2));(out/'definitions.json').write_text(json.dumps(defs,indent=2))
(out/'imports.sym').write_text('\n'.join(lines)+'\n')
c=['data/compilers/wibo','data/compilers/4.1/791/bin/armlink.exe','--cpu=MPCore','--fpu=VFPv2','--arm_only','--no_exceptions','--inline','--datacompressor=off','--no_debug','--no_scanlib','--mangled','--symbols','--map','--entry='+root,'--keep='+root,'--ro_base=0x00500000','--output='+str(out/'candidate.axf'),'--list='+str(out/'candidate.map'),obj,str(out/'imports.sym')]
r=subprocess.run(c,capture_output=True,text=True,env=dict(os.environ,TMP='/tmp'));(out/'link.json').write_text(json.dumps(dict(command=c,returncode=r.returncode,output=r.stdout+r.stderr),indent=2));print(r.stdout+r.stderr);raise SystemExit(r.returncode)
```

<!-- script: distance.py -->
```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
import json,subprocess,os,hashlib
D=Path('build/root-1dfb44');c=json.loads((D/'link.json').read_text())['command'];c=[x.replace('--ro_base=0x00500000','--ro_base=0x001DFB44').replace('candidate.axf','diagnostic-original-address.axf').replace('candidate.map','diagnostic-original-address.map') for x in c]
r=subprocess.run(c,capture_output=True,text=True,env=dict(os.environ,TMP='/tmp'));assert r.returncode==0,r.stdout+r.stderr
with (D/'diagnostic-original-address.axf').open('rb') as f:
 e=ELFFile(f);assert e.header.e_entry==0x1dfb44
 seg=next(x for x in e.iter_segments() if x['p_type']=='PT_LOAD' and x['p_vaddr']<=0x1dfb44<x['p_vaddr']+x['p_filesz']);size=json.loads((D/'definitions.json').read_text())[0]['size'];start=0x1dfb44-seg['p_vaddr'];got=seg.data()[start:start+size]
original=Path('data/ver/eu/code.bin').read_bytes()[0xdfb44:0xe04dc]
result={'diagnostic_only':True,'project_checker_exact':False,'source_generated_bytes':len(got),'original_complete_bytes':len(original),'overlap_differing_bytes':sum(a!=b for a,b in zip(got,original)),'unpaired_bytes':abs(len(got)-len(original)),'source_interval_sha256':hashlib.sha256(got).hexdigest(),'original_interval_sha256':hashlib.sha256(original).hexdigest(),'imports_without_canonical_rows':['sead::Vector3<float>::ex','sead::Vector3<float>::ey'],'command':c}
(D/'diagnostic-distance.json').write_text(json.dumps(result,indent=2));print(result)
```

<!-- script: replay.py -->
```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import *
from unicorn.arm_const import *
import struct,json,random,time,hashlib,collections,sys
D=Path('build/root-1dfb44');ORIG=Path('data/ver/eu/code.bin').read_bytes()
assert hashlib.sha256(ORIG).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
ROOT=0x1dfb44;END=0x1e04dc;B=0x800000;SIZE=0x10000;SP=0xa10000;STOP=0x600000
SELF=B;MAT=B+0x100;ACTOR=B+0x200;REF=B+0x300;ENT=B+0x1000
with (D/'candidate.axf').open('rb') as f:
 e=ELFFile(f);CAND=e.header.e_entry;SEGS=[(s['p_vaddr'],s.data()) for s in e.iter_segments() if s['p_type']=='PT_LOAD']
def w(v):return struct.pack('<I',v&0xffffffff)
def floats(*v):return struct.pack('<'+'f'*len(v),*v)
def vec(v):return floats(*v)
def words(*v):return b''.join(w(x) for x in v)
def identity(t=(0,0,0)):return (1.,0.,0.,t[0],0.,1.,0.,t[1],0.,0.,1.,t[2])
def new(initializing=False):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176)
 u.reg_write(UC_ARM_REG_C1_C0_2,0xf00000);u.reg_write(UC_ARM_REG_FPEXC,0x40000000);u.reg_write(UC_ARM_REG_FPSCR,0)
 u.mem_map(0x100000,0x300000,UC_PROT_ALL if initializing else UC_PROT_READ|UC_PROT_EXEC);u.mem_write(0x100000,ORIG)
 u.mem_map(0x400000,0x100000);u.mem_map(0x500000,0x10000,UC_PROT_READ|UC_PROT_EXEC)
 for a,by in SEGS:u.mem_write(a,by)
 u.mem_map(STOP,0x1000,UC_PROT_READ|UC_PROT_EXEC);u.mem_map(B,SIZE);u.mem_map(0xa00000,0x20000)
 u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,STOP)
 return u
u=new(True);initpcs=set();initwrites=[]
def ih(u,a,s,d):initpcs.add(a)
h=u.hook_add(UC_HOOK_CODE,ih)
u.hook_add(UC_HOOK_MEM_WRITE,lambda u,a,p,s,v,d:initwrites.append((p,s)))
for entry in (0x381350,0x383870):
 u.reg_write(UC_ARM_REG_LR,STOP);u.emu_start(entry,STOP,count=100000)
 assert u.reg_read(UC_ARM_REG_PC)==STOP
INIT=bytes(u.mem_read(0x400000,0x100000));INIT_DATA=bytes(u.mem_read(0x3d0000,0x30000));u.hook_del(h)
assert all(a>=0x3d0000 or 0xa00000<=a<0xa20000 for a,s in initwrites)
assert INIT[0x305c8:0x305e0]==floats(1,0,0,0,1,0)
(D/'initializer.json').write_text(json.dumps({'writes':initwrites,'entries':[0x381350,0x383870],'executed_addresses':sorted(initpcs),'ex':list(struct.unpack('<fff',INIT[0x305c8:0x305d4])),'ey':list(struct.unpack('<fff',INIT[0x305d4:0x305e0]))},indent=2))
REGS=[UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11]
DREGS=list(range(UC_ARM_REG_D8,UC_ARM_REG_D15+1))
SAVED={r:0xabc10000+i*0x1111 for i,r in enumerate(REGS)};SAVED.update({r:0xabc1234511110000+i*0x1111 for i,r in enumerate(DREGS)})
IMPORTS={x['address'] for x in json.loads((D/'imports.json').read_text()) if x['kind']=='A'}
coverage=set();allpcs=set();totalsteps=0;results=[];failures=[]
def run(cfg,candidate):
 u=new();u.mem_protect(0x3d0000,0x30000,UC_PROT_READ|UC_PROT_WRITE);u.mem_write(0x3d0000,INIT_DATA);u.mem_write(0x400000,INIT);u.mem_write(B,bytes([0xa5])*SIZE);u.mem_write(0xa00000,bytes([0xcc])*0x20000)
 u.mem_write(SELF,vec(cfg.get('target',(1,2,5)))+w(cfg.get('actor_address',ACTOR))+w(cfg.get('buffer',ENT))+words(cfg.get('capacity',4),cfg.get('first',0),cfg.get('count',1)))
 u.mem_write(SELF+0x20,bytes([cfg.get('enabled',1),cfg.get('retain',0)]))
 u.mem_write(MAT,vec(cfg.get('matrix',identity())));u.mem_write(ACTOR,vec(cfg.get('actor',identity())));u.mem_write(REF,vec(cfg.get('reference',identity())))
 for i in range(4):
  a=ENT+0x88*i;c=dict(cfg);c.update(cfg.get('entries',{}).get(str(i),{}))
  u.mem_write(a,words(c.get('joint',7))+vec(c.get('horizontal',(-3.1415927,3.1415927)))+vec(c.get('vertical',(-3.1415927,3.1415927))))
  u.mem_write(a+0x14,vec(c.get('up',(0,1,0)))+vec(c.get('side',(1,0,0)))+vec(c.get('front',(0,0,1))))
  u.mem_write(a+0x38,vec(c.get('current',(0,0,0,1)))+vec(c.get('previous',(0.1,0.2,0.3,0.8))))
  u.mem_write(a+0x58,bytes([c.get('was',0)]));u.mem_write(a+0x5c,vec((c.get('amount',0.5),)))
  u.mem_write(a+0x60,w(c.get('reference_address',REF) if c.get('has_reference') else 0)+vec(c.get('ref_front',(0,0,1)))+vec(c.get('ref_up',(0,1,0)))+vec(c.get('ref_side',(1,0,0))))
 for addr,hexby in cfg.get('writes',[]):u.mem_write(addr,bytes.fromhex(hexby))
 u.reg_write(UC_ARM_REG_R0,cfg.get('self',SELF));u.reg_write(UC_ARM_REG_R1,cfg.get('mat',MAT));u.reg_write(UC_ARM_REG_R2,cfg.get('selected',7));u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,STOP)
 for r,v in SAVED.items():u.reg_write(r,v)
 u.reg_write(UC_ARM_REG_FPSCR,cfg.get('fpscr',0))
 pcs=set();calls=[];steps=0;fault=None
 def hook(u,a,s,d):
  nonlocal steps
  steps+=1;pcs.add(a)
  if a in IMPORTS:calls.append(a)
 def invalid(u,access,a,s,v,d):
  nonlocal fault
  fault=[access,a,s];return False
 u.hook_add(UC_HOOK_CODE,hook);u.hook_add(UC_HOOK_MEM_INVALID,invalid)
 try:u.emu_start(CAND if candidate else ROOT,STOP,count=cfg.get('budget',100000))
 except UcError as ex:
  if fault is None:fault=['emulator',str(ex)]
 status='fault' if fault else 'return' if u.reg_read(UC_ARM_REG_PC)==STOP else 'bounded'
 if status=='return':
  assert u.reg_read(UC_ARM_REG_SP)==SP
  assert all(u.reg_read(r)==v for r,v in SAVED.items()),'callee-saved register changed'
 result={'status':status,'fault':fault,'r0':u.reg_read(UC_ARM_REG_R0),'memory':bytes(u.mem_read(B,SIZE)),'globals':bytes(u.mem_read(0x3d0000,0x130000)),'caller_stack':bytes(u.mem_read(SP,0x100)),'calls':calls,'steps':steps,'fpscr':u.reg_read(UC_ARM_REG_FPSCR),'pc':u.reg_read(UC_ARM_REG_PC)}
 return result,pcs

def fixtures():
 cases=[]
 def add(name,**cfg):cases.append(dict(name=name,**cfg))
 add('basic')
 for n in [-2147483648,-1,0]:add('nonpositive-count-'+str(n),count=n)
 add('miss',selected=8);add('last-match',count=4,first=2,entries={'2':{'joint':1},'3':{'joint':2},'0':{'joint':3}})
 for enabled in [0,1]:
  for retain in [0,1]:
   for was in [0,1]:
    for target in [(0,0,-1),(1,0,0),(0,1,0),(0,0,0),(1,2,5),(5,1,0.2),(-1,-2,5)]:
     for ref in [0,1]:add(f'flags-{enabled}{retain}{was}-{target}-{ref}',enabled=enabled,retain=retain,was=was,target=target,has_reference=ref)
 for amount in [0,1,-1,2,0.00000001,0.25,0.75]:
  for current in [(0,0,0,1),(0,0,0,-1),(0.1,0.2,0.3,0.8),(2,0,0,0),(0,0,0,0)]:add('interpolation-'+str((amount,current)),amount=amount,current=current)
 for horizontal,vertical in [((-.2,.2),(-.3,.3)),((0,0),(0,0)),((1,-1),(1,-1))]:add('limits-'+str(horizontal),horizontal=horizontal,vertical=vertical)
 rng=random.Random(0x1dfb44)
 for i in range(150):
  a=rng.uniform(-3.0,3.0);b=rng.uniform(-3.0,3.0);c=rng.uniform(-1.0,8.0)
  add('random-'+str(i),target=(a,b,c),amount=rng.uniform(-0.5,1.5),has_reference=i%2,was=(i//2)%2,retain=(i//4)%2,enabled=i%11!=0,horizontal=(-rng.random(),rng.random()),vertical=(-rng.random(),rng.random()),matrix=identity((rng.random(),rng.random(),rng.random())))
 for i in range(80):
  matrix=tuple(rng.uniform(-2,2) for j in range(12))
  reference=tuple(rng.uniform(-2,2) for j in range(12))
  add('nonorthogonal-'+str(i),matrix=matrix,reference=reference,actor=matrix,has_reference=i%2,side=tuple(rng.uniform(-2,2) for j in range(3)),up=tuple(rng.uniform(-2,2) for j in range(3)),front=tuple(rng.uniform(-2,2) for j in range(3)))
 for bits in [0,0x80000000,1,0x80000001,0x007fffff,0x00800000,0x7f800000,0xff800000,0x7fc00001,0xffc00001]:
  value=struct.unpack('<f',w(bits))[0]
  for fpscr in [0,0x1000000,0x2000000,0x400000,0x800000,0xc00000]:
   add('float-target-'+hex(bits)+'-'+hex(fpscr),target=(value,1,5),fpscr=fpscr)
   add('float-current-'+hex(bits)+'-'+hex(fpscr),current=(value,0,0,1),fpscr=fpscr)
   add('float-basis-'+hex(bits)+'-'+hex(fpscr),side=(value,0,0),fpscr=fpscr)
 add('actor-joint-alias',actor_address=MAT)
 add('reference-joint-alias',has_reference=True,reference_address=MAT)
 add('entry-joint-alias',mat=ENT+0x14)
 for name,args in [('null-self',{'self':0}),('null-matrix',{'mat':0}),('null-buffer',{'buffer':0}),('negative-first',{'first':-10000}),('oversize-first',{'first':0x70000000}),('oversize-count-budget',{'count':0x7fffffff,'selected':9,'capacity':4,'budget':10000})]:add(name,**args)
 return cases

start=time.monotonic()
for cfg in fixtures():
 a,ap=run(cfg,False);b,bp=run(cfg,True);totalsteps+=a['steps']+b['steps'];coverage.update(x for x in ap if ROOT<=x<END);allpcs.update(ap);allpcs.update(bp)
 failures_here=[]
 for key in ('status','fault','memory','globals','caller_stack'):
  if a[key]!=b[key]:failures_here.append(key)
 if a['status']=='return':
  for key in ('r0','calls','fpscr'):
   if a[key]!=b[key]:failures_here.append(key)
 result={'name':cfg['name'],'status':a['status'],'original_steps':a['steps'],'candidate_steps':b['steps'],'original_pc':a['pc'],'candidate_pc':b['pc'],'failures':failures_here}
 if failures_here:
  detail={'case':cfg,'failures':failures_here,'original':{k:v for k,v in a.items() if not isinstance(v,bytes)},'candidate':{k:v for k,v in b.items() if not isinstance(v,bytes)}}
  for key in ('memory','globals','caller_stack'):
   if a[key]!=b[key]:detail[key+'_differences']=[{'offset':i,'original':x,'candidate':y} for i,(x,y) in enumerate(zip(a[key],b[key])) if x!=y][:100]
  failures.append(detail)
 results.append(result)
summary={'cases':len(results),'outcomes':dict(collections.Counter(x['status'] for x in results)),'failures':failures,'total_steps':totalsteps,'seconds':time.monotonic()-start,'original_root_instructions_visited':len(coverage),'coverage_addresses':sorted(coverage),'all_executed_addresses':sorted(allpcs),'results':results,'models':[]}
(D/('replay-'+(sys.argv[1] if len(sys.argv)>1 else 'final')+'.json')).write_text(json.dumps(summary,indent=2));print({k:v for k,v in summary.items() if k not in ('failures','coverage_addresses','all_executed_addresses','results')});print('failures',len(failures));print(json.dumps(failures[:2],indent=2))
raise SystemExit(bool(failures))
```

<!-- script: preserve.py -->
```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection
import hashlib,json,subprocess,time,sys
B=Path(sys.argv[1]).resolve();C=Path.cwd();O=Path('build/root-1dfb44');O.mkdir(parents=True,exist_ok=True)
REPORT=B/'build/dot-baseline-754/report.json';EXPECTED='c208f39fadf49e66c5c98c97b445881ba2e51ade07b5724610cf0309175ec028'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
assert sha(REPORT)==EXPECTED
for root in (B,C):
 subprocess.run(['git','diff','--exit-code','HEAD','--','Game','lib','data/config.json','data/ver/eu/map.csv'],cwd=root,check=True,capture_output=True)
report=json.loads(REPORT.read_text());assert report['checkpoint']=='754f99a30a337756df5aa01c28b4ed6a977fecd5' and report['accepted'] and report['clean_build_returncode']==0
checks=report['prior_checks'];assert len(checks)==753 and len({x['symbol'] for x in checks})==733 and all(x['returncode']==0 for x in checks)
assert (B/'data/ver/eu/map.csv').read_bytes()==(C/'data/ver/eu/map.csv').read_bytes()
def canonical(path):
 with path.open('rb') as f:
  e=ELFFile(f);st=e.get_section_by_name('.symtab');ret=[]
  for i,s in enumerate(e.iter_sections()):
   if not s['sh_flags']&2:continue
   r=[]
   for rel in e.iter_sections():
    if isinstance(rel,RelocationSection) and rel['sh_info']==i:
     for v in rel.iter_relocations():
      sym=st.get_symbol(v['r_info_sym']);ndx=sym['st_shndx'];name=e.get_section(ndx).name if isinstance(ndx,int) else ndx
      r.append((v['r_offset'],v['r_info_type'],sym.name,sym['st_value'],sym['st_size'],sym['st_info']['bind'],sym['st_info']['type'],name))
   ret.append((s.name,s['sh_type'],s['sh_flags'],s['sh_addralign'],s['sh_size'],sha_bytes(s.data()),r))
  defs=sorted((s.name,s['st_value'],s['st_size'],s['st_info']['bind'],s['st_info']['type'],e.get_section(s['st_shndx']).name) for s in st.iter_symbols() if isinstance(s['st_shndx'],int) and s['st_info']['type']=='STT_FUNC')
  return ret,defs

def sha_bytes(data):return hashlib.sha256(data).hexdigest()
start=time.monotonic();records=[];inputs={};pair_defs={}
paths=sorted(B.glob('build/eu/obj/**/*.provenance.json'))
for prov in paths:
 rel=prov.relative_to(B);p=json.loads(prov.read_text())
 if not p['source'].startswith(('Game/','lib/')):continue
 q=json.loads((C/rel).read_text())
 assert p['schema']==q['schema']==1 and p['inputs_stable'] and q['inputs_stable']
 for key in ('source','object','version','compiler','compiler_sha256','inputs'):assert p[key]==q[key],(str(rel),key)
 for root,r in ((B,p),(C,q)):
  assert sha(root/r['object'])==r['object_sha256'];assert sha(root/r['compiler'])==r['compiler_sha256']
  for name,h in r['inputs'].items():assert sha(root/name)==h;inputs[name]=h
  committed=subprocess.check_output(['git','ls-files','-z','--',*r['inputs']],cwd=root).decode().split('\0');assert set(r['inputs'])<=set(committed)
 assert [x.replace(str(B),'$ROOT') for x in p['command']]==[x.replace(str(C),'$ROOT') for x in q['command']]
 left=canonical(B/p['object']);right=canonical(C/q['object']);assert left==right,p['object']
 pair_defs[p['object']]=left[1]
 records.append({'object':p['object'],'baseline_sha256':p['object_sha256'],'proposal_sha256':q['object_sha256'],'raw_equal':p['object_sha256']==q['object_sha256'],'allocated_equal':True})
for check in checks:assert any(d[0]==check['symbol'] for d in pair_defs[check['object']])
extra=[]
for prov in C.glob('build/eu/obj/**/*.provenance.json'):
 if not (B/prov.relative_to(C)).exists():
  p=json.loads(prov.read_text())
  if not p['source'].startswith(('Game/','lib/')):continue
  extra.append(p['source']);defs=canonical(C/p['object'])[1];assert [d[0] for d in defs]==['fn_001DFB44'],defs
assert extra==['lib/al/src/Model/alJointAim1DFB44.cpp']
result={'baseline_checkpoint':report['checkpoint'],'baseline_report_sha256':EXPECTED,'roots':733,'definitions':753,'prior_objects':len(records),'raw_equal_objects':sum(x['raw_equal'] for x in records),'allocated_equal_objects':len(records),'unchanged_inputs':len(inputs),'extra_sources':extra,'seconds':time.monotonic()-start,'map_sha256':sha(C/'data/ver/eu/map.csv'),'records':records,'inputs':inputs,'scope':'Exhaustive unchanged-input and compiler-object equivalence to the separately verified 733-root/753-definition baseline. This is not a new canonical 753-check run.'}
(O/'preservation-equivalence.json').write_text(json.dumps(result,indent=2));print({k:v for k,v in result.items() if k not in ('records','inputs')})
```

<!-- script: seal.py -->
```python
from pathlib import Path
import json,hashlib,subprocess,datetime
D=Path('build/root-1dfb44');hashes={}
for p in [Path('lib/al/src/Model/alJointAim1DFB44.cpp'),Path('lib/al/include/Model/alJointAim1DFB44.h'),Path('data/ver/eu/map.csv'),Path('data/config.json'),Path('Game/project_globals.h'),Path('make.py'),Path('data/compilers/wibo'),Path('data/compilers/4.1/791/bin/armcc.exe'),Path('data/compilers/4.1/791/bin/armlink.exe'),Path('data/compilers/4.0/902/bin/armcc.exe'),Path('data/compilers/4.0/902/bin/armlink.exe'),*Path('tools').rglob('*.py')]:
 hashes[str(p)]=hashlib.sha256(p.read_bytes()).hexdigest()
prov=json.load(open('build/eu/obj/lib/al/src/Model/alJointAim1DFB44.provenance.json'))
counts={}
for p in Path('build/eu/obj').rglob('*.provenance.json'):
 j=json.loads(p.read_text());n=j['source']
 if n.startswith('Game/'):k='Game'
 elif n.startswith('lib/al/'):k='lib/al'
 elif n.startswith('lib/CtrSDK/'):k='lib/CtrSDK'
 else:continue
 counts[k]=counts.get(k,0)+1
result={'checkpoint':subprocess.check_output(['git','rev-parse','HEAD'],text=True).strip(),'base':'754f99a30a337756df5aa01c28b4ed6a977fecd5','observed_utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),'hashes':hashes,'candidate_build_provenance':prov,'compiled_sources':counts,'compiler_894_present':Path('data/compilers/4.1/894').exists()}
(D/'evidence.json').write_text(json.dumps(result,indent=2));print({k:v for k,v in result.items() if k not in ('hashes','candidate_build_provenance')});print({k:v for k,v in hashes.items() if not k.startswith('tools/')})
```

