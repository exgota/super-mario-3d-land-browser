# Integer argument readers: packet 002794F8

## Result

**All ten remain unmatched. Zero new exact functions or bytes.** Each retained candidate is 92 bytes, with 38 differing bytes. Canonical checker returns `m -> m`. The bounded replay passes 3,840 original/candidate/independent-model pairs (384 per function), including actual retail BYAML parsing helpers. This is bounded behavioral evidence, not exact coverage or universal functional equivalence.

Base/main and checker commit: `4f70f6154e8d7e0e8b2d7fe1b78f7c82ab4d1654`. Source was committed before building at local `92120c22719a12175eab2fa4f6f2ed352f15f6ad`. Canonical `tools/check.py` blob `7c0ccd93b7387a2f9afa9b304b5254f34149b318`; `tools/low/checkExactBytes.py` blob `ce8fc0a5d747c1521d84a1ca1fbeaeab09e3bb47`. No tools, flags, original intervals, or binary changed.

## Bounded follow-ups to the historical eight forms

1. Float-reader helper argument order `(info, key, out)`, grounded in the independently matching reader already on main. Commit `5afddf770cb5c092d8568a290483971c0dea758c`: all ten `U -> m`, 92 bytes, 38 differences. No improvement over packet best.
2. Wrapper-local integer and outer copy, grounded in the independently matching Boolean-reader structure. Commit `e874df770e625b6c8c71a288a6c73ee5c56f3076`: all ten `m -> M`, 88 bytes. Worse.
3. Restored form 1 at source commit above; this is a restoration, not another hypothesis. All ten `M -> m`; fresh clean rebuild/check then `m -> m`.

The historical eight-form cap is not reset. Two new evidence-backed hypotheses failed to improve; no further cosmetic control-flow permutations are proposed. A new clue about the original shared inline helper or ARMCC register allocation is needed.

## Build/check evidence

`python make.py eu -ca` completed the clean compile, link, and code.bin export on the stated main snapshot with the committed source. No local source/tool patches beyond the proposed new file. Existing metadata rows were named locally to expose these source symbols; all original address/size boundaries stayed unchanged. Those map changes are not published and main must independently adopt/revalidate any identities.

For every symbol below, run:

```sh
. ./development_environment.sh
export DEVKITARM=/usr
python make.py eu -ca
.venv/bin/python tools/check.py SYMBOL --object build/eu/obj/lib/al/src/Placement/alPlacementIntegerArgumentFunction.o
```

ActorInfo symbols are `_ZN2al10tryGetArgNEPiRKNS_13ActorInitInfoE` for N=0 through 8. The ByamlIter symbol is `_ZN2al10tryGetArg2EPiRKNS_9ByamlIterE`. Exact evidence for each final invocation: `m -> m: The linked candidate differs from the unchanged original interval.` These are source-closure checks, not raw object-byte comparisons.

| Address | Interface / local row identity | Bytes | Differing bytes | Rank | Linked candidate SHA-256 |
|---|---|---:|---:|---|---|
| 0x2794f8 | Arg0 ActorInitInfo | 92 | 38 | m | f2d96b384e059d9ec5a20676f20528ab35fffaf9fda7f561a84266622a703dc4 |
| 0x27d1dc | Arg1 ActorInitInfo | 92 | 38 | m | d97a74b5068c4410db0fae5bf8d099c31635e71a92da7eaddbda3ae851bb6d66 |
| 0x27d180 | Arg2 ActorInitInfo | 92 | 38 | m | 16c3a6bf80594b8fd6df677caa962f289571611f50ca0674f4b2a616cd2a6b59 |
| 0x27aff8 | Arg3 ActorInitInfo | 92 | 38 | m | 9aaa0abb5ac663271e34144549ed5358fe127d543c3fc08411c8b9af84c1c61a |
| 0x2693d8 | Arg4 ActorInitInfo | 92 | 38 | m | 6665c5811c432791976151447a3e01789c8f02f3cb26cf7a0b92627c3fd07954 |
| 0x2671d4 | Arg5 ActorInitInfo | 92 | 38 | m | 2b757d96fc71762949581f49e5cf5b1e1ca656a323e9757f2ce1930134f24ede |
| 0x266148 | Arg6 ActorInitInfo | 92 | 38 | m | 9a235075c556d041a82475437d101755a854b34fd09e4f990222cd8f9c83b4ad |
| 0x2730f8 | Arg7 ActorInitInfo | 92 | 38 | m | fc48b9d85ddb29e87e051cb68248f58c626e8f6bf500f4801ebd9c462ae9ec8d |
| 0x1bd080 | Arg8 ActorInitInfo | 92 | 38 | m | 84a600016ed94707f8b3b559e4a684a3674d363267d31c497243698957f7902d |
| 0x2670d8 | Arg2 ByamlIter | 92 | 38 | m | bc7c05170a6cef7988e50ab44d7d337cc27bfcd91a876a7f085d7b07687a6beb |

## Replay scope

The companion [replay](integer-arguments-replay.md) executes original and canonical-linked candidate code on Unicorn 2.1.4 ARM1176. Actual `isValid`, integer lookup, BYAML hash/key search and string comparison execute; no imported helper is mocked. Twelve scenarios times 32 values cover valid integers including -1 sentinel, invalid/null views, missing keys/pairs, empty key/root tables, non-hash roots and four wrong value types. Checks compare all non-stack memory, Boolean result, helper-call trace, SP/callee-saved registers, and an independent key/type/sentinel model. Maximum 308 instructions. Candidate hashes are pinned above and result JSON contains original hashes.

Excluded: malformed offsets, aliasing, reentrancy, concurrency, fault behavior and physical hardware. Constructed views do not establish complete file-header validation. Full linked game hash does not match the EU original; M2 is not reached.
