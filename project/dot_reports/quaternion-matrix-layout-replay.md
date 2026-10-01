# Matrix22 and quaternion bounded ARM1176 replay

This is a diagnostic harness, not the exact checker. It executes only canonical ARMCC sections and the original locally supplied EU interval. It validates every recorded object input before replay and rejects unexpected relocations. Use installed Unicorn 2.1.4 and pyelftools; do not install software just to run this note.

The final tree contains the guarded Matrix22 source. For the quaternion baseline, use an isolated worktree at commit `0f757fe` and build that committed source with the same normal project command. The final tree deliberately omits the disproven quaternion baseline. The pragma attempt and failed helper forms are not replayed by this harness.

Extract the Python block into ignored `build/packet_replay/replay.py`. Run from the chosen worktree root after sourcing development_environment.sh (on the dot Linux host set DEVKITARM=/usr):

```sh
python make.py eu
python tools/check.py _ZN4sead15Matrix22CalcCtrIfE8multiplyERN2nn4math5MTX22ERKS4_S7_ --object build/eu/obj/lib/al/src/Math/seadMatrix22Multiply.o
python build/packet_replay/replay.py matrix build/eu/obj/lib/al/src/Math/seadMatrix22Multiply.o build/packet_replay/matrix-result.json
```

For the committed quaternion baseline worktree:

```sh
python make.py eu
python tools/check.py _ZN4sead11QuatCalcCtrIfE18makeVectorRotationERN2nn4math4QUATERKNS3_4VEC3ES8_ --object build/eu/obj/lib/al/src/Math/seadQuatVectorRotation.o
python build/packet_replay/replay.py quat build/eu/obj/lib/al/src/Math/seadQuatVectorRotation.o build/packet_replay/quaternion-result.json
```

The unchanged exact checks are expected to return exit 1 for full-size mismatches. Run them sequentially: the CLI writes local map ranks. Do not commit those diagnostic map changes. The harness compares memory and the quaternion boolean result; Matrix22's unspecified public return contract is not inferred. All condition-code differences are counted separately, while non-NZCV FPSCR differences fail the status comparison. Cases use nonoverlapping records except the explicitly legal same-MTX22-object aliases. No quaternion aliasing contract is assumed.

```python
from pathlib import Path
import sys,json,hashlib,struct,random,math
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM
from unicorn.arm_const import *
from elftools.elf.elffile import ELFFile
root=Path.cwd();modearg=sys.argv[1];obj=Path(sys.argv[2]);outname=sys.argv[3]
quaternion=modearg=='quat';sym='_ZN4sead11QuatCalcCtrIfE18makeVectorRotationERN2nn4math4QUATERKNS3_4VEC3ES8_' if quaternion else '_ZN4sead15Matrix22CalcCtrIfE8multiplyERN2nn4math5MTX22ERKS4_S7_'
start,end=(0x260554,0x260600) if quaternion else (0x27c208,0x27c23c)
record=json.loads(obj.with_suffix('.provenance.json').read_text());assert hashlib.sha256(obj.read_bytes()).hexdigest()==record['object_sha256']
for name,sha in record['inputs'].items(): assert hashlib.sha256((root/name).read_bytes()).hexdigest()==sha,name
binary=(root/'data/ver/eu/code.bin').read_bytes();assert hashlib.sha256(binary).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
with obj.open('rb') as f:
 e=ELFFile(f);sec=e.get_section_by_name('i.'+sym);candidate=sec.data()
 assert not any(s.name.startswith('.rel') and s['sh_info']==e.get_section_index(sec.name) and s['sh_size'] for s in e.iter_sections())
retail=binary[start-0x100000:end-0x100000]
DATA=0x1000000;STOP=0x71000000
class Engine:
 def __init__(self,code):
  self.u=u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176);u.mem_map(start&~4095,4096);u.mem_write(start,code);u.mem_map(DATA,4096);u.mem_map(STOP,4096);u.reg_write(UC_ARM_REG_C1_C0_2,0xf<<20);u.reg_write(UC_ARM_REG_FPEXC,1<<30)
 def run(self,a,b,mode,alias):
  u=self.u;u.mem_write(DATA,b'\xa5'*1024);pa=DATA+256;pb=DATA+512;po=DATA+768
  if alias=='same_inputs':pb=pa
  if alias=='output_left':po=pa
  if alias=='output_right':po=pb
  if alias=='all_same':pb=pa;po=pa
  u.mem_write(pa,struct.pack('<%dI'%len(a),*a));u.mem_write(pb,struct.pack('<%dI'%len(b),*b))
  u.reg_write(UC_ARM_REG_R0,po);u.reg_write(UC_ARM_REG_R1,pa);u.reg_write(UC_ARM_REG_R2,pb);u.reg_write(UC_ARM_REG_CPSR,0x10);u.reg_write(UC_ARM_REG_LR,STOP);u.reg_write(UC_ARM_REG_FPSCR,mode)
  for i in range(4,12):u.reg_write(UC_ARM_REG_R0+i,0xab000000+i)
  for i in range(16,32):u.reg_write(UC_ARM_REG_S0+i,0x3f800000+i)
  u.emu_start(start,STOP,count=200);assert u.reg_read(UC_ARM_REG_PC)==STOP
  assert all(u.reg_read(UC_ARM_REG_R0+i)==0xab000000+i for i in range(4,12))
  assert all(u.reg_read(UC_ARM_REG_S0+i)==0x3f800000+i for i in range(16,32))
  return bytes(u.mem_read(DATA,1024)),u.reg_read(UC_ARM_REG_R0) if quaternion else None,u.reg_read(UC_ARM_REG_FPSCR),struct.unpack('<4I',u.mem_read(po,16))
def bits(x):return struct.unpack('<I',struct.pack('<f',x))[0]
special=[0,0x80000000,0x3f800000,0xbf800000,0x40000000,0x7f800000,0xff800000,0x7fc00001,0xffc12345,0x7f800001,0xff800001,1,0x80000001,0x007fffff,0x00800000,0x7f7fffff,0xff7fffff]
n=3 if quaternion else 4;rng=random.Random(start);base=[0x3f800000]+[0]*(n-1);cases=[('finite',tuple(base),tuple(base))]
for v in special:
 for i in range(2*n):
  a=base.copy();b=base.copy();(a if i<n else b)[i%n]=v;cases.append(('special',tuple(a),tuple(b)))
for i in range(512):cases.append(('raw',tuple(rng.getrandbits(32) for _ in range(n)),tuple(rng.getrandbits(32) for _ in range(n))))
for i in range(256):
 a=[rng.uniform(-1,1) for _ in range(n)];b=[rng.uniform(-1,1) for _ in range(n)]
 if quaternion:
  an=math.sqrt(sum(v*v for v in a));bn=math.sqrt(sum(v*v for v in b));a=[v/an for v in a];b=[v/bn for v in b]
 cases.append(('normalized' if quaternion else 'finite',tuple(bits(v) for v in a),tuple(bits(v) for v in b)))
if quaternion:
 for v in [0xbf800000,0xbf7fffff,0xbf7ffffe,0xbf7ffffd,0xbf800001]:cases.append(('threshold',(v,0,0),(0x3f800000,0,0)))
modes={'RN':0,'RP':1<<22,'RM':2<<22,'RZ':3<<22,'RN_FZ':1<<24,'RN_DN':1<<25,'RN_FZ_DN':3<<24};aliases=['distinct'] if quaternion else ['distinct','same_inputs','output_left','output_right','all_same'];engines=[Engine(retail),Engine(candidate)];failures=[];stats={};condition_diffs=0
for name,mode in modes.items():
 for alias in aliases:
  for i,(kind,a,b) in enumerate(cases):
   ra,rb=[e.run(a,b,mode,alias) for e in engines];condition_diffs+=bool((ra[2]^rb[2])&0xf0000000)
   data_equal=ra[:2]==rb[:2];status_equal=(ra[2]&0x0fffffff)==(rb[2]&0x0fffffff)
   key=kind;group=stats.setdefault(key,{'pairs':0,'data_failures':0,'status_failures':0});group['pairs']+=1;group['data_failures']+=not data_equal;group['status_failures']+=not status_equal
   if not data_equal or not status_equal:
    if len(failures)<25:failures.append({'mode':name,'alias':alias,'case':i,'kind':kind,'a':[hex(x) for x in a],'b':[hex(x) for x in b],'result':[[hex(x) for x in r[3]] for r in [ra,rb]],'return':[ra[1],rb[1]],'fpscr':[hex(ra[2]),hex(rb[2])],'data_equal':data_equal,'status_equal':status_equal})
summary={'symbol':sym,'source':record['source'],'source_sha256':record['inputs'][record['source']],'candidate_bytes':len(candidate),'candidate_sha256':hashlib.sha256(candidate).hexdigest(),'retail_sha256':hashlib.sha256(retail).hexdigest(),'object_sha256':record['object_sha256'],'cases':len(cases),'modes':modes,'aliases':aliases,'stats':stats,'fpscr_nzcv_differences':condition_diffs,'first_failures':failures,'engine':'Unicorn2.1.4 ARM1176; all written memory, bool result, FPSCR excluding call-clobbered NZCV, r4-r11 and s16-s31; synthetic emulator evidence only; zero exact credit.'}
Path(outname).write_text(json.dumps(summary,indent=2)+'\n');print(json.dumps({k:v for k,v in summary.items() if k not in ['first_failures','modes','aliases']},indent=2));print(json.dumps(failures[:3],indent=2))
```
