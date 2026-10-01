# Project state

Last updated: 2026-10-01 (initial setup by Claude, before the first model session).

## Current milestone
M0: toolchain settled. See `project/BRIEF.md` Section 6, Phase 0.

## Done
- Repository created from RE-Pepper with history; unclean libraries removed; all ranks reset to `U`.
- Native macOS toolchain verified: build, diff, check and progress all work.
- First clean match: `nn::os::detail::ConvertSvcToLibraryPriority` (0x0010766C), ARMCC 4.0/902.

## Next tasks
1. Write clean sead headers in a new `lib/sead` module, driven by the includes in `Game/` and `lib/al`.
2. Enable `lib/al`, then `Game`, in `data/config.json` until both compile.
3. Match 3 small game functions and settle ARMCC 4.1 build 791 versus 894 for game code.
4. Then start the Phase 1 pilot (50 functions) and stop for the owner's review when it is done.

## In flight
Nothing.

## Blockers
None.
