# Root 00321698: blocked by an accepted private import type

Base: `8ae3d9d55e639057950efb7ba3a0c7ccc29c5f65`.
Branch: `dot/root-321698`. Interval: `[0x00321698, 0x003218B4)`, 540 bytes, rank U.
Owner code SHA-256: `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.

## Module and ABI evidence

The neighboring constructor at 003218B4 calls the accepted
`al::LayoutActor::LayoutActor(const char*)` at 0027E648, installs the mapped
003D520C derived vtable, and initializes pointers at 0x30/0x34/0x38 and bool
fields at 0x3C/0x3D after the established 0x30-byte LayoutActor base.
Its layout archive string is `PauseMenu`. The appropriate module is Game,
under its existing ARMCC 4.1 build 791 settings.

The mapped dispatch row at 003B81B4 points to 00321690. That eight-byte
wrapper reads the NerveKeeper host from its second argument and falls into
00321698. The target consumes only that host pointer; no second argument
survives as a parameter and no return value is defined on every path.
The low-confidence `unsigned int(void*, unsigned int*)` guess in
`docs/facts/00321690.md` is therefore not a usable contract for this body.
The grounded source shape is a void PauseMenu state method, apparently
`exeAppear`, with the LayoutActor base at offset zero.

The method starts the `InOut`/`Appear` layout animation on its first step,
updates menu entries 1 and 2 according to its flags and game predicates,
and repeats those updates before setting the next nerve when the animation
ends. Calls to accepted `al::isFirstStep` and `al::setNerve` agree with the
established IUseNerve base contract.

## Blocking declaration

At 003216BC, the target passes its LayoutActor-derived object to 0027E6C8.
That callee accesses LayoutActor members at 0x18, 0x20 and 0x28 and accepts
both animation strings plus the fourth integer argument. Its natural
contract is `void(al::LayoutActor*, const char*, const char*, int)`.

Accepted `Game/backup/src/Factory/fn_0017C41C.cpp` declares the same C symbol
`fn_0027E6C8` with a pointer to its anonymous-namespace `Actor` instead.
That private type has no identity usable by another translation unit and
is not the established LayoutActor class. The accepted caller is rank O;
its latest source commit is `47807304a253ba33a68c47e1c52b10ad8998de38`.
No consistent shared declaration exists on this base. A second incompatible
C declaration, cast, or fabricated alias would conceal the conflict.

Stop pending integrator-owned reconciliation of that accepted import.
No accepted Factory source or shared header was changed.

## Data and verification

All direct target data references have existing complete map rows:
003B8168–003B8170 (`InOut`), 003B8170–003B8178 (`Appear`), and
003F1994–003F1998 (nerve object). The dispatch row and installed derived
vtable also exist. No missing-row request is needed.

Source forms attempted: 0. No C++ candidate, build, or canonical check was
run because the real shared ABI contract blocks source construction.
No match or progress credit is claimed. The complete map remains byte-for-byte
identical to the base; no scratch names or rank changes are required.
