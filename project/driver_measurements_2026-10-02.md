# Driver measurements, 2026-10-02

Observed at 2026-10-02T17:06:23-04:00. This report measures retained worker evidence and canonical main separately.

## Draft carryover

The read-only `runs`/`jobs` database join covers completed production runs. A run has a draft only when its saved `.factory/job.md` output contains the earlier-worker draft marker. A negative classification requires a complete saved packet; incomplete evidence stays unknown. Model trials and provider errors are excluded. Timeouts remain in the denominator when the packet is observable.

| Saved packet | Runs | Runs with a worker match | Worker match rate |
| --- | ---: | ---: | ---: |
| unknown | 13 | 7 | 53.85% |
| with draft | 30 | 3 | 10.00% |
| without draft | 574 | 345 | 60.10% |

Excluded: 101 model-trial runs and 3 provider-error runs. Worker matches are the factory's post-run checker results, before the integrator accepts source; these rates do not measure accepted bytes.

The aggregate comparison is confounded by task selection: drafts mostly appear on harder retry jobs, while the no-draft group includes many small first attempts. In the closest retained stratum, Sol xhigh tier 2 single functions of 32-255 bytes, drafts yielded 3/23 matching runs (13.04%); no-draft runs yielded 0/2. That denominator is too small to establish a carryover benefit or harm. Preserve the current policy; this is an observational report, not a new experiment.

The complete per-run classifications and model/effort/tier/kind/size strata are retained locally in `~/super-mario-3d-land-factory/logs/driver_measurements.json`. They can be regenerated with the local `measure_driver_results.py` using only the standard library and read-only inputs.

## Accepted bytes after the credits crossing

The interval starts at 2026-10-02T16:34:00-04:00 and ends at the observation above (0.539759 hours). The first-parent main checkpoint immediately before the crossing is `8376271323f77bdf7bde070ceb180835f0553c3a`. The frozen current checkpoint is `e2336037d4f709acc082b3322daedadf9500564a`.

Canonical exact intervals increased from 112,756 to 113,400 complete bytes: 644 net accepted bytes, or 1,193.12 bytes/hour. There are 22 newly exact functions, 0 lost exact functions and 0 changed exact boundaries. Both totals use function rows whose rank is O and their complete end-minus-start interval.

This interval includes integration time. It does not attribute account-wide credits to the factory, and it does not imply a sustained future acceptance rate.
