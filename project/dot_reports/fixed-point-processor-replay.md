# Reproduce the 001F9F48 fixed-point replay

This recipe replays the complete original root and the canonical compiled C++ body. It contains no executable or asset data. Use an isolated checkout containing source checkpoint `dc85325ec478671f3fe83404a676a26de5f669e5` or its unchanged source/header content, with the local owner-provided EU `data/ver/eu/code.bin` and `exh.bin`. The executable SHA256 must be `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`. Keep all generated files under ignored `build/fixed-point/`.

Dependencies used here are the project's approved ARMCC 4.1/791 through wibo, its normal Python environment plus Unicorn 2.1.4/pyelftools, and GCC 14.2.0 with UBSan. No new global/per-file compiler settings are needed. Commit source/header changes before the project build. The source guards require `NON_MATCHING`, supplied normally by the project.

## Build and canonical check

From the repository root:

```sh
. ./development_environment.sh
# On this Linux host only; the project's Mac environment uses its existing value.
export DEVKITARM=/usr
mkdir -p build/fixed-point
python make.py eu -ca > build/fixed-point/clean-build.log 2>&1
```

The target row remains unnamed U. This exact temporary naming check preserves its address, pool, end and type and restores every map byte, including the checker-updated rank, in a finally block:

```python
from pathlib import Path
import subprocess
p=Path('data/ver/eu/map.csv'); original=p.read_bytes()
old=b'0x001F9F48,          ,0x001FA184,          ,U,f,,'
new=b'0x001F9F48,          ,0x001FA184,          ,U,f,fn_001F9F48,'
assert original.count(old)==1
try:
    p.write_bytes(original.replace(old,new))
    result=subprocess.run(['python','tools/check.py','fn_001F9F48','--object',
                          'build/eu/obj/lib/al/src/Util/FixedPointBufferState.o'],
                          capture_output=True,text=True)
    print(result.stdout+result.stderr)
    assert result.returncode==1
    assert 'different size from the original interval' in result.stdout
finally:
    p.write_bytes(original)
```

Expected result is a size rejection: 564 compiled bytes versus 572 original bytes, zero exact credit. The complete candidate section SHA256 is `e79602d5bc24c07e365d7a1783476c1c98d6a9181c98637211fbda9b9b454653`. The replay itself verifies canonical build provenance, source/header hashes, executable hash, section hash and absence of relocations before execution. It maps the untouched local image and independently executes original initialization for each side. It never edits the image or substitutes a helper implementation.

## ARM replay script

Save the following block as `build/fixed-point/replay.py`:

```python
from pathlib import Path
from unicorn import *
from unicorn.arm_const import *
from elftools.elf.elffile import ELFFile
import hashlib, struct, json, random, sys
ROOT=Path.cwd(); sys.path.insert(0,str(ROOT))
from tools.low.buildProvenance import verify_build_output
for name,digest in {
 'lib/al/src/Util/FixedPointBufferState.cpp':'9ef1ed62d6b87c5971cd915c626649dc5aede5231a77b27f8b791005abda4d84',
 'lib/al/include/Util/FixedPointBufferState.h':'c1dd6eb34f7fb735914251347ef0927d70184c88781a13a6469135640fd72ea7'
}.items():assert hashlib.sha256((ROOT/name).read_bytes()).hexdigest()==digest,name
B=(ROOT/'data/ver/eu/code.bin').read_bytes()
assert hashlib.sha256(B).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
OBJ=ROOT/'build/eu/obj/lib/al/src/Util/FixedPointBufferState.o'
assert verify_build_output(OBJ)['compiler']=='4.1/791'
with OBJ.open('rb') as f:
 e=ELFFile(f); sec=e.get_section_by_name('i.fn_001F9F48'); candidate=sec.data()
 assert not any(s['sh_type']=='SHT_REL' and s['sh_info']==e.get_section_index(sec.name) and s['sh_size'] for s in e.iter_sections())
assert hashlib.sha256(candidate).hexdigest()=='e79602d5bc24c07e365d7a1783476c1c98d6a9181c98637211fbda9b9b454653'
S,W,I,P,T,K,STOP=0x2000000,0x2100000,0x2200000,0x2300000,0x2310000,0x600000,0x1000000
SP=0x100f000
REGS=[UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11]
def put(u,p,values):u.mem_write(p,struct.pack('<'+'I'*len(values),*[x&0xffffffff for x in values]))
def word(u,p):return struct.unpack('<I',u.mem_read(p,4))[0]
def call(u,pc,*args):
 for n,a in enumerate(args):u.reg_write(UC_ARM_REG_R0+n,a)
 sentinels=[0xbabba000+i for i in range(8)]
 for r,v in zip(REGS,sentinels):u.reg_write(r,v)
 ds=[0x4040404000000000+i for i in range(8)]
 for n,v in enumerate(ds):u.reg_write(UC_ARM_REG_D8+n,v)
 u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,STOP)
 try:u.emu_start(pc,STOP,count=3000000)
 except UcError as ex:return ('fault',str(ex),u.reg_read(UC_ARM_REG_PC))
 if u.reg_read(UC_ARM_REG_PC)!=STOP:return ('limit',u.reg_read(UC_ARM_REG_PC))
 assert [u.reg_read(r) for r in REGS]==sentinels,('ABI GPR',hex(pc))
 assert u.reg_read(UC_ARM_REG_SP)==SP,('ABI SP',hex(pc))
 assert [u.reg_read(UC_ARM_REG_D8+n) for n in range(8)]==ds,('ABI VFP',hex(pc))
 return ('return',u.reg_read(UC_ARM_REG_R0))
def initialized(params=None,misalign=0):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176)
 u.mem_map(0x100000,0x400000);u.mem_write(0x100000,B)
 u.mem_map(K,0x1000);u.mem_write(K,candidate)
 u.mem_map(STOP,0x10000);u.mem_map(S,0x400000)
 u.reg_write(UC_ARM_REG_C1_C0_2,0xf00000);u.reg_write(UC_ARM_REG_FPEXC,0x40000000)
 for pc,args in [(0x385988,()),(0x1fa598,(S,))]: assert call(u,pc,*args)[0]=='return'
 if params:
  d1,decay,d2,terminal,damping,primary,out,lengths=params
  put(u,T,lengths)
  u.mem_write(P,struct.pack('<IIIffIff',d1,decay,d2,terminal,damping,T,primary,out))
  assert call(u,0x1fa454,S,P)==('return',1),params
 size=call(u,0x1fa374,S)[1]
 assert size<0x100000
 assert call(u,0x1fa350,S,W+misalign,size)==('return',1)
 assert call(u,0x1f9e20,S)[0]=='return'
 put(u,P,[I,I+0x280,I+0x500,I+0x780])
 return u,size

def snapshot(u,size):return tuple(bytes(u.mem_read(p,n)) for p,n in [(S,0x104),(W,0x100000),(I,0xa00),(P,16)])
def inputs(kind,seed):
 rng=random.Random(seed)
 boundary=[0,1,127,128,129,0x7fffffff,0x80000000,0x80000001,0xffffffff,0xffffff81,0xffffff80,0x1000000,0xff000000]
 if kind=='boundary':return [boundary[(i+seed)%len(boundary)] for i in range(640)]
 if kind=='wide':return [rng.getrandbits(32) for _ in range(640)]
 return [rng.randrange(-32768,32768)&0xffffffff for _ in range(640)]

results=[]; failures=[];pairs=0
params_list=[None,(0,1,0,0.,0.,0.,0.,[160,320,480]),(5,4000,15,1.,1.,1.,1.,[160,320,480]),(20,1000,30,.5,.95,.6,.4,[320,480,640]),(60,4000,80,.6,.7,.6,.4,[3040,3680,2080])]
for case,params in enumerate(params_list):
 for kind in ['small','boundary','wide']:
  for align in [0,3,31]:
   a,sa=initialized(params,align);c,sc=initialized(params,align)
   assert sa==sc and snapshot(a,sa)==snapshot(c,sc)
   # More than the longest default feedback ring's 23 blocks; all five wrap.
   for frame in range(26):
    values=inputs(kind,case*100+frame)
    put(a,I,values);put(c,I,values)
    ra=call(a,0x1f9f48,S,P);rc=call(c,K,S,P)
    pairs+=1
    ok=ra[0]==rc[0]=='return' and snapshot(a,sa)==snapshot(c,sc)
    if not ok:failures.append({'case':case,'kind':kind,'align':align,'frame':frame,'original':ra,'candidate':rc});break
   results.append({'case':case,'kind':kind,'align':align,'frames':frame+1,'work_size':sa})
# Synthetic histories and coefficients retain valid initialized pointer geometry.
for seed in range(64):
 a,sa=initialized();c,sc=initialized();rng=random.Random(seed)
 values=[rng.getrandbits(32) for _ in range(sa//4)]
 put(a,W,values);put(c,W,values)
 for off in [0xb8,0xbc,0xc8,0xcc,0xd0,0xdc,0xe0,0xe8]:
  v=[0,1,127,128,0x7fffffff,0x80000000,0xffffffff,rng.getrandbits(32)][(seed+off//4)%8]
  put(a,S+off,[v]);put(c,S+off,[v])
 values=inputs('boundary',seed);put(a,I,values);put(c,I,values)
 ra=call(a,0x1f9f48,S,P);rc=call(c,K,S,P);pairs+=1
 if ra[0]!=rc[0] or snapshot(a,sa)!=snapshot(c,sc):failures.append({'synthetic':seed,'original':ra,'candidate':rc})
# Disabled means input table is never touched, even if null/unmapped.
for enabled in [0]:
 a,sa=initialized();c,sc=initialized()
 a.mem_write(S+0x100,bytes([enabled]));c.mem_write(S+0x100,bytes([enabled]))
 before=snapshot(a,sa)
 ra=call(a,0x1f9f48,S,0);rc=call(c,K,S,0);pairs+=1
 assert ra[0]==rc[0]=='return' and snapshot(a,sa)==snapshot(c,sc)==before
# Regression for the packet's accidental extra accumulator addition.
negative_params=(0,4000,0,0.,.5,0.,1.,[160,320,480])
a,sa=initialized(negative_params);c,sc=initialized(negative_params)
for u in [a,c]:
 put(u,word(u,S+0x78),[128]);put(u,S+0xcc,[64])
ra=call(a,0x1f9f48,S,P);rc=call(c,K,S,P);pairs+=1
assert ra[0]==rc[0]=='return' and snapshot(a,sa)==snapshot(c,sc)
assert word(a,I)==32 and (128+64)-((128+64)*64//128)==96
faults=[]
for defect in ['null_inputs','null_primary','null_feedback','huge_primary_index']:
 a,sa=initialized();c,sc=initialized();ptr=P
 if defect=='null_inputs':ptr=0
 else:
  off={'null_primary':0x38,'null_feedback':0x58,'huge_primary_index':0x9c}[defect]
  put(a,S+off,[0x10000000 if defect=='huge_primary_index' else 0]);put(c,S+off,[0x10000000 if defect=='huge_primary_index' else 0])
 ra=call(a,0x1f9f48,S,ptr);rc=call(c,K,S,ptr)
 faults.append({'defect':defect,'original':ra,'candidate':rc})
 assert ra[0]==rc[0]=='fault'
report={'object_sha256':hashlib.sha256(OBJ.read_bytes()).hexdigest(),'section_sha256':hashlib.sha256(candidate).hexdigest(),'section_size':len(candidate),'independent_setups':45*2+64*2+2+2+8,'return_pairs':pairs,'failures':failures,'supported_sequences':results,'unsupported_faults':faults}
Path('build/fixed-point/replay.json').write_text(json.dumps(report,indent=2))
print(json.dumps({k:v for k,v in report.items() if k!='supported_sequences'},indent=2))
assert not failures

```

Run it, then generate the support-only module used by the native script:

```sh
python build/fixed-point/replay.py > build/fixed-point/arm-replay.log 2>&1
python - <<'PY'
from pathlib import Path
s=Path('build/fixed-point/replay.py').read_text()
Path('build/fixed-point/replay_support.py').write_text(s.split('results=[]; failures=[];pairs=0')[0])
PY
```

Expected JSON: section_size 564, independent_setups 230, return_pairs 1236, failures []. Four malformed-state entries separately report an unmapped read in both bodies. They are not counted as equivalence. The successful calls verify r4–r11/SP/d8–d15 and compare the whole receiver, work arena, all four slabs and incoming pointer table. Frame histories carry forward between calls; initialization is independent across sides/configurations.

## Native replay script

Save this block as `build/fixed-point/native_replay.py`. It builds this same committed C++ source at -O0 and -O3 under UBSan, translates the independently initialized ARM fields/pointers into the native layout, and compares the original full-root outputs and memory after each block. It deliberately does not execute malformed pointers in native C++.

```python
from replay_support import *
import ctypes as C, subprocess
U=C.c_uint32; UP=C.POINTER(U)
arrayfields=[('parameters',U*14,0),('primaryBuffers',UP*4,0x38),('secondaryBuffers',UP*4,0x48),('feedbackBuffers',UP*8,0x58),('terminalBuffers',UP*4,0x78),('unknown088',U*4,0x88)]
scalars=[('primaryLength',0x98),('primaryIndex',0x9c),('secondaryLength',0xa0),('secondaryIndex',0xa4),('firstFeedbackLength',0xa8),('secondFeedbackLength',0xac),('firstFeedbackIndex',0xb0),('secondFeedbackIndex',0xb4),('firstFeedbackCoefficient',0xb8),('secondFeedbackCoefficient',0xbc),('terminalLength',0xc0),('terminalIndex',0xc4),('terminalCoefficient',0xc8)]
last=[('accumulators',U*4,0xcc),('primaryCoefficient',U,0xdc),('outputCoefficient',U,0xe0),('unknown0E4',U,0xe4),('accumulatorCoefficient',U,0xe8),('retainedParameters',U*5,0xec),('enabled',C.c_ubyte,0x100)]
allfields=arrayfields+[(n,U,o) for n,o in scalars]+last
class State(C.Structure):_fields_=[(n,t) for n,t,o in allfields]
class Native:
 def __init__(self,u):
  self.state=State();self.work=(U*(0x100000//4))();self.samples=(U*640)();self.table=(UP*4)()
  C.memmove(self.work,bytes(u.mem_read(W,0x100000)),0x100000)
  C.memmove(self.samples,bytes(u.mem_read(I,0xa00)),0xa00)
  for n,t,o in allfields:
   if n.endswith('Buffers'):
    for k in range(t._length_):
     addr=word(u,S+o+4*k)
     getattr(self.state,n)[k]=C.cast(C.addressof(self.work)+addr-W,UP) if addr else UP()
   elif issubclass(t,C.Array):
    for k in range(t._length_):getattr(self.state,n)[k]=word(u,S+o+4*k)
   else:setattr(self.state,n,word(u,S+o) if t==U else u.mem_read(S+o,1)[0])
  for k in range(4):self.table[k]=C.cast(C.addressof(self.samples)+0x280*k,UP)
 def set_input(self,values):
  for k,v in enumerate(values):self.samples[k]=v
 def snapshot(self):
  data=bytearray(0x104)
  for n,t,o in allfields:
   if n.endswith('Buffers'):
    for k in range(t._length_):
     ptr=C.cast(getattr(self.state,n)[k],C.c_void_p).value
     v=ptr-C.addressof(self.work)+W if ptr else 0
     struct.pack_into('<I',data,o+4*k,v)
   elif issubclass(t,C.Array):
    for k,v in enumerate(getattr(self.state,n)):struct.pack_into('<I',data,o+4*k,v)
   elif t==U:struct.pack_into('<I',data,o,getattr(self.state,n))
   else:data[o]=getattr(self.state,n)
  return bytes(data),bytes(self.work),bytes(self.samples),struct.pack('<4I',I,I+0x280,I+0x500,I+0x780)
 def run(self,fn,null=False):fn(C.byref(self.state),None if null else self.table)
params_list=[None,(0,1,0,0.,0.,0.,0.,[160,320,480]),(5,4000,15,1.,1.,1.,1.,[160,320,480]),(20,1000,30,.5,.95,.6,.4,[320,480,640]),(60,4000,80,.6,.7,.6,.4,[3040,3680,2080])]
reports=[]
for opt in ['-O0','-O3']:
 out=ROOT/'build/fixed-point'/('native'+opt+'.so')
 cmd=['g++','-std=c++03',opt,'-g','-fPIC','-shared','-fsanitize=undefined','-fno-sanitize-recover=all','-DNON_MATCHING=1','-Ilib/al/include','lib/al/src/Util/FixedPointBufferState.cpp','-o',str(out)]
 subprocess.run(cmd,check=True)
 lib=C.CDLL(str(out));fn=lib.fn_001F9F48;fn.argtypes=[C.POINTER(State),C.POINTER(UP)];fn.restype=None
 count=0
 for case,params in enumerate(params_list):
  for kind in ['small','boundary','wide']:
   for align in [0,3,31]:
    a,sa=initialized(params,align);seed,ss=initialized(params,align);n=Native(seed)
    assert snapshot(a,sa)==n.snapshot()
    for frame in range(26):
     values=inputs(kind,case*100+frame);put(a,I,values);n.set_input(values)
     assert call(a,0x1f9f48,S,P)[0]=='return';n.run(fn);count+=1
     assert snapshot(a,sa)==n.snapshot(),(opt,case,kind,align,frame)
 for x in range(64):
  a,sa=initialized();c,sc=initialized();rng=random.Random(x)
  values=[rng.getrandbits(32) for _ in range(sa//4)]
  put(a,W,values);put(c,W,values)
  for off in [0xb8,0xbc,0xc8,0xcc,0xd0,0xdc,0xe0,0xe8]:
   v=[0,1,127,128,0x7fffffff,0x80000000,0xffffffff,rng.getrandbits(32)][(x+off//4)%8]
   put(a,S+off,[v]);put(c,S+off,[v])
  n=Native(c);values=inputs('boundary',x);put(a,I,values);n.set_input(values)
  assert call(a,0x1f9f48,S,P)[0]=='return';n.run(fn);count+=1
  assert snapshot(a,sa)==n.snapshot(),(opt,'synthetic',x)
 a,sa=initialized();c,sc=initialized()
 a.mem_write(S+0x100,b'\0');c.mem_write(S+0x100,b'\0');n=Native(c)
 before=snapshot(a,sa)
 assert call(a,0x1f9f48,S,0)[0]=='return';n.run(fn,True);count+=1
 assert snapshot(a,sa)==n.snapshot()==before
 params=(0,4000,0,0.,.5,0.,1.,[160,320,480])
 a,sa=initialized(params);c,sc=initialized(params)
 for u in [a,c]:put(u,word(u,S+0x78),[128]);put(u,S+0xcc,[64])
 n=Native(c);assert call(a,0x1f9f48,S,P)[0]=='return';n.run(fn);count+=1
 assert snapshot(a,sa)==n.snapshot() and n.samples[0]==32
 reports.append({'optimization':opt,'return_pairs':count,'ubsan':'no diagnostics','command':cmd,'library_sha256':hashlib.sha256(out.read_bytes()).hexdigest()})
Path('build/fixed-point/native-replay.json').write_text(json.dumps(reports,indent=2))
print(json.dumps(reports,indent=2))

```

Run `python build/fixed-point/native_replay.py > build/fixed-point/native-replay.log 2>&1`. Expected JSON: 1,236 return pairs and no UBSan diagnostics for each optimization level. The .so hashes include local build-path/debug metadata and need not match another computer. The source/header, owner executable and canonical ARM section hashes above are the reproducibility identities.

These scripts intentionally retain a 3-million-instruction budget per original call. A fault or limit is reported, not replaced with a model. Valid replay geometry uses distinct setup-created rings and the caller-observed in-place sample slabs. Arbitrary overlapping receiver/rings, non-160-multiple ring sizes, invalid pointer tables and oversized cursors are outside the supported domain. Synthetic coefficient/history cases are arithmetic evidence rather than a claim that the actual game reaches those states. No original hardware or gameplay replay is claimed.
