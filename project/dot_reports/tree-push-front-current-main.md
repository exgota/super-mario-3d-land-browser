# Tree proposal reverified on current main

The source from the Tree push-front proposal was applied unchanged to fresh main `3de056e0dcb33619c32f8b46ef64f74c90d28934` and committed locally as `f2f08a2b0185b5e4200baf5fa34a76a478b17183`. The new worktree had no build output before this verification. On 2026-10-01 at22:33–22:35 UTC, `python make.py eu -ca` compiled, archived, linked and exported successfully. No local main patch, import naming change or compiler configuration change was required.

The executed Python reproduction in [the original report](tree-push-front.md) then passed all four unchanged canonical `tools/check.py --object` calls:

```text
_ZN4sead8TreeNode14pushFrontChildEPS0_ U -> O: The complete source-generated function interval matches byte for byte.
_ZN4sead8TreeNode13detachSubTreeEv O -> O: The complete source-generated function interval matches byte for byte.
_ZN4sead8TreeNode27clearChildLinksRecursively_Ev O -> O: The complete source-generated function interval matches byte for byte.
_ZN4sead8TreeNodeC2Ev O -> O: The complete source-generated function interval matches byte for byte.
```

The new root remains64 complete bytes. This is revalidation of the same one-function proposal, not a second exact addition. All three accepted definitions emitted by the affected translation unit remain exact. A full663-root audit was not rerun. Main still owns acceptance.

The latest source/checker/configuration changes were inspected before testing. Tree source and headers, tools, compiler configuration and build driver are unchanged from the earlier e87 baseline. New main accepts the five older dot proposals totaling372 bytes, but Tree remains U. This note updates the tested baseline without adding main-owned metadata to the dot branch.

Fingerprints:

- Applied source: `7baffec4b181a3f1ad57356bc67a46ca099fe864e861dfd62aa01a83ce9983b4`
- Canonical ARMCC4.1/791 object: `cf403035aa687020be5300066ae4764b61167fe170784abaac8a173cb2e6d06e`
- Restored main map: `514c3ec6f2343e8a40fc73996df78fc95b2e33f4897fbdda6bb0311df6cf40fa`
- Unmodified check.py: `e177e0abe130e645e75c5f0dc24abb19dbe83310de55f1f099ec8fc385e07317`
- Unmodified checkExactBytes.py: `aef81a3a21c49bb17b8a19f139d02607899257e04dbe461ece2992cc726fc2e2`

The source SHA256 agrees with the original proposal. Object hashes differ across worktrees; complete canonical interval equality, rather than whole-object identity, is the measurement. The reproduction restored the map byte-for-byte after diagnostic rank updates. No source form, instruction search, ownership identity or flag was added in this pass.
