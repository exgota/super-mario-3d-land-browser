# Texture surface upload: bounded NonMatching reconstruction

Frozen intake base: `54d39734c4edffeeac012157ab71fb11c40d815b`.
Branch: `dot/texture-surface-upload`. Final source commit: `6adc4fa`.
Work window: 2026-10-02 02:56:19 UTC to 2026-10-02T03:19:26.921293+00:00,
1387.921 elapsed seconds. These are actual wall times;
compilation, replay and source inspection overlapped.

The original unnamed root at `0x0020FC34..0x00210618` contains **2532 complete
bytes**. The submitted normal SDK 902 C++ section contains **2272 bytes**.
**Zero exact roots and zero exact bytes are claimed.** This is a bounded
NonMatching reconstruction of the full routine, with the original public name
unknown. The existing map row remains unnamed, type `f`, rank `U`, with its
original pool and end boundaries. Root must decide any later enrollment.

## Submitted family and independent identity

Only new source `lib/CtrSDK/sources/gx_TextureUpload.cpp` and these reports are
proposed. No shared header or pre-existing translation unit changes. No held
FileHandle, NoteObj, Execute or Controller family is touched. STATE, newest
daily, DOT_BRIEF, BRIEF and Guide were read; the frozen STATE and packet history
contained no owner or earlier attempt for this root. The SDK module location
follows the graphics-library caller/callee evidence; it is not a recovered
original source path.

The named original `glCompressedTexImage2D` at `0x0038EE70` independently calls
this root at `0x0038EF80` and `0x0038F004`. Its 2D call supplies owner+0x34 as the
surface; its cube-face address calculation uses 68-byte descriptor steps.
The independent original `__srf_initSurface` writes matching fields: resident,
staging and input pointers at 0/4/8; width/height/format/type at 0xc/0x10/0x14/0x18;
normalized pixel format at 0x1c; total bytes at 0x20; levels/bits per pixel at
0x34/0x38; allocation region/alignment at 0x3c/0x40. The source preserves the
unidentified 16-byte middle span instead of inventing fields. Owner mode at
0x2c and mip-generation flag at 0x30 are the only owner fields used.

Existing unnamed data row 0x003E2654 supplies the allocation callback cell.
Only the first word of row 0x003E2658 supplies the release callback cell; that
row spans 16 bytes and its remaining identity is unknown. The source neither
defines those cells nor claims a public name for their pointees.

The routine manages surface reuse/reallocation, transforms all 14 normalized
pixel formats, flips and tiles linear pixels into 8x8 Morton order, packs
half-byte channels, reorders ETC blocks into 2x2 groups, uploads successive mip
levels and optionally submits generation of lower levels. Modes 0x01010000,
0x01020000 and 0x01030000 use conversion/staging; 0x02010000 assigns the incoming
buffer directly. GPU copies use the observed staging/input choice.

## Actual compiler/check results and capped history

All builds used the repository's unchanged normal build step and global flags.
Only the temporary existing-row symbol `fn_0020FC34` was assigned for each
object check; the map was restored byte-for-byte immediately afterward.
No boundary, Type or data identity changed. The final check was run after the
fresh clean build against committed C++ source, with verified project provenance:

```text
python tools/check.py fn_0020FC34 --object build/eu/obj/lib/CtrSDK/sources/gx_TextureUpload.o
U -> M: The complete compiled section, including its literal pool, has a different size from the original interval.
```

1. SDK 902, integrated conversion and byte-swap intrinsics: 2168 bytes. The
   initial `__rev16` spelling failed compilation; supported `__rev` plus shift
   was substituted before this measured form. Commit 6392df7.
2. Identical C++ in the configured 791 module as a compiler diagnostic: 2212
   bytes, also a size rejection. Commit 50285c4. This did not establish original
   compiler identity and was not adopted as a module/flag rescue.
3. Normal SDK 902, portable swap expressions and reading all RGB input bytes
   before storing any output: 2256 bytes. Commit 83ab33e. Its 898 bounded cases
   agreed, but an additional 18-case mip probe exposed 9 selector differences.
4. Normal SDK 902, preserve the original scratch selector: 2272 bytes.
   Commit 6adc4fa. This is the final source; no fifth form was attempted.

No meaningful same-size differing-byte count exists. All four forms remain
unsuccessful for exact matching. The original compiler build is unresolved;
neither the 902 nor 791 results establishes it. ARMCC 894 is unavailable here.
The capped packet with complete original disassembly and literal tables is
`project/pro_requests/0020FC34.md`.

The fourth form addresses an independently grounded behavior, not a register
allocation permutation. At 0x0020FE94, ETC conversion puts zero in original r8
as the block origin. Linear conversion updates r8 to each flipped destination
row at 0x002102BC..0x002102F4. The later switch at 0x00210554 writes a mip selector
only for pixel formats 0..4; other formats pass the retained r8 at 0x002105BC.
The final C++ retains those high-level scratch assignments. The prior probe
showed original 0 versus candidate 0x1908 for software-tiled formats 5..13 with
mip generation; native-order controls agreed. Those 9 failures are preserved
in the ignored `limits.json` and are not reclassified as old-source successes.

## Whole-root replay and explicit limits

The final project-generated object was linked by unmodified ARM linker 902
at actual replay entry 0x00500000. Original entry 0x0020FC34 and all original
executable bytes stayed unchanged. Both roots were executed on Unicorn's
ARM1176 model. No hand assembly, opcode replacement, object patching, invented
original symbol or copied source implementation was used.

The main suite passed 1402/1402 paired cases: 1390 normal returns
and 12 allocator-null memory faults. Supplementary default-format and odd
half-byte-count controls passed 24/24.
Total: **1426 paired cases (2852 root executions)**, including 1414 returns
and 12 fault pairs. The union executed **600/600
original root instructions**, excluding its 31 switch-target words and 2 literal
words, and 537 compiled-root instructions.
Instruction coverage is not exhaustive input or path proof.

Cases cover pixel formats 0..13 plus 14/15/0xffffffff default controls; dimensions
from 0 through 32 including odd/non-tile sizes; levels -1/0/1/2/3/4; all six observed
storage modes and unknown-mode controls; native/software order; null data;
null allocation/release callbacks; allocation failure; temporary/source alias;
reuse and each descriptor mismatch; real surface release/reinitialization;
and mip generation for every format, including zero dimensions.

Original code actually executed includes `__srf_initSurface`, 0x00205FA4 and
0x0028B068 in lifecycle fixtures, `__rt_memcpy`, `__aeabi_memcpy4`,
`_memcpy_lastbytes`, `__aeabi_uidivmod`, `__arm_div5` and `__arm_div2`.
Other lifecycle fixtures use declared deterministic models for allocation
success/failure and retained-storage controls. User allocation/free callbacks
are modeled and deliberately clobber caller-saved registers. All actual
compiled source execution is the one root; no source helper remains.

GPU upload 0x002106F0 is modeled as an argument-recorded CPU copy. Cache/service
entry 0x00391030 is an argument-recorded return. Mip submission 0x00205EE8 records
its five arguments but does not generate pixels. Thus these tests do not prove
GPU commands, hardware cache behavior, rendered pixels, texture sampling,
scene submission or gameplay. Allocation callback implementations and arbitrary
surface initializer inputs are not reconstructed here.

Each paired run compares owner/surface state, the full 0x50000-byte input/
resident/staging/temporary slab (including unused guard contents), observable
callback/submission arguments, termination/fault class and returned callee-saved
registers/SP. Void-root r0 and caller-saved registers are not ABI outputs.
The 12 fault pairs agree on null address, access type/width, memory and events;
**in-flight register and stack states differ**, and no full fault-state identity
is claimed. This distinction was discovered and retained in the reports rather
than presented as a same-register success.

The main suite executed 23288710 original and
22702195 candidate ARM instructions in 94.393
seconds. Its maximum was 135295 original and
133445 candidate instructions per case. Every run terminated
or faulted before the 400000-instruction bound; no bounded cycles/nontermination
were observed. These are emulator instruction counts, **not hardware cycles**.
No signed-overflow, huge-dimension, concurrency, arbitrary pointer or universal
memory-safety claim is made. Default-format behavior uses deterministic
allocator bytes and does not establish that such textures are supported.

## Clean build and prior-root preservation

Final `python make.py eu -ca` returned 0 from 2026-10-02T03:12:47.674755+00:00 through 2026-10-02T03:13:25.500399+00:00,
37.826 seconds. Its allocated canonical image equals the frozen
baseline byte-for-byte. All 468 pre-existing Game/lib/config/map/tool inputs
are byte-identical. All 177 baseline canonical C++ objects have identical
allocated-section bytes, symbol identities and relocation contents.

This is equivalence-backed preservation of the coordinator's prior 731 roots /
751 actual definition checks, **not a new 751-check run**. Their baseline report
has zero failures, clean-build return 0, and SHA256
`114120a460faf7dcbca722a5afffc48904e81c052a1598788ad627eaccbf9b82`.
The 169 objects named by those 751 checks all compare, plus the other 8 baseline
objects: PlayerFunction, SceneObjFactory, ProductStateStage,
StageProgressAccessors, Application, alActorExecutionHooks,
alStageSwitchTypeCount and alByamlIteratorConstructors. The latter 8 have no
prior-check entry; they were not silently omitted from the 177-object audit.

The source object's complete source/helper closure is its single 2272-byte
`i.fn_0020FC34` section and ordinary unwind metadata. No unmapped code helper or
new allocated source data remains. Its ten external imports resolve to existing
map rows: the two callback cells, initializer, allocation/release roots, memcpy,
unsigned division, upload, cache/service and mip-submission entries. A direct
behavior link uses those same established addresses and no embedded target bytes.

Main later advanced to 754f99a while this work was in flight. All claims here
remain pinned to 54d3973. The patch does not modify any existing file; root still
owns canonical intake and any new-main preservation check. Exact accepted
throughput for this work window is 0 bytes/hour.

## Frozen identities and local evidence

Source SHA256: `5aab23ba7a1dcbd14b478d217c18d9daf1ef949ecc67a60e74e78de0674c8418`.
Project object SHA256: `570bfeca85f52fa1b90df72faf00bc9a220b8a10da84f64df640c22756f97d76`.
Replay AXF SHA256: `090ea02d030b339a688157fc6a84735a46b94882df5294dc26e4c47c61b81f48`.
Original executable SHA256:
`e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.
Restored map SHA256: `434ad589b05ee299e967025dc3bc9ace954e547db7f83911fba2dc72daf8eb90`.

Checker SHA256: `e177e0abe130e645e75c5f0dc24abb19dbe83310de55f1f099ec8fc385e07317`.
Exact-byte implementation SHA256: `aef81a3a21c49bb17b8a19f139d02607899257e04dbe461ece2992cc726fc2e2`.
Provenance implementation SHA256: `343d1a0954aba9a4805e71420be9a99d91185db4c8b329d1363d11efcee53529`.
Compiler binary SHA256: `e6aca4ebc280a54065406bcc3a0ae3d9f4d0ac5a4032c3a193adb836c72e70fe`.

The source's four effective build inputs and SHA256 values are:

- `Game/project_globals.h`: `6c0b7103d61651e77e1a5db8bb1f5bdf509ad3d64c419c4ba22cadaed6a933d1`
- `data/config.json`: `5d41e617eb5b26374ed60b5383a0940ce0ce7d046344f7f815b7564144822c45`
- `lib/CtrSDK/include/nn/types.h`: `40aa4e5ee7d8980e077963ee87bc6bdeb8908249603c073b5980aed937d5e0bc`
- `lib/CtrSDK/sources/gx_TextureUpload.cpp`: `5aab23ba7a1dcbd14b478d217c18d9daf1ef949ecc67a60e74e78de0674c8418`

Ignored local evidence lives under `build/research20fc34/`. Source and reports
are the only committed artifacts. The evidence contains original disassembly,
per-form source/object snapshots, build/check logs, the unmodified linker
command and symbol definitions, deterministic replay harness, archived nine
selector divergences, the final main/supplementary replay results and all 177
object-equivalence entries. These ignored binaries are not included in the
proposal. Hashes of the final replay/equivalence artifacts:

- `replay.py`: `d8fe3971d1aece78d578fb5d2520a0a2033b0a65c403a5bc015b0c23d1c4e1e7`
- `additional.py`: `93f8da11f82eac3f4cbbf2df5a558b2a3bc462c8ac5c52f28486d8a2924e420e`
- `replay.json`: `1a693b3eeebe6cb2d953430398db0c14519e6b43a326fa998949de820d876256`
- `additional-replay.json`: `183e906d395b1de07c25e3dd1ea7dc587303ff01a068f076ece06709481f8914`
- `preserve.py`: `9771b1adbb1bec951bc6e2642f917af4ab021d4bed9b053ae959da94f997681e`
- `preservation.json`: `1af125be95f566104e6cf753b95e57d30f17619c6a89458492fef74d1786ffd7`
- `final-clean-build.json`: `7808012f6954ba34e18788d56994e96570285719b8027b1cf79af9e3c7503f23`
- `final-check.log`: `f8ff6a75281202afd35b60a0bd26fa6ade1285bc69e30bdda330a6ef183d607a`
- `limits.json`: `d9356dcfe619df7766eafe65bdad6e60d3f5065d5c9c745308afbc633cf6f468`

To reproduce, apply this family unchanged to the named base, supply the owner's
ignored verified EU binary and approved compilers, source the repository
environment and run the normal clean build. Temporarily give the already
existing unnamed function row the ABI label `fn_0020FC34`, run the exact object
check shown above, and restore the original map bytes. The checker should
reject the 2272/2532 size difference. Root can rerun the existing strict prior
root/definition gate for canonical acceptance; local equivalence alone grants
no new exact credit.

## Reproducible bounded replay appendix

The following scripts are supplied as report text so the deterministic replay
and preservation evidence can be reproduced without committing a tool or any
private game data. Save each fenced block under the indicated ignored path,
after the normal canonical build and source provenance verification. The
linker script uses only compiler output and existing map import addresses.
Run `link_behavior.py`, `replay.py`, `additional.py`, then `preserve.py` from the
repository root. The preservation script expects the separately verified
frozen baseline worktree at sibling path `mario-main54d`; it is not a substitute
for creating and checking that baseline. The replay needs the already-used
Unicorn and pyelftools Python packages. The checker itself is never modified.

### `build/research20fc34/link_behavior.py`

```python
from pathlib import Path
import sys,subprocess,json
sys.path.insert(0,str(Path.cwd()))
from elftools.elf.elffile import ELFFile
from tools.low.checkExactBytes import _read_map,_resolve_symbol
from tools.low.buildProvenance import verify_build_output
root=Path.cwd();out=root/'build/research20fc34';obj=root/'build/eu/obj/lib/CtrSDK/sources/gx_TextureUpload.o'
print(verify_build_output(obj))
rows=_read_map(root/'data/ver/eu/map.csv'); defs=[]
with obj.open('rb') as f:
 e=ELFFile(f);tab=e.get_section_by_name('.symtab')
 for s in tab.iter_symbols():
  if s['st_shndx']=='SHN_UNDEF' and s.name and not s.name.startswith('Lib$$Request'):
   addr,kind,row=_resolve_symbol(s,None,rows)
   defs.append(f'0x{addr:08X} {kind} {s.name}\n')
(out/'behavior.sym').write_text('#<SYMDEFS>#\n'+''.join(defs))
(out/'behavior.sct').write_text('LOAD 0x00500000\n{\n CODE 0x00500000\n {\n  * (i.fn_0020FC34, +FIRST)\n  * (+RO)\n }\n}\n')
cmd=[str(root/'data/compilers/wibo'),str(root/'data/compilers/4.0/902/bin/armlink.exe'),'--cpu=MPCore','--fpu=VFPv2','--arm_only','--no_exceptions','--inline','--datacompressor=off','--no_debug','--no_scanlib','--mangled','--symbols','--map','--entry=fn_0020FC34','--keep=fn_0020FC34',f'--scatter={out}/behavior.sct',f'--output={out}/behavior.axf',f'--list={out}/behavior.map',str(obj),str(out/'behavior.sym')]
r=subprocess.run(cmd,capture_output=True,text=True);print(r.stdout,r.stderr);r.check_returncode()
(out/'behavior-link.json').write_text(json.dumps(dict(command=cmd,imports=defs),indent=2))

```

### `build/research20fc34/replay.py`

```python
from pathlib import Path
import sys,json,struct,hashlib,random,time
from elftools.elf.elffile import ELFFile
from unicorn import *
from unicorn.arm_const import *
ROOT=Path.cwd();OUT=ROOT/'build/research20fc34';CODE=(ROOT/'data/ver/eu/code.bin').read_bytes()
assert hashlib.sha256(CODE).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
OWNER=0x800000;SURF=0x800100;INPUT=0x810000;RESIDENT=0x820000;STAGING=0x830000;TEMP=0x840000
ALLOC=0x7f0000;FREE=0x7f0010;STOP=0x7f1000
R=[UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3]
SAVED=[UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11,UC_ARM_REG_SP]
BPP=[32,24,16,16,16,16,16,8,8,8,4,4,4,8]
SUPPORT_ORIGINAL=set()
SEG=[]
with (OUT/'behavior.axf').open('rb') as f:
 e=ELFFile(f)
 for s in e.iter_sections():
  if s['sh_flags']&2: SEG.append((s['sh_addr'],s.data()))
 entry=e.header.e_entry

def run(candidate,case):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176)
 u.mem_map(0x100000,0x400000);u.mem_write(0x100000,CODE)
 u.mem_map(0x500000,0x10000)
 if candidate:
  for a,b in SEG:u.mem_write(a,b)
 u.mem_map(0x700000,0x100000);u.mem_map(0x800000,0x60000)
 u.mem_write(0x800000,b'\xa6'*0x60000)
 def rd(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def wr(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 pf=case.get('pf',0);w=case.get('w',16);h=case.get('h',16);levels=case.get('levels',1);mode=case.get('mode',0x1010000);fmt=case.get('format',0x1908);typ=case.get('type',0x1401)
 bpp=BPP[pf] if pf<14 else 32
 total=sum(max(0,(w>>i)*(h>>i)*bpp//8) for i in range(max(levels,0)))
 wr(OWNER+44,case.get('old_mode',mode));u.mem_write(OWNER+48,bytes([case.get('mips',0)]))
 for i,v in enumerate([0 if case.get('empty') else RESIDENT,0 if case.get('empty') else STAGING,0,w,h,fmt,typ,pf,total,0,0,0,0,levels,bpp,0x10000,0]):wr(SURF+i*4,v)
 if case.get('mismatch') is not None: wr(SURF+case['mismatch'],case.get('mismatch_value',123))
 u.mem_write(INPUT,bytes((i*37+case.get('seed',0)*19)&255 for i in range(0x10000)))
 wr(0x3e2654,0 if case.get('null_alloc') else ALLOC);wr(0x3e2658,0 if case.get('null_free') else FREE)
 sp=0x7e0000
 for i,v in enumerate([h,fmt,typ,0 if case.get('null_data') else INPUT,case.get('initFlags',0),case.get('native',0),mode]):wr(sp+i*4,v)
 for r,v in zip(R,[OWNER,SURF,levels,w]):u.reg_write(r,v&0xffffffff)
 for i,r in enumerate(SAVED[:-1]):u.reg_write(r,0xa4000000+i)
 u.reg_write(UC_ARM_REG_SP,sp);u.reg_write(UC_ARM_REG_LR,STOP)
 events=[];invalid=[];coverage=set();steps=0;allocs=0
 def finish(value=0):
  for r in R+[UC_ARM_REG_R12]:u.reg_write(r,0xdeadbeef)
  u.reg_write(UC_ARM_REG_R0,value&0xffffffff);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 def hook(u,a,z,d):
  nonlocal steps,allocs
  steps+=1
  if (0x20fc34<=a<0x210610) or (0x500000<=a<0x510000):coverage.add(a)
  elif 0x100000<=a<0x500000:SUPPORT_ORIGINAL.add(a)
  if a not in [ALLOC,FREE,0x37f0e0,0x205fa4,0x28b068,0x2106f0,0x391030,0x205ee8]:return
  args=[u.reg_read(r) for r in R]
  if a==ALLOC:
   allocs+=1;ptr=TEMP+(allocs-1)*0x2000
   if case.get('real_surface'):
    if allocs==1:ptr=RESIDENT
    elif allocs==2 and mode in [0x1020000,0x1030000]:ptr=STAGING
   if case.get('alias_alloc'):ptr=INPUT
   if allocs==case.get('fail_alloc'):ptr=0
   events.append(['allocate',args,ptr]);finish(ptr)
  elif a==FREE:events.append(['release',args]);finish()
  elif a==0x37f0e0:
   events.append(['init_surface',args,[rd(u.reg_read(UC_ARM_REG_SP)+i*4) for i in range(3)]])
   if not case.get('real_surface'):
    wr(SURF+12,args[0]);wr(SURF+16,args[1]);wr(SURF+20,args[2]);wr(SURF+24,args[3]);wr(SURF+52,levels);finish()
  elif a==0x205fa4:
   events.append(['allocate_surface',args[:2]])
   if not case.get('real_surface'):
    wr(SURF,0 if case.get('fail_surface') else RESIDENT);wr(SURF+4,STAGING);finish(0 if case.get('fail_surface') else 1)
  elif a==0x28b068:
   events.append(['release_surface',args[:2]])
   if not case.get('real_surface'):
    if not case.get('keep_surface'):wr(SURF,0)
    finish()
  elif a==0x2106f0:
   events.append(['upload',args[:3]])
   u.mem_write(args[0],bytes(u.mem_read(args[1],args[2])));finish()
  elif a==0x391030:events.append(['flush',args[:2]]);finish()
  elif a==0x205ee8:
   fmt_mip=rd(u.reg_read(UC_ARM_REG_SP));events.append(['mip',args,fmt_mip])
   # Endpoint only: observable call arguments, no GPU generation implementation.
   finish()
 def bad(u,access,addr,size,value,d):invalid.append([access,addr,size]);return False
 u.hook_add(UC_HOOK_CODE,hook);u.hook_add(UC_HOOK_MEM_INVALID,bad)
 fault=None
 try:u.emu_start(entry if candidate else 0x20fc34,STOP,count=400000)
 except UcError as e:fault=str(e)
 domains=[bytes(u.mem_read(a,n)) for a,n in [(OWNER,0x144),(INPUT,0x50000)]]
 return {'terminated':u.reg_read(UC_ARM_REG_PC)==STOP,'fault':fault,'invalid':invalid,'events':events,'memory':[hashlib.sha256(v).hexdigest() for v in domains],'saved':[u.reg_read(r) for r in SAVED]},coverage,steps,domains

def suite():
 cases=[]
 for pf in range(14):
  for w,h in [(8,8),(16,8),(8,16),(16,16),(32,8),(12,12),(24,16),(4,4),(1,1),(0,8),(8,0)]:
   cases.append(dict(pf=pf,w=w,h=h,seed=len(cases)))
  for levels in [-1,0,1,2,3,4]:cases.append(dict(pf=pf,w=32,h=32,levels=levels))
  for mode in [0x1010000,0x1020000,0x1030000,0x2010000,0x2020000,0x2030000,0,0x1010001]:
   for native in [0,1]:cases.append(dict(pf=pf,mode=mode,native=native))
  for flag in ['null_data','null_free','null_alloc','empty']:
   cases.append(dict(pf=pf,**{flag:1}))
 for pf in range(14):
  for native in [0,1]:
   for mode in [0x1010000,0x1020000,0x1030000,0x2010000,0x2020000,0x2030000]:
    for levels in [0,1,2,4]:cases.append(dict(pf=pf,native=native,mode=mode,mips=1,levels=levels,w=32,h=32))
 for pf in range(12):
  for w,h in [(8,8),(9,9),(16,8)]:cases.append(dict(pf=pf,w=w,h=h,alias_alloc=1))
 for pf in range(5,14):
  for native in [0,1]:
   for w,h in [(0,8),(8,0),(4,4),(9,9)]:cases.append(dict(pf=pf,w=w,h=h,native=native,mips=1,levels=2))
 for off in [12,16,20,24,52]:
  for flags in [{},{'keep_surface':1},{'fail_surface':1},{'null_data':1}]:cases.append(dict(mismatch=off,**flags))
 for flags in [{'old_mode':0x1020000},{'empty':1},{'empty':1,'fail_surface':1}]:
  for native in [0,1]:cases.append(dict(native=native,**flags))
 for mode in [0x1010000,0x1020000,0x1030000,0x2010000,0x2020000,0x2030000]:
  for mismatch in [12,16,20,24,52]:cases.append(dict(real_surface=1,mode=mode,mismatch=mismatch))
 for mode in [0x1010000,0x1020000,0x1030000,0x2010000,0x2020000,0x2030000]:
  for native in [0,1]:
   for flags in [{},{'fail_alloc':1},{'null_alloc':1},{'null_free':1}]:
    cases.append(dict(real_surface=1,empty=1,mode=mode,native=native,**flags))
 return cases
if __name__=='__main__':
 start=time.time();fault_register_differences=[];nonterminating=[];failures=[];origcov=set();candcov=set();totals=[0,0];faults=0;maxsteps=[0,0]
 cases=suite()
 for n,c in enumerate(cases):
  a,ca,sa,ma=run(False,c);b,cb,sb,mb=run(True,c)
  origcov|=ca;candcov|=cb;totals[0]+=sa;totals[1]+=sb;maxsteps=[max(maxsteps[0],sa),max(maxsteps[1],sb)]
  if a['fault']:faults+=1
  diff=[k for k in a if a[k]!=b[k]]
  if not a['terminated'] and not b['terminated'] and a['fault'] and a['fault']==b['fault']:
   if 'saved' in diff:
    fault_register_differences.append(dict(n=n,case=c,original=a['saved'],candidate=b['saved'],fault=a['invalid']));diff.remove('saved')
  if not a['terminated'] and not a['fault']:nonterminating.append(dict(n=n,case=c,steps=sa))
  if n%100==0:print('progress',n,'of',len(cases),flush=True)
  if diff:
   failures.append({'n':n,'case':c,'different':diff,'original':a,'candidate':b})
   if len(failures)<=3:print('FAIL',json.dumps(failures[-1]),flush=True)
 result=dict(original_support_addresses=sorted(SUPPORT_ORIGINAL),fault_register_differences=fault_register_differences,nonterminating=nonterminating,cases=len(cases),passed=len(cases)-len(failures),failures=failures,original_instructions=len(origcov),candidate_instructions=len(candcov),original_coverage=sorted(origcov),candidate_coverage=sorted(candcov),fault_cases=faults,instruction_totals=totals,max_instructions=maxsteps,seconds=time.time()-start,entry=entry)
 (OUT/'replay.json').write_text(json.dumps(result,indent=2)+'\n')
 print(json.dumps({k:v for k,v in result.items() if k not in ['failures','original_coverage','candidate_coverage','fault_register_differences','original_support_addresses']}));sys.exit(bool(failures))

```

### `build/research20fc34/additional.py`

```python
import replay,json,time
from pathlib import Path
start=time.time();cases=[]
for pf in [14,15,0xffffffff]:
 for native in [0,1]:
  for mips in [0,1]:cases.append(dict(pf=pf,native=native,mips=mips,levels=2))
for pf in [10,11]:
 for w,h in [(2,1),(6,1),(3,2),(9,2),(1,2),(10,1)]:cases.append(dict(pf=pf,w=w,h=h))
results=[];coverage=set();candidate=set();totals=[0,0]
for c in cases:
 a,ca,sa,_=replay.run(False,c);b,cb,sb,_=replay.run(True,c);coverage|=ca;candidate|=cb;totals[0]+=sa;totals[1]+=sb
 results.append(dict(case=c,original=a,candidate=b,different=[k for k in a if a[k]!=b[k]]))
base=json.loads(Path('build/research20fc34/replay.json').read_text());d=dict(cases=len(cases),passed=sum(not r['different'] for r in results),results=results,combined_original_instructions=len(coverage|set(base['original_coverage'])),combined_candidate_instructions=len(candidate|set(base['candidate_coverage'])),instruction_totals=totals,seconds=time.time()-start)
Path('build/research20fc34/additional-replay.json').write_text(json.dumps(d,indent=2)+'\n');print(json.dumps({k:v for k,v in d.items() if k!='results'}));print([r for r in results if r['different']])

```

### `build/research20fc34/preserve.py`

```python
from pathlib import Path
import subprocess,json,hashlib
from elftools.elf.elffile import ELFFile
root=Path.cwd();base=root.parent/'mario-main54d';out=root/'build/research20fc34'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def signature(p):
 with p.open('rb') as f:
  e=ELFFile(f);sections=list(e.iter_sections());names={i:s.name for i,s in enumerate(sections)}
  allocated=[(s.name,s['sh_type'],s['sh_flags'],s['sh_addralign'],s['sh_entsize'],s['sh_size'],s.data().hex()) for s in sections if s['sh_flags']&2]
  symbols=[]
  for s in e.get_section_by_name('.symtab').iter_symbols():
   if s['st_info']['type']=='STT_FILE':continue
   symbols.append((s.name,s['st_value'],s['st_size'],dict(s['st_info']),dict(s['st_other']),names.get(s['st_shndx'],s['st_shndx'])))
  reloc=[(s.name,s.data().hex()) for s in sections if s['sh_type']=='SHT_REL']
  return dict(allocated=allocated,symbols=symbols,relocations=reloc)
tracked=subprocess.check_output(['git','ls-tree','-r','--name-only','54d39734c4edffeeac012157ab71fb11c40d815b','--','Game','lib','data/config.json','data/ver/eu/map.csv','tools'],text=True).splitlines()
changed=[str(p) for p in tracked if (base/p).is_file() and (not (root/p).is_file() or sha(base/p)!=sha(root/p))]
report=json.loads((base/'build/dot-baseline-54d/report.json').read_text());checked_objects=set(x['object'] for x in report['prior_checks']);objects=sorted(json.loads(p.read_text())['object'] for p in (base/'build/eu/obj').rglob('*.provenance.json') if json.loads(p.read_text())['source'].startswith(('Game/','lib/')))
entries=[]
for p in objects:
 a=signature(base/p);b=signature(root/p)
 entries.append(dict(object=p,allocated_same=a['allocated']==b['allocated'],symbols_same=a['symbols']==b['symbols'],relocations_same=a['relocations']==b['relocations'],baseline_sha256=sha(base/p),candidate_sha256=sha(root/p)))
r=dict(base='54d39734c4edffeeac012157ab71fb11c40d815b',baseline_report_sha256=sha(base/'build/dot-baseline-54d/report.json'),baseline_prior_roots=report['prior_roots'],baseline_actual_checks=len(report['prior_checks']),baseline_failures=sum(x['returncode']!=0 for x in report['prior_checks']),existing_tracked_inputs_compared=len(tracked),baseline_objects_without_prior_roots=sorted(set(objects)-checked_objects),changed_inputs=changed,objects=entries,canonical_axf_allocated_same=signature(base/'build/eu/RE-Pepper.axf')['allocated']==signature(root/'build/eu/RE-Pepper.axf')['allocated'])
(out/'preservation.json').write_text(json.dumps(r,indent=2)+'\n');print(json.dumps({k:v for k,v in r.items() if k!='objects'}));print('objects',len(entries),'bad',[x['object'] for x in entries if not all(x[k] for k in ['allocated_same','symbols_same','relocations_same'])])

```
