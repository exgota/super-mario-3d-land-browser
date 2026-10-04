# Ordinary vector import contract repair

Branch: `dot/vector-import-contracts-delivery-31d93976`.
Base: `31d93976ea7d77ce2bc9fc4580355b7c918fbe78`.
Claims: none for new functions. This is an accepted-function preservation repair.

## Change and provenance

The existing `math/seadVectorCalcCtr.h` gains ordinary `add` and `multScalar`
declarations using the authoritative VEC3 in `nn/math/math_Vector3.h`.
Fifteen selected Factory TUs call these APIs with their actual components.
Eleven local wrappers move their existing three-float storage into a zero-offset
VEC3 member; four already used that representation. Existing operators,
construction/copy order, aggregate behavior, unrelated imports and the inherited
Vector3Data wrapper in 0032787C remain. In 001BD204, the related private `sub`
call also moves to the already-established public API. No new implementation,
cast, alias, flag, checker, map, rank, ledger or STATE change is included.

API references: RE-Pepper/sead_ctr commit
`c0f95fb9c697ffc5f82fcf0b4046459a82cb5fb6`,
`include/math/seadVectorCalcCtr.h` and `.hpp`; 3dsdecomp/sead commit
`67454e68a413dbc37911236322acbb3e517e5291`, corresponding headers.
The first README credits open-ead and 3dsdecomp and records libpia_pead debug
library provenance. Neither inspected tree contains a license file. The owner
permitted these repositories on 2026-10-04. No implementation, assembly,
pragma or forced-inline attribute was copied; no upstream license is invented.

Ownership review covered fresh STATE, 252 locally fetched published refs,
open PRs, effect-math-import-8991 and the named integrator branches. Historical
October 1 proposals coin-box-follow ca32741e and effect-set-transform 37479230 touch
this header but contain no current reservation; their acceptance/release is not
claimed. No active published overlap was found. Private queues were not visible.
The coordinated exact-SHA fetch completed at 2026-10-04 03:28 UTC. Relative to
5380f863, this base adds only the unrelated 12-byte fn_00265ECC and map/ledger
updates. Cohort inputs, its 42 map rows and policy are unchanged. Earlier ref
observations are separate observations; commit dates do not prove fetch freshness.

## Preservation evidence and limits

This committed 31d93976 delivery passes the normal build/link/export and all 42
canonical checks in the 17-object cohort: O to O, preserving 4,528 original bytes.
Configured ARMCC 4.1 build 791 and all 16 source/header inputs are unchanged from
5066213c. A separate unchanged baseline and candidate on 5380f863 previously
passed the same 42 checks. No new pristine baseline or full audit is claimed here.
The cd0c0225 and 5380f863 whole-link proofs remain tied to those exact baselines.

Historical 5380 comparison: 13 complete cohort objects are byte-identical; 4 differ in
private anonymous-namespace RTTI-name checksums: 00173698, 003449B4, 003600F8,
0036844C. Every changed byte is checksum text in `.constdata` or `.strtab`.
Section order/headers/sizes, remaining data, executable bytes, raw symbol-table
entries and all relocation sections are identical. In-memory normalization of
only those checksum substrings makes the entire objects identical; no object
was modified. Compiler commands, flags and compiler hashes are unchanged.
Provenance changes only for selected source/header hashes and added real-header
dependencies; final inputs are stable. Exactly those four of 1,098 objects differ.

Linked AXF, map, script and export were byte-identical to the unchanged 5380
baseline. The following hashes describe that historical 5380 proof only; no 31d
whole-link identity is claimed. Compact output is not the complete original EU.

- AXF SHA256: `8fca7782251c1ce7045bfba73202a040334627848934bc02ac3779aa36d07b6d`
- Export SHA256: `4a46ed924092bef9d6d3bfa1482b29a878ddc3e6421c34a59b9fd6225d4c4e00`
- Map SHA256: `dae547bd21406bf82ae0ffa29f50a533a409e50f56333afb99d45d860d1a262e`

Intake dry validation runs the actual merge_submission guards with fetch/merge
blocked: ordinary dot rejects the 15 existing Factory files, as does an integrator
request without operator_approved. The operator-approved integrator case passes
pre-merge checks and stops before merge. This proves the route only; no approval,
acceptance, ref change or external write is claimed.

## Complete preservation scope

The fifteen selected objects each define their existing fn_ root:
0016D830, 00171BE0, 00173698, 001BAD90, 001BD204, 00213854, 00213B74,
0030E468, 0032787C, 003449B4, 0034D9AC, 00355820, 003600F8, 00360DC0,
0036844C. The two additional unchanged-source consumer objects define:

- group_0011D7A8: fn_0011D7A8, fn_00307F90
- group_002438D4: fn_002438D4, fn_00252B00, fn_0025C040, fn_0026CC14,
  fn_0026EFD8, fn_00277224, fn_002775AC, fn_00279ABC, fn_0027DFE8,
  fn_0028058C, fn_00289C1C, nngxlowWriteHWRegs, fn_002A7828, fn_002A7B94,
  fn_002CD5DC, fn_002D67C0, fn_002DAB20, fn_002DB2A8, fn_002DDA14,
  fn_002DDAA4, fn_002DE418, fn_002DF460, fn_002DF9F4, fn_002E00EC,
  fn_002E2FC8

Compiler dependencies confirm exactly these 17 TUs after migration. The five
original header consumers were 00171BE0, 001BAD90, 00355820 and the two groups.
The four previously composed wrappers were 00171BE0, 00173698, 001BAD90, 00355820.

Reproduce: source `development_environment.sh`, supply the configured project
toolchain, run `python make.py eu`, then
`python tools/check.py SYMBOL --object build/eu/obj/Game/backup/src/Factory/OBJECT.o`
for every definition listed above. This lane sourced the existing recovery GNU
ARM binutils environment after the project environment. Exact credit comes only
from the canonical checker. Source-only backup excludes STATE, manifests,
logs, disassembly and game bytes; detailed local receipts are retained.
