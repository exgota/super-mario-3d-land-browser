# Course-selection owner shared type prerequisite

Branch: dot/root-18899c. Base: e2336037d4f709acc082b3322daedadf9500564a.
Target: 0018899C, 544 bytes, U. Exact claims: none.
Existing course_selection_evidence.md identifies a separately allocated 0x68-byte CourseSelectMap child, stored in this target receiver's +0x188 field; that size does not establish the receiver's allocation extent.
The evidence also documents the shared WorldSelectionInterface used by the child.
This root calls fn_0026AF18 at 00188A70 with its selection object, stores the result at +0x180, and uses the selection's integer-returning virtual slot +8 later.
Accepted Factory fn_00372F94.cpp declares fn_0026AF18 as anonymous Auxiliary*(anonymous Selection*); both types are TU-private.
Published root-1b5924-abi-blocked.md independently establishes an arithmetic integer course-index return for 0026AF18/0026AF34, conflicting with the Factory pointer-return declarations; that audit is not repeated here.
The input-owner evidence in this note is separate from that established return-contract conflict.
The existing public WorldSelectionInterface represents the same observed selection interface but is not that inaccessible C++ type.
A new local copy or differently typed extern cannot establish a coherent shared declaration; Factory reconciliation belongs to the integrator.
No source/header/Factory changes, build, or canonical check were attempted. No runtime fault, byte mismatch, or demotion claim.
Direct literal rows 003CDFB8 and 003F1128 exist. No missing-row request or complete dispatch-table identity is asserted.
