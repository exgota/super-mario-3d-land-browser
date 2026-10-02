# Audio-manager update shares missing BSS row

Branch: dot/root-2c36b8. Base: 713975727c0447ea8cdea708a9ab13cc539a92d0.
Target: 002C36B8, 552 bytes, U. Exact claims: none.
Missing address: 00430DB0. No exact or containing row exists in the frozen map.
This is the same dependency as published root-2c1d20-needs-rows.md, which independently establishes a minimum 0x2A0-byte constructor extent. That audit is not repeated here.
This root accesses indexed records at base +0x64 with stride0xC and writes eight-byte timing values at base +0x290 with stride8; valid index bounds are not newly established.
The separate static guard 003F38B8 already has a four-byte row. Full object ownership and allocation end remain unproved.
No source, build, canonical check, or replay was attempted. Integrator should establish the justified data row before retrying.
