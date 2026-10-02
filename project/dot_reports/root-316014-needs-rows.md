# Root 00316014: missing basis-vector rows

Base: `f8e79c19657c5dd4d57631b6456bd353e8fe1726`.
Branch: `dot/root-316014`. Target remains U at
`00316014..00316228`, 532 bytes, pool beginning `003161CC`.
Stopped at data preflight: zero source forms, builds, or checker attempts.
No matching claim, candidate object, or source-build provenance exists.

## Required data coverage

- `004305D4`: root pool word `003161F8` supplies this address; root reads
  three floats at offsets 0, 4, 8 and negates them into a local direction.
  Known accessed minimum is 12 bytes, `004305D4..004305E0`.
- `004305C8`: root pool word `00316204` supplies this address as a reference
  argument to `001E5B8C`. Independent initialization establishes a 12-byte
  float vector, `004305C8..004305D4`.

Neither address has a containing row in the base map. Existing rows for
`004305E0` (ez), `004305EC` (ones), and `004305F8` (zero) do not cover them.
The clean `seadVector.h` already declares `Vector3<float>::ex` and `ey`.
No broader containing allocation, padding, or map ownership is inferred.

## Independent identity evidence

`__sti___14_seadVector_cpp` at `00383870..00383E00` loads floating-point
zero at `003839A4` and one at `00383AC8`, using literals `00383D68/70`.
Its `00383B10..00383B1C` sequence initializes `004305C8` to (1, 0, 0),
using pointer literal `00383D84`. Its `00383B20..00383B28` sequence
initializes `004305D4` to (0, 1, 0), using pointer literal `00383D88`.
These corroborate the previously documented ex/ey identities in
`lib/sead/README.md` and `project/dot_reports/area-cube.md` independently
of the target's desired relocations. Each object has three float members;
no binary content from these BSS addresses is assumed present in code.bin.

## Module and ABI observations

The root initializes a LiveActor using the local archive name `Nokonoko`,
then prepares rail/front direction, a subordinate actor, and nerve state.
This belongs in the Game enemy family under configured ARMCC 4.1/791.
Its init dispatch is at `003D401C` in existing row `003D4018..003D40B0`.
Constructor `003165D4` calls the established LiveActor constructor,
installs `003D4018`, clears pointers at +60/+64/+84, and initializes
two three-float fields at +6C/+78. No new dispatch-table row is needed.
The SafeString address point at `003D9C3C` is already covered by its
existing table row. Both nerve singleton addresses `003F2274/003F2288`
also have rows. All direct callees have mapped function intervals.

## Verification and handoff

Owner code SHA-256 rechecked:
`e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.
The map was never changed; no scratch names, flags, tools, configuration,
shared headers, or Factory wrappers were edited. Only this report is
committed. Integrator-owned data-row work must precede source matching.
Final report commit is the tip of `dot/root-316014`; parent is the base
above. No `tools/check.py --object` command was run because preflight
stopped before an approved project build could produce a candidate.
