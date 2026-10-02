# Player direction normalization shared type prerequisite

Branch: dot/root-17231c. Base: e2336037d4f709acc082b3322daedadf9500564a.
Target: 0017231C, 540 bytes, U. Exact claims: none.
The historical factory job 12093 facts are context, not an exhausted-attempt cap or active ownership reservation. No current source body or dot ownership was found.
Fresh preflight instead finds an actual accepted shared declaration conflict: group_002438D4.cpp defines rank-O fn_00279ABC as float(anonymous-namespace Vec3&).
That TU-private Vec3 is incomplete and inaccessible to another TU; the same wrapper forwards to the mapped vector normalization helper.
This target passes a three-float local vector to that wrapper and later uses the existing PlayerProperty/sead::Vector3f interface.
A second local Vec3 definition or a differently typed extern would not provide one coherent C++ declaration for the accepted wrapper.
Integrator-owned Factory cleanup must establish a shared vector type/declaration before this lane implements the call.
No Factory/header/source changes or canonical checks were attempted. No runtime fault, byte mismatch, or demotion claim.
The existing docs/facts/0017231C.md remains useful for field/callee evidence; no data-row gap is claimed here.
