# Tier 1 Sol size order

Owner decision: 2026-10-03. The driver owns `root/tier-one-size-order`,
frozen base `ec57ad8d09cec8a7916e786146216256f97212ab`. Only the integrator moves main, ranks, or ledger.

Tier 1 Sol jobs now start at 32 bytes and ascend by function body size.
Priority breaks ties, followed by job ID. This also covers the class slot
while it falls back to ordinary Sol work and the tier 2 slot when it reaches
its tier 1 fallback. Mixed groups use each function's body size. Tier 1 jobs
of 192–255 bytes wait behind smaller eligible open jobs. Already leased
jobs are unavailable for another lease; already matched rows are skipped.

The dedicated s1 256–511-byte band, Luna's sub-32-byte ownership, retry-tier
priority, models, timeouts, six-slot cap, load guard and acceptance paths
retain their existing behavior. No game build input or matching claim changes.

## Verification and deployment

Final production and public-mirror SHA-256:
`5dc4e5ee2604833b50aff93717f6bc19435d45f300f10efe5b3aa51ac15c21b2`.

Ten direct production-lease checks pass against isolated SQLite fixtures:
ascending order despite adversarial priority in s4/s5/s6 and class fallback;
group/single ordering; group-only filtering; previously matched rows; Luna
ownership; s1 reservation; and unchanged tier 2 retry priority. The old
production control selected 255 bytes first from the same fixture.

All three normal isolated reference-safety checks pass using the candidate:

- A deliberately broken proposal has 24 of 25 exact results and never moves
  the target reference.
- The valid proposal has 25 of 25 exact results and moves the reference once,
  after the last checker completes, with one final commit.
- A changed build input after checking retains the old reference and halts
  the isolated test integrator.

Candidate deployed at `2026-10-03T12:02:29.961364-04:00`,
46.9 seconds after the final safety result,
through the unchanged `restart_supervisor.sh` production procedure.

Local evidence: `logs/tier_one_size_order_direct_checks.json`,
`logs/tier_one_size_order_reference_safety_results.json`,
`logs/tier_one_size_order_deployment.json`, and the matching source patch.
Tests use disposable clones, their own databases/flags, and disabled pushes.
No production map, ledger, target binary, checker, or protected input changed.

## Three-hour observation

Measure three consecutive 3,600-second windows from the verified new
supervisor start event. Count only accepted factory `match`/`class_match`
events, excluding source submissions, attempted/checker-only bytes and test
clones. Compare each hourly count and the three-hour mean with the owner's
860–1,100 accepted bytes/hour baseline. Report timestamped 1/5/15-minute
load averages from the scheduled checks, plus enabled slots and resource
events. Load snapshots are not whole-hour mean load. Scheduler causality
cannot be isolated from concurrent port work, accepted job mix or load guard.

The existing hourly driver heartbeat collects these observations; no extra
continuous driver or trial is required. Future outcomes are not yet measured.
