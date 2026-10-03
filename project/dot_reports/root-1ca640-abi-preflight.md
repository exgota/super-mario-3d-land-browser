# Actor-light configuration needs one shared controller layout

Branch: dot/root-1ca640. Base: 64d1bb9f3c97318481de9baa08de473608b9e042.
Target 001CA640..001CA88C, 588 bytes, U. No source forms or build/check claims.
Caller 001CA5E8 allocates 0x2C bytes, invokes constructor 001CAAB0, stores the controller at LiveActor offset +0x48, and falls through into this root with that pointer.
The root parses three light-mode fields at +0x00/+0x04/+0x08 and stores either a supplied pointer at +0x0C or an alternate-state pointer at +0x24.
Accepted alLiveActor.cpp privately defines al::ActorLightCtrl as a 12-byte placeholder; the public header only forward-declares it. This is the same unreconciled layout identified for root 001CA3D0.
A second complete definition or differently named view would hide the shared-type conflict. Stop under the accepted-private-contract exclusion.
The integrator owns the shared controller declaration. Existing public Byaml and void* manager imports do not resolve this object's layout.
No missing data row is claimed. No accepted source/header, Factory, map, tools, ranks, configuration, ledger or STATE changed. No exact credit.
