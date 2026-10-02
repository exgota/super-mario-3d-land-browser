# Actor message handler shared receiver prerequisite

Branch: dot/root-30f534. Base: 3d69bf00a769676c77f07465163def56a559b48f.
Target: 0030F534, 556 bytes, U. Exact claims: none.
The target passes its actor receiver both to public al::invalidateClipping/startAction and to fn_00277AF0 at 0030F600 and 0030F6FC.
Accepted Factory fn_0031A9CC.cpp declares fn_00277AF0 using its anonymous-namespace Actor*, with no shared declaration of that receiver type.
A new al::LiveActor declaration would add another incompatible C++ contract. Integrator-owned Factory reconciliation is required; no cast or alternate export is proposed.
The separate fn_001C96B8 call already has an established al::LiveActor* declaration in accepted al sources and is not treated as a new blocker.
No direct data-row gap was found. Other dependencies were not exhaustively screened after the established private-receiver conflict.
No source, build, canonical check, or replay was attempted. No runtime failure or demotion claim.
