# BalanceTruck movement power: exact 536-byte proposal

Target: `0x0026D6C0..0x0026D8D8` (536 bytes including the local literal pool).
Branch: `dot/root-26d6c0`.
Base: `8ae3d9d55e639057950efb7ba3a0c7ccc29c5f65`.
Source head: `4e6beca57` (full identity recorded in the frozen manifest).
Final head is the report-only commit recorded in that manifest.

`BalanceTruck::updateMovePower` is a descriptive reconstruction name. The method
accelerates signed movement power, clamps its held-frame counter to +/-40,
returns the power toward zero when released, and selects Front/Back/Wait actions.

## ABI and module evidence

- Constructor `0x00135420` calls the existing `al::MapObjActor` constructor and
  installs the primary vtable at `0x003C6BDC`.
- That vtable identifies init `0x00134714` and control `0x001348B4`; local sound
  names and the factory name identify the class as BalanceTruck.
- Constructor stores and three callers (`0x00134364`, `0x00134E0C`,
  `0x001351C0`) agree on floats at 0x88/0x8C, direction integers at 0x90/0x94,
  and the signed held-duration counter at 0xB0.
- The new header derives from the existing MapObjActor class and asserts the
  0xB4 allocation size independently observed at 0x003966E8 in creator
  0x003966DC, which calls constructor 0x00135420 at 0x00396704.
  Unused fields retain their offsets as explicit unknowns.
- `al::startAction` uses its existing shared declaration. The reaction calls use
  the mapped `const al::LiveActor*` signatures. No casts or Factory edits.
- This is Game module code, built by its existing ARMCC 4.1/791 configuration.
  The target uses ordinary local literals and no new external data import.

## Canonical verification

Environment: source `../mario-dot/development_environment.sh`, then
`export DEVKITARM=/usr`.

```
python make.py eu
python tools/check.py _ZN12BalanceTruck15updateMovePowerEv --object build/eu/obj/Game/backup/src/MapObj/BalanceTruck.o
M -> O: The complete source-generated function interval matches byte for byte.
```

The final normal build linked `RE-Pepper.axf` and exported `code.bin` successfully.
The unchanged checker accepted committed source and headers from the canonical
project-built object, including the complete 536-byte section and literal pool.

Two meaningful source forms were built; one failed. The first produced 532 bytes
and the canonical size-mismatch result. Removing redundant direction locals and
using the observed clamp boundary choices produced the exact second form.
No padding, register tricks, generated assembly, flag changes, or oracle changes.

## Reproduction and handoff

Set only the existing target row to scratch rank M with symbol
`_ZN12BalanceTruck15updateMovePowerEv`. Other symbols already exist on the base.
Existing helper aliases and exact types (no scratch rename needed):
- 0x0027F244: `_ZN2al11startActionEPNS_9LiveActorEPKc`,
  `void al::startAction(al::LiveActor*, const char*)`
- 0x0026D610: `_ZN2al21startHitReactionStartEPKNS_9LiveActorE`,
  `void al::startHitReactionStart(const al::LiveActor*)`
- 0x0027845C: `_ZN2al19startHitReactionEndEPKNS_9LiveActorE`,
  `void al::startHitReactionEnd(const al::LiveActor*)`

The entire scratch map was restored byte-for-byte before the report commit.
Only source, the new class header, and this report are committed.

Canonical object: `build/eu/obj/Game/backup/src/MapObj/BalanceTruck.o`.
Object SHA256:
`0d2a7d48d6b270b8cdc96cb9ca5e1ec1fc621962a15630ebc76f892f253dc013`.
Provenance: the adjacent `BalanceTruck.provenance.json`, produced by the normal
project build; SHA256:
`935ae122c91b17798f053339fdc01046e6789402d2cfa698390a13468d2e0d83`.
The frozen local package includes source, header, report, patch, manifest, and
copies of the genuine object and provenance. No game bytes are in the proposal.

This is a locally exact proposal pending coordinator publication and independent
acceptance. No rank, ledger, tool, config, or shared-header changes are included.
No replay or family preservation gate was run, as instructed.
