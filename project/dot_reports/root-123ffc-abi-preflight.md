# Scene transition update needs a typed entry contract

Branch: dot/root-123ffc. Base: 64d1bb9f3c97318481de9baa08de473608b9e042.
Target 00123FFC..00124248, 588 bytes, U. No source forms or build/check claims.
The root reads scene-like state fields through r0, updates counters at +0x84/+0x8C and checks child-state pointers before choosing one of eleven mapped nerves.
Accepted nerve stub 00123FF4 in Factory/group_00117BEC.cpp declares this exact C-linkage entry as int fn_00123FFC(int,int*) and forwards the pointer word at keeper offset zero as its integer argument.
The first argument is dereferenced throughout the real body. Retaining an integer placeholder with casts would not establish a coherent shared object contract.
Case-insensitive current Game/lib/header inspection found no established ordinary public entry alias. docs/facts/00123FF4.md repeats the stub's integer guess without body evidence.
Stop under the accepted-contract exclusion; the integrator owns the stub/root signature repair. No return-type certainty is inferred from unused r0 residue.
All directly referenced nerve addresses have rows. No accepted source, Factory, map, tools, ranks, configuration, ledger or STATE changed. No exact credit.
