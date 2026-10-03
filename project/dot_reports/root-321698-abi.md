# Root 00321698: earlier import blocker withdrawn

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
The original review missed the existing accepted lowercase alias described below.
The uppercase private declaration remains inconsistent, but does not require a
new incompatible declaration or fabricated alias for this target.

The earlier stop on this import alone is withdrawn; fresh target screening is required.
No accepted Factory source or shared header was changed.

## Data and verification

All direct target data references have existing complete map rows:
003B8168–003B8170 (`InOut`), 003B8170–003B8178 (`Appear`), and
003F1994–003F1998 (nerve object). The dispatch row and installed derived
vtable also exist. No missing-row request is needed.

Source forms attempted: 0. No C++ candidate, build, or canonical check was
run during the original preflight. That preflight conclusion is corrected below.
No match or progress credit is claimed. The complete map remains byte-for-byte
identical to the base; no scratch names or rank changes are required.

## 2026-10-02 correction: established typed alias

Current main `6b0e2a1385b814b70344023d9816424edcc7d832` contains the accepted
ordinary `Game/backup/src/Layout/CourseSelectMap.cpp` declaration
`fn_0027e6c8(al::LayoutActor*, const char*, const char*, int)` at line 9.
It is used by rank-O root `fn_00159fec` and other CourseSelectMap methods.
Reusing this existing lowercase symbol and public contract is distinct from
inventing an alternate export. Preserve that exact spelling and leave the
uppercase Factory declaration untouched. No new match or compilation is claimed
by this correction. The target may be reopened after current ownership and all
remaining import checks; this note does not preapprove other unresolved ABIs.

## 2026-10-03 bounded source follow-up

The reopened attempt on main `6b0e2a1385b814b70344023d9816424edcc7d832`
used the existing lowercase alias and preserved both separate string-row imports.
Three source forms were attempted; the unsupported combined-string trial was
discarded and counted. Final committed source `b64523093497f77100025aeb3acfb2dc6cee25ff`
built, linked and exported normally. Unchanged `tools/check.py` returned exit 1:
`m -> m: The linked candidate differs from the unchanged original interval.`
The complete candidate is 540/540 bytes, with 17 differing bytes across seven
instructions and an equal literal pool. This is not exact or accepted credit.
The remaining pointer setup and branch order did not justify further variants.
The NON_MATCHING candidate, object and provenance remain local; this branch
submits only the corrected status note. Original map bytes were restored.
