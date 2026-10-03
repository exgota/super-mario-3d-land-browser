# 0039053C: integer texture parameter setter

Status: unfinished, nonexact draft. No exact claim or main acceptance.
Original function interval: 740 bytes, 0x0039053C through 0x00390820.
Historical base: b8cf0ef685ffaf49b7a8b8180c8b53cbf540bdd2.
Closest and only compiled attempt: form 1, 492-byte complete section versus 740 bytes; canonical size rejection (M).
The normal build and full diff were verified before the workspace rollback.
Original object, provenance and full diff files are unavailable in the current snapshot; this is a historical result, not a newly reproduced check.

The two draft files were recovered verbatim from the captured original write and match their historical source hashes.
Header SHA-256: 1e58e6c15ca03f252b6a4377f8a9521e14bc0f73a8794c0569cbf99fe8f67fe0.
Source SHA-256: 50c3bc315cb7e01b73a4bf77cb0e20ddf6a068013ac529bad909d4e6c303e96c.
Strip this function directory prefix to recover the original lib/CtrSDK paths.
No map edits or compiled artifacts are included.

What differs: the common parameter-prefix model merges the original target-specific parameter dispatch and border-clamp paths.
Next idea: independently recheck the distinct 0x78-byte 2D and 0x1CC-byte cube owner constructors, then consider separate ordinary typed owners if that evidence supports them. No shared base was established.
That second form has not been written or compiled. Do not duplicate branches merely to change code size, add casts, force inlining or alter compiler flags.
Preserve the first-form attempt count and use a fresh main/build/check before any exact delivery.
