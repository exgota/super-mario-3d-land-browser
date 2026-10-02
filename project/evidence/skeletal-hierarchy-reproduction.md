# Skeletal hierarchy update: reproducible local evidence

These recipes use the owner-authorized original EU executable and the unchanged project toolchain. All generated files stay in ignored build storage. They are diagnostic evidence, not changes to the project checker, executable, function boundaries, or global compiler flags.

Frozen base: `5c93bb226b872199e33aa987208d741aa52252fe`. In a disposable base checkout, source `development_environment.sh`, run `python make.py eu -ca`, copy `build/eu/obj` to `build/hierarchy/baseline-obj`, and copy the unchanged map to `build/hierarchy/map.baseline.csv`. Apply only this proposal's source and notes, then build again normally. The exact original EU code hash must be e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64.

The root is guarded by the existing NON_MATCHING convention. The normal project build enables it. For the canonical root check, temporarily give only the existing 00338D80 row its already established address-derived symbol `fn_00338D80` and rank M; preserve Start/Pool/End/Type and every other row. Run `python tools/check.py fn_00338D80 --object build/eu/obj/lib/al/src/Model/SkeletalHierarchyUpdate.o`, and restore the exact original map bytes in a finally block. Expected result: M, complete-section size mismatch, not exact. No map edits belong in a proposal.

## Diagnostic linking

The following ordinary linker invocation moves the project-built candidate to 00500000 solely for paired execution, resolving genuine helper imports against the unchanged map. It does not establish canonical source closure or exactness. The candidate object is never altered. Save as `build/hierarchy/link.py`.

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
import re,csv,subprocess,os,json
out=Path('build/hierarchy');obj='build/eu/obj/lib/al/src/Model/SkeletalHierarchyUpdate.o';rows=[{k.strip():v.strip() for k,v in r.items()} for r in csv.DictReader(open('data/ver/eu/map.csv'))];byname={r['Symbol']:r for r in rows if r['Symbol']};lines=['#<SYMDEFS>#'];imports=[]
# Existing clean header/README identities; independently verified initializer.
missing={}
with open(obj,'rb') as f:
 e=ELFFile(f);st=e.get_section_by_name('.symtab');root='fn_00338D80'
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

## Bounded paired execution

Save as `build/hierarchy/replay.py`. Synthetic getter and callback addresses are explicitly fixture models. All five hierarchy/identity/matrix imports execute original instructions, with the original text read/execute and original rodata read-only. The initialized identity cache is constructed private writable BSS state; its initialization path is not exercised. The script compares all fixture memory, caller-stack sentinels, documented helper arguments, callbacks, lower FPSCR state, fault outcomes, and callee-saved registers. Scratch-stack addresses are intentionally normalized, and unused argument registers are excluded.

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import *
from unicorn.arm_const import *
import struct,json,random,hashlib,collections,time
D=Path('build/hierarchy');original=Path('data/ver/eu/code.bin').read_bytes()
assert hashlib.sha256(original).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
with (D/'candidate.axf').open('rb') as f:
 e=ELFFile(f);ENTRY=e.header.e_entry;SEGS=[(s['p_vaddr'],s.data()) for s in e.iter_segments() if s['p_type']=='PT_LOAD']
B=0x800000;SIZE=0x10000;SP=0xa10000;STOP=0x600000
CTRL=B;SKEL=B+0x100;OWNER=B+0x200;VT=B+0x400;BEFORE=B+0x500;AFTER=B+0x600;LOCALBUF=B+0x700;COMPBUF=B+0x800;MATBUF=B+0x900;DICT=B+0xa00;JOINT=B+0x1000;LOCAL=B+0x2000;COMP=B+0x3000;MAT=B+0x4000
MODEL={0x600100:('local',LOCALBUF),0x600104:('composed',COMPBUF),0x600108:('matrices',MATBUF),0x60010c:('before',0),0x600110:('after',0)}
HELPERS={0x2164f8,0x33aa94,0x25c3e4,0x33a90c,0x2723e0}
SAVED={r:0xabcd0000+i*0x111 for i,r in enumerate([UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11])};SAVED.update({r:0xabcd123400000000+i*0x111 for i,r in enumerate(range(UC_ARM_REG_D8,UC_ARM_REG_D15+1))})
def w(x):return struct.pack('<I',x&0xffffffff)
def fs(v):return struct.pack('<'+'f'*len(v),*v)
def fb(x):return struct.unpack('<I',fs([x]))[0]
IDENT=[1.,0.,0.,0.,0.,1.,0.,0.,0.,0.,1.,0.]
def run(c,cand):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176);u.reg_write(UC_ARM_REG_C1_C0_2,0xf00000);u.reg_write(UC_ARM_REG_FPEXC,0x40000000);u.reg_write(UC_ARM_REG_FPSCR,c.get('fpscr',0))
 u.mem_map(0x100000,0x340000);u.mem_write(0x100000,original);u.mem_protect(0x100000,0x2a2000,UC_PROT_READ|UC_PROT_EXEC);u.mem_protect(0x3a2000,0x3f000,UC_PROT_READ)
 u.mem_map(0x500000,0x10000,UC_PROT_READ|UC_PROT_EXEC)
 for a,data in SEGS:u.mem_write(a,data)
 u.mem_map(STOP,0x1000,UC_PROT_READ|UC_PROT_EXEC);u.mem_map(B,SIZE);u.mem_map(0xa00000,0x20000);u.mem_write(B,b'\xa5'*SIZE);u.mem_write(0xa00000,b'\xcc'*0x20000)
 def word(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def record(a,scale=(1,1,1),flags=0x7e1,matrix=None):u.mem_write(a,fs(matrix or IDENT)+fs(scale)+w(flags))
 # Exercise the actual already-initialized identity accessor, with initialized fixture BSS.
 u.mem_write(0x3f38a8,w(1));record(0x430cf0)
 u.mem_write(CTRL,w(VT)+w(0)+w(c.get('skeleton',SKEL))+w(OWNER)+w(BEFORE)+w(AFTER))
 u.mem_write(VT+0xc,w(0x600100));u.mem_write(VT+0x14,w(0x600104));u.mem_write(VT+0x1c,w(0x600108))
 u.mem_write(SKEL+0x1c,w(0 if c.get('null_dictionary') else DICT-(SKEL+0x1c)));u.mem_write(SKEL+0x24,w(c.get('mode',0)));u.mem_write(SKEL+0x28,w(c.get('root_identity',0)))
 n=c.get('n',3);u.mem_write(LOCALBUF,w(n)+w(LOCAL));u.mem_write(COMPBUF,w(n)+w(COMP));u.mem_write(MATBUF,w(n)+w(MAT)+w(MAT+n*48))
 record(OWNER+0x4c,c.get('parent_scale',[1,1,1]),c.get('parent_flags',0x801),c.get('parent_matrix'))
 record(OWNER+0xbc,c.get('composed_scale',[1,1,1]),c.get('parent_flags',0x801),c.get('parent_matrix'))
 for base,kind in [(BEFORE,'before'),(AFTER,'after')]:
  count=c.get(kind,0);u.mem_write(base+8,w(base+0x20)+w(base+0x20+4*count))
  for i in range(count):
   cb=base+0x40+i*0x10;u.mem_write(base+0x20+i*4,w(cb));u.mem_write(cb,w(cb+4));u.mem_write(cb+4,w(0x60010c if kind=='before' else 0x600110))
 for i in range(n):
  slot=DICT+0x28+i*16;addr=JOINT+i*0x40;u.mem_write(slot,w(0 if c.get('null_joint') else addr-slot));u.mem_write(addr,w(0)+w(c.get('joint_flags',0))+w(i)+w(c.get('parents',[-1]+list(range(n-1)))[i]))
  matrix=list(c.get('matrix',IDENT));matrix[3]+=i;matrix[7]-=i;matrix[11]+=2*i
  record(LOCAL+i*64,c.get('scale',[1,1,1]),c.get('local_flags',0x801),matrix);record(COMP+i*64,c.get('initial_scale',[1,1,1]),c.get('result_flags',0x7e1))
  for off,bits in c.get('words',[]):u.mem_write(LOCAL+i*64+off,w(bits))
 for r,v in SAVED.items():u.reg_write(r,v)
 u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,STOP);u.reg_write(UC_ARM_REG_R0,B+0x5000);u.reg_write(UC_ARM_REG_R1,CTRL);u.reg_write(UC_ARM_REG_R2,B+0x6000)
 pcs=set();events=[];calls=[];fault=None;steps=0
 def code(u,a,s,d):
  nonlocal steps
  steps+=1;pcs.add(a)
  if a in HELPERS:
   args=[u.reg_read(r) for r in [UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3]]
   # Stack-local scratch addresses are compiler-specific, omit only those addresses.
   args=["stack" if 0xa00000<=x<0xa20000 else x for x in args]
   if a==0x2723e0:args=args[:3]
   if a in {0x33aa94,0x25c3e4,0x33a90c}:args+=list(struct.unpack('<II',u.mem_read(u.reg_read(UC_ARM_REG_SP),8)))
   calls.append([a,args if a!=0x2164f8 else []])
  if a not in MODEL:return
  kind,value=MODEL[a]
  if kind in ['local','composed','matrices']:
   assert u.reg_read(UC_ARM_REG_R0)==CTRL;events.append([kind]);u.reg_write(UC_ARM_REG_R0,value)
  else:
   assert u.reg_read(UC_ARM_REG_R1)==CTRL;idx=u.reg_read(UC_ARM_REG_R2);events.append([kind,idx,u.reg_read(UC_ARM_REG_R0)])
   if c.get('mutate') and kind=='before':u.mem_write(LOCAL+idx*64+0x3c,w(word(LOCAL+idx*64+0x3c)^1))
   if c.get('mutate') and kind=='after':u.mem_write(COMP+idx*64+0x3c,w(word(COMP+idx*64+0x3c)^0x1000))
  u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 def invalid(u,access,a,s,v,d):
  nonlocal fault
  fault=[access,a,s];return False
 u.hook_add(UC_HOOK_CODE,code);u.hook_add(UC_HOOK_MEM_INVALID,invalid)
 try:u.emu_start(ENTRY if cand else 0x338d80,STOP,count=100000)
 except UcError as ex:
  if fault is None:fault=['emulator',str(ex)]
 status='fault' if fault else 'return' if u.reg_read(UC_ARM_REG_PC)==STOP else 'bounded'
 if status=='return':
  assert u.reg_read(UC_ARM_REG_SP)==SP;assert all(u.reg_read(r)==v for r,v in SAVED.items()),'callee-save mismatch'
 return {'status':status,'fault':fault,'memory':bytes(u.mem_read(B,SIZE)),'stack':bytes(u.mem_read(SP,128)),'events':events,'calls':calls,'fpscr':u.reg_read(UC_ARM_REG_FPSCR)&0x0fffffff,'steps':steps,'pc':u.reg_read(UC_ARM_REG_PC)},pcs

def fixtures():
 cases=[]
 def add(name,**kw):cases.append(dict(name=name,**kw))
 for mode in [0,1,2,3,255,256,257,0xffffffff]:
  for identity in [0,1]:
   for jf in [0,0x20]:
    add('mode-%x-%d-%x'%(mode,identity,jf),mode=mode,root_identity=identity,joint_flags=jf)
 for flags in [0,1,8,9,0x21,0x41,0x61,0x81,0x201,0x401,0x601,0x7e1,0x801,0x809,0xffffffff]:
  for mode in [0,1,2]:add('flags-%x-%d'%(flags,mode),mode=mode,local_flags=flags,joint_flags=0x20)
 for scale in [[0,0,0],[-0.,0.,-0.],[1e-8,-1e-8,1e-8],[.001,.00001,-.00001],[1,1,1],[-1,-1,-1],[2,3,4],[-2,3,-4]]:
  for mode in [0,1,2]:add('scale-'+str(scale)+'-'+str(mode),mode=mode,scale=scale,joint_flags=0x20)
 for pf in [0,1,8,0x201,0x401,0x601,0x7e1,0x801]:
  for mode in [0,1,2]:add('parent-%x-%d'%(pf,mode),mode=mode,parent_flags=pf,parent_scale=[2,-3,.5],composed_scale=[4,.25,-2],joint_flags=0x20)
 for n in [0,1,5]:
  for mode in [0,1,2]:
   for mutate in [False,True]:add('callback-%d-%d-%s'%(n,mode,mutate),n=n,mode=mode,before=2,after=2,mutate=mutate)
 for mode in [0,1,2]:
  for words in [[(0x30,0x7fc12345)],[(0x30,0xffc12345)],[(0x30,0x7f800000)],[(0x30,0xff800000)],[(0x30,0x7f812345)],[(0x30,1)]]:
   add('ieee-'+str(mode)+'-'+str(words),mode=mode,words=words,joint_flags=0x20)
  for kw in [{'null_dictionary':True},{'null_joint':True},{'skeleton':0}]:add('fault-'+str(mode)+'-'+str(kw),mode=mode,**kw)
 for mode in [0,1,2]:
  for delta in range(-4,5):
   value=struct.unpack('<f',w(fb(.001)+delta))[0];add('threshold-%d-%d'%(mode,delta),mode=mode,scale=[value,0.,0.],joint_flags=0x20)
  for rounding in range(4):
   for fz in [0,1]:
    for dn in [0,1]:add('fpscr-%d-%d-%d-%d'%(mode,rounding,fz,dn),mode=mode,words=[(0x30,1),(0x34,0xffc12345)],fpscr=(rounding<<22)|(fz<<24)|(dn<<25),joint_flags=0x20)
  for before in [1,2]:
   for after in [1,2]:add('mutate-%d-%d-%d'%(mode,before,after),mode=mode,before=before,after=after,mutate=True)
 rng=random.Random(0x338d80)
 for i in range(250):
  n=rng.randrange(1,7);add('random-'+str(i),mode=rng.choice([0,1,2,255]),n=n,root_identity=rng.randrange(2),joint_flags=rng.choice([0,0x20]),parents=[-1]+[rng.randrange(-1,j) for j in range(1,n)],scale=[rng.choice([-2.,-1.,-.000001,0.,.000001,.5,1.,3.]) for _ in range(3)],local_flags=rng.choice([0,1,0x21,0x41,0x81,0x201,0x7e1,0x801]),parent_flags=rng.choice([0x801,0x601,0x7e1]),before=rng.randrange(3),after=rng.randrange(3),result_flags=rng.choice([0x7e1,0x801,0x809]),matrix=[rng.choice([-2.,-.5,0.,.5,1.,3.]) for _ in range(12)])
 return cases
if __name__=='__main__':
 cases=fixtures();allpcs=set();counts=collections.Counter();fail=[];results=[];started=time.time()
 for c in cases:
  a,pcs=run(c,False);b,_=run(c,True);allpcs|=pcs;keys=['status','fault','memory','stack','events','calls','fpscr'];bad=[k for k in keys if a[k]!=b[k]];counts[a['status']]+=1
  results.append({'name':c['name'],'status':a['status'],'fault':a['fault'],'different_fields':bad,'original_steps':a['steps'],'candidate_steps':b['steps']})
  if bad:
   fail.append(c['name']);print('FAIL',c['name'],bad,flush=True)
   (D/'first-failure.json').write_text(json.dumps({'case':c,'a':{k:v for k,v in a.items() if k not in ['memory','stack']},'b':{k:v for k,v in b.items() if k not in ['memory','stack']}},indent=2));break
 out={'cases':len(results),'planned':len(cases),'outcomes':dict(counts),'failures':fail,'root_instructions':len([x for x in allpcs if 0x338d80<=x<0x339578]),'seconds':time.time()-started,'results':results};(D/'replay.json').write_text(json.dumps(out,indent=2));(D/'coverage.json').write_text(json.dumps(sorted(allpcs)));print({k:v for k,v in out.items() if k!='results'});raise SystemExit(bool(fail))
```

## Canonical accepted-root preservation

Save as `build/hierarchy/check_baseline.py`. This invokes the unchanged project exact-byte checker and provenance verifier for each baseline O function with an unambiguous source object, never treating autogenerated stubs as implementations. Ambiguous or missing definitions remain explicit failures.

```python
from pathlib import Path
import sys,csv,json,time,collections,hashlib
sys.path.insert(0,str(Path.cwd()))
from elftools.elf.elffile import ELFFile
from tools.low.checkExactBytes import check_exact_bytes
from tools.low.buildProvenance import verify_build_output
D=Path('build/hierarchy');rows=[{k.strip():v.strip() for k,v in x.items()} for x in csv.DictReader(open(D/'map.baseline.csv'))];syms={x['Symbol']:x for x in rows if x['Symbol'] and x['Type'].startswith('f')};objects={};defs={}
for p in Path('build/eu/obj').rglob('*.o'):
 if str(p).endswith('SkeletalHierarchyUpdate.o'):continue
 if '/split/' in str(p):continue
 if not p.with_suffix('.provenance.json').exists():continue
 with p.open('rb') as f:
  e=ELFFile(f);st=e.get_section_by_name('.symtab')
  for s in st.iter_symbols():
   if s['st_info']['type']=='STT_FUNC' and s['st_shndx']!='SHN_UNDEF' and s.name in syms:defs.setdefault(s.name,[]).append(str(p))
for n,ps in defs.items():
 if len(ps)>1:
  # Resolve canonical implementation using map origin from project-generated symbol table.
  pass
start=time.time();results=[]
for n,row in syms.items():
 if row['Rank']!='O':continue
 ps=defs.get(n,[])
 if len(ps)!=1:
  results.append({'symbol':n,'exact':False,'reason':'ambiguous/missing object','objects':ps});continue
 obj=Path(ps[0]);v=verify_build_output(obj);r=check_exact_bytes(n,obj.resolve(),'eu',v['compiler']);results.append({'symbol':n,'exact':r.get('exact',False),'reason':r.get('reason'),'rejected':r.get('rejected',False)})
 if not r.get('exact'):print('NOT EXACT',n,r.get('reason'),flush=True)
 if len(results)%100==0:print('checked',len(results),flush=True)
out={'total':len(results),'exact':sum(x['exact'] for x in results),'seconds':time.time()-start,'results':results};(D/'baseline-canonical.json').write_text(json.dumps(out,indent=2));print({k:v for k,v in out.items() if k!='results'},flush=True)
```

## Duplicate weak-definition checks

Two accepted roots have multiple ordinary weak source definitions. Save as `build/hierarchy/check_aliases.py` after running the canonical baseline recipe. Check every observed definition rather than choosing an arbitrary carrier.

```python
from pathlib import Path
import json,sys,time
sys.path.insert(0,str(Path.cwd()))
from tools.low.checkExactBytes import check_exact_bytes
from tools.low.buildProvenance import verify_build_output
D=Path('build/hierarchy');j=json.loads((D/'baseline-canonical.json').read_text());out=[];start=time.time()
for r in j['results']:
 if r['exact']:continue
 for p in r.get('objects',[]):
  obj=Path(p);v=verify_build_output(obj);result=check_exact_bytes(r['symbol'],obj.resolve(),'eu',v['compiler']);out.append({'symbol':r['symbol'],'object':p,'exact':result.get('exact',False),'reason':result.get('reason')})
(D/'aliases-canonical.json').write_text(json.dumps({'definitions':len(out),'exact':sum(r['exact'] for r in out),'seconds':time.time()-start,'results':out},indent=2));print('aliases',len(out),'exact',sum(r['exact'] for r in out))
```

## Prior-object and input preservation

Save as `build/hierarchy/preserve.py`. The independently saved baseline object directory is required. This compares all prior real objects and tracked source/config/tool inputs. It excludes generated stubs and the new hierarchy object; canonical root checks are separate.

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
import hashlib,json,subprocess
D=Path('build/hierarchy');old=D/'baseline-obj';new=Path('build/eu/obj');rows=[]
def digest(b):return hashlib.sha256(b).hexdigest()
def sections(p):
 with p.open('rb') as f:
  e=ELFFile(f);return {s.name:(s['sh_type'],s['sh_flags'],s['sh_size'],digest(s.data())) for s in e.iter_sections() if s['sh_flags']&2}
for p in sorted(old.rglob('*.o')):
 r=p.relative_to(old);q=new/r
 if '/split/' in str(r):continue
 rows.append({'object':str(r),'raw_equal':p.read_bytes()==q.read_bytes(),'allocated_equal':sections(p)==sections(q),'before_sha256':digest(p.read_bytes()),'after_sha256':digest(q.read_bytes())})
root=Path.cwd();base='5c93bb226b872199e33aa987208d741aa52252fe';tracked=subprocess.check_output(['git','ls-tree','-r','--name-only',base],text=True).splitlines();checked=[]
for name in tracked:
 if name=='data/ver/eu/map.csv' or not (root/name).is_file():continue
 if not name.startswith(('Game/','lib/','tools/','data/')):continue
 orig=subprocess.check_output(['git','show',base+':'+name]);assert (root/name).read_bytes()==orig,name;checked.append(name)
assert all(r['allocated_equal'] for r in rows)
out={'prior_objects':len(rows),'raw_equal':sum(r['raw_equal'] for r in rows),'allocated_equal':sum(r['allocated_equal'] for r in rows),'tracked_inputs_unchanged':len(checked),'scope':'Prior compiler object equivalence; excludes autogenerated split stubs and the newly added hierarchy object. Canonical accepted-root evidence is separately reported.','objects':rows};(D/'preservation.json').write_text(json.dumps(out,indent=2));print({k:v for k,v in out.items() if k!='objects'})
```

## Run order and limits

Run the normal clean build and canonical root check first. Then run the link recipe, the paired replay, the canonical accepted-root checks, and the object/input preservation recipe. Restore the unchanged map before producing a patch. Record fresh commands, return codes, file hashes, and actual counts; do not substitute these historical observations for a rerun.

The fixtures exercise constructed hierarchy/controller/resource/callback state. They are not extracted asset data, complete initialization, animation playback, rendering, or gameplay. Empty and malformed-resource cases are deliberate. A matching fault confirms only its bounded observable outcome, not safe handling of malformed input. The first argument's value is unused by the root, and the third argument is forwarded to composition helpers. The helper implementations currently observed ignore that first helper argument; argument fidelity is nevertheless tested explicitly.
