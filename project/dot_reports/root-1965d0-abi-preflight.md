# Motion calculation needs shared context and vector contracts

Branch: dot/root-1965d0. Base: 64d1bb9f3c97318481de9baa08de473608b9e042.
Target 001965D0..00196824, 596 bytes, U. No source forms or build/check claims.
The root calls 0026E1DC at 001965E0, preserves its result and uses it as a virtual-dispatch object. The original helper obtains that pointer from globals and does not consume a caller argument.
Accepted Factory/fn_00173698.cpp instead declares void fn_0026E1DC(Motion*), while fn_00171BE0.cpp and fn_001BAD90.cpp declare no-argument calls returning different private Settings*/Limit* types.
The root additionally uses vector helper 0027306C, whose accepted declarations use private Vec3 types.
Case-insensitive current Game/lib/header inspection found no established ordinary public alias resolving these contracts.
Stop under the accepted-private-contract exclusion; the integrator owns shared context-return and vector declarations.
No direct external data row is required by this root's pool. No accepted source, Factory, map, tools, ranks, configuration, ledger or STATE changed. No exact credit.
