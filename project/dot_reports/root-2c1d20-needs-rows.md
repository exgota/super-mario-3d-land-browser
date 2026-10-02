# Root 002C1D20 needs a BSS owner row

- Base: `5d35374520b2e537b131ca2dba05cdcd48b725d9`.
- Branch: `dot/root-2c1d20`; notes only, local commit for coordinator intake.
- Target: `fn_002C1D20`, `[0x002C1D20, 0x002C1F24)`, 516 bytes,
  literal pool `[0x002C1F14, 0x002C1F24)`, still `U` on this base.
- Outcome: stopped during data/ABI preflight; zero source forms attempted.

## Missing row

`0x00430DB0` has no map row starting at or covering the address. The root's
literal pool supplies that address to construction at `0x002C1DE8` and again
to `fn_002C3970` at `0x002C1E08`. Its static-initialization guard at
`0x003F38B8` already has the four-byte row `[0x003F38B8, 0x003F38BC)`;
the guard is not the blocker.

The constructor `fn_002585C4` establishes a minimum object extent of `0x2A0`
bytes, through `[0x00430DB0, 0x00431050)`: its two-iteration loop stores a
four-byte word at object offset `0x294 + index * 8`, reaching offset `0x29C`
for index one. The preceding array-construction helper `fn_0028EABC`
returns its original input pointer, grounding the constructor's pointer
adjustments back to the same object. The root also carries destructor
`fn_002C3C38`; that destructor processes the array at object offset `0x64`.
These observations establish a lower bound, not the true allocation end or
an independently proven owner identity. No missing boundary is invented.

## Required next step and limits

Independently establish the BSS owner and full extent, then add its data row
in the main metadata lane before retrying this root. No module/class name
has been inferred merely from neighboring functions. No source, headers,
map, ranks, tools, flags, or binaries were changed. The complete map remains
byte-identical to the base.

No build or canonical `tools/check.py` command was run because the required
data owner is missing. There is no candidate object or exact-match claim.
The local owner binary SHA was verified as
`e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.
