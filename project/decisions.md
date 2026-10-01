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
