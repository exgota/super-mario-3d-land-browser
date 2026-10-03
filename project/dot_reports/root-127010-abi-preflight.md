# Title-layout construction needs a shared layout-query contract

Branch: dot/root-127010. Base: 64d1bb9f3c97318481de9baa08de473608b9e042.
Target 00127010..0012725C, 588 bytes, U. No source forms or build/check claims.
The root constructs a title-screen layout and invokes 001C7F2C repeatedly on its layout object for named button/pane setup.
Accepted Factory/group_0018F99C.cpp defines fn_001C7F2C(Object*) using an anonymous private Object with a pointer at +0x18, forwarding that field to fn_001C7F34(void*).
Case-insensitive current Game/lib/header inspection found no ordinary public alias for the outer helper. The inner void* callee is a different function and cannot silently replace the original call.
Stop under the accepted-private-contract exclusion; the integrator owns the shared layout-query declaration.
No new data rows are requested. No accepted source, Factory, map, tools, ranks, configuration, ledger or STATE changed. No exact credit.
