# Pending exact-source integration verification

All five pending proposals pass together, and all 569 accepted roots at main 5025a6c remain byte-exact. The clean configured project build compiles, links, and exports. No compatibility regression was found.

- Base: `5025a6cd5ec8531fb1bc40ff4b570a0c01ef201c`
- Local branch: `dot/pending-exact-integration`
- Local committed C++ source/header checkpoint: `6203886b4ef3925b282c3cab073ff8557713b5f9`
- Worktree: `/workspace/scratch/73cdb2c524af/mario-pending-exact-integration`
- This branch is a compatibility proposal for independent main intake, not an update to main's accepted totals.

## Composition

The commit changes only ten source/header files. It takes the pending Byaml getter and reader from `cf3d26bdc61227b58f161779e609c3e7b5e4e38f`, BlockDragon source/header and its required PtrArray::capacity accessor from `f4df7cad1ccab40aae9748b8d1f3eaeaa6229243`, two graph constructor source/header additions from `f15d147ee0d9b25dc8933f16dcea1eb6b9693715`, and the direct placement identifier reader from `bde3d9b7d1f20954aa3bccb7b88e5032060ffcc7`.

The older Byaml snapshot lacks main's accepted `fn_0024BB7C`. The transplant appends only the new `getByamlDataAndKeyName` body and leaves main's existing source intact. The graph transplant preserves main's out-of-line `PlayerActionNode::getAction` declaration and its no-inline definition. No source-search variants were introduced.

The guarded BlockDragon initializer accompanies its callback, but remains a NonMatching proposal with the documented unresolved table/member-pointer identities and unverified behavior. Its inclusion adds no exact credit. No table or BSS identities were changed or asserted.

## Verification

The original EU input SHA-256 remains `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`. The shared binary/compiler/venv are local symlinks; this worktree's entire build output is new and unshared.

After committing the source, `. ./development_environment.sh; export DEVKITARM=/usr; TMP=/tmp python make.py eu -ca` returned zero and exported code.bin. Only the five documented function names were assigned to existing local map rows; starts, pools, ends, classes, and sections were unchanged. Only the unchanged canonical checker assigned local O ranks. No map, config, tools, ledger, or STATE change is committed.

The unchanged `tools/check.py --object` reports `U -> O: The complete source-generated function interval matches byte for byte.` for all five roots:

| Root | Complete interval | Bytes |
|---|---|---:|
| ByamlIter::getByamlDataAndKeyName | 0x0033753C..0x003375DC | 160 |
| BlockDragonGenerator::startAppear | 0x001931B4..0x001931E8 | 52 |
| PlayerActionGraph constructor | 0x00181558..0x00181570 | 24 |
| PlayerActionGraphBuildOutputs constructor | 0x001B4658..0x001B4680 | 40 |
| ShapeModelNo reader, fn_00252EC4 | 0x00252EC4..0x00252F24 | 96 |

The preserved-name set was frozen from main before local diagnostics. The serial full preservation batch checks all 568 uniquely defined accepted roots from their canonical source objects. Every one returned zero and printed complete interval equality. The remaining accepted weak `al::Nerve::executeOnEnd` root passes separately from `Game/backup/src/Enemy/Fugumannen.o`; its 19 canonical weak definitions are not counted as 19 roots.

Thus the compatibility check covers 574 distinct roots and 30,480 complete bytes: the original 569/30,108 plus the five pending 372 bytes. Those totals are proposals, not accepted project totals. No isolated linker result substitutes for canonical checker acceptance.

Targets took 4.363s, the 568-root serial batch 325.149s, and the separate weak check 0.533s. These are measured script windows, not labor totals.

The subsequent normal `make.py eu` also links and exports with all five newly checked O rows active. It reports all 44 Game, 113 al, and one SDK source objects unchanged. SHA-256 comparison confirms every one of the 140 distinct canonical objects used by the 574 checks is unchanged after this activated-map link. No oracle rerun is needed for unchanged source/object inputs.

## Artifacts and rerun

- `/tmp/mario-pending-exact-clean-build.log`: clean configured build
- `/tmp/mario-pending-exact-activated-build.log`: normal link with the five O proposals active
- `/tmp/mario-pending-exact-postlink-object-hashes.json`: unchanged checked object hashes after that link
- `/tmp/mario-pending-exact-targets.log` and `.json`: five canonical checks
- `/tmp/mario-pending-exact-preservation.log` and `.json`: 568 prior canonical roots
- `/tmp/mario-pending-exact-weak.log` and `.json`: weak-root companion
- `/tmp/mario-pending-exact-summary.json`: full result records, object/source/checker hashes and five diagnostic names
- `/tmp/mario-pending-exact-original-map.csv`: immutable main map text used for root selection
- `/tmp/mario-pending-exact-name-diagnostics.json`: only five permitted name/checker-rank diagnostics
- `/tmp/mario_pending_exact_checks.py`: external serial runner; repository tools are unchanged

The local map is restored to main after freezing all results. To reproduce the new checks, apply the five diagnostic symbols to their existing rows, retaining all boundaries and classifications, then run the normal committed-source clean build and canonical commands listed in the target JSON. The preservation names are frozen in `/tmp/mario-pending-exact-preservation-names.json`.

M2 is still open. This audit does not establish whole-image equality, gameplay, rendering, or runtime equivalence.

Main advanced to `a4f1579f9f0f13dacfe91d92501cf74154c131ad` (618 accepted roots) after this frozen audit. That newer main has not been tested here; the complete 569-root preservation result applies only to the stated 5025a6c base.

The committed [summary.json](pending-exact-integration/summary.json) preserves all recorded checks, hashes, and command arguments from this audit. Machine-local artifact paths above are provenance references. For a new environment, replace the worktree prefix and re-run the unchanged project build/check commands; do not copy binaries.
