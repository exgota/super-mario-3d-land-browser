# Reproduce the CFL mesh-root diagnostic

Run from the committed proposal checkout with the approved private executable,
exheader, compilers, wibo and Python environment installed as described by the
project brief. The scripts below are the executed local scripts, embedded in
notes to keep the tools directory unchanged. They require Capstone, Unicorn and
pyelftools. No private binary data is embedded here.

Save each Python block to its indicated ignored build path, then run:

```sh
mkdir -p build/root280798
. ./development_environment.sh
export DEVKITARM=/usr
sha256sum data/ver/eu/code.bin
python make.py eu -ca > build/root280798/clean-build.log 2>&1
python build/root280798/check.py
python build/root280798/link.py
python build/root280798/replay.py > build/root280798/final-replay.log 2>&1
python build/root280798/extended.py > build/root280798/final-extended.log 2>&1
python build/root280798/preserve.py
```

The canonical check must report the 2,952/2,904 section-size mismatch. A zero
shell status for the wrapper does not turn that rejection into success: inspect
its printed canonical output. The wrapper restores the exact original map bytes.
The separate diagnostic link validates every import against an existing map row;
it is never passed to the canonical checker as an object.

The preservation comparison expects a sibling pristine `mario-main45a` checkout
of `45a4305466a7c4cbd87589169384102e88f8d1b5`, clean built with the same tools. Its
existing `build/dot-baseline-45a/report.json` records the independently completed
707-root/726-definition empty-batch gate. If recreating that baseline, use a
fresh ignored output directory and this empty manifest:

```json
{"prior_checkpoint":"45a4305466a7c4cbd87589169384102e88f8d1b5","candidates":[]}
```

Run the baseline's unchanged `python tools/acceptance_batch.py <manifest>
--output build/<fresh-output>`, after committing intended inputs and sourcing its
environment. This reruns the full baseline gate; it is not necessary to reproduce
the root replay itself. The equivalence script compares every prior canonical
object, source input, command and compiler, excluding only the new TU and the
explicit nonallocated checkout-path metadata described in the main report.

The two final replay outputs report 229 and 315 pairs, 509,719 and 692,805 compared
bytes, 2,594 and 2,820 events, and no extended-suite FPSCR differences. Consult the
main report for the precise model, memory, malformed-input and termination limits.

## build/root280798/check.py

```python
from pathlib import Path
import subprocess,sys,hashlib,json
p=Path('data/ver/eu/map.csv'); original=p.read_bytes(); old=b'0x00280798,0x00280E04,0x002812F0,          ,U,f,,'; assert original.count(old)==1
try:
 p.write_bytes(original.replace(old,b'0x00280798,0x00280E04,0x002812F0,          ,U,f,fn_00280798,'))
 c=subprocess.run([sys.executable,'tools/check.py','fn_00280798','--object','build/eu/obj/lib/CtrSDK/sources/cfl_MeshResource.o'],capture_output=True,text=True)
 print(c.stdout,c.stderr);Path('build/root280798/check-current.log').write_text(c.stdout+c.stderr)
finally:p.write_bytes(original)
assert p.read_bytes()==original

```

## build/root280798/link.py

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
import csv,subprocess,os,json,hashlib
root=Path.cwd();out=root/'build/root280798';out.mkdir(exist_ok=True)
obj=root/'build/eu/obj/lib/CtrSDK/sources/cfl_MeshResource.o'
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
(out/'candidate.sct').write_text('CANDIDATE_LOAD 0x00280798\n{\n CANDIDATE_CODE 0x00280798\n {\n  cfl_MeshResource.o (i.fn_00280798, +FIRST)\n  cfl_MeshResource.o (+RO)\n }\n}\n')
cmd=[str(root/'data/compilers/wibo'),str(root/'data/compilers/4.0/902/bin/armlink.exe'),'--cpu=MPCore','--fpu=VFPv2','--arm_only','--no_exceptions','--inline','--datacompressor=off','--no_debug','--no_scanlib','--mangled','--symbols','--map','--entry=fn_00280798','--keep=fn_00280798',f'--scatter={out}/candidate.sct',f'--output={out}/candidate.axf',f'--list={out}/candidate.map',str(obj),str(out/'symbols.sym')]
p=subprocess.run(cmd,env={**os.environ,'TMP':'/tmp'},stdout=subprocess.PIPE,stderr=subprocess.STDOUT,text=True);print(p.stdout);assert p.returncode==0
print('Diagnostic root link only; canonical checker ran separately.')

```

## build/root280798/replay.py

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import *
from unicorn.arm_const import *
import struct,json,random,hashlib,itertools
ROOT=Path.cwd();OUT=ROOT/'build/root280798';BINARY=(ROOT/'data/ver/eu/code.bin').read_bytes();BASE=0x280798;END=0x71000000;DB=0x1000000;STACK=0x70000000;SP=STACK+0x18000
assert hashlib.sha256(BINARY).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
with (OUT/'candidate.axf').open('rb') as f:
 e=ELFFile(f);CANDIDATE=next(s.data() for s in e.iter_sections() if s['sh_addr']==BASE and s['sh_size'])
R=[UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3,UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11,UC_ARM_REG_R12]
RESOURCE=DB;TRANSLATION=DB+0x1000;OCC=DB+0x2000;HEAP=DB+0x10000;CALLBACK=0x40000000
U=lambda *v:struct.pack('<'+'I'*len(v),*[x&0xffffffff for x in v])
def make_blob(case):
 rng=random.Random(case['seed']);cat=case['category'];counts=case['counts'];parts=case['parts'];wide=case['wide']
 blob=bytearray()
 if cat==2:blob.extend(struct.pack('<9f',*[rng.uniform(-10,10) for _ in range(9)]))
 if cat==5:blob.extend(struct.pack('<18f',*[rng.uniform(-10,10) for _ in range(18)]))
 blob.extend(struct.pack('<4H',*counts,len(parts)|(wide<<15)))
 components=[3,3,2];stride=sum(n*2 for n,c in zip(components,counts) if c>=2)
 nshort=max(counts)*stride//2+sum(n for n,c in zip(components,counts) if c==1)
 blob.extend(struct.pack('<'+'h'*nshort,*[rng.randint(-1200,1200) for _ in range(nshort)]))
 for mode,n in parts:blob.extend(struct.pack('<2H',mode,n))
 n=sum(n for mode,n in parts);blob.extend(struct.pack('<'+('H' if wide else 'B')*n,*[i%(300 if wide else 250) for i in range(n)]))
 # Each two-byte bound contains inclusive low/high tile coordinates.
 if cat==6 and len(parts)==1:
  for i in range(n//3):
   x0,y0=rng.randrange(16),rng.randrange(16);x1,y1=rng.randrange(x0,16),rng.randrange(y0,16)
   blob.extend(bytes([x0*16+x1,y0*16+y1]))
 return bytes(blob)
class Engine:
 def __init__(self,candidate):
  u=self.u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176);u.mem_map(0x100000,0x400000);u.mem_write(0x100000,BINARY)
  if candidate:u.mem_write(BASE,CANDIDATE)
  self.extent=len(CANDIDATE) if candidate else 2904
  u.mem_map(DB,0x200000);u.mem_map(STACK,0x20000);u.mem_map(END,0x1000);u.mem_map(CALLBACK,0x1000)
  u.reg_write(UC_ARM_REG_C1_C0_2,0xf<<20);u.reg_write(UC_ARM_REG_FPEXC,1<<30);u.hook_add(UC_HOOK_CODE,self.hook)
 def get(self,a,n=1):
  out=struct.unpack('<'+'I'*n,self.u.mem_read(a,n*4));return out[0] if n==1 else out
 def put(self,a,*v):self.u.mem_write(a,U(*v))
 def alloc(self,n,align):
  p=(self.cursor+align-1)&~(align-1);self.cursor=p+max(n,16);assert self.cursor<DB+0x200000;self.u.mem_write(p,bytes([0xcd])*max(n,16));self.allocations.append((p,n));return p
 def ret(self,result=0):
  u=self.u
  for i in [0,1,2,3,12]:u.reg_write(R[i],0xcc000000+i)
  for i in range(16):u.reg_write(UC_ARM_REG_S0+i,0x7fc00000+i)
  u.reg_write(R[0],result&0xffffffff);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 def hook(self,u,pc,size,_):
  self.steps+=1
  if BASE<=pc<BASE+self.extent:return
  if 0x28ba44<=pc<0x28bacc or 0x28f0a0<=pc<0x28f114 or 0x2881e8<=pc<0x28820c :
   if pc==0x2881e8:self.trace.append(('allocatorQuery',bool(u.reg_read(R[0])),bool(u.reg_read(R[1]))))
   return
  a,b,c,d=[u.reg_read(r) for r in R[:4]];result=0
  if pc==0x283d1c:
   self.trace.append(('resource',bool(a),b,c));result=self.case.get('size',len(self.blob))
   if a:u.mem_write(a,self.blob)
  elif pc==0x2848f4:
   self.trace.append(('allocate',a,b));result=0 if self.case.get('null_allocation') else self.alloc(a,b)
  elif pc==0x2847bc:self.trace.append(('free',a))
  elif pc==CALLBACK:
   self.trace.append(('buffer',a,b,c,d));result=self.alloc(d,16)
  elif pc==0x296048:self.trace.append(('flush',a,b))
  elif pc==0x281ddc:
   self.trace.append(('gpuCopy',a,b,c));u.mem_write(b,bytes(u.mem_read(a,c)))
  elif pc in [0x28c890,0x28ca18,0x284894,0x284868,0x2847e4]:self.trace.append(('sync',pc))
  else:raise AssertionError(('unhandled',hex(pc),[hex(x) for x in [a,b,c,d]],self.trace[-4:]))
  self.ret(result)
 def run(self,case):
  self.case=case;self.blob=make_blob(case);u=self.u;u.mem_write(DB,bytes([0xa5])*0x10000);u.mem_write(STACK,bytes(0x20000));self.cursor=HEAP;self.steps=0;self.trace=[];self.allocations=[]
  self.put(RESOURCE+0x67c,case['kind']);self.put(RESOURCE+0x68c,case['flags']);self.put(0x3e2654,CALLBACK,0)
  u.mem_write(0x3ef07c,bytes(case['axes'])+bytes(2));u.mem_write(TRANSLATION,struct.pack('<3f',*case['translation']))
  rng=random.Random(case['seed']+10);pattern=case['occupancy'];u.mem_write(OCC,bytes([(1 if pattern==2 else int(rng.randrange(7)==0) if pattern==1 else 0) for _ in range(256)]))
  u.reg_write(UC_ARM_REG_CPSR,0x10);u.reg_write(UC_ARM_REG_FPSCR,case.get('fpscr',0))
  for i,r in enumerate(R):u.reg_write(r,0xaa000000+i)
  for i,v in enumerate([RESOURCE,case['category'],17,TRANSLATION if case['translate'] else 0]):u.reg_write(R[i],v)
  u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,END);self.put(SP,case['mirror'],OCC if pattern>=0 else 0)
  for i in range(16):u.reg_write(UC_ARM_REG_D0+i,0x123456789abcdef0+i)
  for i,v in enumerate(case['scales']):u.reg_write(UC_ARM_REG_S0+i,struct.unpack('<I',struct.pack('<f',v))[0])
  u.emu_start(BASE,END,count=300000)
  assert u.reg_read(UC_ARM_REG_PC)==END,('incomplete',hex(u.reg_read(UC_ARM_REG_PC)),self.steps)
  assert u.reg_read(UC_ARM_REG_SP)==SP
  assert all(u.reg_read(R[i])==0xaa000000+i for i in range(4,12))
  assert all(u.reg_read(UC_ARM_REG_D0+i)==0x123456789abcdef0+i for i in range(8,16))
  return {'resource':bytes(u.mem_read(RESOURCE,0x800)).hex(),'allocations':[(p,n,bytes(u.mem_read(p,n)).hex()) for p,n in self.allocations], 'events':self.trace,'steps':self.steps,'fpscr':u.reg_read(UC_ARM_REG_FPSCR)}
def case(**kwargs):
 c=dict(category=0,counts=[4,4,4],parts=[(4,12)],wide=0,seed=41,kind=0x10000,flags=0,axes=[0,0,0,1,2,0],translation=[1.25,-2.5,3.75],translate=0,mirror=0,occupancy=-1,scales=[1.,1.]);c.update(kwargs);return c
if __name__=='__main__':
 cases=[]
 for category in range(9):
  for counts in [[0,0,0],[1,1,1],[3,3,3],[4,0,1],[1,5,0],[5,1,4]]:
   for wide in [0,1]:cases.append(case(category=category,counts=counts,wide=wide,seed=len(cases)+41,translate=len(cases)%2,mirror=(len(cases)//2)%2))
 for pattern,wide,kind,flag in itertools.product([-1,0,1,2],[0,1],[0x10000,0x20000,0x30000],[0,0x10000000]):cases.append(case(category=6,parts=[(4,60)],wide=wide,kind=kind,flags=flag,occupancy=pattern,seed=len(cases)+41))
 for permutation in itertools.permutations(range(3)):
  for neg in itertools.product([0,1],repeat=3):cases.append(case(axes=[neg[1],neg[2],neg[0],permutation[1],permutation[2],permutation[0]],translate=1,scales=[.75,1.5],mirror=1,seed=len(cases)+41))
 for category,scales,flag in itertools.product([0,7,8],[[1.25,1.75],[-1.,.5],[1.1,1.1]],[0,8]):cases.append(case(category=category,scales=scales,flags=flag,translate=1,seed=len(cases)+41))
 cases.extend([case(size=-1),case(size=0),case(parts=[]),case(category=2,parts=[]),case(category=5,parts=[]),case(category=6,parts=[(4,0)],occupancy=1),case(parts=[(4,6),(5,9)],category=0)])
 engines=[Engine(False),Engine(True)];maximum=0;events=0;bytes_compared=0
 for n,c in enumerate(cases):
  try:runs=[x.run(c) for x in engines]
  except Exception as error:
   (OUT/'failure.json').write_text(json.dumps({'case':c,'index':n,'error':str(error)},indent=2));raise
  a,b=runs;maximum=max(maximum,a['steps'],b['steps']);events+=len(a['events']);bytes_compared+=0x800+sum(x[1] for x in a['allocations'])
  compare=[k for k in a if k not in ('steps','fpscr')]
  if any(a[k]!=b[k] for k in compare):
   (OUT/'failure.json').write_text(json.dumps({'case':c,'index':n,'retail':a,'candidate':b},indent=2));raise AssertionError(('mismatch',n,[k for k in compare if a[k]!=b[k]]))
  print('PASS',n,a['steps'],b['steps'],flush=True)
 result={'pairs':len(cases),'maximum_instructions':maximum,'ordered_events':events,'compared_bytes':bytes_compared,'candidate_bytes':len(CANDIDATE),'candidate_sha256':hashlib.sha256(CANDIDATE).hexdigest(),'original_callees':['__rt_memcpy and __aeabi_memcpy4','nngxGetAllocator'],'fpscr_initial':0,'limit_per_run':300000,'cases':cases}
 (OUT/'replay-results.json').write_text(json.dumps(result,indent=2));print(json.dumps({k:v for k,v in result.items() if k!='cases'},indent=2))

```

## build/root280798/extended.py

```python
import replay as r
from pathlib import Path
import json,itertools,math
out=r.OUT;cases=[]
for fpscr in [0,1<<24,1<<25,(1<<24)|(1<<25),1<<22,2<<22,3<<22]:
 for scales in [[-0.,0.],[1.1,1.1],[.7,1.23],[2**-140,2**-126],[-2.,-1.],[.00390625,1.99609375]]:
  for category in [0,2,5,6,7,8]:cases.append(r.case(category=category,scales=scales,fpscr=fpscr,translate=1,translation=[-.5,.25,.0],mirror=1,occupancy=1,flags=8,seed=len(cases)+700))
for category in range(9):
 for size in [-10,-1,0]:cases.append(r.case(category=category,size=size))
 cases.append(r.case(category=category,null_allocation=True))
for counts in itertools.product(range(3),repeat=3):cases.append(r.case(counts=list(counts),category=6,occupancy=1,parts=[(4,9)],seed=len(cases)+900))
engines=[r.Engine(False),r.Engine(True)];events=bytes_compared=maximum=0;fpscr_differences=[]
for n,c in enumerate(cases):
 a,b=[x.run(c) for x in engines];maximum=max(maximum,a['steps'],b['steps']);events+=len(a['events']);bytes_compared+=0x800+sum(x[1] for x in a['allocations'])
 keys=[k for k in a if k not in ('steps','fpscr')]
 if any(a[k]!=b[k] for k in keys):
  (out/'extended-failure.json').write_text(json.dumps({'index':n,'case':c,'retail':a,'candidate':b},indent=2));raise AssertionError((n,[k for k in keys if a[k]!=b[k]]))
 if a['fpscr']!=b['fpscr']:fpscr_differences.append({'case_index':n,'retail':a['fpscr'],'candidate':b['fpscr']})
 print('PASS',n,flush=True)
result={'pairs':len(cases),'max_instructions':maximum,'ordered_events':events,'compared_bytes':bytes_compared,'fpscr_differences':fpscr_differences,'cases':cases}
(out/'extended-results.json').write_text(json.dumps(result,indent=2));print(json.dumps({k:v for k,v in result.items() if k!='cases'},indent=2))

```

## build/root280798/preserve.py

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection
import json,hashlib
root=Path.cwd();base=root.parent/'mario-main45a';out=root/'build/root280798'
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
 if rel.parts[0] not in ('Game','lib') or p.stem=='cfl_MeshResource':continue
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

