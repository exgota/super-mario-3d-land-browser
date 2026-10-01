# Factory and placement identifier packets

Base: `5025a6cd5ec8531fb1bc40ff4b570a0c01ef201c`. Branch: `dot/factory-identifiers`. Final source checkpoint: `bde3d9b`. Approved ARMCC 4.1/791 through the existing wibo, normal unchanged `make.py eu` build and `tools/check.py --object`. EU executable SHA-256 is `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.

## Result and scope

The direct ShapeModelNo reader, neutral identity `fn_00252EC4`, passes all 96 bytes at the unchanged interval `0x00252EC4–0x00252F24`, including its pool at `0x00252F20`. Its complete generated and retail interval SHA-256 is `46f557753a3ae03cdeaa96d22b71d60a53c930eb1c5124d6ff5d3b3b5d424c61`.

Both identifier predicates remain unresolved. The canonical candidate for each is 80 bytes with the same five differing bytes as the packet's best surrogate. Their unsuccessful source was removed from the final tree. ActorFactory::getCreator remains unresolved; this pass did not add another factory source candidate or target compile. No functional NonMatching or full-game behavior claim is made for these three failures.

The final source diff adds only the direct reader to `lib/al/src/Placement/alPlacementArgumentFunction.cpp`. No shared header or existing function body changes. No map, tools, ledger, STATE, compiler flags, or game data are committed. The existing function row requires its independently supported neutral name `fn_00252EC4` for normal canonical checking; its bounds and pool remain unchanged. Rank changes belong to the receiving run's checker.

## Grounded API boundary

The packet's private helpers duplicated the already accepted public integer `al::tryGetArg(int*, const PlacementInfo&, const char*)` at `0x00249B00–0x00249B50`. Both the mapped routine and the identifier reader validate placement, try the signed integer key, reject exactly -1, write only accepted values, and return success. The direct identifier reader adds output initialization to -1.

Putting this thin wrapper alongside the actual accepted definition allows ARMCC to inline that real API:

```cpp
extern "C" const char dat_003A2DBC[];

extern "C" bool fn_00252EC4( int* out, const al::PlacementInfo* info )
{
        *out = -1;
        return al::tryGetArg( out, *info, dat_003A2DBC );
}
```

This reproduces the target's 96-byte complete interval without a fabricated helper identity, retained unmapped helper, manual opcode shaping, or changed flags. The exact generic integer function is left intact. The same API factoring does not cure either predicate's register assignment, and that negative result is kept separate.

## Independent receiver and data identities

The direct reader's R1 is a placement prefix, not an ActorInitInfo pointer. Named `AreaObj::init` at `0x0027E0A4` saves its incoming AreaInitInfo address in R6 and passes it unchanged at `0x0027E0CC`, with output at `sp+0x14`. The existing AreaInitInfo class places its eight-byte PlacementInfo member at offset zero. The caller then uses the returned integer to select its area shape. At this base there is no C++ AreaObj::init definition and no competing fn_00252EC4 declaration.

A separate actor-initialization consumer at `0x002D5668` saves input R1 in R5, loads `[R5]` into R7 at `0x002D569C`, then passes R7 directly at `0x002D5704`. It repeats the pattern from `[R5]` at `0x002D5750` and calls the same reader at `0x002D5780`. That independently demonstrates use with the placement itself, and supports the published `const PlacementInfo*` parameter. A future AreaObj C++ caller should pass the address of its placement member through an appropriate accessor, rather than redeclare this extern-C symbol with an incompatible AreaInitInfo pointer type.

The first call at `0x002D5704` branches on the reader's Boolean result; the second call's consumer separately compares the integer against zero at `0x002D578C`. These are different contracts: any successfully read integer except -1 is accepted by the direct reader, including -2; the predicates return true only for a retained value >= 0. Missing/invalid input leaves the direct output -1.

The predicates independently load input field zero. Caller `0x0015E34C` saves its initialization argument in R5, passes it at `0x0015E384`, inverts the Boolean and stores it at actor+0x88. Callers `0x00303824` and `0x00309AC8` retain their initialization arguments in R7/R6 and pass them at `0x0030389C` and `0x00309B48`. This supports ActorInitInfo receiver structure, without inventing original public predicate names.

The retail pool of the direct reader and ShapeModelNo predicate references the existing whole row `0x003A2DBC–0x003A2DCC`: ShapeModelNo, terminator, and padding. Whole-row SHA-256: `ca9301d42f55b2c4fe936b9cc46a9fac0477b24a6cbec6c22709081620a85f6c`. The GenerateParent predicate references the existing whole row `0x003A2E30–0x003A2E40`, SHA-256 `ede66dcc1a887bb808aae2c312817596061a9d79d1fbe356630b2e22c5f784e3`. The imported address-derived label does not change either row's extent or grant data matching credit.

## Verification and attempts

Commands, run from the repository root after sourcing development_environment.sh:

```sh
python make.py eu
python tools/check.py fn_00252EC4 --object build/eu/obj/lib/al/src/Placement/alPlacementArgumentFunction.o
python tools/check.py _ZN2al9tryGetArgEPiRKNS_9ByamlIterEPKc --object build/eu/obj/lib/al/src/Placement/alPlacementArgumentFunction.o
python tools/check.py _ZN2al9tryGetArgEPfRKNS_9ByamlIterEPKc --object build/eu/obj/lib/al/src/Placement/alPlacementArgumentFunction.o
python tools/check.py _ZN2al9tryGetArgEPbRKNS_9ByamlIterEPKc --object build/eu/obj/lib/al/src/Placement/alPlacementArgumentFunction.o
python make.py eu -ca
```

The first canonical direct check prints `U -> O: The complete source-generated function interval matches byte for byte.` All three accepted controls print `O -> O` with that same exact result. The final reduced source rebuild links and exports, and the four canonical checks pass again. The final clean `make.py eu -ca` also links and exports; all four checks are repeated on its fresh object.

Packet history remains seven distinct structures plus one failed syntax compile: eight root-source compilations, seven companion compilations, 15 ARMCC invocations in that search. This pass adds one substantive structure, with two initial root-source invocations: the first failed because the new predicate wrappers lacked the ActorInitInfo definition, and the include-only correction compiled successfully at `05341f1`. The direct reader matched then. Two further source compilations verify pruning the failed predicates at `bde3d9b` and the final clean build; neither is a new source hypothesis. Combined targeted-source accounting is therefore eight distinct successful structures, two failed source compilations, two verification recompilations, 12 root-source compilations and seven historical companions. Unrelated translation units compiled by the required normal/clean project builds are not called additional target attempts. No compiler alternative was run and no labor-time total is inferred.

The two predicates at `05341f1` both print `U -> m: The linked candidate differs from the unchanged original interval.` ShapeModelNo differences are `0x001CD79D`, `0x001CD7A1`, `0x001CD7A4`, `0x001CD7B4`, and `0x001CD7BC`. GenerateParent differences are `0x00218B21`, `0x00218B25`, `0x00218B28`, `0x00218B38`, and `0x00218B40`. Candidate placement is R6/key R5; retail placement is R5/key R6. Their complete candidate hashes are respectively `a3b84588bc1901b756f8dd81cef4de3d8f4d73f91ae2164900c50a7f6d22e7e9` and `ed3b5a7bef5d22b5d30ce65b1be9a9999458b9dc2cd07fe2aa6347b4d8b7de79`. No cosmetic reorderings were tried afterward.

## ActorFactory review, no new compile

The original getCreator interval and pool remain `0x00268EB0–0x00268FB8` and `0x00268FAC`. Whole-interval SHA-256: `96bcff2e19841f2e5930ff3d5e380ac564c639ad2f273f1a9473ec8ce22aca2c`. Existing packet history stays six distinct ordinary C++ structures and 12 compiler invocations. The best packet forms remain 264 bytes, nine bytes different in the initial zero/count setup. The previously retained conversion helper grows the closure to 280 bytes. No new API or data-flow observation justified repeating those forms.

Independent reads confirm the factory+4 Byaml iterator created by the constructor at `0x0026902C`, the ObjectName/ClassName pools, the 225-entry bound, and eight-byte registry stride. The factory callers at `0x00144800`, `0x0019CD00` and `0x00274FEC` corroborate creator resolution; the `0x0019CD00` caller subsequently invokes the returned function pointer with the input name.

The original registry row `0x003B99F0–0x003BA0F8` is exactly 1,800 bytes, SHA-256 `65739891b98bb1398a398bd74b3927c8d1808003025a0b5011e1057458d8c8fe`. All 225 callback words are non-null, pairwise distinct, and starts of existing mapped function intervals. This is bounded identity evidence, not recovery or acceptance of their bodies. The production table's null callback entries are scaffold placeholders; using that incomplete source table for a full factory behavior claim would be unsound.

Retail conversion has no fallback to the original object name. It ignores the child-iterator success result, initializes the name outputs per iteration, preserves the three short-circuit call decisions, and stops conversion after the first successful ClassName read. A null resulting ClassName exits with null; it does not continue to a later conversion entry. Registry resolution returns the first matching callback and otherwise null. Those bounded control-flow observations agree with the packet's translation, but neither execution of the complete graph nor a differential behavior test was performed.

ObjectName's existing whole row `0x003BA0F8–0x003BA104` has SHA-256 `68fbbbeaeb899df259d05b1d3c8f519151f1520573af77db4aeb4c7ed07fd9b2`. ClassName's existing row `0x003BA104–0x003BA118` has SHA-256 `a59338a3fc723a27200f48da546917ce4296b1bdab31155f130cbd3af0aebd16`; bytes after its terminator include nonzero values and are not assumed to be ordinary zero padding. No string-row split, tail ownership, or new public API name is proposed.
