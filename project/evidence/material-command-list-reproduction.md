# Material command-list reproduction

This note contains source-only diagnostic recipes. The owner's original code.bin
and exh.bin remain local and are never part of the proposal. Start from frozen
main ab98515b4cf1520b53e7af373facf6d392737be6 and apply this source/notes family.
The recipes require the normal project Python environment plus pyelftools,
capstone and Unicorn. Do not change compiler flags or checker behavior.

## Canonical build and scratch enrollment

Source development_environment.sh before project commands. Save the original map
in build/root1141/map.baseline.csv. For this isolated build only, set row
001141CC's rank to M and its empty symbol to fn_001141CC, the project's ordinary
address-derived fallback name. Leave its original start, pool, end, type and
section unchanged. No such map change belongs in a submission. The unchanged
checker requires the source to be committed before a claim can be checked.

Run python make.py eu -ca, then python tools/check.py fn_001141CC --object
build/eu/obj/lib/al/src/Graphics/MaterialCommandLists.o. The expected result for
this source is rejection because the complete section has 2088 bytes, compared
with the original 2020-byte interval. This is not exact-byte credit.

Before adding the TU, make a separate clean build of the frozen base and save
its map and compiler-object inventory. The inventory recipe is:

```python
from pathlib import Path
import hashlib, json
Path('build/root1141').mkdir(parents=True, exist_ok=True)
Path('build/root1141/map.baseline.csv').write_bytes(Path('data/ver/eu/map.csv').read_bytes())
objects = {str(p): hashlib.sha256(p.read_bytes()).hexdigest()
           for p in Path('build/eu/obj').rglob('*.o')}
Path('build/root1141/baseline-objects.json').write_text(json.dumps(objects, indent=2))
```

Retain that inventory while applying the new TU. Use the following script after
the final clean build for the unchanged canonical exact-byte routines.
It checks every accepted root and every actual weak definition of those roots,
with provenance verification. Generated split stubs are excluded from canonical
source definitions. They remain in the baseline object hash inventory so their
expected enrollment-related change is visible.

## Canonical preservation

Save this as build/root1141/preserve.py and run it from the repository root.

```python
from pathlib import Path
import sys,csv,json,time,hashlib,subprocess
sys.path.insert(0,str(Path.cwd()))
from elftools.elf.elffile import ELFFile
from tools.low.checkExactBytes import check_exact_bytes
from tools.low.buildProvenance import verify_build_output
D=Path('build/root1141');rows=[{k.strip():v.strip() for k,v in x.items()} for x in csv.DictReader(open(D/'map.baseline.csv'))];syms={x['Symbol']:x for x in rows if x['Symbol'] and x['Type'].startswith('f') and x['Rank']=='O'};defs={}
for p in Path('build/eu/obj').rglob('*.o'):
 if p.name=='MaterialCommandLists.o' or '/split/' in str(p):continue
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

## Diagnostic link

Save this as build/root1141/link.py and run it from the repository root.

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
import re,csv,subprocess,os,json
out=Path('build/root1141');obj='build/eu/obj/lib/al/src/Graphics/MaterialCommandLists.o';rows=[{k.strip():v.strip() for k,v in r.items()} for r in csv.DictReader(open('data/ver/eu/map.csv'))];byname={r['Symbol']:r for r in rows if r['Symbol']};lines=['#<SYMDEFS>#'];imports=[]
# Existing clean header/README identities; independently verified initializer.
missing={}
with open(obj,'rb') as f:
 e=ELFFile(f);st=e.get_section_by_name('.symtab');root='fn_001141CC'
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

## Paired constructed replay

Save this as build/root1141/replay.py and run it from the repository root.

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import *
from unicorn.arm_const import *
import struct,json,random,hashlib,time,collections
D=Path('build/root1141');original=Path('data/ver/eu/code.bin').read_bytes()
assert hashlib.sha256(original).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
with (D/'candidate.axf').open('rb') as f:
 e=ELFFile(f);ENTRY=e.header.e_entry;SEGS=[(s['p_vaddr'],s.data()) for s in e.iter_segments() if s['p_type']=='PT_LOAD']
B=0x800000;SIZE=0x10000;OWNER=B;MAT=B+0x1000;ALT=B+0x2000;SP=0xa10000;STOP=0x600000
HELPERS={0x28ed00,0x281a54,0x28f864,0x28f6f0,0x28f618,0x2847e4,0x2819d0,0x28ca18,0x2819a8,0x281978,0x288284,0x281874,0x28140c,0x2877fc,0x281360,0x284894}
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
 setword(OWNER+0x6c,0 if c.get('null') else MAT);setword(OWNER+0x60,c.get('selected',0))
 for i in range(4):setword(OWNER+0x70+4*i,(B+0x800+i*0x40) if c.get('selected_present',True) else 0)
 for m in [MAT,ALT]:
  for off in range(0,0x800,4):setword(m+off,0)
  for off,v in [(0x68c,c.get('sign',0)),(0x5dc,c.get('state',0)),(0x564,c.get('color',0)),(0x578,c.get('mode',0)),(0x448,c.get('blend',0)),(0x648,c.get('blend_color',0)),(0x4b0,c.get('secondary',0)),(0x5ec,c.get('secondary_state',0)),(0x770,c.get('parameter',0x12345678)),(0x69c,c.get('resource',0)),(0x6b8,c.get('resource2',0))]:setword(m+off,v)
  # Alternate storage remains valid even when a modeled helper switches the owner pointer.
  if m==ALT:
   for off in [0x6c8,0x6cc,0x75c]:setword(m+off,1)
 setword(0x4244a8+0x50,c.get('identifier',0x1234));setword(0x3ef06c,c.get('fallback',0xdeadbeef))
 for r,v in SAVED.items():u.reg_write(r,v)
 u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,STOP);u.reg_write(UC_ARM_REG_R0,OWNER)
 pcs=set();events=[];fault=None;steps=0;state={'active':c.get('active',0xabc),'next':50,'position':c.get('position',0),'base':c.get('base',0x900000),'query':0,'execute':0,'apply':0}
 def normal(a):return 'stack' if 0xa00000<=a<0xa20000 else a
 def code(u,a,s,d):
  nonlocal steps
  steps+=1;pcs.add(a)
  if a not in HELPERS:return
  r=[u.reg_read(reg) for reg in [UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3]];args=[];ret=0
  if a==0x28ed00:
   key=r[0];value={0x201:c.get('saved',0),0x207:state['active'],0x206:state['base'],0x202:state['position']}[key];setword(r[1],value);args=[key,normal(r[1]),value];state['query']+=1
   if c.get('switch_on_query')==state['query']:setword(OWNER+0x6c,ALT)
  elif a==0x28f864:
   state['next']+=1;setword(r[1],state['next']);args=[r[0],normal(r[1])]
  elif a==0x28f6f0:state['active']=r[0];args=r[:1]
  elif a==0x28f618:args=r[:2];state['base']=(state['base']+0x1234)&0xffffffff;state['position']=c.get('position',0)
  elif a==0x288284:
   args=[bytes(u.mem_read(r[0],r[1])).hex(),r[1],r[2]];state['position']=(state['position']+r[1])&0xffffffff
  elif a==0x281874:
   args=r[:3];state['execute']+=1;ret=0x600+state['execute'];state['position']=(state['position']+12)&0xffffffff
   if c.get('switch_on_execute')==state['execute']:setword(OWNER+0x6c,ALT)
  elif a==0x28140c:
   args=r[:2];state['apply']+=1;state['position']=(state['position']+8)&0xffffffff
   if c.get('switch_on_apply')==state['apply']:setword(OWNER+0x6c,ALT)
   if c.get('mutate_mode'):setword(MAT+0x578,word(MAT+0x578)^1)
  elif a==0x2877fc:ret=c.get('full_color',0)
  elif a==0x281360:
   args=[normal(x) for x in r];setword(r[0],0x7788);setword(r[1],0x3355);setword(r[2],state['position']);setword(r[3],0x789a)
  elif a in {0x281a54,0x2847e4,0x2819a8,0x281978,0x284894}:args=r[:1]
  elif a==0x28ca18:ret=c.get('error',0)
  events.append([hex(a),args])

  for reg,value in [(UC_ARM_REG_R1,0xdead0001),(UC_ARM_REG_R2,0xdead0002),(UC_ARM_REG_R3,0xdead0003),(UC_ARM_REG_R12,0xdead000c)]:u.reg_write(reg,value)
  u.reg_write(UC_ARM_REG_R0,ret);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 def invalid(u,access,a,s,v,d):
  nonlocal fault
  fault=[access,a,s];return False
 u.hook_add(UC_HOOK_CODE,code);u.hook_add(UC_HOOK_MEM_INVALID,invalid)
 try:u.emu_start(ENTRY if cand else 0x1141cc,STOP,count=10000)
 except UcError as ex:
  if fault is None:fault=['emulator',str(ex)]
 status='fault' if fault else 'return' if u.reg_read(UC_ARM_REG_PC)==STOP else 'bounded'
 if status=='return':
  assert u.reg_read(UC_ARM_REG_SP)==SP;assert all(u.reg_read(r)==v for r,v in SAVED.items()),'callee-save mismatch'
 return {'status':status,'fault':fault,'memory':bytes(u.mem_read(B,SIZE)).hex(),'globals':bytes(u.mem_read(0x3ef06c,0xa0)).hex(),'stack':bytes(u.mem_read(SP,128)).hex(),'events':events,'state':state},pcs

def fixtures():
 cases=[]
 def add(name,**kw):cases.append(dict(name=name,**kw))
 for flags in range(32):add('branches-'+str(flags),saved=9 if flags&1 else 0,mode=flags&2,blend=flags&4,secondary=flags&8,selected_present=bool(flags&16))
 for color in [-3,-1,0,1,2,3,4,5,6,100,0x7fffffff,0x80000000]:
  for full in [0,1]:add('color-%d-%d'%(color,full),color=color,full_color=full)
 for blend in [-1,0,1,10,11,12,0x7fffffff,0x80000000]:add('blend-'+str(blend),blend=1,blend_color=blend)
 for sign in [0,1,0x7fffffff,0x80000000,0xffffffff]:
  for state in [0,1,6,7]:add('state-%x-%d'%(sign,state),sign=sign,state=state)
 for resource in [-1,0,1,0x7fffffff,0x80000000]:add('resource-'+str(resource),resource=resource,resource2=resource)
 for p in [0,1,2,3,4,0xfffffffd,0xffffffff]:add('alignment-'+str(p),position=p,base=0xffff0000)
 for identifier in [0,1,0xffff,0x10000,0xffffffff]:add('identifier-'+str(identifier),identifier=identifier)
 for key,n in [('switch_on_query',14),('switch_on_execute',5),('switch_on_apply',8)]:
  for k in range(1,n+1):add(key+'-'+str(k),**{key:k},mode=1,blend=1,secondary=1)
 add('mutating-mode',mutate_mode=True,mode=1,blend=1,secondary=1)
 add('null-material',null=True)
 rng=random.Random(0x1141cc)
 for i in range(100):add('random-'+str(i),selected=rng.randrange(4),selected_present=bool(rng.randrange(2)),saved=rng.randrange(2)*999,sign=rng.getrandbits(32),mode=rng.randrange(2),blend=rng.randrange(2),secondary=rng.randrange(2),state=rng.randrange(8),secondary_state=rng.randrange(6),color=rng.randrange(9),blend_color=rng.randrange(15),full_color=rng.randrange(2),identifier=rng.getrandbits(32),parameter=rng.getrandbits(32),position=rng.getrandbits(32),base=rng.getrandbits(32),resource=rng.randrange(-1,3),resource2=rng.randrange(-1,3))
 return cases
if __name__=='__main__':
 start=time.time();pcs=set();results=[]
 for c in fixtures():
  a,pa=run(c,False);b,pb=run(c,True);pcs|=pa
  if a!=b:
   (D/'first-failure.json').write_text(json.dumps({'case':c,'original':a,'candidate':b},indent=2));print('FAIL',c['name'],[k for k in a if a[k]!=b[k]]);raise SystemExit(1)
  results.append({'name':c['name'],'status':a['status'],'events':len(a['events'])})
 root=set(range(0x1141cc,0x11494c,4));out={'cases':len(results),'statuses':dict(collections.Counter(r['status'] for r in results)),'root_visited':len(root&pcs),'root_total':len(root),'unvisited':[hex(a) for a in sorted(root-pcs)],'seconds':time.time()-start,'results':results};(D/'replay.json').write_text(json.dumps(out,indent=2));print({k:v for k,v in out.items() if k!='results'})

```

## Interpretation and cleanup

The diagnostic link relocates only the genuine project ARMCC output and binds its
sixteen imports to original addresses. It never assembles original instructions
or substitutes generated bytes for source. All original instructions in the
root execute unchanged. The API fixtures intercept imports on both sides and
clobber caller-save registers; original imported function bodies do not execute.

The suite includes 100 fixed-seed cases, every branch combination, handle release
sign boundaries, query offset wrapping/alignment, both color conversions and
upper-bound clamps, context identifier patterns, selected-list presence, owner
material-pointer changes at imported calls, mode mutations and a null-material
fault. It compares emitted commands, call arguments/order, all fixture memory,
selected globals, caller-stack sentinels, final modeled API state and callee-save
registers. The null fault is a malformed-input observation, not an assertion that
null input is supported. Full instruction coverage does not prove full-domain
correctness. The fixture does not reproduce allocation or GPU behavior.

Restore data/ver/eu/map.csv byte-for-byte from map.baseline.csv after checks.
Only the new TU and notes belong in the patch. No executable, template bytes,
compiler object, map, ranks, tools, STATE or ledger belongs in the submission.
