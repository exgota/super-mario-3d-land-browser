# Root 0x0022F804: reproducible diagnostic replay

This appendix contains runnable research code, not game data. The source remains NonMatching with zero exact credit. Use source revision `d3e2fb7` or a report-only descendant with the same source/header, base main `3de056e`, the configured ARMCC 4.1/791 compiler, and the owner's locally held EU executable/exheader. The Python environment used Unicorn 2.1.4, Capstone 5.0.7 and pyelftools 0.33.

The strict checker is run unchanged. It rejects the absent `dat_0042A4B0` identity before comparing function bytes. The diagnostic link then supplies only that data address, using an in-memory binding with `End=None`; it is not a canonical map proposal and asserts no object extent. Existing map rows resolve all other imports. The linked bytes come only from the project's canonical output of committed C++, with ordinary helpers already inlined. No original instructions, assembler implementation, patched object or substitute ARM is generated.

In a fresh worktree, arrange the normal ignored `.venv`, `data/compilers`, `data/ver/eu/code.bin` and `exh.bin` as in the project's instructions. Do not copy the private files into the repository or commit them. Save the following three Python blocks in the indicated ignored paths, then run from the repository root:

```sh
. ./development_environment.sh
export DEVKITARM=/usr
mkdir -p build/research
python make.py eu -ca
python build/research/prepare.py
python build/research/identity.py
python build/research/replay.py
```

The final expected replay has 356 fixture pairs, 6,185 ordered observations and 845 visited original root instruction addresses. It compares all 131,072 fixture bytes and 152 global bytes per pair, return values, SP and preserved core/VFP registers. The candidate section must be 3,020 bytes with SHA-256 `8aea5d89d3b0ae22d015aebcb3092022feec07630fa2048dbb7b64c1ccc8ad4b`. Any fault, exhausted instruction budget, byte/trace mismatch or ABI failure exits nonzero. The summary and identity outputs are generated under `build/research/` and are not committed.

The complete original executable is loaded into separate ARM1176 instances. Only the candidate root's canonical linked section is overlaid in one instance. The original guard routine, constructor and destructor execute normally when reached. Synthetic allocator-vtable addresses only identify emulator callbacks; they are neither source imports nor purported retail functions. The callback models are explicitly defined below. Reentrant mutation fixtures are deliberate constructed cases, separate from claims about retail heap behavior.

## Preparation (`build/research/prepare.py`)

```python
import hashlib,json,subprocess,sys
from pathlib import Path
sys.path.insert(0,str(Path.cwd()))
from tools.low.checkExactBytes import _read_map,_isolate_function,UnresolvedOriginalSymbol
from tools.low.buildProvenance import verify_build_output
from elftools.elf.elffile import ELFFile
out=Path('build/research'); obj=Path('build/eu/obj/lib/al/src/SelectorStorage22F804.o');p=Path('data/ver/eu/map.csv');old=p.read_bytes()
row=next(x for x in old.splitlines() if x.startswith(b'0x0022F804,'))
try:
 p.write_bytes(old.replace(row,row.replace(b',U,f,,',b',U,f,fn_0022F804,')))
 r=subprocess.run([sys.executable,'tools/check.py','fn_0022F804','--object',str(obj)],capture_output=True,text=True)
 (out/'strict-check.txt').write_text(r.stdout+r.stderr);print('STRICT',r.returncode,r.stdout,r.stderr)
 assert r.returncode==1 and 'Source closure rejected' in r.stdout
finally:p.write_bytes(old)
assert p.read_bytes()==old
expected={'build/eu/obj/lib/al/src/SelectorStorage22F804.o': 'bf4a8c16429cbf52215383a22e56cc9debd1578896b2bc14fe3178ddbcd93930', 'lib/al/src/SelectorStorage22F804.cpp': 'd60d655443a4bd2a58d8e252883d82861c89034f954ce752d160067789fb49c1', 'lib/al/include/Resource/SelectorStorage22F804.h': '204c387f673f124adf0f92beda8a5e992ba10e15544292ed62f778e395282bd7', 'data/ver/eu/map.csv': '514c3ec6f2343e8a40fc73996df78fc95b2e33f4897fbdda6bb0311df6cf40fa', 'data/ver/eu/code.bin': 'e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64', 'data/compilers/4.1/791/bin/armcc.exe': 'd1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d'}
for name,digest in expected.items():assert hashlib.sha256(Path(name).read_bytes()).hexdigest()==digest,name
provenance=verify_build_output(obj)
rows=_read_map(p)
try:
 _isolate_function(obj,'fn_0022F804',rows,out/'unbound.o')
 raise AssertionError('Expected unknown BSS identity')
except UnresolvedOriginalSymbol as error:
 assert str(error)=='No established original address for dat_0042A4B0.',str(error)
# DIAGNOSTIC ONLY: identity missing from canonical map; independently witnessed.
# End=None explicitly declines to assert a data-object extent.
# No on-disk map, checker, rank, original boundary or object is edited.
rows.append({'Start':0x42a4b0,'End':None,'Symbol':'dat_0042A4B0','Type':'d','SectionName':''})
section,compiled,imports=_isolate_function(obj,'fn_0022F804',rows,out/'candidate.o')
(out/'imports.json').write_text(json.dumps(imports,indent=2))
(out/'symbols.sym').write_text('#<SYMDEFS>#\n'+''.join('0x%08X %s %s\n'%(i['address'],i['kind'],i['symbol']) for i in imports))
(out/'candidate.sct').write_text('REPLAY 0x0022F804 { CODE 0x0022F804 { candidate.o (i.fn_0022F804, +FIRST) } }\n')
command=['data/compilers/wibo','data/compilers/4.1/791/bin/armlink.exe','--cpu=MPCore','--fpu=VFPv2','--arm_only','--no_exceptions','--inline','--datacompressor=off','--no_debug','--no_scanlib','--entry=fn_0022F804','--keep=fn_0022F804','--scatter='+str(out/'candidate.sct'),'--output='+str(out/'replay.axf'),str(out/'candidate.o'),str(out/'symbols.sym')]
r=subprocess.run(command,capture_output=True,text=True);(out/'link.log').write_text(r.stdout+r.stderr);assert r.returncode==0,r.stdout+r.stderr
with (out/'replay.axf').open('rb') as f:s=ELFFile(f).get_section_by_name('CODE');data=s.data();assert s['sh_addr']==0x22f804
hashes={str(x):hashlib.sha256(x.read_bytes()).hexdigest() for x in [obj,Path('lib/al/src/SelectorStorage22F804.cpp'),Path('lib/al/include/Resource/SelectorStorage22F804.h'),p,Path('data/ver/eu/code.bin'),Path('data/compilers/4.1/791/bin/armcc.exe')]}
summary={'size':len(data),'object_section_size':len(compiled),'linked_sha256':hashlib.sha256(data).hexdigest(),'hashes':hashes,'imports':len(imports),'canonical':'source closure rejected; unknown original data identity','diagnostic_extra_identity':'dat_0042A4B0 address binding only; extent unknown, observed prefix 16 bytes'}
assert len(data)==3020 and hashlib.sha256(data).hexdigest()=='8aea5d89d3b0ae22d015aebcb3092022feec07630fa2048dbb7b64c1ccc8ad4b'
(out/'prepare.json').write_text(json.dumps(summary,indent=2));print(json.dumps(summary,indent=2))
```

## Independent identity probes (`build/research/identity.py`)

```python
import csv,json,struct,hashlib
from pathlib import Path
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
binary=Path('data/ver/eu/code.bin').read_bytes()
assert hashlib.sha256(binary).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
rows=[{k.strip():v.strip() for k,v in r.items()} for r in csv.DictReader(Path('data/ver/eu/map.csv').open())]
def owner(address):
 return next((r for r in rows if int(r['Start'],16)<=address<int(r['End'],16)),None)
cs=Cs(CS_ARCH_ARM,CS_MODE_ARM);cs.skipdata=True
callers=[i.address for i in cs.disasm(binary[:0x2d0000],0x100000) if i.mnemonic in ['b','bl'] and i.op_str=='#0x22f804']
assert callers==[0x231cf8,0x29fd60,0x2a864c,0x2af104,0x2b0dcc]
refs={}
for value in [0x42a4b0,0x3f0698,0x3d83cc]:
 refs[hex(value)]=[hex(i+0x100000) for i in range(0,len(binary),4) if struct.unpack_from('<I',binary,i)[0]==value]
assert refs['0x42a4b0']==['0x230624']
assert refs['0x3d83cc']==['0x230590','0x2a5d7c']
assert struct.unpack_from('<I',binary,0x2306c0-0x100000)[0]==0x3d86f8
assert struct.unpack_from('<I',binary,0x3d86f8-0x100000)[0]==0x2b3e7c
assert struct.unpack_from('<I',binary,0x3d83cc-0x100000)[0]==0x2a5d70
summary={'direct_callers':[{'address':hex(x),'owner':owner(x)} for x in callers], 'aligned_literal_references':refs,'unknown_bss_extent':True,'source_claim':'Only observed 16-byte prefix at 0x0042A4B0; no independent whole-object bound.'}
Path('build/research/identity-final.json').write_text(json.dumps(summary,indent=2));print(json.dumps(summary,indent=2))
```

## Replay (`build/research/replay.py`)

```python
import argparse,hashlib,json,struct
from pathlib import Path
from unicorn import Uc,UcError,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE
from unicorn.arm_const import *
from elftools.elf.elffile import ELFFile
BASE=0x100000; ROOT=0x22f804; MEM=0x500000; STORAGE=MEM; RESOURCE=MEM+0x500
ALLOC=MEM+0x200; ALLOC2=MEM+0x220; VT=MEM+0x300; STOP=0x600000; NEW=STOP+4; FREE=STOP+8; STACK=0x710000
ARRAYS=[0xc,0x20,0x30,0x40,0x50]
DESCRIPTORS=[0x3f06c8,0x3f06d0,0x3f06d8,0x3f06f0,0x42a4b0,0x3f06e0,0x3f06e8,0x3f06f8,0x3f0700,0x3f0710,0x3f0708,0x3f0718]
REGS=[UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3]
SAVED=list(range(UC_ARM_REG_R4,UC_ARM_REG_R11+1));VFP=list(range(UC_ARM_REG_D8,UC_ARM_REG_D15+1))
def pack(*v):return struct.pack('<'+'I'*len(v),*[x&0xffffffff for x in v])
def run(binary,candidate,case):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176);u.reg_write(UC_ARM_REG_C1_C0_2,0xf00000);u.reg_write(UC_ARM_REG_FPEXC,0x40000000)
 u.mem_map(BASE,0x400000);u.mem_write(BASE,binary)
 if candidate is not None:u.mem_write(ROOT,candidate)
 u.mem_map(MEM,0x20000);u.mem_map(STOP,0x1000);u.mem_map(0x700000,0x20000)
 u.mem_write(MEM,bytes((i*73+19)&255 for i in range(0x20000)))
 def w(a,*v):u.mem_write(a,pack(*v))
 def word(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 w(ALLOC,VT);w(ALLOC2,VT);w(VT,0,0,NEW,FREE)
 selectors=case['selectors'];selarray=MEM+0x400 if case.get('negative_relative') else MEM+0x600
 w(RESOURCE+0x10,case['values'],0,len(selectors),selarray-(RESOURCE+0x1c))
 w(selarray,*selectors)
 w(STORAGE,0x3d86f8,ALLOC,RESOURCE)
 old_pointers={}
 for i,off in enumerate(ARRAYS):
  ptr=MEM+0x2000+i*0x200 if case['old'] else 0
  allocator=0 if case['old']==2 else ALLOC2 if case['old']==3 else ALLOC
  w(STORAGE+off,allocator,ptr,ptr+8 if ptr else 0,0xc0000007)
  old_pointers[ptr]=i
 for i,a in enumerate(range(0x3f0698,0x3f06c8,4)):
  guard=case['guard'] if case['guard']>=0 else [0,1,2,3,0x80000000][i%5]
  w(a,guard)
 u.mem_write(0x3f06c8,bytes([0xa5])*0x58);u.mem_write(0x42a4b0,bytes([0x96])*16)
 trace=[];pcs=set();allocations=[0];mutated=set();last=[ROOT]
 def hook(uc,pc,size,_):
  last[0]=pc
  if ROOT<=pc<0x230568:pcs.add(pc)
  if pc==STOP:uc.emu_stop();return
  if pc==0x28a998:trace.append(['guard',uc.reg_read(UC_ARM_REG_R0),word(uc.reg_read(UC_ARM_REG_R0))]);return
  if pc==NEW:
   allocator,n,alignment=[uc.reg_read(r) for r in REGS[:3]];allocations[0]+=1
   result=0 if allocations[0]==case['fail'] else MEM+0x8000+allocations[0]*0x400
   trace.append(['allocate',allocator,n,alignment,result])
   if result:u.mem_write(result,bytes([0x3a+allocations[0]])*0x400)
   uc.reg_write(UC_ARM_REG_R0,result);uc.reg_write(UC_ARM_REG_PC,uc.reg_read(UC_ARM_REG_LR));return
  if pc==FREE:
   allocator,ptr=[uc.reg_read(r) for r in REGS[:2]];trace.append(['deallocate',allocator,ptr])
   uc.mem_write(ptr,bytes([0xdc])*0x40)
   index=old_pointers.get(ptr,-1);mode=case.get('mutation')
   if mode and index==case.get('mutate_index',1) and index not in mutated:
    mutated.add(index);a=STORAGE+ARRAYS[index];begin=word(a+4)
    if mode=='grow':w(a+8,begin+4,0x40000001)
    elif mode=='clamp':w(a+8,begin,1)
    elif mode=='shrink':w(a+8,begin+16,7)
    elif mode=='no_allocator':w(a,0);w(a+8,begin,0x40000001)
   uc.reg_write(UC_ARM_REG_PC,uc.reg_read(UC_ARM_REG_LR));return
 u.hook_add(UC_HOOK_CODE,hook)
 def enter(address,args):
  for r,v in zip(REGS,args):u.reg_write(r,v)
  for j,r in enumerate(SAVED):u.reg_write(r,0xa1100000+j*0x1111)
  for j,r in enumerate(VFP):u.reg_write(r,0x1122334455660000+j)
  u.reg_write(UC_ARM_REG_SP,STACK);u.reg_write(UC_ARM_REG_LR,STOP)
  try:u.emu_start(address,STOP+0x10,count=200000)
  except UcError as e:raise AssertionError(f'fault at {last[0]:08x}: {e}; case={case}')
  assert last[0]==STOP,('budget',hex(last[0]),case)
  assert u.reg_read(UC_ARM_REG_SP)==STACK,('SP',case)
  assert [u.reg_read(r) for r in SAVED]==[0xa1100000+j*0x1111 for j in range(8)],('callee registers',case)
  assert [u.reg_read(r) for r in VFP]==[0x1122334455660000+j for j in range(8)],('VFP',case)
  return u.reg_read(UC_ARM_REG_R0)
 if case.get('constructor'):enter(0x23062c,[STORAGE,RESOURCE,MEM+0x1700,ALLOC])
 returns=[]
 for _ in range(case.get('repeats',1)):returns.append(enter(ROOT,[STORAGE,case['optional']]))
 if not case['fail']:
  begin=word(STORAGE+0x10);expected=[DESCRIPTORS[x] if 0<=x<12 else 0 for x in selectors]
  assert list(struct.unpack('<'+'I'*len(selectors),u.mem_read(begin,4*len(selectors))))==expected,('independent selector oracle',case)
 if case.get('destructor'):enter(0x2b3e7c,[STORAGE])
 return {'returns':returns,'memory':bytes(u.mem_read(MEM,0x20000)),'globals':bytes(u.mem_read(0x3f0698,0x88))+bytes(u.mem_read(0x42a4b0,16)),'trace':trace},pcs

def cases():
 base={'selectors':[0],'values':3,'optional':1,'old':1,'guard':0,'fail':0}
 out=[]
 for selector in list(range(12))+[12,127,-1,-2147483648]:
  for optional in [0,1]:
   for guard in [0,1,2,-1]:out.append(dict(base,selectors=[selector],optional=optional,guard=guard))
 for selectorlist in [[],list(range(12)),[4,0,4,11,-1,8,9,10,12]]:
  for values in [0,1,7]:
   for old in range(4):
    for optional in [0,1]:out.append(dict(base,selectors=selectorlist,values=values,old=old,optional=optional,negative_relative=True))
 for fail in range(1,6):
  for guard in [0,1]:
   for optional in [0,1]:
    for old in [0,1,2]:out.append(dict(base,selectors=list(range(12)),fail=fail,guard=guard,optional=optional,old=old))
 for mutation in ['grow','clamp','shrink','no_allocator']:
  for index in [1,2,3,4]:
   for values in [0,2,5]:out.append(dict(base,selectors=[4,11],mutation=mutation,mutate_index=index,values=values))
 for selectorlist in [[],list(range(12)),[4,11,0,4]]:
  for optional in [0,1]:
   for values in [0,4]:out.append(dict(base,selectors=selectorlist,optional=optional,values=values,constructor=True))
 for selectorlist in [[],list(range(12)),[4,11,0,4]]:
  for optional in [0,1]:
   for values in [0,4]:out.append(dict(base,selectors=selectorlist,optional=optional,values=values,constructor=True,destructor=True))
 for fail in range(1,6):
  for optional in [0,1]:
   for old in [0,1]:out.append(dict(base,fail=fail,optional=optional,old=old,destructor=True))
 for guard in [0,1,2,-1]:out.append(dict(base,selectors=list(range(12)),guard=guard,repeats=3))
 unique={json.dumps(x,sort_keys=True):x for x in out};return list(unique.values())
if __name__=='__main__':
 binary=Path('data/ver/eu/code.bin').read_bytes();assert hashlib.sha256(binary).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
 with Path('build/research/replay.axf').open('rb') as f:candidate=ELFFile(f).get_section_by_name('CODE').data()
 coverage=set();caseset=cases();passed=0;tracecalls=0
 for i,case in enumerate(caseset):
  a,visited=run(binary,None,case);b,_=run(binary,candidate,case)
  for key in a:
   if a[key]!=b[key]:
    print('FAIL',i,case,key)
    if key in ['trace','returns']:print(a[key],b[key])
    else:print([(hex(j),x,y) for j,(x,y) in enumerate(zip(a[key],b[key])) if x!=y][:32])
    raise SystemExit(1)
  coverage.update(visited);passed+=1;tracecalls+=len(a['trace'])
 summary={'pairs':passed,'fixture_bytes_per_pair':0x20000,'global_bytes_per_pair':0x98,'ordered_boundary_calls':tracecalls,'original_pc_count':len(coverage),'original_pcs':[hex(x) for x in sorted(coverage)],'candidate_sha256':hashlib.sha256(candidate).hexdigest()}
 Path('build/research/replay.json').write_text(json.dumps(summary,indent=2));print({k:v for k,v in summary.items() if k!='original_pcs'})
```
