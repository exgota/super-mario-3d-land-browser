# Vector frame update: shared helper prerequisite

- Branch `dot/root-1b8c20`; base `6b0e2a1385b814b70344023d9816424edcc7d832`.
- Target `0x001B8C20..0x001B8E5C`, 572 bytes, remains U.
- The target calls 279ABC repeatedly on three-float vector temporaries, alongside vector cross/multiply/add operations.
- Published root-17231c-abi-preflight.md and root-160770-abi-preflight.md document the independently established 279ABC prerequisite: accepted Factory callers use incompatible private Vec3 types and no coherent public helper declaration exists.
- This note identifies another dependent root without repeating that audit or inventing a private alias/cast adapter. Full receiver class identity remains unproved.
- No source/build/check attempt or exact claim. Shared interface repair remains integrator-owned; only this short note is included.
