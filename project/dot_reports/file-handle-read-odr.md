# FileHandle read: independent translation unit blocked by the class declaration

This is a notes-only result against `54d39734c4edffeeac012157ab71fb11c40d815b`, tree `27f91b81aba3d13f443b8b8d9995e279e82ddf85` (592 blobs). Branch: `dot/file-handle-read-odr`. Investigation began 2026-10-02 at 02:40:28 UTC. The compatibility blocker was reported at 02:41:07 UTC, before any source change or compile. The input hash, declaration comparison and retail disassembly were verified by 02:42:19 UTC.

There is no source proposal, exact claim, NonMatching adoption or new attempted compiler form. The historical best result is 64 bytes with 23 differing bytes under both 791 and 894, as recorded in `project/pro_requests/002D9AB8.md`. It was **not reproduced in this pass**. No checker was run, so there is no new `tools/check.py` output to report. The four earlier forms remain the complete compiler-backed attempt history. No compiler, checker, map, rank, boundary, flag, SDK source or binary was changed.

## Compatibility result

The accepted `lib/al/src/File/seadFileDevice.cpp` has this entire FileHandle definition at line 16:

```cpp
class FileHandle : public HandleBase {
private:
    unsigned char unknown14[32];
    int mReadDivisionSize;
    friend class FileDevice;
};
```

The packet's best-form definition additionally has:

```cpp
public:
    unsigned int read(unsigned char* buffer, unsigned int size);
```

The comparison below proves that this is the sole difference in the **entire declaration prefix**, including all the companion record definitions and forward declarations. A new TU with the token-identical current FileHandle definition cannot define `FileHandle::read`, since no such member is declared. A new TU with the packet definition has a different definition of the same class from the accepted TU. An added nonvirtual method might leave observed storage offsets unchanged, but that does not satisfy the C++ ODR or the requested token-identity condition.

There is only one complete `FileHandle` definition in the current Game/lib source tree. No existing shared header provides the omitted member declaration. A separately named free function with the desired linker name, changed class identity, macro rewriting of the accepted definition, or hand-written alias is not a compliant independent-TU solution. None was implemented.

This lane was authorized to add a TU only while preserving existing class definitions. That condition cannot be met. The appropriate prerequisite is an owner-coordinated declaration change, with the class definition kept identical wherever used. This note neither proposes such a patch nor assumes ownership of the accepted FileDevice source. The existing class representation remains a bounded prefix view; physical inheritance, complete class extent and original return-type spelling are not established by this result.

## Residual diagnosis

Fresh Capstone disassembly of the unchanged 64-byte retail interval agrees with the packet:

```text
002d9ab8: push     {r4, lr}
002d9abc: ldr      ip, [r0, #0x10]
002d9ac0: sub      sp, sp, #8
002d9ac4: cmp      ip, #0
002d9ac8: moveq    r0, #0
002d9acc: beq      #0x2d9af0
002d9ad0: mov      r4, #0
002d9ad4: stm      sp, {r2, r4}
002d9ad8: mov      r3, r1
002d9adc: mov      r2, r0
002d9ae0: add      r1, sp, #4
002d9ae4: mov      r0, ip
002d9ae8: bl       #0x2436f8
002d9aec: ldr      r0, [sp, #4]
002d9af0: add      sp, sp, #8
002d9af4: pop      {r4, pc}
```

The direct call to the accepted `FileDevice::tryRead` root is independently visible. The packet's best-form source already forced this call out of line. Moving the wrapper into a separate TU would naturally retain that boundary, but does not by itself supply a new explanation for disabling the early-return shrink wrapping. The historical discrepancy is the placement of frame creation and the null-device return; instructions from `002D9AD0` onward are reported equal in that packet. This pass has no new evidence that ordinary equivalent control-flow spelling will alter that optimization. No cosmetic variables, dummy lifetimes/stores, volatile/register, attributes, pragmas, padding, flags, assembly or invented callees were tried.

Retail's wrapper returns zero for a null associated device. Otherwise it initializes its count output to zero, calls original tryRead, ignores the callee's status return and returns the count. A future replay must retain the actual tryRead semantics: disabled device, null handle, association rejection and null buffer may return before writing the count; divided reads may return a partial total; failures after earlier chunks report only the previous total; an undivided virtual read may write the supplied count even on failure. Treat virtual I/O endpoints as explicitly supplied endpoint behavior, and preserve the original signed comparison/division behavior. Do not invent recovery on null objects, invalid memory, stack-guard failure or endpoint contract violations.

No differential replay was executed: there is no allowed candidate to compare. No source preservation build, clean link, object definition/weak-provider audit or current 731-root checker gate was executed in this lane. Historical gates are not presented as current verification. These checks remain mandatory for any eventual source proposal after ownership and declaration compatibility are resolved.

## Definition inventory and unchanged scope

The source file defines exactly these five out-of-line functions, all presently O in the frozen map:

| Address | Size | Definition |
|---|---:|---|
| 002229F4 | 96 | FileDevice::tryCloseDirectory |
| 00222A54 | 152 | FileDevice::tryReadDirectory |
| 002436F8 | 264 | FileDevice::tryRead |
| 00243808 | 96 | FileDevice::tryClose |
| 002D99F8 | 132 | FileDevice::tryWrite |

Their total is 740 bytes. The map contains 731 O function rows. FileHandle::read remains U on its unchanged f row, `002D9AB8..002D9AF8`, 64 bytes and no pool. This is a source/map inventory, not an object-level definition census. The notes-only patch creates no strong or weak provider, and cannot add duplicate weak providers. Existing compiler-generated weak-provider multiplicity was not inspected. The source/header patch is empty; NoteObj, FileDevice, headers and all other actual definitions are untouched.

## Input hashes

SHA-256:

```text
EU code.bin                           e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64
retail 002D9AB8..002D9AF8              f8292c47f48e6d18733cfeb67f1eaa84d5006faf689758136022803abd478314
lib/al/src/File/seadFileDevice.cpp     b34e4114823f4bcb4b79ab3bbc36b14b92120d46f4860e2a8ff5deba466dce2d
project/pro_requests/002D9AB8.md       7bce376c94b61c362920448bb3c4e54bf96c9d44d3346ac8b9f7f723bee2233c
current FileHandle record             e9fdbde7466513bb9ff5d394bd2420473d75599a3b4547ca217784cbcbffc27e
packet FileHandle record              0cad103ecf2b035e340662c4424f05e6ac9775c7a8acc42a36d7e877209a9219
current complete declaration prefix   41fa47f2e41bb290f98e9cad943f4ecf1404d04ac11ea1292c9684eaca2e6723
data/ver/eu/map.csv                    434ad589b05ee299e967025dc3bc9ace954e547db7f83911fba2dc72daf8eb90
tools/check.py                        e177e0abe130e645e75c5f0dc24abb19dbe83310de55f1f099ec8fc385e07317
data/config.json                      5d41e617eb5b26374ed60b5383a0940ce0ce7d046344f7f815b7564144822c45
Game/project_globals.h                6c0b7103d61651e77e1a5db8bb1f5bdf509ad3d64c419c4ba22cadaed6a933d1
```

There is no effective compiler input/header closure because no compilation occurred. The listed preinclude header is an unchanged repository input, not a claim that a build consumed it. The approved private binary was read only from the existing mario-dot location; no binary was copied into the report or commit.

## Executed analysis recipes

Worktree creation, from the existing mario-dot checkout:

```sh
git worktree add -b dot/file-handle-read-odr /workspace/scratch/73cdb2c524af/mario-file-handle-read-odr 54d39734c4edffeeac012157ab71fb11c40d815b
```

The following declaration and map/source searches were run against the frozen checkout. AGENTS, DOT_BRIEF, BRIEF (including sections 1–5), Guide, STATE, the newest daily report, the packet and the full FileDevice source were read before worktree creation. The frozen checkout contained no build directory.

```sh
cat AGENTS.md project/DOT_BRIEF.md project/BRIEF.md Guide.md project/STATE.md
cat project/daily/2026-10-02.md project/pro_requests/002D9AB8.md lib/al/src/File/seadFileDevice.cpp
rg -n 'class FileHandle|struct FileHandle|FileHandle::read' Game lib --glob '*.{h,hpp,cpp,cxx}'
git rev-parse HEAD^{tree}
git status --porcelain
sha256sum lib/al/src/File/seadFileDevice.cpp project/pro_requests/002D9AB8.md data/ver/eu/map.csv tools/check.py data/config.json
sha256sum /workspace/scratch/73cdb2c524af/mario-dot/data/ver/eu/code.bin
```

The first complete analysis script was run from the new worktree with `/workspace/scratch/73cdb2c524af/mario-dot/.venv/bin/python`:

```python
from pathlib import Path
import re, hashlib, csv, json, subprocess
from capstone import Cs, CS_ARCH_ARM, CS_MODE_ARM
root=Path('.')
code_path=Path('/workspace/scratch/73cdb2c524af/mario-dot/data/ver/eu/code.bin')
code=code_path.read_bytes()
assert hashlib.sha256(code).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
source_path=root/'lib/al/src/File/seadFileDevice.cpp'
packet_path=root/'project/pro_requests/002D9AB8.md'
source=source_path.read_text(); packet=packet_path.read_text()
packet_cpp=packet.split('```cpp\n',1)[1].split('```',1)[0]
record=lambda t: re.search(r'class FileHandle : public HandleBase \{.*?\n\};',t,re.S).group(0)
tokenize=lambda t: re.findall(r'[A-Za-z_]\w*|\d+|::|[^\s]',t)
current_record=record(source); packet_record=record(packet_cpp)
assert 'read(' not in current_record
assert 'read(' in packet_record
assert tokenize(current_record)!=tokenize(packet_record)
assert packet_record.replace('public:\n    unsigned int read(unsigned char* buffer, unsigned int size);\n','')==current_record
rows=list(csv.reader((root/'data/ver/eu/map.csv').open()))
family=[r for r in rows if len(r)>6 and r[6].strip() in ['_ZN4sead10FileDevice17tryCloseDirectoryEPNS_15DirectoryHandleE','_ZN4sead10FileDevice16tryReadDirectoryEPjPNS_15DirectoryHandleEPNS_14DirectoryEntryEj','_ZN4sead10FileDevice7tryReadEPjPNS_10FileHandleEPhj','_ZN4sead10FileDevice8tryCloseEPNS_10FileHandleE','_ZN4sead10FileDevice8tryWriteEPjPNS_10FileHandleEPKhj','_ZN4sead10FileHandle4readEPhj']]
print('source sha256',hashlib.sha256(source_path.read_bytes()).hexdigest())
print('packet sha256',hashlib.sha256(packet_path.read_bytes()).hexdigest())
print('current FileHandle record sha256',hashlib.sha256(current_record.encode()).hexdigest())
print('packet FileHandle record sha256',hashlib.sha256(packet_record.encode()).hexdigest())
print('record token identity',tokenize(current_record)==tokenize(packet_record))
print('only record difference is public read declaration',True)
print('source definitions',re.findall(r'^(?:__attribute__\(\(noinline\)\) )?bool FileDevice::(\w+)\(',source,re.M))
print('accepted O function rows',sum(len(r)>6 and r[4].strip()=='O' and r[5].strip().startswith('f') for r in rows))
print('family rows',json.dumps(family))
start=0x2d9ab8; end=0x2d9af8; base=0x100000
retail=code[start-base:end-base]
print('retail function sha256',hashlib.sha256(retail).hexdigest())
for i in Cs(CS_ARCH_ARM,CS_MODE_ARM).disasm(retail,start): print(f'{i.address:08x}: {i.mnemonic:<8} {i.op_str}')
print('base blob count',sum(line.split()[1]=='blob' for line in subprocess.check_output(['git','ls-tree','-r','HEAD'],text=True).splitlines()))
```

The second script confirmed the complete prefix identity after removal of only the two added member-declaration lines. It also hashed the remaining instructions/preinclude input:

```python
from pathlib import Path
import hashlib, re
s=Path('lib/al/src/File/seadFileDevice.cpp').read_text()
p=Path('project/pro_requests/002D9AB8.md').read_text().split('```cpp\n',1)[1].split('```',1)[0]
s=s.split('bool FileDevice::tryCloseDirectory',1)[0]
p=p.split('bool FileDevice::tryCloseDirectory',1)[0]
removed='public:\n    unsigned int read(unsigned char* buffer, unsigned int size);\n'
assert p.replace(removed,'')==s
print('entire declaration prefix differs only by public read declaration: True')
print('current prefix SHA256',hashlib.sha256(s.encode()).hexdigest())
for path in ['Game/project_globals.h','AGENTS.md','project/DOT_BRIEF.md','project/BRIEF.md','Guide.md','project/STATE.md']:
 data=Path(path).read_bytes(); print(path,hashlib.sha256(data).hexdigest())
```

Both scripts exited 0. No C++ file was created. The only failed exploratory hash request used nonexistent root-level `project_globals.h`; the actual file was located and hashed at `Game/project_globals.h`. An exploratory search also confirmed that this snapshot has no `data/ver/eu/config.json`. Neither failure was a build failure or an attempt.

Closure: blocked before reproduction, zero new forms, zero compiles, zero exact bytes and an empty source/header patch. The independent-TU hypothesis offers no demonstrated shrink-wrapping mechanism beyond the already observed out-of-line call. Stop this pass rather than spend the new four-form budget on an ODR-invalid starting point.

Precommit verification at 2026-10-02T02:45:09Z confirmed unchanged FileDevice, map and checker hashes above, and an empty `git diff --name-only 54d39734c4edffeeac012157ab71fb11c40d815b -- Game lib data tools project/STATE.md project/ledger.csv`. The investigation/report window to this check was 4 minutes 41 seconds; it includes reading and report preparation, with zero build/check time. Publication is left to the coordinator. Parent separately confirmed at this point that FileHandle and NoteObj source changes remain held pending an existing ownership question; this lane raised no new user clarification.

Coordination update, 2026-10-02 02:47 UTC: the supervisor confirmed that the main run’s large_library_matching lane actively owns seadFileDevice.cpp and ArchiveFileDevice callbacks, while large_actor_matching actively owns NoteObj.cpp and NoteObj.h. The ownership question is resolved. FileHandle declaration/source and NoteObj source changes remain held until STATE explicitly releases those families. This report contains no source proposal and does not request a new ownership decision.
