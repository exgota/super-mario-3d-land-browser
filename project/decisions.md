# Decision log

Append only. Newest entries at the bottom.

## 2026-10-01: Setup decisions (made by Claude during environment setup, approved scope from the owner)

**Removed RE-Pepper's CtrSDK, NintendoWare and sead submodules.** Their READMEs say CtrSDK and NintendoWare were "referenced from a debug binary that was taken from a purchased and dumped hard drive, which a still active game studio threw out", and that sead was referenced from a `libpia_pead.a` downloaded from debugging.games. Brief rule 6 forbids leaked material. All 76 map entries upstream had ranked `O`, `M` or `m` were in that code, so they were reset to `U`.

**Toolchain runs natively on macOS through wibo's macOS build.** Running wibo inside an x86_64 Linux container under Rosetta failed with `rosetta error: invalid gdt selector index 4`. decompals/wibo 1.2.0 ships `wibo-macos`, which runs armcc under Rosetta 2. Changes: `tools/low/glob.py` adds `isMacOS()` and `needsWibo()`; `tools/pypstem/callProcess.py` and `tools/pypstem/manSetup.py` use them, and setup downloads `wibo-macos` on macOS.

**Build changes needed after the removal.**
- `lib/CtrSDK` is now a clean module with a hand-written `include/nn/types.h` (the global preinclude needs it).
- `tools/pypstem/stepLink.py` skips modules that produced no library, so header-only modules link.
- `tools/pypstem/stepSplit.py` always imports `__ctr_start` and emits a strong stub for map functions flagged `g`, because the entry point was previously supplied by the removed SDK code.

**Fixed `tools/progress.py`.** Line 73 had a syntax error (`"d" sym[...]` instead of `"d" in sym[...]`).

**Compiler evidence so far.** The SDK module's configured compiler, ARMCC 4.0/902, produced a byte-exact match for `ConvertSvcToLibraryPriority`. This says nothing about game code. Game code's build (4.1/791 versus 4.1/894) is untested.

## 2026-10-01: Require literal-pool bytes before accepting a match

The existing checker invokes asm-differ with `-I -i`, which ignores addresses and large immediates, and disassembles only through the map's pool boundary. An assembly score of zero therefore does not prove the full function interval byte-exact. `tools/check.py` now requires a direct comparison of the unchanged map interval, including the literal pool, before returning `O`. No target bytes, boundaries, differ settings, or progress logic changed.

Validation: the existing SDK function remained `O`. Flipping one byte of its literal pool in the ignored generated build output produced `m`, and restoring the output produced `O` again. The original dump was never modified. Symbols without a valid executable address now return `U` instead of `O`.

## 2026-10-01: Enable source files incrementally

The brief requires enabling Game and lib/al one file at a time. The build previously scanned every source recursively. An optional module `source_files` list now selects paths relative to its source directory, fails for missing configured files, and filters compilation commands. Selection changes invalidate the module archive so removed files cannot survive through cached objects. Existing modules without a list retain recursive discovery. Global compiler flags remain unchanged.

## 2026-10-01: Initial game compiler comparison

Both ARMCC 4.1/791 and 4.1/894 produced the same six byte-exact PlayerTrigger leaf functions using unchanged global flags. The project checker then accepted both clear methods and both set/isOn overloads under 791. The constructor and al::calcHashCode compiled with different register choices from the target under both compiler builds. These probes do not distinguish the compiler and M0 remains open.

The existing Game source remains in backup/src and backup/include; configuration names those directories rather than creating duplicate source paths. Ledger token and wall-time fields for these initial probes are coarse estimates allocating shared analysis across functions. Actual subscription billing is unavailable, so the cost field says unavailable rather than inventing a dollar value.

## 2026-10-01: Restore required source-language declarations

Existing source uses nullptr even though ARMCC 4.1 implements C++03, and uses staticd for explicit symbol sections without defining it. The global preinclude now supplies a zero null-pointer constant and the existing static-data section convention. The clean integer-types header includes standard stdint.h for uintptr_t used by Byaml headers. These repairs change declarations and macro availability, not compiler flags.

## 2026-10-01: Clean header integration and source compile audit

The new lib/sead module contains 14 headers reconstructed from local retail assembly and clean game call sites. Its README records observed offsets and explicitly leaves unknown runtime behavior as declarations. The first isolated source audit compiled 104 of 110 existing Game/lib/al translation units with the project flags.

The six failures required a missing SafeString include, three declaration-only clean SDK call headers, Togezo's undefined sdata macro spelling, and the broken scene-factory source/header. The factory at 0x00166AC8 allocates 12 bytes, passes 23 to SceneObjHolder, and loads callback 0x0015DF30 from its literal at 0x00166AF0. GhostPlayer's constructor passes 20 at 0x0012CC4C; rp::createCoinRotater passes 7 at 0x00276588. Only these needed scene IDs are declared. SceneObjHolder's mapped constructor uses a void* callback return, which its header now reproduces. The factory callback remains an external declaration rather than invented actor-creation behavior.

After these repairs, the audit compiled 110 of 110 translation units under ARMCC 4.1/791. This establishes compilation only, not runtime correctness or matching. The direct compilation audit logs remain ignored in build/source_audit.

An inherited __nop intrinsic deliberately standing in for an instruction in alSensorFunction was removed under hard rule 3. It is no longer a permissible matching aid. Its function remains nonmatching. Searches for __nop, sdata( and SCENE_OBJ_LIST return zero hits in Game and lib/al. The global asm declaration macros remain because they support the brief's low-level SDK exemption.

## 2026-10-01: Full integrated ARMCC build passes

The normal `python make.py eu` build now compiles all 35 Game and 75 lib/al source files, archives both modules, links and exports successfully under 4.1/791. Rechecking the six trigger matches and the original SDK match after the full build kept all seven at O. Existing incomplete source still emits warnings, including an uninitialized heapSize and missing return in tryGetPlacementInfo. Compilation is not evidence that those routines work, and they remain outside accepted matching counts.

## 2026-10-01: Hash register allocation recovered in C++

Removing calcHashCode's dead initial character load leaves the actual loop assignment unchanged and reproduces the target's r2 character / r1 accumulator allocation. Six equivalent expressions were compiled in isolation; only the uninitialized local followed by the existing loop assignment reproduced the full 44-byte interval. The normal build and check.py then promoted the function from m to O. This is a Phase 0 attempt, not part of the capped pilot. Both compiler builds reproduce these bytes, so this match does not settle M0.

## 2026-10-01: Name two lookup intervals without changing their boundaries

The unnamed function at 0x00140E94..0x00140EDC searches 12 pointers at 0x003E2968..0x003E2998 and returns the matching index or -1. Its entries are PhotoOpening, PhotoWorldInterval01 through 07, PhotoEnding, PhotoEndingLuigi, PhotoLuigi and PhotoComplete. The table is named sPhotoScenarioNames from this observed content. The function keeps the address-based name fn_00140e94, since no original semantic symbol is established.

The unnamed function at 0x00148C7C..0x00148CC8 searches five eight-byte records at 0x003EFA40..0x003EFA68 and returns the corresponding value or -1. The records are Exception=0, Map=1, Entrance=2, Object=3 and Event=4. The table is named sPlacementCategoryEntries from this observed structure; fn_00148c7c retains an address-based function name. Both functions call the existing isEqualString at 0x00292308. The symbol-name fields alone changed; starts, ends, pool boundaries, types, ranks and target bytes remained unchanged. These are reconstructed labels, not claims of recovered original names.

## 2026-10-01: Verify each compiled function at its original addresses

The compact stub link changes addresses, so it cannot prove externally linked calls and literal addresses byte-exact. The checker now isolates a complete compiler-generated function section and its relocations, turns other definitions into imports, and links only that section at the original map address. Symbol definitions contain independently established original addresses from map.csv. No target instructions or bytes enter link inputs. The final comparison includes the complete literal pool, validates the EU executable hash and rejects unknown addresses, shared sections, wrong sizes, wrong output addresses and unexpected allocated sections.

Validation: ByamlStringTableIter::findStringIndex links its strcmp call to 0x0028AA60 and matches all 108 bytes. isPadTriggerA also matches after the same-source definition of isPadTrigger is converted to an import at 0x0024CA4C. Unknown allocator symbols and a wrong-sized PlayerActionMultiCondition section are rejected. A one-byte mutation of the compiled SDK literal pool is rejected with one differing byte; the untouched object is accepted. The existing SDK, hash and trigger checks retain O under the new path.

The new tool's direct invocation avoids tools/low/glob.py shadowing Python's standard glob module. The checker locates object paths by source path plus archive/object identity; the inherited .syms cache currently records incomplete fromelf symbol lists, so membership in that list cannot establish function identity. The object ELF itself must contain the exact requested compiled definition. The old assembly differ remains a diagnostic for mismatch severity and never establishes O.

## 2026-10-01: Final sensor compiler evidence and measured attempt floor

The final production sensor lookup is byte-exact under791 (84bytes) and fails894 (80bytes), checked with identical source hash b42d3a8bd68398dcad506e075040217ef5352e50f127f572e0c1fdec6680f3da and aggregate header hash 9907262d5f675901cf64ce4e2b16bcdf757253ed9142be95aca4635e95d7f0b5. Commands, compiled bytes and strict linker evidence remain in ignored build/compiler_probe/final_sensor_evidence.json. A permanent rerunnable compiler verification will be recorded when all three M0 functions are ready.

The scratch search measured41 distinct sensor source revisions, each compiled under both versions. The final paired production check added one evaluation. Five root audit/integration builds also compiled the sensor function, giving a measured lower bound of47 attempt iterations; the ledger's earlier rough40 was corrected. Earlier repeated whole-source sweeps cannot all be counted from persisted logs and are excluded from that floor. Token/time fields remain estimates. The pilot's eight-iteration cap does not apply to this Phase0 compiler-settlement work.

## 2026-10-01: Distinguish legacy word similarity from exact coverage

Inspection showed progress.py's byte percentage compares individual four-byte words in the compact image, including nonmatching functions and non-BSS data rows. Its matching function count comes from ranks, but its byte percentage is a similarity score and its Total Functions includes data. Earlier daily percentages quoted that tool output; they must not be read as exact coverage. The progress script is unchanged under the brief's oracle-integrity rule. Exact coverage reports will use only function rows gradedO, with their full unchanged mapped intervals. This avoids claiming partial word coincidences as reconstruction.

## 2026-10-01: Use compiler invocations for the attempt counter

To make the ledger convention explicit, attempts counts compile-and-compare invocations rather than distinct source forms. The sensor search therefore has a measured lower bound of86:82 successful paired scratch compilations,2 final paired production compilations and2 root production integration checks. Three additional root all-source compilation audits and earlier paired sweeps are excluded because their per-function comparison count is not fully recoverable. The previous47 source-evaluation floor used a different unit and was corrected in the ledger. The two short lookup rows count their2 final paired scratch compilations plus1 root integration compilation; earlier exploratory revisions are not silently claimed as counted. This counting correction does not change any matched function rank.

## 2026-10-01: Restore the observed sensor data ABI

A direct read of the retail17-entry table showed KickKoura as its third entry, and Dossun=8 / KillerMagnum=7. The inherited source had KickKoura near the end and swapped those enum values. The table order and enum declarations now follow the binary. This restores the existing ABI; it does not introduce a new persisted format. The lookup's instruction bytes alone could match while its separate source data was wrong, so code-interval coverage does not establish data coverage or runtime readiness. The data row remainsU until its own reconstruction is verified.

## 2026-10-01: Permanent conservative compiler exit check

`python tools/checkGameCompiler.py` compiles the manifest's source under791 and894 with the unchanged project flags, records commands and source/header hashes, and invokes strict original-address byte checking. It accepts an alternative failure as compiler evidence only for a successfully compiled body with a different complete section size or different linked bytes. Missing addresses or linker/tool failures are not proof. Changed source/header files invalidate the check. With the one established sensor function, it correctly reports1 /3 discriminators and exits nonzero: M0 is not proved yet.

The current brief removes the pilot review stop and the extra ledger fields. STATE now points through the pilot directly into scaling. Historical decision entries remain append-only.

## 2026-10-01: Fix the pilot sample before its first attempt

The refreshed manifest contains 20 small functions, 20 medium functions and 10 large or branch-heavy functions. Previously matched hash and frame-accessor entries were replaced with the unmatched collision predicate and KeyPoseKeeper constructor. The sample draws only from already available clean game source, so its results cannot be treated as an unbiased forecast of all unnamed functions. No selected rank is O at selection time.

Two lanes continue the required M0 compiler investigation while the root performs independent pilot matching under the configured791 build. This is dependency scheduling: each accepted code interval still requires the strict target comparison, and M0 remains explicitly incomplete until three discriminators pass. Three late ledger rows still contained removed fields after the brief update; they now retain only timestamp, address, symbol, outcome, attempts and minutes.

## 2026-10-01: Check compiled objects without the compact scaffold

Enrolling the whole pilot in the compact image exposed67 unresolved dependencies from unrelated functions retained in the same archive objects. No candidate was accepted from that failed build. check.py now accepts --object and sends that compiler-generated object directly through the existing strict linker/comparison. Only check.py still writes O. Unknown original addresses and byte/size mismatches still reject a match. The direct mode returns a failing process status for rejected candidates.

The unchanged SDK object remains O through direct invocation; the nonmatching PlayerTrigger constructor remains m. Pilot ranks were restored to their pre-enrollment values after the failed compact link. Each subsequent candidate enters through its own strict object check. The compact image is a diagnostic scaffold and carries no runtime-completion claim.

## 2026-10-01: Recover nerve transition ordering and floating comparison semantics

The inherited NerveKeeper::update executed mNerve after tryChangeNerve had cleared it. Retail code executes the current nerve at offset4, taking a pending nerve from offset8 when present. Calling getCurrentNerve and ordering the transition stores as pending=null, current=next, step=0 reproduces all160 bytes. The matched update no longer carries a NonMatching guard.

The near-zero helper conditionally negates with ARM LE, including unordered comparisons. The source form value>0 ? value : -value reproduces that condition; the inherited value<0 and a tested value<=0 produce LO and LS instead. The third pilot iteration matches all40 bytes. This is binary-derived floating-point control flow, with no inserted instruction or flag change. The two matched PlayerProperty setters also lose their stale NonMatching guards.

The pilot tracks timestamped iterations with source/object hashes and compiler/comparison evidence under ignored build/pilot. Ledger minutes measure elapsed time from the first recorded compile/check to acceptance, including interleaved work on revisited functions. Initial passes measure their direct processing time; preparation and source-reading overhead is reported separately in the pilot report rather than invented per-function estimates.

## 2026-10-01: Establish sensor-group and allocation import identities

HitSensor::validate calls0x001CB5D4 at offset0x3C. The24-byte callee appends its sensor argument to the pointer array at offset8 and increments count at offset4. HitSensor::invalidate calls0x001CB5EC at offset0x38. That92-byte callee searches the same array, replaces a found entry with the final entry and decrements its count. These verified operations establish the reconstructed SensorHitGroup::add and remove ABI names. The other system-validation methods cross-check the same calls.

NerveKeeper allocation calls0x002932B0, and the NerveStateCtrl array allocation plus GhostPlayerRecorder array allocation call0x00292A78. Both60-byte callees obtain a current heap through0x00293088 and invoke its allocation slot0x14 with the requested size and alignment4, returning null if there is no heap. They ignore the nothrow-tag argument, consistent with ARMCC force_new_nothrow and no_exceptions. GhostPlayerRecorder deletion calls0x00255680, whose68-byte callee checks null, finds the containing heap through0x0028E8F8 and invokes its free slot0x18. Standard C++ ABI names for scalar nothrow new, array nothrow new and array delete are assigned from these separate observed call roles. These are reconstructed import identities, not claims of recovered original debug names.

Only five previously empty symbol-name columns changed. Starts, ends, literal-pool boundaries, types and target bytes are unchanged. The allocation bodies remainU until reconstructed code passes its own check. Candidate byte comparisons occur only after these independent callee/call-role inspections.

## 2026-10-01: Identify the recorder frame constructor

GhostPlayerRecorder::create passes36-byte elements and its literal0x0018D400 to the already mapped __aeabi_vec_ctor_nocookie_nodtor helper at0x0028EABC. The pointed-to interval is exactly one bx lr. The clean Frame type is36 bytes and leaves its fields uninitialized, so its generated default constructor also returns immediately. The empty symbol column at0x0018D400 is assigned the corresponding reconstructed C++ ABI name. No boundaries, pool fields or target bytes change.

## 2026-10-01: Match the animation predicate and pointer-list append

PlayerActionConditionAnimEnd::check now expresses the named-animation case as positive branches, followed by the same explicit frame-limit predicate used by its unnamed-animation case. This removes ARMCC boolean materialization caused by the inherited negated compound expression. Its fourth pilot iteration matches the full236-byte interval, including the SafeString vtable literal. The branches and call offsets come from the retail body.

The pointer-list append takes a reference to its condition list before allocating a node. OffsetListNode now accepts its stored value by value, which preserves the incoming condition pointer in r5 across allocation instead of spilling a referenced parameter to the stack. The fourth pilot iteration reproduces all76 bytes, including null handling, node-field initialization and list-size increment. This is a binary-derived correction to the clean internal template API, with no compiler flag change.

## 2026-10-01: Require committed-source project-build provenance

The updated brief adds hard rule5. Earlier direct object checks used scratch ARMCC compilations of repository source. Their code and hashes were real, but the new rule requires the project build output specifically. All52 prior O rows will be rechecked from freshly generated project objects before the coverage is reported as accepted under this rule. No target or function boundary changes are involved.

The old build emitted C++ to temporary assembly, renamed assembly sections and invoked armasm. It now invokes armcc directly with -c and the existing module flags, records the actual command, compiler/source/object hashes and repository dependency hashes, and builds the normal archive from that object. The checker accepts only the canonical build path with this record. It verifies that source, configuration and referenced repository headers equal committed Git blobs, rejects edited output, and rejects copied/scratch objects. SDK assembly remains a build input where permitted, but it cannot pass this C++ object mode.

The removed C++ assembly postprocessing helper has zero code references in tools. The strict byte comparison and original executable remain unchanged. Provenance rejection returns failure before changing a rank. The compact main link remains under separate repair; no failed link is reported as a runnable game.

## 2026-10-01: Revalidate accepted matches through committed-source build output

All52 previous O functions pass check.py --object again using their canonical objects under build/eu/obj, produced directly by the project build at ccc99c2. Coverage remains2892 of2,756,024 mapped function bytes, or0.104933%. The SDK48-byte interval uses its configured4.0/902 compiler; all51 Game/al intervals use4.1/791. The verification output is retained locally in build/provenance_validation.json.

A copied untouched compiler object outside the canonical build path rejects. The actual ByamlHashIter project object rejects because its source differs from the committed Git blob. An incorrect object hash in the provenance record rejects through the verifier; restoring only that record restores acceptance. None of these rejection checks changes a rank or edits an object. Evidence is in build/provenance_rejection_checks/results.json.

checkGameCompiler.py now takes its primary candidate from the canonical verified project object and requires check.py to accept it. The alternative compiler remains a diagnostic source compilation with unchanged flags. The sensor lookup still discriminates791 from894, so M0 remains1 of3.

## 2026-10-01: Recover element-pointer iteration and park unsupported pilot fragments

PlayerActionMultiCondition::setup stores the current element pointer, keeps the iterator offset across the virtual setup call, and reads the list offset again for each sentinel comparison. The clean iterator now models that element pointer directly and reconstructs its next ListNode by adding its stored offset. Ordinary C++ labels express one shared condition block, preserving the retail control-flow graph. The fifth diagnostic iteration matches all72 bytes. Acceptance awaits committed-source project compilation and check.py; the diagnostic object itself never enters the accepted checker. The inherited stale incorrectness comment is removed.

Nine large pilot fragments have not established all callee identities, data addresses or class layouts. Twenty named reachable imports still lack evidence in the compact link after unrelated archive sections are removed. Their outcomes are abandoned, their ranks return to U and source remains available. This distinguishes unverified fragments from functionally correct NonMatching C++, rather than inventing link stubs or addresses. The pilot cap is a ceiling; repeated compilation cannot resolve missing identity evidence. Ledger elapsed minutes for closure include interleaved project work and are not summed as independent labor.

ByamlHashIter::findPair reaches the eight-iteration limit. A missing compiler-header environment caused the seventh invocation to fail; that invocation still counts. The final standard binary-search source preserves the observed null, bound and signed difference behavior and remains NonMatching. Its remaining register allocation and instruction order differences are logged for later work.

## 2026-10-01: Establish scalar delete independently of the compact link

Rebuilding the animation-predicate source retains its compiler-generated SafeString vtable and deleting-destructor helper, which needs scalar delete. The already named retail MainFileDevice deleting destructor at0x002DD6E0 restores its vtable, destroys its owned child, invokes FileDevice D1 at0x002452B0 and then tail-calls0x002743AC. That existing68-byte callee handles null, looks up the containing heap through0x0028E8F8 using the singleton pointer at0x003E23B8, and invokes the heap free slot0x18. Its behavior independently agrees with the previously established array-delete body at0x00255680. These separate scalar and array caller roles establish reconstructed ABI identities.

Only the empty symbol column at0x002743AC becomes _ZdlPv. Its rank stays U. All map starts, ends, pool boundaries and target bytes stay unchanged. This callee is not newly counted as reconstructed source, and no unknown address is invented to make the compact link pass.

## 2026-10-01: Restore the main compact link and finish the pilot

The compact linker now retains enrolled functions and their compiler-generated code/data/helper closure in separate ignored archives. This removes unresolved imports from unrelated archive methods without touching canonical ARMCC objects or adding invented semantic stubs. Existing numeric identifiers receive scaffold aliases only when their encoded address equals an existing unnamed function start. These aliases count as zero reconstruction. Copied sections are checked byte for byte after serialization. Projected output has no eligible build provenance and cannot receive object matches. make.py also loses its obsolete disabled-Game comment and fixes the existing True7 typo; split mode itself was not exercised.

The normal make.py eu build links and exports after parking unsupported fragments and establishing the independently observed scalar-delete name. progress.py runs unchanged. The normal checker and direct canonical checker both preserve the72-byte list setup match. All53 O functions pass canonical committed-source checking after the shared iterator/header change. Exact full-interval coverage is2964 /2,756,024 bytes, separate from the legacy progress word-similarity value2936 /3,092,336.

M1 passes with40 matches,1 functionally correct NonMatching search and9 abandoned fragments. The committed report states per-function timings, class rates, sampling bias and conditional throughput projections. The eight-iteration ceiling remains for the first Phase2 pass, with early parking for identity blockers. Phase2 shared matching and a separate dump-loading runtime lane continue immediately; M0 remains incomplete and M2 is still the final objective.

## 2026-10-01: Reconstruct the sensor-group storage and constructor

Six HitSensorDirector allocation sites request12 bytes and call0x002496F8 with capacities16,128,512,2048,1024,1024. The132-byte constructor stores capacity at+0, count at+4 and pointer buffer at+8, then zeroes each allocated pointer. There is no vptr. The24-byte add writes at count and increments it without a capacity guard. The92-byte remove searches the saved count, replaces the found entry with the last entry and decrements count. The own group header records these private fields directly; shared sead headers are unchanged.

The existing unnamed constructor interval receives the reconstructed ABI name _ZN2al14SensorHitGroupC1EiPKc. Its size, bounds, pool and target bytes are unchanged. The three functions are enrolled as M until committed-source project checks run. Their first diagnostic source compilations reproduce all three complete intervals under791, but scratch output itself establishes no accepted rank.

## 2026-10-01: actor helper ABI and independent effect import

Retail getters establish LayoutActor audio at0x24 and its opaque layout object at0x18. The base remains0x30 bytes, with its existing virtual interfaces at0/4/8. The source now represents those observed fields. calcAnim is independently identified from the primary vtable slot at0x10 and the unchanged interval0x001BE028..0x001BE04C; its child calculation field at0x14 remains an opaque view. Constructors stay behind NON_MATCHING because their vtable data identities remain unresolved.

The kill caller and the retail clear-all loop establish EffectKeeper::deleteAndClearEffectAll at the existing56-byte row0x001BFC10..0x001BFC48. That loop reads count+4 and buffer+8, deactivates live handles and clears references. This names an import without moving boundaries or claiming its implementation. Twelve helper rows are enrolled asM for canonical checking. Full diagnostic and layout evidence is in build/phase_two_actor_helpers/identity_evidence.md and ready_batch_checks.json, which remain ignored.

## 2026-10-01: typed Byaml record and iterator wrappers

ByamlData keeps its eight-byte size but its tag is one byte at+4, with three padding bytes. Independent retail writers at0x00109C9C,0x00109CE4 and0x00109D20 store the value word at+0 and tag byte at+4. The readers at0x00278C4C,0x0027E084 and0x00290F7C confirm byte loads. Its separate serialized ByamlHashPair representation is unchanged. Four existing source methods and two newly implemented iterator wrappers are enrolled asM; all prior Byaml matches will be checked again.

CameraParamVision::init passes the Vision child output to0x00290FB0. CourseList::World passes its indexed child output to0x0029107C. Both callees write the base/root pair and return whether its data base is valid. Indexed data lookup0x0028CA48 independently dispatches established array/hash data writers. These observations establish the three recovered names at existing rows; no boundary is changed. The wrappers use ordinary ByamlIter values and compiler-generated pair stores. String wrappers and separately compiled constructors remain diagnostic until the verifier supports proven source closure. Evidence and paired commands remain under ignored build/phase_two_byaml.

## 2026-10-01: course selection source integration

The owner constructor allocates0x68 bytes and the resource constructor initializes the verified LayoutActor base, marker pointers and eight visibility flags. CourseSelectMapLayout names a recovered representation, not an asserted original class identity. The update stays fn_00159fec because its source class/method name is unknown. Its unchanged724-byte interval includes the complete literal pool. All unnamed calls and both external name tables use only existing map row-start addresses. The independently established StringTmp<32> variadic constructor names0x0027BEC8 without claiming its body. Layout and identity evidence is committed in project/course_selection_evidence.md. Paired raw ARMCC diagnostics match791 and894, so this is not a compiler discriminator. Canonical acceptance remains pending.

## 2026-10-01: compiler-local switch labels and mapped scaffold data

ARMCC emits __switch$$ as a local, zero-size STT_FUNC at offset180 inside the course update section. Eight compiler ABS32 case-table relocations refer to it. The checker now preserves that label in its own section only when its name, local binding, zero size, aligned nonzero in-section offset all agree. All other nonzero function definitions in the selected section still reject. This fixes compiler ELF interpretation, with no target, interval, flags, relocation addends or byte comparison change. An untouched ordinary C++ fixture with two real functions in one section still rejects. All56 previously accepted canonical functions and the724-byte course candidate compare exactly after the change; every input object hash remains unchanged. Ignored build/switch_marker_validation contains compiler commands and results.

The compact scaffold now resolves dat_address names only at existing mapped data starts. It generates explicitly labelled zero-filled weak data placeholders with the unchanged mapped size and section type. These placeholders are not reconstructed assets, never establish addresses and count zero toward strict function coverage. The two course tables retain32/64-byte intervals; an unknown dat_deadbeef request generates no declaration. The main build links and exports, and unchanged progress.py runs. Its similarity percentage can rise from placeholders and must never be reported as complete byte-exact coverage. The old find_scaffold_function_aliases name has zero hits in tools.

## 2026-10-01: EffectKeeper count, array and active flag

The clear-all loop0x001BFC10 and update0x002553B4 independently agree on effect count+4 and array+8. The update reads/writes its active flag at+0x10; LayoutActor movement and the LiveActor effect-update path establish its API identity. The header records these observed offsets and keeps intervening fields opaque, without asserting a complete object size. Both source loops compile identically under791 and894. Root will commit before rebuilding and award ranks only from the canonical objects. A concurrent header edit correctly invalidated ten in-flight provenance checks without writing ranks. Freeze shared header input before the build/check checkpoint; this is root coordination, not a relaxed acceptance rule.

## 2026-10-01: stage progress source closure

The existing252-byte row0x0016BCE0 updates save progress from the current stage. Its normal coin bound is3; MysteryBox classification selects1 or2 based on established course progress. Independently observed fields include stage lives+0, character+4, clear flag+8, pending state+0x20, course index+0x24 and progress pointer+0x30; course progress reads signed bytes+0x60/+0x64. The partial representations are scoped in StageProgressReconstruction to avoid asserting or conflicting with complete retail classes. Source filenames describe stage progress. Two eight-byte accessors remain separately compiled ordinary C++ so the existing linker can inline them late. They have no map addresses and must never receive guessed imports. Canonical source closure acceptance is pending; the initial240-byte early-inline body is not accepted. Paired diagnostic evidence under build/compiler_discriminator_probe/layout_runtime_loop records seven structural iterations,16.964 shared working minutes and an exact252-byte single-region link under both compilers.

The stage sources compile canonically, but the current compact projection discards their separately defined strong helpers and fails the main link. The stage root remainsU until source closure support lands, so the normal main build keeps linking. Its C++ remains committed and compiled for the tooling positive fixture. No source-generated helper is assigned a fabricated original address.

## 2026-10-01: effect reference helper identity

The unchanged44-byte interval0x001C5A88..0x001C5AB4 clears an effect-set handle reference by comparing its saved identifier with the pointed effect object's current identifier. Its pointer and identifier fields are independently observed at entry+4, reference+4 and effect object+0x0C. The function remains address-named fn_001C5A88 and rankU until committed source is built and checked. Initial ordinary C++ diagnostics produce44 bytes with the redundant conditional branch under791, and40 without it under894. This is a possible compiler discriminator, not an accepted match or M0 claim.

## 2026-10-01: compact build freshness

The scaffold previously returned unchanged after rewriting enrolled imports. A map-only root change could therefore leave an old main image. Stub generation and dependency generation now report content changes, and import assembly only rebuilds when needed. make.py links whenever these inputs change. It also exports a fresh binary after recovering a missing generated ELF, even when no source object changed. Verification moved only the generated main ELF aside, observed a successful normal relink/export with a newer binary, then observed that a second unchanged normal build did neither. Targets, canonical C++ objects, global flags, differ and progress.py remain unchanged. The refreshed legacy similarity currently reads1112/3,092,336 bytes, lower than the earlier3180; compact address/layout changes affect that metric. Strict coverage remains4868/2,756,024 bytes in77 complete functions.

## 2026-10-01: effect action and deletion methods

The existing88-byte row0x001BFBB8 and56-byte row0x001BFC60 operate on the established EffectKeeper count+4 and array+8. The independently identified constructor initializes the action string at+0x0C and signed mode byte+0x11. The string method compares and stores changed names, then sends them and the mode to each set through0x001E9B30. The deletion method instead calls0x001EA1DC, whose handle deletion passes true and clears active+0x18; clear-all passes false and also clears references. The descriptive API names setActionName and deleteEffectAll are reconstructed identities, not debug symbols. Direct callers/function-pointer literals were not found for these two methods. The field/constructor/per-set evidence supplies their class association. One structural variant per method compiles identically under both versions.

The standalone effect-reference helper contains no imports or literals. Its ordinary conditional nulling reproduces the full44-byte target under791;894 produces40 bytes. The source is committed separately from the larger effect-set loop so early inlining cannot conflate identities. Both existing-header methods and this new translation unit are enrolledM for canonical verification. Shared preparation windows and full paired commands are under build/phase_two_effect_helpers and build/phase_two_effect_set_helpers. No scratch result awards a rank.

## 2026-10-01: second canonical compiler discriminator and Phase2 checkpoint

checkGameCompiler.py accepts the committed normal-build primary sensor lookup and effect reference helper, then compiles unchanged source under894. Both primary checks pass; both alternative objects genuinely compile and fail the complete interval check. fn_001C5A88 produces44 bytes under791 and40 under894 because894 removes its redundant conditional branch. The tool reports2/3, headers unchanged and M0 false. No global flag changed. Current complete accepted coverage is80 functions/5056 bytes, including21 new helpers and three later effect methods. Each accepted function has its own rank/ledger commit.

Phase2 timing rows state their basis in ignored acceptance manifests. Actor-helper minutes cover root canonical checks only; earlier shared source preparation was unmeasured. Byaml and effect rows include measured shared diagnostic windows plus canonical verification. These windows overlap across functions. Course-map timing includes18 paired compiler invocations and its canonical check. Failed stale-header checks are recorded in phase_two_canonical_acceptance.json; retries are in phase_two_canonical_retry.json. No timing is an independent labor estimate.

## 2026-10-01: player transition flag and stage enrollment

The sole direct caller0x002D412C accesses the independently known PlayerActor Player+0x74 and PlayerModelHolder+0xA4 fields and invokes0x001BB19C on the subsystem at+0x114. Its adjacent update0x001BB1C4 checks the Move action, emits SePmDashLoopStart and decrements+0x1C each frame. The standalone Game source therefore describes previous/current signed flags+0x19/+0x18 and a transition countdown+0x1C, without asserting a full class name/layout. The40-byte candidate retains a redundant branch under791;894 produces36 bytes. EnrolledM does not claim acceptance.

The source-closure tests are complete and the stage root is now enrolledM for the main build. Existing rows for two effect-set deletion loops and the24-byte related flag helper receive only fn_address identities, rankU. Independent observed fields stay partial. Tooling and source acceptance follow separately; no function boundaries change.

## 2026-10-01: source-generated C++ inline closure

The stage-progress root at 0x0016BCE0 needs two eight-byte accessors whose original standalone addresses are unknown. Committed C++ definitions in StageProgressAccessors.cpp provide those helpers through canonical direct ARMCC project objects. The unchanged linker `--inline` removes both helper sections and produces the full original 252-byte root interval.

The checker first keeps the established one-section path. An unresolved branch helper activates discovery among project C++ object records. Discovery requires one canonical definition. Explicit `--inline-object` inputs follow the same closure checks and reject unrelated objects. Every reached function must start its own section; an ordinary shared-function section rejects. The existing narrow ARMCC `__switch$$` label exception remains unchanged.

The closure projection copies only source-generated root/helper bytes and relocations. Established map addresses resolve runtime imports. No retail code enters a linker input, and unknown helper addresses are never invented. The initial compiler-section size is recorded but does not constrain the final size, because genuine inlining can expand or contract a function. This is compiler/linker phase separation. Before linking, every input must pass committed-source build provenance and use the requested configured compiler. After linking, object provenance, provenance-record hashes and the map hash are checked again. The final linker Global Symbols table must identify the selected ARM function from its projected root object at the exact original start and full size. The final ELF must contain exactly one allocated region at the unchanged target start with the full target size. Its linker memory map must contain only the selected root section, proving that every helper section disappeared. The target executable is read only after linking for its established hash and full interval comparison, including the literal pool.

The compact main projection now follows reachable unmapped strong C++ helpers after verifying their callers and definitions. It records the canonical inputs and requires those helper sections to disappear from the final linker memory map. Existing mapped functions remain original-address imports. Projected objects stay ignored and are ineligible for canonical object acceptance.

Staged validation passed all 80 accepted canonical functions, totaling 5056 complete interval bytes, in 12.096 seconds. Production validation repeated the same 80 intervals in 11.976 seconds. Automatic and explicit stage checks both produced SHA256 4c5cc59e0419dca56bc35cdcde3e6571459894fc064320c27bd2fbb1077bbcd5 for all 252 bytes. These diagnostic checks do not set a rank.

Real unrelated, missing, copied and ambiguous helper cases reject. The ordinary shared-section C++ fixture rejects because it contains a second function. A diagnostic-only keep option retained one unchanged canonical helper, enlarged the allocated output to 260 bytes, and triggered residual-helper rejection. The actual check.py CLI rejection returned exit 1 and left the complete map hash unchanged. The committed PlayerTrigger constructor provides a real 16-byte full-extent mismatch control. It returns rejected=False, and an in-memory previous O state requests a downgrade to m. Structural rejection requests no rank write. This contract check intercepted writes, so no map rank changed.

In the parent-approved negative window, a changed helper object rejected before the byte oracle. An edited source rejected against its existing provenance, and a normal project rebuild of that uncommitted source rejected against committed Git inputs. Source, object, dependency and provenance bytes and timestamps were restored exactly and successfully reverified before concurrent work resumed. No altered object reached the byte oracle.

Evidence is under build/inline_closure/production_validation/evidence.json, cli_rejection.json, staged_validation/evidence.json, negative_provenance/evidence.json and negative_retained_helper/evidence.json. The root enrolled the stage function asM. The normal main build links/exports with both verified helper definitions, and its post-link check confirms that their allocated sections disappear. Authoritative function acceptance follows this tooling commit. Both earlier compiler versions produced the same stage output, so this function does not add an M0 discriminator.


### M0 compiler milestone, 2026-10-01

The committed-source paired proof now passes three of three Game/al discriminators. The configured ARMCC 4.1/791 matches sensor-name lookup, fn_001C5A88 and Game fn_001BB19C; the same source and headers under 894 differ in complete bytes or size. tools/checkGameCompiler.py verifies each primary canonical project output with check.py before comparing the alternative. build/game_compiler_check/evidence.json records passes=true, three discriminators and unchanged headers. The player helper is 40 bytes under 791 and 36 under 894. Its independent PlayerActor caller and Move/SePmDashLoopStart neighbor establish Game ownership. M0 passes; M2 matching and runtime continue.

### Shared helper source enrollment, 2026-10-01

Twenty-nine reviewed candidates, 1,208 complete bytes, enter the ordinary build as M. The batch adds partial EffectSet/flag views, a behavior-inferred controller registration method and four LiveActor execution methods alongside existing named helpers. project/shared_helper_evidence.md distinguishes established names, inferred names, observed offsets and unresolved callee identities. No function boundaries or compiler flags changed. Only subsequent canonical checker acceptance may promote a row to O. The helper declarations at existing unnamed starts establish import identities without claiming their implementations.

### Genuine vtable data ownership repair, 2026-10-01

Independent retail ownership establishes NerveExecutor's complete table at0x003D62FC..0x003D6310 and LayoutActor's at0x003D5FFC..0x003D604C. Their prior unnamed data rows cross ABI headers, address points and adjacent objects. project/vtable_ownership_evidence.md records table words, constructors/destructors, independent allocation callers and the precise minimal partition. Root reran the scratch ownership audit before applying it. The repair preserves both old union intervals, every mapped byte and every one of18,055 function rows. All changed data ranks stayU. The map has28,045 rows instead of28,043 because each old three-row partition becomes four. The executable hash is unchanged. This separate metadata repair follows BRIEF4.2; no candidate source or accepted rank changes in this commit. Retail null virtual slots remain observed zeros with unresolved elimination history; source-emitted tables are not claimed byte-exact.

### Native retail matrix rounding, 2026-10-01

Ordinary native C++ sine/cosine and matrix construction now reproduce3,422 direct float32 values from freshly rerun read-only retail ARM1176 execution:2,018 trigonometric outputs and1,404 matrix components across117 poses. Sanitized debug and optimized Apple Clang21 builds both pass, including192 owner placement components. A native-file-only fp contract(off) pragma preserves retail VFPv2 separate multiply/add rounding; matching ARMCC flags stay unchanged. Large finite arguments requiring the unreconstructed reduction helper explicitly reject. Seven malformed scene cases and the unsupported-angle case reject; the49-file baseline and50 extracted hashes pass. This is bounded math/reader evidence, not gameplay or differential replay. project/native_scene_evidence.md gives ranges, commands and remaining gaps.

Correction: the prior claim of twelve signed-zero differences came from serializing negative zero as JSON -0 and then parsing it as integer zero. Direct legacy source4197223 comparison finds zero signed-zero differences and four numerical component differences, maximum5.960464477539063e-8. Reports now expose explicit matrix bit fields. The new native math removes the four measured numerical differences in the tested domain.

### LiveActor vtable endpoint repair, 2026-10-01

The LiveActor ABI header is already correctly named at0x003D7974. Independent primary/secondary stores, getter adjustments, adjacent constructor stores and a196-byte successor allocation establish the complete152-byte table ending0x003D7A0C. The old row includes the next table's eight-byte header. Root reran the retail ownership audit and separately repaired the two-row328-byte union as three rows: complete LiveActor table, independently owned168-byte successor and following eight-byte header. All data ranks remainU; every function row and mapped byte is unchanged. The map now has28,046 rows. project/live_actor_vtable_evidence.md records observations, reproducible commands and unresolved null-slot history. The constructor's existing table base was already sufficient for its relocation, so this repair does not rescue a candidate address or change a function interval.

### Actor execution source and shared partial layouts, 2026-10-01

Reviewed source proposals restore endClipped, makeActorAppeared and movement through ordinary separately compiled flag readers and empty action/shadow hooks. Retail signed-byte reads and effective no-operation gates independently support those source views. Helpers have no guessed original addresses; canonical acceptance requires all separate code to disappear and the sole caller extent to match. Detailed paired diagnostics and hypotheses are retained under build/phase_two_actor_execution.

A review found the earlier LiveActor CPP-local ActorExecuteInfo definition conflicts with the existing shared partial header used by executor registration. This is an ODR defect. The definitions are consolidated: preserve the independently established request keeper at+0, add opaque bytes+4..+0x17 and the separately observed pointer+0x18, then use an inline getter in LiveActor. No full object-size or pointer-identity claim is made. All earlier accepted functions require a fresh canonical recheck after these header changes. The NerveExecutor destructor now performs ordinary scalar deletion of its keeper, as independently observed; constructor NonMatching guards are removed only from reviewed complete candidates.

This checkpoint commits source proposals and M enrollment, not new matches. The next step is the normal build and complete accepted-function recheck. At the reviewer's request, local acceptance batching pauses after that recheck while dot proposals are inspected and accepted locally. Dot branches are active, so blocked entries and U functions of0x400 bytes or more stay reserved for the dot.

### Source integration correction, 2026-10-01

The execution helper proposal file contained only appended flag readers, not a full replacement translation unit. Root's source-copy integration removed the existing LiveActorFlag constructor. The accepted-function inventory exposed its missing canonical definition before any new acceptance. Restore the original committed constructor verbatim and append only the three new readers. The recheck runner also incorrectly treated the build's source-symbol inventory as a complete function inventory; it omits some globally defined canonical functions. Resolve accepted roots from actual canonical ELF function definitions with project provenance, never generated stubs, and recheck all111 accepted intervals. No new match was counted from either error.

### Resource-local native model attributes, 2026-10-01

The sanitized CGFX reader independently checks39 selected models,137 shapes,50,076 vertices,621,631 raw/scaled components,133,872 indices and164 bone references. Thirteen malformed cases reject; controlled signed-short fields pass. Scene, baseline assets and all3,422 debug/optimized retail-math comparisons revalidate from the same source; original/copy/50 extracted hashes are unchanged. project/native_mesh_evidence.md records the owner's OpenGL scalar enum variant and exact validation commands. Twenty-eight constant-form groups remain opaque; triangle topology, bone/shape transforms, materials and rendering remain unresolved. This is resource-local decoding, not world model geometry or gameplay.

## 2026-10-01: Import identified unaccepted virtual tables in the compact scaffold

The normal main link failed because newly enrolled constructors retained complete source virtual tables, which referred to seven unmapped virtual methods. The compact scaffold now imports only a whole native virtual-table object whose offset-zero symbol, complete section extent and name agree with one existing U data row. Its generated placeholder is weak and zero-filled. This is a scaffold import, not accepted data or game behavior. Shared sections and data without an independently established identity remain retained; forcibly discarding unidentified data refuses projection. Known SECTION relocations retain their original bytes and addends while referring to the exact native object name.

The normal build links and exports. LayoutActor, LiveActor and NerveExecutor placeholders have the established 80, 152 and 20-byte extents. Narrow fixtures refuse unknown identity, mismatched extent, shared sections and forced unknown-data serialization. Target, map, checker and provenance hashes are unchanged. Evidence is build/scaffold_data_projection/evidence.json and main_build.log. Canonical compiler objects remain untouched except the legitimately rebuilt restored LiveActorFlag source and generated scaffold stubs. Matching checks continue to use direct canonical ARMCC output, never projected objects.

## 2026-10-01: Review source and identities from dot/fugumannen-init

Intake takes three layout headers and the report; Fugumannen.cpp is unchanged. Independent Togezo allocation/calls and the EnemyStateBlowDown constructor establish the0x28-byte state, host/parameter/vector/animation/message offsets and appear override. Independent default initialization and step consumers establish five floats then two integers in its0x1C-byte parameter. Fugumannen remains a0x60-byte base plus speed and state pointers,0x68 total. Four previously unnamed imports receive independently supported names at existing unchanged intervals: EnemyStateBlowDown constructor, float tryGetArg0, startAction and sendMsg41. initNerveState was already named locally. The report records addresses and retail evidence. Five existing source functions are enrolled as M for canonical local verification; no dot rank or ledger is imported. Ledger timing records local checking only and excludes dot preparation.

## 2026-10-01: Review walker layout and source proposals from dot/togezo-init

Independent translation-unit initialization and parameter constructors establish three consecutive BSS objects at0x0042FB10,0x0042FB30 and0x0042FB9C with0x20,0x6C and0x70-byte extents. A new U data row describes their0xFC-byte .bss.Togezo.cpp allocation; no existing interval changes. Integer conversions and inline FixedSafeString storage in the constructor and independent WalkerStateChase initializer justify the completed layouts. Togezo remains0x6C. Independent pose-keeper vtable slots establish getFrontPtr/getFront. Retail action-channel completion and a separate BreakModel consumer establish isActionEnd. Three existing ordinary string rows receive source-specific names, preserving their bytes and intervals. Shared ctor/action names were already independently admitted for Fugumannen.

Source restores ordinary adjacent static parameter objects and symbolic Turn/Search/AttackSuccess strings. The retail Attack nerve branch skips only action startup, so movement runs on every step; source corrects that existing behavioral error. Eight functions are enrolled as M for this repository's committed-source canonical checks. The normal rebuilt object must also verify NOBITS size/offsets. Dot results and source remain proposals until accepted here; local ledger timing excludes external preparation.

## 2026-10-01: Review inclusive cube source from dot/area-cube

The independently disassembled cube routine loads an unsigned selector at+0x14 and uses inclusive finite bounds. Source now tests both X bounds, both Z bounds and the selected Y interval, fixing the retained source's Y-for-X error and strict face exclusion. Comparison order and unordered-float behavior follow retail. The existing calcLocalPos interval receives its independently corroborated semantic name. Three static-initializer stores and an independent rotation getter identify the12-byte Vector3<float>::zero object at0x004305F8 inside exheader BSS. Its new U data row changes no existing interval. The source candidate is compiled unguarded for canonical checking; its rank remains M until this repository verifies all180 bytes. The report preserves the dot's original guarded proposal and six diagnostic variants. Local ledger timing excludes external work.

## 2026-10-01: Keep guarded sensor execution from dot/hit-sensor-director

The locally accepted constructor remains byte-identical, including its CP932 registration name. Intake adds a same-sized Vector3f position member at sensor+8, readers of already established fields, and the independently identified execute override. The retail vtable slot identifies its existing496-byte function interval without any boundary change; numeric helper imports resolve through existing rows. Source preserves six clears, thirteen cross-group calls, cached character count, self-pair rejection and directional Eye contact order. Ordered radiusSquared<=distanceSquared rejection retains the retail unordered-float path. The guarded body is enrolled M for a canonical local diagnostic and bounded differential validation. The dot's eight variants and final nine non-call VFP differences remain excluded from exact coverage; no functional equivalence is claimed before local validation.

## 2026-10-01: Canonical sensor execute diagnostic and waiting packets

The guarded dotHitSensorDirector execute proposal builds from committed source and passes provenance/structure gates, but check.py reports M->m because its complete496-byte interval differs. Its bounded behavioral differential remains in flight; it contributes no exact bytes and is not yet recorded as a functionally verified NonMatching result. The constructor remains accepted.

The first two hard-function packets document capped EffectSet update and ExecuteRequestKeeper request, with complete textual disassembly, source/layouts, actual eight-variant paired results, compiler flags and explicit missing normal-diff limitations. Root confirmed their unchanged target interval hashes and sub600-line sizes. Neither has functional validation or a matching claim. The GPT-6 Pro relay is paused; packets remain committed review inputs for the owner or dot, with no automatic external message.

## 2026-10-01: Preserve raw skeleton and inline model fields

Native reader adds four inline float fields for28 constant-form vertex groups and45 raw float fields plus names/identifiers/parent representations for101 skeleton joints. Independent byte reads validate112 inline fields and4545 joint fields,39 roots/62 children and acyclic ancestry. Twenty-two malformed cases reject cleanly under sanitizers. Rebuilt debug and optimized native math still agree on3422 retail float bits; scene regressions and all50 selected asset/original/copy hashes pass. Root verified the frozen source, note, binary and validation-driver hashes in data/runtime/romfs/native_mesh_frozen_evidence.json. This remains resource-local inspection; no runtime bone/shape transform, GPU constant meaning, rendering or gameplay is inferred. The evidence note corrects the prior instance count:16 bindings use11 unique resources with12 unique joints, not21 unique joints.

## 2026-10-01: Repair independently verified Togezo table endpoint

The named Togezo data row included the following actor's eight-byte ABI header. The retail constructor independently loads address point0x003D3018 and stores its three secondary address points; its last two secondary method slots end at0x003D30A8. A separate neighboring constructor loads0x003D30B0, proving that the zero/zero words at0x003D30A8/AC belong to the next actor. Independent creation paths allocate0x6C for Togezo and0x9C for its unnamed neighbor. Source-generated table extent152 and ABI offsets corroborate the retail partition but do not establish matching data.

A separate data-only repair ends _ZTV6Togezo at0x003D30A8 and begins the following unknown interval there, preserving its end0x003D3148. The union, row count, all18055 complete function rows and all ranks are unchanged. The neighbor remains unidentified data. Root reread both constructor literals and all four ABI headers from the hash-verified target. No source/checker/scaffold guard is altered to fit an incorrect extent. project/togezo_vtable_evidence.md records the independent evidence; full diagnostic material remains under build/togezo_vtable_audit.

## 2026-10-01: Decode serialized primitive topology from its retail consumer

The native reader decodes only descriptor byte+4, retaining the complete raw primitive word. Retail command builder0x002B3940 independently maps0/1/2 to4/5/6 and emits matching index width/count, primitive configuration and draw commands for all137 owner descriptors. Read-only original ARM execution covers19950 instructions, including six mode/geometry-override cases and upper-byte checks. The owner streams contain133872 indices,44624 serialized triangle triples, including degenerate primitives. Six native controlled cases verify all three serialized modes and upper-byte preservation;26 damaged-input cases reject under sanitizers. Caller-selected geometry override, winding/culling, transforms, materials and rendering remain unresolved.

Root reviewed the byte mask/cardinality changes and verified frozen source, note, driver, report and binary hashes in data/runtime/romfs/native_topology_frozen/command_result_manifest.json. Scene regressions,50 asset hashes and both3422-bit math comparisons pass. This is resource-local topology, with no game execution, rendered triangle count or M3 claim.

## 2026-10-01: Supply a verified named BSS import in the compact scaffold

After the cube intake, the compact main lacked Vector3<float>::zero. Its independently evidenced U db interval is0x004305F8..0x00430604. The stub generator now considers actual undefined data references from provenance-verified canonical C++ objects and selects only a unique named U db row with a positive complete extent and no shared SectionName. Unknown, duplicate, shared and empty identities refuse selection; existing address aliases and virtual-table gates are unchanged. The one selected import is declared by seven current canonical objects.

The normal main links/exports with one weak, writable,12-byte all-zero placeholder. ARMCC emits its forced section as initialized SHT_PROGBITS inside ZI, so no NOBITS claim is made. This is scaffold data and contributes no accepted bytes. Target/map/checker/provenance/projection hashes and every canonical Game/lib object remain unchanged; only generated stubs change. Narrow fixture and frozen evidence are in build/named_bss_scaffold/evidence.json. The scanner runs before compilation; a wholly new import may need a subsequent build after canonical output exists. The tool does not relax that evidence gate.

## 2026-10-01: Audit the two BSS additions made during dot source intake

Commits2094174 and2895275 added two new U BSS rows together with source intake rather than dedicated data-evidence commits. The Togezo allocation at0x0042FB10..0x0042FC0C follows the independent translation-unit initializer and parameter constructors; the canonical NOBITS section is0xFC bytes with object offsets0/0x20/0x8C and sizes0x20/0x6C/0x70. Vector3<float>::zero at0x004305F8..0x00430604 follows three independent initializer stores and a separate rotation getter. Both lie inside the exheader BSS extent. Earlier entries and project/dot_reports/togezo-init.md and area-cube.md record the detailed evidence. No existing function or data interval moved, and both rows remain U; neither adds accepted data bytes. Their already reviewed legitimate identities stay in place. Future data-row additions, splits and boundary repairs receive separate evidence commits before source/rank intake.

## 2026-10-01: Validate bounded sensor execution from dot/hit-sensor-director

The committed guarded execute body remains rank m, with 23 differing bytes across nine instructions in its complete 496-byte interval. Root verified all 23 frozen input/evidence hashes against current source, headers, canonical object and checker-linked candidate. Whole-routine ARM11 MPCore/VFP emulation agrees on all 810 pairs: 162 bounded synthetic inputs across five FPSCR modes, using four unchanged retail helpers. Contact counts and order, complete arena memory, helper traces, final FPSCR, surrounding stack and callee-saved registers agree. At most four sensors per group are exercised. The shared retail helper implementations, software emulation, caller-saved registers and active stack frame limit the claim; gameplay, physical hardware, concurrency and larger groups remain unverified.

This is a bounded NonMatching result and adds zero exact bytes. project/hit_sensor_director_validation.md records the measured scope. Ledger attempts combine eight reported dot structural variants and one local canonical check. Its 0.100186 minutes cover the observed 0.352-second canonical diagnostic window and 5.659157-second final ARM11 validation window; preparation and external work are unmeasured and excluded. The 523-line packet is the fourth committed hard-function request. The relay remains paused.

## 2026-10-01: Preserve verified model-local mesh and material identities

The native reader now preserves 137 mesh bindings and 127 material identities across39 models. Independent raw reads verify names, flags, indices and model back-references; original consumer block0x003354C8..0x0033556C agrees on the resolved mesh, shape and material addresses for every owner binding, with5617 instructions and no code edits. Thirty-nine damaged-input checks reject cleanly under sanitizers. Scene and49-file regressions, all50 selected hashes, original/copy dump hashes and3422 direct float-bit comparisons in both debug/optimized builds pass. Root reviewed the bounded layout/ownership changes and verified24 frozen source, note, driver, report and executable hashes from data/runtime/romfs/native_mesh_identity_frozen/command_result_manifest.json. Material internals/state, replacements, visibility, animation, transform application, rendering and gameplay remain unresolved. No M3 claim follows.

## 2026-10-01: Review source and names from dot/placement-map

The single Scene source patch repairs the previously falling-through boolean helper and retains the guarded initializer with ordinary inline traversal. Independent archive callers and the callee's SafeString/format/archive dispatch identify Resource::getByml at0x00290640..0x00290698. Independent factory callers, ObjectName/ClassName lookup and the225-entry function-pointer table identify ActorFactory::getCreator at0x00268EB0..0x00268FB8. Names apply only to existing complete intervals. StageData and AllInfos already occupy separate unchanged12-byte rows; numeric imports need no data edits. Root reviewed current headers, patch and retail evidence in build/dot_proposals/placement-map/root_recommendation.md.

The144-byte tryGetPlacementInfo exact claim is enrolled M for local canonical checking. The344-byte initializer remains guarded and enrolled M for a diagnostic, with no functional claim before bounded local validation. No header, data boundary or row changes are taken. The eight-variant initializer packet and remote report are preserved as proposals; they add no accepted bytes on intake.

## 2026-10-01: Canonical placement-map checkpoint from dot/placement-map

The normal main links/exports after source intake. tryGetPlacementInfo passes the complete144-byte canonical check and has its own credited acceptance commit. The guarded344-byte initializer passes source provenance and section-shape gates, but reports M->m because linked bytes differ. build/dot_proposals/placement-map/local_guarded_diagnostic.json freezes its source checkpoint, direct object and checker-linked AXF hashes for bounded validation. No functional claim or NonMatching ledger outcome is recorded for this initializer yet. Exact-helper ledger time covers only its0.313-second local check, excluding external preparation.

## 2026-10-01: Record independent CourseList shared-data ownership

Before source intake from dot/course-list, root independently reconstructed212 ordinary initializer bytes and compared them with the unchanged target. Fifteen source sections cover16 existing rows; only the28-byte Type/Normal/Miniature section at0x003A2890 spans two rows. Retail independently derives Normal as Miniature-8. project/course_list_data_evidence.md documents the complete extent and source-data hash. No map, boundary, rank or source changes are made here. Canonical emitted-data comparison and function acceptance remain pending.

## 2026-10-01: Review CourseList source from dot/course-list

Intake takes only CourseList.cpp, the separate CourseListCourse.cpp leaf, two reports and the capped World packet. Complete Byaml headers/providers remain unchanged. Independent factory construction and resource-search/create continuation identify findOrCreateResource at the existing0x00243260..0x0024327C interval. The independently established string-tag reader at0x0029101C..0x0029107C receives its semantic name; Resource::getByml is already named by placement intake. No function/data interval moves.

The leaf now uses the retail unsigned0..4 enum test, rejecting negative bit patterns. Its ordinary separate translation unit retains the external call boundary. Source strings follow the separately committed212-byte ownership evidence. Constructor104 and leaf16 are enrolled M for exact checking; guarded init408 and World700 are enrolled M for diagnostics, with no functional claim. The abandoned separate List constructor and artificial allocator arguments are not imported. Local ledger times cover checking only, excluding dot preparation.

## 2026-10-01: Canonical CourseList checkpoint from dot/course-list

Both exact proposals pass this repository's canonical check: constructor 104 bytes and unsigned stage predicate 16 bytes. Each has a credited acceptance commit. All fifteen emitted source-data sections also equal the unchanged retail bytes, totaling 212 bytes. This data comparison adds no function bytes. The guarded init and World roots still fail complete-section-size checks. No functional claim or NonMatching ledger outcome is made for those large roots. Local exact ledger times cover only 0.356/0.306-second checks, excluding external preparation. The followup and World packet remain review proposals; the earlier init packet is preserved.

## 2026-10-01: Review heap-size source from dot/scene-resource-heap

Independent retail control flow proves the 8 MiB default on null stage, empty table and no match, the first matching record's unsigned float conversion, and the six-argument heap helper ABI. Separate heap callers establish parent, lock and direction arguments. The existing source omitted an argument and left size uninitialized on a non-null no-match path. The guarded patch repairs those defects in one MemorySystem source file without headers. Five symbolic strings occupy existing unchanged data rows; no data edit is needed. The three resource/Byaml import identities are already named by placement/course intake.

The 320-byte root is enrolled M for a local canonical diagnostic. The dot reports eight capped variants and grade m; those reports do not assign a local grade or prove behavior. The report and packet preserve the final source form separately from their earlier checkpoint excerpt. Bounded local validation is being prepared with explicit allocation/resource contracts. Exact-byte contribution remains zero unless the checker passes.

## 2026-10-01: Canonical heap diagnostic from dot/scene-resource-heap

The committed MemorySystem source builds normally and main links/exports. Its complete320-byte heap root passes provenance/shape gates but reports M->m for differing linked bytes. Local candidate/object hashes and diagnostic time are frozen in build/dot_proposals/scene-resource-heap/local_guarded_diagnostic.json. Behavior validation is pending; this result adds no exact bytes and no functional NonMatching ledger outcome yet.

## 2026-10-01: Review complete orders from dot/execute-director

The source-only director patch replaces eleven incomplete scalar records with their independently recovered arrays: 58 update records and 67 draw records. Retail consumers establish the 16-byte name/type/capacity/category layout. Source preserves all 125 capacities and 375 CP932 strings, including four escaped trailing backslash bytes whose unescaped forms failed the dot's compiled-data check. Existing named table rows retain their complete intervals; no data row or boundary changes.

Independent four-queue allocation/consumers identify ExecuteRequestKeeper construction. Update/draw table initialization and separate field consumers identify two init methods and six registration/list methods at existing unnamed rows. Their nine names are admitted without implementations or accepted ranks. Root read the unchanged intervals and reviewed the dot report and local independent retail/compiled-table diagnostics. Six director functions are enrolled M for canonical checking, totaling 1012 bytes. Compiled-table semantics must be checked separately from code bytes. The bool table returns and name-first global wrappers belong to the already reviewed local package and are deferred to its source checkpoint; these director callers ignore the return and do not call those globals. Local ledger times measure checking only.

## 2026-10-01: Enroll the actor-group helper checkpoint

Twenty-one existing named U intervals are enrolled M, totaling 1240 complete bytes, with no names, boundaries, data rows or header changes. The only source edit corrects HitSensorKeeper attack dispatch to the current sensor's host while preserving the contacted host's dead check; independent retail dispatch establishes that ownership. Twenty other bodies remain unchanged. Three roots require the established committed-source inline closure for unmapped isDead/isClipped helpers; no standalone address is invented.

The manifest verifies current-source hashes and unchanged import identities. Ledger attempts record the reported distinct source variants plus one local canonical verification. Reused translation-unit compiles and setup failures remain listed separately in build/phase_two_actor_group/ready_candidates.json. Measured prior minutes are shared script windows repeated across related functions, then the local check time. They exclude unmeasured reasoning/selection and are not independent labor totals. The earlier group search swept eight shapes despite its first paired exact; that unnecessary historical search remains honestly recorded. Future lanes stop at the first paired exact.

## 2026-10-01: Verify executor proposal and restore lane separation

All six dot director claims pass locally, adding 1012 exact bytes, with separate credited commits. Root independently resolved every canonical table relocation and compared all eleven extents, 125 capacities and 375 NUL strings against retail. Tables contain 2000 source bytes; complete string-pool placement and table pointer bytes are not claimed exact. Normal main links/exports, and unchanged progress.py reports 168 matches and 12504/3,092,336 bytes of word similarity. The strict headline is 168 functions and 12980/2,756,024 complete function bytes, measured separately.

Dot source intake now uses root's lane. Other lanes resume separate eligible small/medium U families while runtime continues. Blocked entries, unanswered packets and U functions at least 0x400 bytes remain reserved for dot. Seven hard-function packets are committed. The paused Pro relay remains unchanged.

## 2026-10-01: Preserve bounded draw-transfer interfaces

The runtime reports the observed six-word uniform-6 packet and descriptor byte+5 command-copy gate. Original CPU block execution verifies137 packets and548 gate cases with no original-code edits; independent native reads match822 owner words plus four finite-bit fixtures and four gate fixtures. Forty-two malformed cases reject under sanitizers. Scene, topology, mesh/material,50 asset hashes and3422 debug/O3 math comparisons remain valid. Root reviewed the two-file diff and verified24 frozen source/note/driver/report hashes, three executable hashes and all preserved dump/code hashes. Evidence is data/runtime/romfs/native_draw_transfer_frozen/command_result_manifest.json.

[Public PICA register documentation](https://www.3dbrew.org/wiki/GPU/Internal_Registers#GPUREG_SH_FLOATUNIFORM_INDEX) independently identifies float32 mode and reverse component transfer. The retained packet interpretation follows the unchanged retail producer; shader arithmetic, vertex transforms, material state, GPU execution, rendering and gameplay remain unresolved. Texture-reference identity is the next bounded resource-local slice.

## 2026-10-01: Complete actor-group canonical acceptance

All21 enrolled actor-group roots pass the project's committed-source canonical checker, adding1240 complete bytes. Each has a separate acceptance commit. Source closure consumes the independently reconstructed dead/clipped helpers with no residual helper code or invented target addresses. Strict coverage is189 functions and14220/2,756,024 bytes. Fresh small/medium matching proceeds in parallel; dot intake remains one lane.

## 2026-10-01: Restore executor wrapper ABI and registration returns

Seven reviewed local CPP/header files correct two global wrappers to name-first arguments and four table methods to boolean returns. Independent LiveActorKit caller registers establish the global order; director methods remain functor-first. Independent registration loops return a success accumulator and visit every matching list. The private ExecutorRegistrationList view describes only the existing vtable/name prefix and is never allocated. No complete executor class layout is asserted. Two existing global map names are corrected, and the independently supported request import is named at its unchanged interval while its blocked implementation stays reserved.

Eight new roots are enrolled M, totaling564 bytes. Four earlier dot/local overlaps remain accepted and receive no duplicate ledger row. The corrected dot source is preserved byte for byte. Ledger attempts count scratch structural variants plus one local check; prior minutes are the recorded shared executor windows, repeated across related roots and not isolated labor. Root will rebuild, check new roots, and recheck all189 previously accepted functions because shared headers changed. No function/data boundary, data row, compiler flag or checker change is made.

## 2026-10-01: Freeze bounded placement and heap outcomes from dot proposals

Root reviewed the retained source-generated canonical candidates, complete original intervals, frozen artifact hashes and explicit callback contracts. Placement initialization agrees on76/76 ARM11 execution pairs, covering38 synthetic documents with two callback clobber modes. Its33 differing bytes remain rank m. The modeled archive lookup and creator return exclude real archive behavior, allocation and derived actor initialization. Full factory scanning, Byaml traversal, initialization handoff and callee-save/canary checks execute unchanged retail helpers. See placement_initialization_validation.md, credited from dot/placement-map.

Heap creation agrees on380 returning pairs and30 separate fault-agreement pairs across41 synthetic fixtures, five FPSCR modes and two clobber patterns. Resource lookup, document retrieval and allocation are explicit models;17 Byaml/string/float helpers and the final output/flag helper execute unchanged. Null-stage defaults, first matching record, unsigned float conversion and heap flag writes agree in this bounded contract. Faulting malformed Stage cases are not successful returns. Out-of-range conversion probes establish only this emulated instruction behavior, not portable C++ or hardware guarantees. The320-byte root remains rank m. See scene_resource_heap_validation.md, credited from dot/scene-resource-heap.

Both ledger rows include eight dot structural attempts plus one root canonical check. Minutes measure only the final validation windows,1.208631 seconds for placement and2.239063 seconds for heap; external preparation and untimed root work are excluded. These are guarded NonMatching outcomes and add zero exact bytes. Neither result proves gameplay or replay.

The executor shared-header checkpoint passes all197 accepted roots again from canonical committed-source objects,14784 complete bytes. The frozen result is build/executor_header_recheck_frozen.json. Rechecks add no duplicate ledger matches.

## 2026-10-01: Preserve resource-local texture identity

The isolated native reader now retains three material mapper/reference slots, encoded names, raw cache fields and resource-local direct target identities. Independent resource-byte reads and unchanged original resolver execution agree for217 direct references and164 absent slots across127 materials. Four original missing/alias fixtures and eleven native identity/status/cache fixtures preserve explicit unresolved states. The reader reports raw cache values without applying them or guessing alias recursion. Texture images, sampler state, shader/material interpretation and rendering remain unresolved.

Root reviewed the bounded offset/pointer checks, deferred model traversal and explicit unsupported statuses, then verified every frozen manifest input/report/executable/dump hash, including the keyed executable paths. Fifty-one malformed-input cases reject; prior scene/geometry,822 draw words,548 original gate cases and3422 direct-bit math comparisons pass. Original/working dump, executable and selected asset hashes remain unchanged. Evidence is data/runtime/romfs/native_texture_identity_frozen/command_result_manifest.json and project/native_mesh_evidence.md. No Game/al source or matching flags change.

## 2026-10-01: Enroll actor pose and rail access checkpoint

Root reviewed the50 complete intervals and immutable paired diagnostic artifact hashes, totaling912 bytes. Existing names/layouts support LiveActor pose+0x14, translation+4, independently observed virtual slots and concrete pose member offsets. The only CPP edit replaces bulk vector assignment in setScale/setTrans with ordinary memberwise x/y/z copies, matching retail order without flags or headers. All remaining pose and rail functions retain committed source bodies. Rail keeper/rider fields agree with independent constructor consumers. The goal wrapper uses normal linker branch relaxation; its imported rider method is not claimed. No map boundary/data edits or new identities are needed.

Candidates are enrolled M before the normal canonical build. Structural counts are one per unchanged body and two for the two memberwise forms; the root check adds one ledger attempt. Times retain shared measured scratch windows of3.541450 seconds for access,2.480749 for mutation with0.954885 extra for its second form,5.179291 for reused getters and2.586160 for rail. Shared windows overlap and are not per-function labor. The initial access environment failure precedes code generation, was retried with the same structure and is preserved separately. Canonical acceptance remains pending and checks affected previously accepted roots separately.

## 2026-10-01: Identify existing link-child text and string-table getter

The unchanged data row3A2E58..3A2E6C contains GenerateChildren plus its terminator and padding. Independent retail pointer words2670D4 and27AB5C both reference its exact start. Root verifies the words and bytes and assigns the address-derived dat_003A2E58 name separately from CPP/rank intake. No row is added or split.

The unchanged24-byte function28CA30..28CA48 loads the table base at receiver0, reads the word at4+4*index, then adds the table base. Independent String-tag key and index readers call it with the header+8 string-table pointer and payload index. This supports ByamlStringTableIter::getString(int). The existing guarded provider remains unaccepted; only its independently evidenced name is added. No boundary or rank changes.

## 2026-10-01: Compose the Byaml and placement source checkpoint

Sixteen candidates total1280 complete bytes: two existing committed closure readers, six placement readers, six initialization/link helpers and two typed Byaml wrappers. Three literal-to-import substitutions retain original empty, hyphen and Rail identities at existing rows; link indexing uses the separately evidenced GenerateChildren row. The free ActorInitInfo wrapper copies placement and fields4/8/10, then derives ViewId from the stored placement pointer. Independent constructor/copy/init consumers support that24-byte record. Only that now-exact wrapper loses its stale NonMatching guard/comment. The two minimal Placement patches share declaration context, so root composes their exact substitutions while retaining both bodies.

Two already declared Byaml methods are added without changing existing bodies or headers. Independent Camera::load and indexed-string callers support the names at unchanged rows271210 and290F00. The standalone integer converter remains excluded because its exact scratch form perturbs an accepted inline reader. Existing int/bool/float readers must pass canonical rechecks after this CPP addition. Source-generated constructor closure is retained; no helper address is guessed.

Prior structural counts and shared measured minutes are retained from immutable candidate packages, with one root canonical check added. The key-data reader's old timing is not isolated, so its ledger row measures only checking; the string-key wrapper retains its shared4.604-minute window. Scratch evidence is diagnostic and ranks remain M until canonical acceptance. No headers, data/function boundaries or flags change.

## 2026-10-01: Enroll sensor, execution and actor access checkpoint

Sixty-nine candidates total2348 complete bytes: fifty sensor/message helpers1116, seven actor state helpers264, six request/list helpers700, five KeyPose helpers136 and SceneObjHolder constructor132. Retail sensor table17 name/type entries and accepted lookup establish the enum values. Host+28 and LiveActor receiveMsg slot34 are independently supported by accepted dispatch and table consumers. Only the final two isSensorEnemy comparisons change order, retaining the same recognized values and reproducing retail's4/5/6/8/7 sequence.

The actor clipping body enters its callback path when clipped is true, correcting one negated condition. Independent incoming calls and callee behavior support three unchanged-row Collider/ClippingActorHolder import names. No complete Collider storage is claimed. Request queues and execution-info fields are independently established by allocation/constructors and director consumers, including four ordinary inline pointers and draw-list slot18. Address-derived list functions retain unknown public names. The isolated request proposal uses a new alExecuteRequestKeeperFunction.cpp so the existing accepted constructor source is preserved.

KeyPose wrappers move their three bodies verbatim into alKeyPoseFunction.cpp; stale wrapper guards/comment are removed with the old definitions. Translation-unit separation preserves retail calls/save frames without flags or special annotations. Accepted constructor/initializer callers independently establish the36-byte element stride and fields. SceneObjHolder adds an ordinary callback/count constructor, array allocation and pointer-zeroing loop; an independent caller allocates12 bytes and passes23 as count. Its existing callback type returns void*, and root corrected an intake-only typed-result spelling before changing map metadata or building. All existing method bodies, headers and data/function boundaries remain unchanged.

Ledger counts retain one or two structural forms for all bodies, with one root check added. Sensor minutes retain the shared12.987063-second two-batch window, which excludes preparation and overlaps all50 targets. Actor state timing retains its shared measured script windows; request/list timings retain their overlapping preparation windows. KeyPose and SceneObjHolder timing covers only paired compiler windows, excluding prior reasoning. Recheck affected previously accepted source roots after this checkpoint; no duplicate matches belong in the ledger.

## 2026-10-01: Migrate the retained Collider import consumer

The first normal link after identifying Collider::onInvalidate reports one undefined old address alias from the existing makeActorDead source. Root removes its old C declaration and changes that one call to the independently identified member method, including the existing Collider header. The argument ABI and original address remain unchanged. No compatibility alias, new boundary, header layout, rank or flag is added. Root rebuilds and rechecks accepted LiveActor roots because that translation unit changed. The initial69-root canonical acceptance had not started when the main-link failure occurred.

## 2026-10-01: Enroll effect deletion, lookup and emission helpers

Eight candidates total824 complete bytes. Independent entry/reference constructors, object creation and deletion consumers establish live count, identifier, byte flags and resource pointers. The object pointer region10..30 ends at a separately used record array, supporting eight ordinary pointer slots. These private views are never allocated or asserted as complete original classes. The two deletion loops retain their borrowed entry semantics and address-derived names; unknown public method names remain unknown.

Named effect-set constructors and independent interface/setter callers establish name0, keeper count4/buffer8, set entry buffer4/count8/activity18, and config/position/matrix pointers. Separate keeper/set translation units preserve original call boundaries. The emission wrapper sets keeper activity10 and dispatches its matching borrowed set with the existing position pointer. Only independently supported fields are represented; two capped set-emission bodies remain scratch proposals and are excluded from production.

Three deletion targets use three/five actual structural variants, four lookup targets use two, and emission uses four. Root adds one canonical check per ledger outcome. Shared source/inspection windows retain their actual overlap and exclusions, including19.724 minutes for entry deletion, the separately recorded object-loop window,8.398 for lookup and7.879 to first emission equality. Failed environment setup invocations precede valid code generation and are recorded separately. No shared headers, target/map boundaries, flags or object edits occur. Recheck affected accepted effect source roots after the normal build.

The previous source checkpoint passes all69 new candidates. All33 previously accepted source roots affected by actor/import/KeyPose/SceneObjHolder changes pass again; evidence is build/sensor_execution_actor_previous_accepted_recheck.json. Strict coverage before this new intake is332 functions and19324 complete bytes. The obsolete fn_0024C9EC alias has zero references in lib/al and map.csv after its caller migration; historical decisions remain retained.

## 2026-10-01: Preserve serialized texture payload headers and extents

The isolated native reader now records145 observed image TXOB records,580 header fields and580 descriptor fields, with complete serialized extents totaling1851264 bytes. Independent raw reads and extent hashes agree. Original read/execute consumer execution agrees on payload/format/dimensions for217 actual bindings plus nine separately labeled unbound-catalog fixtures,226 cases and16900 instructions. Execution stops before platform-address translation. Raw cache fields stay unapplied; unsupported layouts/formats and dimension/mipmap cases have explicit statuses. Pixel/per-mip decoding, sampler/material/shader behavior and rendering are unverified.

Root reviewed the source bounds/status handling, froze34 input/report/executable/dump hashes, and checked the independent payload report. Sixty-two malformed-input cases reject; prior scene, geometry, identity, transfer, topology and3422 direct-bit math regressions pass. All50 selected asset hashes and original/copy/code hashes remain unchanged. Native source remains outside the matching build. Evidence is data/runtime/romfs/native_texture_payload_frozen/command_result_manifest.json and project/native_mesh_evidence.md. Public CGFX documentation corroborates image fields and numeric formats; direct page access returned403, while indexed primary-page text confirmed the table at https://www.3dbrew.org/wiki/CGFX. Owner records and unchanged retail consumers remain the direct field oracle.

All eight effect helper candidates pass canonical checking,824 complete bytes. Four previously accepted roots from affected effect translation units pass again. Evidence is build/effect_deletion_lookup_emission_previous_accepted_recheck.json. Strict accepted coverage reaches340 functions and20148 complete bytes; rechecks add no ledger duplicates.

## 2026-10-01: Park three capped effect functions with self-contained packets

The two set-emission functions reach eight measured structural forms with8/16 differing bytes over192/204-byte scratch extents. The164-byte entry-creation function also reaches eight forms, with114 differing bytes in its simplest full-size candidate. Its separately compiled helper form retains44 allocated helper bytes and is correctly rejected rather than assigned an invented address. Genuine unedited ARMCC object/link evidence is retained. These are abandoned outcomes, not accepted matches or functionally verified NonMatching code.

Packets001E98DC,001E9A10 and0023F494 contain complete asm/pool bounds, best C++, needed clean headers/layouts, current original-address scratch diffs, all eight forms and actual unchanged compiler commands. tools/diff.py cannot select their currently unnamed/unimplemented production roots; each packet explicitly states this and supplies the genuine scratch diff rather than inventing a canonical result. Root verifies packet hashes/size and commits them for the dot/owner. Pro relay remains paused. Ten packets are now committed, below the approximate15/day budget. No original method spelling is inferred. Production code/maps/ranks remain untouched by these capped candidates.

Ledger minutes retain the final shared source-search windows,12.631603/12.632737 for the set routines and15.099394 for entry creation. Preparation before source creation and later packet packaging are excluded; the two set windows overlap. Eight structural forms are recorded per row, distinct from sixteen paired target compiler invocations or extra helper compiler invocations. Remaining large-function blocker descriptions are refreshed to their actual local canonical diagnostics, preserving bounded behavior limits.

## 2026-10-01: Enroll reaction, placement, camera, scene and nerve checkpoint

Thirty-eight fresh candidates total1032 complete bytes. Four ActorInitInfo placement readers retain current committed bodies/imports, independently supported by constructor fields and named callers. Eleven reaction wrappers retain Shift-JIS source bytes; only BlowHit uses the separately observed existing data start3A97E0. Independent common-tail callers support startHitReaction at the unchanged full27A590..27A5C0 interval. No data row is added or split, and that imported root remains U.

Camera loader, rotator initializer and seven virtual defaults retain current source. Independent Camera constructor fields/address point and parameter/allocator consumers support their ownership and call ABI. Literal strings stay within original function extents. Four Scene state/init methods retain source; their alive flag and holder fields have independent constructor/consumer evidence. Two address-derived execution-info helpers use independently established28-byte records with four ordinary pointer slots and no full original class spelling claim. The excluded56-byte registration/helper candidate remains structurally inadmissible despite raw equality.

Four nerve roots retain current source: action-controller/collector construction, conditional current-nerve access and empty executeOnEnd. Existing accepted consumers establish counts, linked-list/pointer fields and current/pending nerve offsets. The collector literal resolves the existing named static pointer row. Four separate unverified nerve probes are excluded; no proposed scratch header is imported.

Four global SceneObj wrappers call existing original prefix entry267230..267234, a four-byte mov-r0-r0 that falls through into the accepted getter at267234. Five retail call sites independently establish this entry and its returned-r0 holder ABI. The source imports it only by its existing numeric address. It is unimplemented, remains U and adds no matched bytes. No new public name, merged boundary or prefix acceptance is claimed. This minimal import preserves the wrapper call destinations without altering established holder methods.

Structural counts and actual shared windows are retained from each frozen package, with one root canonical check added. Camera minutes cover paired compiler windows only; nerve minutes cover the2.799615-second first batch. Actor-placement timing starts at the interrupted first object mtime and omits its unrecorded compile duration. Reaction and SceneObj wrapper windows overlap among their families; no isolated labor totals are inferred. Root rebuilds and checks source-frozen objects, then rechecks earlier accepted roots in changed translation units.

## 2026-10-01: Repair NerveStateBase table ownership separately

Independent constructor literal2684FC installs address point3D72EC. The base table starts with two zero ABI words at3D72E4 and has eight entries through3D730C,40 bytes. Accepted state/getter/update consumers and an independently constructed derived state support its slots and12-byte object. Separate preceding constructors install3D72DC/3D72CC; successor stack-object and following allocation consumers install3D7314/3D732C. These independently establish the adjacent owners without assigning their public names.

Root verifies the table hash, constructor literal, frozen evidence hashes and source table shape. The old two unnamed rows3D72DC..3D72EC and3D72EC..3D7314 become three rows: prior-owner tail3D72DC..3D72E4, named full table3D72E4..3D730C, and successor header3D730C..3D7314. The exact56-byte union, all bytes and U ranks remain unchanged. No function boundary changes. The two observed null destructor slots are not executable destructor addresses. This data-only evidence commit precedes constructor enrollment as BRIEF rule2 requires. Full evidence is project/nerve_state_base_data_evidence.md; a separate inconsistent code_end observation remains unmodified.

## Stage, area, collision, effect-interface and NerveStateBase enrollment

Thirteen roots, 396 complete bytes, enter rank M before canonical checking. Ten existing stage/area/collision/JMapInfo roots require no source changes. Their independent constructors, serialized fields, array strides, pools and unchanged imports are frozen in build/phase_two_stage_area_access and build/phase_two_collision_attribute_setup. Every frozen package hash and current source hash passes intake review. The two ordinary effect-interface forwarders add one translation unit and address-derived names; independent EffectObj callers, accepted secondary-interface getters and accepted emission/deletion imports establish their ABI. No original method spelling is claimed. The NerveStateBase constructor uses unchanged source after the independently evidenced table ownership repair in d5d925f. No function boundary or shared header changes here.

Ledger effort is one structural form plus canonical checking. Stage/area and collision rows use each package's overlapping measured paired-script seconds, excluding preparation/reporting. Effect rows use the shared 0.016427350-minute first-equality window, excluding prior selection/inspection and later packaging. NerveStateBase uses the measured 0.231255375-second compilation window plus canonical checking. These windows are not independent labor estimates; compiler repeats do not count as new source forms.

## RGB565 native storage checkpoint

Root reviewed the two-file runtime patch and all 149 frozen input/report/executable/fixture paths against manifest ce3329e75dab2d7150a06c7dc9efa09ed43af36ef6a98fbd538b4052feff7e46. Source is 82110a07a0ae31f0d495edbd377f988813a11f7b44faeda77662de7b88d8f304; the evidence note is 1d74a360c603f2de23eaa08a195c8783fc53ad59b302f74b89eebb1684b1c47f. The final validator, sanitizer builds and prior native regressions pass. One earlier scratch-driver failure expected a diagnostic prefix with a space; the corrected expectation passed twice without another production revision.

Eight RGB565 resources, eleven complete supported mip levels and 75,648 bytes yield 37,824 packed words and 113,472 channel integers. Direct Morton indexing and a separate recursive traversal agree on every source offset. All 65,536 word values, six multi-tile patterns, seven unsupported/cache cases and six malformed inputs pass. The public SMDH/GPU register descriptions independently establish the bounded tile convention; OpenGL ES 2.0 section 3.6 establishes component bit positions. The reader preflights the entire extent before decoding, retains unsupported tails/formats explicitly and never applies serialized cache pointers. Eleven actual retail mapper cases establish intake only. GPU sampled pixels, color expansion, display orientation, sampler/shader behavior and rendering remain unverified. No game data enters this commit.

## Stage-switch, matrix, numeric-placement and dot item-name enrollment

Six roots enter rank M, totaling1460 complete bytes. The 84-byte StageSwitchKeeper constructor has independently established five-type enum cardinality, eight-byte accessor layout and runtime imports. Four structural forms reach paired exact using compiler/linker phase separation: a separately committed ordinary count helper disappears fully and receives no guessed original address. All frozen package hashes pass. The minimal patch removes the obsolete constructor NonMatching guard, whereas the scratch source retains it under the same defined macro; an initial exact-file hash assertion caught this documented guard difference before enrollment. Root verified the complete remaining source is identical. Ledger time includes only the measured final paired compile/link window,0.825281 seconds, plus checking, excluding prior preparation and failed-form work.

The 84-byte makeST specialization uses independently evidenced row-major 3-by-4 storage and named callers. Only its established declaration enters the clean matrix header; its ordinary source joins the existing al math unit without configuration/flag changes. The first structure is paired exact. Ledger time uses the overlapping3.856519-minute first-equality window, not later reduced-file packaging. All previous accepted canonical roots must survive the shared-header recheck, without duplicate ledger credit.

The three numeric-placement/rail roots contribute176 bytes. Independent callers, existing Byaml imports, null/invalid checks and integer-minus-one rejection establish their ABI. The float provider uses an ordinary inline helper that disappears at compilation, preserving its caller boundary. All287 package hashes pass, the current placement-source hash equals the frozen base, and the minimal patch preserves prior bodies and headers. Rail/Arg0 require one form each; the generic float reader reaches first paired exact on form8. Ledger times retain the overlapping0.329801/16.593263-minute source-search windows plus canonical checking. The capped translation reader remains separate and unaccepted.

The dot/item-type-lookup proposal contributes an unchanged1116-byte interval using ordinary ordered string comparisons and compiler-generated literals. Independent retail callers consume the integer as an item discriminator; the accepted string comparison is its only import. Original public method/enum spellings remain unknown. Root imported only this source and report, source SHA0430d41ceefa8631cb5ad97d31bf939e1b33a2b8c1f23c4817e27644b2a4bf37. The dot reports one form; root records check-only minutes because its prior wall time is unavailable. No dot map/rank/ledger/tool changes enter main. Local canonical acceptance is required before credit.

## Matrix-header freeze, constructor rejection and three new packets

The shared-header freeze preserves391/391 previous accepted functions at21576 bytes. The ignored runner initially required one unique weak definition for Nerve::executeOnEnd and stopped on nineteen ordinary ODR copies. Root checked every canonical committed-source weak definition independently; all nineteen pass the unchanged checker. Frozen report: build/stage_matrix_header_recheck_frozen.json. No rank/oracle modification resolves the inventory ambiguity, and no duplicate ledger credit is added. Five new roots pass, adding1376 bytes. StageSwitchKeeper's source-closure check rejects because the linked selected root lacks its unique original start/full size; scratch84-byte equality is unaccepted. Its producer is diagnosing unchanged canonical artifacts.

The capped172-byte ActorInitInfo translation reader retains9 differing bytes after8 structures. Its221-line packet, complete target/pool, best source/headers, actual differences/flags and forms are committed. Ledger time is the overlapping measured15.872918-minute window excluding target selection and packaging. Quaternion has7 distinct structures in8 paired runs, including one unchanged repeat; its249-line packet records an early structural stall and best204-byte/131-byte-different output without overstating the attempt cap or functional correctness. Pro relay remains paused; thirteen unanswered packets now remain available to the owner's dot.

The dot/effect-action-update branch explicitly claims no exact function. Root imports only its report and594-line packet while independently reviewing three whole string virtual tables. Its common SafeString/source/caller changes are deferred until those identities close; dependency closure makes the next useful work data evidence, rather than repeated code generation. No source/rank/ledger/functional credit follows from equal length or the external claim.

## Pool-inclusive inline-closure verifier repair

The StageSwitchKeeper rejection exposed a general linker metadata bug: ARM Code symbol size80 excludes its trailing4-byte literal pool, while the unchanged function interval and sole source-generated allocated/memory extent are84 bytes. The untouched canonical AXF is byte-equal across all84 bytes; C1 has one original-start symbol and C2 is a distinct zero-size alias. Producer evidence is frozen under build/phase_two_stage_switch_keeper/canonical_rejection. The verifier previously required code-symbol size to equal the complete interval, incorrectly rejecting valid pools.

The repaired gate still requires one selected ARM symbol from the projected root at the original start, with a positive aligned code size contained in the full extent. Both independent allocated-section and sole-root memory-entry checks still require exact original start/full size; retained helper sections still reject. The unchanged complete byte comparison includes the literal pool. Evidence records instruction size separately. No target, boundary, differ, progress, provenance or compiler flag changes. The general production guard suite preserves automatic/explicit closure equality and refuses unrelated/copied/missing/ambiguous helpers, residual helper code, shared sections and a real byte mismatch. The new canonical-object pool diagnostic passes80+4 bytes without rank/map writes. Evidence: build/pool_inclusive_closure_validation/evidence.json and build/pool_inclusive_closure_guard_validation.log. Acceptance waits for the tool commit and required clean main build.

## Clean-build BSS scaffold dependency repair

Root reproduced the independent audit: a full python make.py eu -ca fails with undefined Vector3<float>::zero from AreaShapeCube. Initial stub generation occurs before ordinary compilation. The named-BSS scanner correctly refuses absent/stale canonical objects, so the first clean stub source lacks the verified twelve-byte U/db zero import. A later incremental build previously supplied it, making successful working-directory links insufficient proof of reproducibility. The audit correctly identifies a build-order defect.

The build now refreshes the existing provenance-gated scaffold after ordinary modules compile and immediately before the appended generated-stub module is compiled/archived. Its changed boolean forces that module's refresh even within one-second source timestamps. Data identity/extent/provenance filters, source objects, target and matching flags remain unchanged. Read-only diagnosis finds seven current verified zero references; absent-object control finds none. Evidence: build/phase_two_actor_group/fresh_build_bss_diagnosis/evidence.json. A first clean build must link/export before this repair is pushed. Every future push requires clean linking, and final M2 additionally requires whole linked code.bin hash equality; per-function counts alone cannot finish the project.

## First-pass clean link and ETC1 native storage acceptance

The repaired python make.py eu -ca completes compilation, links and exports on the first pass. Root independently reproduced the prior failure and observed the repaired full clean result in build/clean_main_link_repair.log. The pool-inclusive constructor now passes the committed checker on its newly clean-built canonical object, giving397 exact functions/23036 complete bytes. The final M2 exit requires both full source-generated function coverage and original EU whole-image hash. Compact scaffold success does not satisfy that exit.

ETC1 changes remain limited to runtime/AssetReader.cpp and project/native_mesh_evidence.md. Root reviewed the diff, primary Khronos block definition and all199 frozen input/report/executable/fixture paths in native_etc1_frozen/command_result_manifest.json, SHA733a6448a3eef3dc634e8aed002ab541aeed5d6ca51c8ed94010df7962d4a884. Source SHA66248808d20014732a6d5b5008d0f6e03b504cf18d38de6ab2986149752cab45 and note SHA53be93473cb8a781dec05938dbf38df163bd567f85bc54ecf6980cf229097788 match production. All110 images/360 supported mip levels cover1136000 bytes and142000 blocks. Every coordinate/source offset, endpoint/delta/table/mode field and2272000 selectors agrees with independent byte/arithmetic extraction and direct/recursive traversal. Undefined endpoint domains remain raw and explicit; no modifiers or colors are produced.

Six tile patterns,4096 arbitrary words,64 selector fixtures,256 delta/base cases,256 mode/table/partition combinations, unsupported/cache and malformed-chain checks pass. Combined damaged-input cases reach74. RGB565, mesh, scene and3422 retail debug/O3 math regressions pass, with original/copy/code and all50 asset hashes unchanged. Retail ETC1 CPU intake comprises159 actual mapper cases/101 images plus9 separately labeled unbound fixtures. Twenty-seven other-format images retain unsupported storage status. GPU sampling, normalized colors/alpha, orientation, material/shader behavior, rendering and gameplay remain unverified.

## Independent AreaShapeCube table boundary repair

The existing named Cube table covers28 bytes but independently belongs to20: ABI header at3D62B8/Bc plus entries330868,0,33091C. Its constructor installs address point3D62C0 from literal1C5108. The next derived constructor independently installs3D62D4 from literal1C5120, proving the next two-zero ABI header at3D62CC/D0 belongs to that next owner. Root checked the literal words and all seven data words directly against the unchanged dump, and verified every frozen package hash. The20-byte named row plus anonymous8-byte successor header preserve the entire old28-byte union. All function boundaries/ranks and all existing neighboring anonymous rows/names are unchanged.

Source tables are not claimed exact: two unknown Cube slots remain incompletely reconstructed. This separate data-only evidence commit does not accept any constructor or manufacture a matching section. The constructor/source proposals remain pending canonical verification. Evidence is project/area_shape_cube_data_evidence.md and build/phase_two_area_shape/manifest.json.

### Capped integer argument reader family

Ten established92-byte U intervals reached eight ordinary C++ structures with unchanged flags. Their common full-extent miss is38 bytes;88-byte candidates also differ and cannot count. Root verified all697 frozen manifest files, copied the self-contained representative packet002794F8.md, and recorded ten abandoned outcomes. Every ledger row uses the same measured5.032019-minute first-source-through-eighth-result window,16:38:12.390047 to16:43:14.311199UTC. These overlapping timings exclude later packet preparation and are not independent person-hours. No source, map name, rank or boundary is changed. The untested separate-TU/source-closure helper hypothesis is reserved for the dot through the unanswered packet.

### Composed helper-family source checkpoint

Root verified and composed82 proposed functions,2472 complete bytes:17 actor factory/creator roots,32 lifecycle/access/dispatch roots,22 Boolean/float placement roots,4 layout/area roots,5 effect/audio roots and2 scalar/matrix math roots. Factory registration adds four independently observed string/creator entries and removes only the obsolete constructor guard. Byaml prefixes and suffixes compose without replacing the shared source. Effect appearance, deletion and control call independently established output-matrix/interface destinations; the audio wrapper preserves the observed integer count ABI, with downstream semantics unknown. The sensor callback remains an address import on its established unchanged interval. Address imports/data aliases use the existing provenance-checked scaffold mechanism, not guessed providers. Native math records declare existing static APIs and external data extents without copying target bytes.

All function starts, ends, pools, types and data rows remain unchanged. New names identify existing unnamed roots from independent table/key/ABI evidence; initial M enrollment claims no equality. No shared header or compiler flag changes. The current matrix header contains the earlier makeST declaration; affected older-header scratch packages must pass a current-header canonical rebuild. All earlier397 accepted roots will be rechecked after the clean build, including every weak ODR definition independently. Frozen queue metadata is build/helper_family_checkpoint_candidates.json and source/map intake hashes are adjacent.

Ledger times will use each frozen first-paired equality window plus the measured canonical check. Many windows overlap and exclude untimed inspection/preparation; layout constructor timings are compile-only. Source forms plus one actual canonical attempt are recorded, with cleanups and reduced packaging kept separate from structural counts. No independent person-hour or rate claim follows.

### Unaccepted whole tables in the compact scaffold

The82-root clean intake exposed three undefined speculative LiveActor method imports through AppearStep's generated table. Independent retail constructor/table and accepted secondary-thunk evidence shows zero primary slots for those methods, so no retail function names or addresses are invented. AppearStep's native table is152 bytes while its established complete mapped U table is160. The previous compact importer retained this unaccepted table solely because the native/mapped sizes differed, causing its speculative entries to enter the link.

The compact importer now requires a unique full source table symbol at offset zero, source-symbol size equal to its complete native section, aligned nonempty source and mapped tables, the established named U const-data row and matching section identity. It imports the whole mapped zero-filled scaffold table instead of its unaccepted native contents even when sizes differ. Projection evidence records both sizes and table_bytes_accepted=false. It never accepts table bytes, changes source objects, changes matching checks or changes any map boundary. Canonical committed-source function checks remain unchanged. Clean linking and a full397-root preservation recheck will validate this build-only correction.

The82-root checkpoint clean build and full397-root preservation recheck pass after the table import repair. Thirteen native source table sizes differ from mapped full U extents; every such import records table_bytes_accepted=false. The LiveActor interface identity audit is committed as project/live_actor_interface_identity_audit.md. No unadjusted method name is invented from a zero primary slot or substituted from an adjusted interface thunk. All82 proposed functions then pass canonical checks individually. The strict count is479/25508 bytes; the unchanged progress tool reports23652 word-similarity bytes.

### Fresh-checkout factory template section classification

The MacBook audit of4f70f61 could find474 of479 accepted roots through the linked map. A separate local fresh clone with no build state reproduced the five missing factory template symbols. All five freshly compiled canonical ARMCC objects still pass the unchanged exact checker. Their weak template definitions occupy compiler sections named t.<symbol>, but five existing map rows were classified f, which generates i.<symbol> scatter selectors. The unmatched sections landed at0x01000000 outside the normal code region. The linked-symbol reader ignores those addresses. This explains the failure without claiming stale source output.

Change only the Type field from f to ft for createActorFunction<AppearStep>, <Fugumannen>, <GhostPlayer>, <TrickHintPanel> and <TransparentWall>, matching independently observed canonical section identities and the other factory template rows. Preserve every start, pool, end, rank, symbol and explicit section field. No source, original bytes, flags, differ or checker changes, and no new exact credit. Rebuild a fresh clone and verify both linked-map and canonical-object checks before pushing. Frozen reproduction: build/clean_checkout_factory_audit_results.json and build/clean_checkout_factory_audit_build.log.

### Composed helper follow-on source checkpoint

Compose35 independently proposed roots/1452 complete bytes from disjoint actor, lifecycle, string, Byaml, matrix, projection and effect families. Preserve every target boundary and compiler flag. Only existing unnamed target rows receive independently justified symbols; all selected ranks enterM until the canonical checker grades them. Minimal source/header patches add SystemKit construction, align the existing StageSwitchAccesser address-name declaration, expose one nonvirtual serialized hash-pair accessor and add ordinary native helpers. Matrix34 copy overloads share one source class declaration. The effect declaration correction aligns the already accepted provider ABI; it is verified by the canonical build rather than claimed from the earlier scratch output.

Root verified frozen package hashes and minimal patches before composition. Frozen intake and candidate timings are in build/helper_followon_checkpoint_source_intake.json and build/helper_followon_checkpoint_candidates.json. Paired diagnostic timings overlap and exclude unmeasured reasoning. Rebuild all committed source, preserve the previous479 roots including every weak canonical definition, then accept each target only through tools/check.py. No data, boundary, oracle or global flag changes.

### Nine follow-on structural stall packets

Import nine frozen self-contained packets and record only their actual distinct forms: two effect transform setters retain allocated unmapped helpers despite exact root bytes; Matrix33zero, Matrix34 basis/translation copy and transpose, Matrix22multiply, quaternion vector rotation and PlayerActionMultiCondition initialization stall before the cap; the Byaml key-name consumer reaches eight forms. Scratch outputs are not canonical accepts, size failures have no byte-distance claim, and none has functional replay evidence. No production candidate source is imported. All targets stay reserved for the dot. Timings preserve measured overlapping family windows and exclude unmeasured preparation; packets identify compiler commands, layouts, complete original code/pools and remaining gaps.

### Actor callbacks and audio checkpoint

Enroll ten unchanged existing actor/callback/name helpers and three ordinary audio helpers,424 complete bytes. Independent constructors/callers establish private audio receiver views and neutral address identities. Canonical build verifies the pruned ready draft. No shared header, target boundary, oracle or flag changes. Freeze all prior canonical objects before the clean build and require them unchanged to preserve514 accepted roots, then check each new root. Shared diagnostic timings overlap and exclude unmeasured preparation.

### Source-and-placeholder full-image build diagnostic

The owner requested early whole-image linking evidence. Route make.py eu --split to an isolated original-address linker diagnostic, avoiding the legacy Game.s generator prohibited by hard rule3. It consumes only existing provenance-verified committed C++ objects, follows source helper closure, and supplies ordinary required function placeholders plus explicitly opaque zero data/linker fill. Projected objects are diagnostic-only and ineligible for matching checks. Placement, providers, interval budgets, residual helpers and contiguous ELF load coverage are verified before the oracle is read. Unexpected changes in an accepted interval fail the diagnostic after recording its comparison. Whole-image equality is separately reported and cannot be inferred from padding or coincident zeros.

Frozen27af5f6 capture verifies142 objects/328 inputs, preserves479 roots/25508 complete bytes, and exports3096576 bytes from a real link. Its SHA2565e2444fff0585891aa04ef273f576bbcd0e8f386761b37e2574a57adf0429d2a differs from retail in2492635 bytes. No target instruction/data bytes entered compiler/linker inputs. The actual installed entry point will be verified against current527-root main after a clean canonical build; its output remains isolated under ignored build/eu/full_image_diagnostic. Normal main flags and checker/map behavior are unchanged. Frozen proposal/routing evidence: build/phase_two_split_build_preparation/routing/review.md.

### SystemKit constructor alias scatter identity

The independent fresh527-root audit reports one linked-map regression: SystemKitC2 at00101148. Fresh ARMCC outputs define C1(size52) and C2(size0 compiler alias) at offsetzero in the same i._ZN2al9SystemKitC1Ev section. The canonical C2 check already passes the full52-byte mapped interval; the default i.C2 scatter selector misses the actual section. Set only this row's explicit SectionName to i._ZN2al9SystemKitC1Ev, independently observed in both fresh-clone and root objects. Preserve every address/pool/end/rank/type/symbol and all target bytes. Re-run the clean linked-map audit before pushing. This is source-section classification evidence, not a guessed second constructor address or new exact credit.

### Frozen area, actor, audio and native utility source checkpoint

The20-root source proposal covers1136 unchanged mapped function bytes. Root verifies all416/352/527/68/89/151/29 entries in the seven frozen manifests before intake, plus the audio resource evidence hashes. AreaObj+0x40 pointer identity comes from its independent initializer and eight-byte ByamlIter copy; the new getter is nonvirtual and preserves fields/layout. PlayerActionGraph moves its existing +4 getter out of line, and GhostPlayer uses a source-local setter bridge; neither helper receives a retail address, and both must disappear completely under the canonical closure gate. Root removes only move's obsolete NonMatching guard. CourseList uses the independently located Application root-task address alias without inventing a method name. No compiler flags, boundaries, pools or section identities change.

ListImpl::clear is separately guarded NonMatching. Its seven-byte mismatch and306 bounded states/612 pairs are not exact credit; canonical verification is still pending. Source intake only marks21 U targets M. Frozen build/container_reader_checkpoint_source_intake.json records the actual source hashes.

### Canonical bounded native list clear

ListImpl::clear remains m after committed-source clean building and the project checker. Root confirms all 56 canonical bytes have the same SHA as the behavior-tested source output; seven differ from the original. All 612 original/compiled pairs agree on entire heap/stack bytes and preserved registers/return state across 306 finite circular lists. Caller-saved R1 through R3, dangling/nonterminating states and game integration are outside the result. The guarded source is recorded NonMatching with zero exact credit. Its measured ledger window overlaps ListNode insertion and excludes the unrecorded root-check duration; the reduced scratch build is a repeat.

### Identifier, audio and factory stall packets

Root verifies all 380 identifier-family and 1,600 factory frozen paths, plus the audio evidence hashes. Three identifier roots share seven valid forms and one failed syntax compile, fifteen physical ARMCC invocations including companions; their 10.544493-minute windows overlap. Audio disable stops early after two forms at two register-byte differences; its 0.834120-minute first-stall window includes both forms and excludes later packaging. Factory lookup stops after six structures, twelve compiler calls and one repeated closure link; measured 0.075913 minutes excludes untimed preparation/reasoning and the first failed helper link. Source stays scratch-only, ranks and boundaries stay unchanged, and no functional claim is made. Three self-contained packets cover five abandoned outcomes.

### Independent whole string virtual-table ownership

Separate retail constructors/consumers and an ordinary ARMCC ABI-shape probe establish three 20-byte tables with address points eight bytes after their bases: BufferedSafeStringBase<char> at 0x003DA508, FixedSafeString<64> at 0x003DA280, and StringTmp<64> at 0x003D7AF0. Retail 0x0027B39C directly installs the Buffered/Fixed64 address points and dispatches slot +8; independent 0x0016ABFC and the capacity-64 variadic constructor install the StringTmp64 address point. Neighbor table/header observations delimit all endpoints. Intermediate unused table loads are not claimed as installations. Each retail table has two zero destructor-position entries; no callable destructor identity is invented.

Root verifies all 13 frozen ownership artifacts and applies only the data patch. Six anonymous U rows are corrected and two successor-header rows added. All 124 affected bytes remain contiguous, all data ranks stay U, and all 18,055 function rows are unchanged. This separate hard-rule2 boundary commit precedes any dot source intake and adds no exact code/data credit. Evidence and complete independent retail intervals are in build/phase_two_string_table_ownership/proposal.md and its hashed artifacts.

### Frozen metadata, audio, random, calendar and sequence source checkpoint

The next 22 source-frozen roots propose 1,588 unchanged mapped bytes. Root verifies 619 FootPrint, 232 area-index, 29/42 random, 42/111 calendar and 153 ProductSequence frozen files, plus three audio evidence groups. FootPrint's four-byte one-pointer record follows an independent allocation/store and scene-object caller chain. Existing placement prefixes are composed without replacing accepted readers; area-index's unchanged function is rebased before Arg4 because its old adjacent namespace context has changed. Random unsigned state words and calendar signed scalar wrappers/byte weekday result follow independent constructors and consumers. Calendar constructor stays in its own translation unit to preserve the observed out-of-line weekday call; one failed scratch header lookup is recorded separately from its two structures.

Audio dispatch preserves six anonymous virtual slots solely by their observed offsets. Its unused compiler type information is not claimed as retail data or a complete concrete class. Keeper/resource/parameter accessors use independently established private field prefixes, preserving separate translation-unit call boundaries. ProductSequence callbacks use existing anonymous nerve/string addresses; one existing integer field becomes public without changing layout or vtable. Retail initializer/store and caller evidence establish that field before compilation. No new data identity, function boundary, pool, compiler flags or helper address is introduced. Canonical acceptance and prior-root preservation are pending after committed-source building.

### Identify quaternion unit BSS from independent initialization

Quatf::unit occupies0x00430500..0x00430510. Retail initializer0x00381350 independently writes float(0,0,0,1); KeyPose C1 and0x00136A88 copy all four values, and existing getQuat returns that address. Lower double producer ends at0x00430500 and upper vector producer begins0x00430510. The change adds only one named U db row, with no function changes or data credit. BSS is not file-backed; startup execution remains unverified. All ten frozen artifacts verify. Independent complete disassembly, producer/consumer/neighbor identities and target fingerprint are in build/phase_two_key_pose_product_data_ownership/proposal.md. This separate hard-rule2 commit adds zero credit.

### Identify complete ProductSequence virtual table

ProductSequence C1 installs address point0x003CB108; its native table is56 bytes. Next constructor0x0016428C installs address point0x003CB140, bounding the anonymous successor header at0x003CB138. The repair names whole table0x003CB100..0x003CB138, clips the preceding anonymous row and keeps the next header anonymous. All68 affected bytes remain contiguous and all ranks stayU. No existing slot/function identity changes or exact credit follow. All ten frozen artifacts verify. Independent complete disassembly, producer/consumer/neighbor identities and target fingerprint are in build/phase_two_key_pose_product_data_ownership/proposal.md. This separate hard-rule2 commit adds zero credit.

### Independent LiveActorGroup table ownership

Retail constructor pool0x00277DDC installs address point0x003D6444. The independent neighbor constructor0x001CAB74 installs0x003D6450, bounding the Group table at0x003D643C..0x003D6448,12 bytes with its sole slot pointing to already accepted registerActor0x001CAB14. The native canonical table also independently occupies12 bytes. Root verifies six frozen files and preserves all48 bytes of the old anonymous union as preceding data, the named U table and anonymous successor header. All function rows and ranks stay unchanged. This separate boundary repair adds zero credit; the blocked Group constructor remains dot-owned and nonexact. Evidence is in build/phase_two_actor_group/live_actor_group_table_identity/report.md.

### Matrix33 inverse structural stall

Root verifies all39 frozen artifacts. Three source structures diverge in size/scheduling from the260-byte original; the aggregate form also retains36 bytes of constant data. No independently established helper address supports a split. Park the source-only proposal and commit its315-line packet for the dot, with zero exact or functional credit. The16.310985-minute shared window includes research and interleaved work; recorded compiler-driver time is1.532115 seconds. No source, rank, interval or tool changes follow.

### Calendar Time constructor physical section identity

ARMCC791 and894 independently define TimeC1(size28) and TimeC2(size0) at offsetzero in the same i.TimeC1 section. The unchanged original0x001080C4..0x001080E0 C2 row therefore selects the actual C1 section explicitly, as SystemKitC2 does. Root verifies all101 frozen artifacts and the two symbol inventories before this classification-only commit. Addresses, pool, symbol, rank and type stay unchanged; no match is credited. Evidence: build/phase_two_calendar_time_constructor/final_evidence.json and complete_linked_disassembly.txt.

### Native model root and owner-local joint rotation data

Root reviews the two-file checkpoint and verifies all1132 hash entries/1128 paths. The explicit scene executable mode fingerprints the entire original EU executable before reading its4096-byte rotation table; no table bytes enter committed source. Independent original constructors and the calcAnim/fall-through adapter chain establish root matrix/scale/flag updates, with constructed actor/controller/cache/camera inputs stated. Actual fresh cache flags7E1 replace the earlier controlledFE1 state, and the corrected22432-word palette regression passes. New1276 cases compare171028 words in each debug/O3 build; all16 scene palettes agree. Unsupported domains stay explicit. Source remains native runtime only, with zero matching or gameplay credit.

### Frozen joint, actor, Rail, audio and calendar source intake

The28-root proposal covers1948 unchanged mapped bytes. Root verifies all frozen actor, lifecycle, Byaml, audio and Time artifacts before intake. The joint wrapper uses a source-local two-register aggregate bridge to preserve call evaluation; it must disappear completely under the canonical closure gate and receives no original address. Independent import paths establish its model/matrix pointer ABI. Rail adds one nonvirtual table getter without changing layout, based on the independent creation/index writers. Scalar and point-vector readers compose into the existing Placement prefix without replacing accepted bodies. Audio reset uses a new typed proposal grounded after its raw-word diagnostic equality; only the forthcoming canonical build can validate that source. SafeString temporary header code/data is not enrolled. TimeC2 selects the independently recorded C1 section in the preceding separate metadata commit. No function boundaries, pools, compiler flags or existing accepted ranks change. Intake setup encoding/context failures occurred before compiler invocation and count as no source-form attempts.

### Four-attempt PowerUp metadata and RailRider constructor packets

Root verifies443 scene-metadata and111 RailRider frozen artifacts. PowerUpItemNum stops after four structures at an8-byte preparation-order miss; its3.467799-minute window runs first compiler start to cap. RailRider C1 stops after four forms, best108 versus116 bytes. Its5.999074-minute recorded start/end window overlaps the three helper proposals;2.887047 seconds is their recorded driver total, not the full wall window. Commit the255/279-line packets without source, rank or boundary changes and without a NonMatching claim. The dot owns both.

### Canonical28-root acceptance and preservation

Every frozen proposal passes tools/check.py on committed project ARMCC output. The joint aggregate bridge is completely eliminated by the provenance-checked closure gate; no fabricated helper address is used. The newly typed audio reset passes its first canonical compile/check, independently of the earlier raw-word object. All569 prior accepted functions/all587 actual definitions pass again. Total597O functions cover32056 complete mapped bytes including pools. Frozen root results: build/joint_rail_calendar_checkpoint_candidates_canonical_acceptance.json and joint_rail_calendar_checkpoint_prior_recheck.json. Clean compact main links/exports42 Game/115 al/1 SDK objects. Independent empty-build linked-map checking remains pending before push.

### Independent Sequence base-constructor import identity

Root verifies seven independent review artifacts and the three submitted hashes. Accepted Sequence method slots, NerveExecutor base construction, name-field stores and the named ProductSequence C1 direct call establish the Sequence constructor receiver/name contract. Name only the existing U row0x00318A84..0x00318C1C with the canonical ARMCC C1 import. Its actual extent is408 bytes including20 pool bytes. The submitted prose said476, which would incorrectly consume two destructor rows; the frozen proposal boundaries/hash were already correct. C1 spelling is an inferred compiler-compatible import alias, not a recovered retail symbol or proof excluding C2. No C2 alias, table split, implementation or credit follows. Evidence: build/phase_two_sequence_constructor_identity_review/report.md. Further string-source intake remains held for the separate FixedSafeString layer audit.

### Independent copy, quaternion-provider and StageTimer identities

Name only three existing U rows, preserving all boundaries/pools/types/ranks. Clean ARMCC791 c_4.l member rt_memcpy_w_mpc.o exports __aeabi_memcpy4 at0 and its complete116-byte text equals the retail aligned-copy routine; named memcpy and generic alignment dispatch independently confirm its argument contract. The compiler member is read-only identity evidence and never a candidate link input. KeyPose fields/calls and provider Euler lookup/degree conversion/default-unit paths independently support the existing tryGetQuat declaration on0x00250D30..0x00250DC4; its implementation is unprobed and uncredited. Two separate StageTimer literal references point to the whole existing12-byte dc row0x003AE120..0x003AE12C, including its retained FF tail. Root verifies98 calendar/226 KeyPose/443 metadata artifacts. These names precede source intake and grant zero exact credit.

### Frozen scene, calendar, list and audio source intake

The21-root batch proposes1200 unchanged mapped bytes. Root verifies all RailRider/metadata/KeyPose/LiveActorKit/calendar/list/array/audio snapshots before intake. Date value members and13-byte assignment follow independent constructors/setter; the clean aligned-copy runtime identity is committed separately and no compiler-library bytes become candidate link inputs. Native list declarations preserve field layout and guarded clear body; pointer-array implementation is a separate12-byte class, with no standalone template-layout change. Include the already-frozen88-byte erase companion in the same source intake to preserve the setter32 without a second source rebuild. Its791 form matches, while894 emits an extra NOP and92-byte extent; no original boundary changes. LiveActorKit is unchanged CP932 source, and its earlier decoded-literal correction proposal was a byte-identical no-op. Audio private views keep anonymous interfaces/byte meanings bounded by independent constructors and callers. No SafeString source/header changes are included while the string-owner audit runs. Only canonical committed-source checking can grant credit.

### Float-component reader family cap

Root verifies295 frozen artifacts and commits one540-line packet for three roots. Four source forms per root preserve152/192/160-byte extents but best differences remain18/12/9 bytes, so all three stop under the owner cap. Eight physical ARMCC invocations include companion units; no894 search, source intake, data naming or NonMatching claim follows. The shared6.342985-minute wall windows overlap analysis/interleaving, not additive labor. Existing X/Y/Z data identities remain read-only proposals.

### Correct fixed-string storage-base ownership

The earlier audit misidentified the intermediate table0x003DA280..0x003DA294 as the char-only FixedSafeString64 wrapper. It missed the copy routine's final store at0x0027B4C4, already present in the frozen evidence. Independent Sequence/member/default constructors also install final wrapper address point0x003D9D0C after storage-base0x003DA288. One ordinary reconstructed class-shape probe corroborates distinct generic storage and char wrapper tables; it does not prove original debug spelling. Root verifies all24 new artifacts and re-reads the previously verified13-artifact evidence, then changes only the U data symbol at0x003DA280 to inferred FixedSafeStringBase<char,64>. Boundaries/ranks/function rows are unchanged. Buffered and StringTmp identities remain supported. No data or function credit follows. Evidence: build/phase_two_string_wrapper_identity_review/report.md, including exact fetched dot1069d604 reports and actual stores versus dead loads.

### Complete char-wrapper string table boundaries

Separate actual final installations establish FixedSafeString64 at0x003D9D04..0x003D9D18/AP0x003D9D0C and FixedSafeString96 at0x003D9D18..0x003D9D2C/AP0x003D9D20. Independent capacity512 predecessor and GraphicsContext successor bound their headers. This separate hard-rule2 repair preserves the exact60-byte union0x003D9CF8..0x003D9D34 as anonymous predecessor, two complete20-byte U tables and anonymous successor header. All18055 function rows and all ranks stay unchanged. Null destructor-position words remain observed data, with no callable destructor identity or matching credit. The production header still collapses these layers; further SafeString source intake needs header adoption and canonical preservation checks.

### Native face-group transfer and independent fallback repair

Read-only original draw191098 and packers254D9C/254DD4 support binary32 FIFO rows in reverse component order, cursor advance including one unwritten trailing word, and secondary palette selection only when joint flag200 is set and the full face mode equals2. This corrects earlier prose; both previously validated CPU matrices remain unchanged. Packet/header controls and46 distinguishing selection controls are frozen with owner faces. Root verified1228 hash entries over1224 paths in data/runtime/romfs/native_face_group_transfer_corrected_frozen/command_result_manifest.json, SHA c6ebfc2031d3a4a850258a8e330f6cdcbc985d0e587d4d9b0a9e1921c7f3318f. The393-case native corpus agrees on9003 words in each sanitized debug/O3 build, with369 original draw cases/658 selected matrices,52 real scene faces/780 words and four rejecting damaged fixtures. All15 recorded commands pass; earlier constructor/root, palette, scene, mesh, raw/color and3422-bit math regressions remain preserved.

Root review found a reachable case absent from the initial corpus: without executable/table input, unit-scale update retained native zero matrix8C while the original constructor had copied serialized matrix84. Preserve-before-update is the constructor invariant here. The no-table path now makes that same raw copy. All16 independently executed constructor/update/draw comparisons pass, including a distinct12-component matrix fixture. The prior16 failures, source and executable hashes remain immutable under native_root_transfer_fallback_failure_frozen/manifest.json, SHA83006874a8eeb8f072f0c9fce1409986450863ba843f83f1bd7b36da1a81f497. This adds no exact decompilation bytes, table-free skeletal rotation, full initialization, shader execution or gameplay claim.

### Owner decision: shared local and dot hard-function routing

The owner replied, verbatim, "yes let's do that" to Claude's proposal to reverse the exclusive hard-function routing from2677bdb. This supersedes the earlier owner request to send the hardest work only to the dot. Local lanes may now take functions of0x200 bytes or more, blocked entries and packets without waiting for the dot. Check open dot branches and reports before selection to avoid simultaneous work. After four unsuccessful attempts in a working pass, preserve the history, packet the function and do other work before returning it to the queue. The dot may also take it and keeps its existing target order. Claude's throughput rationale and proposed wording are supervisory additions, not owner quotes. No compiler, acceptance, data integrity or concurrency rule changes; the current memory-driven limit remains six total lanes.

### Minimal fixed-string storage and wrapper source layers from dot/effect-action-update

After independent table identity and boundary repairs e0e11b3/e388cdf, the clean SafeString header now separates generic FixedSafeStringBase<Character,Capacity> storage from the empty char-only FixedSafeString<Capacity> wrapper. Original copy27B39C and independent Sequence318A84 actually install Buffered, storage-base64 and final wrapper64 address points in that order. The frozen independent class probe emits the corresponding distinct20-byte tables and preserves76-byte fixed64/temporary64 and108-byte fixed96 layouts. These names are inferred reconstruction identities, not recovered debug symbols.

This intake adopts only the class layering corroborated by dot/effect-action-update1069d604. Existing termination bodies and zero initialization remain textually unchanged. The branch's new bounded length and deep-copy implementation are not adopted by this minimal repair. Their behavior remains unaccepted proposals. Source/header preservation and canonical checks are next; this change grants no new exact bytes or string-table-content credit.

### Independent vector, random-holder and scale-key data identities

The frozen9-artifact effect prerequisite review build/phase_two_effect_import_identity/report.md, SHA66e91a0918c8fb17bc34aeda87c81f406eb42a2ba20c3f1e8e26133453f84228, supports whole two-pointer holder3E26DC..3E26E4: independent GlobalRandom producer104A00/104A10 writes disposer+4 and instance+0; RandomState2E4400 consumes the instance. Name only the existing whole8-byte U row. The original reporter2200FC can write a context timestamp, so historical non-mutating dot replay remains bounded.

Root separately reread the actual vector initializer383870..383E00. VLDR3839A4 loads s0=0 from383D68; VLDR383AC8 loads s1=1 from383D70. No relevant VFP alias is overwritten before the stores. Literal383D8C selects4305E0 and stores at383B30/34 produce xyz=(0,0,1);383D90 selects4305EC and383B3C/40/44 produce xyz=(1,1,1). Add two separate nonoverlapping12-byte U BSS rows for clean Vector3f::ez and ones, bounded by the selected neighbor and existing zero4305F8. Names use the independently reconstructed current Vector3 API shape; startup and data bytes remain unaccepted.

Scale keys3A2E14/1C/24 are whole existing8-byte U rows holding scale_x/y/z, independently referenced by direct reader240FA8 and wrapper24EC80 at their frozen literal pools. Assign address-only names, preserving every extent and rank. All18055 function rows remain byte-for-byte unchanged. BSS scaffold zeros and file-backed identities grant zero exact credit.

### Canonical section classifications and address-only effect imports

Root independently inspected the unchanged791/894 ELF outputs in the frozen AppearStep member-callback and Tree188 packages. Explicit AppearStep clone specialization39B2FC has strong i.<clone> code,60 symbol bytes plus4 pool bytes, complete SHA99e32b5faf35ef870d65e3a26296df4d135bd59a71d3c00a23509281a4e1cfab. Typeft incorrectly selects t.<clone>; set Typef on the unchanged complete64-byte U row. TreeNodeC2 at28CC00 is a zero-sized alias at offset0 of its24-byte C1 physical i. section under both compilers. Set only its explicit SectionName=i._ZN4sead8TreeNodeC1Ev. These repairs change neither target extents nor ranks. Scratch objects remain ineligible for acceptance.

The independent effect import review supports neutral function names on existing complete U intervals2200FC/200,291470/32,2E54AC/1240 and2E4E18/1572. The reporter context/message ABI includes an observed timestamp side effect; the copy loads/stores twelve VFP words with a self-copy guard. Separate creation/recreation callers and constructor storage ground their argument roles. Original API spellings and full allocation/report behavior remain unknown. No function pool/boundary or O row changes, no new match.

### Caller-owned shader context and separate mesh draw-flags intake

Original constructor29FB78 and post-material consumer191CE4 independently establish two six-word boolean/integer packets and selective replacement mask186. Mesh2C low bits0/1 map to context7/8; any full face mode2 selects bit1, otherwise bit2. The signed geometry selector controls whether the second cached block is copied. All other caller boolean bits and integer words are preserved. Root compared every634 original packet/advance pair with the native reference corpus and verified1302 current/frozen hashes over1298 paths, manifest SHA99423b448a1cec5017405a05c3ce99e27b0af50ff6840883dbc22a1bacf85c60. Both native builds pass6066 packet words and11 caller/status controls; all16 recorded regression command windows pass.

Existing ModelMesh.flags is identity at0, so the raw draw flags at2C receive a separate field and bounds check. All137 owner fields/transitions agree. Twelve short extents reject, two valid extents succeed and eight damaged fixtures reject. The interface requires supplied post-material caller packets, selector and callback state. Missing inputs, incompatible packet headers and the untraced callback emit no commands. Constructor defaults do not substitute for actual material state. No active-program, vertex arithmetic, renderer, gameplay or exact-decompilation credit follows. Earlier failing fallback and corrected transfer checkpoints remain immutable.

### Four frozen stalls remain available to coordinated lanes

Commit independent packets for Tree push-front2F2AD4 (three source contexts, early ABI/inlining stall), ActorInitInfo scale24EC80 (four forms), NoteObjGenerator init171334 (four) and ExecuteTableHolderUpdate init1E37D8 (four). None has a functional NonMatching or exact claim, and no failed source body enters production. Ledger times are actual shared windows9.0765/2.7615/2.7838/6.7983minutes, with overlaps and untimed preparation preserved in each packet. The large library's first three effective header snapshots were not captured before compilation; its packet states that uncertainty. Owner1714fe8 allows coordinated return after other work, retaining this history. Both large discovery lanes move to different targets.

### Existing ExecuteOrder provider identities

Separate neutral names fn_00242CC4 and fn_00242CCC identify complete existing8/72-byte U rows without boundary, pool, type or rank changes. The first reads order+4 then calls accepted isEqualString292308; the second searches16-byte ExecuteOrder records with a signed count and uses that same name field/equality call. The production ExecuteOrder declaration and independent holder constructors corroborate its fields. The frozen first paired source has exactly two executable sections and no retained helper. Its80 diagnostic bytes still require committed-source canonical checks. Evidence: build/large_library_matching/report.md and providers/diagnostics.json.

### Coordinated33-root actor/callback/tree/audio/Byaml source intake

Root reverified3701 frozen hash claims across18 packages, all current source baselines, exact patch hashes and target intervals before applying32 paired-exact proposals plus unchanged ActorPoseKeeperBase::getFront334F04 as a first canonical12-byte probe. The latter independently matches original LDR/BX plus4305E0 reference and the newly established ez producer. Candidate2000bytes add no credit before the project checker. Every original function boundary/pool/type and every618 O row stays unchanged; only new neutral target names and provisional M ranks enter this source commit.

Two independently frozen Placement additions are composed function-only, preserving56 existing complete source bodies and every unrelated byte. AppearStep clone specialization, Aquarium literal reference, TrickHint On and Wipe callbacks are CPP-only changes. Resource/archive imports retain known receiver/variadic contracts; no formatter or selector implementation is inferred. Audio prefix/slot views remain translation-unit private, without concrete class/lifetime or host aliasing claims. TreeNode declares only its observed four-pointer prefix; future shared headers must consolidate it. Historical SafeString/ListImpl/DateUtil drift stays explicit in build/current_frozen_source_intake_review/header_drift.json. The minimal class-layer repair, all33 current-source checks and full prior-root preservation are required before acceptance.

### Three bounded stalled packets, 2026-10-01

Root verified2524 BeatBlockHolder artifact hashes,4022 fixed-point artifact hashes,50 Kit artifacts and203 frozen packet/history claims before committing these proposals. KitendInit stops after one form due to an unresolved kit+30 operation and callback reload contract. It is an early structural stall, not a four-form cap. BeatBlockHolder and the neutral fixed-point processor each exhaust four forms; ten physical compiles each include declaration repair or two preserved syntax failures. Their packets preserve complete original assembly, best ordinary C++, pools, headers, compiler flags, observed differences and independent layout evidence. No source, rank, table ownership or exact/functional credit changes.

Ledger windows are0.009832 minutes for the Kit paired driver,3.287849 minutes for BeatBlockHolder first-script through last diagnostic, and4.352431 minutes for the fixed-point pass. Preparation/packaging are excluded and machine contention is not isolated. Source packet snapshots remain byte-identical to the frozen handoffs. Future local or dot passes need other work and a new hypothesis before requeueing.

### Executor packet size notation, 2026-10-01

The earlier001E37D8 packet wrote Update holder size44 without marking the hexadecimal unit. Seventeen cleared words establish68 bytes (0x44), and its C++ declaration was already correct. Only the explanatory sentence is corrected. The original frozen build packet/history remain unchanged; no layout, code, rank or acceptance changes.

### Executor-list preparation imports, 2026-10-01

Independent retail direct branches in1E36EC..1E37D8 and1E2044..1E2130 identify preparation callees1E5A68 and1E7328. The whole existing unnamed U/f rows1E5A68..1E5AA0 and1E7328..1E7410 receive neutral address names fn_001E5A68 and fn_001E7328. Their complete original bodies and the independently accepted holder layouts are frozen under build/library_table_list_matching/. No public method name, implementation, boundary, rank or exact credit changes. These names permit ordinary C++ imports in the separately reviewed executor-list proposals.

### BombHei quaternion-block stall, 2026-10-01

Root verifies169 frozen artifact hashes and commits the413-line control packet. Factory/creator/constructor and independent LiveActor control slot establish the method on the existing anonymous U row; no map name or boundary is changed. Four ordinary quaternion source forms each produce704 bytes under791/894, with best55 differences isolated to148 bytes. The other556 bytes include the equal96-byte pool. Eight successful physical compiles emit no retained arithmetic helper. No canonical or functional verification occurred. The shared first-to-last diagnostic window is2.959260 minutes and excludes preparation/packaging. Do other work before requeueing with a new expression hypothesis.

### FallMapParts whole import names, 2026-10-01

Original nerve callback branches identify five unchanged whole unnamed callee rows:268DF8..268E3C,26A9FC..26AA60,27063C..270710,279E5C..279E8C and279E8C..279ED4. Five whole existing NUL-terminated literal rows at3BD9DC/8,3BD9E4/8,3BD9F8/12,3BDA04/8 and3BDA0C/4 have zero padding within their current extents and appear in the complete original callback pools. They receive the frozen source proposal's neutral fn_/dat_ names, preserving every boundary, type and rank. Three already named imports remain verbatim. No method identity, imported implementation/data bytes or exact credit is added. Root verifies133 frozen callback hashes, five supplement records and287 LayoutKit artifacts before source intake. Evidence: build/fall_map_parts_root_identity_review.json.

### Owner re-enables measured GPT-6 Pro relay, 2026-10-01

Owner, as relayed by Claude: "yes yes and yes" to restarting the relay and tests; "let's do a couple tests, really SEE how good it can be, then scale up, i think we still have 200 messages per week". This supersedes the earlier paused-relay decision. The verified private GitHub connector lets Pro fetch pushed packets and committed source/header references from origin. Claude sends paths and saves answers as uncommitted project/pro_responses/<address>.md. Root prioritizes source review, committed-input project build and checker grading, then appends matched / closer N bytes / no help with the observed baseline/differences and commits the result. No source or match is accepted merely because Pro proposes it.

Initial test roots00156758,001F9F48 and0024EC80 stay reserved against local/dot matching until their responses are graded. BombHei0030E678 is local and must be included in the next audited push. Start small and measure before scaling. About200 messages/week is the owner's reported availability, not a newly imposed spend cap or cost ledger. Acceptance, four-unsuccessful-attempt and temporary six-lane rules are unchanged.

### Five dot proposal identities and whole imports, 2026-10-01

Independent retail callers ground Byaml data/key reader33753C, graph constructor181558, graph-output constructor1B4658 and direct ShapeModelNo reader252EC4. The BlockDragon factory/constructor and callback body ground startAppear1931B4. Existing whole U function rows receive their proposed C++ symbols. PlayerActionGraphBuildOutputs is a descriptive reconstruction name, not an original spelling. Its36-byte (0x24) output allocation is independent of the40-byte constructor code; node08 stays unwritten by construction. Root verifies13 direct retail calls and the entire frozen intake manifest.

Whole existing data rows3A2DBC..3A2DCC,3F1E50..3F1E54 and3F1E54..3F1E58 receive neutral dat_ names. The first is the complete ShapeModelNo literal; the latter two are original callback address imports from1931E0/E4. No Nerve table contents, initializer, data bytes, boundary, type, rank or exact credit is changed. Evidence: build/current_dot_source_intake_review_root_hash_review.json and the frozen review. Other component data/source remain held.

### Thirteen source proposals for canonical intake, 2026-10-01

The frozen exact-only dot review adds five proposed bodies/372bytes, preserving prior Graph/Byaml/placement source bodies. The packed Byaml tag getter now extracts the high byte of the existing unsigned word; no other current consumer used this accessor. Graph/output layouts come from independent retail callers and constructors. Callback-only BlockDragon code does not allocate or rely on its incomplete trailing extent. All guarded components and broader matrix/string/graphics/builder changes remain held. Dot credit and historical forms are preserved in the queue; untimed dot preparation remains a lower-bound gap, distinct from verification windows.

Local intake adds four Fall callbacks484bytes, two executor-list bodies472bytes and unchanged LayoutKit creation56bytes. Root also proposes the existing24-byte Byaml string getter as its first canonical diagnostic. Configured NON_MATCHING already enabled that body; removing its isolated guard does not rewrite the function. Eight local and five dot proposals total1408bytes. These are M enrollment only. Rebuild committed source, preserve all651 earlier roots, then run the unchanged checker for exact credit. The thirteen queue attempts each include exactly one pending root canonical check.

### Byaml string getter source-scheduling cap, 2026-10-01

The first committed-source canonical probe and three ordinary scratch variants exhaust this pass. Baseline and the first two variants emit24 bytes with15 differing bytes under both compilers; the last emits20 bytes and fails the extent check. All six additional compiles preserve accepted findStringIndex raw code/relocations. Root verifies79 frozen files before committing the216-line packet. No behavior equality is claimed. Ledger time combines the measured1.275743900-minute scratch wall with0.291332291-second canonical driver; full canonical wall/preparation remain untimed. Source stays unchanged after its first canonical probe; no new source is adopted from the failed variants.

### Collider invalidation preservation blocker, 2026-10-01

Root verifies all1633 frozen handoff files and the332-line packet. Four unsuccessful ordinary C++ forms leave8 differing bytes in the complete88-byte interval under791/894. Eight target compiles and four preservation compiles all succeed. The proposed aggregate-result header changes4 bytes in previously accepted isCollidedGround262BA4..262BC8, so neither source nor header enters production. The independently grounded512-byte storage, three0x8C result strides, sentinel-99999.0f and output-position provider are retained as evidence, without exact or functional credit.

The ledger records6.845910 minutes for the measured preparation and all diagnostic window, including preservation. The narrower target-only window is3.878167 minutes; initial selection and final packaging are excluded. The first sentinel transcription error is disclosed in the packet. Requeue only after other work with a new scheduling hypothesis that also preserves accepted callers. Evidence: build/library_collider_invalidation_matching/handoff_manifest.json.

### Eight-lane restoration and663-root audited push, 2026-10-01

Claude, supervising under the owner's monitoring authority, reports normal memory pressure and lifts the temporary six-lane limit. Restore the BRIEF eight-lane cap with at least half on small/medium matching. Five such lanes, one large actor lane, one runtime lane and root intake are independently bounded; completed turns stop. No collaboration tool exposes thread archival, so stopping a completed turn is not reported as closing its retained history.

Root confirms the clean build links/exports and every362 input/173 canonical object hash remains identical. Full normal linked-map checking preserves663/663 accepted roots with zero regressions, and split preserves all663 complete intervals. Whole-image equality remains false. Claude separately reports the MacBook audit of pushed e87d556 passed all651 prior matches; this is relayed audit evidence, distinct from root's local independent empty-build run. Push the owner relay policy and all42 packets, including BombHei0030E678, now.

### FireBall independent import and whole literal identities, 2026-10-01

Root verifies all248 frozen files and all28 unchanged source/header/config inputs. Independent original sender2796E0..2796FC loads the recipient actor from sensor28, passes decimal message50 and dispatches actor virtual slot34. The retained SensorMsg declaration corroborates sendMsg50's signature. Original278478..2784BC reads actor collider20 and ORs nonnegative distancesC0/14C/1D8, independently agreeing with accepted ground predicate262BA4. These whole existing U rows receive sendMsg50 and isCollided names; no implementation, boundary, pool, type or rank changes.

Whole existing dc row3C0824..3C082C is the complete NUL-terminated Shot string with zero padding and appears in the original callback pool. Name it dat_003C0824 without importing its bytes. The separately established fn_0027063c action starter keeps its current neutral identity; the source uses a narrow ABI declaration with its independently observed actor/string arguments and boolean result. Root confirms the consumer/provider bodies rather than naming it solely to satisfy FireBall's call. All three candidates still require committed-source checks. The24-byte STB_LOCAL initializer remains U because current entry/discovery tooling cannot accept it; its diagnostic equality grants no credit.

### FireBall source-only canonical enrollment, 2026-10-01

Apply the frozen collision-branch return, whole Shot literal import and existing helper inlining, adapted to current fn_0027063c identity. No header, layout, flags, data definition or assembly changes. AttackSensor164 and init104 keep their source bodies unchanged; the callback144 uses the corrected return. Enroll only those three roots as M. The standalone source helper must fully disappear and all three previously accepted definitions in this translation unit must survive canonical checking. Other objects must remain unchanged.

The queue retains two paired source snapshots/four successful physical compiles, prior unresolved-import and local-entry failures, and the measured8.207247-minute shared source/identity window. Per-root source forms are one for attack/init and two for the callback, plus the pending canonical attempt. Packaging/root review remain untimed. No exact credit before the project checker.

### Conditional shader instance evidence, 2026-10-01

Root verifies440 hashes/373 paths and independently decodes the four binder calls and keeper/context stores. Reachable original resource and named-context chains select the same FastShader instance only with explicit optional Shader archive input. Record this bounded interface and corrections in native_mesh_evidence.md. Do not replace missing material-modified state with constructor defaults or treat cached root pointers as DVLE pointers. Keep proposed native changes in scratch until original/native differential checks pass.

### Actor scale wrapper neutral identity, 2026-10-01

Name only the existing whole U/f172-byte row24EC80..24ED2C as fn_0024EC80. Original calls preserve ActorInitInfo's independently established first placement-pointer member and reach accepted isValid276AA0/FloatByKey278C30. Its complete12-byte pool refers to the previously named whole scale_x/y/z rows. No class/public method spelling, boundary, pool, type or rank changes. This identity supports the owner-authorized Pro test without changing the oracle.

### Pro actor scale primary source intake, 2026-10-01

Both stated copy forms pass paired791/894 complete172-byte diagnostics and preserve direct reader156 plus all56 emitted accepted Placement definitions. Root verifies154 frozen files and13 unchanged current inputs. Isolated local branch project builds of committed source41c55a7 and77b8a15 pass both wrapper/direct checker calls; those branch O results add no main credit.

Choose the primary explicit memberwise Vector3 assignment and existing direct-reader delegation. This ordinary C++ copy operator preserves layout and semantics; its return-reference live range changes compiler allocation without flags or assembly. Commit the primary CPP/header and enroll only the wrapper as M. A clean main build and complete prior accepted-root canonical preservation gate are required before credit. The fallback setter stays a tested proposal.

### Pro copy primary fails shared preservation; use stated fallback

The complete primary main audit passes662/666 earlier roots and rejects four: setRotate/setVelocity retain copy-helper code, while PlayerProperty setUpVec/setFrontVec change complete extents. The direct reader and proposed wrapper themselves pass. No new wrapper credit is taken. This is why the full shared-header gate is required; paired Placement-only preservation did not cover the other callers.

Restore baseline implicit assignment and apply Pro's already tested void set(other) fallback with the same delegating wrapper. The copy helper is used only by this reader. Keep the wrapper M; clean rebuild, recheck the four rejected definitions so only the checker restores ranks, then repeat the full666-root gate. Source and oracle boundaries remain unchanged. Evidence: build/pro_scale_main_explicit_assignment_prior_recheck.json.

### First completed Pro grade: fixed-point semantics corrected, bytes still fail

Root verifies1661 artifacts and both isolated committed-source project builds/checker failures. The next form reduces complete raw differences485 to480 while widening the extent deficit to28; primary560/486 adds no raw improvement. Original operands and bounded execution establish the packet's accumulator bug independently. Record the useful correction without a general NonMatching or exact claim. Main source/map remain unchanged. Two supplied forms plus two canonical project attempts are recorded; measured windows overlap and exclude preliminary reads/packaging. Release001F9F48's test reservation now that its response is graded, retaining other-work-before-requeue and overlap checks. Other two responses remain priority.

### Three-test Pro result, 2026-10-01

All three owner-approved responses are graded. One root is exact in main through its stated fallback: scale172, with all666 previous roots/all685 definitions preserved. Its primary matches the target but fails four other roots and is rejected. BeatBlockHolder improves140->31 paired scratch differences but canonical constructor/group ownership remains blocked. Fixed-point improves485->480 raw differences while widening its extent deficit; its accumulator correction is independently supported by bounded original execution. Neither partial result becomes main source or a functional NonMatching claim.

Measured exact hit rate is1/3 roots for this small test, with0/3 primary forms fully accepted on main and one stated fallback accepted. These are observed sample counts, not a prediction of future hit rate. Root records all supplied/conditional forms, environment failures, ownership rejections and actual overlapping timings. Release all three reservations; future local/dot work still requires other work before requeue and active-overlap checks. Claude/owner manage any scaling.

### Owner restructures the acceptance factory, 2026-10-01

Owner, as relayed by Claude: "yes please, can we do so? i dont mind restructuring and getting this right so we can work FAST" and "specifically on the bottleneck part to fix the factory". The approved changes are root as an acceptance gate and work selection by expected accepted bytes/hour. Root stops source assembly. One lane owns each batch header/TU family and submits an apply-clean frozen-base patch with final-source diagnostic evidence. Conflicting patches, unresolved composition and preservation failures go back to that lane. Independent ABI/data/table questions enter a separate evidence queue so they hold only dependent work. Root still commits inputs, builds, checks all earlier roots/definitions and alone accepts ranks/ledger/pushes. No oracle, clean-room or four-form rule changes. Runtime work is deliberately not time-boxed.

Pro's cited bottleneck analysis at e87d556 and approximately2000 accepted bytes/hour are relayed evidence, not newly reproduced timing measurements. Root independently compares canonical map snapshots as of20:03:27/21:03:27/23:03:27UTC:597/32056,618/33256 and667/37224. The prior three-hour gain is5168bytes,1722.667bytes/hour; prior two-hour gain3968bytes,1984bytes/hour. Source snapshots are f8a2eac, f9a7f46 and75ac2e8. The new per-lane report uses assignment-to-acceptance elapsed wall time, includes intake and separates carried-ready proposals from new work. Ledger timing windows overlap and cannot be summed into elapsed throughput. No after-speedup is claimed before observed canonical acceptance.

The latest Claude relay explicitly names a six-lane cap as unchanged. Apply six total lanes conservatively for this restructuring, superseding the earlier eight-lane operating restoration. Completed handoffs stop; retained collaboration histories are not claimed closed. The next Pro batch reserves001E37D8,0030E678,001D1DA0,0016A11C,0024F344,00268EB0,001EA220 and00252B1C. Grade every ranked form. Push75ac2e8 or later after clean linking so the owner/Claude can send the committed BeatBlockHolder31-byte follow-up.

### Supervisory relay correction: eight lanes and existing BombHei work

Claude corrects his preceding statement: the temporary six-lane memory steer was already lifted, so BRIEF rule12 permits eight total lanes. His bytes/hour selection steer replaces the earlier requirement that half of lanes stay small/medium. Restore eight and allocate by the owner-approved family yield score. The six-lane text in the preceding decision records the earlier relay as received; this correction supersedes that operating limit.

Frog pushed dot/bomb-hei-control before the Pro reservation reached it and reports49 differing bytes. Let its existing work continue, without starting a duplicate local pass. Root must verify that proposal and use its observed result as the baseline for every ranked Pro form, retaining the closer valid ordinary source without treating partial output as exact. The eight packets were sent to Pro at3de056e. Claude reports responses should arrive in15..30minutes; this is an estimate, not a received answer. No owner decision is newly inferred from the correction.

### Factory batch-one independent identity queue

Apply four lane-supplied clean evidence patches unchanged after independent provider/caller/literal review. Ten existing whole rows change only Symbol: three RailMoveMovement imports2694A4/27139C/273E28; compiler-routing FileLoader101BCC, SaveDataDirector1027D0, RailKeeper1BCA74 and Rail initializer1E95EC; two complete AppearStep literal rows3BCDA8/3BCDB0; and TrickHint prefix2278CC. Every boundary, pool, section, rank, type and SectionName remains unchanged. No original data bytes or new exact credit are committed.

The constructor names are compatible current compiler imports, not recovered original C1/C2 spellings. FileLoader's ignored incoming bool/overload set stays unresolved. TrickHint2278CC is a12-byte existing fallthrough entry, not a recovered complete implementation. Neutral Rail speed-call return types are ignored by the consumers and remain unresolved. Self-contained library/actor evidence is committed with the patches. Independent review verifies all ten row preimages, original EU hash, provider/caller intervals and two compiler import/alias records. Evidence: build/family_identity_independent_review_75ac2e8 and build/acceptance_factory_batch_one_identity_application.json. Tree/Garigari ownership is not covered by this approval.

## 2026-10-02: Garigari whole-function identities

Independent original factory, constructor, control-slot and member-pointer/operator evidence supports the three existing function rows at00315598,003155CC and0039CC6C. The separate review is build/garigari_identity_independent_review/review.md, SHA2566c3a4b318fc06ef41d0e027d8b153262aa8a6fe39914224bacd4f17db794b216. This commit changes only Symbol on those whole U/f rows and records the evidence. No interval, pool, rank, Type or table/data ownership is changed; the misleading containing table identity and generic clone remain unresolved. Canonical source acceptance is separate.

## 2026-10-02: FallMapParts and KeyPose whole-import identities

Independent original providers and separate actor callers ground fn_002794F8 (integer Arg0 lookup and boolean success), fn_0028058C (unadjusted tail branch to initMapPartsActor), and tryGetArg3 at0027AFF8 (integer output, Arg3 lookup and boolean success). Review build/fall_map_parts_key_pose_identity_review/review.md SHA256d79ab783902c82c7348158461c715c62ae81eb81190e43d309a95a12da9082e6 verifies the three unique whole-row preimages. These patches change only Symbol on existing U/f rows. No extent, rank, pool, Type or implementation changes. Source acceptance follows separately.

## 2026-10-02: First canonical factory acceptance

Root applies lane patches unchanged, verifies all40 final source hashes and complete map extents, and commits intended inputs before building. Independent manifest review confirms unique ownership and unchanged667 prior O identities. Clean build links; unchanged project checker passes all667 prior roots/all686actual canonical definitions and40/40candidates. Gate report SHA2566e065e8c5330b70902ba572c8c9393326f071ee9568e588f162c3ebb82be8a6c. Accepted bytes3276, carried1916/new1360. Factory wall interval2026-10-01T23:03:27+00:00..2026-10-02T00:11:52.904631+00:00 includes conversion, root intake and273.124seconds canonical gate. Aggregate2872.351bytes/hour compares with1722.667prior3h/1984prior2h. Newly investigated attribution alone1192.429bytes/hour, so no sustained throughput improvement is inferred from this mixed inventory window. Lane attribution uses the same measured protocol window and does not sum overlapping ledger minutes. Each ledger row conservatively includes the shared full canonical gate window plus its actual prior recorded windows. Candidate checks are not rerun to create duplicate attempts. Evidence: build/acceptance_factory_local_40_acceptance_summary.json.

## 2026-10-02: ActorFactory whole key-row identities

The original getCreator literal pool directly references 0x003BA0F8 and 0x003BA104, and both existing whole dc rows contain terminated key strings. Independent family review validates the unchanged conversion lookup contract. Name the two whole U rows with neutral address-derived identifiers dat_003BA0F8 and dat_003BA104. Only Symbol changes. No tail content, boundaries, ranks, table ownership or reconstructed data is accepted. Review: build/pro_factory_heap_independent_review/review.md, SHA256 a0304a1adc5b489f5fb505678c150ac043d570eeee7f83beaf50b2d303b7cf1f. Source/header acceptance is separate.

### 2026-10-02: Independent EffectObj and Stream import identities

Six whole anonymous EffectObj import rows and four Stream function rows receive compatible compiler labels. Every boundary, pool, type and U rank remains unchanged. Independent original providers, separate callers, the RAM-source table and two containing-stream constructors ground the contracts. Original public spelling and full class ownership remain unproved. Evidence: project/matching_evidence/effect_obj_initialization.md and project/identity_evidence/stream_source_scalar_8f822f5.md; independent reviews in build/effect_obj_import_independent_review_d097fc2 and build/stream_identity_independent_review_8f822f5. No source or exact credit belongs to this identity commit.

### 2026-10-02: Correct Pro ledger labels and timing attribution

Root corrects three guessed labels to their actual mapped compiler symbols: CourseList0016A11C, ExecuteTableHolderUpdate001E37D8 and ExecuteRequestKeeper00252B1C. Memory's earlier row omitted rank3; its three sequential measured normal-form windows total892.273568seconds. Keeper's earlier row arbitrarily halved the shared six-form window; its own three sequential normal-form windows total848.615763seconds. These values exclude earlier paired diagnostics and shared setup/packaging, and are not aggregate elapsed throughput. All forms were already graded; this corrects recording only, with zero rank/source/credit changes. Exact source reports remain in the frozen project-grading packages.

### 2026-10-02: ItemHolder table boundary and compatible imports from dot/item-holder-family

Independent predecessor/neighbor/following constructors bracket the20-byte table003C488C..003C48A0: predecessor secondary offset-to-top at003C4868 and final slot003C4874, separate28-byte neighbor address point003C4880, following LiveActor-derived primary003C48A8. Repartition the same contiguous216-byte U/dc union003C47D0..003C48A8, preserving every retail byte, function interval, pool, rank and type. Three independent5C creators register275828 as scene object10; separate accepted consumers ground ISceneObj dispatch. Complete evidence: project/identity_evidence/item_holder_table_8f822f5.md and frozen build/item_holder_evidence_peer_review_082c03e/review.md. NameRef is only a compatible private eight-byte string-prefix import alias. Original public spelling remains unproved. No retail getter identity exists; native getter/table contents and optional placement remain unaccepted. Source follows unchanged in a separate commit, with no credit until the full canonical gate.

### 2026-10-02: Automate repeated frozen-family handoff preparation

Root assigns one shared prepare_family_handoff.py to the lifecycle lane, within the owner-approved acceptance-factory restructuring. Combined EffectObj/LiveActor measured preparation874.951598seconds includes only1.001554seconds of physical compilation,0.1145%. Their post-equality windows include real independent import/overlap review, so they are not measured pure packaging or model overhead. Existing drivers share72.84%line sequences and identical ELF inventory routines. Generate repeated mechanical evidence from complete final files and a small explicit specification, using unchanged diagnostic/checker utilities. This single-source-of-truth approach reduces reassembly and inconsistent metadata. Snapshot actual effective dependency closure, preserve all histories/timing limits, and keep independent ABI/data/ownership reasoning manual. Scratch never setsO or creates canonical provenance. Root committed-source cleanbuild and full prior-definition acceptance remain unchanged. Reproduce sealed families and reject meaningful source/dependency/non-target/extent failures before adopting the script.

### 2026-10-02: Dispatch, file-device and NoteObj compatible identities

LiveActor dispatch adds only two neutral names on unchanged full U rows129074 and1D30C4. Independent complete providers and separate callers support entry ABI, not original public spelling;129074's12-byte entry falls through into129080 and no boundary repair or provider body is claimed. FileDevice adds two close compiler labels on existing whole U extents, grounded in independent constructors, actual tables and callback providers. Directory close2DF298 forwards its archive result or returns0for a missing archive; the earlier always-true prose is corrected by the owning lane with its old seal retained. NoteObj receiveMsg identity3125BC follows independently from accepted creatorallocation88, the existing NoteObj table and actor slot13 at3D3804; its full164-byte row/pool/type/rank stay unchanged. Dedicated header remains unchanged, all other aliases/data remain unaccepted. Evidence notes are separately committed before source. The current ready queue totals14roots/5356bytes, pending canonical checking.

## 2026-10-02: reusable family preparation within the acceptance factory

The unchanged460-line prepare_family_handoff.py proposal68693f8 replaces repeated mechanical scratch preparation for existing Game/al translation units. Root verifies1770sealed artifacts, then independently replays EffectObj176/156 and LiveActor192/40 under791/894 in fresh output directories. Full final hashes and all17/19 emitted function contracts equal the sealed references. Historical queues remain empty and add zero credit.

Fresh controls reject supplied-source drift, forbidden ownership, effective dependency drift, changed non-target definitions and200-versus192 whole-interval extent failure. A separate preprocessor failure records both physical compiler failures without byte checks. Protected oracle/build inputs remain unchanged. The tool copies complete files; it never assembles source or resolves ABI/data/ownership. Canonical project build/check and all prior roots/actual definitions remain root's gate. Initial scope excludes new translation units and foreign-root provenance; system headers omitted by unchanged dependency flags are not claimed fully hashed. The interface initially queues only paired-exact proposals; natural alternate-compiler discriminators can still use their explicit diagnostic family packages. Whole-project/data preservation stays separate.

Old EffectObj/LiveActor family windows total874.952seconds with1.002compile seconds. Their post-pair windows include useful manual evidence work, so they are not pure packaging/model time. Fresh mechanical reproductions take about two seconds each. This measures reusable work, without claiming a sustained acceptedbytes/hour improvement. Root evidence: build/family_handoff_tool_root_validation/root_summary.json.

## 2026-10-02: CourseList residual dot intake

Independent source-intake screening grounds the complete408-byte CourseList::init interval and finds the source-only cloud patch unchanged against current main. Its15 effective textual headers/preinclude files remain identical; unrelated ItemHolder additions do not enter its closure. Source-intake now exclusively owns CourseList.cpp atc80f46f to prepare the whole unchanged proposal with local paired diagnostics and actual definition contracts. No header, name, data or boundary change is required. WorldList700 remains guarded and unaccepted. Estimated1387.2 expected bytes/hour is a subjective0.85 chance over0.25hours, without measured throughput;408bytes are carried-ready inventory. Canonical build/check and all731 prior roots remain root's gate.

## 2026-10-02: next independent matching and runtime scopes

FileDevice's ten ArchiveFileDevice callbacks total736 whole bytes. The actual constructor/table, independent handle prefixes and five accepted wrappers ground the family. Library lane exclusively owns the existing FileDevice.cpp at88d6105 with unchanged shared headers/data. It must corroborate directory backend ABI separately and propose neutral whole-row names without table/inheritance/public-name recovery. Subjective score744.53expectedbytes/hour includes0.65canonical chance,0.75hours and80risk-weighted downstream bytes. Existing five740-byte bodies must preserve.

Runtime lane exclusively owns AssetReader.cpp atd64f5c7, exact complete source4260601f, for explicitly supplied complete command framing and a raw cached-copy descriptor. All18screening hashes verify; immutable original copy/wrapper/memcpy execute240controlled cases and compare13824words. Static draw-group copy/reset and actual CGFX mesh paths remain distinct. Missing supplied inputs stay unavailable, skipped copying establishes no values, and only proven complete fixed writes enter history. This shared prerequisite closes a CPU intake boundary without claiming scene/GPU submission. No automatic defaults, active shader arithmetic, vertices or rendering. Freeze the replay whitelist before implementation. Estimated one to two hours applies to this bounded interface only; runtime continues without a new timebox.

## Bubble virtual-table data interval, 2026-10-02

Independent read-only original-EU review establishes Bubble's complete table at 003D27DC..003D2874/152 and primary address point 003D27E4. Original registry 003B9A40 selects creator 0039818C, which allocates 0x94 and calls constructor 0030350C. The constructor stores primary and secondary points; independently identified TreeA/Bunbun neighbors corroborate both null prefixes and all table entries. No compiler candidate was used as evidence.

Replace only the two anonymous U/dc rows 003D274C..003D27E4 and 003D27E4..003D287C with predecessor tail 003D274C..003D27DC, named `_ZTV6Bubble` at 003D27DC..003D2874 and anonymous next header 003D2874..003D287C. Their 304-byte union is identical. Every function row, boundary, pool and accepted rank remains unchanged. Keep all new data U/dc, with zero exact credit and no copied table provider. The C++ ABI name uses the existing Bubble class spelling plus the original runtime/constructor/translation-unit identity; no stripped public symbol claim. Evidence: `project/evidence/bubble_vtable_boundary_review.md`. Constructor acceptance still requires the normal committed-source build/check and prior preservation.

## 2026-10-02: remove two measured mechanical preparation limits

Archive callbacks are independently evidenced but unnamed in their frozen map, while Draw's valid new include is absent from its old dependency closure. Both cannot use the current preparer without returning to hand-written packaging. Byaml lane exclusively owns prepare_family_handoff.py at556bc41/current828eb0bf to support explicit evidenced whole-row target names and additional hashed unchanged Git-base headers. Source/header proposals remain complete files; unowned header bytes must remain equal to frozen Git and current inputs. Names remain separate unapplied proposals for independent human acceptance. The tool must not infer ABI/ownership, rescue objects or change flags/checkers/provenance. New translation-unit bootstrap remains outside this bounded change. Root requires actual historical/new-input replays and meaningful refusal controls before intake.

## Current blocked inventory and Sky successor ownership, 2026-10-02

Removed 4 active blocked table rows whose whole intervals are now O. Historical packets, trials and ledger records remain; checker acceptance is the authority and this cleanup adds no credit.

Assign actor_lifecycle only existing lib/al/src/Npc/alSky.cpp at0fff25a/source6146ea06, unchanged headers, before implementation. Screening finds11actual definitions and no current dot/local overlap. Remaining init72 raw instructions agree but unresolved imports prevent exact credit. Estimate0.90acceptance/25minutes=155.52bytes/hour, downstream0. Neutral28-byte accessor26CCD0 needs independent original provider/consumer evidence. Historical failed nerve forms lower that alternative's expected yield.

## Supplied CPU command intake, fresh root acceptance, 2026-10-02

Unchanged source769e51c/e6f5e56f passes original reference, debug ASan/UBSan and O3 checks from an absent fresh directory containing exactly twelve pre-frozen support files plus complete committed source. Root verifies3958 unique sealed paths before and after, with no generated report, fixture or executable copied. Each mode agrees on1514 streams,11900 packets,131076 raw words,3456 fixed uploads and11924 history words.240 original cached-copy cases agree on13824 copied words,216 copies/24 skips and240 output-canary pairs. Every redirected stderr is empty. Reference2.777seconds/native12.472/preservation94.585 are measured windows; preservation includes its nested17.421second regression run and must not be added twice.

All previous history, inline, uniform, context, face, root, palette, math, selection and49 CLI asset/scene outputs preserve byte-for-byte. The1083 absent owner values stay unavailable without supplied history. Unsupported/unproven fixed-register writes reject the entire prefix. Cached CPU copying and real scene/GPU submission remain separate. No automatic startup, defaults, active shader arithmetic, sampled float24, vertices, rendering or gameplay follows. Root report build/runtime/native_command_stream_root_validation/root_summary.json.

## Hourly733 checkpoint, 2026-10-02

Clean make.py eu -ca exits0, rebuilds45 Game/132 al/1 SDK C++ sources plus the generated stub object, links and exports code.bin. All179 output objects have provenance records. Prior733 acceptance remains backed by the canonical gate; only native runtime and documentation changed since it. Build warnings are retained in build/audited_push_733_d7350e2/clean_build_stderr.txt. The fresh native replay is accepted. Bubble/Archive proposals remain pending independent metadata and canonical acceptance; the hourly private-origin push proceeds before those gates.

## Bubble callback and constructor identities, 2026-10-02

Apply the corrected complete evidence patch unchanged after independent original creator/SafeString/interface/provider review. Four anonymous U function rows receive class ABI labels; every interval, pool, type and rank remains identical. Source272 is still pending. Root verifies406 original sealed paths,12 correction paths and28 independent peer paths. The evidence-only sender typo0x44 is corrected to0x2C, decimal44; source and diagnostics stay unchanged and original seal is retained. Table boundary evidence remains separatea359254. No original public-symbol spelling claim or exact/data credit.

## Draw initializer bounded requeue, 2026-10-02

Four new meaningful forms stop at paired1028/7 residual register-selection bytes. Root verifies1504 sealed artifacts; no fifth form or source adoption follows. All four prior raw contracts and the three accepted neighbor intervals preserve in scratch, without claiming a full canonical gate. Two physical compiler environment failures and unavailable historical dot intermediate snapshots remain explicit. Ledger uses measured21.550834-minute freeze-to-seal window; its8.949492-minute best-diagnostic subset is not added. Source patch remains held; only evidence and592-line packet are committed. Fresh fetch finds new dot/root-1eeba8 shader-output proposal, screened separately without exact credit.

## Archive callback neutral identities, 2026-10-02

Five blank whole U function rows receive only address-spelled identities from separately corroborated original constructor/dispatch/providers and external callers. All intervals, pools, types and ranks stay unchanged; raw table words are never copied or partitioned. Directory close forwards the full32-bit slot2C result with handle+14, independently confirmed by accepted caller2229F4. Root verifies1313 proposal paths and23 independent peer paths. The ordinary source-only weak8-byte accessor receives no retail identity and must disappear under normal canonical inlineclosure; scratch equality292 adds no credit. Public names/inheritance/backend/tableextent remain unknown. Prior frozen FileDevice seal keeps its disclosed3440duplicatecount/3452actualrecords mismatch.

## Archive ledger timestamp correction, 2026-10-02

The five abandoned rows initially used an approximate03:09 timestamp. Replace only those timestamps with the exact recorded seal endpoint2026-10-02T03:06:43.671934UTC from handoff_manifest.json. Attempts and the shared30.274227-minute window are unchanged. No byte/rank/source credit changes.

## Seven-root Bubble/Archive canonical acceptance, 2026-10-02

Observed2026-10-02T03:26:28.023307+00:00: all7/564 complete bytes pass the unchanged normal committed-source checker after a clean build. All733 prior roots/all753 actual canonical definitions preserve in282.349151seconds. Coverage740roots/49140bytes/1.783003%. Archive's weak8-byte accessor is eliminated by normal source-provenance inlineclosure; it receives no original address or byte credit. Bubble C2 is one section alias, not extra credit. Capped seven siblings and Draw1028 stay unaccepted. Source packets and original correction seals remain immutable.

Cumulative factory11916acceptedbytes/4.383618hours=2718.303bytes/hour. Carried5988; newly graded/investigated5928=1352.308bytes/hour. This falls from3147.746 aggregate/1487.360 new attribution at the prior observation. Root evidence intake, capped searches and tooling consumed the later window; no sustained local speedup is claimed. Per-function ledger windows include each shared family preparation plus the full4.705819minute canonical gate and overlap, rather than total labor.

## Fugumannen and MethodTree operating-family ownership before implementation

Assign large_actor_matching only Game/backup/src/Enemy/Fugumannen.cpp atd7350e2/source7811a9b8, unchanged headers. Move2/Move140+152=292newbytes, estimate0.70chance/45minutes=272.53expectedbytes/hour, downstream0/carried0. Original calls known Vector3CalcCtr::multScalar where current source expands scaling inline; provider/12-byte VEC3 ABI and ODR compatibility need independent evidence. Preserve18actual definitions,5accepted local676bytes and factory56. No target attempt/packet/dot conflict across88refs. Source-local imports only, fourformcap.

Assign large_library_matching only existing lib/al/src/Util/seadTreeNode.cpp at754f99a/source189dd2c8, unchanged headers/data. Seven MethodTree operating roots588newbytes, estimate(0.65*588+80downstream)/0.90hours=513.556expectedbytes/hour. Preserve6acceptedTreeNode436bytes/all7definitions. Original constructor/caller ground observed offsets; blank lock providers28CD24/84 and28CDB4/100 need separate neutral identity evidence. Constructor/string/manager tables and hardware lock implementation stay outside. Check89dotrefs, no operating-body claim. Any noinline organization on existing bodies must preserve their complete contracts and normal flags.

## Executor initializer family ownership, 2026-10-02

Assign actor_group_matching only existing alExecutorActorList.cpp at e05e466/sourcea54bec95 before implementation, unchanged headers. Root verifies23 frozen screening artifacts. Eleven outer roots988bytes have independent accepted Update/frozen Draw receiver/name/capacity/storage witnesses; no public class names are established. Estimate(0.85*152leaf+0.60*836loop)/1.05hours=600.762expectedbytes/hour, downstream0. Preserve threeO384bytes/allthree emitted definitions. Shared ExecutorStorage/FunctorExecutorStorage and native declarations must match committed Update tokens exactly. Keep common bases243B94/2415DC/1E7518 external so ordinary compiler inlining cannot erase original calls. Thirteen literal imports point to existing whole U data starts; separate neutral evidence/name patches introduce no data, split or table ownership claim. Four meaningful unsuccessful forms/root, packets then other work.

## 2026-10-02: Material-provider and action-family ownership before implementation

Root assigns compiler_candidates only runtime/AssetReader.cpp against06f7b2a, unchanged accepted source e6f5e56fb61a490f7d9369d368c17fa98303a72b884de8fcca5cac068e73ac0c. All101 frozen screening inputs verify. Original owner preparation and complete uniform transfer agree for127materials/217coordinates/623rows/4320words. The supported proposal is resource-local initialization, with caller boolean words and untouched padding explicit. Freeze the reproduction whitelist before implementation; preserve all previous native outputs. Runtime overrides, scene submission, shader arithmetic and gameplay remain open. The2–4hour estimate is planning only.

Root assigns actor_lifecycle only lib/al/src/LiveActor/alLiveActorFunction.cpp againste05e466, sourceb13a06ef5d27ebe96634ca5b9a12b7ef9d6d1ec5370121c80ef32f36707a8ade, every header unchanged. All37 screening paths verify. The460-byte action/presence family has no active overlap among91dot refs/255retained queues. Preserve all19previous nonzero definitions and11Oroots. Eighteen neutral imports and six target names stay in the independent evidence queue; grounded CPP-local prefixes avoid changing shared ModelCtr layouts. Estimated151.4accepted bytes/hour includes investigation and intake, with zero carried credit.

## 2026-10-02: Independent Sky pointer-provider identity

Root adopts the lane's unchanged Sky import evidence and names only the existing whole28-byte U/f row0026CCD0..0026CCEC as fn_0026CCD0. Provider loads and two external three-word consumers independently support the no-argument pointer ABI. The17-path peer seal0c925a5f and96-path source proposal seal4898dedd rootverify. No public camera spelling, provider implementation, boundary or data credit is added. Existing stage-switch imports retain their independently corroborated receiver/boolean contracts.

## 2026-10-02: Complete calendar conversion family ownership

Before implementation, root assigns byaml_matching only lib/al/src/Util/seadCalendarTime.cpp and complete lib/al/include/Util/seadDateUtil.h against06f7b2a, unchanged source3d6e5e70/header80f2afd8. All17 screening paths verify. Constructor116 and two complete244-byte setters total604newbytes with zero carried/downstream credit. The independent month table and twelve-byte parameter provider need neutral whole-row import review separately; no table copy or boundary split is authorized. Preserve seven CalendarTime definitions/fourO220bytes and both DateUtil definitions/twoO240bytes affected by the header. Constructor aliases are recorded as new definitions without duplicate root credit; use existing manual intake when the unchanged preparer cannot enroll added definitions. Estimated362.4bytes/hour includes70minutes at0.70family acceptance probability.

## 2026-10-02: Adopt reviewed family preparer metadata/header extension

Root applies source.patch51824554 unchanged, production5d5b0c3, after independent three-path reviewf39f0244 and all964proposal hashes verify before/after. Fresh root controls in build/family_handoff_extension_root_validation replay three positive families and seven meaningful refusal/unsuccessful controls. Every protected input preserves, every queue stays empty, zero credit. Extra unchanged headers use actual dependency closure; neutral whole-row naming remains human-reviewed and exact=false until strict checking after identity commit. Checker/provenance/flags/gate and undeclared weak-helper refusal stay unchanged.

Successful full metadata_pending, name-commit transition and non-replay queue coverage remain unverified. Existing replay controls are not enrollment proposals. The first root runner compiled two successful cases then stopped on a missing expected-list name; its reports/logs are retained and the runner resumed without rewriting them. No compiler/source/tool fix was used to alter a result. Proposal window29.755342minutes includes separate final-seal coordination, with initial reads untimed; positive replay and gate work adds no matched bytes.

## 2026-10-02: Independent whole executor initializer neutral identities

Root adopts unchanged evidence and27Symbol-only rows after independent full provider/caller/ODR review in build/executor_initializer_identity_independent_review_06f7b2a. Fourteen functions include eleven988-byte targets and three external bases; thirteen data names identify existing whole U/dc starts only. Every boundary/pool/type/rank/SectionName preserves. Independent original conditional BLNE001E3AC0 closes the caller inventory omission, without rewriting owner evidence. Shared storage types and six native declarations are token-identical to accepted Update. Source-local helpers disappear and final paired objects expose fourteen actual definitions, no extra bodies.

All2497pending source-package hashes verify. Its copied23-record screening index is historical and verifies only at its original screen root; it is not the final-family seal. Strict unchanged-object replay and full committed-source canonical gate remain pending. No data bytes, class/table ownership, public constructor spelling or exact credit follows from these names.

## 2026-10-02: Canonical Sky and complete executor initializer acceptance

The unchanged Sky72/executor988 batch passes all12 roots after a clean linking build. Every740prior root and 760 actual canonical definitions preserves; gate wall 292.016873seconds. Root verified all96Sky/17peer and2684Executor final/61peer paths, with pending2497seal retained. Only the normal checker sets O. Complete coverage752roots/50200bytes/1.821465percent. Neutral imports remain external; no table data or helper/alias credit. Ledger minutes include the full gate once per overlapping root window.

Observed 2026-10-02T04:10:22.621646+00:00, cumulative12976bytes/5.115450hours = 2536.629accepted bytes/hour, carried5988/new6988; new attribution 1366.058bytes/hour. Previous aggregate2718.303/new1352.308 remains historical. Intake and verification time are included. No sustained local investigation speedup claim. The tooling replay side audit independently checked the actual eight protected_input_hashes_before/after entries for all10cases after the first runner used absent key names; original reports remain immutable and all actual fields agree.

## 2026-10-02: Residual BreakModel and serial acceptance transport ownership

Before implementation root assigns large_actor_matching only lib/al/src/Npc/alBreakModel.cpp against757afcc/source609a3f087c1865b4275fdccb289bc7499663a9eb63e9b2b4879b6d911d601e93, every header unchanged. All34screen inputs verify; no91dot overlap. Preserve nineteen existing bindings/fourO220bytes and the unsupported44-byte initializer. Residualinit100has zero carried/downstream credit; estimated0.95acceptance/20minutes=285bytes/hour. Old equivalent object recovery is evidence only, with24inputs proven equal; paired checking/canonical source acceptance remain mandatory.

Root assigns actor_group_matching only new tools/acceptance_worker.py and transport edits to tools/acceptance_batch.py against779cfe8. All13read-only assessment paths verify. This is process amortization under the owner's approved factory-bottleneck work, with no checker/differ/progress/provenance/flag/oracle change or cached acceptance. Latest gate292.016873seconds;61.76–108.08seconds savings are estimates. Invoke unchanged check.main for every ordered actual definition, reset original argv/module rank caches, guard frozen inputs and reject worker/refusal/drift failures. Isolated full CLI/worker equivalence and real positive/negative controls precede adoption.

The existing driver restores candidate-rank changes but does not restore prior-rank mutations on a prior-check failure. Root authorizes a separately reviewable whole-gate map rollback hardening patch, with explicit original failure behavior and separate tests/evidence. It cannot weaken any prior check or claim acceptance of changed failed source. Failed source proposals still return to their owner and must be reverted/fixed before pushing. Keep transport and rollback patch/commit scopes distinct.

## 2026-10-02: Record independent calendar parameter and month-table import identities

Root verifies24peerpaths/seal4bd414ed and applies two Symbol-only whole rows unchanged. Existing48-byte month data and428-byte parameter producer remain external/U. Provider writes twelve bytes into incoming output storage; its finalr0 is not an evidenced returned pointer, so source discards unspecified result. No copied table, boundary split, public provider spelling or exact credit. The independently reviewed note is copied unchanged.

## 2026-10-02: Record independent MethodTree lock and propagation identities

Root verifies36peerpaths/seal4716679e and14complete original providers/callers/pools. Three whole-row Symbol-only names identify two external opaque-receiver lock imports and the68-byte two-pointer propagation target. Return values are unobserved and ignored; no hardware lock implementation, public API spelling, boundary or data change is adopted. Strict final source replay and canonical root preservation remain pending.

## 2026-10-02: Independently reviewed action and channel-presence identities

Root verifies91peerpaths/sealbb62c8ac, original32-path seal and separate correction0b3d1565. Eighteen neutral whole-row names preserve every other field. Original00265128 uses controller+2C,0024FCB8 uses+30; previously swapped JSON observations are superseded by the unchanged separately sealed correction. Fall-through extents and valid-channel/resource preconditions remain explicit. Six completion imports serve the capped192-byte packet; ready268-byte source excludes that body and those imports. No shared ModelCtr layout correction or matching credit follows from metadata.

## 2026-10-02: Accept resource-local material coordinate preparation

The unchanged owner proposal AssetReader.cpp0d30436f, committed604a5ff, passes fresh root original/native debugASan-UBSan/O3 replay and full prior native preservation. Root verifies all6672proposal paths before/after and21independent peer paths; the fresh directory received exactly17source/support files and no generated outputs. Both builds agree on127actual materials/381physical slots/217declared coordinates/623rows/4320packet words,64caller controls1952words and142synthetic cases3548words. Caller boolean words127and output alignment words189remain unavailable without explicit input;3owner packets are complete without alignment input.96presence controls and explicit unavailable/unsupported/malformed cases preserve the supported domain. Default float32 rounding, unmodified resource-local material state and genuine original callees are the acceptance boundary. No active scene submission, GPU/shader arithmetic, rendering or gameplay credit follows. Fresh reference2.120seconds/native58.577/preservation99.496 includes nested18.433regression; do not add it twice. Immutable root reports remain under build/runtime/native_material_coordinate_root_validation/.

## 2026-10-02: Canonical four-family acceptance

Complete unchanged owner proposals MethodTree476/action268/calendar116/BreakModel100, source9833c8f, pass15target roots after cleanlink and every752prior root/772actual definition. Gate328.783424seconds. Root verifies1082original+115strict MethodTree paths,988Action,572Calendar and103BreakModel paths; independent metadata remains separate. Calendar complete owned header is included in preservation; C1/C2alias adds no duplicate credit. No capped sibling is adopted. Complete coverage767roots/51160bytes/1.856297percent. Ledger wall windows overlap and include the complete canonical gate.

Observed2026-10-02T05:04:11.717011+00:00:13936accepted bytes/6.012421hours=2317.868bytes/hour; carried5988/new7948, new attribution1321.930. Prior2536.629aggregate/1366.058new attribution is historical. Continued evidence/tooling/intake work is part of elapsed time; no investigation-only speedup claim.

## 2026-10-02: Park six bounded function passes with complete packets

Root applies only unchanged packet/evidence patches, with their frozen hashes verified: twoFugumannen callbacks after3unsuccessful forms, MethodTree detach/isActionEnd/twoDateTime setters after4forms. No capped source/enrollment patch is applied. Fugumannen best3register bytes per140/152root, MethodTree best12/112, action best42/192, Unix setter final240/244extent refusal and DateTime setter176/244differences remain unaccepted. Both compilers and physical failure histories are preserved. Action uses the separately sealed corrected compiler-output-control explanation; historical packet remains frozen. Six ledger rows record actual forms and overlapping measured family windows, no fake fourth attempt forFugumannen and no scratch exact credit. Total60committed packets. Other work and active-owner/dot overlap checks precede a new pass.

## 2026-10-02: Assign common Executor initializers and retain image audit

Before implementation, large_library_matching exclusively owns existing alExecutorActorList.cpp at pushed86104d9/source4e87a160, unchanged headers/data, for common initializer roots1E7518/164,2415DC/156,243B94/20=340newbytes. Actor-group confirms no prior implementation history or active source ownership; its current reservation is acceptance tooling only. All current definitions/accepted siblings must preserve. Three whole data-row identities require separate independent evidence, with no table/split changes. Provisional expected accepted bytes/hour642.222 uses0.85chance/27minutes and no downstream double-count of the already accepted988bytes. Final read-only screen seal remains pending.

Private origin86104d9 is confirmed pushed05:05UTC after cleanlink/full752prior-root/772definition preservation. The767-root isolated full image also preserves all51160accepted bytes and372inputs/179current-frozen object/provenance pairs. Whole SHA078740c4de1fdd88e9892e0cece2886f4e5e6677843fc9952c21bf8ed3583295 still differs in2471322bytes. No scaffold or incidental padding credit. Next audited push due06:05UTC, start its clean gate by05:55UTC.

## 2026-10-02: Prioritize owner-requested second Beat and third Keeper Pro rounds

The owner asks to grade every supplied form for00156758 and00252B1C in rank order and commitOutcomes/attempt records. Before implementation, large_actor_matching owns private scratch Game/BeatBlockHolder.cpp with unchanged shared headers; actor_lifecycle owns private scratch alExecuteRequestKeeper.cpp and the complete keeper header52bdab31. Frozen sourcebase e66093e carries767accepted roots. Root freezes both received response hashes/copies; the raw pending documents are received in this separate metadata commit so canonical gate inputs remain clean, with no source, rank or trial-success credit. Prior rounds remain in75ac2e8/8c4cb6b. All3forms/address must retain tokens and use unchanged normal791/894 flags/build/checker. Keeper pointer alignment requires enclosing-layout and full767prior-root/actual-definition gates for each form. Beat unresolved constructor/group-table identities remain independent holds; closure refusal is reported honestly without aliases/oracle/flag/boundary rescue. Source stays private scratch until a complete canonical/preservation acceptance. Final graded documents and actual attempt rows follow. ProductStateTitle/SwitchAreaDirector matching waits for this explicit owner priority.

Common Executor final78-path screen is rootverified. Its final494.545expectedbytes/hour (0.80chance/33minutes) supersedes provisional642.222; downstream0 avoids recounting accepted outer988bytes.

## 2026-10-02: Correct Beat grading source reservation before implementation

The historical normal grader names Game/backup/src/MapObj/BeatBlockHolder.cpp; its shortened display log had led root to reserve Game/BeatBlockHolder.cpp incorrectly. Before any compile/source attempt, the exclusive large_actor_matching reservation is corrected to the historical Game/backup/src/MapObj/BeatBlockHolder.cpp. Existing Game source_dir, every include/compiler flag/config and the normal build step stay unchanged. No alternate source directory or enumeration workaround is authorized. Both paths remain absent on main.

## 2026-10-02: Independently approved common Executor data labels

Root verifies58supplier paths and38peer paths/sealf9030640. Independent review decodes24complete original providers/callers/pools and all3whole data rows/80bytes from the verified original. Only previously blank Symbol values change to dat_003D7744/28,dat_003D75E0/28,dat_003D670C/24; U/dc/bounds/pools/section and every other map column remain unchanged. Accepted outer providers and Update/Draw allocation independently corroborate receiver+0 and borrowed name+4. No public class/table ownership, address-point partition, hidden allocator argument, copied data, boundary split or matching credit is inferred. Supplier note is applied unchanged; the reviewer store-address transcription correction is retained separately. Source and canonical acceptance remain separate.

## 2026-10-02: Assign Rail family and bounded native draw ordering

Before new implementation, byaml_matching exclusively owns existing alRail.cpp at604a5ff/source666768e5, all headers unchanged, for812newcomplete bytes across5roots. Root verifies56screen paths and current source/header hashes;99dotrefs/caller/provider inventory finds no overlap. Preserve both constructors sharing oneO28-byte root. Neutral whole-row names and fall-through provider ABI questions remain an independent queue; do not adjust boundaries to consume trailing code. Forward expected462bytes/hour uses0.55chance and58remaining implementation/intake minutes. The conservative250.942figure includes the historical48.782minute Calendar-to-screen upperbound and waits; retain that observation without treating it as measured labor. Choosing by remaining expected work follows marginal analysis; completed screening remains in actual throughput measurements.

compiler_candidates continues exclusive AssetReader.cpp at13eea47/source0d30436f for resource-local mesh draw ordering and visibility, assigned before implementation. Original cached initializer1E061C..1E0754/pair-exchange191B9C agrees on39models/137meshes,15orders differing from simple ordinal order. The original mesh/material/shape gate3354C8 and8ordering/32node controls bound unmodified initialized material/resource state. Exact comparator/tie/exchange behavior and C-locale name comparison must be preserved. Missing or overridden runtime state remains explicit. Freeze replay inputs before the first implementation fragment; every earlier native interface must preserve. No active scene scheduling/camera/GPU/shader/render/gameplay claim or automatic defaults follows. Finalscreen seal and fresh original/native root replay remain required.

## 2026-10-02: Submit independently reviewed serialized checker transport

Root verifies34713supplier/67peer paths and reads the full unchanged transport patch. The persistent worker serially invokes unchangedcheck.main with originalargv for everyactualdefinition, refreshes map/writer/checker modules for each job, performs provenance/original-address/full-interval checks without cachedsuccess, freezes HEAD/tools/oracle/compilers/objects/sidecars/effective inputs and guards finalclosure beforeacceptance. No checker/differ/progress/provenance/flags/source/headers/boundaries change. Final older752-root/772definition fixture agrees withCLI outputs/errors/all180objecthashes/map;24genuinecontrols and process/client cleanup pass. Observed saving30.604048seconds/10.750201percent is oneconcurrent-load observation; initial37.409 andestimated62–108remain historical. Fullnegativegates used earliercleanupcode, with submittedrollbackdriverbyte-identical; finalcodefullnegative/Windows/timeout equivalence is unproved. Fresh current-root clean gate is required before adoption. Whole-gate rollback is a separate scope/commit.

## 2026-10-02: Submit separate whole-gate map transaction

The reviewed driver captures entiremap before cleanbuild/inventory/checks and restores it after worker cleanup on any rejection/exception/interruption/failure; the existing candidate transaction staysnested. This closes the prior-driver failure behavior that left legitimate priorO-to-mrank changes in the failedmap. Restoring ranks proves bookkeeping only, never preservation of failedsource; root mustreturn/fix/revert failedproposals beforepush. NegativeCLI/workerfixtures agree on4real mismatches andwholemaprestoration; finalpositive/control versions and historicalcleanup limit stayexplicit. Current clean fullpreservation gate remainsrequired.

## 2026-10-02: Common Executor acceptance and guarded checker adoption

Unchanged complete sourcea0fc3fe passes both targets1E7518/164 and2415DC/156, after a cleanlink and every767prior root/all787actual definitions. Gate331.952707seconds; cleanbuild94.032278seconds. Coverage769roots/51480complete bytes. The capped20-byte base is absent from submitted CPP and adds no credit. Guarded serialized checker transport9179198 and separate transactional rollbackae06272 are adopted after34,713supplier paths and67independent-review paths verified, positive/negative/drift/provenance/cleanup controls and this fresh root gate. The controlled final fixture measured284.683500CLI versus254.079452worker seconds,30.604048saved. Root gate runs under different load; no new comparative performance claim.

Observed2026-10-02T06:03:57.055478+00:00:14256accepted complete bytes/7.008349hours=2034.145bytes/hour; carried5988/new8268, new attribution1179.736. Per-lane windows overlap and are not summed as labor.

## 2026-10-02: Delegated small-function factory partition

Owner, relayed by Claude: "I would still want you to spearhead this project and have you autonomously manage it ... whatever you think would work best here where it's the least amount of friction and the most effectiveness." Claude's operating decision, not a new owner quote: branchfactory owns newUfunctions below0x100; local lanes finish in-flight work and then own>=0x100/blocked/packets/identity/runtime. Local lanes never edit Game/backup/src/Factory/. At hourly pushes mergefactory before the normal fresh clean/full-map gate, with no per-function factory write-ups or ledger rows. A failed merge gate is excluded from the push and regressed symbols recorded. Row-wise map driver uses start addresses and reports genuine same-row conflicts. Hard rules, provenance, lane and attempt caps remain unchanged. This supersedes local small-function selection for new work.

## 2026-10-02: Integrator takes main; factory reconciled (Claude, owner-directed)

The factory integrator committed each match to `factory`, checked it, then amended. Root's 01:54 merge took one of those commits before the amend, so 24 rows at 0x00346E8C landed on main as M while factory had them O. The integrator now commits each candidate on the scratch branch `integration-candidate`, runs `tools/check.py` there, squashes the result into one commit, and moves the target with `git update-ref <new> <old>` (compare-and-swap) only after every function is O and the final `Game`, `lib` and `data/config.json` trees equal the checked ones. Tests on an isolated clone: a proposal with one broken function left the ref untouched; the valid proposal moved it once, 0.23 s after the last check; a build input changed after the check halted the integrator with the ref untouched. The previous integrator moved the ref twice, the first time 29.9 s before its checks finished.

Reconcile: main and factory merged on the candidate, taking O wherever either side had O (24 rows from factory, 2 Executor initializers from main). A clean build took 55 s and the full check 224 s on b2a2e0a. The check demoted two rows to M: `_ZN4sead12CalendarTimeC2ERKN2nn3fnd18DateTimeParametersE` (O to U: the map row names the C2 constructor, armcc emits the code in a section named for C1, so the linker leaves it at 0x01000010; main alone at a0fc3fe links it there too) and `_ZN2al12MemorySystem23createSceneResourceHeapEPKc` (m to M). Two data rows the check moved from U to m were left at U, since only demotions were authorized. Main 0800d9a: 859 functions exact, 56,852 bytes, 2.06%. The `factory` branch stays at c194c5b, unused.

Owner rule 13: only the integrator moves or pushes main; root and the dot work on `root/` and `dot/` branches, never commit to main, never set ranks, never edit the ledger. A scratch build may still set a row to M to link it; that change is never committed. This is Claude's reading of "never sets ranks".

Lanes: rule 12 allows 8, compiles included. The factory takes at most 4 worker slots and drops one when the Mac swaps; the integrator's builds count as one lane; root runs with at most one subagent; the Pro relay is one lane. The 60-run model trial uses 6 worker slots, so root runs alone and the relay waits until the trial ends. Functions of 0x100 to 0x1FF bytes have no owner under the current partition (factory under 0x100, frog from 0x200). Claude cleans `build/exact_checks` in the main checkout if free disk drops under 8 GB; the halt line is 5 GB.

## 2026-10-02: Model trial on single functions; Luna kept under 32 bytes (Claude, owner rule)

The trial ran 60 single-function jobs: Sol high on 20 spread over four size buckets under 256 bytes, Luna medium and Luna max on 20 each under 64 bytes. Every match went through the integrator. Rates, minutes per job, and tokens per byte counted as uncached input plus output over matched bytes:

| Model | 0-31 B | 32-63 B | 64-127 B | 128-255 B | All | Minutes | Tokens per byte |
|---|---|---|---|---|---|---|---|
| Sol high | 5/5 | 3/5 | 3/5 | 2/5 | 65% | 3.1 | 679 |
| Luna medium | 10/10 | 2/10 | | | 60% | 1.6 | 806 |
| Luna max | 9/10 | 4/10 | | | 65% | 3.2 | 1,239 |

Owner rule: keep Luna for a size bucket only within 10 points of Sol high there. Under 32 bytes both Luna settings qualify (100% and 90% against 100%); Luna medium costs 401 tokens per byte there against Luna max's 999, so Luna medium takes tier 1 single functions under 32 bytes and Luna max is dropped. From 32 to 63 bytes Luna trails by 20 to 40 points, so Sol high takes them. Sol high matched 65% of single functions, above the 30% line, so single functions stay in production. The samples are small (5 per Sol bucket).

## 2026-10-02: Owner changes at 04:35 (Claude, owner-directed)

Functions of 256 to 511 bytes now belong to the factory: 1,762 jobs covering 1,780 functions and 630,140 bytes start at Sol xhigh with a 20-minute timeout in reserved slot s1; misses go to the Pro relay. Rule 12 no longer caps factory worker slots; the swap guard starts at 4 of 10 slot threads, drops one on swap-outs (2,048 pages in 5 minutes) and adds one after 15 quiet minutes. All open jobs were reset to tier 1 (Sol high), since none had a Sol high attempt. Priorities: browser demo, then 100% byte-exact. The root finishes its current branch, then works only on the port runtime toward the six port milestones, with decompiled functions replacing recompiled ones by address. Class layout questions go to the Pro relay.

Integrator faults found and fixed today: a submission with no claims failed on an empty final commit (root heap-layout, after its full check passed); a claim on a row already at M failed on an empty rank commit (root CalendarTime); trial slots paused by the swap guard never exited. Root's two submissions are queued again. Frog pushed three NonMatching reconstructions (dot/course-select-scene-init, dot/model-resource-setup, dot/texture-binding-state) with no exact claims; they are not submitted.

## 2026-10-02: Provider errors retry; failed spend; usage-policy halt (Claude, owner-directed)

Run 79 (band job 11683, two functions of 456 bytes) ended with "Selected model is at capacity" and was counted as a miss, which failed the job and queued it for Pro. Owner fix: a provider error is retried at the same tier after a back-off of 60, 120, 300, 600, then 900 seconds per slot; it never escalates, never goes to Pro, and never counts toward the 50-in-a-row halt. Run 79 is now outcome "error", job 11683 is open at tier 3 again, and its Pro packet was withdrawn before the relay sent it.

Workers now run without `--ephemeral`, so each run's Codex session file records token counts as the turn goes; the integrator reads the last count when a run dies before `turn.completed`, then deletes the session file. Runs ending in an error or a timeout show as failed spend per tier on the dashboard. Run 79 predates this and reports 0 tokens.

A response saying a prompt was flagged for usage policy halts everything: the integrator checks each worker's log, and every 30 seconds scans new events in the root's and the relay's Codex session files; it writes HALT_ALL, and Claude stops the other lanes and tells the owner.

The integrator restarted at 04:54 to load these changes; the restart abandoned three in-flight runs, whose jobs reopened. Root's branch root/integrator-commit-receipts, a patch to the repository's copy of the integrator for the two empty-commit faults, was declined: the integrator is operator-owned and both faults were already fixed. Frog has pushed five NonMatching reconstructions since resuming (course-select-scene-init, model-resource-setup, texture-binding-state, item-spawn-dispatcher, skeletal-animation-construction); none claims an exact function, so none was submitted.

## 2026-10-02: Batched integration; submissions ride the periodic full check (Claude, owner-directed)

The integrator built and checked each factory proposal on its own, about 35 seconds each, and gave each root or dot submission that changed build inputs its own clean build and full check, about 6 minutes, during which no factory match could land. Now one candidate takes every queued proposal touching different files (up to 40 proposals or 400 functions), with one build and one round of per-function checks. A proposal that misses is set aside in `proposals/rejected/` and the rest go through a fresh cycle, so main still receives only checked source. Submissions that change build inputs ride the periodic full check; quiet ones land at once. If a combined check fails, main is checked alone in the probe worktree: rows main itself loses follow the demotion policy, a lone failing branch is rejected, and several failing branches ride alone from then on.

Tests on an isolated clone: two clean proposals made one move and one commit; a batch with one broken proposal moved nothing, set it aside, and landed the clean one in a fresh cycle; a quiet submission landed at once while a claiming one waited, then rode the full check and was accepted with 25 exact; a broken rider was rejected with main unchanged. The three reference-safety tests pass on the batch code.

Each worker's Codex session file is deleted as soon as its tokens are read, and stale ones are cleared at startup. The dashboard shows the integrator's queue and the minutes since the last push to origin. Root's branches kept conflicting with each other on `project/STATE.md`; the repository now merges that file by taking the submitted branch's version, since only the root edits it.

## 2026-10-02: Source-quality order: findings and first measurements (Claude, owner-directed)

Class with the most Factory functions: `al::FunctorV0M`. 72 instantiations at 0x003D5A4C to 0x003D5F4C, each a vtable of `operator()` and `clone`, hold 141 exact Factory functions (7,344 bytes); no other class attributable by vtable reaches 7. The repository already defines the template in `lib/al/include/Functor/alFunctorV0M.h`, and AppearStep.cpp and Garigari.cpp instantiate it, so the owner classed this as a deduplication job, not the pilot result. Each instantiation's host class comes from the method that builds it; instantiations whose host is not named stay in place.

Second pilot: no stateful class has 15 exact Factory functions yet. Attribution by adjacency to named map functions finds only `al::FunctorV0M` (123) and free functions in namespace `al` (18). Attribution by vtable-anchored translation-unit windows finds a base-class region (0x0032C58C to 0x00331528) that 302 vtables point into, whose 36 Factory functions are 8- to 16-byte default virtual methods. Factory output so far is mostly small: functor instances, nerve executors, default virtuals and getters.

Shiftability scan on main 648eb02: 8 raw image address literals under Game, all in `Factory/group_00350028.cpp`: bss addresses 0x0042FA14 and 0x0042F534 passed to its BODY macro. Three more literals in the image ranges are powers of two passed to `fn_00250EE0` (0x100000, 0x200000, 0x400000) and are reported apart as probable flags. Neither bss address has a map row, so a `dat_` name for it cannot resolve until a row exists (a rule 2 evidence commit); a cleanup job is making that fix.

How unmatched code and data get addresses today: the build's generated `stubs.c` gives every function row a weak stub in a section named `i.<symbol>` (2,907 stubs; 229 aliases for unnamed rows that committed source references), and gives each `dat_` name that committed source references a weak zero-filled array, but only where a data row starts at exactly that address (645 arrays). The generated scatter file pins each section at its row's start in `map.csv`. All 10,005 data rows are rank U: no data is reconstructed in source yet. 1,328 data rows are referenced by no function's literal pool, and the 251,524-byte bss has 5 rows covering 304 bytes.

Integrator changes, tested on an isolated clone: cleanup branches may change the Factory directory, ride the full check alone, and are rejected on any loss of an O row (a reformatting branch was accepted; a branch dropping one definition was rejected with main unchanged); facts land as one docs-only commit per batch; the three reference-safety tests pass. Source quality at 07:03: 63.7% of exact bytes are in class files (51,364 of 80,604).

## 2026-10-02: Queue order fixed; class mode for Bubble; literals fixed (Claude, owner-directed)

In the hour to 07:08, 70 of 85 non-trial runs were on functions under 32 bytes (64 matched, 33 of them group jobs) and only 6 reached 64 to 255 bytes (3 matched). The cause was queue order, not Sol failing: sibling-known and group jobs sort first by priority, and singles sort smallest first, so Sol slots worked small groups. Owner fix: Luna medium alone takes every tier 1 job under 32 bytes, single or group; Sol high slots take 64 to 255 bytes, largest first within the range, then 32 to 63; the band slot is unchanged. This supersedes "Luna never takes group jobs" for jobs under 32 bytes.

Second pilot in class mode: Bubble. It is named in the map through its vtable, has `Game/backup/include/Enemy/Bubble.h` and `Game/backup/src/Enemy/Bubble.cpp`, and its translation unit (0x00302840 to 0x00303628) holds 19 unmatched functions up to 511 bytes in every size bucket (5, 4, 2, 5, 3), with 3 exact already. It was the only class meeting all three conditions: classes named by unmatched mangled methods are SDK namespaces without headers here, and vtable windows of other named classes span their own overrides only (at most 6 to 14 unmatched). Today 651 unmatched functions (122,060 bytes) have a known class from their map names. Class-mode integration was tested on a clone by replaying commit a5a2ae45 (three FireBall functions): one move, three functions O; a stale proposal was refused.

The raw address literal job (Sol high, 10.2 minutes, 136,262 fresh tokens) added bss rows 0x0042F534 and 0x0042FA14, 0x20 bytes each (initialization writes eight floats through each), and switched group_00350028.cpp to dat_ names; all 8 functions still match locally. It rides the next periodic full check under the cleanup guard.
## 2026-10-02: FunctorV0M cleanup symbol and vtable evidence

Cleanup lane cleanup/functor-v0m, base e8cc28789146f8adefb87de4b218a032767296bd,
grounds five instances using builder object stores, callback descriptors, named
map methods and existing host headers. The ten function rows acquire the
compiler's template-instance names without changing any other function column
or rank. The five anonymous vtable regions are repartitioned at their ABI header
addresses, eight bytes before the callable address points; the existing
AppearStep clone's compiler relocation establishes that addend independently.
Data ranks remain U and covered intervals are preserved. No function boundary,
target binary, compiler flag, checker or ledger changes. Full attribution,
old/new symbols, exact data partitions and all 66 blocked instances are recorded
in [functor-v0m-cleanup.md](functor-v0m-cleanup.md). AppearStep already uses the
template; Garigari's remaining clone stays raw because its callback has no real
map/header name. Build/check outcomes will be recorded after canonical checks;
this naming evidence itself claims no new accepted bytes.
## 2026-10-02: BSS rows for raw-address cleanup

Add unnamed rank-U `db` rows `[0x0042F534, 0x0042F554)` and
`[0x0042FA14, 0x0042FA34)`, each 0x20 bytes, in a separate boundary
evidence commit before changing `group_00350028.cpp`. The EU initializers
and constructor each write eight four-byte floats through the respective
base: the last word begins at offset 0x1C. Independent initialization of
the next objects at 0x0042F554 and 0x0042FA34 corroborates both endpoints.
Both intervals lie in BSS and overlap no existing map row. Leave Symbol
and SectionName empty so the referenced dat_ aliases use their default
BSS sections; leave every existing row and rank unchanged. Full address
traces and oracle identity are in [raw_address_bss_evidence.md](raw_address_bss_evidence.md).

## 2026-10-02: Duplicate definitions, the full-image regression pass, scoped header rechecks (Claude, owner-directed)

Duplicates. A scan of the strong global symbols in all 438 compiled objects on main found three symbols defined twice. `rp::getPlayerActor` is in `Factory/group_00189160.cpp` (factory commit d2fc3c4e, 06:43) and `Player/PlayerFunction.cpp` (upstream). `alSensorFunction::updateHitSensorsAll` is in `Factory/group_00244898.cpp` (d7b40825, 08:37) and `lib/al/src/LiveActor/alSensorFunction.cpp`. `rp::createCoinRotater` is in `Factory/fn_00276588.cpp` (2ab610a4, 07:07) and `MapObj/CoinRotater.cpp`. No other symbol is defined twice except two template vtables in COMDAT groups, which the linker merges by design. They passed the integrator because it checked only that a proposal's files were new and its claimed rows were not yet exact. It never asked whether another object already defined the symbol, and the archive link silently selected the Factory member. The class-file copies were compiled all along, because make.py defines NON_MATCHING for every build. Branch `cleanup/remove-duplicate-definitions` keeps one definition of each, in the class file. updateHitSensorsAll and createCoinRotater keep their class-file bodies, which tools/check.py reports exact. getPlayerActor takes the Factory body, because check.py's source closure check rejects the class-file body. That body reads `Application::sInstance` through a non-branch relocation. The branch rides the full check alone under the cleanup guard. The integrator now rejects any batch proposal, class-mode proposal or submission whose objects define a strong symbol that another object defines. When two proposals in one batch define the same new symbol, the first keeps it.

Observed while checking the duplicates: when the source closure check rejects a row, tools/check.py returns the row's previous rank, so an `O` row prints "Still matching" without fresh evidence. check.py stays untouched (owner). The integrator does not treat that message as a confirmation of a cleanup.

Regression pass. The periodic full check now builds clean, then runs the full-image byte compare (`make.py eu --split`). That compare links every `O` row at its original address and compares its bytes with the original image. A row it reports different is demoted only when tools/check.py, run on that row, no longer reports `O`. Claims are checked one by one with tools/check.py. When the compare stops before comparing every `O` row (for example, when a row's size changed), the full tools/check.py pass decides, as before. Once a day the full tools/check.py pass also runs as an audit beside the compare. If the audit finds a row the checker demotes that the compare did not flag, the regression pass goes back to tools/check.py until the owner decides. Proof on an isolated clone at 5727eb8f: on one clean build, the compare and the full tools/check.py pass confirmed the same 2,384 `O` rows. Neither flagged nor demoted any row, and the checker gained none. The compare took 129 s and the checker 940 s. A same-size break of `rp::createCoinRotater` was flagged by the compare, confirmed by check.py (`O` to `m`) and demoted. An extra row reported different to the integrator, `al::SystemKit::createFileLoader`, was kept `O` because check.py still reports it exact. A size change stopped the compare before it compared, so the check.py pass decided.

Class-mode header changes. A class-mode proposal now lands directly when its header changes. The integrator rechecks the claim and every exact function in the class file's object and in every object whose dependency file names the header. A header that reaches more than 240 exact functions rides the periodic full check instead. Proof on an isolated clone: a comment-only change to `FireBall.h` landed after rechecking 5 functions in 1 object (65 s). A change to `SceneObjFactory.h` that renumbered `SceneObjType_CoinRotater` rechecked 15 functions in 5 objects and was rejected for `rp::getCoinRotateY` in `CoinRotater.cpp`, with main unchanged.

Port milestone reruns (Claude, 09:05 to 09:40, outputs under ignored build/ only, no images committed). Each used a fresh Azahar recording and the root's sealed native library (SHA-256 e7953898…). Milestone 1, exit check as the owner set it: the static recompiler builds natively and reaches the first frame's GPU command stream, matching Azahar. Passed: all 528 events with ticks and all 79,936 PICA bytes matched through the first top-screen swap, in 226,978,458 guest instructions with no interpreter or JIT fallback. Milestones 2 and 3 had names only. The root chose their checks. Milestone 2, presentation 60 (the title logo): all 1,430 events, 765,728 PICA bytes, 691,200 RGBA bytes and 345,600 framebuffer bytes match. Passed. Its pixels come from Azahar's software GPU driven by the native CPU, so no port renderer is involved yet. Milestone 3, a scripted button, stick and touch movie to presentation 360: all 1,802 input polls plus the rendered capture match. Passed. The native library was run as built, not regenerated from source.

Hard-end trial step 1: Astra high on one band function, job 12061, 08:39 to 08:52. It finished with no provider error and no policy flag, matched nothing, and spent 107,543 fresh tokens. Step 2 opened.

## 2026-10-02: portable guest arithmetic uses pinned integer SoftFloat

The browser libc cannot reproduce the native helper's host rounding/exception control. The port therefore uses official SoftFloat 3e, ARM-VFPv2, generic integer primitives and C11 TLS on both targets. A pinned source/header/license manifest verifies all dependency inputs; downloaded source stays ignored and its BSD notice accompanies built archives. Guest DN/FZ and NaN operand priority remain explicit in the adapter. Original VMUL probes in both precisions establish before-rounding tininess; separately rounded original multiply-accumulate probes forbid fused substitution.

The final target passes 88192 selected original-instruction result/FPSCR comparisons on native and wasm32, 88064 generated native-entry comparisons and two untouched complete recorded-menu Azahar comparisons. Existing Game/lib/config/map/rank/ledger and ARMCC flags are unchanged. No new translated bytes or exact credit follow. See project/portable_floating_point_evidence.md for final hashes, reproducible commands and limits.

## 2026-10-02: link sealed translated entries into wasm32 separately from the platform

The translated target uses module-local O1 and disabled contraction to match the
verified native optimization level and keep compilation of large generated C
units bounded. It compiles all timed source units and the address-selected
priority source override, with the accepted integer arithmetic target as its
link dependency. The builder requires source seals and rank-O/source identity
on frozen integrator main. ARMCC inputs and flags stay unchanged.

The actual linked Node module preserves all 642664 addresses and conservative
2437712 function instruction bytes. All 88192 selected floating-instruction
cases and 4105 source replacement cases match original ARM execution, with zero
CPU fallback. Six refusal/first-difference controls pass. A warning exemption
surrounds only the unused helper in the sealed generated header; strict warnings
remain enabled for the verification runner. No generated header changes.

Host Context/Entry size checks establish only the wasm32 port interface. They
provide no guest ARMCC class-layout evidence. Linked coverage and selected
execution are recorded separately. The platform, browser frame, World 1-1 and
complete rank-O source registry remain open. Final hashes, reproduction and
scope are in project/webassembly_translation_evidence.md.

## Hard-end trial closed (owner, 2026-10-02 13:55)

Step 2 ran 20 functions on gpt-6.1-sol ultra and gpt-6-astra high, 10 attempts each, with no escalation. 21 runs
finished without a stall-stop cut (4 started before the 09:54 deploy). Result: 1 match in 21 runs.

- Astra high, 256-511 byte band misses: 1 of 6 matched (0x001A8D50, 356 bytes, 13.2 minutes), 1,156 fresh tokens
  per matched byte, 13.2 minutes per run.
- Sol ultra, band misses: 0 of 5, 22.2 minutes per run.
- Astra high, 512-1,023 bytes: 0 of 5, 19.3 minutes per run.
- Sol ultra, 512-1,023 bytes: 0 of 5, 32.5 minutes per run.

The owner closed the trial on this result: the 18 reruns of cut runs were cancelled, no Sol ultra or Astra tier is
added, and hard functions stay frog's lane. Weekly usage per run was read from whole-percent readings shared with every
concurrent worker, so it was not used for the decision.
## Names for frog's exact claims (2026-10-02 14:00)

The integrator rejected three exact claims from frog (dot/root-12de18, dot/root-187ab8, dot/root-305644) because their
symbols name map rows that were unnamed on main. This commit names those rows and two callee rows, with no boundary,
extent or rank change. The evidence for each is in the branch's own report under project/dot_reports/ and was read
from the retail executable only:

- 0x0012DE18 KoopaPillar::init: the init slot of primary vtable 0x003C6398; constructor 0x0012E234 builds a
  MapObjActor; the object-name comparisons and the break-model label name the KoopaPillar family.
- 0x0016E7A0 KoopaPillarBreakModel constructor and 0x0026FCB8 al::PillarBaseModel constructor: the callees
  KoopaPillar::init constructs, per the same report.
- 0x00305644 Gorori::init: the init slot of vtable 0x003D2A44; constructor 0x0026AB84 sets the 0xF8 layout.
- 0x00187AB8 CounterCollectCoin::collect: the constructor names the archive "CounterCollectCoinD"; the method name is
  descriptive, as frog's report says, not a recovered symbol.
- 0x003B7AB0 .constdata.CounterCollectCoin.cpp: the 20-byte constant data row that source's constant data links to.

Not named: 0x0028CB38, which frog's KoopaPillar source calls al::StringTmp<128>'s constructor. Main's
lib/al/src/Npc/alEffectObj.cpp already calls that row as fn_0028CB38, so renaming it would break that object;
KoopaPillar's source must call fn_0028CB38 instead.
## 2026-10-02: reduced WebAssembly platform and real entropy

Root/webassembly-platform freezes main 5a3ee8a8 and keeps the matching ARMCC inputs unchanged. A separate opt-in public Azahar profile uses GENERIC architecture, software rendering, actual platform services and the accepted static CPU adapter. It excludes guest ARM CPU interpreters/JITs, shader JITs and native loader/media implementations. Missing static CPU registration throws. Module-local O1/pthreads/wasm exceptions reproduce the already verified translated CPU without changing matching flags. Native option-off branches remain, with no additional native-build claim.

The platform uses pinned public dependency revisions and original license notices. LibreSSL keeps real ChaCha/getentropy with checked pthread locking and zero-initialized allocation; failures abort. Four typed size-zero returns fix wasm32 narrowing. Thread diagnostics use the actual Emscripten API. The finite Node-only link uses NODERAWFS for the owned input copy. Browser file/worker transport is a separate family.

A fresh full host reaches the natural first GPU swap and matches all 528 raw events/ticks and 79936 PICA bytes against corrected Azahar, with zero ARM interpreter/JIT fallback. Actual real-archive entropy/concurrency and C/Node failure checks pass. Source review exposed five tooling gaps; archive/notice provenance, bounded usage, preserved timeout logs and complete source exclusions are fixed before submission. An actual blocked Node entropy provider proves the 60-second refusal/log path. GPU parity and diagnostic CPU counts remain separate claims.

The final build/replay receipts, source hashes, reproduction and explicit gaps are in project/webassembly_platform_evidence.md. No browser frame, World 1-1, complete rank-O registry, semantic gameplay state or new exact/native byte credit follows. Only the integrator moves main/ranks/ledger.

## Factory deployment record and driver transfer, 2026-10-02

The tracked `tools/factory/factory.py` now records the already deployed operator file, SHA-256 `594cfa3994e0b5c0b2248a0ff53f3e96c10eebee947f8128b84f97d7c5b727e0`. Production has used this file since the 13:57 deployment. This submission changes no running factory process and does not restart it. It records the approved six-slot cap, load guard, whole-diff presentation, closed-trial isolation and dot-first submission ordering. The source was compared byte for byte with the deployed file and passed a syntax compile. The preceding driver reported the three reference safety checks and targeted checks before deployment; the retained `tests/ref_safety_candidate.log` records all three passing. Those historical checks were not rerun for this copy.

Codex assumed driver ownership after the Claude session released it. Hourly checks and the safety monitor are active. Root retains port ownership, the relay retains its pending Pro answer, and the owner alone messages frog. The Claude Artifact dashboard cannot be updated by the new driver with the current toolset and remains explicitly stale.

The rejected `dot-course-list-87d89a0ff` submission must not be retried unchanged. The branch final canonical report states that the World constructor is 692 compiled bytes versus a 700-byte original interval and remains M. Its World source body is identical to current main. An earlier text extraction had incorrectly treated a checker command as an exact claim. No match is credited.

Draft carryover and post-crossing acceptance measurements are recorded in `project/driver_measurements_2026-10-02.md`. Worker post-run matches and integrator-accepted bytes remain separate.

## Correct unsupported placement-map intake claim, 2026-10-02

The operator held `dot-placement-map-359c64101` before another full-check attempt. Both `project/dot_reports/placement-map.md` and `placement-temporary-lifetime-followup.md` at commit `359c64101` explicitly say initPlacementMap remains nonmatching. Their exact 144-byte sibling is tryGetPlacementInfo, which is already O on main. A checker command in a report is not a successful checker result. The request was moved unchanged to the local held directory, with its SHA-256 and reason retained in `logs/driver_intake_corrections.jsonl`. No game source, oracle, map, rank or ledger was changed.

The weekly usage reset closes the credit-period acceptance measurement recorded in `project/driver_measurements_2026-10-02.md`. The six-slot production setup and load guard remain unchanged.


## 2026-10-02: Actor registry evidence corrects the historical scan scope

The reproducible 268/222 scan combined five registries and treated literal-pool candidates as table links. Preserve that historical result with explicit heuristic labels. `project/actor_catalog.json` separately traces the complete 225-entry actor registry to primary table installations, retains 13 evidenced current C++ identities, and claims no recovered original class spellings or unique virtual method ownership. The driver independently reproduced every actor and historical record. `project/actor_catalog_evidence.md` and `tools/factory/inspect_actor_registry.py` document and reproduce the check. The RailDot/Seagull containing-row conflict remains a separate map-evidence issue; this commit changes no map row.


## Worker packet reference deployment, 2026-10-02

The owner approved the prepared update. The driver deployed the verified factory copy and restarted it within 44 seconds of the three reference-safety checks passing. New packets embed curated compiler notes and bounded actor references from their own source revision. Four fresh production packets contained the complete notes; their targets correctly had no actor-reference sections. Direct checks covered four packet forms and distinguished Seagull from its broad containing RailDot map row. The integrator and resource guards are unchanged. See `project/packet_reference_deployment.md` for the evidence and live-reference observation still pending. No exact-byte credit or throughput gain is claimed.
## Names for frog's Bug::init claim (2026-10-02 16:45)

dot/root-2d4544 claims Bug::init exact; its symbols name two rows that are unnamed on main. No boundary, extent or rank
change. Evidence, from the retail executable as recorded in project/dot_reports/root-2d4544.md:

- 0x002D4544 Bug::init: retail actor-factory entry 0x003B9A48 names "Bug" and points to creator 0x00397DF4; the class
  vtable's init slot is 0x002D4544.
- 0x0027AF28 EnemyStateHipDropDown constructor: the class is already declared in main's
  Game/backup/include/Enemy/EnemyStateHipDropDown.h, and Bug::init calls this constructor with the host, its
  parameter object and a name.

Held for the owner: dot/root-11a174 (FireFlower), dot/root-1579f0 (BoomerangFlower) and dot/root-177020
(SuperLeafSpecial) also need six shared helper rows named FlowerInit::*, which frog's reports call descriptive
proposals rather than recovered names.
## 2026-10-02: FileSelect operation identity and ABI repair queue

Name the existing 0x0014AEDC..0x0014B0E8 function row `fn_0014AEDC`. Its 0x0014B0D8 pool and boundaries remain unchanged, and its rank remains U. The accepted `fn_0014AED4` caller already refers to that exact name; the owned EU binary branches from the 8-byte caller to this interval. This is an address identity, not a recovered original C++ name.

The source proposed by `dot/root-14aedc` at 57f41a5a8 defines a void operation-state update. Its report explicitly flags the accepted caller's broad integer return and four-argument placeholder as incompatible C++ declarations. The driver held the original submission unchanged. A separate `cleanup/file-select-operation-abi` family, frozen from main 20ec130a8a313b3af9af2262d5c94a4d2bf92e3a, reconciles the root, its nerve execute caller, and three existing nerve helpers in a shared source/header. Both touched Factory translation units are in the preservation scope. No source claim is accepted by this naming commit; canonical checks and the integrator's full preservation gate still decide the cleanup.
