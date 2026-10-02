# root-261de4: blocked by accepted helper declarations

- Base and source baseline: `8ae3d9d55e639057950efb7ba3a0c7ccc29c5f65`.
- Branch: `dot/root-261de4`; final report commit is recorded in the local handoff manifest.
- Target: `[0x00261DE4,0x00261FFC)`, 536 bytes, original rank U.
- Outcome: ABI preflight blocked; zero meaningful source forms, builds, or checker attempts.
- No matching or behavioral-equivalence claim; no implementation proposed.

## Grounded behavior and ownership

This is a gameplay actor-state damage helper called from the sensor-message handler
at `0x00157DF8` (calls at `0x00158024`, `0x00158100`, `0x00158244`). It takes a
state object and message number. Its host pointer is at +0x0C, counter at +0x14,
enabled flag at +0x28, and actor group at +0x30. It clears host velocity, decrements
the counter, and invokes virtual slot +0x14 for every group actor. The group count
and array are at +0x0C/+0x10, consistent with `al::LiveActorGroup`.

Subsequent message predicates choose PressDown, Die, FireBallDown, or TailDown
host actions and install the corresponding nerves. The state prefix agrees with
`al::ActorStateBase`; the full concrete class and host extension are unresolved.
An eventual implementation belongs with Game actor/enemy state code, not SDK code.
The adjacent constructor at `0x00261CB0` was inspected but is not established as
this target's constructor and is not used as class-layout evidence.

## Blocking declarations

At `0x00261E8C`, the target supplies scalar 5 for calls to `fn_0026CCEC` and
`fn_0027455C`. Both are rank O, but `Game/backup/src/Factory/group_00252F24.cpp`
defines them as `void(void*)` and forwards that pointer type into their callees.
The target instead requires a numeric argument (the specific scalar type still
needs shared recovery). `group_0016EB68.cpp` already declares `fn_0027455C(int)`
and passes 3, confirming an existing cross-translation-unit disagreement.

The shared accessor `fn_0027768C` ignores its incoming register and selects object
4; the wrapper's original input is forwarded separately. Its machine behavior
does not justify representing the numeric argument as a pointer.

A second conflict is `fn_0026CCD0`: the target consumes its returned vector pointer
as the second argument of `fn_0026CC18`. `lib/al/src/Npc/alSky.cpp` has the suitable
`const sead::Vector3f*()` declaration, while accepted Factory forwarding code in
`group_002E3648.cpp` declares it `void()`. No consistent shared header resolves it.

These accepted declarations must be reconciled separately before this target is
reconstructed. No pointer casts, alternative aliases, or incompatible imports
were introduced to conceal either issue.

## Data and validation

All four nerve objects already have rows: `0x003F2E68`, `0x003F2E6C`,
`0x003F2E70`, `0x003F2E78` (each four bytes). The target's float and action strings
are in its own mapped literal pool. No missing-row blocker or new row is proposed.

Original code SHA-256 verified:
`e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.
`python make.py eu` and `python tools/check.py SYMBOL --object OBJECT` were not run:
preflight stopped before creating C++ under the assigned ABI-conflict rule.
No object/provenance exists. Map bytes were never edited; no scratch symbols or
ranks are required. Only this report is committed; coordinator owns publication.
