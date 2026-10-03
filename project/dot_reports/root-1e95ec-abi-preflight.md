# Rail initialization needs a shared element-constructor contract

Branch: dot/root-1e95ec. Base: 64d1bb9f3c97318481de9baa08de473608b9e042.
Target al::Rail::init(const al::ByamlIter&) at 001E95EC..001E9834, 584 bytes, U. No source forms or build/check claims.
At 001E96F4 the root supplies accepted constructor 001EB920 to array-construction helper 0028EABC with a 12-byte stride and segment count.
Accepted Factory/group_0010CAB0.cpp defines fn_001EB920(Pair*) using an anonymous private Pair containing two pointer fields.
The root later accesses the element's third word as cumulative segment length. Its public Rail header has opaque integer storage fields and no shared element or callback declaration.
Case-insensitive current Game/lib/header inspection found no ordinary public alias for the existing constructor callback.
A new private element type or manual callback cast would not reconcile the accepted function entity. Stop under the accepted-private-contract exclusion; the integrator owns that shared construction contract.
The currently mapped zero vector and all direct data references have rows; this is not a missing-data claim.
The header's separate init(const PlacementInfo&) declaration is not silently changed into the ByamlIter overload.
No accepted source/header, Factory, map, tools, ranks, configuration, ledger or STATE changed. No exact credit.
