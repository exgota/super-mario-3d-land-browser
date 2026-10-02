# ExecuteRequestKeeper residual pass against 8991

This is a partial source proposal, not a matching or fully functionally equivalent implementation. The faithful Pro form 2 reproduces its 220-byte extent and 24 differing bytes. Four bounded new forms leave the simplest guarded bottom-tested cancellation form at 220 bytes and 21 differing bytes. The normal project checker reports `U -> m`; main's original `U` row is restored exactly. Zero new accepted roots, zero accepted bytes, and zero accepted bytes/hour.

Frozen base: `8991bec8cb27ff1a2dac0003f059d270a0b17493`. Branch: `dot/request-keeper-residual-8991`. Final C++ source commit: `57a5c9ed97d94e0f42252e1374093aceebc2f287`. This family owns only `lib/al/include/Execute/alExecuteRequestKeeper.h` and the new `lib/al/src/Execute/alExecuteRequestKeeper.cpp`. No common Execute, sead, LiveActor, ActorExecuteInfo, tools, config, ledger, STATE, or canonical map changes belong to the patch. The original packet and Pro response are preserved unchanged.

## Reproduction and four-form cap

Pro form 2 was extracted verbatim from the response's second C++ block, preserving every C++ token. Its prescribed header changes the existing opaque 16 bytes into four queue pointers. The source was committed before the unchanged normal project build and actual `tools/check.py --object` run. This reproduces the 24-byte result on the fresh base; it does not rely on the old 667-root gate.

| Form | Commit | Grounded hypothesis | Bytes / differences |
|---|---|---|---|
| Faithful Pro form 2 | `d7cc8a5bb71f` | Reproduce representation-copy slot proxy before changing it | 220 / 24 |
| New 1 | `e29f040b654b` | Combine slot comparison and erase in an inline cancellation-step helper; test whether its live ranges survive differently | 220 / 24 |
| New 2, retained | `ec989ef77f53` | Use the target's explicit positive-count guard and bottom-tested loop; keep the count snapshot and slot proxy | 220 / 21 |
| New 3 | `25ef67666e90` | Keep append's slot temporary within its helper instead of returning an aggregate proxy | 220 / 21 |
| New 4 | `1aee8585ee5c` | Put cancellation, membership and append behind one selected-queue operation boundary | 220 / 21 |

Forms 0 and 1 generate identical complete sections. Forms 3 and 4 also generate identical sections; neither improves the 21-byte count. The pass stops at four unsuccessful new forms. The final source restores new form 2 exactly; the restoration is not a fifth form. No volatile, register request, padding, dummy operation, allocator argument, pragma, compiler flag, helper address, modified object or target bytes were introduced.

Every form was compiled through the normal project build from committed C++; every actual checker run rejected exactness with `U -> m`. Every check's map effects were restored in a `finally` block. All five objects contain exactly one function definition, the requested 220-byte root, and zero relocation sections for it. All helper and memcpy operations disappear completely. No helper address or source-closure exception is required.

ARMCC 4.1/791 is the authoritative compiler. The shared approved toolchain has no 894 executable; the attempted diagnostic did not compile and contributes no result. The coordinator explicitly directed that no 894 installation or diagnostic was needed. Global and per-file flags remain unchanged.

## Final canonical result and preservation

An unchanged `python tools/acceptance_batch.py` run used an empty candidate list, so it served solely as the complete preservation gate. Its clean `python make.py eu -ca` build returned 0. The actual checker preserves all 717 prior roots and all 736 canonical definitions with zero failures in 542.918150 seconds including the clean build. An empty-candidate `accepted` flag in that diagnostic means only that preservation passed; it adds no roots or bytes.

After that gate, the final root was checked again from the rebuilt canonical object. Output:

```text
U -> m: The linked candidate differs from the unchanged original interval.
```

The unchanged exact-check routine confirms the same 21 differing bytes. Final extent is `[0x00252B1C, 0x00252BF8)`, 220 bytes, no pool. The map row already has `_ZN2al20ExecuteRequestKeeper7requestEPNS_9LiveActorEi` on the base, so no identity edit is proposed. The independent caller/constructor evidence remains in the original packet and execute-director report. The function remains `U` on this branch after restoring the canonical map; no boundary, type, section, name, or rank is committed.

The source-only family patch applies cleanly to the exact frozen base. It contains the complete root source and keeper header. The full patch additionally contains this report. Neither includes binary artifacts or oracle files.

## Residual classification

The slot address is formed before the actor comparison; the nonmatch, match-bound and post-replacement count reloads survive; the stored decrement supplies the loop bound; membership is rolled; and append stores the incremented count before loading the append buffer. The remaining differences are register allocation, the extra saved r6, and a predicated initial index zeroing. The selected queue remains in ip, opposing buffer in r5, and selected count in r5, where the target uses r5, ip, and ip respectively. This is not a byte-exact function.

```text
00252b1c ! push {r4, r5}                            | push {r4, r5, r6}
00252b20 = cmp r2, #0                               | cmp r2, #0
00252b24 = mov r3, #0                               | mov r3, #0
00252b28 ! ldr r5, [r0, r2, lsl #2]                 | ldr ip, [r0, r2, lsl #2]
00252b2c = ldreq r3, [r0, #4]                       | ldreq r3, [r0, #4]
00252b30 = beq #0x252b54                            | beq #0x252b54
00252b34 = cmp r2, #1                               | cmp r2, #1
00252b38 = ldreq r3, [r0]                           | ldreq r3, [r0]
00252b3c = beq #0x252b54                            | beq #0x252b54
00252b40 = cmp r2, #2                               | cmp r2, #2
00252b44 = ldreq r3, [r0, #0xc]                     | ldreq r3, [r0, #0xc]
00252b48 = beq #0x252b54                            | beq #0x252b54
00252b4c = cmp r2, #3                               | cmp r2, #3
00252b50 = ldreq r3, [r0, #8]                       | ldreq r3, [r0, #8]
00252b54 = ldr r0, [r3, #4]                         | ldr r0, [r3, #4]
00252b58 = cmp r0, #0                               | cmp r0, #0
00252b5c ! mov r0, #0                               | movgt r0, #0
00252b60 = ble #0x252bac                            | ble #0x252bac
00252b64 ! ldr ip, [r3, #8]                         | ldr r5, [r3, #8]
00252b68 ! add r2, ip, r0, lsl #2                   | add r2, r5, r0, lsl #2
00252b6c = ldr r4, [r2]                             | ldr r4, [r2]
00252b70 = cmp r4, r1                               | cmp r4, r1
00252b74 = ldrne r2, [r3, #4]                       | ldrne r2, [r3, #4]
00252b78 = bne #0x252ba0                            | bne #0x252ba0
00252b7c = ldr r4, [r3, #4]                         | ldr r4, [r3, #4]
00252b80 = cmp r4, r0                               | cmp r4, r0
00252b84 = ble #0x252b94                            | ble #0x252b94
00252b88 = sub r4, r4, #1                           | sub r4, r4, #1
00252b8c ! ldr ip, [ip, r4, lsl #2]                 | ldr r4, [r5, r4, lsl #2]
00252b90 ! str ip, [r2]                             | str r4, [r2]
00252b94 = ldr r2, [r3, #4]                         | ldr r2, [r3, #4]
00252b98 = sub r2, r2, #1                           | sub r2, r2, #1
00252b9c = str r2, [r3, #4]                         | str r2, [r3, #4]
00252ba0 = add r0, r0, #1                           | add r0, r0, #1
00252ba4 = cmp r2, r0                               | cmp r2, r0
00252ba8 = bgt #0x252b64                            | bgt #0x252b64
00252bac ! ldr ip, [r5, #4]                         | ldr r5, [ip, #4]
00252bb0 = mov r0, #0                               | mov r0, #0
00252bb4 ! cmp ip, #0                               | cmp r5, #0
00252bb8 ! ldrgt r3, [r5, #8]                       | ldrgt r2, [ip, #8]
00252bbc = ble #0x252bd8                            | ble #0x252bd8
00252bc0 ! ldr r2, [r3, r0, lsl #2]                 | ldr r4, [r2, r0, lsl #2]
00252bc4 ! cmp r2, r1                               | cmp r4, r1
00252bc8 = beq #0x252bf0                            | beq #0x252bf0
00252bcc = add r0, r0, #1                           | add r0, r0, #1
00252bd0 ! cmp ip, r0                               | cmp r5, r0
00252bd4 = bgt #0x252bc0                            | bgt #0x252bc0
00252bd8 ! add r0, ip, #1                           | add r0, r5, #1
00252bdc ! str r0, [r5, #4]                         | str r0, [ip, #4]
00252be0 ! ldr r3, [r5, #8]                         | ldr r3, [ip, #8]
00252be4 = mvn r2, #3                               | mvn r2, #3
00252be8 = add r0, r2, r0, lsl #2                   | add r0, r2, r0, lsl #2
00252bec = str r1, [r3, r0]                         | str r1, [r3, r0]
00252bf0 ! pop {r4, r5}                             | pop {r4, r5, r6}
00252bf4 = bx lr                                    | bx lr
```

## Fresh layout and type-use audit

The audit compiles the original keeper header and the final header under the same 791 command, with a diagnostic-only private-access exposure to measure existing fields. It rechecks current tracked Game/lib type uses and hashes all seven files containing the keeper spelling. The fourteen matched lines include two reconstruction-namespace labels and two lines in the new implementation; they are not fourteen independent object placements. No current by-value keeper embedding or array of keeper objects exists in the searched sources.

| Layout | Original | Final |
|---|---|---|
| ExecuteRequestKeeper size / alignment | 16 / 1 | 16 / 4 |
| ExecuteDirector size / alignment / keeper offset | 28 / 4 / 16 | 28 / 4 / 16 |
| ActorExecuteInfo size / alignment / keeper offset / draw offset | 28 / 4 / 0 / 24 | 28 / 4 / 0 / 24 |

Original director initialization at `0x001CCC20` requests a 16-byte allocation and stores the resulting keeper at director +0x10. The keeper constructor at `0x001DDAC8` allocates 12-byte queues, stores count and buffer at +4/+8, and stores exactly four queue pointers at keeper +4*i. The original actor-info constructor at `0x001CDCFC` stores its keeper argument at offset 0 and initializes through +0x18. These independent original stores/allocation sizes agree with the current pointer-owning layouts.

This is a bounded current-source and current-root audit. Raising alignment from 1 to 4 is a real ABI change. The result does not authorize an unknown or future by-value enclosure, packed container, or placement into byte-aligned storage. Full keeper/queue ownership and all unimplemented type uses remain unrecovered.

## Whole-root replay and explicit limits

Two isolated Unicorn ARM machines execute the complete original root and the complete compiler-generated root respectively. They use the full verified original image; the candidate machine substitutes only the emitted 220-byte section in memory. The original file is never modified. The root contains no calls, so no callee is stubbed or replaced. Inputs, data memory, writes, returned/fault result and fault address are compared; on successful returns, preserved registers and SP are compared as well. Intermediate register values on a fault are excluded because different register allocation is the known residual.

The 0.700224-second bounded run agrees in 2,484 cases: 2,116 ordinary selected/opposing list combinations over four request kinds, 92 null-actor cases, 92 same-queue aliases, 92 shared-buffer aliases, 12 overlapping-buffer cases, 8 queue-field aliases, 24 negative-count cases, 16 ignored-capacity cases, 8 buffer-boundary faults, 20 other null/malformed faults, and 4 invalid request kinds. The source's cancellation intentionally advances after swapping the last entry: an opposing `[A,A]` becomes logical count 1 with an A still present, then the selected queue receives A. It does not recheck the replacement. Capacity 0, 1, -1 and INT_MAX do not introduce a capacity check. Faulting append cases store the incremented count before the invalid buffer store, agreeing with retail. No recovery or validation is added.

One additional stack-guard control intentionally disagrees. With aligned SP at the first mapped stack page +8, retail's 8-byte save succeeds and returns; this candidate's 12-byte save faults at page -4 before processing queues. Therefore the replay is 2,484 agreements plus one known divergence, not a complete equivalence proof or a functional NonMatching acceptance. Malformed/alias controls describe these emitted ARM instructions; they do not establish defined ISO C++ behavior for every invalid object or alias.

An early harness run wrote banked SP before switching CPSR from Unicorn's initial mode. That made its first case fault at address -8/-12. The setup was corrected to select user mode before initializing registers, and all cases were rerun. The failed harness evidence is retained locally as `harness-bank-setup-replay-result.json`; no source change was made to hide it.

## Source and evidence identities

Final source SHA256: `9dbb5028c4cf9ecd7399b72e999ea50f294def0d7e1ccad1ea10db3e387a7037`.

Final keeper-header SHA256: `757f1877438b57eb28d311f38020ed319544fbc58569d15981c89bc53b737533`.

Final compiler section SHA256: `80f9785edd063668fcb6aa82915c58eb5fb7979487e15e8b2d8d8db463612913`. Final canonical object SHA256: `d89fa68b2033ff6d562c3cac0bbeba432acbd9a5f3ede780c7a854ef9e07b072`.

Verified EU executable SHA256: `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`. Original map SHA256, restored exactly: `2dde00f86cc0bc6f174f3a281853a4faacea1f16b6b628d602bea687b4cdb805`.

Original packet SHA256: `4ea0cc511067e044fd0c99f0ff54aca17794a8329cfb76019db016f0bb84af73`. Original response SHA256: `43aeb186fe8a787a10db07d35af5ccbac2433302c36f623019e860a2fa7b3732`.

The work interval is 2026-10-02T01:53:40+00:00 through 2026-10-02T02:10:21.500963+00:00, 1001.500963 wall seconds. The start is the assignment's UTC clock observation; this includes reading, attempts, build, audit, replay, preservation and packaging. The gate and replay durations above are directly instrumented. Individual commit-to-grade timestamps below are provenance markers, not exclusive compile durations. Zero new accepted bytes over this entire interval is zero accepted bytes/hour.

| Form | Grade start UTC | Exact source SHA256 |
|---|---|---|
| 0 | 2026-10-02T01:56:38.985994+00:00 | `486ef6e09ffed91967033c9765ea7e9951958cc52d2731cf2665295a9aece2e7` |
| 1 | 2026-10-02T01:57:35.735037+00:00 | `078353e7d15d3d93c8548ffc8a62f31a12036eaf62cf5ee659694096d7f206df` |
| 2 | 2026-10-02T01:58:27.567244+00:00 | `9dbb5028c4cf9ecd7399b72e999ea50f294def0d7e1ccad1ea10db3e387a7037` |
| 3 | 2026-10-02T01:58:58.129533+00:00 | `50450b1933f5ada515a1f06b8503b03fa798a64f9b9ac48cc3c0624a64a9afd5` |
| 4 | 2026-10-02T02:00:13.834137+00:00 | `a712dcb0d48c1b28c2d16dc4ae138d477bb0c1b9ff7c35bd12e5a0843401437e` |

Ignored evidence root: `build/keeper_residual/`. These are local reproducibility aids; only source/header/notes are committed.

| Artifact | SHA256 |
|---|---|
| `source-only.patch` | `4ed8172860fb97a84fcdd5108272b2aaebdc2922a2a498f0db742dcc0d1c5f63` |
| `preservation/report.json` | `67793dd4905c4b675491685a3b6c84ea6f6764bd96b94990706f9eee2f9b92ee` |
| `preservation/clean_build.log` | `5d1ea65d7ed1158c77c1faea51a9c57a6a0c295a3cb4610d2d429ef86ce74a1f` |
| `preservation-manifest.json` | `78a259bc8f9dd68caeaae71442297e1eb0631ca5aaac5688b3cfe8d592a8328b` |
| `final/result.json` | `3cea8637c20835b82ba8bb60696f385f0e8a4dd383588c8d4af1e58476417f91` |
| `final/diff.txt` | `3b23b79a8f320041d8f7f87ccc582c984f08040d8a1e9d94cdc22022b6e603fe` |
| `final/alExecuteRequestKeeper.provenance.json` | `1609b5e5e1e854a567d43f6fd780facb2204b90dce17ddc4215284477692297e` |
| `layout/result.json` | `f4419b7d8ddabb6d685d1cebb1b5d489009f04fd17cfbaddcf3389e7b2028efa` |
| `replay-result.json` | `49d2f1addbd9e40b93e9c513829839bb86dfbe68853ad0b139df8dccae0014b2` |
| `grade_form.py` | `9c32f8cd74c3d8018d5f2dd13e54483a27d8b471147384eb498d142c0a16c020` |
| `audit_layout.py` | `c255dde5d08b9f23a5a4504b0af9dfeaab730a76158c0e797b5bd3c180a5a98d` |
| `replay.py` | `2edff28a231a38332a0c7eacbda56019c021bc2d5d9a646d09651864571fa3fc` |

## Executed reproduction commands

Use the normal approved private inputs and compiler environment, with the verified EU hash above. In a checkout of the final source commit, place the following three embedded scripts under `build/keeper_residual/`, then execute the commands below. Each script was executed in this pass; none is part of the target build or modifies the oracle. `grade_form.py` calls the unchanged project checker and restores its temporary rank update.

```sh
. ./development_environment.sh
export DEVKITARM=/usr
mkdir -p build/keeper_residual
python make.py eu -ca
python build/keeper_residual/grade_form.py final
python build/keeper_residual/audit_layout.py
python build/keeper_residual/replay.py
```

Preservation manifest and command (use a fresh output directory):

```json
{
  "prior_checkpoint": "8991bec8cb27ff1a2dac0003f059d270a0b17493",
  "candidates": []
}
```

```sh
python tools/acceptance_batch.py build/keeper_residual/preservation-manifest.json --output build/keeper_residual/preservation
```

The recorded pass ran the preservation clean-build gate before the final grade. To reproduce earlier forms, use their committed C++ source/header checkpoints listed above and run the normal build then the grade script; do not stack their source changes.

### `grade_form.py`

```python
import sys,json,hashlib,subprocess,time,io,shutil
from pathlib import Path
from datetime import datetime,timezone
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
sys.path.insert(0,str(Path.cwd()))
from tools.low.buildProvenance import verify_build_output
from tools.low.checkExactBytes import check_exact_bytes
form=sys.argv[1]; out=Path('build/keeper_residual')/form; out.mkdir(exist_ok=True)
sym='_ZN2al20ExecuteRequestKeeper7requestEPNS_9LiveActorEi'
obj=Path('build/eu/obj/lib/al/src/Execute/alExecuteRequestKeeper.o')
source=Path('lib/al/src/Execute/alExecuteRequestKeeper.cpp')
header=Path('lib/al/include/Execute/alExecuteRequestKeeper.h')
result={'form':form,'utc':datetime.now(timezone.utc).isoformat(),'commit':subprocess.check_output(['git','rev-parse','HEAD'],text=True).strip(),'hashes':{str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in [source,header,obj]},'provenance':verify_build_output(obj)}
for p in [source,header,obj,obj.with_suffix('.provenance.json')]:shutil.copy2(p,out/p.name)
old=Path('data/ver/eu/map.csv').read_bytes()
start=time.monotonic()
try:
 r=subprocess.run([sys.executable,'tools/check.py',sym,'--object',str(obj)],capture_output=True,text=True)
 result['checker']={'command':r.args,'returncode':r.returncode,'stdout':r.stdout,'stderr':r.stderr,'seconds':time.monotonic()-start}
finally:Path('data/ver/eu/map.csv').write_bytes(old)
result['canonical']=check_exact_bytes(sym,obj,output_directory=out/'exact')
elf=ELFFile(io.BytesIO(obj.read_bytes()));sec=elf.get_section_by_name('i.'+sym);current=sec.data()
result['definitions']=[{'name':s.name,'size':s['st_size'],'section':s['st_shndx']} for s in elf.get_section_by_name('.symtab').iter_symbols() if s['st_info']['type']=='STT_FUNC' and isinstance(s['st_shndx'],int)]
result['relocations']=[s.name for s in elf.iter_sections() if s['sh_type'] in ('SHT_REL','SHT_RELA') and s['sh_info']==elf.get_section_index(sec.name)]
original=Path('data/ver/eu/code.bin').read_bytes()[0x152b1c:0x152bf8]
result['raw']={'size':len(current),'different_bytes':sum(a!=b for a,b in zip(current,original))+abs(len(current)-len(original)),'section_sha256':hashlib.sha256(current).hexdigest()}
cs=Cs(CS_ARCH_ARM,CS_MODE_ARM)
a=list(cs.disasm(original,0x252b1c));b=list(cs.disasm(current,0x252b1c))
lines=[]
for i in range(max(len(a),len(b))):
 x=a[i] if i<len(a) else None;y=b[i] if i<len(b) else None
 left=(f'{x.mnemonic} {x.op_str}') if x else '';right=(f'{y.mnemonic} {y.op_str}') if y else ''
 equal=x and y and x.bytes==y.bytes
 lines.append(f'{0x252b1c+4*i:08x} {"=" if equal else "!"} {left:40s} | {right}')
(out/'diff.txt').write_text('\n'.join(lines)+'\n')
(out/'result.json').write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps({k:result[k] for k in ['form','commit','raw','definitions','relocations','checker']},indent=2))
print('\n'.join(x for x in lines if ' ! ' in x))
```

### `audit_layout.py`

```python
import subprocess,json,hashlib,struct,io,os
from pathlib import Path
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
root=Path.cwd();out=root/'build/keeper_residual/layout';out.mkdir(exist_ok=True)
base='8991bec8cb27ff1a2dac0003f059d270a0b17493';keeper=Path('lib/al/include/Execute/alExecuteRequestKeeper.h')
base_header=subprocess.check_output(['git','show',base+':'+str(keeper)])
old=out/'original/Execute/alExecuteRequestKeeper.h';old.parent.mkdir(parents=True,exist_ok=True);old.write_bytes(base_header)
probe=out/'layout.cpp';probe.write_text('''#define private public
#include <Execute/alExecuteRequestKeeper.h>
#include <Execute/alExecuteDirector.h>
#include <LiveActor/alActorExecuteInfo.h>
#include <stddef.h>
extern "C" unsigned keeper_layout[] = {
sizeof(al::ExecuteRequestKeeper), __alignof__(al::ExecuteRequestKeeper),
sizeof(al::ExecuteDirector), __alignof__(al::ExecuteDirector), offsetof(al::ExecuteDirector,mRequestKeeper),
sizeof(al::ActorExecuteInfo), __alignof__(al::ActorExecuteInfo), offsetof(al::ActorExecuteInfo,mRequestKeeper), offsetof(al::ActorExecuteInfo,mPointerAtOffset18)
};
''')
prov=json.loads(Path('build/eu/obj/lib/al/src/Execute/alExecuteRequestKeeper.provenance.json').read_text());base_cmd=prov['command'];summary={'base':base,'source_commit':subprocess.check_output(['git','rev-parse','HEAD'],text=True).strip(),'layouts':{},'hashes':{},'uses':[]}
for variant in ['original','current']:
 cmd=base_cmd.copy();cmd[-1]=str(probe);cmd[cmd.index('-o')+1]=str(out/(variant+'.o'));cmd[cmd.index('--depend')+1]=str(out/(variant+'.d'))
 if variant=='original':cmd.insert(2,'-I'+str(out/'original'))
 env=os.environ.copy();env.update(ARMCC41INC=str(root/'data/compilers/4.1/791/include'),ARMCC41LIB=str(root/'data/compilers/4.1/791/lib'),TMP='/tmp')
 r=subprocess.run(cmd,capture_output=True,text=True,env=env);(out/(variant+'.log')).write_text(r.stdout+r.stderr)
 summary['layouts'][variant]={'command':cmd,'returncode':r.returncode}
 if r.returncode:raise RuntimeError(r.stdout+r.stderr)
 elf=ELFFile(io.BytesIO((out/(variant+'.o')).read_bytes()));sym=elf.get_section_by_name('.symtab').get_symbol_by_name('keeper_layout')[0]
 vals=struct.unpack('<9I',elf.get_section(sym['st_shndx']).data()[sym['st_value']:sym['st_value']+36]);summary['layouts'][variant]['values']=dict(zip(['keeper_size','keeper_alignment','director_size','director_alignment','director_keeper_offset','info_size','info_alignment','info_keeper_offset','info_draw_offset'],vals))
files=subprocess.check_output(['git','ls-files','Game','lib'],text=True).splitlines()
for name in files:
 p=Path(name)
 if not p.is_file():continue
 data=p.read_bytes()
 if b'ExecuteRequestKeeper' not in data:continue
 summary['hashes'][name]=hashlib.sha256(data).hexdigest()
 for i,line in enumerate(data.decode('cp932',errors='replace').splitlines(),1):
  if 'ExecuteRequestKeeper' in line:summary['uses'].append({'path':name,'line':i,'text':line})
code=Path('data/ver/eu/code.bin').read_bytes();summary['executable_sha256']=hashlib.sha256(code).hexdigest();cs=Cs(CS_ARCH_ARM,CS_MODE_ARM);summary['retail']={}
for label,start,end in [('keeper_allocation',0x1ccc1c,0x1ccc3c),('queue_constructor',0x1ddac8,0x1ddb40),('info_stores',0x1cdcfc,0x1cdd1c),('request_fields',0x252b1c,0x252bf8)]:
 data=code[start-0x100000:end-0x100000];summary['retail'][label]={'start':hex(start),'end':hex(end),'sha256':hashlib.sha256(data).hexdigest(),'assembly':[f'{x.address:08x}: {x.mnemonic} {x.op_str}' for x in cs.disasm(data,start)]}
summary['bounded_conclusion']='Keeper extent stays 16 bytes while alignment rises 1 to 4. Current pointer-owning enclosing types stay size 28/alignment 4 with keeper offsets 16 (director) and 0 (actor info); no current by-value embedding or array of keeper objects found. Original allocator call requests 16 bytes and constructor stores four aligned words. No claim covers unknown or future type uses.'
(out/'result.json').write_text(json.dumps(summary,indent=2)+'\n');print(json.dumps({'layouts':summary['layouts'],'use_lines':len(summary['uses']),'owning_files':len(summary['hashes'])},indent=2))
```

### `replay.py`

```python
import io,json,struct,hashlib,itertools,random,time
from pathlib import Path
from datetime import datetime,timezone
from elftools.elf.elffile import ELFFile
from unicorn import Uc,UcError,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_MEM_INVALID,UC_HOOK_MEM_WRITE
from unicorn.arm_const import *
P=Path('build/keeper_residual');START=0x252b1c;STOP=0x3000000;DATA=0x1000000;STACK=0x2000000;SIZE=0x4000;A=0x11111110;B=0x22222220
code=Path('data/ver/eu/code.bin').read_bytes();obj=Path('build/eu/obj/lib/al/src/Execute/alExecuteRequestKeeper.o').read_bytes();elf=ELFFile(io.BytesIO(obj));compiled=elf.get_section_by_name('i._ZN2al20ExecuteRequestKeeper7requestEPNS_9LiveActorEi').data()
assert len(compiled)==220
start=time.monotonic();report={'started_utc':datetime.now(timezone.utc).isoformat(),'code_sha256':hashlib.sha256(code).hexdigest(),'object_sha256':hashlib.sha256(obj).hexdigest(),'compiled_section_sha256':hashlib.sha256(compiled).hexdigest(),'whole_root_start':hex(START),'original_callees':'none: the complete 55-instruction root has no calls','results':[]}
class Machine:
 def __init__(self,candidate):
  self.u=Uc(UC_ARCH_ARM,UC_MODE_ARM);self.u.mem_map(0x100000,(len(code)+4095)&~4095);self.u.mem_write(0x100000,code)
  if candidate:self.u.mem_write(START,compiled)
  self.u.mem_map(DATA,SIZE);self.u.mem_map(STACK,0x1000);self.u.mem_map(STOP,0x1000)
  self.u.hook_add(UC_HOOK_MEM_INVALID,self.invalid);self.u.hook_add(UC_HOOK_MEM_WRITE,self.write)
 def invalid(self,u,access,address,size,value,user):
  self.fault={'access':access,'address':address,'size':size};return False
 def write(self,u,access,address,size,value,user):
  if not STACK<=address<STACK+0x1000:self.writes.append([address,size,value&((1<<(size*8))-1)])
 def run(self,memory,kind,actor,sp):
  self.u.mem_write(DATA,memory);self.u.mem_write(STACK,bytes(0x1000));self.fault=None;self.writes=[];self.u.reg_write(UC_ARM_REG_CPSR,0x10)
  regs=[UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3,UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11,UC_ARM_REG_R12]
  for i,r in enumerate(regs):self.u.reg_write(r,0x80808080+i)
  self.u.reg_write(UC_ARM_REG_R0,DATA);self.u.reg_write(UC_ARM_REG_R1,actor);self.u.reg_write(UC_ARM_REG_R2,kind&0xffffffff);self.u.reg_write(UC_ARM_REG_SP,sp);self.u.reg_write(UC_ARM_REG_LR,STOP);self.u.reg_write(UC_ARM_REG_CPSR,0x10)
  error=None
  try:self.u.emu_start(START,STOP,count=10000)
  except UcError as e:error=e.errno
  pc=self.u.reg_read(UC_ARM_REG_PC);end=bytes(self.u.mem_read(DATA,SIZE));preserved=[self.u.reg_read(r) for r in regs[4:12]]
  return {'returned':pc==STOP,'fault':self.fault,'error':error,'writes':self.writes.copy(),'data_sha256':hashlib.sha256(end).hexdigest(),'preserved':preserved,'sp':self.u.reg_read(UC_ARM_REG_SP)},end
original=Machine(False);candidate=Machine(True)
def fixture(kind,selected,opposing,actor=A,capacity=16):
 mem=bytearray([0xcc])*SIZE;queues=[DATA+0x100+i*0x20 for i in range(4)];buffers=[DATA+0x1000+i*0x400 for i in range(4)]
 def put(address,value):struct.pack_into('<I',mem,address-DATA,value&0xffffffff)
 for i in range(4):
  put(DATA+4*i,queues[i]);put(queues[i],capacity);put(queues[i]+4,0);put(queues[i]+8,buffers[i])
  for j in range(32):put(buffers[i]+4*j,0xfeed0000+j)
 if 0<=kind<4:
  for index,vals in [(kind,selected),(kind^1,opposing)]:
   put(queues[index]+4,len(vals))
   for j,v in enumerate(vals):put(buffers[index]+4*j,v)
 return mem,queues,buffers,put

def check(label,domain,kind,selected=(),opposing=(),actor=A,mutate=None,sp=STACK+0x800,capacity=16,expected_mismatch=False):
 mem,queues,buffers,put=fixture(kind,selected,opposing,actor,capacity)
 if mutate:mutate(queues,buffers,put)
 a,ma=original.run(bytes(mem),kind,actor,sp);b,mb=candidate.run(bytes(mem),kind,actor,sp)
 keys = ['returned','fault','error','writes','data_sha256']
 if a['returned'] and b['returned']: keys += ['preserved','sp']
 same=all(a[key]==b[key] for key in keys) and ma==mb
 entry={'label':label,'domain':domain,'request_type':kind,'actor':actor,'selected_input':list(selected),'opposing_input':list(opposing),'equivalent':same,'expected_mismatch':expected_mismatch,'retail':a,'candidate':b}
 report['results'].append(entry)
 if not same:print('DIFFERENCE',label,a,b,flush=True)
 return a,ma

lists=[(),(A,),(B,),(0,),(A,A),(A,B),(B,A),(B,B),(0,A),(A,0),(A,A,A),(A,B,A),(B,A,A),(A,A,B),(A,B,B),(B,A,B),(B,B,A),(0,0,0),(A,A,A,A),(B,A,B,A),(A,B,A,B),(A,A,B,A),(B,B,B,B)]
for k in range(4):
 for i,s in enumerate(lists):
  for j,o in enumerate(lists):check(f'ordinary-{k}-{i}-{j}','ordinary',k,s,o)
 for j,o in enumerate(lists):check(f'null-actor-{k}-{j}','null_actor',k,(0,B),o,actor=0)
 for i,vals in enumerate(lists):
  check(f'same-queue-{k}-{i}','same_queue',k,vals,vals,mutate=lambda q,b,p,k=k:p(DATA+4*k,q[k^1]))
  check(f'shared-buffer-{k}-{i}','shared_buffer',k,vals,vals,mutate=lambda q,b,p,k=k:p(q[k]+8,b[k^1]))
 for cap in [0,1,-1,0x7fffffff]:check(f'ignored-capacity-{k}-{cap}','capacity_ignored',k,(B,B),(A,B,A),capacity=cap)
 for delta in [4,8,12]:check(f'overlap-buffer-{k}-{delta}','overlap_buffer',k,(B,A,B),(A,B,A),mutate=lambda q,b,p,k=k,delta=delta:p(q[k]+8,b[k^1]+delta))
 check(f'count-alias-{k}','field_alias',k,(),(A,),actor=1,mutate=lambda q,b,p,k=k:p(q[k^1]+8,q[k^1]+4))
 check(f'buffer-field-alias-{k}','field_alias',k,(),(A,),actor=DATA+0x108+0x20*(k^1),mutate=lambda q,b,p,k=k:p(q[k^1]+8,q[k^1]+8))
 for n in [-1,-2,-0x80000000]:
  check(f'opposing-negative-{k}-{n}','negative_count',k,(),(),mutate=lambda q,b,p,k=k,n=n:p(q[k^1]+4,n))
  check(f'selected-negative-{k}-{n}','negative_count',k,(),(),mutate=lambda q,b,p,k=k,n=n:p(q[k]+4,n))
 check(f'null-opposing-{k}','fault',k,(),(),mutate=lambda q,b,p,k=k:p(DATA+4*(k^1),0))
 check(f'null-selected-{k}','fault',k,(),(),mutate=lambda q,b,p,k=k:p(DATA+4*k,0))
 check(f'null-append-buffer-{k}','fault',k,(),(),mutate=lambda q,b,p,k=k:p(q[k]+8,0))
 check(f'null-scan-buffer-{k}','fault',k,(),(A,),mutate=lambda q,b,p,k=k:p(q[k^1]+8,0))
 check(f'guard-append-{k}','capacity_fault',k,(),(),mutate=lambda q,b,p,k=k:p(q[k]+8,DATA+SIZE))
 check(f'guard-scan-{k}','capacity_fault',k,(),(A,),mutate=lambda q,b,p,k=k:p(q[k^1]+8,DATA+SIZE))
 check(f'overflow-count-{k}','fault',k,(),(),mutate=lambda q,b,p,k=k:(p(q[k]+4,0x7fffffff),p(q[k]+8,0)))
for k in [-1,4,5,0x40000000]:check(f'invalid-kind-{k}','invalid_request_type',k)
check('stack-edge-extra-save','stack_guard',0,expected_mismatch=True,sp=STACK+8)
report.update(seconds=time.monotonic()-start,finished_utc=datetime.now(timezone.utc).isoformat(),total=len(report['results']),equivalent=sum(x['equivalent'] for x in report['results']),unexpected_differences=[x['label'] for x in report['results'] if x['equivalent']==x['expected_mismatch']])
report['domains']={d:sum(x['domain']==d for x in report['results']) for d in sorted({x['domain'] for x in report['results']})}
(P/'replay-result.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k!='results'},indent=2))
```
