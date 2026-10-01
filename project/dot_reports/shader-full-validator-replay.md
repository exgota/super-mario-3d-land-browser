# Full-validator replay source appendix

These are diagnostic source scripts, not game data and not matching tools.
Save each named Python block under `build/analysis/` from the repository root.
First build committed source with `python make.py eu`, then run `bootstrap.py`.
Run the four validation scripts and the alias variant afterward. They load the unchanged authorized
EU executable and the canonical object linked by the bootstrap. All generated
files stay under the ignored build directory. Results are bounded controlled
fixtures as described in `shader-full-validator.md`.

The diagnostic does not alter or supply objects to the exact checker. A result
here never changes ranks or grants exact credit. The allocator group models only
the callback, while original command and lookup callees execute from the dump.

## bootstrap.py

```python
from pathlib import Path
import subprocess
root = Path.cwd()
out = root / "build/analysis"
out.mkdir(parents=True, exist_ok=True)
entries = '#<SYMDEFS>#\n0x003E2E30 D dat_003E2E30\n0x003E2E34 D dat_003E2E34\n0x003E2E40 D dat_003E2E40\n0x003E3154 D dat_003E3154\n0x003E2E50 D dat_003E2E50\n0x003E2EB0 D dat_003E2EB0\n0x003E2EC8 D dat_003E2EC8\n0x003E2EE0 D dat_003E2EE0\n0x003A482C D dat_003A482C\n0x003A4844 D dat_003A4844\n0x003A2F6C D dat_003A2F6C\n0x003E2654 D dat_003E2654\n0x00211DD8 A __cb_writeRegs\n0x0028A14C A __cb_multiWriteReg\n0x0028A304 A __cb_fillRegs\n0x00211EE0 A __cb_addDummyWrite\n0x00211DA4 A __tx_getBoundTextureLut\n'
(out / "probe.sym").write_text(entries)
subprocess.run([
    str(root / "data/compilers/wibo"),
    str(root / "data/compilers/4.0/902/bin/armlink.exe"),
    "--cpu=MPCore", "--fpu=VFPv2", "--arm_only", "--no_exceptions",
    "--no_debug", "--no_scanlib", "--mangled", "--symbols", "--map",
    "--ro_base=0x600000", "--entry=__shv_validateShaderValidator",
    "--keep=__shv_validateShaderValidator",
    "--output=build/analysis/full_validator.axf",
    "--list=build/analysis/full_validator.map",
    "build/eu/obj/lib/CtrSDK/sources/shv_FullValidator.o",
    "build/analysis/probe.sym"], check=True)
```

## validate_full_front.py

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import *
from unicorn.arm_const import *
import struct,random,json,hashlib
ROOT=Path.cwd();BINARY=(ROOT/'data/ver/eu/code.bin').read_bytes()
assert hashlib.sha256(BINARY).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
S=0x800000;C=0x802000;V=0x804000;F=0x806000;P=0x808000;DATA=0x810000;CMD=0x820000;SP=0x90f000;END=0x980000
REGS=[UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3,UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11,UC_ARM_REG_R12]
SNAPS=[('shader',S,0x1000),('context',C,0x1000),('validator',V,0x2000),('flags',F,0x100),('command',CMD,0x10000),('cursor',0x3e2e30,8),('global',0x3e2e4c,4)]
def init(candidate):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176)
 u.mem_map(0x100000,0x400000);u.mem_write(0x100000,BINARY)
 u.mem_map(0x500000,0x10000);u.mem_map(0x600000,0x20000)
 entry=0x37b8d0
 if candidate:
  with (ROOT/'build/analysis/full_validator.axf').open('rb') as f:
   e=ELFFile(f)
   for sec in e.iter_sections():
    if sec['sh_flags']&2 and sec['sh_size'] and sec['sh_type']=='SHT_PROGBITS':u.mem_write(sec['sh_addr'],sec.data())
   entry=e['e_entry']
 u.mem_map(S,0x40000);u.mem_map(0x900000,0x10000);u.mem_map(END,0x1000)
 u.reg_write(UC_ARM_REG_C1_C0_2,0xf00000);u.reg_write(UC_ARM_REG_FPEXC,0x40000000)
 u.mem_write(0x3e2e48,struct.pack('<I',V));u.mem_write(0x3e3154,struct.pack('<I',C))
 def hook(u,address,size,data):
  if False:pass

 u.hook_add(UC_HOOK_CODE,hook)
 return u,entry
ORIG=init(False);CAND=init(True)
def fixture(seed,kind):
 r=random.Random(seed);blob=bytearray(r.randbytes(0x40000))
 def w(a,x):struct.pack_into('<I',blob,a-S,x&0xffffffff)
 def by(a,x):blob[a-S]=x
 w(V,S);w(V+4,0);w(P,DATA);w(P+4,r.choice([0,1,2,7,127,128,129,512,513,800]));w(P+8,DATA+0x2000);w(P+12,r.choice([0,1,8,128,129]));w(P+16,P+0x100)
 w(S+0x3e4,P);w(S+0x3e8,0);w(S+0x3ec,1);by(S+0x3f4,seed%2)
 for i in range(2):
  w(P+0x100+i*232+0x30,DATA+0x3000+i*0x100);w(P+0x100+i*232+0x34,seed%4)
 w(C+4,0);by(C+0x19,seed%2);by(C+0x66c,0);w(C+0x508,1)
 by(C+0x578,seed%2);w(C+0x5c8,400);w(C+0x5cc,240)
 for off in [0x514,0x518,0x51c,0x520]:w(C+off,r.choice([-20,0,1,10,240,400,450]))
 w(C+0x5d0,DATA+0x3ff0);w(C+0x604,0);w(C+0x720,0);w(C+0x724,0)
 w(S+0x884,0xffffffff);w(S+0x8b8,0xffffffff)
 for i in range(12):
  active=kind!='empty' and (kind=='all' or i<seed%13)
  enabled=kind not in ['fixed'] and (kind!='mixed' or i%3!=0)
  w(S+0x358+i*12,i if active else 0xffffffff)
  at=C+0x3e8+i*24
  ty=r.choice([0x1400,0x1401,0x1402,0x1406]);sz={0x1400:1,0x1401:1,0x1402:2,0x1406:4}[ty];components=r.randint(1,4)
  w(at+4,components);w(at+8,ty);w(at+12,0 if kind=='planar' else (sz*components+3)&~3)
  w(at+16,DATA if kind!='client' else 0);by(at+21,int(enabled))
  w(C+0x5d4+i*4,DATA+0x4000+((11-i) if seed%2 else i)*0x40)
 if kind=='interleaved':
  for i in range(12):
   at=C+0x3e8+i*24;w(at+4,4);w(at+8,0x1406);w(at+12,192);w(C+0x5d4+i*4,DATA+0x4000+i*16)
 w(S+0x5f4,0);w(S+0x560,0)
 for i in range(6):w(S+0x7a8+i*4,0)
 return blob
fail=[];passed=0
for seed in range(130):
 for kind in ['empty','all','fixed','mixed','planar','client','interleaved']:
  category=[0,1,4,8,0x200,0x1000,0x120d][seed%7]
  dirty=[0,0x200,0x100000,0x1800000,0x600000,0x8042,0x80c2,0xffffffff][seed%8]
  blob=fixture(seed,kind);struct.pack_into('<I',blob,F-S,dirty);struct.pack_into('<I',blob,C+8-S,(~category|0xc50)&0xffffffff)
  outputs=[]
  for cand,(u,entry) in enumerate((ORIG,CAND)):
   u.mem_write(0x420f4c,struct.pack('<189I',*[0x100+i for i in range(189)]));u.mem_write(0x421240,bytes(72));u.mem_write(S,bytes(blob));u.mem_write(0x900000,bytes(0x10000));u.mem_write(0x3e2e30,struct.pack('<II',CMD,CMD+0x10000));u.mem_write(0x3e2e4c,struct.pack('<I',seed%3))
   cases=[]
   for rep in range(2):
    for j,reg in enumerate(REGS):u.reg_write(reg,0xa0000000+j)
    u.reg_write(UC_ARM_REG_R0,F);u.reg_write(UC_ARM_REG_R1,category)
    u.reg_write(UC_ARM_REG_CPSR,0x10);u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,END);u.reg_write(UC_ARM_REG_FPSCR,0)
    try:u.emu_start(entry,END,count=1000000)
    except UcError as error:
     fail.append({'seed':seed,'kind':kind,'category':hex(category),'dirty':hex(dirty),'candidate':cand,'rep':rep,'error':str(error),'pc':hex(u.reg_read(UC_ARM_REG_PC))});break
    expected=END
    if u.reg_read(UC_ARM_REG_PC) not in [expected,END]:
     fail.append({'seed':seed,'kind':kind,'candidate':cand,'rep':rep,'error':'instruction cap','pc':hex(u.reg_read(UC_ARM_REG_PC))});break
    badabi = u.reg_read(UC_ARM_REG_SP)!=SP or any(u.reg_read(REGS[j])!=0xa0000000+j for j in range(4,12))
    if badabi:
     fail.append({'seed':seed,'kind':kind,'candidate':cand,'rep':rep,'error':'callee-saved ABI differs'});break
    cases.append([bytes(u.mem_read(a,n)) for _,a,n in SNAPS])
   outputs.append(cases)
  if len(outputs[0])!=2 or len(outputs[1])!=2:continue
  diffs=[]
  for rep in range(2):
   for (name,a,n),left,right in zip(SNAPS,outputs[0][rep],outputs[1][rep]):
    if left!=right:
     off=next(i for i,(x,y) in enumerate(zip(left,right)) if x!=y)
     diffs.append({'buffer':name,'rep':rep,'offset':hex(off),'original':left[off//4*4:off//4*4+4].hex(),'candidate':right[off//4*4:off//4*4+4].hex()})
  if diffs:fail.append({'seed':seed,'kind':kind,'category':hex(category),'dirty':hex(dirty),'differences':diffs})
  else:passed+=1
print(json.dumps({'passed':passed,'failed':len(fail),'sample_failures':fail[:20]},indent=2))
Path('build/analysis/whole-front-results.json').write_text(json.dumps({'passed':passed,'failed':len(fail),'failures':fail},indent=2))
```

## validate_full.py

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import *
from unicorn.arm_const import *
import struct,random,json,hashlib
ROOT=Path.cwd();BIN=(ROOT/'data/ver/eu/code.bin').read_bytes()
assert hashlib.sha256(BIN).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
S=0x800000;C=0x802000;V=0x804000;F=0x806000;U=0x808000;L=0x840000;T=0x882000;CMD=0x900000;SP=0xa0d000;END=0xb00000
REGS=[UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3,UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11,UC_ARM_REG_R12]
SNAPS=[('shader',S,0x1000),('context',C,0x1000),('validator',V,0x2000),('flags',F,0x100),('luts',L,0x40000),('commands',CMD,0x40000),('cursor',0x3e2e30,8)]
def writew(u,a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
def init(candidate):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176)
 u.mem_map(0x100000,0x400000);u.mem_write(0x100000,BIN)
 u.mem_map(0x600000,0x20000)
 entry=0x37b8d0
 if candidate:
  with (ROOT/'build/analysis/full_validator.axf').open('rb') as f:
   e=ELFFile(f)
   for sec in e.iter_sections():
    if sec['sh_flags']&2 and sec['sh_size'] and sec['sh_type']=='SHT_PROGBITS':u.mem_write(sec['sh_addr'],sec.data())
   entry=e['e_entry']
 u.mem_map(S,0x90000);u.mem_map(CMD,0x40000);u.mem_map(0xa00000,0x10000);u.mem_map(END,0x1000)
 u.reg_write(UC_ARM_REG_C1_C0_2,0xf00000);u.reg_write(UC_ARM_REG_FPEXC,0x40000000)
 writew(u,0x3e2e48,V);writew(u,0x3e3154,C);writew(u,0x3e3180,T)
 def hook(u,address,size,data):
  pass
 u.hook_add(UC_HOOK_CODE,hook)
 return u,entry
ORIG=init(False);CAND=init(True)
def fixture(seed,kind):
 r=random.Random(seed);blob=bytearray(0x90000)
 def w(a,v):struct.pack_into('<I',blob,a-S,v&0xffffffff)
 def by(a,v):blob[a-S]=v
 def fl(a,v):struct.pack_into('<f',blob,a-S,v)
 w(V,S);w(C+8,0);w(C+4,0);w(F,0)
 # State caches and register dirty sets.
 for i in range(189):
  w(S+0x4b4+4*i,r.getrandbits(32));w(V+0x100c+4*i,r.getrandbits(32));by(S+0x3f6+i,r.choice([0,1,3,7,15]))
 w(S+0x560,0);w(S+0x5f4,0);w(S+0xd8c,0);w(S+0xdb8,0x6030)
 w(C+0x574,0x6030);by(S+0x3f4,seed%2)
 w(S+0x2c,U);w(S+0x1c0,U+0x1000);w(S+0x1b0,96);w(S+0x344,96)
 for i in range(96):
  w(S+0x30+4*i,i+(i//9)*2);w(S+0x1c4+4*i,i+(i//7)*2)
 for i in range(512):w(U+4*i,r.getrandbits(32))
 # Texture lookup identities and all six independent converted caches.
 for i in range(32):
  lut=L+i*0x2000;holder=T+0x1000+4*i
  w(C+0x74+4*i,i+1);w(T+0x828+4*i,holder);w(holder,lut)
  for j in range(512):fl(lut+4+4*j,r.choice([-2.,-1.,-.5,0.,.1,.5,.999,1.,1.1,2.]))
  for off,dest in [(0x804,0x900),(0x808,0xd00),(0x80c,0xf00),(0x810,0x1000),(0x814,0x1200),(0x818,0x1400)]:w(lut+off,lut+dest)
  w(lut+0x81c,0x3f)
 for i in range(6):w(S+0x974+4*i,i)
 for i in range(8):
  by(S+0x9a0+112*i,0);w(S+0xa00+112*i,i+6);w(S+0xa0c+112*i,i+14)
 for i in range(7):w(S+0xd70+4*i,i+22)
 w(S+0xde4,29)
 for i in range(3):w(S+0xdf8+4*i,i+29)
 if kind=='empty':pass
 if kind in ['invalid','combined']:
  w(C+4,r.choice([2,0x10,0x20,0x32]));w(C+8,r.choice([0,2,0x10,0x20,0x32]))
 if kind in ['uniform','combined']:
  for off in [0x1b4,0x1b8,0x1bc,0x348,0x34c,0x350]:w(S+off,r.getrandbits(32))
 if kind in ['register','combined']:
  for i in range(6):w(S+0x7a8+4*i,r.getrandbits(32))
 if kind in ['depth','combined']:
  w(F,4);fl(S+0xdcc,r.choice([0.,0.1,1.,-0.5]));fl(C+0x4c,r.choice([0.,.25,1.]));fl(C+0x50,r.choice([0.,.5,1.]));fl(C+0x44,r.choice([0.,-2.,.5,2.]));by(C+0x54,seed%2);w(C+0x5bc,seed%3);by(C+0xc,seed%3==0)
 if kind in ['lighting','combined']:
  w(S+0x560,1);w(S+0x98c,127);w(S+0x790,0);w(F,0x4008 | (4 if kind=='combined' else 0))
  for i in range(8):by(S+0x9a0+112*i,int(i<seed%9))
 if kind in ['texture','combined']:
  w(S+0xd8c,1);w(S+0x564,0xc000);w(F,0x4020 | (0x400c if kind=='combined' else 0))
 if kind in ['fog','gas','combined']:
  w(S+0x5f4,7 if kind in ['gas','combined'] else 1);w(F,0x02004010 | (0x402c if kind=='combined' else 0))
 if kind in ['framebuffer','combined']:
  w(F,struct.unpack_from('<I',blob,F-S)[0]|0x100);w(S+0xdb8,[0x6030,0x6048,0x6051,0x6000][seed%4]);w(C+0x5b8,seed%3);w(C+0x58c,seed%2)
  for off in [0x584,0x585,0x586,0x587,0x57b,0x588,0x57a,0x57c,0x57d]:by(C+off,r.randrange(2))
 return bytes(blob)
fail=[];passed=0
for seed in range(20):
 for kind in ['empty','invalid','uniform','register','depth','lighting','texture','fog','gas','framebuffer','combined']:
  blob=fixture(seed,kind);outputs=[]
  for cand,(u,entry) in enumerate((ORIG,CAND)):
   u.mem_write(S,blob);u.mem_write(CMD,bytes(0x40000));u.mem_write(0xa00000,bytes(0x10000));writew(u,0x3e2e4c,seed%2);writew(u,0x3e2e30,CMD);writew(u,0x3e2e34,CMD+(0 if seed==19 else 0x40000))
   for i in range(189):writew(u,0x420f4c+4*i,0x100+i)
   for i in range(18):writew(u,0x421240+4*i,(0x55555555 if i%2 else 0xaaaaaaaa))
   cases=[]
   for rep in range(2):
    for j,reg in enumerate(REGS):u.reg_write(reg,0xa0000000+j)
    u.reg_write(UC_ARM_REG_CPSR,0x10);u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,END);u.reg_write(UC_ARM_REG_FPSCR,0)
    if cand:
     u.reg_write(UC_ARM_REG_R0,F)
    else:
     u.reg_write(UC_ARM_REG_R0,F)
    try:u.emu_start(entry,END,count=3000000)
    except UcError as error:
     fail.append({'seed':seed,'kind':kind,'candidate':cand,'rep':rep,'error':str(error),'pc':hex(u.reg_read(UC_ARM_REG_PC))});break
    expected=[END]
    if u.reg_read(UC_ARM_REG_PC) not in expected:
     fail.append({'seed':seed,'kind':kind,'candidate':cand,'rep':rep,'error':'instruction cap','pc':hex(u.reg_read(UC_ARM_REG_PC))});break
    badabi = u.reg_read(UC_ARM_REG_SP)!=SP or any(u.reg_read(REGS[j])!=0xa0000000+j for j in range(4,12))
    if badabi:
     fail.append({'seed':seed,'kind':kind,'candidate':cand,'rep':rep,'error':'callee-saved ABI differs'});break
    cases.append([bytes(u.mem_read(a,n)) for _,a,n in SNAPS])
   outputs.append(cases)
  if len(outputs[0])!=2 or len(outputs[1])!=2:continue
  diffs=[]
  for rep in range(2):
   for (name,a,n),left,right in zip(SNAPS,outputs[0][rep],outputs[1][rep]):
    if left!=right:
     off=next(i for i,(x,y) in enumerate(zip(left,right)) if x!=y)
     diffs.append({'buffer':name,'rep':rep,'offset':hex(off),'original':left[off//4*4:off//4*4+4].hex(),'candidate':right[off//4*4:off//4*4+4].hex()})
  if diffs:fail.append({'seed':seed,'kind':kind,'differences':diffs})
  else:passed+=1
out={'passed':passed,'failed':len(fail),'failures':fail}
print(json.dumps({'passed':passed,'failed':len(fail),'sample_failures':fail[:20]},indent=2))
Path('build/analysis/whole-results.json').write_text(json.dumps(out,indent=2))
```

## validate_full_edges.py

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import *
from unicorn.arm_const import *
import struct,random,json,hashlib
ROOT=Path.cwd();BIN=(ROOT/'data/ver/eu/code.bin').read_bytes()
assert hashlib.sha256(BIN).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
S=0x800000;C=0x802000;V=0x804000;F=0x806000;U=0x808000;L=0x840000;T=0x882000;CMD=0x900000;SP=0xa0d000;END=0xb00000
REGS=[UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3,UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11,UC_ARM_REG_R12]
SNAPS=[('shader',S,0x1000),('context',C,0x1000),('validator',V,0x2000),('flags',F,0x100),('luts',L,0x40000),('commands',CMD,0x40000),('cursor',0x3e2e30,8)]
def writew(u,a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
def init(candidate):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176)
 u.mem_map(0x100000,0x400000);u.mem_write(0x100000,BIN)
 u.mem_map(0x600000,0x20000)
 entry=0x37b8d0
 if candidate:
  with (ROOT/'build/analysis/full_validator.axf').open('rb') as f:
   e=ELFFile(f)
   for sec in e.iter_sections():
    if sec['sh_flags']&2 and sec['sh_size'] and sec['sh_type']=='SHT_PROGBITS':u.mem_write(sec['sh_addr'],sec.data())
   entry=e['e_entry']
 u.mem_map(S,0x90000);u.mem_map(CMD,0x40000);u.mem_map(0xa00000,0x10000);u.mem_map(END,0x1000)
 u.reg_write(UC_ARM_REG_C1_C0_2,0xf00000);u.reg_write(UC_ARM_REG_FPEXC,0x40000000)
 writew(u,0x3e2e48,V);writew(u,0x3e3154,C);writew(u,0x3e3180,T)
 def hook(u,address,size,data):
  if False:pass
 u.hook_add(UC_HOOK_CODE,hook)
 return u,entry
ORIG=init(False);CAND=init(True)
def fixture(seed,kind):
 r=random.Random(seed);blob=bytearray(0x90000)
 def w(a,v):struct.pack_into('<I',blob,a-S,v&0xffffffff)
 def by(a,v):blob[a-S]=v
 def fl(a,v):struct.pack_into('<f',blob,a-S,v)
 w(V,S);w(C+8,0);w(C+4,0);w(F,0)
 # State caches and register dirty sets.
 for i in range(189):
  w(S+0x4b4+4*i,r.getrandbits(32));w(V+0x100c+4*i,r.getrandbits(32));by(S+0x3f6+i,r.choice([0,1,3,7,15]))
 w(S+0x560,0);w(S+0x5f4,0);w(S+0xd8c,0);w(S+0xdb8,0x6030)
 w(C+0x574,0x6030);by(S+0x3f4,seed%2)
 w(S+0x2c,U);w(S+0x1c0,U+0x1000);w(S+0x1b0,96);w(S+0x344,96)
 for i in range(96):
  w(S+0x30+4*i,i+(i//9)*2);w(S+0x1c4+4*i,i+(i//7)*2)
 for i in range(512):w(U+4*i,r.getrandbits(32))
 # Texture lookup identities and all six independent converted caches.
 for i in range(32):
  lut=L+i*0x2000;holder=T+0x1000+4*i
  w(C+0x74+4*i,i+1);w(T+0x828+4*i,holder);w(holder,lut)
  for j in range(512):w(lut+4+4*j,r.choice([0,0x80000000,1,0x80000001,0x007fffff,0x00800000,0x7f7fffff,0xff7fffff,0x7f800000,0xff800000,0x7fc00000,0xffc00000,0x7f800001,0xff800001,0x3f7fffff,0x3f800000,0x3f800001,0x3f000000,0xbf800000,0xc0000000,0x3b800000,0xbb800000,0x3a800000,0xba800000]))
  for off,dest in [(0x804,0x900),(0x808,0xd00),(0x80c,0xf00),(0x810,0x1000),(0x814,0x1200),(0x818,0x1400)]:w(lut+off,lut+dest)
  w(lut+0x81c,0x3f)
 for i in range(6):w(S+0x974+4*i,i)
 for i in range(8):
  by(S+0x9a0+112*i,0);w(S+0xa00+112*i,i+6);w(S+0xa0c+112*i,i+14)
 for i in range(7):w(S+0xd70+4*i,i+22)
 w(S+0xde4,29)
 for i in range(3):w(S+0xdf8+4*i,i+29)
 if kind=='empty':pass
 if kind in ['invalid','combined']:
  w(C+4,r.choice([2,0x10,0x20,0x32]));w(C+8,r.choice([0,2,0x10,0x20,0x32]))
 if kind in ['uniform','combined']:
  for off in [0x1b4,0x1b8,0x1bc,0x348,0x34c,0x350]:w(S+off,r.getrandbits(32))
 if kind in ['register','combined']:
  for i in range(6):w(S+0x7a8+4*i,r.getrandbits(32))
 if kind in ['depth','combined']:
  w(F,4);fl(S+0xdcc,r.choice([0.,0.1,1.,-0.5]));fl(C+0x4c,r.choice([0.,.25,1.]));fl(C+0x50,r.choice([0.,.5,1.]));fl(C+0x44,r.choice([0.,-2.,.5,2.]));by(C+0x54,seed%2);w(C+0x5bc,seed%3);by(C+0xc,seed%3==0)
 if kind in ['lighting','combined']:
  w(S+0x560,1);w(S+0x98c,127);w(S+0x790,0);w(F,0x4008 | (4 if kind=='combined' else 0))
  for i in range(8):by(S+0x9a0+112*i,int(i<seed%9))
 if kind in ['texture','combined']:
  w(S+0xd8c,1);w(S+0x564,0xc000);w(F,0x4020 | (0x400c if kind=='combined' else 0))
 if kind in ['fog','gas','combined']:
  w(S+0x5f4,7 if kind in ['gas','combined'] else 1);w(F,0x02004010 | (0x402c if kind=='combined' else 0))
 if kind in ['framebuffer','combined']:
  w(F,struct.unpack_from('<I',blob,F-S)[0]|0x100);w(S+0xdb8,[0x6030,0x6048,0x6051,0x6000][seed%4]);w(C+0x5b8,seed%3);w(C+0x58c,seed%2)
  for off in [0x584,0x585,0x586,0x587,0x57b,0x588,0x57a,0x57c,0x57d]:by(C+off,r.randrange(2))
 if seed%2:
  for off,n,slots in [(0x10c,6,list(range(6))),(0x124,8,list(range(6,14))),(0x144,8,list(range(14,22))),(0x164,3,[22,23,24]),(0x170,4,[25,26,27,28]),(0x180,1,[29])]:
   for i,slot in enumerate(slots):
    w(C+off+i*4,slot+1)
    first=r.choice([0,1,31,64,127])
    count=r.choice([1,2,3,7,16,31,64,127-first+1])
    if first+count>128:count=128-first
    if off==0x170:
     first=r.choice([0,1,255,256,257,480]);count=min(r.choice([1,2,3,7,16,31]),512-first)
    w(C+off+0x84+i*4,count);w(C+off+0x108+i*4,first)
 return bytes(blob)
fail=[];passed=0
for seed in range(24):
 for kind in ['lighting','texture','fog','gas','combined']:
  blob=fixture(seed,kind);outputs=[]
  for cand,(u,entry) in enumerate((ORIG,CAND)):
   u.mem_write(S,blob);u.mem_write(CMD,bytes(0x40000));u.mem_write(0xa00000,bytes(0x10000));writew(u,0x3e2e4c,seed%2);writew(u,0x3e2e30,CMD);writew(u,0x3e2e34,CMD+([0x40000,0,8,64,512,4096][seed%6]))
   for i in range(189):writew(u,0x420f4c+4*i,0x100+i)
   for i in range(18):writew(u,0x421240+4*i,(0x55555555 if i%2 else 0xaaaaaaaa))
   cases=[]
   for rep in range(2):
    for j,reg in enumerate(REGS):u.reg_write(reg,0xa0000000+j)
    u.reg_write(UC_ARM_REG_CPSR,0x10);u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,END);u.reg_write(UC_ARM_REG_FPSCR,[0,0x1000000,0x2000000,0x400000,0x800000,0xc00000][seed%6])
    if cand:
     u.reg_write(UC_ARM_REG_R0,F)
    else:
     u.reg_write(UC_ARM_REG_R0,F)
    try:u.emu_start(entry,END,count=3000000)
    except UcError as error:
     fail.append({'seed':seed,'kind':kind,'candidate':cand,'rep':rep,'error':str(error),'pc':hex(u.reg_read(UC_ARM_REG_PC))});break
    expected=[END]
    if u.reg_read(UC_ARM_REG_PC) not in expected:
     fail.append({'seed':seed,'kind':kind,'candidate':cand,'rep':rep,'error':'instruction cap','pc':hex(u.reg_read(UC_ARM_REG_PC))});break
    badabi = u.reg_read(UC_ARM_REG_SP)!=SP or any(u.reg_read(REGS[j])!=0xa0000000+j for j in range(4,12))
    if badabi:
     fail.append({'seed':seed,'kind':kind,'candidate':cand,'rep':rep,'error':'callee-saved ABI differs'});break
    cases.append([bytes(u.mem_read(a,n)) for _,a,n in SNAPS])
   outputs.append(cases)
  if len(outputs[0])!=2 or len(outputs[1])!=2:continue
  diffs=[]
  for rep in range(2):
   for (name,a,n),left,right in zip(SNAPS,outputs[0][rep],outputs[1][rep]):
    if left!=right:
     off=next(i for i,(x,y) in enumerate(zip(left,right)) if x!=y)
     diffs.append({'buffer':name,'rep':rep,'offset':hex(off),'original':left[off//4*4:off//4*4+4].hex(),'candidate':right[off//4*4:off//4*4+4].hex()})
  if diffs:fail.append({'seed':seed,'kind':kind,'differences':diffs})
  else:passed+=1
out={'passed':passed,'failed':len(fail),'failures':fail}
print(json.dumps({'passed':passed,'failed':len(fail),'sample_failures':fail[:20]},indent=2))
Path('build/analysis/whole-edge-results.json').write_text(json.dumps(out,indent=2))
```

## validate_full_allocations.py

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import *
from unicorn.arm_const import *
import struct,random,json,hashlib
ROOT=Path.cwd();BIN=(ROOT/'data/ver/eu/code.bin').read_bytes()
assert hashlib.sha256(BIN).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
S=0x800000;C=0x802000;V=0x804000;F=0x806000;U=0x808000;L=0x840000;T=0x882000;CMD=0x900000;SP=0xa0d000;END=0xb00000
REGS=[UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3,UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11,UC_ARM_REG_R12]
SNAPS=[('shader',S,0x1000),('context',C,0x1000),('validator',V,0x2000),('flags',F,0x100),('luts',L,0x40000),('commands',CMD,0x40000),('cursor',0x3e2e30,8),('allocations',0x940000,0x20000)]
def writew(u,a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
def init(candidate):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176)
 u.mem_map(0x100000,0x400000);u.mem_write(0x100000,BIN)
 u.mem_map(0x600000,0x20000);u.mem_map(0x700000,0x1000);u.mem_map(0x940000,0x20000)
 entry=0x37b8d0
 if candidate:
  with (ROOT/'build/analysis/full_validator.axf').open('rb') as f:
   e=ELFFile(f)
   for sec in e.iter_sections():
    if sec['sh_flags']&2 and sec['sh_size'] and sec['sh_type']=='SHT_PROGBITS':u.mem_write(sec['sh_addr'],sec.data())
   entry=e['e_entry']
 u.mem_map(S,0x90000);u.mem_map(CMD,0x40000);u.mem_map(0xa00000,0x10000);u.mem_map(END,0x1000)
 u.reg_write(UC_ARM_REG_C1_C0_2,0xf00000);u.reg_write(UC_ARM_REG_FPEXC,0x40000000)
 writew(u,0x3e2e48,V);writew(u,0x3e3154,C);writew(u,0x3e3180,T);writew(u,0x3e2654,0x700000);u.alloc_cursor=0x940000;u.alloc_log=[]
 def hook(u,address,size,data):
  if address==0x700000:
   args=[u.reg_read(reg) for reg in REGS[:4]];u.alloc_log.append(args);out=u.alloc_cursor;u.alloc_cursor+=(args[3]+255)&~255;u.reg_write(UC_ARM_REG_R0,out);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
  if False:pass
 u.hook_add(UC_HOOK_CODE,hook)
 return u,entry
ORIG=init(False);CAND=init(True)
def fixture(seed,kind):
 r=random.Random(seed);blob=bytearray(0x90000)
 def w(a,v):struct.pack_into('<I',blob,a-S,v&0xffffffff)
 def by(a,v):blob[a-S]=v
 def fl(a,v):struct.pack_into('<f',blob,a-S,v)
 w(V,S);w(C+8,0);w(C+4,0);w(F,0)
 # State caches and register dirty sets.
 for i in range(189):
  w(S+0x4b4+4*i,r.getrandbits(32));w(V+0x100c+4*i,r.getrandbits(32));by(S+0x3f6+i,r.choice([0,1,3,7,15]))
 w(S+0x560,0);w(S+0x5f4,0);w(S+0xd8c,0);w(S+0xdb8,0x6030)
 w(C+0x574,0x6030);by(S+0x3f4,seed%2)
 w(S+0x2c,U);w(S+0x1c0,U+0x1000);w(S+0x1b0,96);w(S+0x344,96)
 for i in range(96):
  w(S+0x30+4*i,i+(i//9)*2);w(S+0x1c4+4*i,i+(i//7)*2)
 for i in range(512):w(U+4*i,r.getrandbits(32))
 # Texture lookup identities and all six independent converted caches.
 for i in range(32):
  lut=L+i*0x2000;holder=T+0x1000+4*i
  w(C+0x74+4*i,i+1);w(T+0x828+4*i,holder);w(holder,lut)
  for j in range(512):fl(lut+4+4*j,r.choice([-2.,-1.,-.5,0.,.1,.5,.999,1.,1.1,2.]))
  for off,dest in [(0x804,0x900),(0x808,0xd00),(0x80c,0xf00),(0x810,0x1000),(0x814,0x1200),(0x818,0x1400)]:w(lut+off,0)
  w(lut+0x81c,0x3f)
 for i in range(6):w(S+0x974+4*i,i)
 for i in range(8):
  by(S+0x9a0+112*i,0);w(S+0xa00+112*i,i+6);w(S+0xa0c+112*i,i+14)
 for i in range(7):w(S+0xd70+4*i,i+22)
 w(S+0xde4,29)
 for i in range(3):w(S+0xdf8+4*i,i+29)
 if kind=='empty':pass
 if kind in ['invalid','combined']:
  w(C+4,r.choice([2,0x10,0x20,0x32]));w(C+8,r.choice([0,2,0x10,0x20,0x32]))
 if kind in ['uniform','combined']:
  for off in [0x1b4,0x1b8,0x1bc,0x348,0x34c,0x350]:w(S+off,r.getrandbits(32))
 if kind in ['register','combined']:
  for i in range(6):w(S+0x7a8+4*i,r.getrandbits(32))
 if kind in ['depth','combined']:
  w(F,4);fl(S+0xdcc,r.choice([0.,0.1,1.,-0.5]));fl(C+0x4c,r.choice([0.,.25,1.]));fl(C+0x50,r.choice([0.,.5,1.]));fl(C+0x44,r.choice([0.,-2.,.5,2.]));by(C+0x54,seed%2);w(C+0x5bc,seed%3);by(C+0xc,seed%3==0)
 if kind in ['lighting','combined']:
  w(S+0x560,1);w(S+0x98c,127);w(S+0x790,0);w(F,0x4008 | (4 if kind=='combined' else 0))
  for i in range(8):by(S+0x9a0+112*i,int(i<seed%9))
 if kind in ['texture','combined']:
  w(S+0xd8c,1);w(S+0x564,0xc000);w(F,0x4020 | (0x400c if kind=='combined' else 0))
 if kind in ['fog','gas','combined']:
  w(S+0x5f4,7 if kind in ['gas','combined'] else 1);w(F,0x02004010 | (0x402c if kind=='combined' else 0))
 if kind in ['framebuffer','combined']:
  w(F,struct.unpack_from('<I',blob,F-S)[0]|0x100);w(S+0xdb8,[0x6030,0x6048,0x6051,0x6000][seed%4]);w(C+0x5b8,seed%3);w(C+0x58c,seed%2)
  for off in [0x584,0x585,0x586,0x587,0x57b,0x588,0x57a,0x57c,0x57d]:by(C+off,r.randrange(2))
 return bytes(blob)
fail=[];passed=0
for seed in range(12):
 for kind in ['lighting','texture','fog','gas','combined']:
  blob=fixture(seed,kind);outputs=[]
  for cand,(u,entry) in enumerate((ORIG,CAND)):
   u.alloc_cursor=0x940000;u.alloc_log=[];u.mem_write(0x940000,bytes(0x20000));u.mem_write(S,blob);u.mem_write(CMD,bytes(0x40000));u.mem_write(0xa00000,bytes(0x10000));writew(u,0x3e2e4c,seed%2);writew(u,0x3e2e30,CMD);writew(u,0x3e2e34,CMD+(0 if seed==19 else 0x40000))
   for i in range(189):writew(u,0x420f4c+4*i,0x100+i)
   for i in range(18):writew(u,0x421240+4*i,(0x55555555 if i%2 else 0xaaaaaaaa))
   cases=[]
   for rep in range(2):
    for j,reg in enumerate(REGS):u.reg_write(reg,0xa0000000+j)
    u.reg_write(UC_ARM_REG_CPSR,0x10);u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,END);u.reg_write(UC_ARM_REG_FPSCR,0)
    if cand:
     u.reg_write(UC_ARM_REG_R0,F)
    else:
     u.reg_write(UC_ARM_REG_R0,F)
    try:u.emu_start(entry,END,count=3000000)
    except UcError as error:
     fail.append({'seed':seed,'kind':kind,'candidate':cand,'rep':rep,'error':str(error),'pc':hex(u.reg_read(UC_ARM_REG_PC))});break
    expected=[END]
    if u.reg_read(UC_ARM_REG_PC) not in expected:
     fail.append({'seed':seed,'kind':kind,'candidate':cand,'rep':rep,'error':'instruction cap','pc':hex(u.reg_read(UC_ARM_REG_PC))});break
    badabi = u.reg_read(UC_ARM_REG_SP)!=SP or any(u.reg_read(REGS[j])!=0xa0000000+j for j in range(4,12))
    if badabi:
     fail.append({'seed':seed,'kind':kind,'candidate':cand,'rep':rep,'error':'callee-saved ABI differs'});break
    cases.append([bytes(u.mem_read(a,n)) for _,a,n in SNAPS])
   outputs.append(cases)
  if len(outputs[0])!=2 or len(outputs[1])!=2:continue
  diffs=[]
  for rep in range(2):
   for (name,a,n),left,right in zip(SNAPS,outputs[0][rep],outputs[1][rep]):
    if left!=right:
     off=next(i for i,(x,y) in enumerate(zip(left,right)) if x!=y)
     diffs.append({'buffer':name,'rep':rep,'offset':hex(off),'original':left[off//4*4:off//4*4+4].hex(),'candidate':right[off//4*4:off//4*4+4].hex()})
  if diffs:fail.append({'seed':seed,'kind':kind,'differences':diffs})
  else:passed+=1
out={'passed':passed,'failed':len(fail),'failures':fail}
print(json.dumps({'passed':passed,'failed':len(fail),'sample_failures':fail[:20]},indent=2))
Path('build/analysis/whole-allocation-results.json').write_text(json.dumps(out,indent=2))
```

## validate_full_alias.py

This reuses the complete tail-focused fixtures with the caller-proven flags and
context alias. Save and run this block after `validate_full.py` exists.

```python
from pathlib import Path
source = Path("build/analysis/validate_full.py").read_text()
source = source.replace("F=0x806000", "F=C")
source = source.replace("whole-results.json", "whole-alias-results.json")
exec(compile(source, "validate_full_alias.py", "exec"))
```
