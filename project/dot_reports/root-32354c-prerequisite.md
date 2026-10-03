# SuperLeaf initialization: typed nerve prerequisite

- Target: U `0032354C`, `[0032354C,003237A0)`, 596 complete bytes; pool starts `00323720`.
- Refreshed main/base: `64d1bb9f3c97318481de9baa08de473608b9e042`.
- Local branch: `dot/root-32354c`; compile-only draft head: `81bdd822c`.
- Outcome: blocked before normal linking; no canonical result, match claim, or exact-byte credit.
- One compile-only source form. No code-generation experiments or further source forms.

## Blocker and proposed prerequisite

The independent initializer `003819FC` installs distinct Nerve virtual tables in each existing row from `003F16D0` through `003F16E8`. These are separate objects, not a proposed array or grouped data owner.

Accepted `Factory/group_003460E0.cpp` declares `dat_003F16D0` and `dat_003F16E8` using its private empty `al::Nerve` stand-in. Reusing those declarations with the ordinary public `al::Nerve` fails because that type is abstract. The source lane excludes private Factory contracts and does not change this accepted unit.

The integrator prerequisite is to reconcile those two imports with their independently grounded concrete Nerve types. A small candidate repair is to forward-declare the existing `FlowerInitNerve<unsigned int>` template in that Factory unit, use the two address-specific concrete types, and explicitly convert their addresses in the three existing forwarding bodies. This is a proposal only; affected accepted roots require targeted preservation before intake.

No Factory file, shared header, or data row was changed. No new alias, replacement storage, or stand-in was introduced. The unsupported draft is preserved only in the local handoff packet; this branch's final delta is this report.

## Module and ABI evidence

`0027A94C` installs actor dispatch `003D5474`, whose init slot points to this target. The root's archive string is `SuperLeaf`. The constructor clears fields `+0x64/+0x68` and installs a further interface pointer at `+0x60`; the draft preserves these positions without claiming the original inheritance chain.

The root reuses the accepted `FlowerInit::Param/Appear/Forward/Vertical/Release/Ground` declarations and constructor aliases unchanged, plus existing `fn_002801B8`. It allocates a 0x20-byte Param and registers six states. Existing byte-address declarations for `003F16D8/DC` remain address imports, with pointer conversions justified by the initializer.

The extra constructor at `00141630` is independently grounded: its caller allocates 0x14 bytes, and it calls `al::NerveStateBase` with a label meaning “item leaf state,” stores the LiveActor at `+0x0C` and the same Param at `+0x10`, installs separate dispatch `003C7FFC`, and initializes its nerve using `003F1C1C`. Its dispatch preserves base getNerveKeeper/init/update/control and overrides appear (`00141424`) and kill (`00141418`).

`SuperLeafInitState` is a proposed descriptive identity for that extra helper. Its original name is unrecovered, and this proposal is outside the owner's prior six-helper naming approval. The draft gives it a separate class and constructor identity.

## Actual verification

Approved shared ARMCC 4.1/791 and wibo; source the existing development environment and set `DEVKITARM=/usr`, then:

```sh
python make.py eu
```

Exit 1 during Game compilation, before target object creation or normal link/export:

```text
SuperLeaf.cpp:18:28: Error: #322: object of abstract class type "al::Nerve" is not allowed:
    function "al::Nerve::execute" is a pure virtual function
SuperLeaf.cpp:24:28: Error: #322: object of abstract class type "al::Nerve" is not allowed:
    function "al::Nerve::execute" is a pure virtual function
```

No `tools/check.py --object` call was made: no canonical target object/provenance exists. No replay, preservation run, full-map audit, or tool/config edit was performed.

Scratch-only names were `0032354C -> _ZN9SuperLeaf4initERKN2al13ActorInitInfoE` (rank M) and `00141630 -> _ZN18SuperLeafInitStateC1EPN2al9LiveActorEPN10FlowerInit5ParamE`. Complete original map bytes were restored. Dump SHA-256 verified `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.
