# Actor action setup needs its action-map descriptor

Branch: dot/root-169b08. Base: 64d1bb9f3c97318481de9baa08de473608b9e042.
Target 00169B08..00169D54, 588 bytes, U. No source forms or build/check claims.
Missing address 0042FCAC has no exact or containing row on this base.
At 00169B24 the root passes it as the third argument to 0027FB44 with initial action Wait.
That helper forwards the descriptor to constructor 001CD394, which reads its count and list pointer at offsets 0/4. This establishes an eight-byte readable minimum, not the full owner/allocation extent.
The descriptor's complete table ownership and associated linked entries remain unknown. No vector or array extent is inferred from its address.
The integrator must establish the justified row before source/check work resumes.
No source, game-data, map/rank, Factory, tools, configuration, ledger or STATE changes. No exact credit.
