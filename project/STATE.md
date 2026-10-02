# Root lane state

Owner resumed this lane on 2026-10-02 for CalendarTime placement, clean class layouts, and the port runtime. The previous planning pause is superseded by that direct instruction. Main and all acceptance gates belong to the integrator under BRIEF rule 13.

## Authority and ownership

Work only on `root/<topic>` branches from `origin/main`. Never move, check out, commit to, or push main. Never submit rank changes or edit `project/ledger.csv` or `Game/backup/src/Factory/`. Do not modify the adjacent factory repository.

Current branch: `root/calendar-constructor-placement`, base `0800d9a`. This lane owns the CalendarTime section-placement evidence and its map section override. Source family: `lib/al/src/Util/seadCalendarTime.cpp` and `lib/al/include/Util/seadDateUtil.h`, initially read-only. No other source/header ownership claimed yet.

The factory owns new unmatched functions below 0x100 bytes. Frog owns functions at least 0x200 bytes. Check open dot branches and `project/dot_reports/` before any layout-dependent function work. Do not start subagents until the operator ends the model trial; afterward use at most one.

## Current milestone and evidence

M2 and M3 remain unfinished. No level has run. Prior STATE and daily metrics describe historical snapshots and are not this lane's current acceptance results. Only the integrator's checker and verdict establish new exact credit.

Verified local candidate: `_ZN4sead12CalendarTimeC2ERKN2nn3fnd18DateTimeParametersE`, original interval at 0x002DA434. Owner reports ARMCC places its implementation in the C1-named section at 0x01000010. Verify the existing map SectionName override mechanism before changing source or symbol identity. Any map metadata fix receives its own commit containing only `map.csv` and a decision-log evidence note; ranks and boundaries stay unchanged.

## Next three tasks

1. Reproduce CalendarTime's misplaced section, correct placement, clean-build, check in simulation, and submit the branch with the original C2 row symbol as the claim.
2. Select one clean class-layout family that unblocks active factory/dot work. Ground fields in original binary offsets, record bounded uncertainties, and queue a self-contained Pro question when useful.
3. Advance a small port-runtime component toward M3 and differential replay. Keep original/native validation, unavailable state, and gameplay claims separate. Submit each finished family on its own branch while earlier submissions are verified.

## Submission and resources

Submit with the adjacent factory's `submit` command. Record its returned name and read `.integrator/results/<name>.json`; report every verdict. Push only this lane's audited branches. Continue independent work while the integrator runs the clean build and full-map gate.

Initial disk observation: 13 GiB available. Delete only this lane's disposable scratch under build when finished. Original executable and owner dump remain unchanged and local. Layout answers are proposals, not compiler or match evidence.

## CalendarTime local verification

Evidence commit `109d2ab` changes only map.csv SectionName and project/decisions.md. Fresh clean build returns zero. The normal checker reports M -> O for the 116-byte constructor; the five existing Date, Time, setDate, six-argument constructor and calcWeekDay claims all report Still matching. Checker rank effects were restored in a finally block. Committed rank stays M. Source and header are unchanged. Full-map acceptance belongs to the integrator and is pending. Local logs: build/root_calendar_baseline.log, build/root_calendar_baseline_check.log, build/root_calendar_fixed.log, build/root_calendar_fixed_checks.log.
