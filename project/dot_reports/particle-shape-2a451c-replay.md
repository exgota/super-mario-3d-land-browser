# Reproduce the particle binding family

Run from the frozen branch root with the owner’s private EU inputs and approved compilers locally installed. No binary or replay image is committed. The source freezes atbdfc1a3; restore the exact map in a finally block after diagnostic naming/checking. The following scripts are complete executable text, included as notes rather than changes under tools/.

## Build and canonical checks

```sh
. ./development_environment.sh
export DEVKITARM=/usr
mkdir -p build/particle
sha256sum data/ver/eu/code.bin
python make.py eu -ca
```

Save the following as build/particle/prepare_map.py. It names only four existing rows and preserves the exact original map bytes for restoration.

```python
from pathlib import Path
p=Path("data/ver/eu/map.csv"); backup=Path("build/particle/map-original.csv")
assert not backup.exists(), "Use a fresh scratch directory or verify the existing backup first"
backup.write_bytes(p.read_bytes())
rows=p.read_text().splitlines();names={0x2a451c:"fn_002A451C",0x2a4434:"fn_002A4434",0x3d8384:"dat_003D8384",0x3ba150:"dat_003BA150"}
for i,line in enumerate(rows):
 cols=line.split(",")
 try:a=int(cols[0],16)
 except ValueError:continue
 if a in names:
  assert not cols[6].strip();cols[6]=names[a];rows[i]=",".join(cols)
p.write_text("\n".join(rows)+"\n")
```

```sh
python build/particle/prepare_map.py
python tools/check.py fn_002A451C --object build/eu/obj/lib/CtrSDK/sources/retail_ParticleBinding.o
python tools/check.py _ZN2nw3gfx13ParticleShape18AddVertexParamSizeEjii --object build/eu/obj/lib/CtrSDK/sources/retail_ParticleShapeSize.o
python tools/check.py _ZN2nw3gfx13ParticleShape19AddVertexStreamSizeEjiii --object build/eu/obj/lib/CtrSDK/sources/retail_ParticleShapeSize.o
```

## link_replay.py

Save as build/particle/link_replay.py.

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
import subprocess,sys
from tools.low.checkExactBytes import _resolve_symbol, _read_map
root=Path.cwd(); out=Path('build/particle'); combined='--combined' in sys.argv
objects=[Path('build/eu/obj/lib/CtrSDK/sources/retail_ParticleBinding.o')]
if combined:objects.append(Path('build/eu/obj/lib/CtrSDK/sources/retail_ParticleShapeSize.o'))
rows=_read_map(Path('data/ver/eu/map.csv')); lines=['#<SYMDEFS>#'];defined=set()
for obj in objects:
 with obj.open('rb') as f:
  for s in ELFFile(f).get_section_by_name('.symtab').iter_symbols():
   if s['st_shndx']!='SHN_UNDEF':defined.add(s.name)
seen=set()
for obj in objects:
 with obj.open('rb') as f:
  for s in ELFFile(f).get_section_by_name('.symtab').iter_symbols():
   if s['st_shndx']=='SHN_UNDEF' and s.name and s.name not in defined|seen and not s.name.startswith('Lib$$'):
    address,kind,row=_resolve_symbol(s,None,rows);lines.append(f'0x{address:08X} {kind} {s.name}');seen.add(s.name)
stem='replay-combined' if combined else 'replay';sym=out/(stem+'-imports.sym');sym.write_text('\n'.join(lines)+'\n')
subprocess.run([str(root/'data/compilers/wibo'),str(root/'data/compilers/4.0/902/bin/armlink.exe'),'--cpu=MPCore','--fpu=VFPv2','--arm_only','--no_exceptions','--no_debug','--no_scanlib','--mangled','--symbols','--map','--ro_base=0x600000','--entry=fn_002A451C','--keep=fn_002A451C','--output='+str(out/(stem+'.axf')),'--list='+str(out/(stem+'.map'))]+[str(p) for p in objects]+[str(sym)],check=True)
```

## replay.py

Save as build/particle/replay.py.

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import *
from unicorn.arm_const import *
from collections import Counter
import struct,json,hashlib,time,random,os
ROOT=Path.cwd();OUT=ROOT/'build/particle';CODE=(ROOT/'data/ver/eu/code.bin').read_bytes()
ENTRY=0x2a451c;CAND=0x600000;ALLOC=0x7f0000;FREE=0x7f0010;STOP=0x7f1000;RES=0x800000;HEAP=0x810000;STREAMHEAP=0x811000;OWNER=0x820000;SHAPE=0x830000;ATTR=0x840000;ARRAY=0x850000;VALUES=0x855000;BUF1=0x860000;BUF2=0x861000;SP=0x7e0000
R=[UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3];SAVED=[UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11,UC_ARM_REG_SP]+[UC_ARM_REG_D8+i for i in range(8)]
SEG=[]
IMAGE=os.environ.get('PARTICLE_REPLAY_IMAGE','replay.axf')
with (OUT/IMAGE).open('rb') as f:
 e=ELFFile(f)
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
 events=[];allocs=[];cov=set();callees=Counter();bad=[];steps=0;nalloc=0
 def finish(value=0):
  for r in R+[UC_ARM_REG_R12]:u.reg_write(r,0xa5a5a5a5)
  u.reg_write(UC_ARM_REG_R0,value&0xffffffff);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 def hook(u,a,z,d):
  nonlocal nalloc,steps
  steps+=1
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
 result={'return':ret if terminated else None,'terminated':terminated,'error':error,'invalid':bad,'events':events,'memory':hashlib.sha256(data).hexdigest(),'fpscr':u.reg_read(UC_ARM_REG_FPSCR),'pc':u.reg_read(UC_ARM_REG_PC),'steps':steps,'callees':dict(callees)}
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
 (OUT/'replay-result.json').write_text(json.dumps(result,indent=2)+'\n');print({k:v for k,v in result.items() if k not in ['rows','coverage']})
```

## replay_helpers.py

Save as build/particle/replay_helpers.py.

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

## preserve.py

Save as build/particle/preserve.py.

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection
import json,hashlib
root=Path.cwd();base=root.parent/'mario-main45a';out=root/'build/particle'
def normalized(p):
 e=ELFFile(open(p,'rb'));sections=[];symbols=[]
 def symrec(s):
  n=s['st_shndx'];sec=e.get_section(n).name if isinstance(n,int) else n
  return [s.name,s['st_info']['type'],s['st_info']['bind'],s['st_other']['visibility'],sec,s['st_value'],s['st_size']]
 for sec in e.iter_sections():
  if isinstance(sec,RelocationSection):
   tab=e.get_section(sec['sh_link']);sections.append([sec.name,[(r['r_offset'],r['r_info_type'],symrec(tab.get_symbol(r['r_info_sym']))) for r in sec.iter_relocations()]])
  elif sec['sh_type'] not in ('SHT_SYMTAB','SHT_STRTAB','SHT_NULL'):
   sections.append([sec.name,sec['sh_type'],sec['sh_flags'],sec['sh_addralign'],sec['sh_entsize'],hashlib.sha256(sec.data().replace(str(root).encode(),b'<repo>').replace(str(base).encode(),b'<repo>') if sec.name=='.comment' else sec.data()).hexdigest()])
 tab=e.get_section_by_name('.symtab')
 for s in tab.iter_symbols():
  if s['st_info']['type']!='STT_FILE':symbols.append(symrec(s))
 return {'sections':sections,'symbols':symbols}
records=[];inputs={}
for p in sorted((root/'build/eu/obj').rglob('*.o')):
 rel=p.relative_to(root/'build/eu/obj')
 if rel.parts[0] not in ('Game','lib') or p.stem in ('retail_ParticleBinding','retail_ParticleShapeSize'):continue
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
result={'base':'45a4305466a7c4cbd87589169384102e88f8d1b5','object_count':len(records),'input_count':len(inputs),'all_equal':True,'excluded_difference':'STT_FILE absolute source path, repository prefix in non-allocated .comment, and resulting raw symbol/string table indices; all other symbols, section data, attributes and resolved relocations compare equal','objects':records,'inputs':inputs}
(out/'preservation.json').write_text(json.dumps(result,indent=2));print(json.dumps({k:v for k,v in result.items() if k not in ('objects','inputs')},indent=2))
```

## Execute and restore

```sh
PYTHONPATH=. python build/particle/link_replay.py
python build/particle/replay.py
cp build/particle/replay-result.json build/particle/root-only-replay-result.json
PYTHONPATH=. python build/particle/link_replay.py --combined
PARTICLE_REPLAY_IMAGE=replay-combined.axf python build/particle/replay.py
cp build/particle/replay-result.json build/particle/combined-replay-result.json
python build/particle/replay_helpers.py
python build/particle/preserve.py
cp build/particle/map-original.csv data/ver/eu/map.csv
sha256sum data/ver/eu/map.csv data/ver/eu/code.bin
```

The preservation script requires the independently built pristine45a sibling checkout mario-main45a. It does not copy or share objects. Its excluded fields are documented in the main report and its JSON. For a fresh machine, create that checkout from45a, materialize private inputs/compilers, and run the same normal clean build before comparison. The previously recorded all-root/all-definition canonical gate is separate from this equivalence check.

The replay prints six known null-attribute fault-address divergences, one per FPSCR. These are failures retained intentionally. The other records distinguish returned executions from matching fault controls; a budget exhaustion is never a returning success.
