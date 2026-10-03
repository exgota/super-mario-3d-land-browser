# Graphics update needs its guarded identity-matrix row

Branch: dot/root-335704. Base: 64d1bb9f3c97318481de9baa08de473608b9e042.
Target 00335704..00335954, 592 bytes, U. No source forms or build/check claims.
Missing address 00430C68 has no exact or containing row on this base.
The target's guarded initialization at 00335840 writes the twelve floats through offset +0x2C, setting the 3x4 identity diagonal and clearing the other entries.
This independently establishes a 48-byte writable minimum and an identity-matrix role, without proving the complete enclosing allocation boundary.
At 003358B8 the target also passes the same matrix to 00254DD4. The separate guard at 003F389C already has a row and is not requested.
The integrator must establish the justified matrix row before source/check work resumes.
No source, game-data, map/rank, Factory, tools, configuration, ledger or STATE changes. No exact credit.
