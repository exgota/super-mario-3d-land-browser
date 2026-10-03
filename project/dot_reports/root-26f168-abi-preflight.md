# Actor-stack removal needs a shared motion-helper contract

Branch: dot/root-26f168. Base: 64d1bb9f3c97318481de9baa08de473608b9e042.
Target 0026F168..0026F3B0, 584 bytes, U. No source forms or build/check claims.
The root removes an actor from a short pointer sequence and conditionally adjusts the actor above it; at 0026F260 it passes that actor and float 15.0 to 0026F770.
The same actor is passed through the existing public al::getTrans and al::onCollide interfaces.
Accepted Factory/fn_001A8D50.cpp and group_001572BC.cpp declare C-linkage fn_0026F770 with separate anonymous private Actor* types.
Case-insensitive current Game/lib/header inspection found no established ordinary public alias or shared declaration for that helper.
The float argument and source-level actor role are evidenced, but neither permits inventing a new incompatible private-view declaration.
Stop under the accepted-private-contract exclusion; the integrator owns the shared helper contract repair.
No global data row is required by this root's literal pool. No accepted source, Factory, map, tools, ranks, configuration, ledger or STATE changed. No exact credit.
