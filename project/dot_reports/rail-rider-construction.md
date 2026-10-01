# RailRider constructor: accepted-method reuse probe

Pinned main: `a4f1579f9f0f13dacfe91d92501cf74154c131ad`. Packet: `project/pro_requests/001EC128.md`. The constructor interval remains `0x001EC128..0x001EC19C`, 116 complete bytes. This notes-only proposal adds zero exact coverage and no functional NonMatching claim.

## Grounded source hypothesis

Main now contains the independently accepted `RailRider::moveToRailStart` method in the same translation unit. It performs the same normalization and position/direction calls as the constructor's tail. The fifth source form retained the original member-initializer list and replaced the two unresolved member calls with `moveToRailStart()`. This uses an actual existing method, not an invented allocation helper, qualifier, padding, or compiler override.

```cpp
RailRider::RailRider(Rail* rail)
    : mRail(rail), mCurrentPos(sead::Vector3f::zero),
      mCurrentDir(sead::Vector3f::zero), _1C(0.0),
      mSpeed(0.0), mIsLoop(true)
{
    moveToRailStart();
}
```

The body was committed as `b73ba6e406e913d75a1a7ad606987efec93f1a51` before the configured build. The compiler inlines the real method; the constructor section has only its two original normalization/calculation calls, with no extra source helper body.

## Measured result

A fresh `. ./development_environment.sh; TMP=/tmp python make.py eu -ca` compiled, linked, and exported successfully under the unchanged791 configuration. The unchanged canonical invocation was:

```text
python tools/check.py _ZN2al9RailRiderC1EPNS_4RailE --object build/eu/obj/lib/al/src/Rail/alRailRider.o
U -> M: The complete compiled section, including its literal pool, has a different size from the original interval.
```

The section is108 bytes versus116 original. This form removes the prior unresolved member-import obstruction and emits general-purpose representation copies, but still does not reproduce the original sequence. Its second copy uses LDRD/STRD where the original uses a two-register LDM/STM plus a separate word; the original coordinate reload before normalization is absent. The missing reload alone is not proof of a volatile field: the independently accepted move-to-start body passes its freshly stored zero directly. No source qualifier or address was invented to force it.

All three current accepted controls independently remain `O -> O`: moveToRailStart, moveToNearestRail, and isReachedGoal. This checks the affected translation unit, not all618 current roots. The failed constructor source is withdrawn; the main source and map are restored exactly. The branch therefore retains only this report and its evidence record.

Four main forms plus this one grounded form have been inspected. Three attempts remain under the eight-form cap, but another cosmetic rearrangement is not justified by the present evidence. The constructor remains unresolved pending an independently supported copy/initialization API explanation. No ARM behavioral replay or gameplay claim is made.

## Provenance

`tools/check.py` blob: `7c0ccd93b7387a2f9afa9b304b5254f34149b318`; `tools/low/checkExactBytes.py`: `ce8fc0a5d747c1521d84a1ca1fbeaeab09e3bb47`. The original EU SHA256 remains `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`. Candidate source/object hashes and command outcome are in [rail-rider-construction.json](rail-rider-construction.json). Local diagnostic logs are `/tmp/mario-rail-rider-method-build.log` and `/tmp/mario-rail-rider-method-check.log`; no binary or game data is published.
