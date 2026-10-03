# Motion-record queue needs a shared vector-helper contract

Branch: dot/root-1df8fc. Base: 64d1bb9f3c97318481de9baa08de473608b9e042.
Target 001DF8FC..001DFB44, 584 bytes, U. No source forms or build/check claims.
The root forms two three-float cross-product vectors and passes each to 0027D5C4 at 001DF9A4 and 001DF9E8, then copies a 136-byte record into a bounded queue.
Accepted Factory/group_00263CE8.cpp declares void fn_0027D5C4(Triple*) over its private Triple, whose fields are volatile unsigned words.
Other accepted Factory callers fn_0017E4B8.cpp and fn_00355820.cpp declare that same C-linkage helper as bool over distinct private Vector3 pointer/reference types.
Case-insensitive current Game/lib/header inspection found no established ordinary public declaration or alias that reconciles these types and return contracts.
The new caller ignores the return; this does not establish a void return or justify a fourth incompatible declaration.
Stop under the accepted-private-contract exclusion. The integrator owns the shared vector-helper contract repair.
The referenced quaternion unit already has a row; no data-row prerequisite is claimed.
No accepted source, Factory, map, tools, ranks, configuration, ledger or STATE changed. No exact credit.
