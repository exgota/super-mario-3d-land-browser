# Rotating bar initialization needs upward-vector identity

Branch: dot/root-12bec8. Base: 64d1bb9f3c97318481de9baa08de473608b9e042.
Target 0012BEC8..0012C110, 584 bytes, U. No source forms or build/check claims.
Missing address 004305D4 has no exact or containing row on this base.
At 0012BFF4 the root passes that address to scaling helper 0027CC64, whose three float loads establish a 12-byte readable minimum; 0012C014 also forwards it to 00277884.
The separate root-316014-needs-rows.md identifies this address as sead::Vector3<float>::ey from its initializer. Its full owner/allocation extent remains unknown.
The init chooses NeedleBarCore resources and FireBar length-specific parameters; these source-level string identities do not establish the missing vector's enclosing row.
The integrator must establish the justified row before source/check work resumes.
No source, game-data, map/rank, Factory, tools, configuration, ledger or STATE changes. No exact credit.
