# HitSensorDirector execute: bounded whole-routine differential

Observed on 2026-10-01T13:41:26.532738+00:00. Source checkpoint `c19051346558ea3375ec87d6cc2fe1375d83a365`.

## Result

All 810 execution pairs passed under Unicorn 2.1.4 with the ARM11 MPCore CPU model and VFP enabled. This is 162 synthetic inputs across five FPSCR modes, or 1,620 whole-routine executions. No output field differed. The earlier Cortex-A9 run also passed all 810 pairs and is preserved separately. The ARM11 run took 5.659157 seconds; the maximum was 4,603 instructions on each side, below the 100,000-instruction limit per routine.

The canonical checker already reported M -> m on the committed, direct project ARMCC object. The complete 496-byte interval differs in 23 bytes across nine instructions. This validation supports a bounded NonMatching behavior claim only. It establishes no exact match, gameplay result, physical-hardware result or universal equivalence.

## Frozen inputs

- Symbol: `_ZN2al17HitSensorDirector7executeEv`, `[0x001D1DA0, 0x001D1F90)`, 496 bytes, no literal pool.
- Original EU executable SHA-256: `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.
- Frozen checker-linked AXF: `/Users/exgota/super-mario-3d-land-browser/build/exact_checks/eu/function_001D1DA0_hlvsvg82/candidate.axf`.
- AXF SHA-256: `686478850f70c5e07d3a58cf4e28dfff4b6aabcea6bb8b12a3eefc721f970c9d`.
- Candidate code SHA-256: `3dfeaf9a0c95434689f3b0059f6be68e4373104f6ec0903e2c1f366994de1c9b`.
- Direct canonical project object SHA-256: `b14e18f590727a7245659969d25f5d9b110c2d1b351fc46161c617dcee603393`.
- Full source/header/build/evidence manifest: `hashes.json`; direct compiler provenance: `canonical_provenance.json`.

The harness loads the complete approved retail executable into independent reference and candidate machines. It overlays only the candidate's source-generated execute interval in the candidate. The code region is read/execute; no helper is stubbed, edited or replaced. Both routines call the original implementations at the unchanged imports:

| Address interval | Bytes | Behavior |
| --- | ---: | --- |
| 0x002498A4–0x00249918 | 116 | Clear member sensor contact counts |
| 0x002497B0–0x002498A4 | 244 | Cross-group pair tests |
| 0x002497A4–0x002497B0 | 12 | Indexed member access |
| 0x0024977C–0x002497A4 | 40 | Capacity-bounded contact append |

Instruction hooks reject any execution outside execute and these four original helpers. Clear order is Player, Ride, Eye, Simple, MapObj, Character. The thirteen pair calls retain the complete retail order. Indexed access and insertion traces also agree in each pair.

## Inputs and compared state

The 162 inputs comprise 25 empty/count-clear fixtures, one complete group/order fixture, 42 inside/equal/outside radius fixtures across the thirteen cross-group pairs and character self-pairs, two equal-host fixtures, two directional Eye fixtures, three one-ULP boundary fixtures, sixteen quiet/signaling NaN projections, five infinity/signed-zero/subnormal fixtures, two capacity/order fixtures and 64 deterministic mixed fixtures (seed 20261001). Every group has at most four members. Group capacities retain the constructor values 16, 128, 512, 2048, 1024 and 1024. Sensor capacities include zero, one, two, three, eight and sixteen.

The five initial FPSCR modes are nearest (0x00000000), nearest with default NaN and flush-to-zero (0x03000000), toward positive infinity (0x00400000), toward negative infinity (0x00800000), and toward zero (0x00C00000). Final FPSCR, including emulated exception status, agrees in all pairs.

Each pair compares all 131,072 arena bytes by SHA-256 and each sensor's contact count and ordered pointer list. All writes are constrained to the 16-bit counts, allocated contact arrays and a 512-byte active stack allowance. Protected arena memory ranges from 130,214 to 131,072 bytes per fixture and stays unchanged. The other 65,024 stack bytes, R4–R11, S16–S31 and SP stay unchanged. The caller-saved registers and active stack frame are deliberately excluded from output equality. Code writes are prevented by memory permissions.

Observed examples:

- Equal host pointers suppress both directions. A finite boundary at distance 2 with radii 1+1 produces zero contacts. Inside distance 1.5 produces contacts; outside 2.5 produces none.
- Player/Eye produces Player count 0 and Eye count 1 containing Player, preserving the directional type-zero rule.
- Character capacities 0/1/2/3 produce [], [Character0], [Character0, Character1] and [Character0, Character1, Character2], respectively.
- Cross-group insertion precedes character self-pair insertion when capacity truncates contact arrays. Player capacity 1 retains Character0; Character capacity 2 retains Player then the first available character partner.
- Quiet and signaling NaN coordinate/radius paths preserve contacts. Ordered rejection `radiusSquared <= distanceSquared` leaves the unordered path open, unlike a positive acceptance comparison.
- The smallest subnormal radius has zero contacts under nearest but one in each direction under positive-infinity rounding. An early fixture assertion assumed zero for every rounding mode. It was corrected to the observed retail behavior before the final comparison; this was not a candidate discrepancy.

## Limits

- Synthetic bounded inputs, no game execution or replay.
- Both sides use the unchanged retail clear, pair, access and insertion implementations. Those four helpers are not independently reimplemented or validated here.
- Software ARM11 MPCore emulation is not a physical-hardware floating-point or exception proof.
- Floating control modes were enumerated because the real runtime FPSCR configuration is not established.
- All groups contain at most four sensors in deterministic mixed cases; larger production workloads and lifecycle concurrency are untested.
- The character Eye projection case intentionally exercises a type/group combination beyond ordinary Eye-group membership.
- Stack-frame contents and caller-saved registers are excluded from output equality; surrounding stack, callee-saved registers and complete arena memory are checked.

The pair/group helpers are used unchanged on both sides, so their behavior is common evidence rather than an independent C++ reconstruction proof. Synthetic state uses valid pointers and capacities; corrupt inputs, arbitrarily large groups, real game frames and concurrent lifecycle changes were not measured. Physical ARM11 VFP exception delivery remains unverified.

## Reproduction

```sh
. ./development_environment.sh
python build/hit_sensor_director_differential/verify_execution.py \
  --candidate build/exact_checks/eu/function_001D1DA0_hlvsvg82/candidate.axf \
  --source-commit c190513 \
  --output build/hit_sensor_director_differential/evidence_repeat.json
```

Keep the frozen AXF unchanged. The harness verifies the original dump hash, complete candidate section shape and unchanged AXF hash. `evidence.json` contains all inputs and both complete results per pair. No production source, checker, map, rank, ledger, compiler flags or Git state was edited by this validation lane.
