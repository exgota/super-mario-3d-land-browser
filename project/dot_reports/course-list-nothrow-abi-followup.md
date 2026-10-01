# CourseList: allocator ABI and three controls

Official ARM C++ ABI section 4.1 states that library nothrow new/new[] must not inspect their second argument, so callers may pass an arbitrary value. Source: https://github.com/ARM-software/abi-aa/blob/main/cppabi32/cppabi32.rst . The unexplained retail argument moves therefore do not establish a heap or tag parameter.

Independent exact callers support this: HitSensorDirector's constructor has six `mov r1,r5` with no local r5 definition; ExecuteDirector init uses r6 before defining it; Fugumannen init uses r4 for ordinary allocation. CourseList::World's register can hold an iterator pointer and later a loop index. The observed allocator implementation ignores r1 and selects alignment 4.

Three bounded diagnostic controls used the canonical ARMCC 4.1/791 provenance command, with only scratch source/output/dependency paths changed:
1. Exact HitSensorDirector caller with external SensorHitGroup constructor
2. Same caller plus the independently recovered constructor body
3. Same visible body inside no-inline pragmas

All three produced the identical 364-byte constructor section, SHA-256 `8c9c1a306ded99f55f3d2163a17f9e9771fa28da6f6c1359fc7200242268ea17`; all six constructor calls and arbitrary argument moves remained. Body visibility alone did not alter this allocation. Case 2 was not actually inlined, so this is not evidence about forced inline expansion.

These were diagnostic controls, not project-source matching attempts. No CourseList source or production headers changed. No justified source patch follows. Three-control cap reached; do not invent extra allocation arguments or repeat equivalent variants. Both CourseList hard roots remain unmatched.

## Verification context

Measured on main base `8ca3fc0f2aa6c8fe0d870cf4992b9f6029286a13` with committed source and the normal `python make.py eu` project build (ARMCC 4.1/791). Canonical checker blob `7c0ccd93b7387a2f9afa9b304b5254f34149b318` is unchanged in freshly fetched main `48c97b5157698b6722a6d96330ad718f5351a854` (2026-10-01 15:55 UTC). Byte/word difference counts below are diagnostic only. No original boundaries, tools, flags, or binaries were modified. No exact credit is claimed by these negative followups.
