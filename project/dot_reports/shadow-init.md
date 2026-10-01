# Shadow initialization from BYAML

Branch `dot/shadow-init`, based on main `57f902421f6874f132d5ea971d1aa5b224c5a528`, fetched by the parent on 2026-10-01 at 21:26 UTC. The reserved target is the unchanged unnamed U root `0x001C1064..0x001C1EE8`, 3,716 complete bytes. Reservation commit: `6b71e364558f61fff057c2d7a6fe94b257e59308`.

This is a complete guarded **NonMatching source proposal**, with **zero exact credit**. Source checkpoint: `5049fc68b595aad19e52433b4a7805d6e7372fda`. Two distinct C++ source forms were attempted. No assembly, function-byte stand-in, object editing, invented helper address, map extent change, or prohibited library was used.

The configured `lib/al` compiler is ARMCC 4.1 build 791. `python make.py eu -ca` compiles, links and exports from committed source. The unchanged canonical checker accepts the final build provenance and reports:

```
U -> M: The complete compiled section, including its literal pool, has a different size from the original interval.
```

Its single function section is **3,468 bytes**, 248 bytes shorter than the original complete interval. All source helpers inline into that section. There is no other executable source section. The check stops at size before linked-byte comparison. The first incremental check had rejected stale uncommitted-build provenance; a full clean build corrected that evidence. The first source form (`2eea1e3`) used a switch and emitted 3,456 bytes with a jump table. The retained second form uses the independently observed type 1/2 test and ordered comparisons; ARMCC now reproduces that dispatch structure, but the remaining register, spill and inlined string layout still differs. No accepted rank changed. Temporary symbol-only names on ten existing unnamed function rows are restored byte-for-byte after the diagnostic.

## Files and ABI

`lib/al/include/Shadow/alShadowInitByaml.h` declares the neutral root. `lib/al/src/Shadow/alShadowInitByaml.cpp` contains ordinary C++, guarded by `NON_MATCHING`. The original API name is not established. Local structs describe observed fields and do not replace shared library class layers.

The sole direct ARM BL caller is `0x0026DAD8`, inside `0x0026DA54..0x0026DAE8`. It loads a resource BYAML, allocates 0x14 bytes, calls the constructor at `0x001C2184`, passes keeper r0, LiveActor r1, ActorInitInfo reference r2 and BYAML iterator reference r3, then stores the keeper at actor+0x44. It ignores this root's boolean result. The constructor zeroes four words and sets the byte at +0x10 to one.

The keeper layout is pointer +0, capacity +4, ring head +8, count +0xC and flag byte +0x10. Its independently observed append helper at `0x001C2144` increments count only below capacity, wraps once, and stores one pointer. This root has the same inlined operation. Index access returns element zero when the unsigned index is outside the unsigned count; otherwise it adds head and wraps once. This fallback is preserved, even though coherent loop bounds normally avoid it.

Each returned shadow has a virtual table at +0, borrowed name at +0x70, borrowed vanishing-point name at +0x74 and resolved shadow pointer at +0x78. The root sets +0x74, calls virtual slot +0x60 with ActorInitInfo and execution category, and appends the object. The created object's full class name and remaining fields stay unknown.

Five local fixed strings occupy 0x8C bytes each: table, text pointer, capacity, and 128 bytes of storage. Observed construction tables are `0x003DA510`, `0x003DA224`, and final `0x003D9CBC`. The shared empty-string object at `0x003F361C` is borrowed; no data definition is added. Each initializer preserves the virtual termination call, bounded 65,536-character scans, 127-character clamp, copy and terminator behavior. The actual terminator at `0x0039E0E4` sets text[capacity-1] to zero. These local ABI views deliberately avoid asserting a recovered original class hierarchy.

## Full control flow and ownership

There are 864 recursively reachable original ARM instruction addresses, in these spans:

- `0x001C1064..0x001C13B4`
- `0x001C13F0..0x001C1B38`
- `0x001C1C00..0x001C1EE8`

The two intervening literal/string islands total 260 bytes. No interval or pool marker was changed.

Presence of `DontUseShadow` returns true immediately, independent of its value. Missing or invalid `Shadows` requests four aligned pointer slots, installs them only on successful allocation, and returns false. A valid array/hash gets its size, requests exactly that many pointer slots when positive, then processes each index. Allocation failure retains the old keeper fields and existing ring. A valid empty collection skips allocation but still resolves existing references. Pointer allocations use observed `_ZnajPN4sead4HeapEi` with a null heap and alignment four. The root does not delete discarded old storage or roll back allocated shadows that cannot fit; it preserves the observed ownership behavior.

Every entry initializes type zero, ten float zeros, null name/joint/vanishing/model pointers, five fixed strings, and two false flags. It reads execution category, Name, ActorJointName, VanishingPointShadowName and ModelArcName. The default category is the original Shift-JIS text 影ボリューム, expressed as a normal C++ string literal. `UseActorTransRef` is a presence test. TypeName, when successfully read, searches ten borrowed names from `0x003EF648`; an unknown name becomes type zero. Only failure to read TypeName falls back to integer Type.

The ten observed names, decoded only for this report, correspond to none, rectangular planar shadow, circular planar shadow, cone, square pyramid, sphere, cylinder, rectangular prism, block, and arbitrary model. Types 0, 1, 2 and out-of-range integers create nothing. The seven creator imports are:

| Type | Original destination |
| --- | --- |
| 3 | `0x001E5BE4` |
| 4 | `0x001E7804` |
| 5 | `0x001E710C` |
| 6 | `0x001E7BCC` |
| 7 | `0x001E5C94` |
| 8 | `0x001E6254` |
| 9 | `0x001E6304` |

The entry reads X/Y/Z components from Offset, RotateOffset and Size, then ShadowOffset. Failed fields retain zero. Every entry obtains a borrowed actor scale pointer through `0x0024F1CC`, which dispatches actor+0x14 virtual slot +0x1C. With UseActorTransRef it gets the actor translation. Otherwise a nonnull ActorJointName uses `0x002519A8`, a short frontend that falls through to the model joint lookup. Without a joint name it tests LiveActor virtual slot +0x38 and calls it a second time if the first result was nonnull; the second result is used even if now null. A first null result falls back to translation. The source preserves this two-call behavior.

Types 3–6 and 8 pass the Size vector, matrix, translation, scale, Offset, RotateOffset and ShadowOffset. Type 7 passes scalar sizes in X, Z, Y order in s0/s1/s2 and ShadowOffset in s3. Type 9 additionally passes ModelArcName. Independent disassembly of all seven creator wrappers verifies those register and stack contracts and their immediate copying of local vectors into an intermediate parameter record. All eventually call `0x002408B8`; the root does not reconstruct the allocator/creator internals.

After creation, the root resolves every nonnull vanishing-point name to the first shadow with an equal name. Matches may be self-references or shared objects; missing names store null. A null requested name leaves the prior +0x78 value alone. Both the outer count and each inner search count use the observed snapshots, while indexed access reloads current ring metadata.

## Bounded ARM replay

The original complete root and a separate link of the unchanged project-built object agree on **1,305 returning pairs**, plus **15 separately reported negative fault pairs**, across **264 fixtures and five FPSCR modes**. The final replay link is recreated from the clean-built canonical object. Register values and pointers are normalized only where the source's private stack layout is necessarily different; event arguments include actual borrowed pointer identities and raw float words.

The returning cases compare return value, ordered boundary calls, all creator scalar/vector arguments, virtual initialization category and vanishing name, all keeper bytes including padding, ring storage, and all controlled shadow bytes. Every returning run verifies SP, r4–r11 and d8–d15 preservation. Original BYAML constructors/readers, isEqualString, memcpy, fixed-string termination, branches, field accesses and ring/reference logic execute as ARM instructions. All nine BYAML entry points used by the root execute; none is replaced by a value-level mock.

Cases cover missing/invalid/empty metadata, malformed entries and fields, all numeric and named types, presence-only keys with false/null values, first/second matrix outcomes, joint lookup returning null, every creator returning null, allocation failure, all five wrapped ring heads, duplicate/self/missing names, repeated creator object identities, nonempty/clamped/65,536-byte source strings, and float words for signed zeros, infinities, NaNs and subnormals. FPSCR values are `0`, `0x01000000`, `0x02000000`, `0x03000000`, and `0x00400000`.

Replay reaches **839/864** statically reachable original instruction addresses. The remaining 25 are the five destination-length scans' nonempty/overflow arms. The destination begins with zero and the observed termination routine only writes byte 127, so these arms are not reached under the verified local-string contract. This observation is not a claim of whole-program path completeness.

The 15 fault pairs deliberately let the controlled virtual initializer corrupt count while leaving null ring slots. Both versions then fault on the same invalid read with equal prior call traces. These are negative controls, not successful behavior or allocation-failure equivalence claims.

### Explicit limits

Allocation, seven shadow creators, scale/joint/translation queries and the actor/created-shadow virtual callbacks are controlled boundaries. Their full game implementations, resources, graphics state, scheduling and destruction are not replayed. The fixtures initialize the runtime empty-string object to the observed SafeString table and a controlled source string; they do not replay global startup. Actual original type-name strings and tables remain private binary inputs.

Tested aliases include shared BYAML string values, self-referencing names, duplicate names and creator returns sharing the same shadow object. No claim covers arbitrary keeper/actor/BYAML/stack overlap, concurrent mutation, invalid pointers beyond the labelled negative controls, integer-overflow ring states, malicious vtables, or unbounded allocation sizes. The caller's allocation-null dereference behavior is outside this root's returning fixtures. These bounds are a useful NonMatching result, not an exact match or a complete game-equivalence proof.

Exact work remains blocked by the unresolved production string class layers and the different ARMCC register/spill/control-flow layout. Expanding cosmetic forms without new ABI evidence is not justified. This retained complete source and replay can be integrated or revisited independently of accepted counts.

## Frozen fingerprints

- Source: `10ad90a32ff38d12d016213c6918063cc85340d97cc9039b61416be84b5d49f3`
- Header: `335500e82659049bdea748163d3747b35a1e4c5433d32eddf91beff5ff7d7a56`
- Canonical object: `01fcc72c41032412926ac51b98959c1f0eb673f204f823fa16f157c04de8b266`
- Separate replay link: `3c7fd0b06030f0b2f182a5961d7d35db009133cbb70600ce23d11ca66e45aa45`
- Restored map: `ac7eef756278c86c41472bab66291afed57b6bcbed2336eb2b75aae157ff7cca`
- Original EU executable: `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`

No game data or generated diagnostic artifact is committed. Private local evidence is under `build/analysis/`: `build-clean-final.log`, `check-final-pass.log`, `link-final-pass.log`, `replay-final-pass.log` and `replay-results.json`.

## Repository-relative replay appendix

Use this branch with the authorized EU code/exheader and approved ARMCC/wibo in their ordinary ignored repository paths. Python needs the project's normal dependencies plus Unicorn. Save the three code blocks below into the named files under `build/analysis/`. They contain no binary payload. From the repository root:

```sh
mkdir -p build/analysis
. ./development_environment.sh
# Linux only, if GNU ARM binutils are installed under /usr:
export DEVKITARM=/usr
python make.py eu -ca
python build/analysis/check_shadow_init.py
python build/analysis/link_shadow_init.py
python build/analysis/replay_shadow_init.py
```

The checker wrapper expects the current documented size mismatch, uses the canonical project checker without modification, and always restores map bytes. The replay linker imports only observed original mapped function identities or neutral names whose hexadecimal suffix is the verified call address. It links the original project-built object at a separate address; it never assembles or edits its bytes.

### check_shadow_init.py

```python
from pathlib import Path
import subprocess,hashlib
path=Path('data/ver/eu/map.csv');original=path.read_bytes()
addresses=[0x1c1064,0x1e5be4,0x1e5c94,0x1e6254,0x1e6304,0x1e710c,0x1e7804,0x1e7bcc,0x24f1cc,0x2519a8]
names={a:f'fn_{a:08X}' for a in addresses};seen=set();lines=original.decode().splitlines(keepends=True)
for i,line in enumerate(lines):
 if not line.startswith('0x'):continue
 cells=line.split(',');address=int(cells[0],16)
 if address in names:
  assert not cells[6].strip(),(hex(address),cells[6])
  cells[6]=names[address];lines[i]=','.join(cells);seen.add(address)
assert seen==set(names)
try:
 path.write_text(''.join(lines))
 result=subprocess.run(['python','tools/check.py','fn_001C1064','--object','build/eu/obj/lib/al/src/Shadow/alShadowInitByaml.o'],capture_output=True,text=True)
 Path('build/analysis/check-final.log').write_text(result.stdout+result.stderr)
 print(result.stdout+result.stderr)
 assert result.returncode==1
 assert 'different size from the original interval' in result.stdout
finally:path.write_bytes(original)
assert path.read_bytes()==original
print('Restored map SHA256',hashlib.sha256(original).hexdigest())
```

### link_shadow_init.py

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
import csv,re,subprocess
p=Path('build/eu/obj/lib/al/src/Shadow/alShadowInitByaml.o')
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
subprocess.run(['data/compilers/wibo','data/compilers/4.1/791/bin/armlink.exe','--cpu=MPCore','--fpu=VFPv2','--arm_only','--no_exceptions','--no_debug','--no_scanlib','--mangled','--symbols','--map','--ro_base=0x600000','--entry=fn_001C1064','--keep=fn_001C1064','--output=build/analysis/shadow_init.axf','--list=build/analysis/shadow_init.map',str(p),'build/analysis/imports.sym'],check=True)
print('Function sections:',[(s.name,s['sh_size']) for s in e.iter_sections() if s['sh_flags']&4])
```

### replay_shadow_init.py

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import *
from unicorn.arm_const import *
import struct,random,json,hashlib
BINARY=Path('data/ver/eu/code.bin').read_bytes()
assert hashlib.sha256(BINARY).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
K=0x800000;A=K+0x100;INFO=K+0x200;IT=K+0x300;SCALE=K+0x400;MATRIX=K+0x500;TRANS=K+0x600;AVT=K+0x700;SVT=K+0x800;HEAP=K+0x1000;OLD=K+0x2000;OBJECTS=K+0x3000;DATA=K+0x10000;EMPTY=K+0x30000;SP=0x90f000;END=0x980000;BASE=END+4;INITIALIZE=END+8
REGS=[UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3,UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11,UC_ARM_REG_R12]
CREATORS={0x1e5be4:3,0x1e7804:4,0x1e710c:5,0x1e7bcc:6,0x1e5c94:7,0x1e6254:8,0x1e6304:9}
MODELED={0x2933d0,0x24f1cc,0x2519a8,0x27f3c4,BASE,INITIALIZE}|set(CREATORS)
def pack(x):return struct.pack('<I',x&0xffffffff)
def rd(u,a):return struct.unpack('<I',u.mem_read(a,4))[0]
def wr(u,a,v):u.mem_write(a,pack(v))
def cs(u,a):
 if not a:return None
 out=bytearray()
 for i in range(0x10002):
  v=bytes(u.mem_read(a+i,1))
  if v==b'\0':return out.decode('latin1')
  out+=v
 raise AssertionError('unbounded string')
def vector(u,p):return list(struct.unpack('<3I',u.mem_read(p,12)))
def byaml(value):
 keys=set();strings=set()
 def collect(v):
  if isinstance(v,dict):
   for k,x in v.items():keys.add(k);collect(x)
  elif isinstance(v,list):
   for x in v:collect(x)
  elif isinstance(v,str):strings.add(v)
 collect(value);keys=sorted(keys or {'Unused'});strings=sorted(strings or {'Unused'});out=bytearray(16)
 def align():out.extend(bytes((-len(out))%4))
 def table(items):
  align();start=len(out);out.extend(pack(0xc2|(len(items)<<8)));out.extend(bytes(4*(len(items)+1)))
  for i,s in enumerate(items):struct.pack_into('<I',out,start+4+i*4,len(out)-start);out.extend(s.encode('latin1')+b'\0')
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
  if isinstance(v,tuple) and v[0]=='bits':return 0xd2,v[1]
  if isinstance(v,float):return 0xd2,struct.unpack('<I',struct.pack('<f',v))[0]
  return 0xd1,v&0xffffffff
 _,root=node(value);struct.pack_into('<2sHIII',out,0,b'YB',1,ko,so,root);return bytes(out)
class Runner:
 def __init__(self,candidate):
  self.u=u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176)
  for a,n in [(0x100000,0x400000),(0x600000,0x20000),(K,0x50000),(0x900000,0x10000),(END,0x1000)]:u.mem_map(a,n)
  u.mem_write(0x100000,BINARY);self.entry=0x1c1064;self.end=0x1c1ee8
  if candidate:
   with Path('build/analysis/shadow_init.axf').open('rb') as f:
    e=ELFFile(f);self.entry=e['e_entry']
    for sec in e.iter_sections():
     if sec['sh_flags']&2 and sec['sh_size'] and sec['sh_type']=='SHT_PROGBITS':u.mem_write(sec['sh_addr'],sec.data())
    obj=ELFFile(Path('build/eu/obj/lib/al/src/Shadow/alShadowInitByaml.o').open('rb'));self.end=self.entry+obj.get_section_by_name('i.fn_001C1064')['sh_size']
  u.reg_write(UC_ARM_REG_C1_C0_2,0xf00000);u.reg_write(UC_ARM_REG_FPEXC,0x40000000)
  u.hook_add(UC_HOOK_CODE,self.hook);u.hook_add(UC_HOOK_MEM_INVALID,self.fault_hook);self.cover=set();self.calls=set();self.byaml_calls=set()
 def fault_hook(self,u,access,address,size,value,data):
  self.fault=[access,address,size];return False
 def hook(self,u,a,size,data):
  if self.entry<=a<self.end:self.cover.add(a)
  if a in {0x271210,0x276aa0,0x278c30,0x27e068,0x290fb0,0x29101c,0x29107c,0x2910e8,0x2910f8}:self.byaml_calls.add(a)
  if a not in MODELED:return
  self.calls.add(a);r=[u.reg_read(x) for x in REGS[:4]];sp=u.reg_read(UC_ARM_REG_SP);st=lambda i:rd(u,sp+i*4)
  ev=[hex(a)];ret=0
  if a==0x2933d0:
   ev+=r[:3];ret=0 if self.f.get('alloc_fail') else HEAP
  elif a==0x24f1cc:ev+=[r[0]];ret=SCALE
  elif a==0x2519a8:ev+=[r[0],cs(u,r[1])];ret=0 if self.f.get('joint_null') else MATRIX
  elif a==0x27f3c4:ev+=[r[0]];ret=TRANS
  elif a==BASE:
   ev+=[r[0]];ret=self.f.get('matrix',[MATRIX,MATRIX])[self.base_calls%2];self.base_calls+=1
  elif a in CREATORS:
   t=CREATORS[a];ev+=[cs(u,r[0]),r[1]]
   if t==7:
    floats=[u.reg_read(UC_ARM_REG_S0+i) for i in range(4)]
    ev += [floats[:3],r[2],r[3],st(0),vector(u,st(1)),vector(u,st(2)),floats[3]]
   elif t==9:
    ev += [cs(u,r[2]),vector(u,r[3]),st(0),st(1),st(2),vector(u,st(3)),vector(u,st(4)),u.reg_read(UC_ARM_REG_S0)]
   else:ev += [vector(u,r[2]),r[3],st(0),st(1),vector(u,st(2)),vector(u,st(3)),u.reg_read(UC_ARM_REG_S0)]
   index=self.created;self.created+=1
   if index not in self.f.get('creator_fail',[]):
    ret=OBJECTS+self.f.get('creator_alias',{}).get(index,index)*0x100;wr(u,ret,SVT);wr(u,ret+0x70,r[0]);wr(u,ret+0x78,0xdead0000+index)
    if ret not in self.objects:self.objects.append(ret)
   self.base_calls=0
  elif a==INITIALIZE:
   ev += [r[0],r[1],cs(u,r[2]),cs(u,rd(u,r[0]+0x74))]
   # Controlled interference after creation exercises reloaded ring metadata.
   if 'callback_count' in self.f:wr(u,K+12,self.f['callback_count'])
  self.events.append(ev);u.reg_write(UC_ARM_REG_R0,ret);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 def run(self,f,fpscr=0):
  u=self.u;self.f=f;self.events=[];self.created=0;self.objects=[];self.base_calls=0;self.fault=None
  u.mem_write(K,bytes(0x50000));u.mem_write(0x900000,bytes(0x10000))
  blob=byaml(f['data']);u.mem_write(DATA,blob);wr(u,IT,DATA);wr(u,IT+4,DATA+struct.unpack_from('<I',blob,12)[0])
  if f.get('invalid_input'):wr(u,IT+4,0)
  wr(u,A,AVT);wr(u,AVT+0x38,BASE);wr(u,SVT+0x60,INITIALIZE)
  wr(u,0x3f361c,0x3d9c3c);wr(u,0x3f3620,EMPTY);u.mem_write(EMPTY,f.get('empty','').encode('latin1')+b'\0')
  capacity=f.get('capacity',0);head=f.get('head',0);names=f.get('old',[])
  wr(u,K,OLD if capacity else 0);wr(u,K+4,capacity);wr(u,K+8,head);wr(u,K+12,len(names));u.mem_write(K+16,b'\x01\xaa\xbb\xcc')
  for i,(name,target) in enumerate(names):
   obj=OBJECTS+0x8000+i*0x100;self.objects.append(obj);wr(u,obj,SVT)
   text=K+0xc000+i*0x200;u.mem_write(text,name.encode()+b'\0');wr(u,obj+0x70,text)
   if target is not None:u.mem_write(text+0x100,target.encode()+b'\0');wr(u,obj+0x74,text+0x100)
   wr(u,obj+0x78,0xdead0000+i);wr(u,OLD+((head+i)%capacity)*4,obj)
  for j,r in enumerate(REGS):u.reg_write(r,0xa0000000+j)
  for j in range(16):u.reg_write(UC_ARM_REG_D0+j,0x3ff0000000000000+j)
  for r,v in zip(REGS,[K,A,INFO,IT]):u.reg_write(r,v)
  u.reg_write(UC_ARM_REG_CPSR,0x10);u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,END);u.reg_write(UC_ARM_REG_FPSCR,fpscr)
  try:u.emu_start(self.entry,END,count=6000000)
  except UcError as e:
   if self.fault and f.get('expect_fault'):return {'fault':self.fault,'events':self.events}
   raise AssertionError(('fault',hex(u.reg_read(UC_ARM_REG_PC)),f)) from e
  assert u.reg_read(UC_ARM_REG_PC)==END,('cap',hex(u.reg_read(UC_ARM_REG_PC)))
  assert u.reg_read(UC_ARM_REG_SP)==SP
  assert all(u.reg_read(REGS[i])==0xa0000000+i for i in range(4,12))
  assert all(u.reg_read(UC_ARM_REG_D0+i)==0x3ff0000000000000+i for i in range(8,16))
  return {'return':u.reg_read(UC_ARM_REG_R0),'events':self.events,'keeper':bytes(u.mem_read(K,20)).hex(),'buffers':bytes(u.mem_read(HEAP,0x2000)).hex(),'objects':[(p,bytes(u.mem_read(p,0x100)).hex()) for p in self.objects]}
def fixtures():
 out=[]
 for value in [{},{'DontUseShadow':False},{'Shadows':None},{'Shadows':4},{'Shadows':[]},{'Shadows':{}},{'Shadows':[{},4,None,True,'bad'] }]:out.append({'data':value})
 names=[]
 for o in range(10):
  p=struct.unpack_from('<I',BINARY,0x3ef648-0x100000+4*o)[0];names.append(BINARY[p-0x100000:].split(b'\0')[0].decode('latin1'))
 for t in range(-2,12):
  for mode in range(6):
   p={'Type':t,'Name':'shape','Offset':{'X':-1.25,'Y':2.5,'Z':-0.0},'RotateOffset':{'X':3.75,'Y':-5.0,'Z':6.25},'Size':{'X':7.5,'Y':8.75,'Z':10.0},'ShadowOffset':-11.25,'ModelArcName':'Arc'}
   if mode==1:p['UseActorTransRef']=False
   if mode==2:p['ActorJointName']='Joint'
   if mode==3:p['TypeName']=names[t] if 0<=t<10 else 'unknown'
   if mode==4:p['TypeName']=42;p['Size']={'X':1,'Y':None,'Z':True}
   out.append({'data':{'Shadows':[p]},'matrix':[0,0] if mode==5 else [MATRIX,MATRIX]})
 for alias in [{1:0},{2:0},{1:0,2:0}]:
  out.append({'data':{'Shadows':[{'Type':3,'Name':'a','VanishingPointShadowName':'b'},{'Type':4,'Name':'b','VanishingPointShadowName':'a'},{'Type':7,'Name':'a','VanishingPointShadowName':'a'}]},'creator_alias':alias})
 for seed in range(100):
  r=random.Random(seed);items=[]
  for i in range(r.randrange(1,12)):
   p={'Type':r.randrange(-3,13),'Name':r.choice(['a','b','a',''])}
   if r.randrange(2):p['VanishingPointShadowName']=r.choice(['a','b','missing',''])
   if r.randrange(3)==0:p['ExecCategory']='category'
   if r.randrange(3)==0:p['ActorJointName']='joint'
   if r.randrange(4)==0:p['UseActorTransRef']=None
   items.append(p)
  out.append({'data':{'Shadows':items},'creator_fail':[r.randrange(len(items))], 'joint_null':bool(seed%2)})
 for head in range(5):
  for fail in [False,True]:
   for data in [{},{'Shadows':[]},{'Shadows':[{'Type':3,'Name':'new','VanishingPointShadowName':'a'}]}]:
    out.append({'data':data,'capacity':5,'head':head,'old':[('a','b'),('b','a'),('a','absent')],'alloc_fail':fail})
 out.append({'data':{'Shadows':[{'Type':3}]},'invalid_input':True})
 for text in ['', 'x', 'x'*127, 'x'*128, 'x'*512, 'x'*0x10000]:out.append({'data':{'Shadows':[{'Type':3}]},'empty':text})
 for bits in [0,0x80000000,0x7f800000,0xff800000,0x7fc12345,0x7f812345,1,0x807fffff,0x3f800001]:
  for t in [3,7,9]:
   v=('bits',bits);out.append({'data':{'Shadows':[{'Type':t,'Offset':{'X':v,'Y':v,'Z':v},'RotateOffset':{'X':v,'Y':v,'Z':v},'Size':{'X':v,'Y':v,'Z':v},'ShadowOffset':v}]}})
 for matrix in [[MATRIX,0],[0,MATRIX]]:out.append({'data':{'Shadows':[{'Type':3}]},'matrix':matrix})
 for count in [0,1,2,3]:out.append({'data':{'Shadows':[{'Type':3},{'Type':4}]},'callback_count':count,'expect_fault':count!=0})
 return out
if __name__=='__main__':
 a,b=Runner(False),Runner(True);fs=fixtures()
 pairs=0;faults=0
 for fpscr in [0,0x1000000,0x2000000,0x3000000,0x400000]:
  for i,f in enumerate(fs):
   x,y=a.run(f,fpscr),b.run(f,fpscr);pairs+=1;faults+=int('fault' in x)
   if x!=y:
    Path('build/analysis/replay-failure.json').write_text(json.dumps({'index':i,'fpscr':fpscr,'fixture':f,'original':x,'source':y},indent=2));raise AssertionError(('mismatch',i,f))
 report={'fixtures':len(fs),'pairs':pairs,'returning_pairs':pairs-faults,'fault_pairs':faults,'original_covered':len(a.cover),'source_covered':len(b.cover),'calls':[hex(x) for x in sorted(a.calls)],'byaml_calls':[hex(x) for x in sorted(a.byaml_calls)]}
 from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM,CS_GRP_JUMP
 md=Cs(CS_ARCH_ARM,CS_MODE_ARM);md.detail=True;pending=[0x1c1064];reachable=set()
 while pending:
  pc=pending.pop()
  if pc in reachable or not 0x1c1064<=pc<0x1c1ee8:continue
  ins=list(md.disasm(BINARY[pc-0x100000:pc-0x100000+4],pc))[0];reachable.add(pc)
  if ins.group(CS_GRP_JUMP) and ins.operands[0].type==2:pending.append(ins.operands[0].imm)
  ret=('pc' in ins.op_str and ins.mnemonic in ('pop','ldm','ldmia')) or ins.mnemonic=='bx'
  if ins.mnemonic!='b' and not ret:pending.append(pc+4)
 report['reachable_instructions']=len(reachable);report['uncovered_original']=[hex(x) for x in sorted(reachable-a.cover)]
 Path('build/analysis/replay-results.json').write_text(json.dumps(report,indent=2));print(json.dumps(report,indent=2))
```
