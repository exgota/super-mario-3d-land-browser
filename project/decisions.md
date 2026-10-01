# Decision log

Append only. Newest entries at the bottom.

## 2026-10-01: Setup decisions (made by Claude during environment setup, approved scope from the owner)

**Removed RE-Pepper's CtrSDK, NintendoWare and sead submodules.** Their READMEs say CtrSDK and NintendoWare were "referenced from a debug binary that was taken from a purchased and dumped hard drive, which a still active game studio threw out", and that sead was referenced from a `libpia_pead.a` downloaded from debugging.games. Brief rule 6 forbids leaked material. All 76 map entries upstream had ranked `O`, `M` or `m` were in that code, so they were reset to `U`.

**Toolchain runs natively on macOS through wibo's macOS build.** Running wibo inside an x86_64 Linux container under Rosetta failed with `rosetta error: invalid gdt selector index 4`. decompals/wibo 1.2.0 ships `wibo-macos`, which runs armcc under Rosetta 2. Changes: `tools/low/glob.py` adds `isMacOS()` and `needsWibo()`; `tools/pypstem/callProcess.py` and `tools/pypstem/manSetup.py` use them, and setup downloads `wibo-macos` on macOS.

**Build changes needed after the removal.**
- `lib/CtrSDK` is now a clean module with a hand-written `include/nn/types.h` (the global preinclude needs it).
- `tools/pypstem/stepLink.py` skips modules that produced no library, so header-only modules link.
- `tools/pypstem/stepSplit.py` always imports `__ctr_start` and emits a strong stub for map functions flagged `g`, because the entry point was previously supplied by the removed SDK code.

**Fixed `tools/progress.py`.** Line 73 had a syntax error (`"d" sym[...]` instead of `"d" in sym[...]`).

**Compiler evidence so far.** The SDK module's configured compiler, ARMCC 4.0/902, produced a byte-exact match for `ConvertSvcToLibraryPriority`. This says nothing about game code. Game code's build (4.1/791 versus 4.1/894) is untested.

## 2026-10-01: Require literal-pool bytes before accepting a match

The existing checker invokes asm-differ with `-I -i`, which ignores addresses and large immediates, and disassembles only through the map's pool boundary. An assembly score of zero therefore does not prove the full function interval byte-exact. `tools/check.py` now requires a direct comparison of the unchanged map interval, including the literal pool, before returning `O`. No target bytes, boundaries, differ settings, or progress logic changed.

Validation: the existing SDK function remained `O`. Flipping one byte of its literal pool in the ignored generated build output produced `m`, and restoring the output produced `O` again. The original dump was never modified. Symbols without a valid executable address now return `U` instead of `O`.

## 2026-10-01: Enable source files incrementally

The brief requires enabling Game and lib/al one file at a time. The build previously scanned every source recursively. An optional module `source_files` list now selects paths relative to its source directory, fails for missing configured files, and filters compilation commands. Selection changes invalidate the module archive so removed files cannot survive through cached objects. Existing modules without a list retain recursive discovery. Global compiler flags remain unchanged.

## 2026-10-01: Initial game compiler comparison

Both ARMCC 4.1/791 and 4.1/894 produced the same six byte-exact PlayerTrigger leaf functions using unchanged global flags. The project checker then accepted both clear methods and both set/isOn overloads under 791. The constructor and al::calcHashCode compiled with different register choices from the target under both compiler builds. These probes do not distinguish the compiler and M0 remains open.

The existing Game source remains in backup/src and backup/include; configuration names those directories rather than creating duplicate source paths. Ledger token and wall-time fields for these initial probes are coarse estimates allocating shared analysis across functions. Actual subscription billing is unavailable, so the cost field says unavailable rather than inventing a dollar value.

## 2026-10-01: Restore required source-language declarations

Existing source uses nullptr even though ARMCC 4.1 implements C++03, and uses staticd for explicit symbol sections without defining it. The global preinclude now supplies a zero null-pointer constant and the existing static-data section convention. The clean integer-types header includes standard stdint.h for uintptr_t used by Byaml headers. These repairs change declarations and macro availability, not compiler flags.

## 2026-10-01: Clean header integration and source compile audit

The new lib/sead module contains 14 headers reconstructed from local retail assembly and clean game call sites. Its README records observed offsets and explicitly leaves unknown runtime behavior as declarations. The first isolated source audit compiled 104 of 110 existing Game/lib/al translation units with the project flags.

The six failures required a missing SafeString include, three declaration-only clean SDK call headers, Togezo's undefined sdata macro spelling, and the broken scene-factory source/header. The factory at 0x00166AC8 allocates 12 bytes, passes 23 to SceneObjHolder, and loads callback 0x0015DF30 from its literal at 0x00166AF0. GhostPlayer's constructor passes 20 at 0x0012CC4C; rp::createCoinRotater passes 7 at 0x00276588. Only these needed scene IDs are declared. SceneObjHolder's mapped constructor uses a void* callback return, which its header now reproduces. The factory callback remains an external declaration rather than invented actor-creation behavior.

After these repairs, the audit compiled 110 of 110 translation units under ARMCC 4.1/791. This establishes compilation only, not runtime correctness or matching. The direct compilation audit logs remain ignored in build/source_audit.

An inherited __nop intrinsic deliberately standing in for an instruction in alSensorFunction was removed under hard rule 3. It is no longer a permissible matching aid. Its function remains nonmatching. Searches for __nop, sdata( and SCENE_OBJ_LIST return zero hits in Game and lib/al. The global asm declaration macros remain because they support the brief's low-level SDK exemption.
