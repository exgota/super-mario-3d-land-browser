# Service-state processing needs its global state row

Branch: dot/root-28c640. Base: 64d1bb9f3c97318481de9baa08de473608b9e042.
Target 0028C640..0028C890, 592 bytes, U. No source forms or build/check claims.
Missing address 0041CFA0 has no exact or containing row on this base.
The target holds that base in r8, reads a byte at +0x18, a word at +0x14 and a pointer at +0xA0, and writes the byte at +0x18.
These direct accesses establish a minimum span of 0xA4 bytes; the full global owner and allocation extent remain unknown.
No larger table or region boundary is inferred. The integrator must establish the justified row before source/check work resumes.
No source, game-data, map/rank, Factory, tools, configuration, ledger or STATE changes. No exact credit.
