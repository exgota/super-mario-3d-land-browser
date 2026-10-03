# Audio resource table return-value prerequisite

Branch: dot/root-25fc4c. Base: cf2886000d0c49e12faddf4cd77a1d47372735c8.
Target 0025FC4C..0025FE8C, 576 bytes, U. No source forms or build/check claims.
At 0025FDB8 the root passes an eight-byte SafeString view to 002513E4 and stores the returned r0 word into the current bank entry at 0025FDC0.
Accepted Factory/group_0022D800.cpp declares fn_002513E4(void*) as void, as well as its indirect DispatchVTable::invoke target.
The original helper tail-dispatches through singleton 003E2AE4 and virtual slot 0x0C; the caller proves a consumed result, but not whether its C++ type is an integer ID or pointer.
Case-insensitive current Game/lib/header inspection found no established ordinary public value-returning alias. docs/facts/002513E4.md repeats the void guess without caller evidence.
Stop under the accepted-private-contract exclusion; the integrator owns the shared wrapper and indirect-call return contract repair.
All directly referenced data addresses have rows on this base. No missing-row claim is made.
No accepted source, Factory, map, tools, ranks, configuration, ledger or STATE changed. No exact credit.
