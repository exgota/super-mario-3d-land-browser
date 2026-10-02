# Swimming-control update: reproducible local evidence

Run these recipes only in a disposable checkout of the submitted branch with the
owner-authorized original EU executable and approved toolchain installed. Do not
publish generated binaries, original data or compiler files. The scripts below
are evidence recipes, not changes to the build, checker or runtime tools. They
use the committed normal project-built object without modifying it.

First run `. ./development_environment.sh` and `python make.py eu -ca`. The source
is guarded by NON_MATCHING, which the unchanged normal build enables. To run the
canonical checker for this previously unnamed root, temporarily enroll only its
existing map row as symbol fn_00174024/rankM, preserving Start/Pool/End/Type, then
run `python tools/check.py fn_00174024 --object
build/eu/obj/Game/backup/src/Player/SwimControlUpdate.o`. Restore the exact map
bytes in a finally block. The expected result is a complete-section size mismatch,
not O. No metadata edits belong in a submitted patch.

Save the following blocks as their indicated ignored build files. All dependency
addresses are resolved from the existing unchanged map. The diagnostic linker
places the ordinary compiled root at00500000 and binds genuine imports to their
original addresses. This is only for replay and is not the canonical checker or
a source-closure acceptance result. Synthetic virtual callback destinations in
the fixture are explicit test models, never helper addresses in source.

## Diagnostic linking

Save as `build/swim-control-update/link.py`.

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
import re,csv,subprocess,os,json
out=Path('build/swim-control-update');obj='build/eu/obj/Game/backup/src/Player/SwimControlUpdate.o';rows=[{k.strip():v.strip() for k,v in r.items()} for r in csv.DictReader(open('data/ver/eu/map.csv'))];byname={r['Symbol']:r for r in rows if r['Symbol']};lines=['#<SYMDEFS>#'];imports=[]
# Existing clean header/README identities; independently verified initializer.
missing={}
with open(obj,'rb') as f:
 e=ELFFile(f);st=e.get_section_by_name('.symtab');root='fn_00174024'
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

## Original-code replay

Save as `build/swim-control-update/replay.py`.

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import *
from unicorn.arm_const import *
import struct,json,random,time,hashlib,collections,sys
D=Path('build/swim-control-update');original=Path('data/ver/eu/code.bin').read_bytes()
assert hashlib.sha256(original).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
with (D/'candidate.axf').open('rb') as f:
 e=ELFFile(f);ENTRY=e.header.e_entry;SEGS=[(s['p_vaddr'],s.data()) for s in e.iter_segments() if s['p_type']=='PT_LOAD']
ROOT=0x174024;END=0x174828;B=0x800000;SIZE=0x10000;SP=0xa10000;STOP=0x600000
SELF=B;CTX=B+0x100;CONFIG=B+0x200;INPUT=B+0x300;ANIM=B+0x400;PROP=B+0x1000;CVT=B+0x4000;IVT=B+0x5000;AVT=B+0x6000
CFGS={0x354:3.,0x358:4.,0x35c:2.,0x360:5.,0x364:3,0x368:.1,0x36c:1.5,0x370:.8,0x374:.9,0x378:.95}
MODEL={0x600100+i*4:('config',x) for i,x in enumerate(CFGS)}
MODEL.update({0x600200+i*4:('input',x) for i,x in enumerate([0x14,0x18,0x4c,0x50])})
MODEL.update({0x600300+i*4:('animator',x) for i,x in enumerate([8,0x18,0x2c,0x30,0x34])})
MATH={0x173f4c,0x270844,0x258a54,0x279abc,0x27cd04,0x27cc64,0x27cb48,0x27cb64}
def w(x):return struct.pack('<I',x&0xffffffff)
def fs(v):return struct.pack('<'+'f'*len(v),*v)
def fb(v):return struct.unpack('<I',fs([v]))[0]
REGS=[UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11]
SAVED={r:0xabc10000+i*0x1111 for i,r in enumerate(REGS)};SAVED.update({r:0xabc1234511110000+i*0x1111 for i,r in enumerate(range(UC_ARM_REG_D8,UC_ARM_REG_D15+1))})
def run(c,cand):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176)
 u.reg_write(UC_ARM_REG_C1_C0_2,0xf00000);u.reg_write(UC_ARM_REG_FPEXC,0x40000000);u.reg_write(UC_ARM_REG_FPSCR,c.get('fpscr',0))
 u.mem_map(0x100000,0x300000,UC_PROT_READ|UC_PROT_EXEC);u.mem_write(0x100000,original)
 u.mem_map(0x400000,0x100000);u.mem_map(0x500000,0x10000,UC_PROT_READ|UC_PROT_EXEC)
 for a,data in SEGS:u.mem_write(a,data)
 u.mem_map(STOP,0x1000,UC_PROT_READ|UC_PROT_EXEC);u.mem_map(B,SIZE);u.mem_map(0xa00000,0x20000)
 u.mem_write(B,bytes([0xa5])*SIZE);u.mem_write(0xa00000,bytes([0xcc])*0x20000)
 u.mem_write(SELF,w(0x3cc408)+w(c.get('context',CTX))+w(c.get('counter',0)))
 u.mem_write(CTX,w(c.get('property',PROP))+w(c.get('animator',ANIM)));u.mem_write(CTX+0x14,w(c.get('input',INPUT)))
 u.mem_write(CONFIG,w(CVT));u.mem_write(INPUT,w(IVT));u.mem_write(ANIM,w(AVT))
 for a,(kind,slot) in MODEL.items():u.mem_write({'config':CVT,'input':IVT,'animator':AVT}[kind]+slot,w(a))
 u.mem_write(PROP,fs(c.get('trans',[7,8,9])+c.get('front',[0,0,1])+c.get('up',[0,1,0])+c.get('velocity',[.25,.5,.75])))
 u.mem_write(PROP+0x6c,fs(c.get('axis',[0,1,0])))
 for offset,bits in c.get('words',[]):u.mem_write(PROP+offset,w(bits))
 for r,v in SAVED.items():u.reg_write(r,v)
 u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,STOP);u.reg_write(UC_ARM_REG_R0,c.get('self',SELF))
 pcs=set();events=[];calls=[];fault=None;steps=0
 def code(u,a,s,d):
  nonlocal steps
  steps+=1;pcs.add(a)
  if a in MATH:calls.append(a)
  if a==0x26e1dc:
   events.append(['config-get']);u.reg_write(UC_ARM_REG_R0,c.get('config',CONFIG));u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR));return
  if a not in MODEL:return
  kind,slot=MODEL[a];r0=u.reg_read(UC_ARM_REG_R0);assert r0=={'config':CONFIG,'input':INPUT,'animator':ANIM}[kind]
  if kind=='config':
   value=c.get('cfg',{}).get(str(slot),CFGS[slot]);events.append([kind,slot,value])
   u.reg_write(UC_ARM_REG_R0,value) if slot==0x364 else u.reg_write(UC_ARM_REG_S0,fb(value))
  elif kind=='input':
   value=c.get({0x14:'x',0x18:'y',0x4c:'paddle',0x50:'swim'}[slot],0)
   bits=c.get('x_bits' if slot==0x14 else 'y_bits',fb(value)) if slot in [0x14,0x18] else value
   events.append([kind,slot,bits]);u.reg_write(UC_ARM_REG_S0,bits) if slot in [0x14,0x18] else u.reg_write(UC_ARM_REG_R0,value)
  else:
   name=''
   if slot in [8,0x18,0x34]:
    p=u.reg_read(UC_ARM_REG_R1);q=struct.unpack('<I',u.mem_read(p+4,4))[0];name=bytes(u.mem_read(q,20)).split(b'\0')[0].decode()
   events.append([kind,slot,name]);value=c.get('is_anim',False) if slot==0x18 else c.get('paddle_anim',False) if slot==0x34 else c.get('anim_done',False) if slot==0x30 else 0
   u.reg_write(UC_ARM_REG_R0,int(value))
  u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 def invalid(u,access,a,s,v,d):
  nonlocal fault
  fault=[access,a,s];return False
 u.hook_add(UC_HOOK_CODE,code);u.hook_add(UC_HOOK_MEM_INVALID,invalid)
 try:u.emu_start(ENTRY if cand else ROOT,STOP,count=50000)
 except UcError as ex:
  if fault is None:fault=['emulator',str(ex)]
 status='fault' if fault else 'return' if u.reg_read(UC_ARM_REG_PC)==STOP else 'bounded'
 if status=='return':
  assert u.reg_read(UC_ARM_REG_SP)==SP
  assert all(u.reg_read(r)==v for r,v in SAVED.items()),'callee-save mismatch'
 return {'status':status,'fault':fault,'memory':bytes(u.mem_read(B,SIZE)),'stack':bytes(u.mem_read(SP,256)),'events':events,'calls':calls,'fpscr':u.reg_read(UC_ARM_REG_FPSCR)&0x0fffffff,'steps':steps,'pc':u.reg_read(UC_ARM_REG_PC)},pcs

def fixtures():
 cases=[]
 def add(name,**c):cases.append(dict(name=name,**c))
 for x in [-1,-.1,0,.1,1]:
  for y in [-1,-.1,0,.1,1]:
   for swim in [0,1]:
    for paddle in [0,1]:add('axes-'+str((x,y,swim,paddle)),x=x,y=y,swim=swim,paddle=paddle)
 for counter in [-2147483648,-2,-1,1,2,2147483647]:add('counter-'+str(counter),counter=counter)
 for p in [0,1]:
  for end in [0,1]:
   for anim in [0,1]:
    for swim in [0,1]:add('anim-'+str((p,end,anim,swim)),paddle_anim=p,anim_done=end,is_anim=anim,swim=swim)
 for v in [[0,0,0],[0,.2,0],[0,1.5,0],[0,10,0],[2,0,0],[2,-5,1]]:
  for swim in [0,1]:add('velocity-'+str((v,swim)),velocity=v,swim=swim)
 add('damping-crosses-cruise-speed',velocity=[0,1.6,0],swim=1)
 add('damping-crosses-zero-speed',velocity=[0,1.6,0],swim=1,cfg={str(0x370):0.0})
 rng=random.Random(0x174024)
 for i in range(180):add('random-'+str(i),x=rng.uniform(-1,1),y=rng.uniform(-1,1),front=[rng.uniform(-1,1) for _ in range(3)],up=[rng.uniform(-1,1) for _ in range(3)],axis=[rng.uniform(-1,1) for _ in range(3)],velocity=[rng.uniform(-8,8) for _ in range(3)],swim=i%2,paddle=i%3==0,counter=i%5,paddle_anim=i%7==0,anim_done=i%4==0)
 for bits in [0,0x80000000,1,0x80000001,0x007fffff,0x00800000,0x7f800000,0xff800000,0x7fc00001,0xffc00001]:
  for fpscr in [0,0x1000000,0x2000000,0x400000,0x800000,0xc00000]:
   value=struct.unpack('<f',w(bits))[0]
   for offset in [0xc,0x18,0x24]:add('word-'+str((bits,fpscr,offset)),words=[(offset,bits)],fpscr=fpscr,swim=1)
   add('axis-'+str((bits,fpscr)),x_bits=bits,y_bits=bits,fpscr=fpscr)
 for name in ['self','context','property','input','animator','config']:add('null-'+name,**{name:0})
 return cases
start=time.monotonic();results=[];failures=[];coverage=set();helpers=set();steps=0
cases=fixtures();(D/'fixtures.json').write_text(json.dumps(cases,indent=2))
for c in cases:
 a,ap=run(c,False);b,bp=run(c,True);coverage.update(x for x in ap if ROOT<=x<END);helpers.update(ap);steps+=a['steps']+b['steps']
 bad=[k for k in ['status','fault','memory','stack','events','calls','fpscr'] if a[k]!=b[k]]
 results.append({'name':c['name'],'status':a['status'],'original_steps':a['steps'],'candidate_steps':b['steps'],'failures':bad})
 if bad:
  d={'fixture':c,'bad':bad,'original':{k:v for k,v in a.items() if not isinstance(v,bytes)},'candidate':{k:v for k,v in b.items() if not isinstance(v,bytes)}}
  for k in ['memory','stack']:
   if a[k]!=b[k]:d[k]=[(i,x,y) for i,(x,y) in enumerate(zip(a[k],b[k])) if x!=y][:80]
  failures.append(d)
r={'cases':len(cases),'failures':failures,'results':results,'outcomes':dict(collections.Counter(x['status'] for x in results)),'original_instructions_visited':len(coverage),'coverage':sorted(coverage),'original_all_addresses':sorted(helpers),'total_steps':steps,'seconds':time.monotonic()-start,'models':['configuration getter returns fixture provider','provider virtual slots are constant and have no side effects','input virtual slots return fixture controls','animator virtual slots return fixture booleans and record string operations'],'original_math_helpers':sorted(MATH),'instruction_limit_per_run':50000}
(D/('replay-'+(sys.argv[1] if len(sys.argv)>1 else 'final')+'.json')).write_text(json.dumps(r,indent=2));print({k:v for k,v in r.items() if k not in ['failures','results','coverage','original_all_addresses']});print('failure count',len(failures));print(json.dumps(failures[:2],indent=2));raise SystemExit(bool(failures))
```

## Prior-object preservation

Save as `build/swim-control-update/preserve.py`.

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection
import hashlib,json,subprocess,time,sys
B=Path(sys.argv[1]).resolve();C=Path.cwd();O=Path('build/swim-control-update');O.mkdir(parents=True,exist_ok=True)
REPORT=B/'build/dot-baseline-de61/report.json';EXPECTED='3dc43ab15b453b9c79d328aadadd0d4ffb319b0b6d05c59b7dd722fd245f1970'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
assert sha(REPORT)==EXPECTED
for root in (B,C):
 subprocess.run(['git','diff','--exit-code','HEAD','--','Game','lib','data/config.json','data/ver/eu/map.csv'],cwd=root,check=True,capture_output=True)
report=json.loads(REPORT.read_text());assert report['checkpoint']=='de61f9bfed35d5dd22a4ffb24cc98a910ef0b7b0' and report['accepted'] and report['clean_build_returncode']==0
checks=report['prior_checks'];assert len(checks)==913 and len({x['symbol'] for x in checks})==893 and all(x['returncode']==0 for x in checks)
assert (B/'data/ver/eu/map.csv').read_bytes()==(C/'data/ver/eu/map.csv').read_bytes()
def canonical(path):
 with path.open('rb') as f:
  e=ELFFile(f);st=e.get_section_by_name('.symtab');ret=[];sections=list(e.iter_sections());relocations={}
  for rel in sections:
   if isinstance(rel,RelocationSection):
    relocations.setdefault(rel['sh_info'],[]).append(rel)
  for i,s in enumerate(sections):
   if not s['sh_flags']&2:continue
   r=[]
   for rel in relocations.get(i,[]):
    for v in rel.iter_relocations():
     sym=st.get_symbol(v['r_info_sym']);ndx=sym['st_shndx'];name=sections[ndx].name if isinstance(ndx,int) else ndx
     r.append((v['r_offset'],v['r_info_type'],sym.name,sym['st_value'],sym['st_size'],sym['st_info']['bind'],sym['st_info']['type'],name))
   ret.append((s.name,s['sh_type'],s['sh_flags'],s['sh_addralign'],s['sh_size'],sha_bytes(s.data()),r))
  defs=sorted((s.name,s['st_value'],s['st_size'],s['st_info']['bind'],s['st_info']['type'],sections[s['st_shndx']].name) for s in st.iter_symbols() if isinstance(s['st_shndx'],int) and s['st_info']['type']=='STT_FUNC')
  return ret,defs

def sha_bytes(data):return hashlib.sha256(data).hexdigest()
start=time.monotonic();records=[];inputs={};pair_defs={};differences=[]
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
 left=canonical(B/p['object']);right=canonical(C/q['object'])
 if left!=right:differences.append({'object':p['object'],'baseline':left,'candidate':right})
 pair_defs[p['object']]=left[1]
 records.append({'object':p['object'],'baseline_sha256':p['object_sha256'],'proposal_sha256':q['object_sha256'],'raw_equal':p['object_sha256']==q['object_sha256'],'allocated_equal':left==right})
for check in checks:assert any(d[0]==check['symbol'] for d in pair_defs[check['object']])
print('object comparisons complete',len(records),len(differences),flush=True)
(O/'object-differences.json').write_text(json.dumps(differences,indent=2))
difference_checks=[]
for difference in differences:
 for check in checks:
  if check['object']!=difference['object']:continue
  command=[sys.executable,'tools/check.py',check['symbol'],'--object',check['object']]
  run=subprocess.run(command,text=True,capture_output=True)
  difference_checks.append({'command':command,'returncode':run.returncode,'output':run.stdout,'error':run.stderr})
  assert run.returncode==0 and 'matches byte for byte' in run.stdout,(command,run.stdout,run.stderr)

extra=[]
for prov in C.glob('build/eu/obj/**/*.provenance.json'):
 if not (B/prov.relative_to(C)).exists():
  p=json.loads(prov.read_text())
  if not p['source'].startswith(('Game/','lib/')):continue
  extra.append(p['source']);defs=canonical(C/p['object'])[1];assert [d[0] for d in defs if d[3]=='STB_GLOBAL']==['fn_00174024'],defs
assert extra==['Game/backup/src/Player/SwimControlUpdate.cpp']
print('affected canonical checks complete',len(difference_checks),flush=True)
alias_records=[]
for source in extra:
 obj='build/eu/obj/'+source[:-4]+'.o'
 for d in canonical(C/obj)[1]:
  if d[3]!='STB_WEAK':continue
  providers=[{'object':o,'definition':x} for o,defs in pair_defs.items() for x in defs if x[0]==d[0]]
  assert providers,d
  matches=[]
  own_sections={x[0]:x for x in canonical(C/obj)[0]}
  for provider in providers:
   old_sections={x[0]:x for x in canonical(B/provider['object'])[0]}
   if d[5] in own_sections and provider['definition'][5] in old_sections:
    if own_sections[d[5]]==old_sections[provider['definition'][5]]:matches.append(provider['object'])
  assert matches,d
  alias_records.append({'symbol':d[0],'size':d[2],'matching_baseline_providers':matches})
# Generated compact placeholders remain scaffolding, never accepted C++ roots.
print('weak alias checks complete',len(alias_records),flush=True)
bst=B/'build/eu/obj/build/eu/split/stubs.o';cst=C/'build/eu/obj/build/eu/split/stubs.o'
old=dict((x[0],x) for x in canonical(bst)[0]);new=dict((x[0],x) for x in canonical(cst)[0]);assert all(new[k]==v for k,v in old.items())
added=sorted(set(new)-set(old));assert added==['i.fn_00173F4C','i.fn_00174024','i.fn_00258A54','i.fn_0026E1DC','i.fn_00270844','i.fn_00279ABC'],added
result={'objects_with_differences':[x['object'] for x in differences],'affected_canonical_checks':difference_checks,'weak_aliases':alias_records,'generated_stub_prior_sections_preserved':len(old),'generated_stub_sections_added':added,'baseline_checkpoint' :report['checkpoint'],'baseline_report_sha256':EXPECTED,'roots':893,'definitions':913,'prior_objects':len(records),'raw_equal_objects':sum(x['raw_equal'] for x in records),'allocated_equal_objects':sum(x['allocated_equal'] for x in records),'unchanged_inputs':len(inputs),'extra_sources':extra,'seconds':time.monotonic()-start,'map_sha256':sha(C/'data/ver/eu/map.csv'),'records':records,'inputs':inputs,'scope':'Exhaustive unchanged-input and compiler-object equivalence to the separately verified 893-root/913-definition baseline. This is not a new canonical 913-check run.'}
(O/'preservation-equivalence.json').write_text(json.dumps(result,indent=2));print({k:v for k,v in result.items() if k not in ('records','inputs')})
```

Run `python build/swim-control-update/link.py`, then
`python build/swim-control-update/replay.py final`. The fixture generator is seeded
with00174024 and emits `fixtures.json`; nonfinite inputs use exact integer IEEE
words, including their sign, so the JSON is reproducible without NaN conversion.
Every run is capped at50,000 instructions. Actual input, animation and
configuration methods are modeled: they return the specified values, do not
mutate state and record calls. The configuration getter returns the fixture
provider. Original rotation, trigonometric, vector and direction/speed helpers
execute unchanged. No game loop, real provider side effect, arbitrary aliasing,
full-image correctness, rendering or gameplay result follows.

The comparison checks the complete64KiB fixture arena, protected caller-stack
bytes, ordered virtual events and imported helper calls, floating exception and
mode bits, return/fault classification and fault access/address/size. It verifies
callee-saved core/VFP registers and SP on return. r0 from the void update and
FPSCR condition-code flags are deliberately outside the contract. The six null
pointer cases compare bounded faults, not supported runtime inputs. Negative
counter cases describe this ARMCC build, not a portable signed-overflow promise.

For preservation, run the third recipe with an independently clean built de61
checkout as its argument. It validates the exact recorded canonical baseline
report before comparing all discovered prior Game/lib object provenance, inputs,
compiler hashes, normalized build commands, allocated sections, relocations and
actual definitions. Any non-identical object triggers fresh canonical checks for
its accepted definitions. New weak aliases and generated scaffolding are audited
separately. The scripted2,376old scaffold-section equality and six additions do
not turn stubs into game behavior or accepted source.
