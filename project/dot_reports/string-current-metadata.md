# Corrected string layers on reviewed current metadata

The already-published corrected string hierarchy now clean-builds on main `a4f1579f9f0f13dacfe91d92501cf74154c131ad` and preserves **all618 accepted roots**. Tested source commit: `3d18f6c03b7653094f11a5395541356c80196ac0`. New main `57f902421f6874f132d5ea971d1aa5b224c5a528` changes only project documentation; its source, map, configuration and checkers are identical to the tested base. This is a current-base integration of existing source, not a new source-search form.

The generic `FixedSafeStringBase<Character,N>` owns storage and copying. The char-only `FixedSafeString<N>` is a distinct wrapper. Main has separately reviewed and named the generic64 table at003DA280 and wrapper64/96 tables at003D9D04/003D9D18. The old wrapper64 label at003DA280 remains withdrawn. These are adopted metadata identities on main, not new exact function coverage.

## Build and preservation

After committing source, `. ./development_environment.sh; export DEVKITARM=/usr; TMP=/tmp python make.py eu -ca` returns zero, links and exports. Each of616 uniquely defined prior roots passes unchanged `tools/check.py <symbol> --object <canonical-object>` with `O -> O: The complete source-generated function interval matches byte for byte.` The two weak-definition roots are checked separately: Nerve::executeOnEnd from Enemy/Fugumannen.o and ISceneObj::initSceneObj from AreaObj/alSwitchAreaDirector.o. Both also pass O -> O, for618 distinct roots total. Multiple weak definitions are not counted repeatedly.

The serial unique-root pass took392.8825 seconds; the two weak checks took1.0273 seconds. A first inventory-only attempt discovered the newly accepted weak ISceneObj root and stopped before any comparison; the external runner was updated to enumerate both genuine duplicate definitions. No project checker was changed. All per-root outputs and object hashes are preserved in [string-current-metadata.json](string-current-metadata.json).

## Action root remains unmatched

The complete source section for `fn_001E9B30` is1240 bytes versus the unchanged1244-byte original. The prior metadata obstruction is now understood and resolved for diagnostic comparison. Three independently evidenced existing-row names are needed locally:

- 0027AD3C..0027ADA8: `_ZN2al9StringTmpILi64EEC1EPKcz`
- 0028AE64..0028AEA8: `_ZN4sead22BufferedSafeStringBaseIcE7formatVEPKcSt9__va_list`
- 0028E1E4..0028E230: `_ZN4sead22BufferedSafeStringBaseIcE6formatEPKcz`

The first is directly called by the original action root at001E9B6C. Without that actual constructor identity, the closure checker correctly retains its separate104-byte source section and rejects the residual helper. Naming it at its observed original call target is an identity proposal, not an invented helper address or a requirement that it inline. The two format entries are supported by the previously published constructor audit and the actual formatV/format call sites in this original root. No function or data bounds are changed.

With all three names on their existing rows, the unchanged invocation is:

```text
python tools/check.py fn_001E9B30 --object build/eu/obj/lib/al/src/Effect/alEffectSetAction.o
U -> M: The complete compiled section, including its literal pool, has a different size from the original interval.
```

Thus the string layers preserve current production matches, while this action body adds zero exact bytes. No new action behavioral claim is made. The original oracle, all tools and compiler flags remain unchanged. Map edits are local diagnostics only and are restored; main owns any later import-name acceptance.

## Provenance

Unchanged checker blobs: `tools/check.py`=`7c0ccd93b7387a2f9afa9b304b5254f34149b318`; `tools/low/checkExactBytes.py`=`ce8fc0a5d747c1521d84a1ca1fbeaeab09e3bb47`. EU input SHA256 remains `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`. Source and object hashes are in the JSON evidence. The compiled source is identical to the earlier corrected-layer proposal; this branch simply applies it on the current reviewed baseline.

Local logs: `/tmp/mario-string-current-clean-build.log`, `/tmp/mario-string-current-action-check-named-ctor.log`, `/tmp/mario-string-current-preservation.log`, and `/tmp/mario-string-current-weak.log`. Their paths identify local provenance; the committed JSON carries the actual comparison records. No binary is published.
