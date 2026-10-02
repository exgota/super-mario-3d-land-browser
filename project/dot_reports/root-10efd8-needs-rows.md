# CFLi_InitializeDatabase: missing data row

- Frozen base: `f8e79c19657c5dd4d57631b6456bd353e8fe1726`.
- Branch: `dot/root-10efd8`.
- Target: `CFLi_InitializeDatabase`, `[0x0010EFD8, 0x0010F1F0)`, 536 bytes, rank U.
- Result: stopped during data/ABI screening; zero source forms, builds or checker attempts.

## Required data evidence

The target directly addresses `0x0042427C`. The frozen map has no row beginning
there and no containing row. This is a real static-storage reference, not an
inferred vtable prefix or an address inside another mapped object.

The target writes a 32-bit count of six at offset 0, then passes six records at
offset `4 + index * 0x5C` to `CFLi_GetDefaultMiiDataCore`,
`CFLi_GetDefaultCreateIDonCTR` (record offset `0x0C`), and
`CFLi_ClearCreatorName`. The last helper writes 20 bytes at record offset
`0x48`, independently proving each accessed record reaches offset `0x5C`.
Together these accesses establish a minimum required span of `0x22C` bytes,
`[0x0042427C, 0x004244A8)`. The owning object, full extent, and true end boundary
remain unknown; `0x004244A8` is only the observed minimum end.

An independently evidenced data-row proposal is needed before reconstruction.
No row, owner, absolute-address replacement, or synthetic table was invented.

## Other screening facts

- Global state at `0x003EF054` already has a 20-byte row; the target accesses
  offsets 0, 4, 8, 12 and 16 within it.
- Alternate device interface `0x003A59D4` has an existing 96-byte row.
- Accepted `CFLi_GetDeviceInterfaceFile` returns `0x003AC780`, which has an
  existing 44-byte row. The interface's first entry receives the target's two
  pointer arguments; no shared wrapper or symbol contract was changed.
- The target's direct function callees all have existing function rows.
- The allocation helper at `0x00285120` advances a caller-owned buffer pointer,
  decreases its remaining size, aligns to 32 bytes and clears the allocation.

## Verification and limits

The owner's local `code.bin` SHA-256 was verified as
`e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.
The map is byte-unchanged (SHA-256
`f937248d5883a83dfc65edf3e704c38b4b6c1fdd2dce6bf816ea46bd416dcda8`).
No source, headers, Factory files, build configuration, ranks, ledger, or tools
were edited. There is no candidate object or build provenance and no exact
claim. Only this report is submitted; the coordinator publishes it.
