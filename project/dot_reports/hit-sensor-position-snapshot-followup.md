# Hit-sensor execute: value-snapshot followup

One new API-shape hypothesis after the eight historical candidates. The local const-reference getPos shape had not been established independently; a source-local value-return snapshot tested whether target paired position loads originated from an inlined value getter. Committed probe `679511d` kept shared headers unchanged and produced 496 bytes, but differing bytes increased from 23 to 52. This is worse. Baseline restored in `8d9b5f4`.

Canonical commands:
```
.venv/bin/python tools/check.py _ZN2al17HitSensorDirector7executeEv --object build/eu/obj/lib/al/src/LiveActor/alHitSensorDirector.o
.venv/bin/python tools/check.py _ZN2al17HitSensorDirectorC1Ev --object build/eu/obj/lib/al/src/LiveActor/alHitSensorDirector.o
```
Probe output: `m -> m: The linked candidate differs from the unchanged original interval.` Constructor control remained `O -> O`.

Linked baseline SHA-256: `3dfeaf9a0c95434689f3b0059f6be68e4373104f6ec0903e2c1f366994de1c9b`.
Probe SHA-256: `d455b40f406ac5a9c8edfbc726930ed7d5fcec1f0f1e4f6e4c28f4ac8ad9339d`.
Remaining blocker is VFP load scheduling/temporary register allocation. Main already documents 810 bounded behavioral pairs; this probe adds no behavioral or exact-match claim.

## Verification context

Measured on main base `8ca3fc0f2aa6c8fe0d870cf4992b9f6029286a13` with committed source and the normal `python make.py eu` project build (ARMCC 4.1/791). Canonical checker blob `7c0ccd93b7387a2f9afa9b304b5254f34149b318` is unchanged in freshly fetched main `48c97b5157698b6722a6d96330ad718f5351a854` (2026-10-01 15:55 UTC). Byte/word difference counts below are diagnostic only. No original boundaries, tools, flags, or binaries were modified. No exact credit is claimed by these negative followups.
