# Dual-screen method tree constructor proposal

## Result and acceptance blocker

**Zero accepted exact credit.** Canonical `tools/check.py --object` rejects unresolved source closure. The unchanged project-built candidate independently links to all 1,284 original bytes when bound to the observed, but not yet accepted, whole-vtable identity. This is strong diagnostic evidence awaiting main's metadata review and canonical revalidation, not an O claim.

Target: `_ZN4sead23DualScreenMethodTreeMgrC1Ev`, `002E3144..002E 3648`, 1,284 bytes. Main `4f70f6154e8d7e0e8b2d7fe1b78f7c82ab4d1654`, refreshed 2026-10-01 18:08 UTC, ranked U. All fourteen queued packets already had bounded attempts or active identity work, so this is the next large U reconstruction.

## Source iterations

1. Binary-derived node/manager fields and straightforward constructor: 1,288 bytes. The name field was modeled as a raw pointer and ARMCC reordered name stores broadly. Source committed as local d83215b; moved into the enabled Actor source module at f05d985 before the actual compile. Initial clean setup had not compiled the disabled Sead source path, so it is not counted as a function attempt.
2. Independent MethodTreeNode constructor evidence reveals an embedded SafeString at+0x24, pointer at+0x28. Correcting the field type and name setter gives 1,284 bytes. Committed before project build as local c7fa3e2. No further codegen form is needed: independent linked bytes agree completely.

The source is under `lib/al/src/Framework/` because the project's Sead module currently has no enrolled sources. Header remains under `lib/sead/include/framework/`. No configuration or compiler flag changed.

## Canonical results

```sh
. ./development_environment.sh
export DEVKITARM=/usr
python make.py eu
python tools/check.py _ZN4sead23DualScreenMethodTreeMgrC1Ev --object build/eu/obj/lib/al/src/Framework/seadDualScreenMethodTreeMgr.o
```

With main names unchanged: `Source closure rejected: A source helper has no unique canonical C++ definition: _ZN4sead14MethodTreeNodeC1EPNS_15CriticalSectionE`.

With only the existing 0028B818 constructor row named locally as C1 instead of C2: `Source closure rejected: An unresolved source helper is referenced by a non-branch relocation.` The unresolved allocated relocation is `_ZTVN4sead23DualScreenMethodTreeMgrE`. No table rows or original intervals were modified. The full normal project build links and exports successfully. A subsequent fresh `python make.py eu -ca` also completed compile, link and export on the final committed source; canonical closure rejection and the diagnostic identical hashes were reproduced afterward.

Canonical checker commit is the stated main base: frontend blob `7c0ccd93b7387a2f9afa9b304b5254f34149b318`, exact checker blob `ce8fc0a5d747c1521d84a1ca1fbeaeab09e3bb47`. Compiler ARMCC4.1/791 hash `d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d`.

## Binary ownership and layout evidence

- MethodTreeMgr base occupies 0x20 bytes. Its constructor 002DBF14 installs a vptr and constructs the critical section at+4; eleven node construction calls start at+0x20 and advance 0x50 bytes each.
- MethodTreeNode constructor 0028B818 independently writes the name string vptr at+0x24 and pointer at+0x28, callback at+0x48, pause word at+0x44, critical-section pointer at+0x3C and user pointer at+0x4C. No virtual-base construction is observed; the complete/base constructor alias at this address needs main review. Root's eleven member-constructor BLs all target that exact address.
- Root size is 0x394, with final Boolean fields+0x390 and+0x391. Unknown fields stay opaque. Node names, eight child links, eight unpaused nodes and three paused roots come directly from the retail constructor and its inline callback/lock sequence.
- Literal 002E3544 contains 003DA678, actually installed at 002E3150. The proposed whole table base is 003DA670 (address point+8), through 003DA694, 36 bytes: two zero headers; runtime-type-info slot 0036E938; zeroD1/D0; attach slot 00222B28; zero root-query slot; pauseAll slot 002E2FE8; zero pauseAppCalc slot.
- The following table has zero headers at 003DA694/698 and independent address point 003DA69C. Literal 002E07F4 is loaded at 002E06BC and stored at 002E06C4 by a separate construction path. Another independent path loads the same address point from 002E08BC and stores it at 002E0838. This bounds the proposed 36-byte table without consuming its neighbor.
- A minimal main-owned boundary repair would preserve the exact union of existing data rows 003DA61C..003DA678 and 003DA678..003DA69C: fragment 003DA61C..003DA670, named whole table 003DA670..003DA694, fragment 003DA694..003DA69C. This is a proposal only. No dot map patch is supplied and no function/original byte boundaries change.

The permitted open-ead/sead README was reviewed first; its later-title reconstruction and guessed inline names are acknowledged. Only API shape was consulted: MethodTreeMgr virtual interface, MethodTreeNode naming/pause interface, and INamable's SafeString field. All CTR offsets, logic, node names and target bindings above come from the owner's binary. No platform implementation or matching hacks were used.

## Independent diagnostic link

The companion [link reproduction](dual-screen-method-tree-diagnostic.md) links the complete unmodified canonical object at 002E3144. It imports five independently observed function addresses and the proposed vtable base. It does not change the oracle, object, target interval, source code or data rows. This separate diagnostic does not replace the canonical closure gate.

Both complete 1,284-byte intervals SHA-256: `8292cf0596dce49ae6eeaea21765728ac736875f33f2812b5e9c2dfdf08d1922`. Differing bytes: 0. Canonical object SHA-256: `23340b4055298f203954dc86ef 36480fabcc1c7fbaaaf3c5c7b7f1f565549434`.

Main must review the constructor alias and whole-table identity, rebuild this source under its current checker, and obtain genuine `--object` acceptance before counting the function. No game bytes are included in this branch.
