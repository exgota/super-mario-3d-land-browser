# Course-select state dispatcher: ABI preflight blocker

- Target: `0x001B5924..0x001B5B34`, 528 bytes, rank U.
- Base: `adf0c83ef211ec89162d6578804bf2b4d1bbe357`, fetched from main on 2026-10-02 at 19:17 UTC.
- Branch: `dot/root-1b5924`; isolated worktree `mario-root-1b5924`.
- No current-main source/report or pre-existing local/remote branch overlap was found for this root.
- Outcome: stopped before source implementation; zero meaningful compiler forms, zero exact credit.

## ABI evidence

The constructor at `0x001B5C0C` identifies its object as the course-select main state.
It calls the NerveStateBase constructor, stores its host at +0x0c and its selection
object at +0x14, and initializes the state/result fields at +0x10/+0x3c.
The installed dispatch table has an existing data row at `0x003D0CA4..0x003D0CCC`;
its entry at `0x003D0CB4` selects the target, following the initializer at `0x001B58A8`.

The target obtains a course index from `0x0026AF18`, then passes that same value
to `0x00260780` and `0x00251C4C`. The committed CourseSelectMap reconstruction
already declares `fn_00260780(int)` and uses it with an arithmetic course index.
The `0x00251C4C` helper compares its input to the index computed by `0x003273B4`.
The callee `0x0026AF18` loads two selection fields and falls through into
`0x0026AF34`, which sums prior-course counts and adds the selected course offset.
These consumers and arithmetic establish an integer return, not an object pointer.

However, accepted `Game/backup/src/Factory/fn_00372F94.cpp` declares the same
external C import as `Auxiliary* fn_0026AF18(Selection*)` and forwards it to an
`Auxiliary*`-taking declaration of `fn_0025DE7C`. The separate accepted wrapper
`fn_00327084` in `Factory/group_0024FEB0.cpp` also declares `fn_0026AF34` as
`void*(void*)`, although its arithmetic callee consumes three register arguments.
The dispatcher cannot use the grounded integer signature while preserving these
known shared declarations. Reconciliation belongs to the integrator; no casts,
alternate aliases, Factory edits, or shared-header changes were attempted.

## Data and verification

All five imported nerve objects have existing four-byte data rows: `0x003EFFE0`,
`0x003EFFB4`, `0x003EFFC8`, `0x003EFFC4`, and `0x003EFFCC`.
The three Japanese message literals are inside the target's existing interval.
No missing-row claim or new data boundaries are proposed.

The owner executable hash is `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.
The complete map stayed byte-identical to the fetched base (SHA-256
`35db0dfae859bd927591170b243af8d06514f881102ed73eef9dddcd9f36b6a0`).
No build or canonical `tools/check.py --object` command ran, because ABI preflight
stopped the attempt. No source object or build provenance was produced.
