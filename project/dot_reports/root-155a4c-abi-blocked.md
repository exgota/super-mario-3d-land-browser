# U00155A4C: animation-query ABI prerequisite

- Base: `64d1bb9f3c97318481de9baa08de473608b9e042`, refreshed from public main on 2026-10-03.
- Branch: `dot/root-155a4c`; report only, zero source forms attempted and zero exact-byte claims.
- Target: `[0x00155A4C, 0x00155C94)`, 584 bytes including its four-byte literal pool.
- Source/final head: this report-only commit; no source or header changes.

## Independently grounded behavior

The target is a free actor animation query, appropriately owned by the `lib/al` LiveActor/Model family. It receives an actor in r0, animation-name pointer in r1, and signed channel selector in r2, and returns a float in s0. No target-address vtable/data pointer was found; no specific actor subclass or original function name is asserted.

The preceding wrapper at 0x00155A28 follows actor +0x1C, then action keeper +8, and delegates to 0x00331790 when that controller exists. Otherwise it falls through with selector -1. Controller 0x00331790 also tail-calls this root at 0x00331834 with its actor at +0, the supplied name, and -1 when no named action entry is found.

The root follows actor +0x28, keeper +0, and six model channel pointers. Existing ordinary `al::LiveActor`, `ModelKeeper`, and the source-local `ActionAnimationController` in `lib/al/src/LiveActor/alLiveActorFunction.cpp` independently support that chain and channel offsets. The older `alModelCtr` header does not describe all six observed slots and should not be expanded by guesswork.

Selectors 0 through 5 use offsets +0x20, +0x28, +0x2C, +0x30, +0x24, +0x34 respectively. Selector -1 checks those channels in that same order for an animation-name match, then returns the first matching channel's resource float at +0x14. Other selectors, or no match, return 1.0f. The only root literal is that float; no missing data row blocks this target.

## Existing accepted-contract blocker

- `Game/backup/src/Factory/group_0026246C.cpp` defines O-ranked `fn_0026246C`, `fn_002624AC`, and `fn_002624EC` as `float(Input*)`, where Input and the return-chain types are private anonymous-namespace structs. Its import `fn_0024E5E0(void*)` omits the animation name.
- `Game/backup/src/Factory/group_00262484.cpp` defines O-ranked `fn_00262484`, `fn_002624C4`, and `fn_00262504` as `int(Subject*)`, where Subject is another private anonymous-namespace struct. Its import `fn_00216E3C(void*)` also omits the animation name.
- Original helper bodies preserve r1 into their table lookups. Both 0x0024E5E0 and 0x00216E3C consume r1 as the name compared against eight-byte entries. A natural declaration for the target therefore needs the extra `const char*` argument, as well as compatible shared player types.

A case-insensitive search of all ordinary Game/al sources and headers found no established callable route for these six query addresses. `AnimPlayerSimple::isAnimExist(const char*)` is only declared and referenced by the existing `NON_MATCHING` tryStartMclAnimIfExist body; it has no mapped symbol or ordinary definition to reuse. Factory wrappers additionally disagree over return types, so they do not establish a compatible route.

Stop before implementation. Reopening requires a separately owned reconciliation of these six query contracts and the two name-lookup imports, with targeted preservation of their accepted callers. Do not invent aliases, adapters, duplicate definitions, or anonymous lookalike types in this lane.

## Verification and limits

Owner code SHA-256 verified: `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.
The current map confirms target U and all six query helpers O. No map bytes, ranks, build configuration, source, or accepted Factory files were changed. No normal make.py build, object check, replay, or full-map audit was run because the accepted-contract prerequisite prevents a valid source proposal. No object/provenance or canonical check result is claimed.
