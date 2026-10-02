# Reproducing the local shader command-list proposal

Freeze main f24e61c39726fafa9041462f603c6a6ea52cb829, then apply this family patch.
Use the owner-supplied code.bin (SHA-256 e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64)
and local licensed compiler setup. Never commit those inputs or generated objects.
Source development_environment.sh before project commands.

Before adding/enrolling this source, perform a clean project build and record
SHA-256 hashes for genuine compiler-produced objects, excluding the generated
split-stub family, into build/root2ca5/baseline-objects.json. Save the unchanged
map at build/root2ca5/map.baseline.csv. This baseline build used all frozen O roots.
Temporarily enroll only 002CA5E4 with its existing boundary and pool as scratch M,
and use the ordinary fallback name fn_002CA5E4. Do not commit this scratch edit.

After committing the source, run python make.py eu -ca, then:
python tools/check.py fn_002CA5E4 --object build/eu/obj/lib/al/src/Graphics/ShaderCommandLists.o
The reported size rejection is the canonical outcome, not an exact match.

The scripts below are diagnostic recipes, never replacement project tools.
Copy them into ignored build/root2ca5 with the indicated names and run them there
from the repository root using the project Python environment. The linker uses
the unchanged project-produced object and original mapped imports at a scratch
relocation base. It neither changes the object nor earns checker credit.
The paired replay compares emitted command bytes, modeled call events, object
and allocated memory, selected globals, entry-stack preservation, callee-saved
integer registers, and d8. Caller-save integer and VFP registers are clobbered.
It supplies bounded constructed inputs and modeled import behavior, not the
original helper bodies. It does not prove behavior for arbitrary aliasing,
malformed indices, concurrency, the full input domain, GPU execution or gameplay.

The preservation recipe enumerates every frozen O root and every actual defining
object, including weak definitions. Each genuine object must pass the unchanged
project provenance checker before check_exact_bytes. The script does not set
ranks. It additionally hashes every original genuine object against the baseline.
The generated split stub is expected to differ for temporary enrollment and is
excluded from object-equality totals. Full linked-image equality is not claimed.
Restore map.baseline.csv to data/ver/eu/map.csv after checks.

## link.py

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
import re,csv,subprocess,os,json
out=Path('build/root2ca5');obj='build/eu/obj/lib/al/src/Graphics/ShaderCommandLists.o';rows=[{k.strip():v.strip() for k,v in r.items()} for r in csv.DictReader(open('data/ver/eu/map.csv'))];byname={r['Symbol']:r for r in rows if r['Symbol']};lines=['#<SYMDEFS>#'];imports=[]
# Existing clean header/README identities; independently verified initializer.
missing={}
with open(obj,'rb') as f:
 e=ELFFile(f);st=e.get_section_by_name('.symtab');root='fn_002CA5E4'
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

## replay.py

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import *
from unicorn.arm_const import *
import struct,json,random,hashlib,time,collections
D=Path('build/root2ca5'); original=Path('data/ver/eu/code.bin').read_bytes()
assert hashlib.sha256(original).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
with (D/'candidate.axf').open('rb') as f:
 e=ELFFile(f);ENTRY=e.header.e_entry;SEGS=[(s['p_vaddr'],s.data()) for s in e.iter_segments() if s['p_type']=='PT_LOAD']
B=0x800000;SIZE=0x20000;SELF=B;VERT=B+0x1000;CMD=B+0x4000;BIN=B+0x10000;PROG=BIN+40;SHADER=BIN+0x100;SP=0xa10000;STOP=0x600000
HELPERS={0x28ba44,0x296048,0x298460,0x2284b0,0x29848c,0x10b2bc}
SAVED={r:0xabcd0000+i*0x111 for i,r in enumerate([UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11])}
def w(x):return struct.pack('<I',x&0xffffffff)
def run(c,cand):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176);u.reg_write(UC_ARM_REG_C1_C0_2,0xf00000);u.reg_write(UC_ARM_REG_FPEXC,0x40000000)
 u.mem_map(0x100000,0x340000);u.mem_write(0x100000,original);u.mem_protect(0x100000,0x2a2000,UC_PROT_READ|UC_PROT_EXEC);u.mem_protect(0x3a2000,0x3f000,UC_PROT_READ)
 u.mem_map(0x500000,0x10000,UC_PROT_READ|UC_PROT_EXEC)
 for a,data in SEGS:u.mem_write(a,data)
 u.mem_map(STOP,0x1000,UC_PROT_READ|UC_PROT_EXEC);u.mem_map(B,SIZE);u.mem_map(0xa00000,0x20000);u.mem_write(B,b'\xa5'*SIZE);u.mem_write(0xa00000,b'\xcc'*0x20000)
 def word(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def setword(a,v):u.mem_write(a,w(v))
 rng=random.Random(c.get('seed',7));n=c.get('code',0);m=c.get('desc',0);constants=c.get('constants',[]);regs=c.get('regs',[])
 u.mem_write(0x3f03e0,bytes([c.get('initialized',0)]));u.mem_write(0x429af0,b'\x77'*0x9c0)
 setword(BIN+4,8);setword(BIN+8,0x100)
 for off,v in [(8,BIN+0x1000-PROG),(12,n),(16,BIN+0x2000-PROG),(20,m)]:setword(PROG+off,v)
 for i in range(n):setword(BIN+0x1000+i*4,rng.getrandbits(32))
 for i in range(m*2):setword(BIN+0x2000+i*4,rng.getrandbits(32))
 for off,v in [(8,c.get('shader_id',0)),(24,BIN+0x3000-SHADER),(28,len(constants)),(40,BIN+0x4000-SHADER),(44,len(regs))]:setword(SHADER+off,v)
 u.mem_write(SHADER+18,struct.pack('<H',c.get('entry',0)))
 for i,values in enumerate(constants):u.mem_write(BIN+0x3000+i*20,struct.pack('<HHIIII',0,*values))
 for i,values in enumerate(regs):u.mem_write(BIN+0x4000+i*8,struct.pack('<HHHH',*values,0))
 for r,v in SAVED.items():u.reg_write(r,v)
 u.reg_write(UC_ARM_REG_D8,0x123456789abcdef0);u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,STOP)
 for r,v in zip([UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3],[SELF,VERT,CMD,0 if c.get('null') else BIN]):u.reg_write(r,v)
 setword(SP,0xabcdef12);setword(SP+4,c.get('upload',0))
 pcs=set();events=[];fault=None
 def code(u,a,s,d):
  pcs.add(a)
  if a not in HELPERS:return
  r=[u.reg_read(reg) for reg in [UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3]];ret=0;args=[]
  if a==0x28ba44:
   data=bytes(u.mem_read(r[1],r[2]));u.mem_write(r[0],data);args=[r[0],r[1],r[2],hashlib.sha256(data).hexdigest()]
  elif a==0x296048:args=r[:2]
  elif a==0x298460:
   args=r;u.mem_write(r[0],w(r[1])+w(0)+w(r[2])+w(r[3]))
  elif a==0x2284b0:
   data=bytes(u.mem_read(r[1],r[2]));args=[r[0],r[2],data.hex()];base=word(r[0]);position=word(r[0]+4);u.mem_write(base+position,data);setword(r[0]+4,position+r[2])
  elif a==0x29848c:
   args=r[:2];position=word(r[0]+4);rounded=(position+r[1]-1)&~(r[1]-1);u.mem_write(word(r[0])+position,b'\0'*(rounded-position));setword(r[0]+4,rounded)
  elif a==0x10b2bc:args=r[:1];ret=c.get('physical',0x12345678)
  events.append([hex(a),args])
  for reg,value in [(UC_ARM_REG_R1,0xdead0001),(UC_ARM_REG_R2,0xdead0002),(UC_ARM_REG_R3,0xdead0003),(UC_ARM_REG_R12,0xdead000c)]:u.reg_write(reg,value)
  for reg in [UC_ARM_REG_D0,UC_ARM_REG_D1,UC_ARM_REG_D2,UC_ARM_REG_D3,UC_ARM_REG_D4,UC_ARM_REG_D5,UC_ARM_REG_D6,UC_ARM_REG_D7]:u.reg_write(reg,0xabcdef0123456789)
  u.reg_write(UC_ARM_REG_R0,ret);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 def invalid(u,access,a,s,v,d):
  nonlocal fault
  fault=[access,a,s];return False
 u.hook_add(UC_HOOK_CODE,code);u.hook_add(UC_HOOK_MEM_INVALID,invalid)
 try:u.emu_start(ENTRY if cand else 0x2ca5e4,STOP,count=50000)
 except UcError as ex:
  if fault is None:fault=['emulator',str(ex)]
 status='fault' if fault else 'return' if u.reg_read(UC_ARM_REG_PC)==STOP else 'bounded'
 if status=='return':
  assert u.reg_read(UC_ARM_REG_SP)==SP;assert all(u.reg_read(r)==v for r,v in SAVED.items()),'callee-save mismatch';assert u.reg_read(UC_ARM_REG_D8)==0x123456789abcdef0
 return {'status':status,'fault':fault,'memory':bytes(u.mem_read(B,SIZE)).hex(),'globals':[bytes(u.mem_read(0x429af0,0x9c0)).hex(),bytes(u.mem_read(0x3f03e0,1)).hex()],'stack':bytes(u.mem_read(SP,128)).hex(),'events':events},pcs

def fixtures():
 cases=[]
 def add(name,**kw):cases.append(dict(name=name,**kw))
 for upload in [0,1]:
  for initialized in [0,1]:add('upload-%s-%s'%(upload,initialized),upload=upload,initialized=initialized)
 for n in [0,1,2,127,128,129,255,256,257]:
  for m in [0,1,127,128,129]:add('chunks-%s-%s'%(n,m),code=n,desc=m)
 for semantic in range(8):
  for mask in range(16):add('semantic-%s-mask-%s'%(semantic,mask),regs=[(semantic,semantic%7,mask)])
 add('skip-semantic',regs=[(9,0,65535)])
 add('register-cap',regs=[(i%8,i%7,15) for i in range(9)])
 add('null',null=True)
 for address in [0,1,15,16,17,0x7fffffff,0x80000000,0xffffffff]:add('physical-'+str(address),physical=address)
 rng=random.Random(0x2ca5e4)
 for i in range(80):add('random-'+str(i),seed=i,code=rng.randrange(300),desc=rng.randrange(140),constants=[(rng.randrange(65536),*[rng.getrandbits(32) for _ in range(4)]) for _ in range(rng.randrange(8))],regs=[(rng.choice(list(range(8))+[9]),rng.randrange(7),rng.randrange(65536)) for _ in range(rng.randrange(12))],physical=rng.getrandbits(32),shader_id=rng.getrandbits(32),entry=rng.randrange(65536),upload=rng.randrange(2),initialized=rng.randrange(2))
 return cases
if __name__=='__main__':
 start=time.time();pcs=set();results=[]
 for c in fixtures():
  a,pa=run(c,False);b,pb=run(c,True);pcs|=pa
  if a!=b:
   (D/'first-failure.json').write_text(json.dumps({'case':c,'original':a,'candidate':b},indent=2));print('FAIL',c['name'],[k for k in a if a[k]!=b[k]]);raise SystemExit(1)
  results.append({'name':c['name'],'status':a['status'],'events':len(a['events'])})
 root=set(range(0x2ca5e4,0x2ca9bc,4))|set(range(0x2ca9f0,0x2cad94,4));out={'cases':len(results),'statuses':dict(collections.Counter(r['status'] for r in results)),'root_visited':len(root&pcs),'root_total':len(root),'unvisited':[hex(a) for a in sorted(root-pcs)],'seconds':time.time()-start,'results':results};(D/'replay.json').write_text(json.dumps(out,indent=2));print({k:v for k,v in out.items() if k!='results'})
```

## preserve.py

```python
from pathlib import Path
import sys,csv,json,time,hashlib,subprocess
sys.path.insert(0,str(Path.cwd()))
from elftools.elf.elffile import ELFFile
from tools.low.checkExactBytes import check_exact_bytes
from tools.low.buildProvenance import verify_build_output
D=Path('build/root2ca5');rows=[{k.strip():v.strip() for k,v in x.items()} for x in csv.DictReader(open(D/'map.baseline.csv'))];syms={x['Symbol']:x for x in rows if x['Symbol'] and x['Type'].startswith('f') and x['Rank']=='O'};defs={}
for p in Path('build/eu/obj').rglob('*.o'):
 if p.name=='ShaderCommandLists.o' or '/split/' in str(p):continue
 if not p.with_suffix('.provenance.json').exists():continue
 with p.open('rb') as f:
  e=ELFFile(f);st=e.get_section_by_name('.symtab')
  for s in st.iter_symbols():
   if s['st_info']['type']=='STT_FUNC' and s['st_shndx']!='SHN_UNDEF' and s.name in syms:defs.setdefault(s.name,[]).append(str(p))
start=time.time();results=[]
for n,row in syms.items():
 ps=defs.get(n,[])
 if not ps:results.append({'symbol':n,'exact':False,'reason':'missing object'});continue
 for p in ps:
  obj=Path(p);v=verify_build_output(obj);r=check_exact_bytes(n,obj.resolve(),'eu',v['compiler']);results.append({'symbol':n,'object':p,'exact':r.get('exact',False),'reason':r.get('reason'),'rejected':r.get('rejected',False)})
  if not r.get('exact'):print('NOT EXACT',n,r.get('reason'),flush=True)
 if len(results)%100==0:print('checked',len(results),flush=True)
old=json.loads((D/'baseline-objects.json').read_text());unchanged={p:hashlib.sha256(Path(p).read_bytes()).hexdigest()==h for p,h in old.items()};out={'roots':len(syms),'checks':len(results),'exact':sum(x['exact'] for x in results),'seconds':time.time()-start,'unchanged_objects':sum(unchanged.values()),'baseline_objects':len(old),'changed_objects':[p for p,v in unchanged.items() if not v],'results':results};(D/'preservation.json').write_text(json.dumps(out,indent=2));print({k:v for k,v in out.items() if k!='results'},flush=True)
```
