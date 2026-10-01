# createSceneResourceHeap: bounded differential evidence

The canonical guarded candidate and unchanged retail routine agree on **380 returning pairs** and **30 fault-agreement pairs**, with zero differential failures. These are 41 explicit synthetic fixtures, five FPSCR configurations, and two caller-saved register clobber patterns. Fault agreement is reported separately from successful returns. This is bounded NonMatching evidence, not a byte-exact match or a universal functional claim.

## Frozen inputs and reproduction

- Source checkpoint: `f7572a5`, provided by root after the repository's canonical build and diagnostic.
- Function: `_ZN2al12MemorySystem23createSceneResourceHeapEPKc`, complete range `0x0024F344..0x0024F484`, 320 bytes including literals.
- Original `code.bin` SHA256: `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.
- Original complete function SHA256: `1429504d792a22ffe11bc12d79e001d71119b2651cf95acc05abf8ca298f2ba3`.
- Root candidate: `build/exact_checks/eu/function_0024F344_b7fblpn7/candidate.axf`, SHA256 `c4fc10c2e7e7863aa2104d368e5ee9e2cca6b261e62d9e6fcbde8311db6b7651`. An identical immutable copy is retained here as `candidate.axf`.
- Candidate function SHA256: `5586aa45c1992f30b3dfbdcd191685942932201b30652d66601ff203d3e624fd`.
- Canonical object checkpoint: `build/eu/obj/lib/al/src/Memory/alMemorySystem.o`, SHA256 `dabe3165cc38dd6a3e2215d8961901c08f7c1f7f91de87f06dc1be53b60967df`. This is root's recorded provenance, not a new build or acceptance by this lane.
- Root's diagnostic checkpoint is preserved as `input_checkpoint.json`; it reports plain `M -> m` on the complete interval.

Run from the repository root:

```sh
. ./development_environment.sh
python build/dot_proposals/scene-resource-heap/validation/verify_execution.py
```

The harness uses installed Unicorn 2.1.4, ARM11 MPCore, and pyelftools. It compiles and links nothing. It maps unchanged retail code read/execute and overlays only the existing candidate function in the candidate emulator's memory. It does not modify either input file. One script, frozen fixtures and results are retained in this ignored directory. `manifest.json` records artifact hashes.

## Three modeled external contracts

1. `0x00243260`, resource lookup: require an ordinary eight-byte SafeString with address point `0x003D9C3C` and text `ObjectData/GameSystemDataTable`; return a fixed opaque resource pointer. Filesystem, cache, loading and resource creation behavior are excluded.
2. `0x00290640`, BYAML retrieval: require that resource pointer and a SafeString containing `HeapSizeDefine`; return the current synthetic document pointer. One explicit fixture returns a null document. Archive lookup, decompression, ownership and errors are excluded.
3. `0x00105EBC`, FrameHeap allocation: record the requested size, SafeString `SceneHeapResource`, null parent, forward direction `1`, and lock argument `1`; return a fixed nonnull heap pointer with seeded flags `0xA5F0D6BF` at offset `0x6C`. No real allocation, heap sizing, alignment, locking, parent choice, failure path or operating-system behavior is exercised.

Each modeled return overwrites caller-saved `r1-r3`, `r12`, `s0-s15`, and condition flags with one of two explicit patterns. It preserves `sp`, `lr`, callee-saved registers and FPSCR. This checks register lifetime across the contracts, not arbitrary external implementations.

## Unchanged execution retained

The original BYAML constructor and verifier, string-table verifier, key search, indexed/hash readers, string getter, float getter, string comparison, both floating-point multiplications, and unsigned conversion execute as retail instructions. There are no host substitutes for those operations. `summary.json` records the 17 executed helper intervals and their unchanged SHA256 values.

The final retail helper `0x002911E8` also executes unchanged. It writes the returned heap pointer through `MemorySystem + 0xC`, then clears bit mask `4` at `heap + 0x6C`. Both runs produce the same pointer and flags `0xA5F0D6BB`. Only the allocation primitive called by that helper is modeled.

The entire synthetic arena is compared by hash. Every write must fall within the four-byte output field, four-byte heap flags, or a bounded 1024-byte stack region. All other arena bytes and surrounding stack canaries remain unchanged. Returning runs restore the stack pointer and preserve `r4-r11` and `s16-s31`. Execution must reach the return sentinel within 100,000 instructions and two seconds; no run reached either bound.

Compared observations include external call sequence and arguments, complete retail helper-entry trace, visited row indices, output arena, FPSCR control/status, and fault details. Instruction counts and stack-write counts are recorded but are excluded from semantic comparison.

## Explicit fixture coverage

There are 19 structural fixtures, 13 finite conversion fixtures, and nine instruction-behavior probes. The three structural fault fixtures produce 30 pairs. The other 38 fixtures produce 380 returning pairs.

Structural returns cover null stage argument, null document, empty table, one and three nonmatching rows, case-sensitive nonmatch, matching first/middle/last rows, duplicate ordering, a duplicate whose first match lacks a float, missing/wrongly typed float values, and an empty stage name. The finite documents are freshly constructed little-endian BYAML v1 with sorted key/value string tables, array roots and explicit hash records. They contain no copied game assets.

The independent expected-size checks cover default `0x00800000` bytes, 12/16/24/32 MiB matches, missing or wrong float tag yielding zero, signed zero, values immediately below/equal/above one byte, 1.5 bytes truncated to one, values around the unsigned midpoint, the largest selected float below the unsigned limit (`0xFFFFFF00` bytes), and subnormal/normal boundaries. Duplicate fixtures assert the exact visited row order and that the first matching record stops traversal.

All five FPSCR configurations are explicit: round nearest; round nearest plus default NaN and flush to zero (`0x03000000`); round toward positive infinity (`0x00400000`); round toward negative infinity (`0x00800000`); round toward zero (`0x00C00000`). Trap enables are clear. Floating status starts clear for each run and is compared exactly afterward.

Instruction probes cover the unsigned limit and above it, negative half a byte, negative one MiB, quiet/signaling NaN, positive/negative infinity, and the largest finite input. No independent language-level result is asserted for this category. Under this Unicorn ARM11 model, both routines request `0xFFFFFFFF` for positive overflow/infinity and zero for negative inputs/NaNs, with matching FPSCR status. These observations do not establish portable C++ behavior for out-of-range conversion or prove hardware behavior. The negative half-byte probe returns zero with the inexact flag.

## Fault agreement and limits

The missing `Stage` field, wrong Stage tag, and noncontainer row fixtures all leave a null stage string. Retail's unchanged `isEqualString` helper dereferences it at `0x0029230C`; both executions fault reading address zero before allocation. This is observable fault agreement, not a successful heap-creation case or a safety claim. A null stage argument is different: both routines skip resource lookup and request the default 8 MiB.

This evidence supports the guarded source's behavior only for these synthetic fixtures and these explicit modeled contracts. It adds zero exact matches. It does not cover game stage assets, arbitrary/malformed BYAML, missing resource exceptions, resource lifetime, allocation success/failure semantics, real console VFP fidelity, hardware traps, all FPSCR combinations, game loading, rendering or replay. The original maps, ranks, ledger, sources, compiler flags and tools were not changed.
