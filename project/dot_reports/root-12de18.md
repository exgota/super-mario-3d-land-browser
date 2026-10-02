# KoopaPillar initialization, 0x0012DE18

Original pinned base: `eeaca9ad7a606674b428b29630da497031ff913d` (fresh main confirmed 2026-10-02 16:26 UTC).
Branch: `dot/root-12de18`.
Verified source head: `9dfad45a` (compatibility correction).
Target: `KoopaPillar::init`, `[0x0012DE18, 0x0012E01C)`, 516 bytes including its pool.
Base rank: U. Local canonical result: O. The committed map remains unchanged.
No overlapping dot source or report was found before starting.

## Verification

From the worktree, source the established environment and select the installed ARM binutils:

```sh
. /workspace/scratch/73cdb2c524af/mario-dot/development_environment.sh
export DEVKITARM=/usr
python make.py eu
python tools/check.py _ZN11KoopaPillar4initERKN2al13ActorInitInfoE --object build/eu/obj/Game/backup/src/MapObj/KoopaPillar.o
```

The final normal build completed, linked, and exported under configured Game ARMCC 4.1/791.
Exact checker output, with terminal color escapes omitted:

```text
M -> O: The complete source-generated function interval matches byte for byte.
```

The original code SHA-256 remained `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.
Only existing-row names and target M enrollment were changed locally for checking; the entire map was restored afterward.
No tools, flags, configuration, boundaries, ranks, ledger, or STATE changes are proposed.

## Original-base names for reproduction

The names below reproduce the original pinned base; they are not missing prerequisites on the named base in the readiness addendum.

- `0x0012DE18`: `_ZN11KoopaPillar4initERKN2al13ActorInitInfoE`
- `0x0016E7A0`: `_ZN21KoopaPillarBreakModelC1EPN2al9LiveActorEPKcS4_`
- `0x0026FCB8`: `_ZN2al15PillarBaseModelC1EPNS_9LiveActorERKNS_13ActorInitInfoEPKcPKN4sead8Matrix34IfEE`

The latter two identify already mapped callees; this report makes no matching claim for them.
`0x0028CB38` retains its existing `fn_0028CB38` identity and requires no map or shared-header change.
The nerve uses the existing four-byte data row `0x003F2934` through its default name `dat_003F2934`.
Other unknown callees retain their existing automatic `fn_` names.

## Source evidence and attempts

The source comes from the authorized EU binary and current clean repository headers; no outside implementation was used.
The primary vtable at `0x003C6398` places this function in LiveActor's init slot.
Constructor `0x0012E234` calls MapObjActor and initializes the fields at 0x60, 0x64, 0x68, 0x6C, 0x70, and 0x74.
Object-name comparisons and the break-model label establish the KoopaPillar family.
The base-model constructor at `0x0026FCB8` installs the `0x003D6124` LiveActor interface.
Independent caller `0x00314214` supplies a named host joint matrix as its fifth argument, establishing a pointer ABI.
The break-model constructor at `0x0016E7A0` installs vtable `0x003CBE94` and stores host/model-name pointers at 0x60/0x64.
The two helper class names are descriptive reconstructions; their external constructors remain declarations.
The formatted model name uses the same 140-byte buffer ABI and extern-C function signature as the existing alEffectObj caller.

The original reconstruction checked three meaningful source forms:

1. Initial draft: 512 bytes; checker reported `M -> M: The complete compiled section, including its literal pool, has a different size from the original interval.`
2. Correct pointer ABI and switch without fallback: 520 bytes; the same canonical size-mismatch result.
3. Explicit fallback item initialization: 516 bytes; complete byte-exact result above.

An initial abstract-nerve declaration error was corrected before the first compiled candidate.
No padding, register constraints, volatile changes, assembly, replay, or emulation were used.

## Existing-name compatibility correction

Current main `45de738f` was inspected on 2026-10-02 before this correction. Its relevant headers are unchanged from the base, and alEffectObj already calls `fn_0028CB38` with a vtable/string-pointer/capacity/128-character buffer.
KoopaPillar now calls that existing symbol with the same buffer ABI and reads the constructed SafeString through its existing `cstr()` method. The Japanese source label remains Shift-JIS.
One correction source form was built and checked; the exact result above covers the complete 516-byte interval. The final object imports `fn_0028CB38` directly and contains no StringTmp symbol.
The map was restored in full; only this source and report changed after the original proposal.

## Named-base readiness, 2026-10-02

Identical frozen family files from `66ad8d4193dce9cfec111f354d8a34e533b1d936` were checked in local verification snapshot `94f97ef04ce620deffe8ec7aec4d7b67b08d3d57`, based on main `adf0c83ef211ec89162d6578804bf2b4d1bbe357`.
One normal `python make.py eu` build linked and exported successfully; this root passed its targeted canonical `tools/check.py --object` check for all 516 bytes (exit 0).
The checker printed `M -> O: The complete source-generated function interval matches byte for byte.`
That base already supplies the root and both helper constructor names; `fn_0028CB38` retains its established identity. No identity changes were required.
Only target M enrollment was temporary; the complete map was restored. No full-map audit was performed.
This addendum changes notes only. Source/header bytes remain identical to the frozen proposal; integrator acceptance is still pending.
