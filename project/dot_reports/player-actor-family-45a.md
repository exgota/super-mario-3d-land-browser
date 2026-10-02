# PlayerActor family on current checkpoint 45a4305

Frozen base: `45a4305466a7c4cbd87589169384102e88f8d1b5`. Branch: `dot/player-actor-family-45a`. Final source head before this note: `191e93a00295889fce08c843eaa976b75b5a0359`.

This apply-clean family contains two complete guarded C++ **NonMatching proposals**, with **zero accepted exact bytes**:

| Root | Identity | Original complete bytes | Project-built section bytes | Canonical result |
| --- | --- | ---: | ---: | --- |
| 00130438..001310B0 | PlayerActor::attackSensor | 3192 | 3004 | extent mismatch |
| 00131ABC..00132704 | PlayerActor::initSpecial | 3144 | 3080 | extent mismatch |

The complete proposed target family is 6,336 original bytes. Neither root is accepted, so no fraction of those bytes counts as exact. Both normal project CLI probes return 1 and say that the complete compiled section, including its literal pool, differs in size from the original interval.

## Ownership and immutable inputs

The dot independently refreshed main at 2026-10-02T00:26 UTC and received 45a4305. The exact tree snapshot was independently materialized and verified before the lane began initSpecial source work. Current STATE still reserves all eight Pro roots and assigns GhostPlayer/TreeNode/Garigari to actor_group; this PlayerActor family overlaps none of them. The parent explicitly directed one owning PlayerActor lane to keep these two methods together. Root/main owns STATE, map, ledger and canonical acceptance.

The earlier attackSensor checkpoint `ee962a4a096d6001d65211dacd735cbf7ed252a3` remains immutable against b16e2a0. Its complete source/notes patch was applied unchanged on 45a4305, then committed as c2ad18b. Its source hash stays identical; this note reports fresh current-base build, replay and preservation. The historical [attackSensor report](root-130438.md) retains its original base and is not a current-main acceptance claim.

This family edits only two new source TUs and two new reports. It edits no existing header or accepted source, and no map, function boundary, compiler configuration, flags, oracle, STATE, ledger or tool. No original game bytes are copied into a source body. All private data, objects, linked diagnostics and generated scripts remain ignored.

Selection assumption: the shared PlayerActor ownership and already recovered sensor/field relationships make the adjacent initializer useful downstream. Exact acceptance is uncertain and was not assigned a measured success probability. Investigation was selected for complete caller wiring and class-layout recovery, not a claimed achieved byte rate. No numerical downstream-byte yield is asserted.

## initSpecial identity, data and module evidence

Constructor 002768AC installs the actor table pointer 003C6828 at 002768F4. The root address 00131ABC occurs in that same table at 003C6888, slot +0x60. Existing clean PlayerActor declarations identify this slot as initSpecial(ActorInitInfo const&, PlayerActorInitInfo const&). Its actual caller convention saves this, initialization-info, and player-info pointers; its own body reads the model/animation-info fields and wires the player's subsystems. This independently connects the method to the same class as attackSensor. No other owner's TU or header is adopted.

The configured Game module uses ARMCC4.1/791. The established PlayerActor identity, existing accepted getProperty accessor, current Game sources, and project M0 evidence support using that module. The lane did not change flags or try another compiler.

The source imports six inlined interface-table addresses from already existing data rows: 003D0BE8..003D0C00, 003D092C..003D0940, 003CDE10..003CDE1C, 003A8C44..003A8C54, 003A8C54..003A8C64 and 003A8C64..003A8C68. The original stores those exact pointers into newly allocated 12-, 4-, 4-, 8-, 8- and 8-byte objects. Their contents and original class names are not reconstructed or credited. The source imports only the established row starts using neutral dat_ symbols. It invents no row or helper address.

The final animation-name buffer uses the existing FixedSafeString<64> definition. Retail installs 003D9D0C, eight bytes into the existing 003D9D04..003D9D18 table. No string-layer header change is needed. Existing PlayerActor fields and isolated raw pointer views expose only observed offsets; unknown subsystem class names, full types and constructor source signatures remain unresolved. Neutral fn_ prototypes describe the observed ABI arguments and returns, rather than asserting recovered original names.

The complete original interval has 740 instruction addresses, an internal 76-byte literal/string island at 00131E88..00131ED4, and a final 108-byte pool at 00132698..00132704. Literal placement does not establish an early end of executable code.

## initSpecial source and verification scope

One committed meaningful source form was tested for initSpecial, commit 191e93a. The root resets scale and two rotation components, converts rotation to a quaternion through the original helper, rotates two basis vectors with the observed floating operation order, creates Foot/Head/Body/TailAttack/Eye sensor requests, and binds their position vectors. It allocates and connects player/model/animation/control subsystems, copies the initial position/front/up, installs observed interface adapters, creates reaction services, and registers normal and RaccoonDog animation names.

The source preserves allocation sizes, constructor argument ordering, nullable subobject adjustments and the normal-path post-construction stores. It does not add allocation-failure recovery that retail lacks. Several failed allocations are followed by unconditional dereferences in retail; the returning replay domain uses successful allocations and makes no fault-timing claim.

The unchanged checker resolves the root's imports and rejects its 3,080-byte full section against 3,144 retail bytes. All private pointer/math helpers inline into the root; no fabricated address is assigned to them. The TU also emits the existing FixedSafeString template's small weak destructor/termination methods and table, none of which earns new credit. Diagnostic replay links the unchanged full compiler object at a scratch address, while canonical root checking resolves data to the established original rows.

The final initSpecial replay passes **364 complete caller-wiring fixtures** in **11.779 seconds**, and visits **740/740 executable root instruction addresses**. This is caller-wiring evidence under stated models, not verification of whole-player initialization or runtime behavior.

Thirty-two direct imports execute original code unchanged. These include original quaternion trig/math, vector scalar multiply, pose accessors and setters, leaf constructors, nested array initialization, linked-list insertion, property setters, animation-string formatting, and the observed interface binding at 00132D08. The original callee implementations can themselves call the modeled allocator, so this is not an entirely original runtime chain.

Thirty direct-import boundaries are modeled, including allocation at 002932B0 and the 29 resource/large-constructor/registration boundaries enumerated in the complete recipe below. The underlying array allocator 00292A78 is also modeled. Four virtual endpoint kinds model pose rotation/scale access, model update and actor appearance. Deep model/player constructors provide explicitly constructed state needed by later root accesses; these are fixture state, not recovered initializer implementations. The models record exact argument words, sensor names/radii/offset bits, constructor pointer wiring, and final registration strings. The complete direct-import sequence must match as well.

Each fixture compares the full 1MiB constructed object/heap region plus 448KiB of original writable-global range 003E0000..00450000, allocation size/address sequence, modeled events, and cumulative FPSCR exception bits0x9F. Writes outside those compared ranges or the excluded 256KiB stack range fail. r4..r11, d8..d15 and SP canaries must survive. The root returns void; private stack bytes and caller-saved register residue are outside the output claim.

The 364 cases comprise 64 count/enable/angle combinations, 240 deterministic randomized positions, finite rotations, registration arguments and short/long names, plus 60 angle-bit/floating-mode cases. Registration counts are 0..5. Finite rotations cover [-720,720], and angle-bit cases include signed zero, subnormal limits, minimum normal and positive/negative finite angles under default/FZ/DN and three nondefault rounding modes. This suite does not claim NaN/infinity handling in the trig path, floating traps, arbitrary aliasing or callback side effects, malformed-pointer/fault behavior, allocation-failure equivalence, dynamic catalog mutation, concurrent initialization, complete resources, real device execution or gameplay. Instruction-address coverage does not imply all paths or all inputs.

## Current-base family gate

The build began with no shared build directory and compiles 47 Game / 129 al / 1 SDK sources, then links and exports. Both sources were committed before project compilation and canonical checking. Final current-base CLI target probes repeat both expected extent rejections. Both temporary names are confined to the existing U rows and the map is restored exactly.

The complete current-base prior gate passes **707 prior roots / 726 actual canonical definitions**, with **zero failures**, in **160.933 seconds**. It checks the unique strong definition or every weak definition for each previously O root using unmodified tools/check.py. PlayerActor::getProperty is included. Main's own acceptance gate remains authoritative.

The unchanged attackSensor source also passes a fresh current-build replay of **18208 fixtures / 36416 repeated calls**, visits **780/780 root instruction addresses**, and takes **91.722 seconds**. Its direct retail callees, virtual endpoint models and bounded floating/alias limits are described completely in the earlier report's recipe. The final object was relinked from this current build; no earlier object was substituted.

Source and canonical-object SHA256 values:

- `PlayerActorAttackSensor.cpp` source: `89d89585a546531ea2b6373fed5fe419fc76a7e6b3388769a05cbcddd25900fc`
- `PlayerActorInitSpecial.cpp` source: `e0e1f57a30db139e09fb777a73acd5e6261011d7219f4e9f1ed6a9065e1ac432`
- `PlayerActorAttackSensor.o` object: `bef7f45656d34ef88d7d88bebdfa8cdd3ab06935d1ac2fd433de6894bbc94df4`
- `PlayerActorInitSpecial.o` object: `5ecc124d4debf469fa71ef9442cea8af56f8b79b66e60299d73cee5ba6479c80`
- Original/restored current map: `d517e7aa3eefdbb859e781d7d974f8daf4fbcb33b964edb7d0a5ef6a11db8bd9`
- Owner executable: `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`

AttackSensor investigation began 2026-10-02T00:06:01Z and its first frozen checkpoint was ready at00:26:16Z. Current-base refresh and initSpecial work began at00:26:51Z; its committed source build/check finished around00:32:49Z. The combined gate and caller-wiring replay finished around00:42UTC. These wall windows include overlapping work and must not be summed as elapsed time. New accepted bytes are zero, so accepted throughput remains **0 bytes/hour**. The carried attackSensor source is explicitly separated from newly investigated initSpecial work. Parent intake time is still outstanding.

## Executable reproduction

Use only owner-local ignored code.bin/exh.bin and the approved791/wibo/Python/Unicorn setup. Commit the intended source first. Build normally from the family root:

```sh
. ./development_environment.sh
export DEVKITARM=/usr
TMP=/tmp python make.py eu -ca
mkdir -p build/player_actor build/root130438
```

Save/run this as `build/player_actor/probe_family.py`; both expected target rejections are retained and all map bytes are restored:

```python
from pathlib import Path
import subprocess,os,hashlib,json
p=Path('data/ver/eu/map.csv');old=p.read_bytes();results=[]
roots={0x130438:('_ZN11PlayerActor12attackSensorEPN2al9HitSensorES2_','PlayerActorAttackSensor'),0x131abc:('_ZN11PlayerActor11initSpecialERKN2al13ActorInitInfoERK19PlayerActorInitInfo','PlayerActorInitSpecial')}
try:
 lines=[];found=set()
 for line in old.decode().splitlines(True):
  f=line.split(',')
  try:a=int(f[0],16)
  except ValueError:lines.append(line);continue
  if a in roots:assert not f[6].strip();f[6]=roots[a][0];found.add(a)
  lines.append(','.join(f))
 assert found==set(roots);p.write_text(''.join(lines))
 for a,(symbol,source) in roots.items():
  r=subprocess.run(['python','tools/check.py',symbol,'--object',f'build/eu/obj/Game/backup/src/Player/{source}.o'],capture_output=True,text=True,env=dict(os.environ,TMP='/tmp'))
  result={'root':hex(a),'returncode':r.returncode,'output':r.stdout+r.stderr};results.append(result);print(result)
finally:
 p.write_bytes(old);assert p.read_bytes()==old;print('restored',hashlib.sha256(old).hexdigest())
Path('build/player_actor/final-checks.json').write_text(json.dumps(results,indent=2))
```

Save/run this as `build/player_actor/link_init.py` to link the unchanged project object for diagnostic replay. Every import must correspond to an existing map row:

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
import re,csv,subprocess,os
obj='build/eu/obj/Game/backup/src/Player/PlayerActorInitSpecial.o'
rows=[{k.strip():v.strip() for k,v in r.items()} for r in csv.DictReader(open('data/ver/eu/map.csv'))];byname={r['Symbol']:int(r['Start'],16) for r in rows if r['Symbol']};lines=['#<SYMDEFS>#']
with open(obj,'rb') as f:
 e=ELFFile(f);st=e.get_section_by_name('.symtab');root=next(s.name for s in st.iter_symbols() if s.name.startswith('_ZN11PlayerActor11initSpecial'))
 for s in st.iter_symbols():
  if s['st_shndx']!='SHN_UNDEF' or not s.name or '$$' in s.name or s['st_info']['bind']=='STB_WEAK':continue
  n=s.name;m=re.fullmatch('(fn|dat)_([0-9A-F]{8})',n);a=int(m[2],16) if m else byname[n];kind='D' if m and m[1]=='dat' else 'A'
  assert any(int(r['Start'],16)==a for r in rows);lines.append(f'0x{a:08X} {kind} {n}')
Path('build/player_actor/init-imports.sym').write_text('\n'.join(lines)+'\n')
subprocess.run(['data/compilers/wibo','data/compilers/4.1/791/bin/armlink.exe','--cpu=MPCore','--fpu=VFPv2','--arm_only','--no_exceptions','--inline','--datacompressor=off','--no_debug','--no_scanlib','--mangled','--symbols','--map','--entry='+root,'--keep='+root,'--ro_base=0x00500000','--output=build/player_actor/init-candidate.axf','--list=build/player_actor/init-candidate.map',obj,'build/player_actor/init-imports.sym'],check=True,env=dict(os.environ,TMP='/tmp'))
```

Save/run this as `build/player_actor/preserve.py` for the full current-base prior-root/definition gate. Keep it sequential with any other tool that writes map.csv:

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
import csv,io,subprocess,sys,json,time,runpy,contextlib
D=Path('build/player_actor');mp=Path('data/ver/eu/map.csv');old=mp.read_bytes()
rows=[{k.strip():v.strip() for k,v in x.items()} for x in csv.DictReader(io.StringIO(old.decode()))]
prior={x['Symbol'] for x in rows if x['Rank']=='O' and 'f' in x['Type']};defs={x:[] for x in prior}
for p in sorted(Path('build/eu/obj').rglob('*.o')):
 rel=p.relative_to('build/eu/obj')
 if rel.parts[0] not in ('Game','lib') or not rel.with_suffix('.cpp').is_file():continue
 with p.open('rb') as f:
  st=ELFFile(f).get_section_by_name('.symtab')
  if st:
   for s in st.iter_symbols():
    if s.name in defs and isinstance(s['st_shndx'],int) and s['st_info']['type']=='STT_FUNC':defs[s.name].append((s['st_info']['bind']=='STB_GLOBAL',str(p)))
start=time.time();checks=[]
try:
 for i,symbol in enumerate(sorted(prior)):
  strong=[d for d in defs[symbol] if d[0]];paths=sorted({d[1] for d in strong or defs[symbol]});assert paths,(symbol,'missing');assert not strong or len(paths)==1,(symbol,'ambiguous')
  for p in paths:
   stream=io.StringIO();arguments=sys.argv;sys.argv=['tools/check.py',symbol,'--object',p];status=0
   try:
    with contextlib.redirect_stdout(stream),contextlib.redirect_stderr(stream):
     try:runpy.run_path('tools/check.py',run_name='__main__')
     except SystemExit as ex:status=ex.code or 0
   finally:sys.argv=arguments
   checks.append({'symbol':symbol,'object':p,'returncode':status,'output':stream.getvalue()})
   (D/'preservation-checkpoint.json').write_text(json.dumps(checks,indent=2))
  if (i+1)%100==0:print('checked',i+1,'/',len(prior),flush=True)
 result={'roots':len(prior),'definitions':len(checks),'passed':sum(x['returncode']==0 for x in checks),'seconds':time.time()-start,'checks':checks}
 (D/'preservation.json').write_text(json.dumps(result,indent=2));assert all(x['returncode']==0 for x in checks)
 print({k:v for k,v in result.items() if k!='checks'})
finally:
 mp.write_bytes(old);assert mp.read_bytes()==old
```

Save/run the complete initializer replay as `build/player_actor/replay_init.py`. It constructs all fixtures and writes init-result.json and init-coverage.json without any additional input artifact:

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import *
from unicorn.arm_const import *
import struct,json,random,hashlib,time
D=Path('build/player_actor');ROOT=0x131abc;STOP=0x600000;B=0x800000;SIZE=0x100000;SP=0xa30000
A=B;INFO=B+0x400;PI=B+0x800;MI=B+0xa00;AI=B+0xb00;POSE=B+0x1000;PROP=B+0x1400;PART=B+0x2000;HEAP=B+0x20000
original=Path('data/ver/eu/code.bin').read_bytes();assert hashlib.sha256(original).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
with (D/'init-candidate.axf').open('rb') as f:
 e=ELFFile(f);entry=e.header.e_entry;segments=[(s['p_vaddr'],s.data()) for s in e.iter_segments() if s['p_type']=='PT_LOAD']
def w(v):return struct.pack('<I',v&0xffffffff)
def ui(v):return struct.unpack('<I',v)[0]
def fs(v):return struct.pack('<f',v)
def vec(v):return b''.join(fs(x) for x in v)
def cs(u,a):
 b=bytes(u.mem_read(a,256));return b.split(b'\0')[0].decode('ascii',errors='replace')
def rw(u,a):return ui(u.mem_read(a,4))
imports={int(s.split()[0],16) for s in (D/'init-imports.sym').read_text().splitlines()[1:] if s.split()[1]=='A'}
# These constructor/resource boundaries are models, not implementations.
modeled={0x2cc99c:1,0x270ab0:2,0x26e64c:2,0x26e5d4:4,0x1cd4cc:4,0x1eb94c:2,0x1eb9d0:3,
0x182c18:7,0x14f554:3,0x1b4ccc:5,0x30933c:22,0x1a49d8:3,0x1a7ffc:4,0x1b8574:4,
0x1314b8:1,0x26e4c8:5,0x1ebddc:3,0x26e4a4:4,0x1e622c:2,0x28065c:1,0x26e2c8:1,
0x1851d0:0,0x18511c:0,0x185004:1,0x185040:1,0x1850e0:1,0x185194:1,0x15c228:4,0x15bc90:6}
# Other imports, including leaf constructors, vectors, trig, linked-list insertion,
# array initialization and formatting, execute original code without interception.
endpoints={0x600100:'poseRotation',0x600104:'poseScale',0x600108:'modelUpdate',0x60010c:'actorAppear'}
coverage=set();reached=set();cfg={};events=[[],[]];calls=[[],[]];heap=[HEAP,HEAP];allocs=[[],[]]
regs=list(range(UC_ARM_REG_R4,UC_ARM_REG_R11+1));can=[0xa0001000+i*0x1111 for i in range(8)]
vregs=list(range(UC_ARM_REG_D8,UC_ARM_REG_D15+1));vcan=[0x1122334455667788+i*0x1111 for i in range(8)]
def allocate(u,which,n):
 assert 0<n<0x10000,('allocation',n)
 a=heap[which];heap[which]+=((n+63)//64)*64+64;assert heap[which]<B+SIZE
 u.mem_write(a,bytes([0xa5])*n);allocs[which].append((a,n));return a

def machine(which):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176);u.mem_map(0x100000,0x600000);u.mem_write(0x100000,original)
 for a,b in segments:u.mem_write(a,b)
 u.mem_map(B,SIZE);u.mem_map(0xa00000,0x40000);u.reg_write(UC_ARM_REG_C1_C0_2,0xf00000);u.reg_write(UC_ARM_REG_FPEXC,0x40000000)
 def ret(v):u.reg_write(UC_ARM_REG_R0,v);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 def hook(u,a,size,data):
  if a==STOP:u.emu_stop();return
  if which==0 and ROOT<=a<0x132704:coverage.add(a)
  if a in imports:calls[which].append(a);reached.add(a)
  if a not in modeled and a not in endpoints and a not in (0x2932b0,0x292a78):return
  r=[u.reg_read(x) for x in [UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3]]
  if a in (0x2932b0,0x292a78):
   events[which].append(['allocate',r[0]]);ret(allocate(u,which,r[0]));return
  if a in endpoints:
   name=endpoints[a];args=[r[0]];value=0
   if name=='poseRotation':value=POSE+0x10
   elif name=='poseScale':value=POSE+0x1c
   elif name=='modelUpdate':args.append(bytes(u.mem_read(r[1],4)).hex())
   events[which].append([name,args]);ret(value);return
  if a not in modeled:return
  n=modeled[a];args=r[:min(n,4)]+[rw(u,u.reg_read(UC_ARM_REG_SP)+4*i) for i in range(max(n-4,0))];value=r[0]
  if a in (0x26e5d4,0x1cd4cc):
   args=[r[0],cs(u,r[1]),r[2],bytes(u.mem_read(r[3],12)).hex(),u.reg_read(UC_ARM_REG_S0)];value=allocate(u,which,0x40)
  elif a in (0x1eb94c,0x1eb9d0):args[1]=cs(u,r[1])
  elif a==0x182c18:
   u.mem_write(r[0],w(B+0x18100));u.mem_write(r[0]+0x18,w(PART+0x180));u.mem_write(r[0]+0x20,w(PART+0x200))
  elif a==0x30933c:
   for off,p in [(0,PROP),(0x14,PART+0x100),(0x38,PART+0x300),(0x40,PART+0x400),(0x48,PART+0x480),(0x54,PART+0x500),(0x58,PART+0x580),(0x80,PART+0x800),(0xac,PART+0xac0)]:u.mem_write(r[0]+off,w(p))
  elif a in (0x1ebddc,0x26e4a4):args[-1]=cs(u,args[-1])
  elif a==0x1851d0:value=cfg.get('enabled',1)
  elif a==0x18511c:value=cfg.get('count',2)
  elif a==0x185004:value=cfg.get('arg5',0x10203040)^r[0]
  elif a==0x185040:value=cfg.get('arg4',0x23456789)+r[0]
  elif a==0x1850e0:value=B+0x3000+128*r[0]
  elif a==0x185194:value=B+0x4000+128*r[0]
  elif a==0x15c228:args[2]=cs(u,args[2])
  elif a==0x15bc90:args[3]=cs(u,args[3])
  events[which].append([hex(a),args]);ret(value)
 def writes(u,access,a,size,value,data):
  assert (B<=a and a+size<=B+SIZE) or (0x3e0000<=a and a+size<=0x450000) or (0xa00000<=a and a+size<=0xa40000),('write outside comparison',hex(a),size)
 u.hook_add(UC_HOOK_CODE,hook);u.hook_add(UC_HOOK_MEM_WRITE,writes);return u
machines=[machine(0),machine(1)]
initial_globals=original[0x2e0000:]+b'\0'*(0x70000-len(original[0x2e0000:]))
def fixture(c):
 mem=bytearray(SIZE)
 def put(a,v):mem[a-B:a-B+len(v)]=v
 def ptr(a,v):put(a,w(v))
 ptr(A,B+0x18200);ptr(A+0x14,POSE);ptr(A+0xa0,PART+0xe00);ptr(A+0x114,PART+0xe40);ptr(A+0x120,PART+0xe80)
 ptr(POSE,B+0x18000);ptr(B+0x18000,0x600100);ptr(B+0x18000+0x18,0x600100);ptr(B+0x18000+0x1c,0x600104)
 put(POSE+4,vec(c.get('position',(3.25,-2,7))));put(POSE+0x10,vec(c.get('rotation',(4,37,9))))
 ptr(B+0x18100,0x600108);ptr(B+0x18200+0x10,0x60010c)
 ptr(PI,MI);ptr(PI+4,AI);ptr(MI,PART+0xf00);ptr(MI+0x6c,PART+0xf40);ptr(AI,PART+0xf80);ptr(PART+0x400,c.get('figure',3))
 for i in range(10):put(B+0x3000+128*i,(c.get('name','Motion')+str(i)).encode()+b'\0')
 for off,bits in c.get('floatBits',[]):ptr(B+off,bits)
 return mem
results=[];start=time.time()
def run(c):
 global cfg
 cfg=c;mem=fixture(c)
 for which,u in enumerate(machines):
  u.mem_write(B,bytes(mem));u.mem_write(0x3e0000,initial_globals);u.reg_write(UC_ARM_REG_FPSCR,c.get('fpscr',0));events[which]=[];calls[which]=[];heap[which]=HEAP;allocs[which]=[]
 out=[]
 for which,u in enumerate(machines):
  for r,v in zip(regs,can):u.reg_write(r,v)
  for r,v in zip(vregs,vcan):u.reg_write(r,v)
  u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,STOP);u.reg_write(UC_ARM_REG_R0,A);u.reg_write(UC_ARM_REG_R1,INFO);u.reg_write(UC_ARM_REG_R2,PI)
  try:u.emu_start(ROOT if which==0 else entry,STOP+4,count=2000000)
  except Exception as ex:
   print('fault',which,hex(u.reg_read(UC_ARM_REG_PC)),events[which][-5:],flush=True);raise
  assert u.reg_read(UC_ARM_REG_PC)==STOP,('did not return',hex(u.reg_read(UC_ARM_REG_PC)))
  assert [u.reg_read(r) for r in regs]==can and [u.reg_read(r) for r in vregs]==vcan and u.reg_read(UC_ARM_REG_SP)==SP
  out.append((bytes(u.mem_read(B,SIZE)),bytes(u.mem_read(0x3e0000,0x70000)),u.reg_read(UC_ARM_REG_FPSCR)&0x9f))
 if out[0]!=out[1] or events[0]!=events[1] or calls[0]!=calls[1] or allocs[0]!=allocs[1]:
  detail={'fixture':c,'events':events,'calls':[[hex(a) for a in xs] for xs in calls],'fpscr':[x[2] for x in out],'memory_diff':[(hex(B+i),a,b) for i,(a,b) in enumerate(zip(out[0][0],out[1][0])) if a!=b]}
  (D/'init-failure.json').write_text(json.dumps(detail,indent=2));raise AssertionError(str(detail)[:3000])
 results.append(c)
fixtures=[{'count':n,'enabled':e,'rotation':(10,angle,-10)} for n in [0,1,2,5] for e in [0,1] for angle in [0,1,30,45,90,180,-90,360]]
for i in range(240):
 r=random.Random(i);fixtures.append({'count':r.randrange(6),'enabled':r.randrange(2),'rotation':tuple(r.uniform(-720,720) for _ in range(3)),'position':tuple(r.uniform(-1000,1000) for _ in range(3)),'figure':r.randrange(7),'arg4':r.getrandbits(32)-5,'arg5':r.getrandbits(32),'name':r.choice(['Run','Wait','Jump','A'*45])})
for bits in [0,0x80000000,1,0x80000001,0x007fffff,0x00800000,0x3f000000,0xbf000000,0x42b40000,0xc2b40000]:
 for mode in [0,0x1000000,0x2000000,0x400000,0x800000,0xc00000]:
  fixtures.append({'count':2,'floatBits':[(0x1014,bits)],'fpscr':mode})
try:
 for i,c in enumerate(fixtures):
  run(c)
  if (i+1)%50==0:print('passed',i+1,flush=True)
except Exception:print('FAILED',len(results),flush=True);raise
(D/'init-result.json').write_text(json.dumps({'fixtures':len(results),'seconds':time.time()-start,'root_instructions':len(coverage),'original_direct_imports':sorted(hex(a) for a in reached-modeled.keys()-{0x2932b0,0x292a78}),'modeled_direct_imports':sorted(hex(a) for a in reached&(modeled.keys()|{0x2932b0,0x292a78}))},indent=2));(D/'init-coverage.json').write_text(json.dumps(sorted(coverage)));print((D/'init-result.json').read_text())
```

For the current-build attackSensor replay, save/run the probe/link/replay Python blocks in [root-130438.md](root-130438.md) after the current build, using their stated build/root130438 paths. Those scripts contain the complete fixture generation and original-callee replay and work unchanged on this family. Do not run the two map-mutating preservation/probe scripts concurrently.
