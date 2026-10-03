# Camera vector setup needs an existing data identity

Branch:dot/root-19a34c. Base:cf2886000d0c49e12faddf4cd77a1d47372735c8.
Target0019A34C..0019A58C,576bytes,U. No source forms or build/check claims.
Missing address004305D4 has no exact or containing row on this base.
The target passes it to0027CC64 at0019A3C0 and0019A41C; that vector-scaling helper reads three floats at offsets0/4/8, establishing a12-byte readable minimum.
The complete enclosing allocation/owner boundary remains unknown; no table extent is invented.
Existing root-316014-needs-rows.md independently identifies sead::Vector3<float>::ey at004305D4 through its initializer; root-186a00-needs-rows.md records the same dependency.
The integrator must establish the justified row before source/check work resumes.
No source, game-data, map/rank, Factory, tools, configuration, ledger or STATE changes. No exact credit.
