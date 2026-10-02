# Actor init 0x00313AF8: accepted private-type ABI blocker

- Base: `e2336037d4f709acc082b3322daedadf9500564a`, refreshed from public main.
- Branch: `dot/root-313af8`.
- Target: `[0x00313AF8, 0x00313D18)`, 544 bytes, rank U.
- Existing receiver: `[0x003135E0, 0x003137E0)`, 512 bytes, rank O.
- Result: blocked before source changes; zero compiled forms or checker attempts.

The accepted `Game/backup/src/Enemy/ReceiveMsg3135E0.cpp` and its report were
inspected first. The shared retail vtable identifies this target as the same
actor's init method. Init stores EnemyStateBlowDown at +0x64 and the receiver
flag at +0xCC, agreeing with the accepted declaration. An owned actor-header
refactor could preserve that layout, but it cannot resolve the separate import
below.

Init calls `fn_00242A38` at call site 0x00313CA0 with the actor and its current
translation. That existing, accepted 12-byte wrapper occupies
`[0x00242A38, 0x00242A44)` and is rank O on this base. Its definition is in
`Game/backup/src/Factory/group_001EC0E8.cpp`, introduced by commit
`a3e9096d276c9296234c7effa0c204c543ad405c`:

```cpp
extern "C" void fn_00242A38(Owner* self, const sead::Vector3<float>& position);
```

`Owner` is an anonymous-namespace type in that translation unit. The wrapper
reads its pointer at +0x40, then a nested pointer at +4, and calls the existing
RailRider::moveToNearestRail method. LiveActor independently has its rail keeper
at +0x40, confirming the actor operation without making the C++ types identical.
The private Owner type cannot be named from a new actor translation unit. A
LiveActor pointer declaration would conflict with the accepted definition;
repeating Owner elsewhere would create a distinct type. Neither a cast nor a
second alias would establish a consistent declaration.

Unblocking requires an explicitly scoped reconciliation of that accepted
Factory wrapper into a shared, compatible declaration, followed by a focused
check of its affected roots. Factory changes are outside this task's scope.
No missing data row has been claimed, and no map row or boundary was changed.

No source/header edit, build, or canonical checker call was performed. Both
actor roots retain their original source and ranks; the accepted receiver was
not changed, so its recheck was not needed. No exact claim is made for init.
No object or build provenance exists for this report-only result.

The local EU code SHA-256 was verified as
`e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.
The scratch map remains byte-identical to the base, SHA-256
`8e490e86394ddd3b867c5a217297f8c441c2ca5eb06bc0d64ec4551ddaaaeabc`.
Only this compact note is included in the proposal.
