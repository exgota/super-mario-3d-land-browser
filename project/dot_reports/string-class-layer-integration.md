# Corrected string class-layer integration

Based on main `4f70f6154e8d7e0e8b2d7fe1b78f7c82ab4d1654`, refreshed 2026-10-01 18:08 UTC. Tested source committed before building as local `e865296`.

The generic `FixedSafeStringBase<Character,N>` now owns storage and deep-copy logic; the char-only `FixedSafeString<N>` is a distinct empty wrapper. This follows the independent constructor/relocation audit in [string-class-layer-audit.md](string-class-layer-audit.md). The old FixedSafeString64 owner at003DA280 remains withdrawn. Generic-base64 at003DA280 and char-wrapper64 at003D9D04 remain proposed identities, not adopted map metadata.

## Measured results

- Fresh `python make.py eu -ca` compiled, linked and exported code.bin successfully.
- All 479 existing O function roots remain exact under unchanged `tools/check.py --object`:478 roots each had one canonical object; the weak Nerve::executeOnEnd root was checked separately from Game/backup/src/Enemy/Fugumannen.o. Every result was `O -> O: The complete source-generated function interval matches byte for byte.`
- The action-update root now emits1240 bytes versus the original1244-byte interval and the prior1244-byte proposal. The hierarchy change is the fourth semantic proposal, not an exact match.
- With main metadata unchanged, canonical root check rejects the unnamed formatV import. After applying only the two previously evidenced existing function-row names below, it reports `Source closure rejected: An unresolved source helper is referenced by a non-branch relocation.` Whole-table identities remain unresolved. No table rows or intervals were modified.
- Zero new exact functions or bytes; no action-update functional claim.

Canonical command:

```sh
. ./development_environment.sh
export DEVKITARM=/usr
python make.py eu -ca
python tools/check.py fn_001E9B30 --object build/eu/obj/lib/al/src/Effect/alEffectSetAction.o
```

Local-only existing-row names (bounds and original bytes unchanged):0028AE64..0028AEA8 `_ZN4sead22BufferedSafeStringBaseIcE7formatVEPKcSt9__va_list`;0028E1E4..0028E230 `_ZN4sead22BufferedSafeStringBaseIcE6formatEPKcz`. Rootfn001E9B30 was already named on main. These names are not published as map edits and main must independently review them.

Compiler ARMCC4.1/791 SHA-256 `d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d`. Checker is main4f70: check.py blob `7c0ccd93b7387a2f9afa9b304b5254f34149b318`, checkExactBytes.py blob `ce8fc0a5d747c1521d84a1ca1fbeaeab09e3bb47`. No tool/flag/local main source patches. The published source/header set is the tested set; metadata naming remains local-only.

SHA-256: header `3e0a97b55a7234785afe4bd221c531d88a8a002326b4a4dca892dd3545ef2d46`; action source `de08007803b39d30015f8c3ae5276d9ddebb956210bf4c8ba4239e5f10ed8ab0`; object `b8712e8cbb2308a230d8f98a73050e399891bbb35e2b1ee4fa3192ef0896ce09`.

This supersedes earlier source/header byte counts, while preserving their historical attempt record. Do not use old owner rows to force a canonical check. Main whole-table ownership review remains the next acceptance step.
