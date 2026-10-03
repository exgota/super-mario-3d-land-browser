# Audio command setup needs scratch-buffer identity

Branch: dot/root-2c5380. Base: 64d1bb9f3c97318481de9baa08de473608b9e042.
Target 002C5380..002C55C4, 580 bytes, U. No source forms or build/check claims.
Missing address 0042B040 has no exact or containing row on this base.
The root passes that address and capacity 0x4000 to file/header reader 002C13D0 at 002C53A8.
The reader computes an input-dependent combined length, checks it against the supplied capacity, and uses the destination on the successful path.
The 0x4000 value is an observed capacity bound, not proof of the global allocation's complete extent or an exact row boundary. A fixed minimum readable/writable extent is not established here.
The integrator must identify the real scratch-buffer owner and extent before source/check work resumes.
No source, game-data, map/rank, Factory, tools, configuration, ledger or STATE changes. No exact credit.
