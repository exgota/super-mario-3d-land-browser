# Course-result curtain construction: 0x0018CC38

Exact proposal on `dot/root-18cc38`; two source forms, zero failed canonical checks.
The first form was superseded after ABI review; only the final natural-allocation form is proposed.
Fresh main base: `5d35374520b2e537b131ca2dba05cdcd48b725d9`.
Source commit: `af04df5f261a202186907daee7f9dc54a5e8cfc4`.
Main was refreshed before editing; this interval remained U and no competing source/report was found.

## Canonical result

Interval: `[0x0018CC38, 0x0018CE40)`, 520 bytes including its literal pool.
Source: `Game/backup/src/Layout/CourseResultCurtain.cpp`.

```sh
. /workspace/scratch/73cdb2c524af/mario-dot/development_environment.sh
export DEVKITARM=/usr
python make.py eu
python tools/check.py fn_0018CC38 --object build/eu/obj/Game/backup/src/Layout/CourseResultCurtain.o
```

The normal project build completed `Linking RE-Pepper.axf` and `Exporting code.bin`.
Unchanged checker output:

```text
M -> O: The complete source-generated function interval matches byte for byte.
```

The object and adjacent `.provenance.json` remain in the isolated worktree
`/workspace/scratch/73cdb2c524af/mario-root-18cc38` for independent verification.
Object SHA-256: `c5fa276d9f1d6466f4aef5bde66c97db7c324cb0e9bbd7c4755097e2b754baa4`.
The provenance records direct `tools.pypstem.stepBuild` ARMCC 4.1/791 compilation of committed C++.
The owner code SHA-256 was verified as `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.

## Grounding and limits

The Japanese construction label identifies the course-result curtain. The object initializes
an eight-byte NerveExecutor prefix and fields through 0x2c. It creates top and bottom curtains,
coin/time counter LayoutActors, and a one-up layout, then initializes its nerve.
The top allocation is 0x38 bytes, bottom WipeSimple is 0x34, and the other three are 0x30.
The mapped base constructor, WipeSimple and LayoutActor constructors ground the family and ABI.
The unnamed top constructor at 0x0018665C independently constructs LayoutActor and initializes
its extra fields at 0x30 and 0x34. The 0x002756F4 child constructor also derives from LayoutActor.
Adjacent course-result handlers use the parent's five child pointers and counter fields.
No direct caller or address reference to this constructor was found in the local scan.

Installed dispatch 0x003CE674 and initial nerve 0x003F1114 already have data rows.
Only the observed getNerveKeeper dispatch prefix is typed; no table owner/boundary was invented.
Allocation uses ordinary `new` expressions under unchanged project flags. The source does not
declare the mapped `_ZnwjRKSt9nothrow_t` import or assert an alternate parameter contract.
ARMCC naturally reproduces the observed call convention; the allocator body ignores its second register.
Unknown child storage and parent fields remain explicitly opaque. This is an ABI-view function,
not a complete declaration of the original class or its original overload names.
Natural Japanese literals use the repository's existing Shift-JIS convention.

Only scratch target-row changes were needed: symbol `fn_0018CC38` and initial rank M.
All original map bytes were restored after checking. No other map names, boundaries, flags,
tools, shared Factory files, or headers changed. No replay or all-O gate was run.
The integrator owns independent acceptance and ranking; this dot branch proposes source only.
