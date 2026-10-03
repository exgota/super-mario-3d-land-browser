# Pipe-exit state needs vector and callback-data identities

Branch: dot/root-2f5cac. Base: 64d1bb9f3c97318481de9baa08de473608b9e042.
Target 002F5CAC..002F5EF0, 580 bytes, U. No source forms or build/check claims.
Missing addresses 004305D4 and 0042FCB8 have no exact or containing rows on this base.
At 002F5D98 the root scales the vector at 004305D4 using helper 0027CC64, whose three float loads establish a 12-byte readable minimum.
Existing root-316014-needs-rows.md independently identifies this address as sead::Vector3<float>::ey; the enclosing allocation extent remains unknown.
At 002F5DFC the root passes 0042FCB8 to 00273008, which forwards it to the pointed object's virtual slot 0x88 before its termination call.
The latter callback's concrete type and the argument's readable span/owner remain unproved; no vector identity or extent is inferred solely from neighboring code.
The integrator must establish both justified rows before source/check work resumes.
No source, game-data, map/rank, Factory, tools, configuration, ledger or STATE changes. No exact credit.
