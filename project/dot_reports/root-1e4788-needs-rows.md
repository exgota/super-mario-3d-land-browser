# 001E4788: missing identity-matrix data rows

- Refreshed public main: `6b0e2a1385b814b70344023d9816424edcc7d832`
- Branch: `dot/root-1e4788`
- Target: unnamed `U` function `[0x001E4788,0x001E49C4)`, 572 bytes
- Source forms attempted: 0; exact credit: 0
- Original code SHA-256 verified:
  `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`

The constructor references two absent writable-data rows:

| Address | Observed minimum | Evidence |
| --- | --- | --- |
| `0x00430C98` | `0x40` bytes | Initializes a 4-by-4 float identity matrix, then passes its address to `0x00249CAC` twice |
| `0x00430C68` | `0x30` bytes | Initializes a 3-by-4 float identity matrix, then passes its address to `0x00291470` twice |

These are minimum accessed extents, not proposed map boundaries. Neither address
falls within a row on the refreshed base. Exact object ownership and full storage
extents remain unknown; their adjacency alone does not establish either boundary.
The existing four-byte guard rows at `0x003F38A0` and `0x003F389C` do not cover the
matrix storage. The installed dispatch address `0x003D74E8` already has a row and
is not a blocker.

The observed object begins with that dispatch pointer and a float initialized to
45.0 at offset `0x04`. A `0x60`-byte subobject at `0x08` is constructed through
`0x00295E48` and initialized through `0x00295768`. Matrix copies then populate
`0x68`, `0xA8`, `0xE8`, and `0x118`; the accessed object extent is at least `0x148`.
Finally the constructor calls `0x001E449C` and returns its original object pointer.
That sibling computes perspective/stereo projection state, supporting a camera
or projection-controller role. An exact class name and allocation size were not
established. No direct ARM `BL` caller was found in the executable code range;
this is not evidence that the constructor is unused.

All called function addresses have existing map rows. The investigation stopped
at the missing-data prerequisite; it did not introduce a class contract, source,
header, data owner, hardcoded address, map partition, or Factory adaptation.
No compile, normal link/export, or canonical `tools/check.py --object` run was
attempted. A canonical check cannot be claimed. The complete map is unchanged.

Next prerequisite: the integrator must independently establish and add both
writable-data rows before a grounded source form can be built and checked.
