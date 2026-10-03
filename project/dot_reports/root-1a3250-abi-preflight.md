# Camera motion update needs a shared vector-copy contract

Branch: dot/root-1a3250. Base: 64d1bb9f3c97318481de9baa08de473608b9e042.
Target 001A3250..001A349C, 588 bytes, U. No source forms or build/check claims.
At 001A3400 the root passes two three-float vector addresses to 00263CE8.
Accepted Factory/group_00263CE8.cpp defines this helper using anonymous private Triple pointers, with volatile unsigned fields, and forwards its output to a private void fn_0027D5C4(Triple*) declaration.
Other accepted callers give the latter helper different private float-vector pointer/reference types and a bool result, as recorded by root 001DF8FC.
Case-insensitive current Game/lib/header inspection found no established ordinary public alias for 00263CE8.
Stop under the accepted-private-contract exclusion; the integrator owns the shared vector-copy/normalization contracts.
No direct external data is required by this root's pool. No accepted source, Factory, map, tools, ranks, configuration, ledger or STATE changed. No exact credit.
