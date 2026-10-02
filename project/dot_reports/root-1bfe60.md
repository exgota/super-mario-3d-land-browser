# Actor timer update: exact local proposal

Base: `713975727c0447ea8cdea708a9ab13cc539a92d0`.
Branch: `dot/root-1bfe60`.
Final source commit: `4ab575e68b82f0a35fd79e0baa9983ab0213760a`.
The handoff manifest records the final report commit and file hashes.
Root: `001BFE60`, complete interval `[001BFE60,001C008C)`, 556 bytes.
Symbol: `_ZN2al10ActorTimer6updateEv`.
Local work: 2026-10-02 22:46–22:56 UTC; two meaningful source forms, one miss.
This is a local exact proposal; only the integrator awards accepted credit.

## Owner and behavior

`ActorTimer` is a descriptive reconstructed name, not an asserted original name.
Constructor `00270F00` installs existing dispatch row `003D61BC`, whose update
entry is this root. The constructor and six callers establish a 24-byte helper
with `al::LiveActor*` at +4, blink flag +8, stopped flag +9, fixed-period flag +A,
remaining frames +C, blink threshold +10, shown-transition flag +14, sound flag +15.
Callers include accepted `00314C30`; no caller or Factory source is changed.

The update decrements active timers, chooses TimerNormal above 90 frames and
TimerFast otherwise, and stops on expiration, clipping, or actor death.
The blink phase uses four-frame periods in fixed mode, otherwise three below
45 frames and five above. Hidden/shown transitions update the model and Blink
sound; stopping restores visibility when applicable and plays TimerEnd.
The host flags are dead +54, clipped +55, hidden +59. The +8 audio-interface
conversion agrees with the existing LiveActor multiple-inheritance layout.
This is shared actor support in `lib/al`, built with its configured ARMCC 791.

## Source closure and imports

New files are `alActorTimer.h`, `alActorTimer.cpp`, and `alActorTimerFlag.cpp`
under the existing LiveActor include/source directories. The separate dead-flag
predicate reads the signed character representation used by the original.
It follows the existing committed signed flag-reader pattern; it does not cast
an actor to an unrelated private type. All three flag readers disappear during
normal linker inlining; the exact closure retains only the 556-byte root.

The closure also uses unchanged `alLiveActorFlag.cpp` for clipping/hidden readers.
Existing `fn_0026a9fc(LiveActor*)`, `al::hideModel(LiveActor*)`, and
`fn_00268df8(IUseAudioKeeper*, const sead::SafeString&)` identities are preserved.
Audio helpers `001BF140`/`001BF250` dispatch through the audio interface's +8 slot;
the former receives the event string and integer 2, the latter the event string.
The prior ResultTimerUpdate proposal is a distinct UI owner; its reconciled
`fn_0027109C` declaration is untouched. No accepted types or APIs are repaired.

## Validation

Both committed forms were built by the normal project build and linked/exported.
Form 1 (`719f56150`) produced 572 bytes: canonical checking rejected its oversized
closure. Its local show/hide wrappers preserved a host pointer across predicates.
Form 2 retains member reloads across those predicates, as the original does.

```
. /workspace/scratch/73cdb2c524af/mario-dot/development_environment.sh
export DEVKITARM=/usr
python make.py eu
python tools/check.py _ZN2al10ActorTimer6updateEv --object build/eu/obj/lib/al/src/LiveActor/alActorTimer.o
M -> O: The complete source-generated function interval matches byte for byte.
```

Final root object SHA-256:
`0632e701eab7f965f45dbf2ddb4810e8027461a9ce75832d2534b4daab8117da`.
The handoff retains genuine project objects and provenance for the root and both
flag-reader source objects. The owner executable hash was verified before work.
No compiler flags, checker, boundaries, tools, or accepted source were changed.
No replay or full-map preservation gate was run.

Scratch mapping: set the existing `001BFE60` row to the symbol above and rank M.
All other imports resolve through existing rows/names, including string rows
`003B2AA8`, `003B2AB0`, and SafeString vtable `003D9C34` plus its ABI prefix.
The complete original map bytes were restored; no map/rank/ledger/data is submitted.
