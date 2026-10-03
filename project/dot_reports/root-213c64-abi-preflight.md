# Motion update needs a shared vector-helper declaration

Branch: dot/root-213c64. Base: 64d1bb9f3c97318481de9baa08de473608b9e042.
Target 00213C64..00213EB0, 588 bytes, U. No source forms or build/check claims.
At 00213D14 the root passes a computed three-float vector to 0027D5C4.
Accepted Factory/group_00263CE8.cpp declares void fn_0027D5C4 over private Triple*, while fn_0017E4B8.cpp and fn_00355820.cpp use bool and distinct private Vector3 pointer/reference types.
Case-insensitive current Game/lib/header inspection found no established ordinary public alias. This repeats the concrete shared-helper prerequisite documented for root 001DF8FC.
An ignored return in this root does not establish void return semantics or justify another incompatible declaration.
Stop under the accepted-private-contract exclusion; the integrator owns the shared helper contract repair.
No direct external data is required by this root's pool. No accepted source, Factory, map, tools, ranks, configuration, ledger or STATE changed. No exact credit.
