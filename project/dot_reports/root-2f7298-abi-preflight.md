# Actor message distance-test prerequisite

Branch: dot/root-2f7298. Base: cf2886000d0c49e12faddf4cd77a1d47372735c8.
Target: 002F7298..002F74D4, 572 bytes, U. No source forms or build/check claims.
At 002F730C the root passes the host actor and player pointers in r0/r1 and radius 100.0 in s0 to 0021E2D0, then tests its boolean result.
The actual 0021E2D0 body obtains both positions through 0028028C, computes squared separation, and returns whether it is below the squared float radius.
Accepted Factory/fn_0021E1D8.cpp instead defines an inline C-linkage fn_0021E2D0 taking an integer index, context pointer and private anonymous Vector4 reference, returning void through a drawing interface.
That accepted-source name is incompatible with the observed distance-test contract even though its inline body may disappear into its caller; it is not evidence for the actual callee.
Case-insensitive inspection of current Game/lib sources and public headers found no established ordinary public alias for the distance helper.
Stop under the accepted-private-contract exclusion; the integrator owns reconciling the accepted inline helper name and shared distance-call contract.
Existing nerve rows 003F2804/003F2808 are present; this is not a missing-data-row claim.
No accepted source, Factory, map, ranks, tools, configuration, ledger or STATE changed. No exact credit.
