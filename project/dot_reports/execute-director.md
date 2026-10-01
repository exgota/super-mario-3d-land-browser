# ExecuteDirector recovery

Result: six ExecuteDirector functions pass the project's committed-source canonical-object check, totaling 1,012 bytes. The originally blocked target is `al::ExecuteDirector::init`, `_ZN2al15ExecuteDirector4initEv`, unchanged retail interval `0x001CCA94..0x001CCD40` (684 bytes). All eleven corrected execution tables additionally pass a compiled-object semantic check. Research used the owner-supplied EU executable and repository source only. The parent owns builds, map edits, commits, and acceptance checks. No binary, tool, boundary, or compiler-flag changes belong to this proposal.

## Source defects corrected

The inherited source declared eleven scalar `ExecuteOrder` records but passed them to consumers as arrays containing 125 records. This was not a complete functional implementation even if the caller instructions could match. The corrected source defines all eleven arrays: 58 update records and draw arrays of 4, 1, 2, 32, 1, 5, 10, 2, 1, and 9 records. The first entries of draw arrays 4 and 5 also had incorrect capacity 128; retail stores 64 for each.

The source remains CP932. Replacement was confined to the data declaration block and array argument syntax, with byte-preserving reads/writes. Each extracted string was decoded and re-encoded with an explicit round-trip assertion. An initial source-text comparison passed all 125 capacities and 375 strings, but compiled-object inspection caught four incorrect string values: ARMCC interprets the CP932 trailing byte 0x5C in the character used in cursor and software-reset names as a C++ escape. Draw array 6 records 1 and 9, and draw array 9 records 6 and 8, were affected. The fix doubles those four raw backslash bytes inside the source literals. The resulting C++-lexed source comparison passes all 500 semantic fields and eleven array extents. Compiled-object validation is required below; source-text comparison alone was insufficient.

## Independent import evidence

`_ZN2al20ExecuteRequestKeeperC1Ej` belongs at the existing unnamed interval `0x001DDAC8..0x001DDB44`. Its body loops exactly four times, allocates a 12-byte queue, initializes capacity/count/data at offsets 0/4/8, allocates capacity times four bytes, initializes the entries with the null-pointer constructor at `0x001D8D74`, and writes four queue pointers at this + index times four. It returns this. This independently matches the 16-byte request keeper and size_t argument.

The identity is corroborated outside the target init call. Named `alActorSystemFunction::addToExecutorMovement` at `0x0024B938` loads actor +0x18, then its first word (request keeper), passes the actor and request kind zero, and branches to `0x00252B1C`. That request method indexes the same four-pointer queue layout. Actor execution-info construction at `0x001CDCFC` stores its keeper argument at offset zero; director actor-registration code at `0x001CC820` supplies director +0x10 to that constructor. Queue consumers `0x001DD9E8`, `0x001DDA58`, `0x001DD8F8`, and `0x001DD98C` use keeper offsets 0, 4, 8, and 12 and read queue count/data at +4/+8. The allocation and consumer evidence establishes the object independently of any desired branch relocation.

`_ZN2al24ExecuteTableHolderUpdate4initEPKNS_12ExecuteOrderEi` belongs at `0x001E37D8..0x001E3B64`. Its body stores the third argument as the order count at this +0xC, allocates the order list at +0x10, traverses the second argument in 16-byte strides, and dispatches its +4 string against ActorMovement, ActorCalcAnim, ActorMovementCalcAnim, LayoutUpdate, Execute, and Functor. Constructors receive record +0 as the name and +8 as capacity. Its field writes fit the independently matched 0x44-byte update-holder constructor at `0x001E3B64`.

`_ZN2al22ExecuteTableHolderDraw4initEPKcPKNS_12ExecuteOrderEi` belongs at `0x0024B168..0x0024B56C`. Its body stores the name argument at this +0, count at +0x10, allocates a list at +0x14, and traverses the order argument in 16-byte strides. It dispatches ActorModelDraw, ActorModelDrawModelCache, LayoutDraw, LayoutDrawBottom, Draw, and Functor. Actor and layout draw constructors receive the record name/capacity. The body propagates this +0x48 and +0x4C to the appropriate draw executor, agreeing with target initialization and the independently matched 0x50-byte draw-holder constructor at `0x001E2130`. Note that this routine resumes code after its embedded string pool at `0x0024B4F8`; the existing full interval is retained unchanged.

These exact mangled import names are present in the original canonical ARMCC director object, not guessed spellings. All three original map intervals already exist; assigning names requires no boundary changes.

## Static table identities

All tables already have named, non-overlapping data rows. Retail update/draw consumers establish a 16-byte record layout independently. Their original ranges and counts are:

- sUpdateExecuteOrder: `0x003A8FA4..0x003A9344`, 58
- sDraw0ExecuteOrder: `0x003A9344..0x003A9384`, 4
- sDraw1ExecuteOrder: `0x003A95B4..0x003A95C4`, 1
- sDraw2ExecuteOrder: `0x003A9384..0x003A93A4`, 2
- sDraw3ExecuteOrder: `0x003A93A4..0x003A95A4`, 32
- sDraw4ExecuteOrder: `0x003A95A4..0x003A95B4`, 1
- sDraw5ExecuteOrder: `0x003A95C4..0x003A9614`, 5
- sDraw6ExecuteOrder: `0x003A9614..0x003A96B4`, 10
- sDraw7ExecuteOrder: `0x003A96B4..0x003A96D4`, 2
- sDraw8ExecuteOrder: `0x003A9764..0x003A9774`, 1
- sDraw9ExecuteOrder: `0x003A96D4..0x003A9764`, 9

Every record is reconstructed as typed C++ containing readable name/type/category strings and an integer capacity. No raw data blob, instruction bytes, absolute-pointer array, or substitute assembly was introduced. The existing staticd sections preserve table identity. The target caller's table-pointer literals agree with these existing rows, and its count arguments agree with every unchanged interval size divided by 16.

## Verification status

The baseline canonical ARMCC 4.1/791 director object has a 684-byte init section and the same instruction/pool layout as retail by disassembly inspection. Its unresolved imports were the three identities above; the eleven static-table section identities are already represented in the map. This inspection is not a match claim.

Source-level semantic comparison of the corrected arrays: 125 records, 375 strings, 125 capacities, 11 extents: PASS.

The parent built committed source `628f84c` and reported tools/check.py changing init from U to O for the complete 684-byte interval. That check correctly establishes the caller's function bytes, but does not validate its table contents. The subsequent compiled-table check exposed the four string errors above, so that source checkpoint is not the final proposal.

The NON_MATCHING guard was restored byte-preservingly by the parent before that commit/build and is retained in this proposal; the canonical project build explicitly enables it. A transient cloud shell transport failure interrupted one read-only inspection and the first guard-restoration attempt; it has recovered and no result depends on the interrupted commands.

The correction was committed as `15b86c1` and rebuilt by the parent with the canonical project ARMCC 4.1/791 build. Full compiled-data validation then passed: all 125 records, 125 capacities, 375 pointed-to string byte sequences, and all eleven section lengths (2,000 total bytes). Each table's ARM ABS32 relocation was resolved inside the compiler object as its symbol's section plus symbol value plus the stored addend; the complete NUL-terminated string was compared with retail. This catches C++ lexical errors, unlike the initial source-text check. Every table has exactly three pointer relocations per record.

Final validated source SHA256: `c4e590f66fd12cb33747e9e738559cc5dfd496263466f3c3955c797aa75090ba`.

Canonical director object SHA256: `c584dd3413e8f90d97e4d78e991779bcf29439674ff09a7835c90ae32b28e3b3`. The object's project-build provenance records the same source digest, ARMCC 4.1/791, stable inputs, and unchanged shared headers. The director header digest is `9ad974aec48501d135b4766b9c519d98b0782367bb4a39c0102d0ebfa4f84848`.

The full rebuilt data image is not claimed byte-exact: string addresses and global string-pool placement have not been reconstructed. The code check and compiled-table semantic check are separate claims.

## Adjacent independent identities

The same object contains five small existing director methods which also passed strict checking after their callees were named. These imports were copied exactly from the object and independently checked against retail behavior:

- `0x001E3614`: `_ZN2al24ExecuteTableHolderUpdate15tryRegisterUserEPNS_12IUseExecutorEPKc`
- `0x001E1E94`: `_ZN2al22ExecuteTableHolderDraw15tryRegisterUserEPNS_12IUseExecutorEPKc`
- `0x001E3680`: `_ZN2al24ExecuteTableHolderUpdate18tryRegisterFunctorERKNS_11FunctorBaseEPKc`
- `0x001E1FD8`: `_ZN2al22ExecuteTableHolderDraw18tryRegisterFunctorERKNS_11FunctorBaseEPKc`
- `0x001E36EC`: `_ZN2al24ExecuteTableHolderUpdate23createExecutorListTableEv`
- `0x001E2044`: `_ZN2al22ExecuteTableHolderDraw23createExecutorListTableEv`

User registration searches the Execute/Draw executor list by its +4 name and appends the supplied user via `0x001E5F0C`. Functor registration searches the Functor executor list and appends via `0x001DC870`. The update holder uses registered-count/list pairs at +0x30/+0x34 and +0x3C/+0x40; the draw holder uses +0x34/+0x38 and +0x40/+0x44. Those fields are independently initialized by the table-init dispatch branches. Each createExecutorListTable method first finishes its actor executor lists, then counts and copies enabled executors into its compact execution list.

The four tryRegister methods actually return a boolean success accumulator; the current shared headers declare void. Director callers ignore the return value, so this does not alter their generated calls. Shared-header correction is deferred and no shared header was edited in this proposal.

## Canonical checker output

On committed source `15b86c1261540e76259cb58f2ad053d8b2ed349a`, after the canonical `python make.py eu` build, the parent ran the following checks against the unmodified project object. The local map contains the independently supported names above; those local map edits are not part of the branch proposal.

```sh
. ./development_environment.sh
object=build/eu/obj/lib/al/src/Execute/alExecuteDirector.o
python tools/check.py _ZN2al15ExecuteDirector4initEv --object "$object"
python tools/check.py _ZN2al15ExecuteDirectorC1Ei --object "$object"
python tools/check.py _ZN2al15ExecuteDirector23createExecutorListTableEv --object "$object"
python tools/check.py _ZN2al15ExecuteDirector12registerUserEPNS_12IUseExecutorEPKc --object "$object"
python tools/check.py _ZN2al15ExecuteDirector15registerFunctorERKNS_11FunctorBaseEPKc --object "$object"
python tools/check.py _ZN2al15ExecuteDirector19registerFunctorDrawERKNS_11FunctorBaseEPKc --object "$object"
```

The checked log is `/tmp/mario-execute-director-family-check.log`. Its output, with terminal color controls removed:

```text
_ZN2al15ExecuteDirector4initEv
O -> O: The complete source-generated function interval matches byte for byte.
_ZN2al15ExecuteDirectorC1Ei
U -> O: The complete source-generated function interval matches byte for byte.
_ZN2al15ExecuteDirector23createExecutorListTableEv
U -> O: The complete source-generated function interval matches byte for byte.
_ZN2al15ExecuteDirector12registerUserEPNS_12IUseExecutorEPKc
U -> O: The complete source-generated function interval matches byte for byte.
_ZN2al15ExecuteDirector15registerFunctorERKNS_11FunctorBaseEPKc
U -> O: The complete source-generated function interval matches byte for byte.
_ZN2al15ExecuteDirector19registerFunctorDrawERKNS_11FunctorBaseEPKc
U -> O: The complete source-generated function interval matches byte for byte.
```

The unchanged full intervals are init 684 bytes, constructor 32, createExecutorListTable 64, registerUser 80, registerFunctor 80, and registerFunctorDraw 72. Main-line acceptance remains the main run's responsibility.

## Current-main revalidation

The parent independently rebuilt the corrected source in the separate current-main worktree using the canonical tools from main snapshot `a360142fbddd9ab875ab68314c55445ade05fd00`. Source was committed there as `ba501af`; revalidation HEAD `cd918ea` also includes an unrelated MemorySystem source edit. All six complete function intervals passed again: init O to O, the other five U to O, 1,012 total function bytes. The result was read back from `/tmp/mario-current-execute-family-check.log`. The director source digest in both worktrees is `c4e590f66fd12cb33747e9e738559cc5dfd496263466f3c3955c797aa75090ba`.

The current-main object has SHA256 `c5df5651c0f839cc439c7e222d71854bf335adb6474a8bd37c772347b346f968`. The full relocation-resolved semantic data check was repeated on this exact object, after independently confirming the current-main retail executable's approved SHA256: all 125 records, 125 capacities, 375 string byte sequences, 2,000 table bytes, and eleven section lengths passed. The selected director code and table section bytes also agree between the two worktrees despite the whole-object hash difference. No assumption about object metadata substituted for repeating the validation.

The current-main compact scaffold still has unrelated unresolved roots. These results concern the project's provenance-checked canonical ARMCC object checks and the separately verified compiled table contents; they do not claim a successful complete game link or execution. Only source and this report are proposed for publication, with no map, rank, ledger, STATE, tool, compiler configuration, or game-data changes.
