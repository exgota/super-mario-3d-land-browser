# CFLi_InitializeSystemWork: missing data rows

- Frozen main: `3d69bf00a769676c77f07465163def56a559b48f`; branch `dot/root-116df0`.
- Target `0x00116DF0..0x00117020`, 560 bytes, remains U.
- Missing address `0x004245D4`: observed stores at +0x8/+0xC, +0x10, +0x14 and +0x18 establish a minimum 0x1C-byte span. Full object extent and identity are unproved.
- Missing address `0x0042457C`: observed stores through +0x54 establish a minimum 0x58-byte span. It is later passed to `0x002806D0`; no larger allocation extent is inferred from that call.
- Fresh map has neither an exact nor a containing row for either address. These are separate data objects; do not combine the intervening space.
- Existing individual rows around `0x003EF168` do not by themselves establish a complete global-state object identity.
- No source, compiler form or canonical checker attempt. Integrator must establish the missing rows before this target resumes.
- No game bytes, disassembly, map, rank, tool or Factory changes are included.
