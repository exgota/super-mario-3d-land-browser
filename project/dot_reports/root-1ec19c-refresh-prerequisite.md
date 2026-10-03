# Actor initializer 001EC19C: refreshed contract preflight

Base: `64d1bb9f3c97318481de9baa08de473608b9e042`, freshly fetched main.
Branch: `dot/root-1ec19c-refresh`; the old published branch is unchanged.
Target: unnamed U row `0x001EC19C–0x001EC3BC`, 544 bytes;
literal pool starts at `0x001EC398`. Concrete actor identity is unproved.

## Result

The old init-info prerequisite is obsolete. One remaining private receiver
contract stops source work. Zero source forms, builds, or canonical checks;
no exact, nonmatching, demotion, or runtime-fault claim.

## Cleared: fn_0027D1DC initialization-info type

The old `dot/root-1ec19c` report stopped at a private ActorInitInfo type.
Accepted `Game/backup/src/Factory/fn_0018938C.cpp` now declares
`void fn_0027D1DC(int*, const al::ActorInitInfo*)`.
Commit `d0557ac53e037afb8840568f638e6fe2191c42e3` made this type public;
the later Teresa cleanup `b1d4a94a8593766f11513e0cb2f82c9b90f6e790`
preserves it. This caller passes its initialization argument at `0x001EC20C`
and consumes the output as a signed integer. The old private-info blocker
must not be carried forward.

## Usable: existing integer fn_0027D180 contract

`Game/backup/src/Enemy/Bubble.cpp` already declares and uses
`bool fn_0027D180(int*, const al::ActorInitInfo&)`.
Bubble::init is O at `0x0030324C–0x003033C0` on this base, so this is an
established ordinary public route, not a proposed alias or adapter.

Independent target evidence supports it: the local at stack +0x44 starts
at integer 100, is passed to this helper at `0x001EC218`, and undergoes
signed-integer-to-float conversion at `0x001EC248`. The callee reads an
integer through accepted `al::ByamlIter::tryGetIntByKey` at `0x0027E068`,
rejects the -1 sentinel, and conditionally writes the integer output.
Factory still declares `void fn_0027D180(float*, const al::ActorInitInfo*)`.
That older conflict is disclosed; it is not a new blocker for this target
because Bubble already provides the binary-supported public contract.

## Unresolved: fn_00270AB0 receiver type

The target's first call, at `0x001EC1B0`, forwards its receiver and info to
`fn_00270AB0`. Accepted Factory root `0x0018938C–0x00189480` remains O and
declares this import as `void fn_00270AB0(Actor*, const al::ActorInitInfo*)`,
where Actor is its anonymous-namespace type. That receiver remains private.

Independent callee evidence supports LiveActor: it accesses the pose
keeper at +0x14, constructs ActorPoseKeeperTRSV when absent, then applies
placement transforms. Its accepted setters at `0x0027F394` and `0x0026E778`
are public `al::setTrans` and `al::setScale` with LiveActor receivers.
`alLiveActor.h` defines the corresponding pose-keeper member.
Case-insensitive Game/lib searches found no other established source alias
for `0x00270AB0`. The public LiveActor signature in
`docs/facts/0018938C.md` is explicitly inferred prose, not a usable import.

The accepted Factory owner must reconcile this receiver contract before
retrying this target. No Factory/shared repair, invented alias, or adapter
was attempted. This is the surviving receiver prerequisite also described
by `dot/root-145830`; its old private-info wording is no longer current.
Other layout/data/callee prerequisites were not exhaustively screened.

Original executable SHA-256 verified:
`e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.
Only this note changes. No map edits or external writes; scratch map bytes
equal base. No object/provenance packet exists for this preflight-only note.
