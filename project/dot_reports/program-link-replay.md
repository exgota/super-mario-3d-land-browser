# Program-link replay appendix

This is a complete, self-contained analysis recipe for the bounded comparison
reported in `program-link-root.md`. It changes no implementation, target bytes,
map, rank or checker. Run all commands from the repository root after checking
out this branch. Generated files remain under the ignored build directory.

## Prerequisites

Use the normal project development environment and configured ARMCC 4.0 build 902
plus its installed wibo runner. Provide only your own authorized EU executable
at `data/ver/eu/code.bin`; the script checks its required SHA256 before execution.
The normal project's build also needs its existing private exheader/compiler
inputs and Python dependencies as described by AGENTS.md. No binary or extracted
state is embedded in this appendix.

The tested interpreter is Python 3.12.14, with Unicorn 2.1.4 and pyelftools 0.33.
If these two replay packages are absent, install the pinned versions into the
project's existing virtual environment before the recipe:

```sh
. ./development_environment.sh
python -m pip install unicorn==2.1.4 pyelftools==0.33
```

The recipe extracts the entire analysis script from this note, verifies its
hash, runs the ordinary project build, links that untouched canonical object
for diagnostic execution, and asserts the recorded result. The symbol file
contains independently observed original addresses; the link at 0x00600000 is
for behavior only and is not a canonical exact-check artifact.

## Repository-relative recipe

<!-- program-link-replay-setup -->
```sh
set -eu
. ./development_environment.sh
export DEVKITARM=/usr
mkdir -p build/program_link_validation
python - <<'PY_EXTRACT'
from pathlib import Path
import hashlib
import importlib.metadata
assert importlib.metadata.version('unicorn') == '2.1.4'
assert importlib.metadata.version('pyelftools') == '0.33'
text = Path('project/dot_reports/program-link-replay.md').read_text()
marker = '\n<!-- program-link-replay-python -->\n'
body = text.split(marker, 1)[1].split('```python\n', 1)[1].split('\n```', 1)[0] + '\n'
assert hashlib.sha256(body.encode()).hexdigest() == '43d480393cf6e71da7086a6e3e4464744dee96578655416c33d48a8f49122412'
Path('build/program_link_validation/reproduce.py').write_text(body)
PY_EXTRACT
python make.py eu > build/program_link_validation/packaged-build.log 2>&1
cat > build/program_link_validation/original_symbols.sym <<'SYMDEFS'
#<SYMDEFS>#
0x003E2E40 D dat_003E2E40
0x003E2654 D dat_003E2654
0x003E2658 D dat_003E2658
0x003E3154 D dat_003E3154
0x00420160 D dat_00420160
0x0028D1F0 A fn_0028D1F0
0x0028F0A0 A __aeabi_memcpy4
0x0028BA44 A __rt_memcpy
0x0028AA60 A strcmp
SYMDEFS
./data/compilers/wibo ./data/compilers/4.0/902/bin/armlink.exe \
    --cpu=MPCore --fpu=VFPv2 --arm_only --no_exceptions --no_debug \
    --no_scanlib --mangled --symbols --map --ro_base=0x600000 \
    --entry=fn_00245D50 --keep=fn_00245D50 \
    --output=build/program_link_validation/candidate.axf \
    --list=build/program_link_validation/candidate.map \
    build/eu/obj/lib/CtrSDK/sources/ProgramLink.o \
    build/program_link_validation/original_symbols.sym
python build/program_link_validation/reproduce.py > build/program_link_validation/packaged-replay.log
python - <<'PY_VERIFY'
from pathlib import Path
from elftools.elf.elffile import ELFFile
import hashlib, json
r = json.loads(Path('build/program_link_validation/results.json').read_text())
assert r['fixtures'] == 1769
assert r['returning_pairs'] == 1763
assert r['paired_faults'] == 6
assert r['input_groups'] == {'synthetic': 1549, 'retail_initializer': 220}
assert r['failures'] == []
assert r['original_covered_words'] == 1607
assert r['initializer_defaults_sha256'] == 'c64dce6e8b8e62e18b16e1f1ec988bf7c08366031f39c2cce0d2891016c5f737'
obj = Path('build/eu/obj/lib/CtrSDK/sources/ProgramLink.o')
assert r['object_sha256'] == hashlib.sha256(obj.read_bytes()).hexdigest()
with obj.open('rb') as f:
    elf = ELFFile(f)
    code = [(s.name, s['sh_size']) for s in elf.iter_sections() if s.name.startswith('i.')]
assert code == [('i.fn_00245D50', 5908)]
summary = dict(r)
summary.pop('covered_addresses')
print(json.dumps(summary, indent=2))
PY_VERIFY

```

## Complete analysis script

Script SHA256: `43d480393cf6e71da7086a6e3e4464744dee96578655416c33d48a8f49122412`.
The script checks all returning memories, allocation/release call logs, return
completion, stack pointer and r4-r11. Six deliberately failing register-bank
allocations are counted separately as paired faults. As described in the main
report, this does not prove exact fault-site equality, invalid-input equivalence,
real allocator behavior, arbitrary aliasing, or reentrant callbacks.

The tiny callback trampoline lives only in modeled emulator memory; it is not
part of the reconstructed source or canonical object. Both memory-library calls
and the shader-manager initializer execute unchanged original code.

<!-- program-link-replay-python -->
```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import *
from unicorn.arm_const import *
import struct, random, json, hashlib, time, sys
ROOT=Path.cwd(); OUT=ROOT/'build/program_link_validation'; BINARY=(ROOT/'data/ver/eu/code.bin').read_bytes()
assert hashlib.sha256(BINARY).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
S=0x800000;C=0x804000;D=0x808000;R=0x80a000;REF=0x80b000;ST=0x80c000;U0=0x880000;U1=0x890000;STR=0x810000;BIND=0x812000;CHAIN=0x813000;OLD=0x820000;ALLOC=0x840000;SP=0xa1f000;END=0x980000;CALL=0x990000
REGS=[UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3,UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11,UC_ARM_REG_R12]
SNAPS=[('program',S,0xe9c),('context',C,0x1800),('control',D,0x600),('inputs',R,0xa000),('old',OLD,0x10000),('allocated',ALLOC,0x50000),('builtins',0x420160,297*12),('uniforms',U0,0x20000)]
def init(candidate):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176);u.mem_map(0x100000,0x400000);u.mem_write(0x100000,BINARY);u.mem_map(0x600000,0x20000)
 entry=0x245d50
 if candidate:
  with (OUT/'candidate.axf').open('rb') as f:
   e=ELFFile(f)
   for sec in e.iter_sections():
    if sec['sh_flags']&2 and sec['sh_size'] and sec['sh_type']=='SHT_PROGBITS':u.mem_write(sec['sh_addr'],sec.data())
   entry=e['e_entry']
 u.mem_map(S,0x100000);u.mem_map(0xa00000,0x20000);u.mem_map(END,0x1000);u.mem_map(CALL,0x1000)
 u.mem_write(CALL,bytes.fromhex('1eff2fe1')*8)
 u.reg_write(UC_ARM_REG_C1_C0_2,0xf00000);u.reg_write(UC_ARM_REG_FPEXC,0x40000000)
 u.mem_write(0x3e2e48,struct.pack('<I',C));u.mem_write(0x3e3154,struct.pack('<I',D))
 state={}
 def hook(uc,a,n,user):
  if a not in (CALL,CALL+4):return
  args=[uc.reg_read(x) for x in REGS[:4]]
  if a==CALL:
   ix=state['next'];state['next']+=1
   out=0 if state.get('fail')==ix else ALLOC+ix*0x10000
   state['calls'].append(('allocate',args,out));uc.reg_write(UC_ARM_REG_R0,out)
  else:state['calls'].append(('release',args))
  uc.reg_write(UC_ARM_REG_PC,uc.reg_read(UC_ARM_REG_LR))
 u.hook_add(UC_HOOK_CODE,hook,begin=CALL,end=CALL+4)
 return u,entry,state
ORIG=init(False);CAND=init(True)
COVER=set()
def cover(u,a,n,user):
 if 0x245d50<=a<0x2476cc:COVER.update(range(a,min(a+n,0x2476cc),4))
ORIG[0].hook_add(UC_HOOK_BLOCK,cover,begin=0x245d50,end=0x2476cc)
# Obtain table and default registers by executing the untouched retail initializer.
boot,_,_=init(False)
boot.reg_write(UC_ARM_REG_SP,SP);boot.reg_write(UC_ARM_REG_LR,END);boot.reg_write(UC_ARM_REG_R0,C)
boot.emu_start(0x1064e4,END,count=200000)
assert boot.reg_read(UC_ARM_REG_PC)==END
AUTH_DEFAULTS=bytes(boot.mem_read(C+0x1300,0x2f4))
AUTH_ENABLES=bytes(boot.mem_read(C+0x16b1,0xbd))
AUTH_BUILTINS=bytes(boot.mem_read(0x420160,297*12))
AUTH_HASH=hashlib.sha256(AUTH_DEFAULTS+AUTH_ENABLES+AUTH_BUILTINS).hexdigest()

def fixture(case):
 rng=random.Random(case['seed']);blob=bytearray(rng.randbytes(0x100000));table=bytearray(rng.randbytes(297*12))
 def w(a,x):struct.pack_into('<I',blob,a-S,x&0xffffffff)
 def h(a,x):struct.pack_into('<H',blob,a-S,x&65535)
 def b(a,x):blob[a-S]=x
 handle=case.get('handle',0x12345)
 w(C,S if case.get('active') else 0);w(C+8+(handle&511)*4,CHAIN if case.get('chain') else S);w(CHAIN,S);w(CHAIN+4,handle^0x200);w(S,0);w(S+4,handle);w(S+8,1);w(S+12,REF);w(S+16,REF+0x10 if case.get('secondary') else 0);b(S+0x14,case.get('changed',1));b(S+0x15,0);b(S+0x16,case.get('linked',1));b(S+0x17,0)
 w(REF,R);w(REF+4,0);w(REF+16,R if not case.get('differentResource') else R+0x40);w(REF+20,1);w(R+0x10,ST)
 for off in [0x1c,0x2c,0x1c0]:w(S+off,OLD+off*16 if case.get('old') else 0)
 w(S+0x3e4,R if case.get('sameResource') else 0)
 for off in [0x1300]:
  if case.get('zeroDefaults'):blob[C-S+off:C-S+off+0x2f4]=bytes(0x2f4)
 for stageid in range(2):
  st=ST+stageid*0xe8;uniforms=case.get('uniforms'+str(stageid),[]); up=U0 if stageid==0 else U1
  b(st+1,case.get('merge',0));b(st+2,case.get('mode',0));b(st+3,case.get('param3',3));b(st+4,case.get('param4',4));b(st+5,case.get('param5',2));h(st+8,case.get('mask'+str(stageid),0x7f));w(st+0xc,case.get('countC'+str(stageid),4));w(st+0x10,case.get('count10'+str(stageid),5));w(st+0x58,up);w(st+0x5c,len(uniforms));w(st+0xe0,STR)
  for i,u in enumerate(uniforms):
   for j,x in enumerate(u):w(up+i*20+j*4,x)
  outs=case.get('outputs'+str(stageid),[1,2,3,0x1f1f1f1f,0x1f1f1f1f,0x1f1f1f1f,0x1f1f1f1f])
  for i,x in enumerate(outs):w(st+0x38+i*4,x)
  for i in range(16):w(st+0x60+i*8,0);w(st+0x64+i*8,i*16)
 for i in case.get('attributes',[]):w(ST+0x60+i*8,0x1406+i)
 for i in range(16):blob[STR-S+i*16:STR-S+i*16+16]=('attribute%02d'%i).encode().ljust(16,b'\0')
 bindings=case.get('bindings',[]);w(S+0x18,BIND if bindings else 0)
 for i,(attr,slot) in enumerate(bindings):w(BIND+i*12,STR+attr*16);w(BIND+i*12+4,slot);w(BIND+i*12+8,BIND+(i+1)*12 if i+1<len(bindings) else 0)
 for i in range(297):struct.pack_into('<I',table,i*12+4,[0x8b50,0x8b51,0x8b52,0x8b53,0x8b54,0x8b55,0x1406,0xffffffff][(i+case['seed'])%8])
 if case.get('originalDefaults'):
  blob[C-S+0x1300:C-S+0x15f4]=AUTH_DEFAULTS
  blob[C-S+0x16b1:C-S+0x176e]=AUTH_ENABLES
  table=bytearray(AUTH_BUILTINS)
 if case.get('cachedGeometryMode'):
  w(C+0x1300+6*4,0x08000100)
 reject=case.get('reject')
 if reject=='blocked':b(S+0x15,1)
 elif reject=='primary':w(S+12,0)
 elif reject=='attached':w(S+8,0)
 elif reject=='resource':w(REF,0)
 return blob,table,handle
FAIL=[];TOTAL=0;FAULTS=0;START=time.time()
def run(case):
 global TOTAL,FAULTS
 blob,table,handle=fixture(case);results=[]
 for u,entry,state in (ORIG,CAND):
  u.mem_write(S,bytes(blob));u.mem_write(0x420160,bytes(table));u.mem_write(0xa00000,bytes(0x20000));u.mem_write(0x3e2654,struct.pack('<I',0 if case.get('noAllocate') else CALL));u.mem_write(0x3e2658,struct.pack('<I',0 if case.get('noRelease') else CALL+4))
  for j,reg in enumerate(REGS):u.reg_write(reg,0xa0000000+j)
  for j in range(32):u.reg_write(UC_ARM_REG_S0+j,0x3f000000+j)
  u.reg_write(UC_ARM_REG_CPSR,0x10);u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_FPSCR,case.get('fpscr',0));state.clear();state.update(next=0,fail=case.get('fail'),calls=[])
  u.reg_write(UC_ARM_REG_R0,handle);u.reg_write(UC_ARM_REG_LR,END)
  fault=None
  try:u.emu_start(entry,END,count=2000000)
  except UcError as error:fault=(str(error),u.reg_read(UC_ARM_REG_PC),u.reg_read(UC_ARM_REG_R0))
  if not fault and (u.reg_read(UC_ARM_REG_PC)!=END or u.reg_read(UC_ARM_REG_SP)!=SP or [u.reg_read(x) for x in REGS[4:12]]!=[0xa0000000+j for j in range(4,12)]):fault=('completion/ABI',u.reg_read(UC_ARM_REG_PC),u.reg_read(UC_ARM_REG_SP))
  results.append(([bytes(u.mem_read(a,n)) for _,a,n in SNAPS],state['calls'][:],fault))
 TOTAL+=1
 if results[0][2] or results[1][2]:
  if case.get('expectedFault') and results[0][2] and results[1][2]:FAULTS+=1
  else:FAIL.append({'case':case,'faults':[x[2] for x in results]})
  return
 if results[0][1]!=results[1][1]:FAIL.append({'case':case,'calls':[x[1] for x in results]})
 for (label,a,n),x,y in zip(SNAPS,results[0][0],results[1][0]):
  if x!=y:
   dif=[j for j,(p,q) in enumerate(zip(x,y)) if p!=q]
   FAIL.append({'case':case,'buffer':label,'differences':len(dif),'first_offsets':[hex(d) for d in dif[:12]],'original':x[dif[0]//4*4:dif[0]//4*4+4].hex(),'candidate':y[dif[0]//4*4:dif[0]//4*4+4].hex()})

cases=[]
for seed in range(5):
 for secondary in [False,True]:
  for active in [False,True]:
   for mode in range(4):
    cases.append(dict(seed=seed,secondary=secondary,active=active,mode=mode,old=seed%2,chain=seed%3==0,noRelease=seed==3))
for reject in ['blocked','primary','attached','resource']:
 cases.append(dict(seed=100,reject=reject,secondary=True,old=True,active=True))
cases.append(dict(seed=101,secondary=True,differentResource=True))
# Every uniform packing switch and unknown values; all bitset words and edges.
types=[0x1406,0x8b50,0x8b51,0x8b52,0x8b54,0x8b56,0x8b5a,0x8b5b,0x8b5c,0,0x8b53,0x8b55,0x8b57,0x8b59,0x8b5d,0xffffffff,0x7fffffff,0x80000000]
for typ in types:
 for first,n in [(0,0),(0,1),(1,2),(30,3),(31,1),(32,2),(63,2),(64,1),(90,6),(0,96)]:
  for secondary in [False,True]:
   for active in [False,True]:
    cases.append(dict(seed=len(cases)+31,secondary=secondary,active=active,old=True,mode=len(cases)%4,
     uniforms0=[(typ,first,len(cases)%4,17,n)],uniforms1=[(typ,first,3,29,n)]))
# Repeated/overlapping ranges and mixed types give dense indices nontrivial order.
rng=random.Random(324523)
for seed in range(700):
 us=[]
 for stage in range(2):
  records=[]
  for j in range(rng.randrange(0,22)):
   typ=rng.choice(types);first=rng.randrange(96);count=rng.randrange(min(96-first,8)+1)
   if typ in (0x8b54,0x8b56):first=rng.randrange(256);count=rng.randrange(256)
   records.append((typ,first,rng.getrandbits(32),rng.getrandbits(32),count))
  us.append(records)
 attrs=rng.sample(range(16),rng.randrange(17));bindings=[(rng.randrange(16),rng.randrange(12)) for j in range(rng.randrange(8))]
 outall=rng.sample(range(1,100),rng.randrange(8));out0=rng.sample(outall,rng.randrange(len(outall)+1));out1=rng.sample(outall,rng.randrange(len(outall)+1))
 cases.append(dict(seed=seed+1000,handle=rng.getrandbits(32),secondary=seed%3!=0,active=seed%2,chain=seed%4==0,
  old=seed%5!=0,noRelease=seed%7==0,mode=seed%5,merge=seed%2,sameResource=seed%2,uniforms0=us[0],uniforms1=us[1],
  attributes=attrs,bindings=bindings,outputs0=out0+[0x1f1f1f1f]*(7-len(out0)),outputs1=out1+[0x1f1f1f1f]*(7-len(out1)),
  mask0=rng.randrange(65536),mask1=rng.randrange(65536),countC0=rng.getrandbits(32),count100=rng.getrandbits(32),
  count101=rng.getrandbits(32),param3=rng.randrange(256),param4=rng.randrange(256),param5=rng.randrange(256)))
# Reject paths, callback absence, and location-allocation failure.
for secondary in [False,True]:
 for old in [False,True]:
  cases.append(dict(seed=3000,secondary=secondary,old=old,noAllocate=True))
  cases.append(dict(seed=3001,secondary=secondary,old=old,fail=0))
  cases.append(dict(seed=3002,secondary=secondary,old=old,fail=1 if not secondary else 2,
   uniforms0=[(0x1406,31,2,1,5)],uniforms1=[(0x8b51,63,1,2,5)]))
  cases.append(dict(seed=3003,secondary=secondary,old=old,fail=0,expectedFault=True,uniforms0=[(0x1406,2,0,1,1)]))
  if secondary:cases.append(dict(seed=3004,secondary=True,old=old,fail=1,expectedFault=True,
   uniforms0=[(0x1406,2,0,1,1)],uniforms1=[(0x1406,3,0,2,2)]))
# Capacity boundary and exact 96-register collections without excessive stack data.
for count in [2047,2048,2049]:
 cases.append(dict(seed=4000+count,uniforms0=[(0x8b54,i%16,i%4,i*13,1) for i in range(count)],old=True))
 cases.append(dict(seed=4100+count,secondary=True,uniforms0=[(0x8b54,i%16,i%4,i*13,1) for i in range(1024)],
  uniforms1=[(0x8b56,i%16,i%4,i*11,1) for i in range(count-1024)],old=True))
# Floating settings are fixed stores, checked across supported rounding/FZ/DN modes.
for mode in [0,0x1000000,0x2000000,0x400000,0x800000,0xc00000]:
 for secondary in [False,True]:cases.append(dict(seed=5000+mode,secondary=secondary,fpscr=mode,active=True,merge=1))
# Authentic startup defaults and existing geometry-mode cache equality.
for seed in range(220):
 case=cases[805+seed%700].copy();case['originalDefaults']=True;case['seed']=6000+seed;cases.append(case)
for seed in range(8):cases.append(dict(seed=7000+seed,secondary=True,mode=2,cachedGeometryMode=True,active=seed%2))
for case in cases:
 run(case)
 if len(FAIL)>20:break
counts={"synthetic":0,"retail_initializer":0}
for case in cases: counts["retail_initializer" if case.get("originalDefaults") else "synthetic"]+=1
result={"input_groups":counts,'initializer_defaults_sha256':AUTH_HASH,'fixtures':TOTAL,'returning_pairs':TOTAL-FAULTS,'paired_faults':FAULTS,'original_covered_words':len(COVER),'covered_addresses':[hex(a) for a in sorted(COVER)],'failures':FAIL,'seconds':time.time()-START,'object_sha256':hashlib.sha256((ROOT/'build/eu/obj/lib/CtrSDK/sources/ProgramLink.o').read_bytes()).hexdigest()}
(OUT/'results.json').write_text(json.dumps(result,indent=2));print(json.dumps(result,indent=2))
```

## Packaged-recipe verification

The repository-relative setup block above was extracted from this Markdown
and executed on 2026-10-01 after notes-only parent commit `be9e79c`.
The normal build reported the SDK sources unchanged. Source/header and canonical
object SHA256 values were checked before and after execution; all were identical.
The object remains
`746bbf1ccccaf338a461d30bc3c0aa5bd0750af1edc4675cb5bf94e23a8a7042`.

The packaged run reproduced 1,769 fixture pairs: 1,763 returning pairs,
6 paired faults, 1,549 synthetic-state cases, and 220 original-initializer cases.
It reported zero differences, a 5,908-byte single code section, and all 1,607
static executable word addresses in visited retail blocks. The assertions in
the setup block checked these totals and the initializer-data hash.
This remains bounded behavioral evidence and adds no canonical exact credit.
