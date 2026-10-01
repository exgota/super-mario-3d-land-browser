# Byaml data and key-name reader: canonical match

**One proposed exact function,160 complete bytes.** `_ZNK2al9ByamlIter22getByamlDataAndKeyNameEPNS_9ByamlDataEPPKci` at0033753C..003375DC passes the unchanged canonical `tools/check.py --object` gate. Main's previous527 accepted roots remain exact. Main must independently import/revalidate before adding this proposal to its accepted totals.

Base and checker commit: `e2cbf8db72566da78fc16b77a5b90026bfd9ea0a`, refreshed19:03 UTC on2026-10-01. Original EU SHA-256 remains `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.

## Grounded change after the packet's eight forms

The serialized pair's first word stores the key index in its low24 bits and the type in its high8 bits. The existing ByamlHashPair::getType returned that entire word as an enum, forcing each caller to normalize it manually. The corrected getter returns the decoded type. The reader can then use the real type API directly, while getKeyIndex remains unchanged.

This is one new API/layout hypothesis after the packet's eight structures. Initial source commitab4e3db placed the reader in a separate translation unit; canonical closure rejected its140-byte root plus a retained36-byte isTypeHash helper. Moving the same reader beside the existing isTypeHash definition, at71009e601f4cc89108c058fab92252edccdd8730, made the real predicate fully compiler-inline and produced a160-byte complete canonical match. No helper address or boundary was invented and no retained code was ignored.

The final guard-only cleanup is source commit `70f29790ec5e18c1da2ae115949c094ae91a8f93`. Removing the NonMatching guard did not change a single byte of the whole ARMCC object. The published source is the final unconditional reader. Cumulative semantic form count is nine: eight historical plus this grounded getter-API form. There were two integration compiles, one final clean rebuild and one guard-only rebuild in this lane; repeated checks/restoration are not new hypotheses.

## Canonical evidence

```sh
. ./development_environment.sh
export DEVKITARM=/usr
python make.py eu -ca
python tools/check.py _ZNK2al9ByamlIter22getByamlDataAndKeyNameEPNS_9ByamlDataEPPKci --object build/eu/obj/lib/al/src/Yaml/alByamlIter.o
```

First accepted output: `U -> O: The complete source-generated function interval matches byte for byte.`

The clean rebuild compiled, linked and exported successfully; its repeated check reported `O -> O: The complete source-generated function interval matches byte for byte.` The later guard-only normal rebuild also linked/exported and repeated the same exact result. Its entire ARMCC object SHA-256 is identical to the clean-tested object.

Canonical linked image has exactly one allocated160-byte root extent and no residual helper code. All original branches and bytes match, including type/key extraction and failure paths.

- Original and linked160-byte interval SHA-256: `ea4d91c0824e91f41dcc69246164714533133ba5dd3ce850eff194b6d81228f6`
- Complete canonical ARMCC object: `cd963a46e0853188a848670adce8dbf5c5d5c5018f927b2b42e77b10671beaab`
- Final alByamlIter.cpp: `35e1f683f39ec8c38f475acf6a2607e54c05b9c16651fe5fb64d71f01a5a6466`
- alByamlHashIter.h: `bb9f21cde638edcb9148d597eb0f3d30dbc2fcaed3cc2eb6b77f21ebc19b0070`
- ARMCC4.1/791: `d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d`
- tools/check.py Git blob: `7c0ccd93b7387a2f9afa9b304b5254f34149b318`
- tools/low/checkExactBytes.py Git blob: `ce8fc0a5d747c1521d84a1ca1fbeaeab09e3bb47`

## Preservation and integration

Full preservation checks passed527/527 unique-object roots, consisting of526 old roots plus this new root. The remaining old weak Nerve::executeOnEnd root passed separately from the canonical Game/backup/src/Enemy/Fugumannen.o object. Therefore all527 previously accepted roots remain exact and this proposal adds one more. All result records exited0 and reported full interval equality. The guard-only cleanup's identical complete object preserves this evidence without another broad rerun.

The only local identity addition names the existing0033753C..003375DC row with the symbol above. Function boundaries, pool status, code.bin, flags and tools are unchanged. Only check.py assigned O locally. No map/rank/ledger/STATE edits are published. Main owns the independent symbol/rank intake and must run its own canonical check.

This is a source/header proposal built on the stated immutable main snapshot. It does not establish M2: the complete linked game image still differs from the original.
