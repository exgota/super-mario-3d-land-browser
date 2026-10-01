# Quaternion ARM1176 replay

Extract and run this script with repository, canonical object and published source paths. It verifies all object/input provenance hashes before execution. Requires Unicorn2.1.4 and pyelftools. No binary instructions or game data are embedded. Scope is in quaternion-matrix.md.

```python
from pathlib import Path
import struct,random,json,hashlib,sys,math
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM
from unicorn.arm_const import *
from elftools.elf.elffile import ELFFile
assert len(sys.argv)==4,'usage: replay.py REPO CANONICAL_OBJECT SOURCE_FILE'
root=Path(sys.argv[1]);obj=Path(sys.argv[2]);source=Path(sys.argv[3]);provenance=json.loads(obj.with_suffix('.provenance.json').read_text())
assert hashlib.sha256(obj.read_bytes()).hexdigest()==provenance['object_sha256']
for name,sha in provenance['inputs'].items():
 p=source if name==provenance['source'] else root/name
 assert hashlib.sha256(p.read_bytes()).hexdigest()==sha,name
binary=(root/'data/ver/eu/code.bin').read_bytes();assert hashlib.sha256(binary).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
BASE=0x270778;END=0x71000000;OUT=0x1000000;QUAT=OUT+0x100
with obj.open('rb') as f:
 e=ELFFile(f);sec=e.get_section_by_name('i._ZN4sead15Matrix34CalcCtrIfE5makeQERN2nn4math5MTX34ERKNS3_4QUATE');candidate=sec.data()
 assert not any(s.name.startswith('.rel') and s['sh_info']==e.get_section_index(sec.name) and s['sh_size'] for s in e.iter_sections())
retail=binary[BASE-0x100000:0x270844-0x100000]
class Engine:
 def __init__(self,code):
  self.u=u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176);u.mem_map(BASE&~4095,4096);u.mem_write(BASE,code);u.mem_map(OUT,4096);u.mem_map(END,4096)
  u.reg_write(UC_ARM_REG_C1_C0_2,0xf<<20);u.reg_write(UC_ARM_REG_FPEXC,1<<30)
 def run(self,words,mode):
  u=self.u;u.mem_write(QUAT,struct.pack('<4I',*words));u.mem_write(OUT,b'\xa5'*48);u.reg_write(UC_ARM_REG_R0,OUT);u.reg_write(UC_ARM_REG_R1,QUAT);u.reg_write(UC_ARM_REG_CPSR,0x10);u.reg_write(UC_ARM_REG_LR,END);u.reg_write(UC_ARM_REG_FPSCR,mode)
  for i in range(4,12):u.reg_write(UC_ARM_REG_R0+i,0xab000000+i)
  u.emu_start(BASE,END,count=100);assert u.reg_read(UC_ARM_REG_PC)==END
  assert all(u.reg_read(UC_ARM_REG_R0+i)==0xab000000+i for i in range(4,12))
  return bytes(u.mem_read(OUT,48)),u.reg_read(UC_ARM_REG_FPSCR)
def bits(x):return struct.unpack('<I',struct.pack('<f',x))[0]
special=[0,0x80000000,0x3f800000,0xbf800000,0x40000000,0x7f800000,0xff800000,0x7fc00001,0xffc12345,0x7f800001,0xff800001,1,0x80000001,0x007fffff,0x00800000,0x7f7fffff]
cases=[(0,0,0,0x3f800000)]
for v in special:
 for i in range(4):q=[0,0,0,0x3f800000];q[i]=v;cases.append(tuple(q))
rng=random.Random(BASE)
for i in range(256):cases.append(tuple(rng.choice(special) for _ in range(4)))
for i in range(512):cases.append(tuple(rng.getrandbits(32) for _ in range(4)))
for i in range(256):
 vals=[rng.uniform(-1,1) for _ in range(4)];scale=math.sqrt(sum(v*v for v in vals));cases.append(tuple(bits(v/scale) for v in vals))
modes={'RN':0,'RP':1<<22,'RM':2<<22,'RZ':3<<22,'RN_FZ':1<<24,'RN_DN':1<<25,'RN_FZ_DN':3<<24};engines=[Engine(retail),Engine(candidate)];failures=[];passed=0
for name,mode in modes.items():
 for i,q in enumerate(cases):
  a,b=[e.run(q,mode) for e in engines]
  if a!=b:
   failures.append({'mode':name,'case':i,'input_words':[hex(w) for w in q],'output_word_differences':[(j,hex(x),hex(y)) for j,(x,y) in enumerate(zip(struct.unpack('<12I',a[0]),struct.unpack('<12I',b[0]))) if x!=y],'fpscr':[hex(a[1]),hex(b[1])]})
  else:passed+=1
summary={'cases':len(cases),'modes':modes,'pairs':len(cases)*len(modes),'passed':passed,'failures':len(failures),'first_failures':failures[:12],'candidate_bytes':len(candidate),'candidate_sha256':hashlib.sha256(candidate).hexdigest(),'retail_sha256':hashlib.sha256(retail).hexdigest(),'object_sha256':provenance['object_sha256'],'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'engine':'Unicorn2.1.4 ARM1176, all12 output word bits and complete FPSCR compared, valid nonaliasing input/output, finite normalized samples and arbitrary/special binary32 values; emulator evidence only, no exact credit.'}
print(json.dumps(summary,indent=2));Path('/tmp/quaternion-arm-replay-results.json').write_text(json.dumps(summary,indent=2))
```
