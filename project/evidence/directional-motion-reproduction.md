# Reproducing the local directional-motion proposal

Freeze main 3d17628733ab941c39229f725ea9f496fe5dde70 and apply this family patch.
Use the owner's code.bin with SHA-256 e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64
and the repository's existing local compiler setup. No binary, compiler or
produced object belongs in the source proposal. Source development_environment.sh.

Before adding the candidate, build frozen source cleanly and record genuine
compiler-object SHA-256 hashes, excluding generated split stubs, in
build/root1a3f/baseline-objects.json. Save the frozen map as map.baseline.csv
there. Enroll existing root 001A3F08 as scratch M using ordinary fallback name
fn_001A3F08. Do not change its boundary, type, section or embedded-pool marker.
Commit the source, run python make.py eu -ca, then:

python tools/check.py fn_001A3F08 --object build/eu/obj/lib/al/src/Movement/DirectionalMotionUpdate.o

The complete-section size rejection is the canonical result. Restore the exact
map afterward. Scratch ranks and names must not enter the source patch.

The recipes below belong in ignored build/root1a3f and run from the repository
root with the project Python. They are diagnostics, not replacement tools. The
linker uses the unchanged genuine compiler object at a scratch relocation base,
with imports bound to mapped original addresses. It does not earn exactness.

Paired replay runs both original and compiler-produced root machine code.
Direct imports and virtual methods are explicit fixtures; none of their original
bodies execute. Vector helper fixtures implement deterministic float32 operations;
quaternion construction returns controlled finite inputs. Root-local arithmetic,
branches, bit tests and square roots execute as machine instructions. Calls are
compared with exact float argument bits, not decimal tolerances. Caller-save
integer/VFP registers are clobbered; callee-saved r4-r11 and d8-d15, entry-stack
bytes, controller/motion memory and helper event sequences are checked.

Fixtures cover inactive/active input, both mode queries, positive/negative speed,
threshold/counter boundaries, reset, direction reversal, interpolation and
acceleration/deceleration paths, fixed quaternion cases, missing listener, frame
wrap, changing virtual returns and finite randomized vectors/quaternions. This
is a bounded constructed domain. NaN/infinity payloads, FPSCR exception flags,
arbitrary aliasing, asynchronous changes, actual gameplay semantics of imported
helpers, animation, camera, collision and real input are not proven. Instruction
coverage and bit-equal modeled outcomes do not remove those limits.

Preservation enumerates every frozen O root and actual defining compiler object,
including weak/repeated definitions. It invokes unchanged project provenance and
exact-byte checks and additionally compares all original genuine object hashes
with the clean baseline. Generated split stubs are excluded from hash totals
because enrollment changes them. Full linked-code equality is not claimed.

The final all-definition gate has an inherited failure on this frozen main:
_ZN2rp14getPlayerActorEv passes in its Factory object but is rejected for unresolved
non-branch data relocation in Player/PlayerFunction.o. The same canonical command
on a separate clean frozen-main checkout without the candidate also rejects it.
To reproduce that control, clean-build main, then run:
python tools/check.py _ZN2rp14getPlayerActorEv --object build/eu/obj/Game/backup/src/Player/PlayerFunction.o
Restore any scratch checker rank edits afterward. Do not omit the failing
definition or call this an all-definition pass. The candidate does not change
that family, and all of its own baseline compiler objects remain unchanged.

## link.py

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
import re,csv,subprocess,os,json
out=Path('build/root1a3f');obj='build/eu/obj/lib/al/src/Movement/DirectionalMotionUpdate.o';rows=[{k.strip():v.strip() for k,v in r.items()} for r in csv.DictReader(open('data/ver/eu/map.csv'))];byname={r['Symbol']:r for r in rows if r['Symbol']};lines=['#<SYMDEFS>#'];imports=[]
# Existing clean header/README identities; independently verified initializer.
missing={}
with open(obj,'rb') as f:
 e=ELFFile(f);st=e.get_section_by_name('.symtab');root='fn_001A3F08'
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
import struct,json,random,hashlib,time,collections,math
D=Path('build/root1a3f');original=Path('data/ver/eu/code.bin').read_bytes()
assert hashlib.sha256(original).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
with (D/'candidate.axf').open('rb') as f:
 e=ELFFile(f);ENTRY=e.header.e_entry;SEGS=[(s['p_vaddr'],s.data()) for s in e.iter_segments() if s['p_type']=='PT_LOAD']
B=0x800000;SIZE=0x10000;SELF=B;MOTION=B+0x1000;INPUTOBJ=B+0x2000;TRIGGER=B+0x2100;BLEND=B+0x2200;LISTENER=B+0x2300;VECTOR=B+0x3000;CONFIG=B+0x4000;SP=0xa10000;STOP=0x600000;VBASE=0x610000
HELPERS={0x26e1dc,0x27305c,0x279abc,0x27cd04,0x252864,0x1735b0,0x27306c,0x173580,0x267738,0x27cc64,0x27cb48,0x1a3a58,0x1a3938}
SAVED={r:0xabcd0000+i*0x111 for i,r in enumerate([UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11])}
def w(x):return struct.pack('<I',x&0xffffffff)
def f(x):return struct.pack('<f',x)
def ff(x):return struct.unpack('<f',f(x))[0]
def bits(x):return struct.unpack('<I',f(x))[0]
def value(x):return struct.unpack('<f',w(x))[0]
def dot(a,b):return ff(ff(ff(a[0]*b[0])+ff(a[1]*b[1]))+ff(a[2]*b[2]))
def run(c,cand):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176);u.reg_write(UC_ARM_REG_C1_C0_2,0xf00000);u.reg_write(UC_ARM_REG_FPEXC,0x40000000)
 u.mem_map(0x100000,0x340000);u.mem_write(0x100000,original);u.mem_protect(0x100000,0x2a2000,UC_PROT_READ|UC_PROT_EXEC);u.mem_protect(0x3a2000,0x3f000,UC_PROT_READ)
 u.mem_map(0x500000,0x10000,UC_PROT_READ|UC_PROT_EXEC)
 for a,data in SEGS:u.mem_write(a,data)
 u.mem_map(STOP,0x1000,UC_PROT_READ|UC_PROT_EXEC);u.mem_map(VBASE,0x10000,UC_PROT_READ|UC_PROT_EXEC);u.mem_map(B,SIZE);u.mem_map(0xa00000,0x20000);u.mem_write(B,b'\xa5'*SIZE);u.mem_write(0xa00000,b'\xcc'*0x20000)
 def word(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def setword(a,v):u.mem_write(a,w(v))
 def vec(a):return list(struct.unpack('<fff',u.mem_read(a,12)))
 def setvec(a,v):u.mem_write(a,b''.join(f(x) for x in v))
 def vecbits(a):return [word(a+4*i) for i in range(3)]
 vmap={};slotid=0
 for obj,group,slots,table in [(CONFIG,'config',[0x10,0x34,0x38,0x3c,0x40,0x44,0x48,0x4c,0x50,0x54,0x90,0xc4,0x2dc],B+0x5000),(INPUTOBJ,'input',[8,12,0x30],B+0x5400),(TRIGGER,'trigger',[0x24],B+0x5500),(BLEND,'blend',[0],B+0x5600),(LISTENER,'listener',[8],B+0x5700)]:
  setword(obj,table)
  for off in slots:
   target=VBASE+4*slotid;slotid+=1;vmap[target]=(group,off);setword(table+off,target)
 for off,v in [(0x10,MOTION),(0x14,INPUTOBJ),(0x18,TRIGGER),(0x20,LISTENER if c.get('listener',True) else 0),(0x28,BLEND),(0x38,c.get('frames',17)),(0x40,c.get('timer',0))]:setword(SELF+off,v)
 setvec(MOTION+12,c.get('forward',[1,0,0]));setvec(MOTION+0x24,c.get('velocity',[1,0,0]));setvec(VECTOR,c.get('input',[1,0,0]))
 for r,v in SAVED.items():u.reg_write(r,v)
 for reg in range(UC_ARM_REG_D8,UC_ARM_REG_D15+1):u.reg_write(reg,0x123456789abcdef0)
 u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,STOP);u.reg_write(UC_ARM_REG_R0,SELF)
 pcs=set();events=[];fault=None;queries=collections.Counter()
 defaults={0x10:0.125,0x34:4,0x38:8,0x3c:5,0x40:6,0x44:7,0x48:4,0x4c:8,0x50:10,0x54:20,0x90:12,0xc4:0.5,0x2dc:10}
 def normal(a):return 'stack' if 0xa00000<=a<0xa20000 else a
 def code(u,a,s,d):
  pcs.add(a)
  if a not in HELPERS and a not in vmap:return
  r=[u.reg_read(reg) for reg in [UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3]];fv=[u.reg_read(z) for z in [UC_ARM_REG_S0,UC_ARM_REG_S1,UC_ARM_REG_S2]];ret=0;fret=None;args=[]
  if a in vmap:
   group,off=vmap[a];queries[(group,off)]+=1;q=queries[(group,off)];args=[group,off,r[0]]
   if group=='config':
    v=c.get('config',{}).get(str(off),defaults[off]);v=v[min(q-1,len(v)-1)] if isinstance(v,list) else v
    if off in [0x3c,0x40,0x44,0x48,0x4c]:ret=int(v)
    else:fret=float(v)
   elif group=='input':ret=VECTOR if off==12 else c.get('active',1) if off==8 else c.get('mode',0)
   elif group=='trigger':
    ret=c.get('reset',0)
   elif group=='blend':
    v=c.get('blend',0);fret=v[min(q-1,len(v)-1)] if isinstance(v,list) else v
   elif group=='listener':args.append(r[1])
  elif a==0x26e1dc:args=r[:1];ret=CONFIG
  elif a==0x27305c:
   args=r[:1];setvec(r[0]+12,c.get('reset_forward',[0,0,1]));setvec(r[0]+0x24,c.get('reset_velocity',[0,1,2]))
  elif a==0x279abc:
   args=[normal(r[0]),vecbits(r[0])];v=vec(r[0]);size=ff(math.sqrt(max(0,dot(v,v))))
   if size:setvec(r[0],[ff(x/size) for x in v])
  elif a==0x27cd04:
   av=vec(r[1]);bv=vec(r[2]);args=[normal(r[0]),vecbits(r[1]),vecbits(r[2])];setvec(r[0],[ff(ff(av[1]*bv[2])-ff(av[2]*bv[1])),ff(ff(av[2]*bv[0])-ff(av[0]*bv[2])),ff(ff(av[0]*bv[1])-ff(av[1]*bv[0]))])
  elif a==0x252864:args=[r[0],fv[0]];fret=ff(value(fv[0])*c.get('friction_factor',0.8))
  elif a==0x1735b0:args=fv[:2];fret=value(fv[0]) if abs(value(fv[0]))>value(fv[1]) else 0
  elif a==0x27306c:
   n=vec(r[1]);v=vec(r[2]);args=[normal(r[0]),vecbits(r[1]),vecbits(r[2])];projection=dot(n,v);setvec(r[0],[ff(v[i]-ff(n[i]*projection)) for i in range(3)])
  elif a==0x173580:
   args=fv[:3];fret=c.get('approach_override',min(ff(value(fv[0])+value(fv[2])),value(fv[1])))
  elif a==0x267738:
   args=[normal(r[0]),vecbits(r[1]),vecbits(r[2]),fv[0]];u.mem_write(r[0],b''.join(f(v) for v in c.get('quaternion',[0,0,0,1])))
  elif a==0x27cc64:
   args=[normal(r[0]),vecbits(r[1]),fv[0]];setvec(r[0],[ff(v*value(fv[0])) for v in vec(r[1])])
  elif a==0x27cb48:
   args=[normal(r[0]),vecbits(r[1]),vecbits(r[2])];av=vec(r[1]);bv=vec(r[2]);setvec(r[0],[ff(x+y) for x,y in zip(av,bv)])
  elif a in [0x1a3a58,0x1a3938]:args=[r[0],r[1],fv[0]]
  events.append([vmap[a] if a in vmap else hex(a),args])
  for reg,v in [(UC_ARM_REG_R1,0xdead0001),(UC_ARM_REG_R2,0xdead0002),(UC_ARM_REG_R3,0xdead0003),(UC_ARM_REG_R12,0xdead000c)]:u.reg_write(reg,v)
  for reg in [UC_ARM_REG_D0,UC_ARM_REG_D1,UC_ARM_REG_D2,UC_ARM_REG_D3,UC_ARM_REG_D4,UC_ARM_REG_D5,UC_ARM_REG_D6,UC_ARM_REG_D7]:u.reg_write(reg,0xabcdef0123456789)
  u.reg_write(UC_ARM_REG_R0,ret)
  if fret is not None:u.reg_write(UC_ARM_REG_S0,bits(fret))
  u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 def invalid(u,access,a,s,v,d):
  nonlocal fault
  fault=[access,a,s];return False
 u.hook_add(UC_HOOK_CODE,code);u.hook_add(UC_HOOK_MEM_INVALID,invalid)
 try:u.emu_start(ENTRY if cand else 0x1a3f08,STOP,count=20000)
 except UcError as ex:
  if fault is None:fault=['emulator',str(ex)]
 status='fault' if fault else 'return' if u.reg_read(UC_ARM_REG_PC)==STOP else 'bounded'
 if status=='return':
  assert u.reg_read(UC_ARM_REG_SP)==SP;assert all(u.reg_read(r)==v for r,v in SAVED.items()),'callee-save mismatch';assert all(u.reg_read(r)==0x123456789abcdef0 for r in range(UC_ARM_REG_D8,UC_ARM_REG_D15+1)),'VFP callee-save mismatch'
 return {'status':status,'fault':fault,'memory':bytes(u.mem_read(B,SIZE)).hex(),'stack':bytes(u.mem_read(SP,128)).hex(),'events':events},pcs

def fixtures():
 cases=[]
 def add(name,**kw):cases.append(dict(name=name,**kw))
 for active in [0,1]:
  for mode in [0,1]:
   for speed in [-2,-0.1,-0.05,0,1,3.7,4,6,10]:
    for timer in [0,9,10]:add('basic-%s-%s-%s-%s'%(active,mode,speed,timer),active=active,mode=mode,velocity=[speed,-2,1],timer=timer)
 for vector in [[-1,0,0],[1,0,1],[0,0,0],[0,1,0],[0.01,0,0],[3,1,2]]:
  for speed in [0,3,9]:add('direction-%s-%s'%(vector,speed),input=vector,velocity=[speed,-30,1],quaternion=[0.1,0.2,0.3,0.9])
 for blend in [-1,0,0.25,1,2,[0.5,0.8]]:add('blend-'+str(blend),blend=blend,velocity=[1,2,3])
 add('reset',reset=1);add('no-listener',listener=False);add('wrapped-frame',frames=0xffffffff,timer=0xffffffff,velocity=[8,0,0]);add('negative-approach',approach_override=-2);add('over-approach',approach_override=20);add('under-desired-friction',velocity=[10,0,0],friction_factor=0.1);add('changing-config',config={str(0x34):[4,5,6,7,8],str(0x38):[8,9,10],str(0xc4):[0.2,0.7],str(0x90):[1,2]})
 rng=random.Random(0x1a3f08)
 for i in range(100):add('random-'+str(i),active=rng.randrange(2),mode=rng.randrange(2),velocity=[rng.uniform(-2,15),rng.uniform(-20,20),rng.uniform(-5,5)],input=[rng.uniform(-2,2),rng.uniform(-1,1),rng.uniform(-2,2)],blend=rng.choice([0,0.2,0.8]),forward=[rng.uniform(-1,1),0,rng.uniform(-1,1)],quaternion=[rng.uniform(-1,1) for _ in range(4)],timer=rng.randrange(15))
 return cases
if __name__=='__main__':
 start=time.time();pcs=set();results=[]
 for c in fixtures():
  a,pa=run(c,False);b,pb=run(c,True);pcs|=pa
  if a!=b:
   (D/'first-failure.json').write_text(json.dumps({'case':c,'original':a,'candidate':b},indent=2));print('FAIL',c['name'],[k for k in a if a[k]!=b[k]]);raise SystemExit(1)
  results.append({'name':c['name'],'status':a['status'],'events':len(a['events'])})
 root=set(range(0x1a3f08,0x1a42b0,4))|set(range(0x1a42c8,0x1a4688,4));out={'cases':len(results),'statuses':dict(collections.Counter(r['status'] for r in results)),'root_visited':len(root&pcs),'root_total':len(root),'unvisited':[hex(a) for a in sorted(root-pcs)],'seconds':time.time()-start,'results':results};(D/'replay.json').write_text(json.dumps(out,indent=2));print({k:v for k,v in out.items() if k!='results'})
```

## preserve.py

```python
from pathlib import Path
import sys,csv,json,time,hashlib,subprocess
sys.path.insert(0,str(Path.cwd()))
from elftools.elf.elffile import ELFFile
from tools.low.checkExactBytes import check_exact_bytes
from tools.low.buildProvenance import verify_build_output
D=Path('build/root1a3f');rows=[{k.strip():v.strip() for k,v in x.items()} for x in csv.DictReader(open(D/'map.baseline.csv'))];syms={x['Symbol']:x for x in rows if x['Symbol'] and x['Type'].startswith('f') and x['Rank']=='O'};defs={}
for p in Path('build/eu/obj').rglob('*.o'):
 if p.name=='DirectionalMotionUpdate.o' or '/split/' in str(p):continue
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
