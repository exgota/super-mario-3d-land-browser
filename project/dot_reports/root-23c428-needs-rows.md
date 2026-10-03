# Service operation needs its synchronization object row

Branch: dot/root-23c428. Base: 64d1bb9f3c97318481de9baa08de473608b9e042.
Target 0023C428..0023C674, 588 bytes, U. No source forms or build/check claims.
Missing address 00421288 has no exact or containing row on this base.
The root loads the address at 0023C45C, reads/updates the lock word at +0x00, accesses thread ownership at +0x04 and recursion state at +0x08.
These accesses establish a 12-byte minimum span; the complete synchronization-object allocation/owner remains unknown.
The separate initialization flag at 003E3194 already has a row and is not requested.
The integrator must establish the justified row before source/check work resumes. No table boundary is inferred from nearby globals.
No source, game-data, map/rank, Factory, tools, configuration, ledger or STATE changes. No exact credit.
