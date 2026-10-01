# EffectSet emission packet response

Main base: `48c97b5157698b6722a6d96330ad718f5351a854`, fetched 2026-10-01 16:05 UTC. Both priority packet rows were U at selection. This proposal preserves the packet's best ordinary C++ source, adds the required NON_MATCHING guard, and verifies it through the canonical project build. It does not establish original method spellings.

## Exact-match status

Zero exact credit. `fn_001E98DC` is 192 bytes with 8 diagnostic differing bytes; `fn_001E9A10` is 204 bytes with 16. Baseline checkpoint `e7012f6`, final restored checkpoint `14cfb39`. Canonical unchanged `python make.py eu` compiled, linked and exported. A clean-build first pass hit the existing Vector3::zero scaffold bootstrap gap; an unchanged second build succeeded. No tools were edited.

Checker blob: `7c0ccd93b7387a2f9afa9b304b5254f34149b318` from the main base. Commands:
```
.venv/bin/python tools/check.py fn_001E98DC --object build/eu/obj/lib/al/src/Effect/alEffectSetEmissionFunction.o
.venv/bin/python tools/check.py fn_001E9A10 --object build/eu/obj/lib/al/src/Effect/alEffectSetEmissionFunction.o
```
Initial output for both: `U -> m: The linked candidate differs from the unchanged original interval.`
Final restored output for both: `m -> m: The linked candidate differs from the unchanged original interval.`

Local-only map enrollment names the existing unchanged rows at 001E98DC and 001E9A10 with their address identities, and names imports 0023F02C and 0023F23C as fn_0023F02C/fn_0023F23C. These imports are the packet's independently observed consumer/gate. No interval, pool, or original byte changes. Main must adopt/revalidate these identities to reproduce checks; map changes are not published.

## One additional bounded hypothesis

The two independent emission bodies repeat the same entry configuration/filter comparison. A private inline entry predicate tested whether this shared source-level operation explained entry/string saved-register allocation. This was a diagnostic source-organization hypothesis, not a recovered original member identity or new mapped helper. Checkpoint `3d49584` used the same flags and headers; both sections remained their complete sizes and canonical m. Differences worsened to 12/20 bytes. The helper had no separate retained body. It was withdrawn. Historical packet cap was eight forms; this records one followup rather than resetting that cap. Further equivalent reorderings are not justified.

## Bounded behavioral evidence

The restored baseline and original intervals passed 2,048 paired interpreted ARM executions each (4,096 pairs total), plus an independent direct model of the emitted callback sequence and activity/result state. Maximum instruction counts: 305 void, 316 boolean. The replay checks memory, callee-saved registers/SP, termination/comparison/gate/emission traces and the boolean return. The void return register is deliberately ignored.

Domain: entry counts -2,-1,0,1,2,3,4,8; valid distinct arrays/configurations; optional null entry filter or equal/unequal string values; valid embedded 64-byte set filter; varied raw activity bytes and gate outcomes. The known virtual termination writes the embedded buffer's final byte. String comparison has ordinary content semantics. Gate and emission imports are explicit deterministic non-mutating contracts; their internals are not executed. Call-clobbered integer registers are overwritten after each import to test preservation. This is bounded contract-level evidence, not native hardware execution, full runtime correctness, malformed-memory safety, concurrency, or exact matching. The lower-level emission/gate imports remain dependencies.

Complete linked SHA-256:
- Void original: `491f473bda3a5e6ea3221584853f412dfbd415ce643de281c1855f3f999d01d7`
- Void candidate: `2629fab1a67817528d6cdfd4c3e4a2df8a30b191eea03694aec3d6213f90ae0c`
- Boolean original: `72f5ed4fe30a4fa5c61790c49c639923440dd48e5c623c3490e3ec698bc2e8a2`
- Boolean candidate: `53881caf36a34c764452d0b38ac062b43c4da9c858f66d2afad669782584a178`

The restored canonical linked sections have the same hashes as the replayed baseline. The reproducible interpreter is included in `effect-set-emission-replay.md`; it reads the user's local binary and canonical linked candidates without changing them. Main's original packets are preserved.
