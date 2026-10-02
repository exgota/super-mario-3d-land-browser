# Player action eligibility 0032CBE4: runtime prerequisite

Base: `713975727c0447ea8cdea708a9ab13cc539a92d0`; branch `dot/root-32cbe4`.
Source commit: `7de61aae8`; target interval `[0x0032CBE4,0x0032CE0C)`, 552 bytes.
One grounded source form, no exact claim and no functional-equivalence claim.
The reconstruction source remains in the frozen local packet; only this note is submitted.

## Verification

`python make.py eu` compiled the committed source with configured ARMCC 4.1/791.
Its genuine `PlayerActionEligibility.o` root section is 564 bytes.
Normal linking failed on unresolved `__cxa_guard_acquire` and `__cxa_guard_release`.
Acquire has an independently grounded existing function row; release does not have
an established standalone identity or a canonical source definition in this tree.
After scratch naming the existing acquire/guard/descriptor rows:

`python tools/check.py fn_0032CBE4 --object build/eu/obj/Game/backup/src/Player/PlayerActionEligibility.o`

Exit 1: `Source closure rejected: A source helper has no unique canonical C++ definition: __cxa_guard_release`

This is not an exact-byte mismatch result: source closure rejected first.
No complete normal linked/exported target build was obtained.
Stopped after one form rather than insert an empty release stand-in, false import,
compiler flags, padding, assembly, or a modified oracle. No replay or full gates.
The complete map was restored byte for byte; no ranks, tools or game data committed.

## ABI and ownership evidence

Retail dispatch table row `003CF144..003CF188` contains this entry at `+0x18`.
Receiver `+8` is passed to `00173F3C`, which reads current node at `+0`, then
its action at `+4`; this agrees with existing PlayerActionGraph/PlayerActionNode.
Action slot 0 returns the runtime type; descriptor dispatch slot 0 tests a type.
Receiver `+4` points to a holder; its first pointer owns the two float3 inputs
at `+0x24` and `+0x6c`. The second type accepts only when their dot product is negative.
The other three successful type tests accept directly. All four preserve null-action checks.
Concrete enclosing context, motion object, and four action class identities remain unknown.

Guard/descriptor pairs, in test order: `003F372C/003F3730`, `003F37DC/003F37E0`,
`003F36BC/003F36C0`, `003F376C/003F3770`; each component has its own existing row.
All descriptors install dispatch `003D9D88`, also an existing mapped row.
Its first slot is `0039D2D8`, which tests identity and follows a base descriptor.
Helper `0028A998` sets a zero guard to one and reports whether initialization was acquired.
Retail initialization tails have no call or additional guard write; the compiler's
natural local-static form instead emits an out-of-line release call.
A justified reconstruction of that runtime lowering is the prerequisite; this is
not evidence that an original data row or standalone release row is missing.

No existing source/header defines these two import aliases on the base.
No accepted Factory/private type was edited. The older unmerged `dot/root-32d168`
proposal uses private Actor types for the same getter and explicit guards. Before
combining either proposal, reconcile its getter to the existing PlayerAction and
PlayerActionGraph types; do not retain incompatible declarations or cast around them.

## Scratch names and frozen evidence

Root row: `fn_0032CBE4`, rank M; acquire row `0028A998`: `__cxa_guard_acquire`.
For each descriptor address A above, its name is
`_ZZN23PlayerActionEligibility12typeAAAAAAAAEvE4type`; guard A-4 uses
`_ZGVZN23PlayerActionEligibility12typeAAAAAAAAEvE4type` (eight uppercase hex digits).
The frozen local packet contains committed source, object, build provenance,
checker output, patch and hashes. Binaries/provenance are local verification inputs,
not publication artifacts. The coordinator owns publication and independent acceptance.
