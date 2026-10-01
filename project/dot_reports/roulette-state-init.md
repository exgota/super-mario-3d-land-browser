# Roulette state constructor: bounded source proposal

Branch: `dot/roulette-state-init`. Base: immutable main `e2cbf8db72566da78fc16b77a5b90026bfd9ea0a`, refreshed through the GitHub connector on 2026-10-01 at 19:12 UTC. Target: the previously untouched U interval `0x00187478..0x0018790C`, 1172 bytes, with existing pool start `0x001877E4`. No existing packet, report or reserved actor covers it.

**Result: zero exact functions and zero exact bytes.** The complete constructor is preserved as a source proposal under `NON_MATCHING`, but this guard does not claim behaviorally validated NonMatching status. All three committed-source forms compile, link and export with the project build. The unchanged canonical checker rejects their unresolved vtable import before comparing bytes. The final compiler section is 1176 bytes, four longer than retail. Register allocation also differs. No equivalence replay was performed.

## Identity and layout

`ItemRouletteState` is a descriptive reconstruction name, not an established original C++ spelling. The embedded Shift-JIS name is `State[ルーレット]`. The constructor calls the accepted `al::NerveStateBase::NerveStateBase(const char*)` at `0x002684E0` and installs address point `0x003CDF2C`.

Two independent callers establish the total allocation and ABI. The actor initialization at `0x0013EF5C` requests 0x40 bytes at `0x0013EF80`, then calls the target at `0x0013EF9C` with `(this, host, ActorInitInfo reference, false)`. The initialization at `0x0018BC70` requests 0x40 bytes at `0x0018BD90`, then calls it at `0x0018BDB0` with the same ABI and `true`. Both store the returned state and register it through `al::initNerveState` at `0x001CACB4`. The final clean ARMCC header accepts `sizeof(ItemRouletteState) == 0x40`.

| Offset | Recovered field | Evidence |
|---|---|---|
| +0x00..+0x0B | NerveStateBase | Accepted base constructor and its independently recovered 12-byte layout |
| +0x0C | Host LiveActor pointer | Constructor stores the second argument; display creation and attachment read it |
| +0x10 | Selected item index, initially 1 | Co-stored with host, read by adjacent roulette routines and final selection search |
| +0x14 | Placement integer, initially -1 | Address passed to Arg7 and then Arg6 integer readers |
| +0x18 | Whether Arg7 was present | Cleared first, set only when the Arg7 reader succeeds |
| +0x1C | Integer counter, initially zero | Constructor stores zero; semantic name remains provisional |
| +0x20..+0x30 | Five display actors | Star, mushroom, leaf, fire flower, boomerang flower, each a four-byte pointer |
| +0x34..+0x3C | Three-float placement offset | Three zero stores; address passed to 0x0027EFB0 unless the final argument is true |

Form 1 reused ActorStateBase's inline constructor. It forced the host store before the derived vptr/member stores, unlike retail. Form 2 uses direct NerveStateBase inheritance and owns the host member. This reproduces the observed joint host/index store and produces the correct 0x40-byte layout. The original base-class spelling is not thereby proven.

## Recovered constructor behavior

The constructor clears all five display pointers, records whether Arg7 exists, and then attempts Arg6 into the same integer. It passes either its offset vector or null to the placement initializer according to the fourth argument. Arg0 selects the item set, defaulting to zero.

The connection/setup call at `0x002742E0` receives local string `DummyModel` and capacity four for sets 0 and 3, three for set 1, or five for set 2. Other integer values bypass that call. A star display is omitted only for set 3. Mushroom and leaf displays are always created. Fire flower is omitted for set 1; boomerang flower is additionally restricted to sets 2 and 3. The leaf model switches between `SuperLeafSpecial` and `SuperLeaf` according to the predicate at `0x0016BB58`.

Each display path first calls its item-resource helper, then factory `0x00256ACC` with `(host, info, Japanese display name, model name, shared DummyModel string, null)`. The returned actor is attached through `0x0027ED20`, hidden through accepted `al::hideModel`, stored into the appropriate pointer slot, and uniformly scaled to 0.7 through `0x0027E9EC`. Retail reloads the host between factory and attachment; the final source preserves that load opportunity instead of caching the host in an inlined helper.

The final bounded search starts from the selected index and carries each remainder result to the next iteration: `next = (next + 6) % 5`. It stops at the first present display, tries at most five slots, and falls back to slot zero if none exists. It passes the selected pointer to `0x0026A9FC`. Form 3 corrects the initial source transcription to preserve this carried value. The constructor does not add an extra null guard absent from the original.

## Import evidence

Every address-derived function import begins an existing, unchanged function interval. This is an ABI reconstruction, not a claim that the import's original name is known.

| Address | Observed role |
|---|---|
| 0x002730F8 / 0x00266148 / 0x002794F8 | Existing Arg7 / Arg6 / Arg0 integer-reader intervals |
| 0x0027EFB0 | Host/placement initialization with nullable vector pointer |
| 0x002742E0 | Forwards host, placement, model name and capacity; retains a non-null returned connection |
| 0x0011BFDC | Star resource wrapper; reads host resource and enters item-holder pool at +0x3C |
| 0x00226C98 | Mushroom resource wrapper; corresponding holder pool at +0x20 |
| 0x00227988 | Leaf resource wrapper; prepares ordinary and special leaf pools |
| 0x0022735C | Fire-flower resource wrapper; corresponding holder pool at +0x0C |
| 0x00225CE8 | Boomerang-flower resource wrapper; corresponding holder pool at +0x28 |
| 0x0016BB58 | Model-selection predicate; its full gameplay meaning is left unnamed |
| 0x00256ACC | Allocates a 0x88-byte display actor and forwards the observed six arguments |
| 0x0027ED20 | Loads host's connection keeper at +0x50 and forwards child with type 2 |
| 0x0027E9EC | Obtains the actor scale through its pose object and writes the scalar to all three components |
| 0x0026B000 | Calls the division runtime and returns remainder from r1 |
| 0x0026A9FC | Actor/display activation path; kept address-named |
| 0x003C0F7C | Entire existing 12-byte data row contains the shared `DummyModel` string |

No import was assigned to an interior function address. Original Japanese labels are ordinary escaped string literals recovered from this constructor's existing pool. No target instruction arrays or assembly are present.

## Vtable ownership blocker

The emitted C++ relocation names `_ZTV17ItemRouletteState` with addend +8. Retail loads address point `0x003CDF2C` from literal `0x001877F8`. Therefore the corresponding complete C++ table begins at `0x003CDF24`. Current map ownership is:

- Named AquariumSwimDebris row: `0x003CDE8C..0x003CDF2C`
- Unnamed row: `0x003CDF2C..0x003CDF54`

The two zero ABI header words at `0x003CDF24/28` are currently in the preceding named row. The unnamed row in turn ends with the next object's two header words at `0x003CDF4C/50`.

Independent neighboring constructors support the ownership boundaries. Accepted AquariumSwimDebris construction at `0x00186D44` loads `0x003CDE94` from `0x00186D6C`, writes it as the primary vptr, and forms its other interface address points at +0x68, +0x74 and +0x88. Its last observed virtual entry is `0x003CDF20`, the accepted stage-switch initializer thunk. The following layout constructor at `0x00187CDC` loads `0x003CDF54` from `0x00187DA4` and writes this primary vptr and derived interface pointers at `0x00187D08`.

The roulette table's complete observed range is `0x003CDF24..0x003CDF4C`, 40 bytes:

| Offset from address point | Entry |
|---|---|
| -8 / -4 | Zero ABI header words |
| +0x00 | 0x00331520, accepted NerveExecutor::getNerveKeeper |
| +0x04 / +0x08 | Zero destructor slots; these are not callable destructor identities |
| +0x0C | 0x0018730C, roulette init override |
| +0x10 | 0x001E2DB0, NerveStateBase::appear |
| +0x14 | 0x001E2DA4, NerveStateBase::kill |
| +0x18 | 0x001E2DBC, NerveStateBase::update |
| +0x1C | 0x001E2DF0, NerveStateBase::control |

Table SHA256: `b2e16e5ae557df3685af642e19d25b010c7272fdba7bd5d933df2a66bc4e145f`.

This is a separately reviewable data-ownership proposal. No table name, function boundary or data boundary was changed. A prior-owner shortened table, full roulette table and successor header would require an independent main-lane data repair before the vtable can be mapped honestly. Assigning the vtable symbol to the current address-point row would shift every constructor reference by eight bytes and is not a valid workaround.

## Compiler and checker evidence

Compiler: configured ARMCC 4.1 build 791 through the approved existing wibo. Commands run after sourcing `development_environment.sh` and setting `DEVKITARM=/usr`:

```sh
python make.py eu
python tools/check.py _ZN17ItemRouletteStateC1EPN2al9LiveActorERKNS0_13ActorInitInfoEb \
  --object build/eu/obj/Game/backup/src/MapObj/ItemRouletteState.o
```

The checker requires a named root. Each check used only a local Symbol-field proposal for the existing U root:

```csv
0x00187478,0x001877E4,0x0018790C,          ,U,f,_ZN17ItemRouletteStateC1EPN2al9LiveActorERKNS0_13ActorInitInfoEb,
```

No other row, field, rank or boundary was changed. This local name proposal was restored after checking and is not part of the branch.

| Form | Source commit | Compiler section | Result |
|---|---|---:|---|
| 1 | c3402e7 | 1208 bytes | Project build links/exports; canonical check rejects unresolved vtable |
| 2 | f286945 | 1176 bytes | Direct base and literal display blocks; same gate rejection |
| 3 | 5ab7a9f | 1176 bytes | Correct carried remainder; same gate rejection |

All three unchanged checker calls print exactly:

```text
Source closure rejected: An unresolved source helper is referenced by a non-branch relocation.
```

The three forms are below the eight-form cap. A pre-build escaped-literal correction is not counted as another form. Further structural searching stops on the established vtable ownership blocker. The remaining size and register-allocation differences must also be resolved before an exact claim; table repair alone does not make this candidate exact.

Final source SHA256: `3208f0875aa25b99a08c351eb4be1c521b6164df5efb35c0848eb938b8f95df3`.
Final header SHA256: `6c5c1ce2b72d4f559934b9daa4ca64adf3a0257868f22fe2e87d243ffaac706b`.
Final canonical object SHA256: `f11201cff31ccd16b44b9b194a90a6d351842d9ccc92088133196f6411944ad6`.
Retail target interval SHA256: `00c03974180b94ccc56d786b2916374f430adfb4ceeecbfa6a23294a77d2517d`.
EU code.bin SHA256 remains `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.

Final verification at 19:24 UTC: `python make.py eu -ca` exits zero, links `RE-Pepper.axf` and exports `code.bin` with the original map restored. The fresh canonical object reproduces the final object SHA256 above. The unchanged `verify_build_output` accepts source provenance and configured compiler `4.1/791`. Resolving the root's actual external relocations through the existing map finds 19 established import identities and exactly one unresolved symbol, `_ZTV17ItemRouletteState`. No helper function remains outside the constructor section.

The restored map SHA256 is `140aeaee501c03d5d2b3b3b83a13db8b99af029fc86c54527592998fc8ccd3e9`. Temporary build/check logs are `/tmp/roulette-state-form{1,2,3}-{build,check}.log` and `/tmp/roulette-state-final-clean-build.log`; canonical objects and provenance stay under the worktree's ignored `build/`. No game data is committed or shared. The branch contains only two source/header files and this report; map, tools, ledger and STATE remain unchanged.
