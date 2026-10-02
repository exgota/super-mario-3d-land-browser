# GhostPlayer state root 0012C470: shared import ABI blocker

Base: `adf0c83ef211ec89162d6578804bf2b4d1bbe357` (fresh-main reservation).
Branch: `dot/root-12c470`. Target remains U, complete interval
`[0x0012C470, 0x0012C680)`, 528 bytes; literal pool starts `0x0012C648`.
Zero source forms, builds, or canonical checks; no match or progress claim.

## Preflight

The owner code SHA-256 is
`e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.
No source/report/local-branch overlap was found for this root.
The existing GhostPlayer constructor at `0012CBE8` establishes the 0x60
LiveActor base and state bytes at 0x69, 0x6A and 0x6C. Existing source and
header are `Game/backup/src/Player/GhostPlayer.cpp` and its matching header.
The nerve dispatch at `0012C468` precedes this separately mapped body;
the installed execute entry is in existing data row `[003B1224,003B1234)`.
This is first-step disappearance state logic with a ten-step transition.

All literal data required by this root has existing map coverage:
SafeString vtable `[003D9C34,003D9C50)`, GhostHide string
`[003B1178,003B1184)`, and destination nerve `[003F0EF4,003F0EF8)`.
The other strings and 300/150 scalar constants are within the root pool.
There is no missing-data-row blocker and no inferred table extension.

## Blocking shared declaration

The root's call at `0012C56C` targets `fn_0022700C`, passing its actor in
r0 and a three-float position reference in r1. The accepted source at
`Game/backup/src/Factory/group_00223D40.cpp` defines that symbol as
`void* fn_0022700C(void*)` and forwards only that argument to
`void* fn_00227010(void*)`.

The four-byte original row immediately forwards into `00227010`. That body saves
r1 at `0022701C`, then uses it as a vector input at `00227090`, `002270C4`
and `002270F8`. The second argument is live; a one-argument declaration
cannot express this call. Adding an incompatible two-argument declaration,
calling through a cast, or changing the target alias is not an acceptable
local workaround. Reconcile the shared wrapper and downstream import first,
then resume this root with that compatible signature.

Also review `fn_00270460`: the existing Factory declaration uses an int for
its second argument, while this root supplies a position address. Its body
forwards that value to a virtual call. This is an ABI representation issue
to resolve consistently, not grounds to add another incompatible import.

No Game/lib/header/Factory edits were made. No scratch map edits were made;
full map SHA-256 remains
`35db0dfae859bd927591170b243af8d06514f881102ed73eef9dddcd9f36b6a0`.
There is no object or provenance artifact because preflight stopped before
implementation. No replay, preservation sweep, tool, config or flag changes.
