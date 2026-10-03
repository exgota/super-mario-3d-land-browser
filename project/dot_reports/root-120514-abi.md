# Root 00120514: PatanBoard functor identity is still Factory-owned

Base: `64d1bb9f3c97318481de9baa08de473608b9e042`, freshly fetched main.
Branch: `dot/root-120514`. Interval: `[0x00120514, 0x00120758)`, 580 bytes, U.
Owner code SHA-256: `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.

## Module and ABI

The factory record at 003B9EE0 pairs the ordinary string `PatanBoard`
with creator 0039604C. That creator allocates 0xC4 bytes and calls
00120924 with a SafeString name. The constructor calls
`al::MapObjActor::MapObjActor(const sead::SafeString&)` at 00280428,
then installs dispatch table 003C4DE8. Its init slot at 003C4DEC
contains 00120514. The target is therefore PatanBoard's virtual
`void init(const al::ActorInitInfo&)`, in Game under ARMCC 4.1/791.

The 0x60-byte LiveActor/MapObjActor base is established publicly.
The constructor and target agree on quaternion 0x60, initial translation
0x70, direction 0x7C, three vectors at 0x88/0x94/0xA0, floats 0xAC/0xB0,
integer arguments 0xB4/0xB8, parent 0xBC, and linked child 0xC0.
The target initializes actor/nerve, captures pose, negates quaternion-side
direction, optionally constructs the same class for linked placement 0,
applies `PatanBoardSmall` dimensions, and reads defaults 250 and 10.
It enables draw clipping, installs callbacks, then selects alive/dead state.

## Structural blocker

The stack callback object at 00120694 has the ordinary FunctorV0M ABI:
callable table at +0, host at +4, and an eight-byte member pointer at +8.
The target copies two members from existing data row 003BEBCC–003BEBDC:
001208E4 first, then 0012088C. Their behaviors are recursive kill/switch
notification and conditional appearance/pose restoration, respectively.
Calls 002787E8 and 0027FC08 consume IUseStageSwitch at host+0x0C and
FunctorBase references, selecting switch indices 4 and 3 respectively.

The public `al::FunctorV0M<T,F>` template exists, but no ordinary
PatanBoard specialization/declaration gives its existing table identity.
Table row 003D5A7C–003D5A8C remains named by the fallback dat_003D5A7C.
Accepted clone 0039B49C in `Factory/group_0039B364.cpp` references it
through a private raw Functor structure; accepted call 0039B4DC in
`Factory/group_0039B3A4.cpp` likewise owns a private raw contract.
Case-insensitive searches of ordinary Game/al sources and headers found
no reusable declaration for this instance. `project/functor-v0m-cleanup.md`
also lists this exact instance as blocked, with unnamed host/callbacks.
The factory/constructor evidence above now resolves the host class.

A natural new specialization would introduce different table and method
symbols. Renaming the existing row would break the accepted raw clone's
import; a fabricated alias, table-pointer adapter, or copied private
layout would conceal that ownership problem. Stop pending integrator-owned
migration of this instance's accepted clone/call/table identities to the
ordinary class/template contract. No accepted Factory files were changed.

## Data and verification

All direct data already have rows: member records 003BEBCC–003BEBDC,
callable table 003D5A7C–003D5A8C, and separate four-byte nerves at
003F2A68 and 003F2A6C. The installed derived table also has a row.
The SafeString address 003D9C3C is its established vtable ABI entry.
No missing-row request or adjacent-data ownership inference is needed.

Source forms: 0. No C++ candidate was committed or compiled; normal
`python make.py eu` and canonical `tools/check.py --object` were not run
because source construction stopped at the accepted identity contract.
No object/provenance or exact claim exists. The whole map is unchanged;
no scratch names or ranks were applied. No replay or full-map audit ran.
