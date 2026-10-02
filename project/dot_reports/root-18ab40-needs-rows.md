# State dispatch: missing vector row

- Branch `dot/root-18ab40`; frozen main `3d69bf00a769676c77f07465163def56a559b48f`.
- Target `0x0018AB40..0x0018AD74`, 564 bytes, remains U.
- Missing `0x004305D4`, minimum observed span 12 bytes. At 18ABEC the target supplies this address as the second argument to 2702EC; that helper forwards it to 272F60, whose initial three-word read establishes the bound.
- Fresh map has no exact or containing row. Nearby vector zero 4305F8 and quaternion unit 430500 already have named rows.
- Published `root-316014-needs-rows.md` independently documents the initializer's ex/ey evidence, with ey at this same 4305D4 address. This target adds access evidence, not another independent owner identification. Related 158D70 and 186A00 reports share the prerequisite.
- No source, build or canonical checker attempt. Integrator owns adding/naming the row and regrading dependent roots.
- No data contents, disassembly, map, rank or tool change is included.
