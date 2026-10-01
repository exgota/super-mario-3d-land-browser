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
