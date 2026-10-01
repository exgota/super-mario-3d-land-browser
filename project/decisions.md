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
