# Nokonoko initializer: capped non-matching best

Target fn_00316014 is 532 complete bytes. Frozen base:
3e8b2608ac46560fc6078ce9b9a851d975861f6b.

Three historical failed forms remain counted at 528, 524 and 548 bytes.
Their old source/compiler proof is unavailable. No historical best was recovered.
The new sequence is explicitly forms 4–8. Fixed metric: canonical validity,
then pinned full-interval asm-differ score, then absolute size gap.

All new forms are canonically valid non-exact results:
- Form 4: 524 bytes, CURRENT 3115, gap 8
- Form 5: 528 bytes, CURRENT 3074, gap 4
- Form 6: 528 bytes, CURRENT 2056, gap 4; retained best
- Form 7: 528 bytes, CURRENT 3356, gap 4
- Form 8: 532 bytes, CURRENT 2652, gap 0

Stop: eight total forms and two consecutive forms no closer than the best.
Form 6 was restored unchanged and rebuilt at aaf47a21; the identical object,
complete 528-byte section and CURRENT 2056 reproduce. Zero new exact bytes.
Fresh baseline, every trial and final check preserve fn_00315C6C at 712 exact
bytes. Its actual object emits only that canonical function. Standard generated
SafeString table/helpers equal the same-tree baseline; no helper match credit.

Source is limited to the neutral root declaration/friend, new initializer and
coherent 0xC8 MapObjActor-derived shell header. Existing virtual init remains;
NokonokoJointControl stays opaque. No synthetic actor table or flags changes.
Separate name-only evidence commits 28a5b918 and 1d5c1bee establish the ordinary
reconstructed shell constructor and neutral root name. No ranks/boundaries changed.

Next uncompiled idea, only after an authorized revisit: investigate scoped
const-reference bindings for the three vector arguments to alter ordinary
lifetime, stack and floating-register scheduling. No form 9 was attempted.
Initial setup/checker-name failures remain retained and excluded from results.
No acceptance, runtime-test or external-publication claim.

Owner paused decompilation on 2026-10-04 at08:25 UTC. No further matching or repair work is running in this task.
