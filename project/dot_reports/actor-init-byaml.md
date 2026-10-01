# Actor initialization from BYAML

`dot/actor-init-byaml`, based on main
`57f902421f6874f132d5ea971d1aa5b224c5a528` fetched by the parent on
2026-10-01 at 20:58 UTC. Target: the unchanged unnamed EU interval
`0x002417E8..0x002428CC`, 4,324 bytes, originally U.

This is a complete guarded **NonMatching source proposal**, with no exact credit.
The retained source checkpoint is `1d8a971b64d8f18954f3bc1c4b65836bf1aa004f`.
Its configured `lib/al` compiler is ARMCC 4.1 build 791, verified from
`data/config.json`, not inferred from the address. The final `python make.py eu -ca` clean project build compiles,
links and exports; the object check and both accepted-wrapper checks were
repeated after that clean build. The unchanged canonical committed-source object checker says:

```
U -> M: The complete compiled section, including its literal pool, has a different size from the original interval.
```

The compiler emits one 4,232-byte function section, 92 bytes shorter than the
original complete interval. No source helper remains outside that root.
The checker accepts committed-input provenance and stops on size before linked
byte comparison. No exact or whole-game functional-equivalence claim follows.

## ABI and ownership

`fn_002417E8` is a neutral source identity, not a recovered original name.
The five-argument ABI is actor r0, ActorInitInfo reference r1, object-name
SafeString reference r2, archive-path SafeString reference r3, and optional
suffix at the incoming stack pointer. Established `al::initActor` calls it at
`0x0027B6E4`; established `al::initActorWithArchiveName` calls at `0x00280250`.
Other direct calls appear at `0x0025A9F8`, `0x00267FBC` and `0x002D57E8`.
The old `alActorInitUtil.cpp` had an empty static `initActorImpl` placeholder
with this same five-argument signature. Its two unaccepted wrappers now call
the reconstructed external root. The two accepted create-actor wrappers in
that translation unit remain canonical `O -> O`:

- `al::initCreateActorNoPlacementInfo`, `_ZN2al30initCreateActorNoPlacementInfoEPNS_9LiveActorERKNS_13ActorInitInfoE`
- `al::initCreateActorWithPlacementInfo`, `_ZN2al32initCreateActorWithPlacementInfoEPNS_9LiveActorERKNS_13ActorInitInfoE`

The resource is borrowed through the already established
`findOrCreateResource(archivePath)`. BYAML iterators keep the resource data base
and node pointers; returned strings are borrowed from that data. Locally
formatted filenames and the optional animation path live in ordinary stack
buffers. Model, pose, sensors, collision, effects, sound, switches and other
actor-owned state are created or changed by the observed callees. This root
performs no direct allocation and does not claim those callees' implementations.

`alActorInitByaml.h` declares the root; `alActorInitByaml.cpp` reconstructs the
body. Every neutral function import is an observed original call destination.
Imports already named in main use existing headers and symbols. The local
`String`/`Buffer` ABI views preserve the observed pointer/capacity/storage layout
and virtual termination call while shared SafeString class-layer intake is
pending. They do not define replacement shared vtables. Observed table addresses
are `0x003D7ABC`, `0x003D7AD0`, `0x003D7AF8` and `0x003D9C3C`.
The four pose entries at `0x003EF600` remain borrowed original data. Their
observed names/callback addresses are TRSV/`0x00266F34`, TFSV/`0x00270B74`,
TFGSV/`0x00244CC4` and TQSV/`0x00280590`; a missing match makes no pose callback.
No data definition or map identity is added for them.

## Complete control flow

The mapped pool hint `0x002419F8` points to executable code, specifically
`ldr r1,[sp,#0x98]`. The real return is `0x00242854`. The four executable spans
contain 940 instruction addresses:

- `0x002417E8..0x00241B1C`
- `0x00241BD0..0x00241F8C`
- `0x00242014..0x0024240C`
- `0x00242490..0x00242858`

The intervening spans and final `0x00242858..0x002428CC` are literal/string
islands. No boundary or pool marker was changed.

In order, the body reads `InitActor[_suffix]`; optionally chooses one of four
pose constructors; initializes placement SRT; sets up Model, InitLight,
MaterialController and Executor; selects a sensor file or legacy fallback and
constructs its valid entries; sets up Collision, Collider, Effect, Sound, Rail
and seven switch categories; registers view clipping; loads clipping parameters,
group clipping and shadow metadata; applies MaterialCode; then initializes
actor actions, fallback animation, nerve actions and the subactor keeper.

Two independently significant details were fixed by whole-root replay:
`init[_suffix]` contains legacy sensor entries under `Sensor`, and the actor
file contains fallback clipping under `Clipping`. The retail constants point
inside the strings `InitSensor` and `InitClipping`. Reading only whole pool
strings would incorrectly use the longer names. The initial source form did so;
its failing results are preserved in the private diagnostic build folder.
The second retained form fixes both keys.

The sensor path truncates MaxCount to 16 bits, and overrides it to zero for
sensor type 13 (`CollisionParts`). The clipping fallback computes maximum
absolute scale with the observed ordered selection behavior, multiplies by the
model's radius and uses a null explicit clipping-center pointer. NaNs, signed
zero and infinities retain the retail selection/result behavior in the bounded
floating tests described below.

Some imports are short mapped frontends that fall through into the next mapped
body: examples are `0x00243A54`, `0x001678B0` and `0x002519A8`. Their map extents
must not be mistaken for complete return boundaries. This proposal preserves
their call entry addresses and does not separately reconstruct those functions.

## Bounded behavioral validation

The canonical unchanged ARMCC object is separately linked at `0x00600000` for
execution comparison. Retail runs at its original address with the original
binary unchanged under Unicorn's ARM1176 CPU model. Both roots run through their
real returns. They agree on the ordered semantic call trace, full 4 KiB
actor/fixture-state region, input strings, stack-pointer restoration, r4-r11
and d8-d15 preservation.

- 600 fixtures: 100 seeds each across empty, complete, legacy fallback, sparse,
  wrong-type and missing-resource inputs
- 288 additional cases: 48 floating triples across six FPSCR modes, including
  positive/negative zero, subnormals, infinities, quiet/signaling NaN payloads,
  and shared object/archive SafeString record inputs
- Total: 888 whole-root original/candidate pairs, zero mismatches in the final
  fixture sets

The 600-case grid reaches all 940 executable instruction addresses in the
retail root. This is instruction coverage, not exhaustive branch combinations
or a proof of general equivalence. Cases include null/empty/ordinary/overlong
suffixes, filename truncation, direct and fallback resources, optional fields,
invalid types and null nodes, malformed sensor entries, absent/successful model
construction, action/nerve/subactor states, and the adjusted `IUseStageSwitch`
receiver at actor+0x0C. Caller-shaped object/archive strings share backing text;
the floating group also aliases the actual SafeString record itself.

Real versus modeled calls are explicit. The original BYAML constructors,
validation, key/index search, value conversions and vector decoder execute.
The original suffix formatter, StringTmp constructors, virtual termination
functions and libc formatting/copy routines execute. Original string equality,
sensor-name lookup, basename and sound-description readers execute. No BYAML,
string formatting, sensor-type lookup, or arithmetic interpreter is substituted.
The named `Vector3f::zero` data are initialized to zero for both executions.

The harness models only the resource retrieval/existence API and actor service
boundaries listed in its MODELED set: pose/model/service creation, registration,
light/shadow lookup and setup, sensors and joint lookup, collision/effect/sound,
rail/switch services, clipping, action and subactor setup, plus the actor's
getNerveKeeper virtual callback. Those models record typed arguments, return
controlled results and apply documented deterministic fixture writes. They do
not implement real allocation, archives, rendering, initialized engine state,
callbacks retaining stack references, reentrant mutation, failures other than
the represented returns, or general aliasing among actor/service structures.
The model-radius service returns a controlled float and getScale returns a
controlled vector. FPSCR exception-status flags and dead stack contents are not
compared. No gameplay, GPU or rendering claim follows.

An early harness revision emitted empty BYAML string tables, which the real
validator correctly rejected. It was repaired to emit a harmless unused entry
when a table would be empty. The final totals above use valid tables and reach
the formerly untested NearClipDistance path. Earlier test totals are superseded.

## Reproduction and limits

Run the normal project setup and `python make.py eu`. The parent authorized a
temporary symbol-only diagnostic name `fn_002417E8` on the existing unnamed map
row. With that name, run:

```
python tools/check.py fn_002417E8 --object build/eu/obj/lib/al/src/LiveActor/alActorInitByaml.o
```

Restore the map immediately afterward, including the checker's local rank write.
The proposal includes no map, tool, flags, ledger, STATE or game-data change.
The original executable remains SHA256
`e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.
No removed SDK, prohibited library, leaked source, assembly, instruction array
or invented helper address was used. Two distinct complete source forms were
compiled and canonically checked. The initial const-qualification compile error
produced no object and was corrected before the first successful form. There
were no cosmetic matching retries or compiler-flag changes.

The replay appendix below supplies the linker and harness source. Save those
blocks under `build/analysis/` using their labels and run them after the project
build. It requires the authorized local EU executable and installed Unicorn and
pyelftools. It contains synthetic fixtures and diagnostic code only.

## Frozen final artifacts

- Canonical object SHA256: `09585be323fffe602cd204f0a5164f41cda9c23c31aec8db84ba2ad8407b1757`
- Root source SHA256: `2feb430976aa4edbc6b264417bb06384bdeb63fdb5fcf25ac0b926b598833da6`
- Separate replay link SHA256: `a061a1f9d387c5635b470dde783e3258a8b95c2a5f06fb539df021181c708bc3`
- Final grid result: 600 passed, zero failed, zero uncovered retail instruction addresses
- Final floating/alias result: 288 passed, zero failed

The private diagnostic logs are `build/analysis/build-clean.log`,
`check-final.json`, `replay-results.json` and `replay-edge-results.json`.
They are reproducible from the committed source and the appendix; no generated
object, executable, game data or copied instruction listing is committed.

## link_actor_init.py

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
import csv,re,subprocess
p=Path('build/eu/obj/lib/al/src/LiveActor/alActorInitByaml.o')
e=ELFFile(p.open('rb'));sy=e.get_section_by_name('.symtab');rows=list(csv.reader(open('data/ver/eu/map.csv')))
by={r[6]:(int(r[0],16),r[5]) for r in rows if r[0].startswith('0x') and r[6]}
lines=['#<SYMDEFS>#']
for s in sy.iter_symbols():
 if s['st_shndx']=='SHN_UNDEF' and s.name:
  n=s.name
  if n in by:a,t=by[n]
  elif re.fullmatch('fn_[0-9A-Fa-f]{8}',n):a=int(n[3:],16);t='f'
  else:continue # ARM runtime requests and unused typeinfo are discarded.
  lines.append(f'0x{a:08X} '+('A' if 'f' in t else 'D')+' '+n)
Path('build/analysis/imports.sym').write_text('\n'.join(lines)+'\n')
subprocess.run(['data/compilers/wibo','data/compilers/4.1/791/bin/armlink.exe','--cpu=MPCore','--fpu=VFPv2','--arm_only','--no_exceptions','--no_debug','--no_scanlib','--mangled','--symbols','--map','--ro_base=0x600000','--entry=fn_002417E8','--keep=fn_002417E8','--output=build/analysis/actor_init.axf','--list=build/analysis/actor_init.map',str(p),'build/analysis/imports.sym'],check=True)
print('Function sections:',[(s.name,s['sh_size']) for s in e.iter_sections() if s['sh_flags']&4])
```

## replay_actor_init.py

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import *
from unicorn.arm_const import *
import struct,random,json,hashlib,sys
BINARY=Path('data/ver/eu/code.bin').read_bytes()
assert hashlib.sha256(BINARY).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
A=0x800000;INFO=A+0x100;OBJECT=A+0x200;ARCHIVE=A+0x220;RESOURCE=A+0x300;NERVE=A+0x400;MODEL=A+0x500;ACTION=A+0x600;SCALE=A+0x700;VT=A+0x800;STRINGS=A+0x1000;DATA=A+0x10000;SP=0x90f000;END=0x980000;GET_NERVE=END+4
REGS=[UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3,UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11,UC_ARM_REG_R12]
def pack(x):return struct.pack('<I',x&0xffffffff)
def rd(u,a):return struct.unpack('<I',u.mem_read(a,4))[0]
def wr(u,a,v):u.mem_write(a,pack(v))
def cs(u,a):
 if not a:return None
 out=bytearray()
 for i in range(2048):
  b=bytes(u.mem_read(a+i,1))
  if b==b'\0':return out.decode('ascii')
  out+=b
 raise AssertionError('unbounded string')
def ss(u,a):return cs(u,rd(u,a+4))
def fl(u,i):return u.reg_read(UC_ARM_REG_S0+i)
def vector(u,p):return list(struct.unpack('<3I',u.mem_read(p,12)))
def byaml(value):
 keys=set();strings=set()
 def collect(v):
  if isinstance(v,dict):
   for k,x in v.items():keys.add(k);collect(x)
  elif isinstance(v,list):
   for x in v:collect(x)
  elif isinstance(v,str):strings.add(v)
 collect(value);keys=sorted(keys or {"UnusedFixtureKey"});strings=sorted(strings or {"UnusedFixtureString"});out=bytearray(16)
 def align():out.extend(bytes((-len(out))%4))
 def table(items):
  align();start=len(out);out.extend(pack(0xc2|(len(items)<<8)));out.extend(bytes(4*(len(items)+1)))
  for i,s in enumerate(items):struct.pack_into('<I',out,start+4+i*4,len(out)-start);out.extend(s.encode()+b'\0')
  struct.pack_into('<I',out,start+4+len(items)*4,len(out)-start)
  return start
 ko=table(keys);so=table(strings)
 def node(v):
  if isinstance(v,dict):
   align();start=len(out);out.extend(pack(0xc1|(len(v)<<8)));out.extend(bytes(8*len(v)))
   for i,k in enumerate(sorted(v)):
    t,x=node(v[k]);struct.pack_into('<II',out,start+4+i*8,keys.index(k)|(t<<24),x)
   return 0xc1,start
  if isinstance(v,list):
   align();start=len(out);out.extend(pack(0xc0|(len(v)<<8)));types=len(out);out.extend(bytes(len(v)));align();values=len(out);out.extend(bytes(4*len(v)))
   for i,x in enumerate(v):t,y=node(x);out[types+i]=t;struct.pack_into('<I',out,values+i*4,y)
   return 0xc0,start
  if isinstance(v,str):return 0xa0,strings.index(v)
  if v is None:return 0xff,0
  if isinstance(v,bool):return 0xd0,int(v)
  if isinstance(v,float):return 0xd2,struct.unpack('<I',struct.pack('<f',v))[0]
  return 0xd1,v&0xffffffff
 _,root=node(value);struct.pack_into('<2sHIII',out,0,b'YB',1,ko,so,root);return bytes(out)

# Actor services are deliberately modeled boundaries. The code between calls,
# virtual dispatch, original string wrappers and complete BYAML readers execute.
MODELED={0x243260,0x290640,0x2690ec,0x270ab0,0x1e7f7c,0x243a54,0x260500,
0x1ca5e8,0x1ebe94,0x1e94e8,0x2627a4,0x1dcf34,0x26e64c,0x1c2cc8,0x1dea48,
0x272610,0x2519a8,0x24f8cc,0x1ebc64,0x1ebddc,0x26d9ec,0x27b650,0x27b638,
0x280538,0x270724,0x26fc64,0x27fcbc,0x2640fc,0x26eddc,0x2640a8,0x26f5a8,
0x28065c,0x27fd10,0x27e9dc,0x1678b0,0x1dbfa0,0x26f56c,0x26da54,0x26e4a4,
0x27063c,0x24fdb8,0x1e33f8,0x2742e0,0x266f34,0x270b74,0x244cc4,0x280590,
GET_NERVE,0x24edec}
SWITCHES={0x280538,0x270724,0x26fc64,0x27fcbc,0x2640fc,0x26eddc,0x2640a8}
POSES={0x266f34,0x270b74,0x244cc4,0x280590}
class Runner:
 def __init__(self,candidate):
  self.u=u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176)
  for a,n in [(0x100000,0x400000),(0x600000,0x20000),(A,0x40000),(0x900000,0x10000),(END,0x1000)]:u.mem_map(a,n)
  u.mem_write(0x100000,BINARY);self.entry=0x2417e8
  if candidate:
   with Path('build/analysis/actor_init.axf').open('rb') as f:
    e=ELFFile(f);self.entry=e['e_entry']
    for sec in e.iter_sections():
     if sec['sh_flags']&2 and sec['sh_size'] and sec['sh_type']=='SHT_PROGBITS':u.mem_write(sec['sh_addr'],sec.data())
  u.reg_write(UC_ARM_REG_C1_C0_2,0xf00000);u.reg_write(UC_ARM_REG_FPEXC,0x40000000)
  u.hook_add(UC_HOOK_CODE,self.hook)
  self.cover=set();self.calls=set()
 def hook(self,u,a,size,data):
  if self.entry<=a<(0x242858 if self.entry==0x2417e8 else self.entry+4232):self.cover.add(a)
  if a not in MODELED:return
  self.calls.add(a);r=[u.reg_read(x) for x in REGS[:4]];sp=u.reg_read(UC_ARM_REG_SP)
  st=lambda i:rd(u,sp+4*i)
  ev=[hex(a)]
  ret=0
  if a==GET_NERVE:ret=NERVE if self.f['nerve'] else 0;ev+=[r[0]-A]
  elif a==0x243260:ev+=[ss(u,r[0])];ret=RESOURCE
  elif a==0x290640:ev+=[r[0]-A,ss(u,r[1])];ret=self.files.get(ev[-1],0)
  elif a==0x2690ec:ev+=[r[0]-A,ss(u,r[1])];ret=int(ev[-1].removesuffix('.byml') in self.files)
  elif a in POSES:ev+=[r[0]-A];wr(u,A+0x14,A+0x900)
  elif a==0x270ab0:ev+=[r[0]-A,r[1]-A];wr(u,A+0x14,A+0x900)
  elif a==0x1e7f7c:
   ev+=[r[0]-A,r[1]-A,list(struct.unpack('<II',u.mem_read(r[2],8))),cs(u,r[3]),st(0),cs(u,st(1))]
   wr(u,A+0x28,MODEL if self.f['model_success'] else 0)
  elif a==0x243a54:ev+=[r[0]-A,cs(u,r[1]),cs(u,r[2])];ret=int(ev[-2] in self.f['model_files'])
  elif a==0x260500:ev+=[r[0]-A,cs(u,r[1]),cs(u,r[2])];ret=self.light
  elif a==0x1ca5e8:ev+=[r[0]-A,list(struct.unpack('<II',u.mem_read(r[1],8)))]
  elif a in {0x1ebe94,0x1e94e8}:ev+=[r[0]-A,fl(u,0),fl(u,1)]
  elif a==0x2627a4:ev+=[r[0]-A,r[1]-A,cs(u,r[2])]
  elif a==0x1dcf34:ev+=[r[0]-A,r[1]-A]
  elif a==0x26e64c:ev+=[r[0]-A,r[1]];wr(u,A+0x30,A+0xa00)
  elif a==0x1c2cc8:ev+=[r[0]-A,cs(u,r[1]),r[2],r[3],vector(u,st(0)),fl(u,0)];ret=A+0xa80
  elif a==0x1dea48:ev+=[r[0]-A,cs(u,r[1]),cs(u,r[2])]
  elif a==0x272610:ev+=[r[0]-A];ret=STRINGS+0x300
  elif a==0x24edec:ev+=[r[0]-A,cs(u,r[1])];ret=A+0xa80
  elif a==0x2519a8:ev+=[r[0]-A,cs(u,r[1])];ret=A+0xc00
  elif a==0x24f8cc:ev+=[r[0]-A,r[1]-A,ss(u,r[2]),r[3],st(0),cs(u,st(1))];wr(u,A+0x24,A+0xc80)
  elif a==0x1ebc64:ev+=[r[0]-A,r[1],fl(u,0),fl(u,1)];wr(u,A+0x20,A+0xd00)
  elif a==0x1ebddc:ev+=[r[0]-A,r[1]-A,cs(u,r[2])];wr(u,A+0x34,A+0xd80)
  elif a==0x26d9ec:ev+=[r[0]-A,ss(u,r[1]),r[2],r[3],st(0)];wr(u,A+0x38,A+0xe00)
  elif a==0x27b650:ev+=[r[0]-A];ret=int(self.f['rail'])
  elif a==0x27b638:ev+=[r[0]-A,r[1]-A];wr(u,A+0x40,A+0xe80)
  elif a in SWITCHES:ev+=[r[0]-A,r[1]-A];wr(u,A+0x3c,A+0xf00)
  elif a in {0x26f5a8}:ev+=[r[0]-A,r[1]-A]
  elif a==0x28065c:ev+=[r[0]-A];u.mem_write(A+0x57,b'\x01')
  elif a==0x27fd10:ev+=[r[0]-A,r[1],fl(u,0)]
  elif a==0x27e9dc:ev+=[r[0]-A];ret=SCALE
  elif a==0x1678b0:ev+=[r[0]-A];u.reg_write(UC_ARM_REG_S0,self.f['model_radius'])
  elif a==0x1dbfa0:ev+=[r[0]-A,fl(u,0)]
  elif a==0x26f56c:ev+=[r[0]-A,r[1]-A,r[2]]
  elif a==0x26da54:ev+=[r[0]-A,r[1]-A,cs(u,r[2])]
  elif a==0x26e4a4:
   ev+=[r[0]-A,cs(u,r[1]),cs(u,r[2]),cs(u,r[3])];wr(u,A+0x1c,ACTION if self.f['action'] else 0)
  elif a==0x27063c:ev+=[r[0]-A,cs(u,r[1])];ret=int(self.f['start_success'])
  elif a==0x24fdb8:ev+=[r[0]-A,cs(u,r[1])]
  elif a==0x1e33f8:ev+=[r[0]-A]
  elif a==0x2742e0:ev+=[r[0]-A,r[1]-A,cs(u,r[2]),cs(u,r[3])];wr(u,A+0x50,A+0xf80)
  else:raise AssertionError(hex(a))
  self.events.append(ev)
  u.reg_write(UC_ARM_REG_R0,ret)
  u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 def run(self,f,fpscr=0):
  u=self.u;self.f=f;self.events=[];self.files={}
  u.mem_write(A,bytes(0x40000));u.mem_write(0x900000,bytes(0x10000));wr(u,0x4305f8,0);wr(u,0x4305fc,0);wr(u,0x430600,0)
  pos=DATA
  for name,v in f['files'].items():
   blob=byaml(v);u.mem_write(pos,blob);self.files[name]=pos;pos=(pos+len(blob)+63)&~63
  self.light=pos;u.mem_write(pos,byaml({'Light':1}))
  wr(u,A,VT);wr(u,VT,GET_NERVE);wr(u,A+0x28,MODEL if f['model'] else 0);wr(u,A+0x50,MODEL if f['subactor'] else 0);wr(u,NERVE+0x14,ACTION if f['nerve_action'] else 0)
  u.mem_write(SCALE,struct.pack('<3I',*f['scale']))
  for at,t in [(STRINGS,f['object']),(STRINGS+0x100,f['archive']),(STRINGS+0x200,f['suffix'] or ''),(STRINGS+0x300,'ObjectData/ResourceBase')]:u.mem_write(at,t.encode()+b'\0')
  for at,p in [(OBJECT,STRINGS),(ARCHIVE,STRINGS+0x100)]:wr(u,at,0x3d9c3c);wr(u,at+4,p)
  if f['alias']:wr(u,ARCHIVE+4,STRINGS)
  for j,r in enumerate(REGS):u.reg_write(r,0xa0000000+j)
  for j in range(16):u.reg_write(UC_ARM_REG_D0+j,0x3ff0000000000000+j)
  u.reg_write(UC_ARM_REG_R0,A);u.reg_write(UC_ARM_REG_R1,INFO);u.reg_write(UC_ARM_REG_R2,OBJECT);u.reg_write(UC_ARM_REG_R3,OBJECT if f.get('object_alias') else ARCHIVE)
  u.reg_write(UC_ARM_REG_CPSR,0x10);u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,END);u.reg_write(UC_ARM_REG_FPSCR,fpscr);wr(u,SP,STRINGS+0x200 if f['suffix'] is not None else 0)
  u.emu_start(self.entry,END,count=2000000)
  assert u.reg_read(UC_ARM_REG_PC)==END,('cap',hex(u.reg_read(UC_ARM_REG_PC)))
  assert u.reg_read(UC_ARM_REG_SP)==SP
  assert all(u.reg_read(REGS[i])==0xa0000000+i for i in range(4,12))
  assert all(u.reg_read(UC_ARM_REG_D0+i)==0x3ff0000000000000+i for i in range(8,16))
  return self.events,bytes(u.mem_read(A,0x1000)),bytes(u.mem_read(STRINGS,0x400))

def fixture(seed,mode):
 r=random.Random(seed);suffix=[None,'','Alt','L'*150][seed%4];app='' if suffix is None else '_'+suffix
 def fmt(s):return (s+app)[:127]
 full={
 'Pose':['TRSV','TFSV','TFGSV','TQSV','Missing'][seed%5],
 'Model':{'AnimArc':'Animation','BlendAnimMax':3},
 'MaterialController':{'ProjTex1dRelTransY':{'Offset':12.5,'HeightScale':80.0}},
 'Executor':{'CategoryName':'MapObj'},'Collision':{'Name':'Rock','Sensor':'Body','Joint':'Root'},
 'Collider':{'Radius':50.0,'X':7.0,'Y':-12.0,'Z':10.0},'Effect':{'Name':'Spark'},
 'Sound':{'Name':'SoundName','MaxCount':7},'Rail':{},
 'Switch':{x:True for x in ['UseAppear','UseKill','UseDeadOn','UseReadA','UseWriteA','UseReadB','UseWriteB']},
 'GroupClipping':{'MaxCount':9},'Flag':{'MaterialCode':{}},'Clipping':{'Radius':99.0}}
 sensors=[{'Name':'Body','Type':'Body','Radius':23.0,'MaxCount':65537,'X':1.0,'Y':2.0,'Z':3.0,'Joint':'Root'},
 {'Name':'Foot','Type':'CollisionParts'},{'Name':'MissingType'},{'Type':'Body'},None,{}, {'Name':'N','Type':'Enemy','MaxCount':-1}]
 clipping={'Invalidate':True,'Radius':125.5,'NearClipDistance':10.0}
 if mode=='empty':init={};files={fmt('InitActor'):init}
 elif mode=='full':init=full;files={fmt('InitActor'):init,fmt('InitSensor'):sensors,fmt('InitClipping'):clipping}
 elif mode=='fallback':
  init=full.copy();init['MaterialController']={'ProjTex1dLocalTransY':{}};init['Executor']={};init['Collision']={};init['Model']={}
  files={fmt('InitActor'):init,fmt('init'):{'Sensor':sensors}}
 elif mode=='sparse':
  init={k:v for k,v in full.items() if r.randrange(2)}
  if 'MaterialController' in init:init['MaterialController']={r.choice(['ProjTex1dRelTransY','ProjTex1dLocalTransY']):{}}
  if 'Model' in init:init['Model']={} if r.randrange(2) else init['Model']
  files={fmt('InitActor'):init}
  if r.randrange(2):files[fmt('InitSensor')]=r.choice([sensors,[],{},None])
  if r.randrange(2):files[fmt('init')]={'Sensor':r.choice([sensors,[],{},None])}
  if r.randrange(2):files[fmt('InitClipping')]={k:v for k,v in clipping.items() if r.randrange(2)}
 elif mode=='wrong_types':
  init={k:r.choice([None,False,1,3.0,'Wrong',[],{}]) for k in full};files={fmt('InitActor'):init,fmt('InitSensor'):sensors}
 elif mode=='missing':files={}
 else:raise AssertionError(mode)
 return dict(files=files,model=seed%2==0,model_success=seed%3!=0,model_files=['InitLight','InitLight'+app,'InitShadow','InitShadow'+app] if seed%3 else ['InitLight','InitShadow'],rail=seed%2,nerve=seed%3!=0,nerve_action=seed%2,action=seed%3!=0,start_success=seed%2,subactor=seed%3==0,object='ObjectData/ActorName',archive='ObjectData/ArchiveName',suffix=suffix,alias=seed%2==0,scale=[struct.unpack('<I',struct.pack('<f',x))[0] for x in r.choices([-3.0,-0.0,0.0,0.25,1.0,2.0],k=3)],model_radius=0x42c80000)

def main():
 orig=Runner(False);cand=Runner(True);results=[];fails=[]
 for seed in range(int(sys.argv[1]) if len(sys.argv)>1 else 20):
  for mode in ['empty','full','fallback','sparse','wrong_types','missing']:
   f=fixture(seed,mode)
   try:
    a=orig.run(f);b=cand.run(f)
    if a!=b:
     i=next((i for i,(x,y) in enumerate(zip(a[0],b[0])) if x!=y),min(len(a[0]),len(b[0])))
     fails.append({'seed':seed,'mode':mode,'event':i,'original':a[0][max(0,i-1):i+2],'candidate':b[0][max(0,i-1):i+2],'actor_equal':a[1]==b[1],'strings_equal':a[2]==b[2]})
    else:results.append((seed,mode))
   except Exception as e:fails.append({'seed':seed,'mode':mode,'error':str(e),'orig_pc':hex(orig.u.reg_read(UC_ARM_REG_PC)),'cand_pc':hex(cand.u.reg_read(UC_ARM_REG_PC))})
   if fails and len(fails)>=10:break
  if len(fails)>=10:break
 expected={a for lo,hi in [(0x2417e8,0x241b1c),(0x241bd0,0x241f8c),(0x242014,0x24240c),(0x242490,0x242858)] for a in range(lo,hi,4)}
 report=dict(uncovered=[hex(x) for x in sorted(expected-orig.cover)],passed=len(results),failed=len(fails),failures=fails,original_covered_instructions=len(orig.cover),candidate_covered_instructions=len(cand.cover),modeled_calls=[hex(x) for x in sorted(orig.calls)])
 Path('build/analysis/replay-results.json').write_text(json.dumps(report,indent=2));print(json.dumps(report,indent=2))
if __name__=='__main__':main()
```

## replay_actor_edges.py

```python
from replay_actor_init import *
import itertools
orig=Runner(False);cand=Runner(True);fails=[];passed=0
values=[0,0x80000000,1,0x80000001,0x3f800000,0xbf800000,0x7f800000,0xff800000,0x7fc00001,0xffc12345,0x7f812345,0xff812345]
triples=[(x,0x3f800000,0xbf800000) for x in values]+[(0x3f800000,x,0xbf800000) for x in values]+[(0x3f800000,0xbf800000,x) for x in values]+[(x,x,x) for x in values]
for i,scale in enumerate(triples):
 for fpscr in [0,0x400000,0x800000,0xc00000,0x1000000,0x3000000]:
  f=fixture(i,'empty');f.update(scale=scale,model=True,object_alias=i%2==0)
  try:
   a=orig.run(f,fpscr);b=cand.run(f,fpscr)
   if a==b:passed+=1
   else:
    j=next((j for j,(x,y) in enumerate(zip(a[0],b[0])) if x!=y),min(len(a[0]),len(b[0])))
    fails.append(dict(scale=[hex(x) for x in scale],fpscr=hex(fpscr),event=j,original=a[0][max(0,j-1):j+2],candidate=b[0][max(0,j-1):j+2]))
  except Exception as e:fails.append(dict(scale=scale,fpscr=fpscr,error=str(e)))
report=dict(passed=passed,failed=len(fails),failures=fails)
Path('build/analysis/replay-edge-results.json').write_text(json.dumps(report,indent=2));print(json.dumps(report,indent=2))
```
