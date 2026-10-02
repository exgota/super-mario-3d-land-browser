# Reproduce complete model/resource setup

Use the approved local EU code.bin/exh.bin and installed ARMCC 791/wibo, Unicorn,
Capstone and pyelftools inputs. No private bytes appear in these scripts. The only
function source is the committed C++ TU; the diagnostic AXF is not a canonical
acceptance input. The default project build defines NON_MATCHING.

Materialize these fenced files from this note, then run from the repository root:

```sh
. ./development_environment.sh
python - <<'EXTRACT_MODEL_RESOURCE_167CAC'
from pathlib import Path
import re
note=Path('project/dot_reports/model-resource-setup-reproduction.md').read_text()
for name,body in re.findall(r'^## (build/model-resource-setup/[^\n]+)\n\n```python\n(.*?)\n```',note,re.M|re.S):
 p=Path(name);p.parent.mkdir(parents=True,exist_ok=True);p.write_text(body+'\n')
EXTRACT_MODEL_RESOURCE_167CAC
python make.py eu -ca > build/model-resource-setup/clean-build.log 2>&1
python build/model-resource-setup/check.py
python build/model-resource-setup/link.py
python build/model-resource-setup/replay.py
python build/model-resource-setup/preserve.py
python build/model-resource-setup/audit.py
```

The check wrapper records canonical exit 1 for the 2064/2116 extent mismatch and
restores the exact map in finally. The wrapper's own exit 0 is not a match.
The diagnostic link verifies every imported address against an existing row and
links only original compiler output, without altering the checker. Its warning
about removed unused RO sections is recorded.

The preservation script expects sibling mario-main861 at the frozen base,
independently clean-built and gated, with baseline report SHA-256
a692f9a29d92c3f8021b62a2254b06fb44776f34cfa2f51849364bde66dee374.
Do not alter that baseline to satisfy this comparison. The inventory script reads
current local worktrees, so its counts can change as other authorized work proceeds.
This script does not alter those worktrees or establish unseen remote identities.

## build/model-resource-setup/check.py

```python
from pathlib import Path
import subprocess,sys,hashlib,json
p=Path('data/ver/eu/map.csv'); original=p.read_bytes(); old=b'0x00167CAC,0x00168488,0x001684F0,          ,U,f,,'; assert original.count(old)==1
try:
 p.write_bytes(original.replace(old,b'0x00167CAC,0x00168488,0x001684F0,          ,U,f,fn_00167CAC,'))
 c=subprocess.run([sys.executable,'tools/check.py','fn_00167CAC','--object','build/eu/obj/lib/al/src/ModelResourceSetup167CAC.o'],capture_output=True,text=True)
 print(c.stdout,c.stderr);Path('build/model-resource-setup/check-current.log').write_text(c.stdout+c.stderr)
finally:p.write_bytes(original)
assert p.read_bytes()==original
Path('build/model-resource-setup/check-result.json').write_text(json.dumps({'returncode':c.returncode,'stdout':c.stdout,'stderr':c.stderr,'map_restored':True,'map_sha256':hashlib.sha256(original).hexdigest()},indent=2))
```

## build/model-resource-setup/link.py

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
import csv,subprocess,os,json,hashlib
root=Path.cwd();out=root/'build/model-resource-setup';out.mkdir(exist_ok=True)
obj=root/'build/eu/obj/lib/al/src/ModelResourceSetup167CAC.o'
rows=[{k.strip():v.strip() for k,v in x.items()} for x in csv.DictReader((root/'data/ver/eu/map.csv').open())]
syms={x['Symbol']:int(x['Start'],16) for x in rows if x['Symbol']}
with obj.open('rb') as f:
 e=ELFFile(f);names=[s.name for s in e.get_section_by_name('.symtab').iter_symbols() if s['st_shndx']=='SHN_UNDEF' and s.name and not s.name.startswith('Lib$$')]
 lines=[]
 for n in names:
  a=int(n.split('_')[1],16) if n.startswith(('fn_','dat_')) else syms[n]
  kind = 'd' if n.startswith('dat_') else 'f'
  assert any(int(row['Start'],16)==a and kind in row['Type'] for row in rows), (n,hex(a))
  lines.append(f'0x{a:08X} {"D" if n.startswith("dat_") else "A"} {n}')
(out/'symbols.sym').write_text('#<SYMDEFS>#\n'+'\n'.join(lines)+'\n')
(out/'candidate.sct').write_text('CANDIDATE_LOAD 0x00167CAC\n{\n CANDIDATE_CODE 0x00167CAC\n {\n  ModelResourceSetup167CAC.o (i.fn_00167CAC, +FIRST)\n  ModelResourceSetup167CAC.o (+RO)\n }\n}\n')
cmd=[str(root/'data/compilers/wibo'),str(root/'data/compilers/4.1/791/bin/armlink.exe'),'--cpu=MPCore','--fpu=VFPv2','--arm_only','--no_exceptions','--inline','--datacompressor=off','--no_debug','--no_scanlib','--mangled','--symbols','--map','--entry=fn_00167CAC','--keep=fn_00167CAC',f'--scatter={out}/candidate.sct',f'--output={out}/candidate.axf',f'--list={out}/candidate.map',str(obj),str(out/'symbols.sym')]
p=subprocess.run(cmd,env={**os.environ,'TMP':'/tmp'},stdout=subprocess.PIPE,stderr=subprocess.STDOUT,text=True);print(p.stdout);assert p.returncode==0
print('Diagnostic root link only; canonical checker ran separately.')
```

## build/model-resource-setup/replay.py

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import *
from unicorn.arm_const import *
import struct,json,hashlib,random,time,csv
ROOT=Path.cwd();OUT=ROOT/'build/model-resource-setup';BINARY=(ROOT/'data/ver/eu/code.bin').read_bytes()
assert hashlib.sha256(BINARY).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
BASE=0x167cac;END=0x71000000;DB=0x1000000;STACK=0x70000000;SP=STACK+0x18000
with (OUT/'candidate.axf').open('rb') as f:
 e=ELFFile(f);CANDIDATE=next(s.data() for s in e.iter_sections() if s['sh_addr']==BASE and s['sh_size'])
R=[UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3,UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11,UC_ARM_REG_R12]
U=lambda *v:struct.pack('<'+'I'*len(v),*[x&0xffffffff for x in v])
ROWS=[{k.strip():v.strip() for k,v in row.items()} for row in csv.DictReader((ROOT/'data/ver/eu/map.csv').open())]
ALLOWED_STARTS=[0x25c334,0x28aa60,0x28ecac,0x28eed4,0x292158,0x2a9dc4,0x2aa23c]
ALLOWED=[(a,next(int(r['End'],16) for r in ROWS if int(r['Start'],16)==a)) for a in ALLOWED_STARTS]
MODELS={0x28cf4c:'thread',0x28cdb4:'mutex_acquire',0x28cd24:'mutex_release',0x2a92d8:'texture_bind',0x2adbdc:'model_bind',0x2b3940:'primitive_flush',0x40000000:'acquire_callback',0x40000004:'release_callback'}
class Engine:
 def __init__(self,case,candidate):
  self.case=case;self.events=[];self.instructions=0;self.returned=False;self.fault=None;self.coverage=set();self.entries={};self.cursor=DB;self.semantic=[]
  u=self.u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176);u.mem_map(0x100000,0x400000);u.mem_write(0x100000,BINARY)
  if candidate:u.mem_write(BASE,CANDIDATE)
  self.extent=len(CANDIDATE) if candidate else 2116
  u.mem_map(DB,0x200000);u.mem_map(STACK,0x20000);u.mem_map(END,0x1000);u.mem_map(0x40000000,0x1000)
  u.reg_write(UC_ARM_REG_C1_C0_2,0xf<<20);u.reg_write(UC_ARM_REG_FPEXC,1<<30)
  u.hook_add(UC_HOOK_CODE,self.hook);u.hook_add(UC_HOOK_MEM_INVALID,self.invalid)
  self.primary=self.reserve(4);self.secondary=self.reserve(4);self.context=self.reserve(16);self.resource=self.reserve(0xa0);self.secondary_resource=self.reserve(0xa0)
  self.w(self.primary,self.resource);self.w(self.secondary,self.secondary_resource)
  self.lock=self.reserve(0x40);vt=self.reserve(0x44);self.w(self.lock,vt);self.w(vt+0x3c,0x40000000,0x40000004);self.w(0x3e26cc,self.lock)
  self.w(self.lock+0x10,0x11111111 if case.get('recursive') else 0,case.get('recursive',0))
  self.models=[];self.materials=[];self.sets=[];self.primitives=[];self.attributes=[];self.textures=[]
  types=case.get('texture_types',[])
  for typ in types:
   t=self.reserve(0x40);self.w(t,typ);self.w(t+0x30,0xabad0000);self.textures.append(t)
  self.w(self.resource+0x24,len(types));self.rel(self.resource+0x28,self.dictionary(self.textures) if types or case.get('empty_dicts') else 0)
  for mi in range(case.get('models',1)):
   m=self.reserve(0xd0);self.models.append(m);shapes=[]
   for si in range(case.get('shapes',1)):
    shape=self.reserve(0x40);shapes.append(shape);self.w(shape,case.get('shape_type',0x10000001));subs=[]
    for subi in range(case.get('subs',1)):
     sub=self.reserve(0x14);subs.append(sub);sets=[]
     for pi in range(case.get('sets',1)):
      ps=self.reserve(0x18);sets.append(ps);self.sets.append(ps);prims=[]
      for pri in range(case.get('primitives',1)):
       p=self.reserve(0x20);prims.append(p);self.primitives.append(p);self.w(p+0x14,0xbaba0000);self.w(p+0x18,0x111000+16*pri,0x20+pri)
      self.w(ps,len(prims));self.rel(ps+4,self.array(prims));self.w(ps+0x10,case.get('set_flags',0))
     self.w(sub+0xc,len(sets));self.rel(sub+0x10,self.array(sets))
    self.w(shape+0x2c,len(subs));self.rel(shape+0x30,self.array(subs));attrs=[]
    for ai,typ in enumerate(case.get('attribute_types',[0x40000000,0,0x40000001])):
     if typ is None:attrs.append(0);continue
     a=self.reserve(0x14);attrs.append(a);self.attributes.append(a);self.w(a,typ);self.w(a+0x10,0xca110000+ai)
    self.w(shape+0x38,len(attrs));self.rel(shape+0x3c,self.array(attrs))
   self.w(m+0xc4,len(shapes));self.rel(m+0xc8,self.array(shapes));mats=[]
   for mati in range(case.get('materials',4)):
    mat=self.reserve(0x290);mats.append(mat);self.materials.append(mat);self.w(mat+0x168,mati%4);self.w(mat+0x28c,0xdddddddd)
    if mati!=case.get('no_shader_link',-1):
     link=self.reserve(0x20);self.rel(mat+0x284,link)
     if mati==case.get('existing_shader',-1):self.rel(link+0x1c,self.reserve(8))
   self.w(m+0xbc,len(mats));self.rel(m+0xc0,self.dictionary(mats) if mats or case.get('empty_dicts') else 0)
   meshes=[]
   for mesi in range(case.get('meshes',1) if shapes and case.get('sets',1) else 0):
    mesh=self.reserve(0x2c);meshes.append(mesh);self.w(mesh+0x18,mesi%len(shapes));self.w(mesh+0x28,mesi%case.get('sets',1))
   self.w(m+0xb4,len(meshes));self.rel(m+0xb8,self.array(meshes))
  self.w(self.resource+0x1c,len(self.models));self.rel(self.resource+0x20,self.dictionary(self.models) if self.models or case.get('empty_dicts') else 0)
  self.shader=self.reserve(16)
  if case.get('shader_dict',True):
   d=self.dictionary([0 if case.get('null_shader') else self.shader]);self.rel(self.secondary_resource+0x40,d)
   self.w(d+0xc,0xffffffff);self.u.mem_write(d+0x10,struct.pack('<HH',1,1));self.w(d+0x1c,0);self.u.mem_write(d+0x20,struct.pack('<HH',1,1))
   name=self.reserve(16);self.u.mem_write(name,(b'OtherShader' if case.get('missing_shader') else b'FastShader')+b'\0');self.rel(d+0x24,name)
  if case.get('fault')=='primary_wrapper':self.primary=0
  if case.get('fault')=='primary_resource':self.w(self.primary,0)
  if case.get('fault')=='model_dict':self.w(self.resource+0x20,0)
  if case.get('fault')=='secondary_resource':self.w(self.secondary,0)
  if case.get('fault')=='null_lock':self.w(0x3e26cc,0)
  if case.get('fault')=='null_shape' and self.models:self.w(self.resolve(self.models[0]+0xc8),0)
  if case.get('fault')=='null_material' and self.models:self.w(self.resolve(self.models[0]+0xc0)+0x28,0)
  self.saved=[0x44440000+i for i in range(8)]
 def r(self,a):return struct.unpack('<I',self.u.mem_read(a,4))[0]
 def w(self,a,*v):self.u.mem_write(a,U(*v))
 def rel(self,a,p):self.w(a,p-a if p else 0)
 def resolve(self,a):return (a+self.r(a))&0xffffffff if self.r(a) else 0
 def reserve(self,n):
  p=self.cursor;self.cursor+=(n+15)&~15;assert self.cursor<DB+0x200000;return p
 def array(self,values):
  if not values:return self.reserve(4) if self.case.get('empty_arrays') else 0
  p=self.reserve(len(values)*4)
  for i,v in enumerate(values):self.rel(p+4*i,v)
  return p
 def dictionary(self,values):
  p=self.reserve(0x1c+16*len(values))
  for i,v in enumerate(values):self.rel(p+0x28+16*i,v)
  return p
 def poison(self,result=0):
  u=self.u
  for i in [0,1,2,3,12]:u.reg_write(R[i],0xdead0000+i)
  for i in range(16):u.reg_write(UC_ARM_REG_S0+i,0x7fc01000+i)
  u.reg_write(R[0],result);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 def invalid(self,u,access,address,size,value,user):self.fault=(access,address,size);return False
 def hook(self,u,pc,size,user):
  self.instructions+=1
  if BASE<=pc<BASE+self.extent:self.coverage.add(pc-BASE)
  elif pc==END:self.returned=True;u.emu_stop();return
  elif pc in MODELS:
   name=MODELS[pc];args=[u.reg_read(r) for r in R[:3]];result=0
   if name=='thread':self.events.append((name,args[0]));result=0x11111111
   elif name in ['texture_bind','model_bind']:
    obj=self.r(args[0]);self.events.append((name,obj,args[1],args[2]));result=(self.textures.index(obj)+1) if name=='texture_bind' else 0x100<<(self.models.index(obj)%4)
    if args[2]==self.secondary_resource:result|=0x40000000
   elif name=='primitive_flush':self.events.append((name,args[1],args[2],*[self.r(args[0]+4*i) for i in range(4)]))
   else:self.events.append((name,args[0]))
   self.poison(result);return
  elif not any(a<=pc<b for a,b in ALLOWED):raise RuntimeError('unmodeled execution %08x'%pc)
  if pc in ALLOWED_STARTS:
   self.entries[hex(pc)]=self.entries.get(hex(pc),0)+1
   if pc==0x25c334:
    a,b,c=[u.reg_read(r) for r in R[:3]];self.events.append(('lookup',self.r(a),bytes(u.mem_read(b,c)).decode(),c))
   if pc==0x2aa23c:self.events.append(('bind',*[u.reg_read(r) for r in R[:3]]))
   if pc==0x2a9dc4:self.events.append(('finalize',self.r(u.reg_read(R[0])),u.reg_read(R[1])))
 def run(self):
  self.returned=False;self.fault=None
  u=self.u
  for i,v in enumerate(self.saved):u.reg_write(R[i+4],v)
  for i in range(8,16):u.reg_write(UC_ARM_REG_D0+i,0x5eed000000000000+i)
  u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,END)
  for i,v in enumerate([self.case.get('unused_r0',0xdefaced),self.primary,self.context,self.case.get('flags',0x12345)]):u.reg_write(R[i],v)
  self.w(SP,self.secondary if self.case.get('secondary',True) else 0,0xdecafbad)
  u.reg_write(UC_ARM_REG_FPSCR,self.case.get('fpscr',0))
  try:u.emu_start(BASE,END+4,count=200000)
  except UcError:
   if not self.fault:raise
  if self.returned:
   assert u.reg_read(UC_ARM_REG_SP)==SP,'sp'
   assert [u.reg_read(R[i+4]) for i in range(8)]==self.saved,'callee-saved integer registers'
   assert [u.reg_read(UC_ARM_REG_D0+i) for i in range(8,16)]==[0x5eed000000000000+i for i in range(8,16)],'callee-saved VFP registers'
  return {'memory':bytes(u.mem_read(DB,self.cursor-DB)),'global':bytes(u.mem_read(0x3e26cc,4)),'events':self.events,'fault':self.fault,'returned':self.returned,'result':u.reg_read(R[0]) if self.returned else None,'fpscr':u.reg_read(UC_ARM_REG_FPSCR)}
def cases():
 result=[]
 def add(label,**kw):result.append(dict(label=label,**kw))
 for models in [0,1,2,3]:
  for sec in [False,True]:
   for flags in [0,0xffffffff,0x2000000,0x12345678]:add('base-%d-%d-%x'%(models,sec,flags),models=models,secondary=sec,flags=flags,texture_types=[0x20000009,0x20000011,0x20000001,0])
 for field in ['shapes','subs','sets','primitives','materials','meshes']:
  for value in [0,1,2,3]:add(field+str(value),**{field:value})
 for field in ['empty_dicts','empty_arrays','shader_dict','missing_shader','null_shader','secondary']:
  for value in [False,True]:add(field+str(value),**{field:value})
 for typ in [0,1,0x10000000,0x10000001,0xffffffff]:add('shape-type'+str(typ),shape_type=typ,attribute_types=[None,0,1,0x40000000,0xffffffff])
 for i in range(4):
  add('existing-shader'+str(i),existing_shader=i);add('absent-link'+str(i),no_shader_link=i)
 for flags in [0,1,2,3,0x80000000,0xffffffff]:add('primitive-flags'+str(flags),set_flags=flags,meshes=3)
 for fpscr in [0,0x400000,0x800000,0xc00000,0x1000000,0x2000000,0x3000000]:add('fpscr'+str(fpscr),fpscr=fpscr)
 for unused in [0,1,0xffffffff]:add('unused-r0'+str(unused),unused_r0=unused)
 for rec in [1,2,17]:add('recursive-lock'+str(rec),recursive=rec)
 rng=random.Random(167)
 for i in range(60):add('random'+str(i),models=rng.randrange(4),shapes=rng.randrange(4),subs=rng.randrange(3),sets=rng.randrange(3),primitives=rng.randrange(4),materials=rng.randrange(5),meshes=rng.randrange(4),secondary=bool(rng.randrange(2)),flags=rng.getrandbits(32),existing_shader=rng.randrange(-1,4),no_shader_link=rng.randrange(-1,4),set_flags=rng.getrandbits(32),shape_type=rng.choice([0,0x10000001,0xffffffff]),texture_types=[rng.choice([0x20000009,0x20000011,0xdeadbeef]) for _ in range(rng.randrange(5))])
 for fault in ['primary_wrapper','primary_resource','model_dict','secondary_resource','null_lock','null_shape','null_material']:add('fault-'+fault,fault=fault)
 return result
if __name__=='__main__':
 began=time.monotonic();reports=[];cov=set();total=0;events=0;maxins=0;faults=[];entries={}
 for case in cases():
  a=Engine(case,False);b=Engine(case,True);ar=a.run();br=b.run();same=ar==br
  if same and ar['returned']:
   ar2=a.run();br2=b.run();assert ar2==br2 and ar2['returned'],case['label']+' repeated call'
   same=True
  cov|=a.coverage;maxins=max(maxins,a.instructions,b.instructions)
  for k,v in a.entries.items():entries[k]=entries.get(k,0)+v
  item={'case':case,'same':same,'original_fault':ar['fault'],'candidate_fault':br['fault'],'original_returned':ar['returned'],'candidate_returned':br['returned'],'original_instructions':a.instructions,'candidate_instructions':b.instructions}
  if ar['fault'] or br['fault']:faults.append(item)
  else:
   if not same:
    item['original_events']=ar['events'];item['candidate_events']=br['events'];item['differing_offsets']=[i for i,(x,y) in enumerate(zip(ar['memory'],br['memory'])) if x!=y];print(json.dumps(item,indent=2));raise AssertionError(case['label'])
   assert ar['returned'] and br['returned'];total+=2*(len(ar['memory'])+len(ar['global']));events+=len(ar2['events'])
  reports.append(item)
 out={'returning_fixture_pairs':len(reports)-len(faults),'returning_invocation_pairs':2*(len(reports)-len(faults)),'compared_bytes':total,'compared_events':events,'fault_diagnostics':faults,'max_instructions':maxins,'original_direct_helper_entries':entries,'original_coverage_offsets':sorted(cov),'seconds':time.monotonic()-began,'cases':reports,'source_sha256':hashlib.sha256((ROOT/'lib/al/src/ModelResourceSetup167CAC.cpp').read_bytes()).hexdigest()}
 (OUT/'replay-results.json').write_text(json.dumps(out,indent=2));print(json.dumps({k:v for k,v in out.items() if k not in ['cases','original_coverage_offsets']},indent=2))
```

## build/model-resource-setup/preserve.py

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection
import json,hashlib,subprocess,collections
root=Path.cwd();base=root.parent/'mario-main861';out=root/'build/model-resource-setup'
def normalized(p):
 e=ELFFile(open(p,'rb'));sections=[];symbols=[]
 def symrec(s):
  n=s['st_shndx'];sec=e.get_section(n).name if isinstance(n,int) else n
  name=s.name.replace(str(root),'<repo>').replace(str(base),'<repo>') if s['st_info']['type']=='STT_FILE' else s.name
  return [name,s['st_info']['type'],s['st_info']['bind'],s['st_other']['visibility'],sec,s['st_value'],s['st_size']]
 for sec in e.iter_sections():
  if isinstance(sec,RelocationSection):
   tab=e.get_section(sec['sh_link']);sections.append([sec.name,[(r['r_offset'],r['r_info_type'],symrec(tab.get_symbol(r['r_info_sym']))) for r in sec.iter_relocations()]])
  elif sec['sh_type'] not in ('SHT_SYMTAB','SHT_STRTAB','SHT_NULL'):
   sections.append([sec.name,sec['sh_type'],sec['sh_flags'],sec['sh_addralign'],sec['sh_entsize'],hashlib.sha256(sec.data().replace(str(root).encode(),b'<repo>').replace(str(base).encode(),b'<repo>') if sec.name=='.comment' else sec.data()).hexdigest()])
 tab=e.get_section_by_name('.symtab')
 for s in tab.iter_symbols():
  symbols.append(symrec(s))
 return {'sections':sections,'symbols':symbols}
records=[];inputs={}
old_objects={p.relative_to(base/'build/eu/obj') for p in (base/'build/eu/obj').rglob('*.o')}
new_objects={p.relative_to(root/'build/eu/obj') for p in (root/'build/eu/obj').rglob('*.o')}
assert new_objects-old_objects=={Path('lib/al/src/ModelResourceSetup167CAC.o')}
assert not old_objects-new_objects
sealed={}
for name in subprocess.check_output(['git','ls-files'],cwd=base,text=True).splitlines():
 p=base/name;q=root/name
 if p.is_file():
  assert q.is_file(),name
  a=hashlib.sha256(p.read_bytes()).hexdigest();b=hashlib.sha256(q.read_bytes()).hexdigest();assert a==b,name;sealed[name]=a
(out/'baseline-tracked-inputs.json').write_text(json.dumps(sealed,indent=2,sort_keys=True))
for p in sorted((root/'build/eu/obj').rglob('*.o')):
 rel=p.relative_to(root/'build/eu/obj')
 if rel.parts[0] not in ('Game','lib') or p.stem=='ModelResourceSetup167CAC':continue
 q=base/'build/eu/obj'/rel;pa=json.loads(p.with_suffix('.provenance.json').read_text());pb=json.loads(q.with_suffix('.provenance.json').read_text())
 assert pa['inputs']==pb['inputs'],str(rel)+' inputs'
 for key,value in pa['inputs'].items():
  assert hashlib.sha256((root/key).read_bytes()).hexdigest()==value
  assert hashlib.sha256((base/key).read_bytes()).hexdigest()==value
  inputs[key]=value
 assert pa['compiler_sha256']==pb['compiler_sha256'],str(rel)+' compiler'
 assert [x.replace(str(root),'<repo>') for x in pa['command']]==[x.replace(str(base),'<repo>') for x in pb['command']],str(rel)+' command'
 a=normalized(p);b=normalized(q)
 if a!=b:
  (out/'preserve-failed.json').write_text(json.dumps({'object':str(rel),'current':a,'baseline':b},indent=2));raise AssertionError(str(rel)+' ELF')
 records.append({'object':str(rel),'current_object_sha256':hashlib.sha256(p.read_bytes()).hexdigest(),'base_object_sha256':hashlib.sha256(q.read_bytes()).hexdigest(),'normalized_elf_sha256':hashlib.sha256(json.dumps(a,sort_keys=True).encode()).hexdigest(),'compiler_sha256':pa['compiler_sha256']})
assert len(records)==179
# The generated scaffold gains only the six expected address aliases.
stub=Path('build/eu/obj/build/eu/split/stubs.o');a=normalized(root/stub);b=normalized(base/stub)
sa={x[0]:x for x in a['sections'] if x[0] not in ('.debug_frame','.rel.debug_frame')};sb={x[0]:x for x in b['sections'] if x[0] not in ('.debug_frame','.rel.debug_frame')}
expected_functions={'i.'+x for x in ['fn_00167CAC','fn_0025C334','fn_0028ECAC','fn_0028EED4','fn_002A9DC4','fn_002AA23C']}
expected_data={'.sdata_'+x for x in ['dat_003A2D40','dat_003E26CC']}
expected=expected_functions|expected_data
assert set(sa)-set(sb)==expected
assert not set(sb)-set(sa)
for n in sb:assert sa[n]==sb[n],n
def debug_frames(path):
 elf=ELFFile(open(path,'rb'));frames={}
 for sec in elf.iter_sections():
  if isinstance(sec,RelocationSection) and sec.name=='.rel.debug_frame':
   tab=elf.get_section(sec['sh_link']);rels=[];owner=None
   for rel in sec.iter_relocations():
    sym=tab.get_symbol(rel['r_info_sym']);name=sym.name
    if name.startswith('i.'):owner=name
    if name.startswith('__ARM_grp_.debug_frame$'):name='<frame-CIE>'
    ndx=sym['st_shndx'];target=elf.get_section(ndx).name if isinstance(ndx,int) else ndx
    rels.append([rel['r_offset'],rel['r_info_type'],name,target,sym['st_value'],sym['st_size']])
   assert owner and owner not in frames
   frame=elf.get_section(sec['sh_info']);frames[owner]=[hashlib.sha256(frame.data()).hexdigest(),rels]
 return frames
fa=debug_frames(root/stub);fb=debug_frames(base/stub)
assert set(fa)-set(fb)==expected_functions
assert not set(fb)-set(fa)
for n in fb:assert fa[n]==fb[n],n+' debug frame'
ca=collections.Counter(json.dumps(x) for x in a['symbols']);cb=collections.Counter(json.dumps(x) for x in b['symbols']);assert not cb-ca
added=[json.loads(x) for x in (ca-cb).elements()]
debug_symbols={x[0] for x in added if x[0].startswith('__ARM_grp_.debug_frame$')}
assert len(debug_symbols)==len(expected_functions)
assert all(x[4] in expected or x[0] in debug_symbols for x in added),added
assert {x[0] for x in added if x[4] not in expected}==debug_symbols
assert {x[0] for x in added if x[1]=='STT_FUNC'}=={x[2:] for x in expected_functions}
oldsource=(base/'build/eu/split/stubs.c').read_text();newsource=(root/'build/eu/split/stubs.c').read_text()
for n in sorted(expected_functions):
 sym=n[2:];address=sym.split('_')[1];kind='STUB_G'
 addition=f'/* Scaffold alias for unnamed function at 0x{address}. */\n{kind}({sym});\n';assert newsource.count(addition)==1;newsource=newsource.replace(addition,'')
for symbol,size in [('dat_003A2D40',32),('dat_003E26CC',4)]:
 address=symbol.split('_')[1];addition=f'/* Zero-filled scaffold data at 0x{address}, not reconstructed data. */\n__weak __attribute__((section(".sdata_{symbol}"), aligned(4))) unsigned char {symbol}[{size}] = {{0}};\n';assert newsource.count(addition)==1;newsource=newsource.replace(addition,'')
assert oldsource==newsource
psa=json.loads((root/stub).with_suffix('.provenance.json').read_text());psb=json.loads((base/stub).with_suffix('.provenance.json').read_text())
assert psa['compiler_sha256']==psb['compiler_sha256']
assert [x.replace(str(root),'<repo>') for x in psa['command']]==[x.replace(str(base),'<repo>') for x in psb['command']]
assert set(psa['inputs'])==set(psb['inputs'])
for key in psa['inputs']:
 assert hashlib.sha256((root/key).read_bytes()).hexdigest()==psa['inputs'][key]
 assert hashlib.sha256((base/key).read_bytes()).hexdigest()==psb['inputs'][key]
 if key!='build/eu/split/stubs.c':assert psa['inputs'][key]==psb['inputs'][key]
stub_delta={'all_prior_sections_symbols_unchanged':True,'old_debug_frame_count':len(fb),'new_debug_frame_count':len(fa),'debug_normalization':'Compare every CFI byte and resolved relocation per function; compiler-generated frame-CIE ordinal names and section-table indices normalize only to that per-function association','added_sections':sorted(expected),'added_symbols':added,'old_generated_source_sha256':psb['inputs']['build/eu/split/stubs.c'],'new_generated_source_sha256':psa['inputs']['build/eu/split/stubs.c']}
(out/'generated-stub-delta.json').write_text(json.dumps(stub_delta,indent=2))
assert (root/'build/eu/code.bin').read_bytes()==(base/'build/eu/code.bin').read_bytes()
result={'base':'86104d96a7f570383bbfefc5fffad998e134496c','object_count':len(records),'input_count':len(inputs),'all_baseline_tracked_files':len(sealed),'total_objects_with_addition':len(new_objects),'prior_canonical_cpp_objects':sum(1 for x in old_objects if x.parts[0] in ('Game','lib')),'all_equal':True,'generated_stub_delta_verified':True,'compact_image_equal':True,'excluded_difference':'Only repository prefix in STT_FILE absolute source path and in non-allocated .comment, and resulting raw symbol/string table indices; all other symbols, section data, attributes and resolved relocations compare equal','objects':records,'inputs':inputs}
(out/'preservation.json').write_text(json.dumps(result,indent=2));print(json.dumps({k:v for k,v in result.items() if k not in ('objects','inputs')},indent=2))
```

## build/model-resource-setup/audit.py

```python
from pathlib import Path
import subprocess,re,json,hashlib,datetime,struct
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection
root=Path.cwd();out=root/'build/model-resource-setup'
h=lambda p:hashlib.sha256(Path(p).read_bytes()).hexdigest()
source=root/'lib/al/src/ModelResourceSetup167CAC.cpp';binary=(root/'data/ver/eu/code.bin').read_bytes()
paths=[];heads={}
for block in subprocess.check_output(['git','worktree','list','--porcelain'],text=True).strip().split('\n\n'):
 lines=block.splitlines();p=Path(lines[0].removeprefix('worktree '));head=next(x[5:] for x in lines if x.startswith('HEAD '));paths.append(p);heads[str(p)]=head
pattern=re.compile(r'(?i)(?:fn_00(?:167cac|25c334|28ecac|28eed4|2a9dc4|2aa23c)|dat_00(?:3e26cc|3a2d40))')
hits=[];seen=set();total=0
for repo in paths:
 names=subprocess.check_output(['git','ls-files','-co','--exclude-standard','-z','--','Game','lib'],cwd=repo).decode().split('\0')
 for name in names:
  if not name.endswith(('.cpp','.h','.hpp','.cc','.c')):continue
  p=repo/name
  if not p.is_file():continue
  content=p.read_bytes();digest=hashlib.sha256(content).hexdigest();total+=1
  if digest in seen:continue
  seen.add(digest)
  text=content.decode(errors='replace')
  matches=[{'line':i,'text':line.strip()} for i,line in enumerate(text.splitlines(),1) if pattern.search(line)]
  if matches:hits.append({'path':str(p),'sha256':digest,'matches':matches})
obj=root/'build/eu/obj/lib/al/src/ModelResourceSetup167CAC.o'
with obj.open('rb') as f:
 e=ELFFile(f);tab=e.get_section_by_name('.symtab');defs=[{'name':s.name,'size':s['st_size'],'section':e.get_section(s['st_shndx']).name} for s in tab.iter_symbols() if s['st_info']['type']=='STT_FUNC' and isinstance(s['st_shndx'],int) and s['st_size']]
 sec=e.get_section_by_name('i.fn_00167CAC');size=sec['sh_size'];alloc=[{'name':s.name,'bytes':s['sh_size'],'sha256':hashlib.sha256(s.data()).hexdigest()} for s in e.iter_sections() if s['sh_flags']&2 and s['sh_size']]
 imports=[s.name for s in tab.iter_symbols() if s['st_shndx']=='SHN_UNDEF' and s.name and not s.name.startswith('Lib$$')]
 rels=[]
 for sec in e.iter_sections():
  if isinstance(sec,RelocationSection) and e.get_section(sec['sh_info']).name=='i.fn_00167CAC':
   for r in sec.iter_relocations():rels.append({'offset':r['r_offset'],'type':r['r_info_type'],'symbol':tab.get_symbol(r['r_info_sym']).name})
checks=['tools/check.py','tools/low/checkExactBytes.py','tools/low/buildProvenance.py','data/config.json','data/ver/eu/map.csv','make.py','data/compilers/4.1/791/bin/armcc.exe','data/compilers/4.1/791/bin/armlink.exe','data/compilers/wibo']
checks += ['build/model-resource-setup/'+n for n in ['check.py','link.py','replay.py','preserve.py','audit.py']]
result={'timestamp_utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),'base':'86104d96a7f570383bbfefc5fffad998e134496c','source_sha256':h(source),'object_sha256':h(obj),'provenance_sha256':h(obj.with_suffix('.provenance.json')),'candidate_complete_section_bytes':size,'original_complete_bytes':2116,'original_interval_sha256':hashlib.sha256(binary[0x67cac:0x684f0]).hexdigest(),'original_pool_sha256':hashlib.sha256(binary[0x68488:0x6849c]).hexdigest(),'original_input_sha256':hashlib.sha256(binary).hexdigest(),'definitions':defs,'allocated_sections':alloc,'imports':imports,'root_relocations':rels,'tool_input_hashes':{p:h(root/p) for p in checks},'declaration_audit':{'working_trees':len(paths),'heads':heads,'physical_source_headers':total,'distinct_source_headers':len(seen),'hits':hits},'baseline_report_sha256':h(root.parent/'mario-main861/build/dot-baseline-861/report.json')}
(out/'audit.json').write_text(json.dumps(result,indent=2));print(json.dumps({k:v for k,v in result.items() if k not in ['declaration_audit','root_relocations','tool_input_hashes']},indent=2));print('declaration hits',len(hits),'trees',len(paths),'unique sources',len(seen))
```
