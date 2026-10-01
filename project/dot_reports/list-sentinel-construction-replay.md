# List sentinel bounded ARM replay

Run from the repository root after the canonical check above. Requires pyelftools, Unicorn2.1.4 and the owner-supplied local EU code.bin. No game bytes are embedded.

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM
from unicorn.arm_const import *
import struct,random,hashlib,json
ROOT=Path.cwd();blob=(ROOT/'data/ver/eu/code.bin').read_bytes();assert hashlib.sha256(blob).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
BASE=0x252054;DB=0x1000000;END=0x71000000;SP=0x70008000
p=max((ROOT/'build/exact_checks/eu').glob('function_00252054_*/candidate.axf'),key=lambda p:p.stat().st_mtime)
with p.open('rb') as f:
 e=ELFFile(f);candidate=next(s.data() for s in e.iter_sections() if s['sh_addr']==BASE and s['sh_size'])
original=blob[BASE-0x100000:BASE-0x100000+44];assert len(candidate)==len(original)==44
engines=[]
for code in [original,candidate]:
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176);u.mem_map(0x100000,0x400000);u.mem_write(0x100000,blob);u.mem_write(BASE,code);u.mem_map(DB,0x10000);u.mem_map(0x70000000,0x10000);u.mem_map(END,4096);engines.append(u)
rng=random.Random(BASE);regs=[UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11]
for n in range(512):
 off=rng.randrange(0,0xffe0,4);obj=DB+off;initial=rng.randbytes(0x10000);expected=bytearray(initial);struct.pack_into('<IIIII',expected,off,0x3d10dc,obj+4,obj+4,0,4)
 results=[]
 for u in engines:
  u.mem_write(DB,initial);u.reg_write(UC_ARM_REG_CPSR,0x10);u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,END);u.reg_write(UC_ARM_REG_R0,obj)
  for i,r in enumerate(regs):u.reg_write(r,0xa5a50000+i)
  u.emu_start(BASE,END,count=32);assert u.reg_read(UC_ARM_REG_PC)==END and u.reg_read(UC_ARM_REG_R0)==obj and u.reg_read(UC_ARM_REG_SP)==SP;assert all(u.reg_read(r)==0xa5a50000+i for i,r in enumerate(regs));results.append(bytes(u.mem_read(DB,0x10000)))
 assert results[0]==results[1]==bytes(expected)
j={'pairs':512,'original_sha256':hashlib.sha256(original).hexdigest(),'candidate_sha256':hashlib.sha256(candidate).hexdigest(),'differing_bytes':sum(a!=b for a,b in zip(original,candidate)),'scope':'Actual ARM1176 original/canonical candidate constructors, randomized aligned object addresses and prior memory; all data memory, returned this pointer, SP and callee-saved registers agree with independent five-word initialization model. No mocked imports. Excludes concurrent observation, MMIO/volatile storage, invalid/faulting addresses and physical hardware. No exact credit.'};print(json.dumps(j,indent=2));(ROOT/'build/list-sentinel-replay-results.json').write_text(json.dumps(j,indent=2))
```
