# Placement map: nested temporary lifetime followup

One new full-expression lifetime hypothesis after the eight historical candidates. Replaced the split stage-data pointer initialization and ByamlIter construction with nested `ByamlIter(stageArchive->getByml(...))`, extending the lookup argument temporary to the constructor full expression. Committed probe `452a2e1` retained 344 bytes, but differing bytes increased from 33 to 56 (20 to 23 instruction words). This is worse. Baseline restored in `b294c8e`.

Canonical commands:
```
.venv/bin/python tools/check.py _ZN2al16initPlacementMapEPNS_5SceneEPKNS_8ResourceERKNS_13ActorInitInfoEPKc --object build/eu/obj/lib/al/src/Scene/alSceneFunction.o
.venv/bin/python tools/check.py _ZN2al19tryGetPlacementInfoEPNS_9ByamlIterEPKNS_8ResourceEPKc --object build/eu/obj/lib/al/src/Scene/alSceneFunction.o
```
Probe output: `m -> m: The linked candidate differs from the unchanged original interval.` The 144-byte helper control remained `O -> O`.

Baseline linked SHA-256: `4a69f3fef5d46caa676d022d18b01eb5d27c9251c938199e1c1ecd0d6349cf98`.
Probe linked SHA-256: `023ec218d4344b4ae8a8ce43347393d670f3d384f0e82679c22d6d0bdc0350d8`.
Remaining blocker is iterator/init-info stack-slot allocation. Target reloads the factory pointer each iteration, so caching it is unsupported. Previous helper-order and separate-unit probes should not be repeated without new evidence.

## Verification context

Measured on main base `8ca3fc0f2aa6c8fe0d870cf4992b9f6029286a13` with committed source and the normal `python make.py eu` project build (ARMCC 4.1/791). Canonical checker blob `7c0ccd93b7387a2f9afa9b304b5254f34149b318` is unchanged in freshly fetched main `48c97b5157698b6722a6d96330ad718f5351a854` (2026-10-01 15:55 UTC). Byte/word difference counts below are diagnostic only. No original boundaries, tools, flags, or binaries were modified. No exact credit is claimed by these negative followups.
