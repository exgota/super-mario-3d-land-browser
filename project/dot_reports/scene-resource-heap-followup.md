# Scene-resource heap: success-scope followup

One new lifetime hypothesis after the eight historical candidates. Keep heap creation inside the successful BYAML lookup scope, return there, and use a separate 8 MiB fallback. Source checkpoint `ad852a8` produced 344 bytes against the original 320; the compiler emitted two calls/two epilogues rather than the target merge. This is worse. The retained baseline was restored in `a16ff7c`.

Canonical command:
```
.venv/bin/python tools/check.py _ZN2al12MemorySystem23createSceneResourceHeapEPKc --object build/eu/obj/lib/al/src/Memory/alMemorySystem.o
```
Probe output: `m -> M: The complete compiled section, including its literal pool, has a different size from the original interval.`
Restored baseline output: `M -> m: The linked candidate differs from the unchanged original interval.`

Remaining blocker: stack/register lifetime and final call merging, not unresolved imported identities. The success-scope hypothesis is exhausted; do not repeat it or reset the historical cap.

## Verification context

Measured on main base `8ca3fc0f2aa6c8fe0d870cf4992b9f6029286a13` with committed source and the normal `python make.py eu` project build (ARMCC 4.1/791). Canonical checker blob `7c0ccd93b7387a2f9afa9b304b5254f34149b318` is unchanged in freshly fetched main `48c97b5157698b6722a6d96330ad718f5351a854` (2026-10-01 15:55 UTC). Byte/word difference counts below are diagnostic only. No original boundaries, tools, flags, or binaries were modified. No exact credit is claimed by these negative followups.
