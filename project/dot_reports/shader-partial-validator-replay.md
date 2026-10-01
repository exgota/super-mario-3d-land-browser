# Replaying the partial shader validator diagnostic

This appendix preserves the exact six final diagnostic scripts in a reviewable text note. They require the owner's fingerprinted EU executable, the normal ARMCC 4.0/902 project build, pyelftools and Unicorn with ARM1176 support. No game data or generated object is included. Run from the repository root after materializing each fenced block at the named ignored `build/research/` path. The original routines execute from the untouched private executable; candidate sections come exclusively from the normal project-generated object. The allocation test supplies a deterministic external allocation callback and compares its arguments and returned storage. These are bounded fixtures, not game execution or exact-byte acceptance.

Build the committed source with `. ./development_environment.sh`, `export DEVKITARM=/usr`, and `python make.py eu -ca`. The canonical command is `python tools/check.py __shv_partialValidateShaderValidator --object build/eu/obj/lib/CtrSDK/sources/shv_PartialValidator.o`; it reports M because 16,276 complete compiled section bytes differ in size from the complete 14,408-byte original interval. The ordinary compact build leaves this proposal unaccepted until the main run imports it.

The following preparation script resolves only existing map identities, links the unmodified canonical object into an isolated diagnostic image, and runs all six suites sequentially. It does not change the target, map, checker or object.

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
import csv, subprocess, sys
root = Path.cwd()
output = root / 'build/research'
output.mkdir(parents=True, exist_ok=True)
obj = root / 'build/eu/obj/lib/CtrSDK/sources/shv_PartialValidator.o'
with obj.open('rb') as stream:
    elf = ELFFile(stream)
    names = [s.name for s in elf.get_section_by_name('.symtab').iter_symbols()
             if s.name and s['st_shndx'] == 'SHN_UNDEF' and not s.name.startswith('Lib$$')]
rows = list(csv.reader((root / 'data/ver/eu/map.csv').open()))[1:]
lines = ['#<SYMDEFS>#']
for name in names:
    row = next(r for r in rows if r[6] == name or
               name == ('dat_' if r[5].startswith('d') else 'fn_') + r[0][2:])
    lines.append(row[0] + ' ' + ('D' if row[5].startswith('d') else 'A') + ' ' + name)
(output / 'whole_symbols.sym').write_text('\n'.join(lines) + '\n')
command = [str(root / 'data/compilers/wibo'),
           str(root / 'data/compilers/4.0/902/bin/armlink.exe'),
           '--cpu=MPCore', '--fpu=VFPv2', '--arm_only', '--no_exceptions',
           '--inline', '--datacompressor=off', '--no_debug', '--no_scanlib',
           '--mangled', '--symbols', '--map',
           '--entry=__shv_partialValidateShaderValidator',
           '--keep=__shv_partialValidateShaderValidator', '--ro_base=0x500000',
           '--output=' + str(output / 'whole.axf'),
           '--list=' + str(output / 'whole.map'), str(obj), str(output / 'whole_symbols.sym')]
subprocess.run(command, check=True)
for suite in ['whole', 'tail', 'luts', 'caches', 'edges', 'allocations']:
    subprocess.run([sys.executable, str(output / ('validate_' + suite + '.py'))], check=True)
```

Expected passed fixture counts, each executed twice: whole 910, tail 540, luts 440, caches 300, edges 120, allocations 60. All final failure lists are empty. Despite the `tail` label, every preserved script below enters both complete roots and checks their return, without stopping the original at an internal boundary. Stack and callee-saved integer registers are additionally checked in the caches, edges and allocations suites. The data-dependent numeric results and emitted bytes are compared; FPSCR exception flags and GPU execution are not claimed.

## build/research/validate_whole.py

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
 u.mem_map(0x500000,0x10000);u.mem_map(0x600000,0x1000)
 entry=0x377fd0
 if candidate:
  with (ROOT/'build/research/whole.axf').open('rb') as f:
   e=ELFFile(f)
   for sec in e.iter_sections():
    if sec['sh_flags']&2 and sec['sh_size'] and sec['sh_type']=='SHT_PROGBITS':u.mem_write(sec['sh_addr'],sec.data())
   entry=e['e_entry']
 u.mem_map(S,0x40000);u.mem_map(0x900000,0x10000);u.mem_map(END,0x1000)
 u.reg_write(UC_ARM_REG_C1_C0_2,0xf00000);u.reg_write(UC_ARM_REG_FPEXC,0x40000000)
 u.mem_write(0x3e2e48,struct.pack('<I',V));u.mem_write(0x3e3154,struct.pack('<I',C))
 def hook(u,address,size,data):

  if address==0x600000:
   u.mem_write(u.reg_read(UC_ARM_REG_R0),bytes(u.reg_read(UC_ARM_REG_R1)))
   u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
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
 w(C+4,0);w(C+8,0);by(C+0x19,seed%2);by(C+0x66c,0);w(C+0x508,1)
 by(C+0x578,seed%2);w(C+0x5c8,400);w(C+0x5cc,240)
 for off in [0x514,0x518,0x51c,0x520]:w(C+off,r.choice([-20,0,1,10,240,400,450]))
 for off in [0x1b4,0x1b8,0x1bc,0x348,0x34c,0x350,0x5f4,0x560,0xd8c]:w(S+off,0)
 for i in range(6):w(S+0x7a8+i*4,0)
 for i in range(189):by(S+0x3f6+i,0)
 w(S+0xdb8,0x6030);w(C+0x574,0x6030)
 for off in [0x44,0x4c,0x50,0xdcc]:w((C if off!=0xdcc else S)+off,0)
 by(C+0x54,0)
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
 return blob
fail=[];passed=0
for seed in range(130):
 for kind in ['empty','all','fixed','mixed','planar','client','interleaved']:
  category=[0,1,4,8,0x200,0x1000,0x120d][seed%7]
  dirty=[0,0x200,0x100000,0x1800000,0x600000,0x8042,0x80c2,0xffffffff][seed%8]
  blob=fixture(seed,kind);struct.pack_into('<I',blob,F-S,dirty)
  outputs=[]
  for cand,(u,entry) in enumerate((ORIG,CAND)):
   u.mem_write(S,bytes(blob));u.mem_write(0x900000,bytes(0x10000));u.mem_write(0x3e2e30,struct.pack('<II',CMD,CMD+0x10000));u.mem_write(0x3e2e4c,struct.pack('<I',seed%3))
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
Path('build/research/whole-results.json').write_text(json.dumps({'passed':passed,'failed':len(fail),'failures':fail},indent=2))
```

## build/research/validate_tail.py

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
 u.mem_map(0x500000,0x10000);u.mem_map(0x600000,0x1000)
 entry=0x377fd0
 if candidate:
  with (ROOT/'build/research/whole.axf').open('rb') as f:
   e=ELFFile(f)
   for sec in e.iter_sections():
    if sec['sh_flags']&2 and sec['sh_size'] and sec['sh_type']=='SHT_PROGBITS':u.mem_write(sec['sh_addr'],sec.data())
   entry=e['e_entry']
 u.mem_map(S,0x40000);u.mem_map(0x900000,0x10000);u.mem_map(END,0x1000)
 u.reg_write(UC_ARM_REG_C1_C0_2,0xf00000);u.reg_write(UC_ARM_REG_FPEXC,0x40000000)
 u.mem_write(0x3e2e48,struct.pack('<I',V));u.mem_write(0x3e3154,struct.pack('<I',C))
 def hook(u,address,size,data):

  if address==0x600000:
   u.mem_write(u.reg_read(UC_ARM_REG_R0),bytes(u.reg_read(UC_ARM_REG_R1)))
   u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
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
 w(C+4,0);w(C+8,0);by(C+0x19,seed%2);by(C+0x66c,0);w(C+0x508,1)
 by(C+0x578,seed%2);w(C+0x5c8,400);w(C+0x5cc,240)
 for off in [0x514,0x518,0x51c,0x520]:w(C+off,r.choice([-20,0,1,10,240,400,450]))
 for off in [0x1b4,0x1b8,0x1bc,0x348,0x34c,0x350,0x5f4,0x560,0xd8c]:w(S+off,0)
 for i in range(6):w(S+0x7a8+i*4,0)
 for i in range(189):by(S+0x3f6+i,0)
 w(S+0xdb8,0x6030);w(C+0x574,0x6030)
 for off in [0x44,0x4c,0x50,0xdcc]:w((C if off!=0xdcc else S)+off,0)
 by(C+0x54,0)
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
 for i in range(6):w(S+0x7a8+i*4,r.getrandbits(32))
 for i in range(189):by(S+0x3f6+i,r.randrange(16))
 w(C+4,[0,0x10,0x20,2,0x32][seed%5]);w(C+8,r.getrandbits(32));by(C+12,seed%2)
 for bank,dataoff,mapoff,countoff,maskoff in [(0,0x2c,0x30,0x1b0,0x1b4),(1,0x1c0,0x1c4,0x344,0x348)]:
  count=[0,1,4,32,64,96][seed%6];w(S+dataoff,DATA+0x1000*bank)
  w(S+countoff,count)
  for i in range(96):w(S+mapoff+4*i,i if seed%2 else i*2)
  for i in range(3):w(S+maskoff+4*i,r.getrandbits(32)&((1<<min(32,max(0,count-i*32)))-1))
 vals=[0,0x80000000,0x3f000000,0x3f800000,0xbf800000,0x40000000,0x7f800000,0xff800000,0x7fc00000,0x00000001,0x00800000]
 for off in [0x44,0x4c,0x50]:w(C+off,vals[r.randrange(len(vals))])
 w(S+0xdcc,vals[seed%len(vals)]);by(C+0x54,seed%2);w(C+0x5bc,seed%2)
 for off in [0x57a,0x57b,0x57c,0x57d,0x584,0x585,0x586,0x587,0x588]:by(C+off,r.randrange(2))
 w(C+0x58c,r.randrange(2));w(C+0x5b8,seed%3);w(S+0xdb8,[0x6030,0x6048,0x6051,0][seed%4]);w(C+0x574,0x6030)
 w(S+0x5f4,7 if seed%2 else 0);by(S+0xdf4,seed%3==0)
 return blob
fail=[];passed=0
for seed in range(180):
 for kind in ['empty','mixed','interleaved']:
  category=[2,0x10,0x20,0x400,0x800,0x832,0x1c32,0xffffffff][seed%8]
  dirty=[0,4,0x100,0x300,0x100000,0x1800000,0x600000,0x8042,0x80c2,0x101804][seed%10]
  blob=fixture(seed,kind);struct.pack_into('<I',blob,F-S,dirty)
  outputs=[]
  for cand,(u,entry) in enumerate((ORIG,CAND)):
   u.mem_write(S,bytes(blob));u.mem_write(0x900000,bytes(0x10000));u.mem_write(0x3e2e30,struct.pack('<II',CMD,CMD+[0,8,64,128,512,0x10000][seed%6]));u.mem_write(0x3e2e4c,struct.pack('<I',seed%3))
   cases=[]
   for rep in range(2):
    for j,reg in enumerate(REGS):u.reg_write(reg,0xa0000000+j)
    u.reg_write(UC_ARM_REG_R0,F);u.reg_write(UC_ARM_REG_R1,category)
    u.reg_write(UC_ARM_REG_CPSR,0x10);u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,END);u.reg_write(UC_ARM_REG_FPSCR,[0,0x1000000,0x2000000,0x400000,0x800000,0xc00000][seed%6]);u.mem_write(0x420f4c,struct.pack('<189I',*[0x100+i for i in range(189)]))
    try:u.emu_start(entry,END,count=1000000)
    except UcError as error:
     fail.append({'seed':seed,'kind':kind,'category':hex(category),'dirty':hex(dirty),'candidate':cand,'rep':rep,'error':str(error),'pc':hex(u.reg_read(UC_ARM_REG_PC))});break
    expected=END
    if u.reg_read(UC_ARM_REG_PC) not in [expected,END]:
     fail.append({'seed':seed,'kind':kind,'candidate':cand,'rep':rep,'error':'instruction cap','pc':hex(u.reg_read(UC_ARM_REG_PC))});break
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
Path('build/research/tail-results.json').write_text(json.dumps({'passed':passed,'failed':len(fail),'failures':fail},indent=2))
```

## build/research/validate_luts.py

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
 u.mem_map(0x500000,0x20000)
 entry=0x377fd0
 if candidate:
  with (ROOT/'build/research/whole.axf').open('rb') as f:
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
 for i in range(12):w(S+0x358+i*12,0xffffffff)
 w(C+0x5c8,400);w(C+0x5cc,240)
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
for seed in range(40):
 for kind in ['empty','invalid','uniform','register','depth','lighting','texture','fog','gas','framebuffer','combined']:
  blob=fixture(seed,kind);outputs=[]
  for cand,(u,entry) in enumerate((ORIG,CAND)):
   u.mem_write(S,blob);u.mem_write(CMD,bytes(0x40000));u.mem_write(0xa00000,bytes(0x10000));writew(u,0x3e2e30,CMD);writew(u,0x3e2e34,CMD+(0 if seed==19 else 0x40000))
   writew(u,0x3e2e4c,seed%3)
   for i in range(189):writew(u,0x420f4c+4*i,0x100+i)
   for i in range(18):writew(u,0x421240+4*i,(0x55555555 if i%2 else 0xaaaaaaaa))
   cases=[]
   for rep in range(2):
    for j,reg in enumerate(REGS):u.reg_write(reg,0xa0000000+j)
    u.reg_write(UC_ARM_REG_CPSR,0x10);u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,END);u.reg_write(UC_ARM_REG_FPSCR,0)
    u.reg_write(UC_ARM_REG_R0,F);u.reg_write(UC_ARM_REG_R1,0xffffffff)
    try:u.emu_start(entry,END,count=3000000)
    except UcError as error:
     fail.append({'seed':seed,'kind':kind,'candidate':cand,'rep':rep,'error':str(error),'pc':hex(u.reg_read(UC_ARM_REG_PC))});break
    expected=[END]
    if u.reg_read(UC_ARM_REG_PC) not in expected:
     fail.append({'seed':seed,'kind':kind,'candidate':cand,'rep':rep,'error':'instruction cap','pc':hex(u.reg_read(UC_ARM_REG_PC))});break
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
Path('build/research/lut-results.json').write_text(json.dumps(out,indent=2))
```

## build/research/validate_caches.py

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
 u.mem_map(0x500000,0x20000)
 entry=0x377fd0
 if candidate:
  with (ROOT/'build/research/whole.axf').open('rb') as f:
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
 for i in range(12):w(S+0x358+i*12,0xffffffff)
 w(C+0x5c8,400);w(C+0x5cc,240)
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
 # Exercise partial cache transfers using already-bound, preconverted slots.
 for i in range(6):w(C+0x10c+4*i,i+1)
 for i in range(8):w(C+0x124+4*i,i+7);w(C+0x144+4*i,i+15)
 for i in range(7):w(C+0x164+4*i,i+23)
 w(C+0x180,30)
 for i in range(3):w(C+0x184+4*i,i+30)
 for countoff,firstoff,n,maxsize in [(0x190,0x214,6,256),(0x1a8,0x22c,8,256),(0x1c8,0x24c,8,256),(0x1e8,0x26c,3,128),(0x1f4,0x278,4,512),(0x204,0x288,1,128)]:
  for i in range(n):
   first=[0,1,maxsize//2-1,maxsize//2,maxsize-2,maxsize-1][(seed+i)%6]
   count=[0,1,2,3,16,128][(seed+i)%6];count=min(count,maxsize-first)
   w(C+countoff+4*i,count);w(C+firstoff+4*i,first)
 return bytes(blob)
fail=[];passed=0
for seed in range(60):
 for kind in ['lighting','texture','fog','gas','combined']:
  blob=fixture(seed,kind);outputs=[]
  for cand,(u,entry) in enumerate((ORIG,CAND)):
   u.mem_write(S,blob);u.mem_write(CMD,bytes(0x40000));u.mem_write(0xa00000,bytes(0x10000));writew(u,0x3e2e30,CMD);writew(u,0x3e2e34,CMD+(0 if seed==19 else 0x40000))
   writew(u,0x3e2e4c,seed%3)
   for i in range(189):writew(u,0x420f4c+4*i,0x100+i)
   for i in range(18):writew(u,0x421240+4*i,(0x55555555 if i%2 else 0xaaaaaaaa))
   cases=[]
   for rep in range(2):
    for j,reg in enumerate(REGS):u.reg_write(reg,0xa0000000+j)
    u.reg_write(UC_ARM_REG_CPSR,0x10);u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,END);u.reg_write(UC_ARM_REG_FPSCR,[0,0x1000000,0x2000000,0x400000,0x800000,0xc00000][seed%6])
    u.reg_write(UC_ARM_REG_R0,F);u.reg_write(UC_ARM_REG_R1,0xffffffff)
    try:u.emu_start(entry,END,count=3000000)
    except UcError as error:
     fail.append({'seed':seed,'kind':kind,'candidate':cand,'rep':rep,'error':str(error),'pc':hex(u.reg_read(UC_ARM_REG_PC))});break
    expected=[END]
    if u.reg_read(UC_ARM_REG_PC) not in expected:
     fail.append({'seed':seed,'kind':kind,'candidate':cand,'rep':rep,'error':'instruction cap','pc':hex(u.reg_read(UC_ARM_REG_PC))});break
    if u.reg_read(UC_ARM_REG_SP)!=SP or [u.reg_read(x)for x in REGS[4:12]]!=[0xa0000000+j for j in range(4,12)]:
     fail.append({'seed':seed,'kind':kind,'candidate':cand,'rep':rep,'error':'ABI restore'});break
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
Path('build/research/cache-results.json').write_text(json.dumps(out,indent=2))
```

## build/research/validate_edges.py

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
 u.mem_map(0x500000,0x20000)
 entry=0x377fd0
 if candidate:
  with (ROOT/'build/research/whole.axf').open('rb') as f:
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
 for i in range(12):w(S+0x358+i*12,0xffffffff)
 w(C+0x5c8,400);w(C+0x5cc,240)
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
   u.mem_write(S,blob);u.mem_write(CMD,bytes(0x40000));u.mem_write(0xa00000,bytes(0x10000));writew(u,0x3e2e30,CMD);writew(u,0x3e2e34,CMD+([0x40000,0,8,64,512,4096][seed%6]))
   writew(u,0x3e2e4c,seed%3)
   for i in range(189):writew(u,0x420f4c+4*i,0x100+i)
   for i in range(18):writew(u,0x421240+4*i,(0x55555555 if i%2 else 0xaaaaaaaa))
   cases=[]
   for rep in range(2):
    for j,reg in enumerate(REGS):u.reg_write(reg,0xa0000000+j)
    u.reg_write(UC_ARM_REG_CPSR,0x10);u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,END);u.reg_write(UC_ARM_REG_FPSCR,[0,0x1000000,0x2000000,0x400000,0x800000,0xc00000][seed%6])
    u.reg_write(UC_ARM_REG_R0,F);u.reg_write(UC_ARM_REG_R1,0xffffffff)
    try:u.emu_start(entry,END,count=3000000)
    except UcError as error:
     fail.append({'seed':seed,'kind':kind,'candidate':cand,'rep':rep,'error':str(error),'pc':hex(u.reg_read(UC_ARM_REG_PC))});break
    expected=[END]
    if u.reg_read(UC_ARM_REG_PC) not in expected:
     fail.append({'seed':seed,'kind':kind,'candidate':cand,'rep':rep,'error':'instruction cap','pc':hex(u.reg_read(UC_ARM_REG_PC))});break
    if u.reg_read(UC_ARM_REG_SP)!=SP or [u.reg_read(x)for x in REGS[4:12]]!=[0xa0000000+j for j in range(4,12)]:
     fail.append({'seed':seed,'kind':kind,'candidate':cand,'rep':rep,'error':'ABI restore'});break
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
Path('build/research/edges-results.json').write_text(json.dumps(out,indent=2))
```

## build/research/validate_allocations.py

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
 u.mem_map(0x500000,0x20000);u.mem_map(0x700000,0x1000);u.mem_map(0x940000,0x20000)
 entry=0x377fd0
 if candidate:
  with (ROOT/'build/research/whole.axf').open('rb') as f:
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
 for i in range(12):w(S+0x358+i*12,0xffffffff)
 w(C+0x5c8,400);w(C+0x5cc,240)
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
  blob=fixture(seed,kind);outputs=[];logs=[]
  for cand,(u,entry) in enumerate((ORIG,CAND)):
   u.alloc_cursor=0x940000;u.alloc_log=[];u.mem_write(0x940000,bytes(0x20000));u.mem_write(S,blob);u.mem_write(CMD,bytes(0x40000));u.mem_write(0xa00000,bytes(0x10000));writew(u,0x3e2e30,CMD);writew(u,0x3e2e34,CMD+(0 if seed==19 else 0x40000))
   writew(u,0x3e2e4c,seed%3)
   for i in range(189):writew(u,0x420f4c+4*i,0x100+i)
   for i in range(18):writew(u,0x421240+4*i,(0x55555555 if i%2 else 0xaaaaaaaa))
   cases=[]
   for rep in range(2):
    for j,reg in enumerate(REGS):u.reg_write(reg,0xa0000000+j)
    u.reg_write(UC_ARM_REG_CPSR,0x10);u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,END);u.reg_write(UC_ARM_REG_FPSCR,0)
    u.reg_write(UC_ARM_REG_R0,F);u.reg_write(UC_ARM_REG_R1,0xffffffff)
    try:u.emu_start(entry,END,count=3000000)
    except UcError as error:
     fail.append({'seed':seed,'kind':kind,'candidate':cand,'rep':rep,'error':str(error),'pc':hex(u.reg_read(UC_ARM_REG_PC))});break
    expected=[END]
    if u.reg_read(UC_ARM_REG_PC) not in expected:
     fail.append({'seed':seed,'kind':kind,'candidate':cand,'rep':rep,'error':'instruction cap','pc':hex(u.reg_read(UC_ARM_REG_PC))});break
    if u.reg_read(UC_ARM_REG_SP)!=SP or [u.reg_read(x)for x in REGS[4:12]]!=[0xa0000000+j for j in range(4,12)]:
     fail.append({'seed':seed,'kind':kind,'candidate':cand,'rep':rep,'error':'ABI restore'});break
    cases.append([bytes(u.mem_read(a,n)) for _,a,n in SNAPS])
   outputs.append(cases);logs.append(u.alloc_log)
  if len(outputs[0])!=2 or len(outputs[1])!=2:continue
  diffs=[]
  if logs[0]!=logs[1]:diffs.append({"allocation_arguments_original":logs[0],"allocation_arguments_candidate":logs[1]})
  for rep in range(2):
   for (name,a,n),left,right in zip(SNAPS,outputs[0][rep],outputs[1][rep]):
    if left!=right:
     off=next(i for i,(x,y) in enumerate(zip(left,right)) if x!=y)
     diffs.append({'buffer':name,'rep':rep,'offset':hex(off),'original':left[off//4*4:off//4*4+4].hex(),'candidate':right[off//4*4:off//4*4+4].hex()})
  if diffs:fail.append({'seed':seed,'kind':kind,'differences':diffs})
  else:passed+=1
out={'passed':passed,'failed':len(fail),'failures':fail}
print(json.dumps({'passed':passed,'failed':len(fail),'sample_failures':fail[:20]},indent=2))
Path('build/research/allocations-results.json').write_text(json.dumps(out,indent=2))
```

## Caller-proven alias arrangement

The partial caller loads control from 0x003E3154 into r4 at 0x0024CEFC..0x0024CF00 and passes r0=r4 at 0x0024CFA4. All six final suites also pass with the dirty-state argument at the control address. This exact fixture-only transformation and rerun produces another 2,370 fixtures with two invocations each, for a combined 9,480 whole-root comparisons across both arrangements. No C++ source or original executable is changed.

```python
from pathlib import Path
import subprocess, sys
names = ['whole', 'tail', 'luts', 'caches', 'edges', 'allocations']
outputs = ['whole', 'tail', 'lut', 'cache', 'edges', 'allocations']
for name, output in zip(names, outputs):
    text = Path('build/research/validate_' + name + '.py').read_text()
    text = text.replace('F=0x806000', 'F=0x802000')
    text = text.replace("'build/research/" + output + "-results.json'",
                        "'build/research/alias-" + output + "-results.json'")
    path = Path('build/research/validate_alias_' + name + '.py')
    path.write_text(text)
    subprocess.run([sys.executable, str(path)], check=True)
```

## Frozen final evidence

Size-label correction: the original manifest's `compiled_bytes: 16260` measured the ELF function symbol. Independent inspection of the same frozen object measures a 16,276-byte complete section, including 16 trailing pool bytes. The two fields below distinguish these quantities. All replay scripts load complete allocated ELF sections; source, object hashes, tested bytes, and results are unchanged.

```json
{
  "source_commit": "b37cf54e36688bda0a9ed0d2213981634dc0f927",
  "code_sha256": "e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64",
  "original_interval": [
    "0x00377FD0",
    "0x0037B818"
  ],
  "original_bytes": 14408,
  "function_symbol_bytes": 16260,
  "complete_compiled_section_bytes": 16276,
  "canonical": "M: complete compiled section size mismatch",
  "fixtures": 4740,
  "comparisons": 9480,
  "failures": 0,
  "sha256": {
    "lib/CtrSDK/sources/shv_PartialValidator.cpp": "46a9c0819f9adf47e2aab61d50a40d45ba89b100209ea529add2d9c13ecda9ef",
    "lib/CtrSDK/include/retail/shv_ValidatorFront.h": "a76e1180f27b2297449a75b3b86ca163d02c0840ff59c27663b5cf0772d08536",
    "lib/CtrSDK/include/retail/shv_PartialValidatorTail.h": "7de2914afb268303c414ef4e1acfa8f7649a7606afd3ef6b905a10db2f3ae9b2",
    "lib/CtrSDK/include/shv_ValidatorAccess.h": "00ed73738c58c07aaaab50b23f89550bb7bff4cc6daf9122d3a19e3fdfe3ebb9",
    "lib/CtrSDK/include/shv_ValidatorTail.h": "81ec5643766d16ccb02431da0a36156655a808b949e199c4c9672288f316fa99",
    "build/eu/obj/lib/CtrSDK/sources/shv_PartialValidator.o": "1f99e7a1c1eec27189c4ae5e3b7a8696eeb7d0c3a0eec1702dd4709c6c146297",
    "build/eu/obj/lib/CtrSDK/sources/shv_PartialValidator.provenance.json": "7ca59f3a01843e4a51a6c39d61e140d961fc6f250084af7cc489713f34cbdd5c",
    "build/research/whole.axf": "ef942251e4f70c633b9d39c3a882842862c34a3eed3790deffa4d519df0635ef",
    "build/research/whole_symbols.sym": "16066001da5cf9b9f645eabc667cc64a0f9828453b9c40aea5302a3b341b9ab3",
    "build/research/canonical-final.txt": "45a54c1af5d739b44f5160edbcdf1fc69f44ef2fcbd9a80fcf97a4b1b6682ae0",
    "build/research/build-final-clean.log": "c1fd94ef9bd0f7c131e4cdd854329c3a9b5ae9675b6064ebe7b8e0b4b4593d9b",
    "build/research/validate_whole.py": "e9fe1fd65966bcbf740cd7ee9c2dfcc5f85795271ccdbcd6ba524ef3b757e911",
    "build/research/validate_tail.py": "92d4ba00cced6833fec1e8c6d7ae3c2b7638e4421d126d02b29b80d1faf26d92",
    "build/research/validate_luts.py": "fdc644c55bffe62cfb2217347076f627b8a81a7c159b8e7a06a5a086c6862b4c",
    "build/research/validate_caches.py": "5e3b3253de22f29ef1745666d42adc24ba4b164a1e766ad58d30573247e6f20b",
    "build/research/validate_edges.py": "9c8b49d77cd6d708fe4c8442df5f8722758507ab92225a26173c8007b2f0a847",
    "build/research/validate_allocations.py": "57a360685ded541e23514df1d5b41a88bff39ae9fd2f4dc728b5e7efb45de2a5",
    "build/research/whole-results.json": "b38e6fad93591b1395d5c0a02ad58f15caee0f0564f2bb1fda56f563627ac566",
    "build/research/tail-results.json": "5baccadcfab76d74d065edcc0844720c96354f8678d76a1a4cfb3e60287d2b87",
    "build/research/lut-results.json": "0570c235f06e94cf8add968f6741db2c0890f88630ebb3106fc5503c642a76e5",
    "build/research/cache-results.json": "4a5d9737dafe25f43688a519868511ad7f66b31782b79f454d840167146bbd86",
    "build/research/edges-results.json": "50ddd8b101230349cb90eb980099c05d30aa42541aec3b5864e0d1449c543dbe",
    "build/research/allocations-results.json": "f6583a8ce9fb2610ddb204c6d1adeed605738ca7b425f46ce6e205b324967188",
    "build/research/validate_alias_whole.py": "d48f68e2a71e59e09cc05dda1f4bc08dfd2c41e962a07dfa4b19a3fdb8658e41",
    "build/research/alias-whole-results.json": "b38e6fad93591b1395d5c0a02ad58f15caee0f0564f2bb1fda56f563627ac566",
    "build/research/validate_alias_tail.py": "1544e0b060fae247875f8adc78eff943524b51ee8801da4b261d6c51efeeda6d",
    "build/research/alias-tail-results.json": "5baccadcfab76d74d065edcc0844720c96354f8678d76a1a4cfb3e60287d2b87",
    "build/research/validate_alias_luts.py": "1d3565770e62ccf25c05bae20edb8f3b12c6ac28c047bde4c8bd616b59bcaebb",
    "build/research/alias-lut-results.json": "0570c235f06e94cf8add968f6741db2c0890f88630ebb3106fc5503c642a76e5",
    "build/research/validate_alias_caches.py": "c98f995c9759a7bfbc38e08d4ee4252f3f1bd6b7ed02200428ef4fbae5acdd93",
    "build/research/alias-cache-results.json": "4a5d9737dafe25f43688a519868511ad7f66b31782b79f454d840167146bbd86",
    "build/research/validate_alias_edges.py": "59c7d2d5aa9b3152e04514500ea368ab2729a20c0bcf44b8b6c35623dc4ecfd0",
    "build/research/alias-edges-results.json": "50ddd8b101230349cb90eb980099c05d30aa42541aec3b5864e0d1449c543dbe",
    "build/research/validate_alias_allocations.py": "307f19191a56618772cf86eb41200c221245eb46bc3732abf33b70a444cf22e0",
    "build/research/alias-allocations-results.json": "f6583a8ce9fb2610ddb204c6d1adeed605738ca7b425f46ce6e205b324967188"
  },
  "fixtures_per_arrangement": 2370,
  "arrangements": [
    "separate dirty input",
    "caller-proven control alias"
  ]
}
```
