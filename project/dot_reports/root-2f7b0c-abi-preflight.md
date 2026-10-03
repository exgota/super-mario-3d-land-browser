# Actor message numeric-ID prerequisite

Branch: dot/root-2f7b0c. Base: cf2886000d0c49e12faddf4cd77a1d47372735c8.
Target: 002F7B0C..002F7D48, 572 bytes, U. No source forms or build/check claims.
At 002F7D18 the root forwards actor, message value, sender and receiver to 00271500, with 0.05 in s0.
The helper directly compares r1 against numeric message ID 0x29 before accessing sensor positions; its second argument is an integer ID, not a dereferenced message object.
Accepted Factory/fn_0016F7A4.cpp declares this C-linkage helper with anonymous private Actor, SensorMsg and HitSensor pointer types, including const SensorMsg* for the numeric ID.
Case-insensitive inspection of current Game/lib sources and headers found no established ordinary public fn_00271500 alias; the map row is unnamed.
Stop under the accepted-private-contract exclusion; a new private view or cast cannot establish one shared C++ declaration.
Existing nerve rows 003F2178/217C/2180/2184/2188 are present; this is not a missing-data-row claim.
The integrator owns the accepted caller/helper declaration repair. No accepted source, Factory, map, ranks, tools, configuration, ledger or STATE changed. No exact credit.
