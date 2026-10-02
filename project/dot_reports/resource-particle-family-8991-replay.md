# Reproduce the frozen resource/particle family

Use the exact source/report branch based on 8991 and the owner's private EU inputs with the approved ARMCC/wibo/venv installed. No input, compiler, object, replay image or binary is included in this report. Run from the checkout root. All source/header inputs must already be committed. Each script below is complete; save it at its indicated ignored build/ path. Run the gate and canonical checks sequentially because the unchanged checker writes the map. No script changes original row types, boundaries, pools or import identities.

```sh
. ./development_environment.sh
export DEVKITARM=/usr
mkdir -p build/family build/research298c58 build/particle
sha256sum data/ver/eu/code.bin data/ver/eu/exh.bin
python - <<'PY_MANIFEST'
from pathlib import Path
import json
Path('build/family/manifest.json').write_text(json.dumps({'prior_checkpoint':'8991bec8cb27ff1a2dac0003f059d270a0b17493','candidates':[]}))
PY_MANIFEST
python tools/acceptance_batch.py build/family/manifest.json --output build/family/gate8991
```

Use an absent output directory for the gate. It performs the normal clean make.py eu -ca build and every accepted-root/actual-definition canonical check. Its accepted:true concerns an empty candidate preservation gate only.

Save the following names-only map preparation as build/family/prepare_map.py. Execute it after the full gate completes. It does not change the live map.

```python
from pathlib import Path
out=Path('build/family');p=Path('data/ver/eu/map.csv');backup=out/'original-map.csv'
assert not backup.exists(), 'Use a fresh output directory or verify the existing backup'
backup.write_bytes(p.read_bytes());lines=p.read_text().splitlines()
names={0x298c58:'fn_00298C58',0x2a451c:'fn_002A451C',0x2a4434:'fn_002A4434',0x3d8384:'dat_003D8384',0x3ba150:'dat_003BA150'}
for i,line in enumerate(lines):
 c=line.split(',')
 try:a=int(c[0],16)
 except ValueError:continue
 if a in names:
  assert not c[6].strip();c[6]=names[a];lines[i]=','.join(c)
(out/'named-map.csv').write_text('\n'.join(lines)+'\n')
```

```sh
python build/family/prepare_map.py
PYTHONPATH=. python build/family/canonical.py
PYTHONPATH=. python build/family/link_replay.py
python build/research298c58/replay.py
python build/family/synthetic_caller_replay.py
PARTICLE_REPLAY_IMAGE=replay-combined.axf python build/particle/replay.py
python build/particle/replay_helpers.py
PYTHONPATH=. python build/family/compare_codegen.py
python build/family/verify_results.py
sha256sum data/ver/eu/map.csv data/ver/eu/code.bin
```

The canonical script expects four nonzero checker statuses, records their complete evidence, then restores the original map in finally. The replay must retain the six null-attribute failures; they are expected known differences, never returning successes. The supplemental verification checks the exact split and register canaries.

The codegen comparison requires the two independent source-package checkouts at sibling paths mario-resource-298c58 and mario-particle-shape-2a451c, with their frozen sources and independently generated normal project objects. Their local heads/source hashes are in the main report. Reproduce each original package's ordinary committed-source clean build first if those objects are absent. No object is copied into the combined build, and both sides pass the project provenance verifier. This comparison does not substitute for the fresh 717-root gate.

## build/family/canonical.py

```python
from pathlib import Path
import subprocess,sys,json,hashlib
from tools.low.checkExactBytes import check_exact_bytes
from tools.low.buildProvenance import verify_build_output
root=Path.cwd();out=root/'build/family';p=root/'data/ver/eu/map.csv';original=p.read_bytes();result=[]
assert original==(out/'original-map.csv').read_bytes()
try:
 p.write_bytes((out/'named-map.csv').read_bytes())
 for name,stem in [('fn_00298C58','AddressResourceFactory'),('fn_002A451C','retail_ParticleBinding'),('_ZN2nw3gfx13ParticleShape19AddVertexStreamSizeEjiii','retail_ParticleShapeSize'),('_ZN2nw3gfx13ParticleShape18AddVertexParamSizeEjii','retail_ParticleShapeSize')]:
  obj=root/f'build/eu/obj/lib/CtrSDK/sources/{stem}.o'
  c=subprocess.run([sys.executable,'tools/check.py',name,'--object',str(obj)],capture_output=True,text=True)
  (out/(name+'-canonical.log')).write_text(c.stdout+c.stderr)
  evidence=check_exact_bytes(name,obj,'eu',verify_build_output(obj)['compiler'],out/('canonical-'+name))
  result.append({'symbol':name,'returncode':c.returncode,'output':c.stdout,'error':c.stderr,'evidence':evidence})
finally:
 p.write_bytes(original)
(out/'canonical-results.json').write_text(json.dumps({'results':result,'restored_map_sha256':hashlib.sha256(p.read_bytes()).hexdigest()},indent=2)+'\n')
for r in result:print(r['symbol'],r['returncode'],r['output'].strip(),r['evidence'].get('evidence',{}).get('different_bytes'))
```

## build/family/link_replay.py

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
import subprocess
from tools.low.checkExactBytes import _resolve_symbol,_read_map
root=Path.cwd();out=root/'build/family';names=['AddressResourceFactory','retail_ParticleBinding','retail_ParticleShapeSize'];objects=[root/f'build/eu/obj/lib/CtrSDK/sources/{n}.o' for n in names]
rows=_read_map(out/'named-map.csv');lines=['#<SYMDEFS>#'];defined=set();seen=set()
for obj in objects:
 with obj.open('rb') as f:
  for s in ELFFile(f).get_section_by_name('.symtab').iter_symbols():
   if s['st_shndx']!='SHN_UNDEF':defined.add(s.name)
for obj in objects:
 with obj.open('rb') as f:
  for s in ELFFile(f).get_section_by_name('.symtab').iter_symbols():
   if s['st_shndx']=='SHN_UNDEF' and s.name and s.name not in defined|seen and not s.name.startswith('Lib$$'):
    address,kind,row=_resolve_symbol(s,None,rows);lines.append(f'0x{address:08X} {kind} {s.name}');seen.add(s.name)
sym=out/'imports.sym';sym.write_text('\n'.join(lines)+'\n')
for stem,entry in [('research298c58/replay','fn_00298C58'),('particle/replay-combined','fn_002A451C')]:
 subprocess.run([str(root/'data/compilers/wibo'),str(root/'data/compilers/4.0/902/bin/armlink.exe'),'--cpu=MPCore','--fpu=VFPv2','--arm_only','--no_exceptions','--no_debug','--no_scanlib','--mangled','--symbols','--map','--ro_base=0x600000','--entry='+entry,'--keep=fn_00298C58','--keep=fn_002A451C','--output='+str(root/'build'/f'{stem}.axf'),'--list='+str(root/'build'/f'{stem}.map')]+[str(p) for p in objects]+[str(sym)],check=True)
```

## build/research298c58/replay.py

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import *
from unicorn.arm_const import *
import struct,json,hashlib,time,re
from collections import Counter
ROOT=Path.cwd(); OUT=ROOT/'build/research298c58'; CODE=(ROOT/'data/ver/eu/code.bin').read_bytes()
ALLOC=0x7f0000;FREE=0x7f0010;STOP=0x7f1000;RES=0x800000;ALLOCATOR=0x810000;OPT=0x820000;OWNER=0x830000;BINDRES=0x840000;ARGS=0x850000;CMD=0x870000;BIND=0x890000
R=[UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3]; SAVED=[UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11,UC_ARM_REG_SP]
FAMILY={0x298c58:"fn_00298C58",0x2a451c:"fn_002A451C",0x22f590:"_ZN2nw3gfx13ParticleShape18AddVertexParamSizeEjii",0x22f554:"_ZN2nw3gfx13ParticleShape19AddVertexStreamSizeEjiii"}
SEG=[]
with (OUT/'replay.axf').open('rb') as f:
 e=ELFFile(f)
 CAND=e['e_entry']
 SYMBOLS={name:int(re.search(r'^\s*'+re.escape(name)+r'\s+(0x[0-9a-fA-F]+)\s+ARM Code', (OUT/'replay.map').read_text(), re.M).group(1),16) for name in FAMILY.values()}
 for s in e.iter_sections():
  if s['sh_flags']&2:SEG.append((s['sh_addr'],s.data()))

def run(candidate,case,fpscr=0):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176)
 u.mem_map(0x100000,0x400000);u.mem_write(0x100000,CODE);u.mem_map(0x600000,0x10000)
 for a,b in SEG:u.mem_write(a,b)
 u.mem_map(0x700000,0x100000);u.mem_write(0x700000,b'\xa6'*0x100000)
 u.mem_map(0x800000,0x200000);u.mem_map(0x1000000,0x100000);u.mem_write(0x900000,b'\xc3'*0x100000)
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def byte(a,v):u.mem_write(a,bytes([v]))
 put(ALLOCATOR,ALLOCATOR+0x100);put(ALLOCATOR+0x108,ALLOC);put(ALLOCATOR+0x10c,FREE)
 put(RES,case.get('flags',0x40000001));put(RES+0x30,BINDRES-RES-0x30);put(RES+0x44,0x880000-RES-0x44);put(0x880000,0x80000000)
 list24=case.get('list24',[]);list28=case.get('list28',[])
 for n,items in enumerate([list24,list28]):
  field=RES+0x34+n*8;array=RES+0x100+n*0x100;put(field,len(items));put(field+4,array-field-4)
  for i,item in enumerate(items):
   a=RES+0x1000+n*0x2000+i*0x100;put(array+i*4,a-array-i*4)
   put(a,item.get('type',0));byte(a+4,item.get('copy',0));put(a+8,item.get('binding',-1));put(a+0x10,item.get('word10',0))
   if item.get('nested'):
    put(a+0xc,0x40-0xc)
    if item['nested']==2:put(a+0x60,0x10)
 put(OPT+0x10,case.get('capacity24',0));put(OPT+0x14,case.get('capacity28',0))
 put(OPT,case.get('option0',0));put(OPT+4,0);put(OPT+8,0);put(OPT+12,0)
 put(OWNER+0x10,OWNER);put(OWNER+0x14,0 if case.get('owner_no_allocator') else ALLOCATOR)
 put(OWNER+0x18,OWNER+0x100);put(OWNER+0x1c,OWNER+0x100+4*case.get('owner_size',0));put(OWNER+0x20,case.get('owner_mode',1)<<30|case.get('owner_capacity',0))
 for i in range(case.get('owner_size',0)):put(OWNER+0x100+i*4,OWNER+0x500+i*0x100)
 if case.get('parent'):put(OWNER+0xc,OWNER+0x1000);put(OWNER+0x1000+0xc,OWNER+0x1000 if case.get('parent_cycle') else 0)
 # Two command buffers, output packets and an empty original binding resource.
 for field,offset in [(0xc,0),(0x10,0x1000),(0x18,0x2000),(0x1c,0x3000),(0x24,0x4000)]:put(CMD+field,0x1000000+offset)
 put(CMD+0x34,0);put(BINDRES,case.get('binding_count',0));put(BINDRES+4,0);put(ARGS,0)
 byte(BIND+0xd0,case.get('reverse',0))
 for i in range(8):byte(BIND+0xd1+i,1 if case.get('enabled',True) else 0);put(BIND+0xe4+8*i,0x2000000+i*0x100);put(BIND+0xe8+8*i,0x3000000+i*0x100)
 for i,r in enumerate(SAVED[:-1]):u.reg_write(r,0xa4000000+i)
 for i in range(8):u.reg_write(UC_ARM_REG_D8+i,0x7fe1234500000000+i)
 u.reg_write(UC_ARM_REG_SP,0x7e0000);u.reg_write(UC_ARM_REG_LR,STOP);put(0x7e0000,ARGS);put(0x7e0004,CMD)
 for r,v in zip(R,[OWNER if case.get('owner') else 0,0 if case.get('null_resource') else RES,OPT,ALLOCATOR]):u.reg_write(r,v)
 u.reg_write(UC_ARM_REG_C1_C0_2,0xf00000);u.reg_write(UC_ARM_REG_FPEXC,0x40000000);u.reg_write(UC_ARM_REG_FPSCR,fpscr)
 entries=Counter();bodies=Counter();events=[];allocs=[];freed=set();cov=set();callees=set();bad=[];steps=0;nalloc=0
 def finish(value=0):
  for r in R+[UC_ARM_REG_R12]:u.reg_write(r,0xa5a5a5a5)
  u.reg_write(UC_ARM_REG_R0,value&0xffffffff);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 def hook(u,a,z,d):
  nonlocal nalloc,steps
  steps+=1
  for original,name in FAMILY.items():
   entry=SYMBOLS[name] if candidate else original
   if a==entry:
    entries[name]+=1
    if not (name=="fn_002A451C" and case.get("model_binding")):bodies[name]+=1
  if 0x298c58<=a<0x2997c0:cov.add(a)
  if a in (0x2997c8,0x2b3f70,0x2998c0,0x29b57c,0x29afe8,0x2a451c,0x29e620,0x231ef0):callees.add(a)
  args=[u.reg_read(r) for r in R]
  if a==ALLOC:
   nalloc+=1;memory=0x900000+(nalloc-1)*0x10000
   if nalloc==case.get('fail_alloc'):memory=0
   events.append(['alloc',args[:3],memory]);allocs.append((memory,args[1]));finish(memory)
  elif a==FREE:events.append(['free',args[:2]]);freed.add(args[1]);finish()
  elif a==0x2b3f70 and 'init_result' in case:events.append(['init_model',args[:2]]);finish(case['init_result'])
  elif a==(SYMBOLS['fn_002A451C'] if candidate else 0x2a451c) and case.get('model_binding'):
   events.append(['binding_model',args+[get(u.reg_read(UC_ARM_REG_SP))]])
   finish(0 if case.get('binding_fail') else BIND)
  elif a==0x28e280:events.append(['hardware_flush',args[0]]);finish()
  elif a==0x10b2bc:events.append(['hardware_address',args[0]]);finish(args[0]+0x10000000)
 def invalid(u,access,address,size,value,data):bad.append([access,address,size]);return False
 u.hook_add(UC_HOOK_CODE,hook);u.hook_add(UC_HOOK_MEM_INVALID,invalid)
 error=None
 try:u.emu_start(CAND if candidate else 0x298c58,STOP,count=500000)
 except UcError as e:error=str(e)
 ret=u.reg_read(UC_ARM_REG_R0);terminated=u.reg_read(UC_ARM_REG_PC)==STOP
 ranges=[(0x800000,0x90000),(0x1000000,0x10000)]+[(a,n) for a,n in allocs if a and a not in freed]
 data=[]
 for a,n in ranges:
  b=bytearray(u.mem_read(a,n))
  # Indeterminate C++ struct padding is excluded. Field values remain compared.
  if a==0x900000 and terminated and ret:
   for field,stride in [(0x58,24),(0x68,28)]:
    begin=get(a+field);end=get(a+field+4)
    for entry in range(begin,end,stride):
     off=entry-a
     if 0<=off<len(b)-3:b[off+1:off+4]=b'\0'*3
  data.append(bytes(b))
 result={'entries':dict(entries),'bodies':dict(bodies),'return':ret,'terminated':terminated,'error':error,'invalid':bad,'events':events,'memory':[(a,n,hashlib.sha256(b).hexdigest()) for (a,n),b in zip(ranges,data)],'saved':[u.reg_read(r) for r in SAVED]+[u.reg_read(UC_ARM_REG_D8+i) for i in range(8)],'fpscr':u.reg_read(UC_ARM_REG_FPSCR),'pc':u.reg_read(UC_ARM_REG_PC),'steps':steps,'callees':[hex(a) for a in sorted(callees)]}
 return result,cov,data

def suite():
 cases=[{}, {'fail_alloc':1},{'fail_alloc':2},{'fail_alloc':3},{'fail_alloc':4}, {'init_result':-1}, {'init_result':-2147483648}, {'init_result':1}, {'init_result':2147483647}]
 cases += [{'flags':x} for x in [0,1,0x40000000,0xffffffff]]+[{'null_resource':1}]
 cases += [{'owner':1,'owner_size':size,'owner_capacity':capacity,'owner_mode':mode} for size,capacity in [(0,0),(0,1),(1,1),(2,2),(2,4)] for mode in [0,1,2,3]]
 cases += [{'owner':1,'owner_no_allocator':1}, {'owner':1,'parent':1}, {'owner':1,'parent':1,'parent_cycle':1}]
 cases += [{'capacity24':a,'capacity28':b} for a,b in [(1,1),(5,7),(7,2),(0,4)]]
 # Execute every direct retail callee on both sides for these data variants.
 for n in [0,1,2,5]:
  cases += [{'list24':[{'binding':-1}]*n}, {'list28':[{'binding':-1}]*n}, {'list24':[{}]*n,'list28':[{}]*n}]
 for tag in [0,0x80000,0x100000,0x200000,0x400000,0x800000,0x1000000,0x2000000,0x4000000,0x8000000,0x10000000,0x20000000,0x40000000,0x80000000]:
  cases += [{'list24':[{'type':tag,'copy':1}]},{'list28':[{'type':tag,'copy':1}]}]
 # Controlled binding table isolates all final root-side binding outcomes.
 for reverse in [0,1,255]:
  for enabled in [False,True]:
   for index in [-1,0,3,7]:
    cases += [{'model_binding':1,'reverse':reverse,'enabled':enabled,'list24':[{'binding':index}], 'list28':[{'binding':index}]}]
 for tag in [0x800002,0x800001,0x800004,0x800003]:
  for nested in [0,1,2]:cases += [{'list28':[{'type':tag,'nested':nested}]}]
 cases += [{'list28':[{'type':0x80000,'word10':x}]} for x in [0,1,2,0x101,0xff01]]
 cases += [{'model_binding':1,'binding_fail':1,'list24':[{}],'list28':[{}]}]
 cases += [{'fail_alloc':n,'list24':[{'copy':1}],'list28':[{'copy':1}]} for n in range(1,7)]
 cases += [{'owner':1,'owner_size':2,'owner_capacity':2,'fail_alloc':n} for n in [3,4]]
 return cases
if __name__=='__main__':
 rows=[];coverage=set();t=time.time();failures=[]
 for fpscr in [0,0x400000,0x800000,0xc00000,0x1000000]:
  for i,c in enumerate(suite()):
   a,ca,ma=run(False,c,fpscr);b,cb,mb=run(True,c,fpscr);coverage|=ca
   keys=['return','terminated','error','invalid','events','memory','saved','fpscr'] if a['terminated'] else ['terminated','error','invalid','events','fpscr']
   diff=[k for k in keys if a[k]!=b[k]]
   row={'case':c,'fpscr':fpscr,'diff':diff,'retail':a,'candidate':b};rows.append(row)
   if diff:
    path=OUT/f'failure-{fpscr}-{i}.json';path.write_text(json.dumps(row,indent=2)+'\n');failures.append(str(path))
    print(i,c,'DIFF',diff,'returned',a['terminated'],'pc',hex(a['pc']),'bad',a['invalid'][-2:])
    if 'memory' in diff:print('memory offsets',[[j for j,(x,y) in enumerate(zip(aa,bb)) if x!=y][:30] for aa,bb in zip(ma,mb)])
   elif fpscr==0 and not a['terminated']:print(i,c,'paired stop',hex(a['pc']),a['invalid'][-2:])
 result={'pairs':len(rows),'agree':sum(not r['diff'] for r in rows),'returning':sum(r['retail']['terminated'] for r in rows),'seconds':time.time()-t,'root_instructions':len(coverage),'failures':failures,'rows':rows,'coverage':sorted(coverage)}
 result['groups']={}
 for group in ['original_direct_callee','binding_provider','initializer_result']:
  selected=[r for r in rows if ('binding_provider' if r['case'].get('model_binding') else 'initializer_result' if 'init_result' in r['case'] else 'original_direct_callee')==group]
  result['groups'][group]={'pairs':len(selected),'agree':sum(not r['diff'] for r in selected),'returning':sum(r['retail']['terminated'] for r in selected),'fault_pairs':sum(bool(r['retail']['error']) for r in selected),'budget_pairs':sum(not r['retail']['terminated'] and not r['retail']['error'] for r in selected)}
  for side in ['retail','candidate']:
   result['groups'][group][side]={kind:dict(sum((Counter(r[side][kind]) for r in selected),Counter())) for kind in ['entries','bodies']}
 result['symbols']=SYMBOLS
 result['image_sha256']=hashlib.sha256((OUT/'replay.axf').read_bytes()).hexdigest()
 (OUT/'replay-result.json').write_text(json.dumps(result,indent=2)+'\n');print({k:v for k,v in result.items() if k not in ['rows','coverage']})
```

## build/family/synthetic_caller_replay.py

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import *
from unicorn.arm_const import *
import struct,json,hashlib,time,re
from collections import Counter
ROOT=Path.cwd(); OUT=ROOT/'build/family/synthetic'; IMAGE_DIR=ROOT/'build/research298c58'; OUT.mkdir(exist_ok=True); CODE=(ROOT/'data/ver/eu/code.bin').read_bytes()
ALLOC=0x7f0000;FREE=0x7f0010;STOP=0x7f1000;RES=0x800000;ALLOCATOR=0x810000;OPT=0x820000;OWNER=0x830000;BINDRES=0x840000;ARGS=0x850000;CMD=0x870000;BIND=0x890000
R=[UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3]; SAVED=[UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11,UC_ARM_REG_SP]
FAMILY={0x298c58:"fn_00298C58",0x2a451c:"fn_002A451C",0x22f590:"_ZN2nw3gfx13ParticleShape18AddVertexParamSizeEjii",0x22f554:"_ZN2nw3gfx13ParticleShape19AddVertexStreamSizeEjiii"}
SEG=[]
with (IMAGE_DIR/'replay.axf').open('rb') as f:
 e=ELFFile(f)
 CAND=e['e_entry']
 SYMBOLS={name:int(re.search(r'^\s*'+re.escape(name)+r'\s+(0x[0-9a-fA-F]+)\s+ARM Code', (IMAGE_DIR/'replay.map').read_text(), re.M).group(1),16) for name in FAMILY.values()}
 for s in e.iter_sections():
  if s['sh_flags']&2:SEG.append((s['sh_addr'],s.data()))

def run(candidate,case,fpscr=0):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176)
 u.mem_map(0x100000,0x400000);u.mem_write(0x100000,CODE);u.mem_map(0x600000,0x10000)
 for a,b in SEG:u.mem_write(a,b)
 u.mem_map(0x700000,0x100000);u.mem_write(0x700000,b'\xa6'*0x100000)
 u.mem_map(0x800000,0x200000);u.mem_map(0x1000000,0x100000);u.mem_write(0x900000,b'\xc3'*0x100000)
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def byte(a,v):u.mem_write(a,bytes([v]))
 put(ALLOCATOR,ALLOCATOR+0x100);put(ALLOCATOR+0x108,ALLOC);put(ALLOCATOR+0x10c,FREE)
 put(RES,case.get('flags',0x40000001));put(RES+0x30,BINDRES-RES-0x30);put(RES+0x44,0x880000-RES-0x44);put(0x880000,0x80000000)
 list24=case.get('list24',[]);list28=case.get('list28',[])
 for n,items in enumerate([list24,list28]):
  field=RES+0x34+n*8;array=RES+0x100+n*0x100;put(field,len(items));put(field+4,array-field-4)
  for i,item in enumerate(items):
   a=RES+0x1000+n*0x2000+i*0x100;put(array+i*4,a-array-i*4)
   put(a,item.get('type',0));byte(a+4,item.get('copy',0));put(a+8,item.get('binding',-1));put(a+0x10,item.get('word10',0))
   if item.get('nested'):
    put(a+0xc,0x40-0xc)
    if item['nested']==2:put(a+0x60,0x10)
 put(OPT+0x10,case.get('capacity24',0));put(OPT+0x14,case.get('capacity28',0))
 put(OPT,case.get('option0',0));put(OPT+4,0);put(OPT+8,0);put(OPT+12,0)
 put(OWNER+0x10,OWNER);put(OWNER+0x14,0 if case.get('owner_no_allocator') else ALLOCATOR)
 put(OWNER+0x18,OWNER+0x100);put(OWNER+0x1c,OWNER+0x100+4*case.get('owner_size',0));put(OWNER+0x20,case.get('owner_mode',1)<<30|case.get('owner_capacity',0))
 for i in range(case.get('owner_size',0)):put(OWNER+0x100+i*4,OWNER+0x500+i*0x100)
 if case.get('parent'):put(OWNER+0xc,OWNER+0x1000);put(OWNER+0x1000+0xc,OWNER+0x1000 if case.get('parent_cycle') else 0)
 # Two command buffers, output packets and an empty original binding resource.
 for field,offset in [(0xc,0),(0x10,0x1000),(0x18,0x2000),(0x1c,0x3000),(0x24,0x4000)]:put(CMD+field,0x1000000+offset)
 put(CMD+0x34,0);put(BINDRES,case.get('binding_count',0));put(BINDRES+4,0);put(ARGS,0)
 # One separately labeled integration fixture: layouts copied from the two
 # independently recovered replay models; zero capacity is in their valid set.
 if case.get('particle_stream_fixture'):
  put(ARGS,ALLOCATOR+0x100)  # same observed allocator dispatch layout, distinct heap
  put(BINDRES,0);put(BINDRES+4,1);put(BINDRES+8,0x860000-BINDRES-8)
  put(0x860000,0x861000-0x860000)
  put(0x861000,0);put(0x861004,0)  # non-fixed type and stream index 0
  put(0x861010,0)  # value field unused for a stream
 byte(BIND+0xd0,case.get('reverse',0))
 for i in range(8):byte(BIND+0xd1+i,1 if case.get('enabled',True) else 0);put(BIND+0xe4+8*i,0x2000000+i*0x100);put(BIND+0xe8+8*i,0x3000000+i*0x100)
 for i,r in enumerate(SAVED[:-1]):u.reg_write(r,0xa4000000+i)
 for i in range(8):u.reg_write(UC_ARM_REG_D8+i,0x7fe1234500000000+i)
 u.reg_write(UC_ARM_REG_SP,0x7e0000);u.reg_write(UC_ARM_REG_LR,STOP);put(0x7e0000,ARGS);put(0x7e0004,CMD)
 for r,v in zip(R,[OWNER if case.get('owner') else 0,0 if case.get('null_resource') else RES,OPT,ALLOCATOR]):u.reg_write(r,v)
 u.reg_write(UC_ARM_REG_C1_C0_2,0xf00000);u.reg_write(UC_ARM_REG_FPEXC,0x40000000);u.reg_write(UC_ARM_REG_FPSCR,fpscr)
 entries=Counter();bodies=Counter();events=[];allocs=[];freed=set();cov=set();callees=set();bad=[];steps=0;nalloc=0
 def finish(value=0):
  for r in R+[UC_ARM_REG_R12]:u.reg_write(r,0xa5a5a5a5)
  u.reg_write(UC_ARM_REG_R0,value&0xffffffff);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 def hook(u,a,z,d):
  nonlocal nalloc,steps
  steps+=1
  for original,name in FAMILY.items():
   entry=SYMBOLS[name] if candidate else original
   if a==entry:
    entries[name]+=1
    if not (name=="fn_002A451C" and case.get("model_binding")):bodies[name]+=1
  if 0x298c58<=a<0x2997c0:cov.add(a)
  if a in (0x2997c8,0x2b3f70,0x2998c0,0x29b57c,0x29afe8,0x2a451c,0x29e620,0x231ef0):callees.add(a)
  args=[u.reg_read(r) for r in R]
  if a==ALLOC:
   nalloc+=1;memory=0x900000+(nalloc-1)*0x10000
   if nalloc==case.get('fail_alloc'):memory=0
   events.append(['alloc',args[:3],memory]);allocs.append((memory,args[1]));finish(memory)
  elif a==FREE:events.append(['free',args[:2]]);freed.add(args[1]);finish()
  elif a==0x2b3f70 and 'init_result' in case:events.append(['init_model',args[:2]]);finish(case['init_result'])
  elif a==(SYMBOLS['fn_002A451C'] if candidate else 0x2a451c) and case.get('model_binding'):
   events.append(['binding_model',args+[get(u.reg_read(UC_ARM_REG_SP))]])
   finish(0 if case.get('binding_fail') else BIND)
  elif a==0x28e280:events.append(['hardware_flush',args[0]]);finish()
  elif a==0x10b2bc:events.append(['hardware_address',args[0]]);finish(args[0]+0x10000000)
 def invalid(u,access,address,size,value,data):bad.append([access,address,size]);return False
 u.hook_add(UC_HOOK_CODE,hook);u.hook_add(UC_HOOK_MEM_INVALID,invalid)
 error=None
 try:u.emu_start(CAND if candidate else 0x298c58,STOP,count=500000)
 except UcError as e:error=str(e)
 ret=u.reg_read(UC_ARM_REG_R0);terminated=u.reg_read(UC_ARM_REG_PC)==STOP
 ranges=[(0x800000,0x90000),(0x1000000,0x10000)]+[(a,n) for a,n in allocs if a and a not in freed]
 data=[]
 for a,n in ranges:
  b=bytearray(u.mem_read(a,n))
  # Indeterminate C++ struct padding is excluded. Field values remain compared.
  if a==0x900000 and terminated and ret:
   for field,stride in [(0x58,24),(0x68,28)]:
    begin=get(a+field);end=get(a+field+4)
    for entry in range(begin,end,stride):
     off=entry-a
     if 0<=off<len(b)-3:b[off+1:off+4]=b'\0'*3
  data.append(bytes(b))
 result={'entries':dict(entries),'bodies':dict(bodies),'return':ret,'terminated':terminated,'error':error,'invalid':bad,'events':events,'memory':[(a,n,hashlib.sha256(b).hexdigest()) for (a,n),b in zip(ranges,data)],'saved':[u.reg_read(r) for r in SAVED]+[u.reg_read(UC_ARM_REG_D8+i) for i in range(8)],'fpscr':u.reg_read(UC_ARM_REG_FPSCR),'pc':u.reg_read(UC_ARM_REG_PC),'steps':steps,'callees':[hex(a) for a in sorted(callees)]}
 return result,cov,data

def suite():
 return [{'particle_stream_fixture':True}]
if __name__=='__main__':
 rows=[];coverage=set();t=time.time();failures=[]
 for fpscr in [0,0x400000,0x800000,0xc00000,0x1000000]:
  for i,c in enumerate(suite()):
   a,ca,ma=run(False,c,fpscr);b,cb,mb=run(True,c,fpscr);coverage|=ca
   keys=['return','terminated','error','invalid','events','memory','saved','fpscr'] if a['terminated'] else ['terminated','error','invalid','events','fpscr']
   diff=[k for k in keys if a[k]!=b[k]]
   row={'case':c,'fpscr':fpscr,'diff':diff,'retail':a,'candidate':b};rows.append(row)
   if diff:
    path=OUT/f'failure-{fpscr}-{i}.json';path.write_text(json.dumps(row,indent=2)+'\n');failures.append(str(path))
    print(i,c,'DIFF',diff,'returned',a['terminated'],'pc',hex(a['pc']),'bad',a['invalid'][-2:])
    if 'memory' in diff:print('memory offsets',[[j for j,(x,y) in enumerate(zip(aa,bb)) if x!=y][:30] for aa,bb in zip(ma,mb)])
   elif fpscr==0 and not a['terminated']:print(i,c,'paired stop',hex(a['pc']),a['invalid'][-2:])
 result={'pairs':len(rows),'agree':sum(not r['diff'] for r in rows),'returning':sum(r['retail']['terminated'] for r in rows),'seconds':time.time()-t,'root_instructions':len(coverage),'failures':failures,'rows':rows,'coverage':sorted(coverage)}
 result['scope']='separate synthetic caller-to-particle-to-both-sizing-helpers fixture; zero geometry capacity, one stream attribute index 0'
 result['groups']={}
 for group in ['original_direct_callee','binding_provider','initializer_result']:
  selected=[r for r in rows if ('binding_provider' if r['case'].get('model_binding') else 'initializer_result' if 'init_result' in r['case'] else 'original_direct_callee')==group]
  result['groups'][group]={'pairs':len(selected),'agree':sum(not r['diff'] for r in selected),'returning':sum(r['retail']['terminated'] for r in selected),'fault_pairs':sum(bool(r['retail']['error']) for r in selected),'budget_pairs':sum(not r['retail']['terminated'] and not r['retail']['error'] for r in selected)}
  for side in ['retail','candidate']:
   result['groups'][group][side]={kind:dict(sum((Counter(r[side][kind]) for r in selected),Counter())) for kind in ['entries','bodies']}
 result['symbols']=SYMBOLS
 result['image_sha256']=hashlib.sha256((IMAGE_DIR/'replay.axf').read_bytes()).hexdigest()
 (OUT/'replay-result.json').write_text(json.dumps(result,indent=2)+'\n');print({k:v for k,v in result.items() if k not in ['rows','coverage']})
```

## build/particle/replay.py

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import *
from unicorn.arm_const import *
from collections import Counter
import struct,json,hashlib,time,random,os,re
ROOT=Path.cwd();OUT=ROOT/'build/particle';CODE=(ROOT/'data/ver/eu/code.bin').read_bytes()
ENTRY=0x2a451c;CAND=0x600000;ALLOC=0x7f0000;FREE=0x7f0010;STOP=0x7f1000;RES=0x800000;HEAP=0x810000;STREAMHEAP=0x811000;OWNER=0x820000;SHAPE=0x830000;ATTR=0x840000;ARRAY=0x850000;VALUES=0x855000;BUF1=0x860000;BUF2=0x861000;SP=0x7e0000
R=[UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3];SAVED=[UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11,UC_ARM_REG_SP]+[UC_ARM_REG_D8+i for i in range(8)]
FAMILY={0x2a451c:"fn_002A451C",0x22f590:"_ZN2nw3gfx13ParticleShape18AddVertexParamSizeEjii",0x22f554:"_ZN2nw3gfx13ParticleShape19AddVertexStreamSizeEjiii"}
SEG=[]
IMAGE=os.environ.get('PARTICLE_REPLAY_IMAGE','replay.axf')
with (OUT/IMAGE).open('rb') as f:
 e=ELFFile(f)
 SYMBOLS={name:int(re.search(r'^\s*'+re.escape(name)+r'\s+(0x[0-9a-fA-F]+)\s+ARM Code', (OUT/'replay-combined.map').read_text(), re.M).group(1),16) for name in FAMILY.values()}
 CAND=e['e_entry']
 for s in e.iter_sections():
  if s['sh_flags']&2:SEG.append((s['sh_addr'],s.data()))

def run(candidate,case,fpscr=0):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176)
 u.mem_map(0x100000,0x400000);u.mem_write(0x100000,CODE);u.mem_map(0x600000,0x10000)
 for a,b in SEG:u.mem_write(a,b)
 u.mem_map(0x700000,0x100000);u.mem_write(0x700000,b'\xa6'*0x100000)
 u.mem_map(0x800000,0x200000);u.mem_write(0x800000,b'\xc3'*0x200000)
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def byte(a,v):u.mem_write(a,bytes([v]))
 for heap in [HEAP,STREAMHEAP]:put(heap,heap+0x100);put(heap+0x108,ALLOC);put(heap+0x10c,FREE)
 attrs=case.get('attributes',[])
 put(RES,case.get('count',3));put(RES+4,len(attrs));put(RES+8,ARRAY-RES-8 if attrs else 0)
 put(OWNER+0x4c,case.get('existing',0));put(SHAPE+0x34,case.get('attribute_start',0));byte(SHAPE+0x30,case.get('swap',0));put(SHAPE+0x1f8,BUF1);put(SHAPE+0x1fc,BUF2)
 for i,attr in enumerate(attrs):
  index,typ=attr[:2];at=ATTR+i*0x40;array=ARRAY+i*4;val=VALUES+i*0x10
  put(array,at-array);put(at,typ);put(at+4,index);put(at+0x10,0 if case.get('null_values') else val-at-0x10)
  for j,x in enumerate(case.get('values',[0x3f012345,0x80000000,0x7fc12345,0x7f800000])):put(val+j*4,x)
 if case.get('null_array'):put(RES+8,0)
 if case.get('null_attribute'):put(ARRAY,0)
 saved=[0xa4000000+i for i in range(8)]+[SP]+[0x7fe1234500000000+i for i in range(8)]
 for r,v in zip(SAVED,saved):u.reg_write(r,v)
 u.reg_write(UC_ARM_REG_LR,STOP);put(SP,0 if case.get('null_shape') else SHAPE)
 for r,v in zip(R,[OWNER if case.get('owner',True) else 0,0 if case.get('null_resource') else RES,HEAP,STREAMHEAP]):u.reg_write(r,v)
 u.reg_write(UC_ARM_REG_C1_C0_2,0xf00000);u.reg_write(UC_ARM_REG_FPEXC,0x40000000);u.reg_write(UC_ARM_REG_FPSCR,fpscr)
 entries=Counter();events=[];allocs=[];cov=set();callees=Counter();bad=[];steps=0;nalloc=0
 def finish(value=0):
  for r in R+[UC_ARM_REG_R12]:u.reg_write(r,0xa5a5a5a5)
  u.reg_write(UC_ARM_REG_R0,value&0xffffffff);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 def hook(u,a,z,d):
  nonlocal nalloc,steps
  steps+=1
  for original,name in FAMILY.items():
   if a==(SYMBOLS[name] if candidate else original):entries[name]+=1
  if ENTRY<=a<0x2a4ffc:cov.add(a)
  if a in (0x22f590,0x22f554,0x22f468,0x22f35c,0x288414,0x292354,0x2a4434,0x28b330):callees[hex(a)]+=1
  args=[u.reg_read(r) for r in R]
  if a==ALLOC:
   nalloc+=1;memory=0x900000 if args[0]==HEAP else 0x940000
   if nalloc==case.get('fail_alloc'):memory=0
   if args[1]>0x30000:raise AssertionError(('unexpected size',args))
   events.append(['allocate',args[:3],memory]);allocs.append((memory,args[1]));finish(memory)
  elif a==FREE:events.append(['release',args[:2]]);finish()
 def invalid(u,access,address,size,value,data):bad.append([access,address,size]);return False
 u.hook_add(UC_HOOK_CODE,hook);u.hook_add(UC_HOOK_MEM_INVALID,invalid)
 error=None
 try:u.emu_start(CAND if candidate else ENTRY,STOP,count=100000)
 except UcError as e:error=str(e)
 ret=u.reg_read(UC_ARM_REG_R0);terminated=u.reg_read(UC_ARM_REG_PC)==STOP
 data=bytes(u.mem_read(0x800000,0x200000))
 if terminated:assert [u.reg_read(r) for r in SAVED]==saved,('callee saved',case,candidate)
 result={'entries':dict(entries),'return':ret if terminated else None,'terminated':terminated,'error':error,'invalid':bad,'events':events,'memory':hashlib.sha256(data).hexdigest(),'fpscr':u.reg_read(UC_ARM_REG_FPSCR),'pc':u.reg_read(UC_ARM_REG_PC),'steps':steps,'callees':dict(callees)}
 return result,cov,data

def suite():
 cases=[{}, {'fail_alloc':1},{'owner':False},{'existing':0xabcdef01}, {'swap':1},{'swap':255},{'attribute_start':3}]
 cases += [{'count':n} for n in [0,1,2,4,5,8,31,64,127]]
 for index in range(16):
  for typ in [0x40000000,0,0x40000001,0xffffffff]:cases.append({'attributes':[(index,typ)]})
 for indices in [list(range(13)),list(range(13))[::-1], [0,0], [10,11,12], [13,14,15], [0,4,7,8]]:
  for typ in [0x40000000,0]:cases.append({'attributes':[(i,typ) for i in indices]})
 for index in range(13):
  for n in [0,1,2,3,8,17]:
   cases.append({'count':n,'attributes':[(index,0)]})
 for failure in [1,2]:cases.append({'attributes':[(0,0)],'fail_alloc':failure})
 for bits in [0,0x80000000,0x7f800001,0x7fc12345,0x7f800000,0xff800000,1,0x007fffff]:cases.append({'attributes':[(10,0x40000000),(0,0x40000000)],'values':[bits]*4})
 rng=random.Random(0x2a451c)
 for _ in range(200):
  indices=rng.sample(list(range(13)),rng.randrange(14))
  cases.append({'attributes':[(i,rng.choice([0x40000000,0])) for i in indices],'count':rng.randrange(65),'swap':rng.choice([0,1,2]),'owner':rng.choice([False,True]),'values':[rng.getrandbits(32) for _ in range(4)]})
 cases += [{'null_resource':True}, {'attributes':[(0,0)],'null_array':True}, {'attributes':[(0,0)],'null_attribute':True}, {'attributes':[(10,0x40000000)],'null_values':True}, {'null_shape':True}]
 return cases
if __name__=='__main__':
 rows=[];coverage=set();t=time.time();failures=[]
 for fpscr in [0,0x400000,0x800000,0xc00000,0x1000000,0x2000000]:
  for i,c in enumerate(suite()):
   a,ca,ma=run(False,c,fpscr);b,cb,mb=run(True,c,fpscr);coverage|=ca
   keys=['return','terminated','error','invalid','events','memory','fpscr']
   diff=[k for k in keys if a[k]!=b[k]]
   row={'case':c,'fpscr':fpscr,'diff':diff,'retail':a,'candidate':b};rows.append(row)
   if diff:
    path=OUT/f'failure-{fpscr}-{i}.json';row['offsets']=[hex(0x800000+j) for j,(x,y) in enumerate(zip(ma,mb)) if x!=y][:50];path.write_text(json.dumps(row,indent=2)+'\n');failures.append(str(path));print(i,c,'DIFF',diff,row['offsets'])
 result={'pairs':len(rows),'agree':sum(not r['diff'] for r in rows),'returning':sum(r['retail']['terminated'] for r in rows),'seconds':time.time()-t,'root_instructions':len(coverage),'failures':failures,'rows':rows,'coverage':sorted(coverage),'image':IMAGE,'image_sha256':hashlib.sha256((OUT/IMAGE).read_bytes()).hexdigest(),'source_sha256':hashlib.sha256((ROOT/'lib/CtrSDK/sources/retail_ParticleBinding.cpp').read_bytes()).hexdigest(),'object_sha256':hashlib.sha256((ROOT/'build/eu/obj/lib/CtrSDK/sources/retail_ParticleBinding.o').read_bytes()).hexdigest()}
 result['family_input_hashes']={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in [ROOT/'lib/CtrSDK/sources/retail_ParticleBinding.cpp',ROOT/'lib/CtrSDK/sources/retail_ParticleShapeSize.cpp',ROOT/'lib/CtrSDK/include/retail/ParticleShape.h']}
 result['entry_counts']={side:dict(sum((Counter(r[side]['entries']) for r in rows),Counter())) for side in ['retail','candidate']}
 (OUT/'replay-result.json').write_text(json.dumps(result,indent=2)+'\n');print({k:v for k,v in result.items() if k not in ['rows','coverage']})
```

## build/particle/replay_helpers.py

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import *
from unicorn.arm_const import *
import itertools,struct,json,hashlib,time,re
out=Path('build/particle');code=Path('data/ver/eu/code.bin').read_bytes();elf=ELFFile((out/'replay-combined.axf').open('rb'))
segments=[(s['sh_addr'],s.data()) for s in elf.iter_sections() if s['sh_flags']&2]
names=['_ZN2nw3gfx13ParticleShape18AddVertexParamSizeEjii','_ZN2nw3gfx13ParticleShape19AddVertexStreamSizeEjiii'];entries=[int(re.search(r'^\s*'+re.escape(n)+r'\s+(0x[0-9a-fA-F]+)\s+ARM Code', (out/'replay-combined.map').read_text(), re.M).group(1),16) for n in names];retail=[0x22f590,0x22f554]
regs=[UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3];savedregs=[UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11,UC_ARM_REG_SP]+[UC_ARM_REG_D8+i for i in range(8)];saved=[0xa8000000+i for i in range(8)]+[0x718000]+[0x7fe1234500000000+i for i in range(8)]
ms=[]
for i in range(2):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176);u.mem_map(0x100000,0x400000);u.mem_write(0x100000,code);u.mem_map(0x600000,0x10000);u.mem_map(0x700000,0x20000)
 for a,b in segments:u.mem_write(a,b)
 ms.append(u)
types=[0,1,0x1400,0x1401,0x1402,0x1403,0x1404,0x1405,0x1406,0x1407,0xffff,0xffffffff];widths=[-1,0,1,2,3,4,16];useds=[-64,-33,-32,-31,-1,0,1,2,15,16,17,31,32,33,63,64,65,127,128,129,255,1024];counts=[-8,-7,-1,0,1,2,3,4,5,8,64,127,4096]
result=[];begin=time.monotonic()
for k in range(2):
 n=0;digest=hashlib.sha256()
 inputs=itertools.product(types,widths,useds) if k==0 else itertools.product(types,widths,counts,useds)
 for args in inputs:
  values=[]
  for i,u in enumerate(ms):
   for r,v in zip(regs,args):u.reg_write(r,v&0xffffffff)
   for r,v in zip(savedregs,saved):u.reg_write(r,v)
   u.reg_write(UC_ARM_REG_LR,0x700000);u.reg_write(UC_ARM_REG_FPSCR,0)
   u.emu_start(entries[k] if i else retail[k],0x700000,count=100)
   assert u.reg_read(UC_ARM_REG_PC)==0x700000,('budget',k,args,i)
   assert [u.reg_read(r) for r in savedregs]==saved,('ABI',k,args,i)
   values.append((u.reg_read(UC_ARM_REG_R0),u.reg_read(UC_ARM_REG_FPSCR)))
  assert values[0]==values[1],(k,args,values)
  digest.update(struct.pack('<II',*values[0]));n+=1
 result.append({'symbol':names[k],'pairs':n,'output_sha256':digest.hexdigest(),'all_equal':True})
r={'results':result,'total_pairs':sum(x['pairs'] for x in result),'seconds':time.monotonic()-begin,'modelled_callees':[],'source_sha256':hashlib.sha256(Path('lib/CtrSDK/sources/retail_ParticleShapeSize.cpp').read_bytes()).hexdigest(),'object_sha256':hashlib.sha256(Path('build/eu/obj/lib/CtrSDK/sources/retail_ParticleShapeSize.o').read_bytes()).hexdigest()};(out/'helper-replay-result.json').write_text(json.dumps(r,indent=2)+'\n');print(json.dumps(r,indent=2))
```

## build/family/compare_codegen.py

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection
import hashlib,json,subprocess,sys
root=Path.cwd();out=root/'build/family';base=root.parent
pairs=[('mario-resource-298c58','AddressResourceFactory'),('mario-particle-shape-2a451c','retail_ParticleBinding'),('mario-particle-shape-2a451c','retail_ParticleShapeSize')]
def normalize(p):
 with p.open('rb') as f:
  e=ELFFile(f);sections=[];relocs=[];symbols=[]
  def sr(s):
   n=s['st_shndx'];sec=e.get_section(n).name if isinstance(n,int) else n
   return [s.name,s['st_info']['type'],s['st_info']['bind'],s['st_other']['visibility'],sec,s['st_value'],s['st_size']]
  for sec in e.iter_sections():
   if sec['sh_flags']&2:sections.append([sec.name,sec['sh_type'],sec['sh_flags'],sec['sh_addralign'],sec['sh_entsize'],sec['sh_addr'],sec['sh_size'],sec['sh_link'],sec['sh_info'],sec.data().hex()])
   if isinstance(sec,RelocationSection):
    tab=e.get_section(sec['sh_link']);target=e.get_section(sec['sh_info']).name
    relocs.append([sec.name,target,[(r['r_offset'],r['r_info_type'],sr(tab.get_symbol(r['r_info_sym']))) for r in sec.iter_relocations()]])
  for s in e.get_section_by_name('.symtab').iter_symbols():
   if s['st_info']['type']!='STT_FILE':symbols.append(sr(s))
  return {'allocated_sections':sections,'relocations':relocs,'symbols':symbols}
results=[]
for tree,name in pairs:
 rel=f'build/eu/obj/lib/CtrSDK/sources/{name}.o';old=base/tree/rel;new=root/rel;a=normalize(old);b=normalize(new)
 result={'source_tree':tree,'source_head':subprocess.check_output(['git','-C',str(base/tree),'rev-parse','HEAD'],text=True).strip(),'object':rel,'old_object_sha256':hashlib.sha256(old.read_bytes()).hexdigest(),'new_object_sha256':hashlib.sha256(new.read_bytes()).hexdigest(),'allocated_bytes_equal':a['allocated_sections']==b['allocated_sections'],'resolved_relocations_equal':a['relocations']==b['relocations'],'all_nonfile_symbols_equal':a['symbols']==b['symbols'],'definitions':[s for s in b['symbols'] if s[1]=='STT_FUNC' and s[4]!='SHN_UNDEF'],'allocated_sections':[{'name':s[0],'size':len(bytes.fromhex(s[-1])),'sha256':hashlib.sha256(bytes.fromhex(s[-1])).hexdigest()} for s in b['allocated_sections']],'imports':[s[0] for s in b['symbols'] if s[4]=='SHN_UNDEF']}
 result['verified_provenance']={}
 for label,tree_root in [('before',base/tree),('after',root)]:
  program='from pathlib import Path; import json; from tools.low.buildProvenance import verify_build_output; print(json.dumps(verify_build_output(Path('+repr(rel)+'))))'
  result['verified_provenance'][label]=json.loads(subprocess.check_output([sys.executable,'-c',program],cwd=tree_root,text=True))
 results.append(result)
 if a!=b:(out/(name+'-comparison.json')).write_text(json.dumps({'before':a,'after':b},indent=2))
result={'checkpoint':subprocess.check_output(['git','rev-parse','HEAD'],text=True).strip(),'results':results,'all_equal':all(r['allocated_bytes_equal'] and r['resolved_relocations_equal'] and r['all_nonfile_symbols_equal'] for r in results)}
(out/'codegen-comparison.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2));assert result['all_equal']
```

## build/family/verify_results.py

```python
from pathlib import Path
import json
root=Path.cwd()
def read(p):return json.loads((root/p).read_text())
gate=read('build/family/gate8991/report.json');caller=read('build/research298c58/replay-result.json');synthetic=read('build/family/synthetic/replay-result.json');particle=read('build/particle/replay-result.json');helper=read('build/particle/helper-replay-result.json')
assert gate['accepted'] and gate['prior_roots']==717 and gate['clean_build_returncode']==0
assert len({x['symbol'] for x in gate['prior_checks']})==717 and all(x['returncode']==0 for x in gate['prior_checks'])
expected=[0xa4000000+i for i in range(8)]+[0x7e0000]+[0x7fe1234500000000+i for i in range(8)]
for dataset in [caller,synthetic]:
 for row in dataset['rows']:
  assert not row['diff']
  for side in ['retail','candidate']:
   if row[side]['terminated']:assert row[side]['saved']==expected
assert caller['pairs']==655 and caller['agree']==655 and caller['returning']==620
assert caller['groups']['binding_provider']['pairs']==125 and caller['groups']['initializer_result']['pairs']==20
closure=[x for x in caller['rows'] if not x['case'].get('model_binding') and 'init_result' not in x['case'] and x['candidate']['bodies'].get('fn_002A451C',0)]
assert len(closure)==445 and all(x['retail']['terminated'] and x['candidate']['terminated'] for x in closure)
assert synthetic['pairs']==5 and synthetic['agree']==5 and synthetic['returning']==5
stream='_ZN2nw3gfx13ParticleShape19AddVertexStreamSizeEjiii'
assert synthetic['groups']['original_direct_callee']['candidate']['bodies'][stream]==5
bad=[x for x in particle['rows'] if x['diff']]
assert particle['pairs']==2310 and particle['agree']==2304 and particle['returning']==2262
assert len(bad)==6 and all(x['case'].get('null_attribute') and x['diff']==['invalid'] and x['retail']['invalid']==[[19,4,4]] and x['candidate']['invalid']==[[19,0,4]] for x in bad)
assert not any(not x['retail']['terminated'] and not x['retail']['error'] for x in particle['rows'])
assert helper['total_pairs']==25872 and all(x['all_equal'] for x in helper['results'])
assert (root/'data/ver/eu/map.csv').read_bytes()==(root/'build/family/original-map.csv').read_bytes()
print('Fresh preservation, caller/control split, synthetic compiled stream closure, register canaries, six retained particle failures, helper replay and exact map restoration verified')
```
