# Course-select scene initializer: complete NonMatching reconstruction

Frozen base: `86104d96a7f570383bbfefc5fffad998e134496c`. Branch: `dot/course-select-scene-init`. Only a new translation unit and this report family change. Existing sources, headers, map, ledger, STATE, tools, configuration and game inputs remain unchanged. No exact credit is proposed.

The unchanged row is `0x0017CB1C–0x0017D398`, 2,172 complete bytes. The final committed C++ source emits 2,156 bytes with the normal Game-module ARMCC 4.1 build 791 settings. The unchanged project checker reports:

```
U -> M: The complete compiled section, including its literal pool, has a different size from the original interval.
```

The map row is temporarily given the neutral `fn_0017CB1C` spelling solely for the checker, then restored byte for byte in a `finally` block. Its original blank name, U rank, Type and boundaries remain unchanged. The source is guarded by `NON_MATCHING`. The normal clean project build links but retains U scaffold behavior; it is not evidence that the source body executes. A separate diagnostic link uses the untouched full canonical compiler object and existing original-address imports, with no size/equality claim.

## Identity and recovered behavior

Independent constructor `0x0017D8C0` calls the accepted `al::Scene` constructor at `0x00274498`, installs the address point `0x003CD21C`, and stores incoming r1/r2 words at object offsets +0x34/+0x38. The inline object label is コース選択シーン. This vtable's +0x14 init slot contains `0x0017CB1C`. Independent caller `0x00212428` allocates 0x6C bytes, loads r1 from its object+0x14 and r2 from `*(object+0x0C)+0x154`, then calls that constructor. These observations establish the scene role and two supplied words; original public C++ parameter types remain unresolved.

The existing `CourseSelectScene`, `CourseList`, `GameDataHolder` and scene headers were inspected for compatibility. The incomplete scene declaration is not changed. The new TU uses private observed ABI views and existing accepted API declarations at their established symbols. It adds no public class identity, shared header correction, resource/table definitions or new compiler flags. All 74 code/data imports resolve to existing whole map rows; `course-select-scene-init/imports.json` records their addresses, extents and types. A read-only audit froze 115 local dot ref heads and screened 660 distinct source/header blobs. Exact-name imports agree with current main after the formatter correction. Prior LightData uses the same unsigned-word `dat_003D7ABC` table view. Two unintegrated older branches still declare incompatible generic formatter signatures: `dot/actor-init-byaml` uses `void(void*, ...)`, and `dot/player-actor-family-45a` uses `int(void*, ...)`. They require reconciliation before combined integration; this proposal does not modify their families. `import-compatibility.json` preserves the matching blobs/ref heads, and `audit-imports.py` repeats the read-only screen.

The initializer selects a normal or special course-select stage name, preloads map archives for stage-type courses, constructs its resource state, camera pair, scene objects, selection/player/map/layout objects and transitions, initializes placement and nerves, finishes the actor kit, and scans the courses again to find/create their resources. It preserves the original pointer snapshot before the player-position getter, including the tested callback alias case. The field names in private ABI views describe only observed accesses.

The map's pool marker is not the end of the body. The first executable region is `0x0017CB1C–0x0017D124`; an interior literal/string pool occupies `0x0017D124–0x0017D244`. Execution resumes at `0x0017D244` and continues through `0x0017D380`, followed by the final pool through `0x0017D398`. The full 1,860 instruction bytes and 312 pool bytes remain in scope. Strings in the TU are semantic CP932 text, matching the repository's existing source encoding. Global camera parameters, string tables and nerve objects remain external imports; no resource/table bytes were copied into the source.

## Attempts and limits

Two meaningful source forms were attempted. The initial form at `ce871b4` did not compile because ARMCC rejects extern objects with abstract `al::Nerve` type. Declaration repair `c0eaff2` compiled a 2,136-byte body; the checker rejected its aggregate listener constant because it introduced unmapped `.constdata` relocations. That rejected form is retained in branch history and receives no credit.

Form 2 at `a1265ee7700e2a4fe982c5caecf36e92ac70d06f` constructs the observed listener fields directly and preserves the camera pointer read before the getter call. It compiles to 2,156 bytes and reaches the checker's unchanged complete-size test. The first final clean build recompiles this same source. Import-compatibility revision `8164705` then uses main's exact `s32 fn_0028E1E4(sead::BufferedSafeString*, const char*, ...)` declaration with explicit observed-view casts, and the existing `sead::Vector3f::zero` declaration in place of a raw array alias. Its fresh clean build produces identical linked root bytes. Physical history is five compilations: one declaration error, four successful source/declaration or clean builds; these are two matching forms plus declaration-only repairs. No third grounded form was found; no padding, volatile, register-shaping, inline assembly, fabricated helper address or metadata-boundary change was used to consume the remaining attempt allowance. Build 894 is absent, so this work makes no comparative compiler-identification claim.

The measured wall window is 2026-10-02 05:17:23–05:48:59 UTC, 31 minutes 36 seconds through the verification checkpoint. Accepted bytes and accepted throughput are both zero. Packaging after this checkpoint is separate. Final clean build plus strict/diagnostic verification took 43.890 seconds; the final sealed replay and preservation durations are recorded in results.json. These measurements are not a claim of decompilation acceptance or sustained throughput.

## Whole-root replay

The executable fixture generator and frozen JSON contain 254 cases, and all 254 entries execute. IDs are 0–239 and 241–254. ID 240 belongs only to an uninstantiated generator base-dictionary label; it is not an excluded diagnostic or a skipped failing case. Final replay compares the untouched original root with the source-generated diagnostic body under the same modeled environment. It records:

- 250 matching normal returns, including callee-saved r4–r11/d8, restored stack pointer, FPSCR, normalized call traces, scene writes and allocated-object bytes
- Two matching actual Unicorn memory faults, with equal access/address/size, preceding traces and observed memory
- Two matching modeled null-precondition rejections, kept separate from CPU faults
- All 465 original root instruction addresses visited across the suite, including every instruction after the interior pool

Twelve direct callee entries execute their original machine code, along with their short tails/helpers: course identifier construction, stage-type comparison wrapper, stage path wrapper, layout/placement/actor-info construction and setup, resource getters, position getter, and the camera aspect provider. The aspect provider performs its real VFP subtraction/division. Fixed-string termination at `0x0039E0E4` also executes unchanged. The report lists every real and modeled entry.

Heavy resource, scene, constructor and allocation behavior is explicitly modeled. Tests vary normal/special paths, signed negative/zero/positive counts, independently varying preload/final counts up to 32, stage-type values including unsigned out-of-range values, source indices, scenario values, selection state, position bits including negative zero, viewport ratios, all 14 root allocation failure positions, and a callback that changes the scene's camera pointer. Models deliberately clobber caller-saved registers and s0–s15. The source's ordinary local storage is distinct from scene/heap data; arbitrary input/stack overlap is not covered. NaN/infinite viewport values, FP trap modes, extreme iteration counts, cyclic subsystem behavior and arbitrary retained stack pointers are not established. Each execution has a 200,000-instruction ceiling; no fixture reaches it. This is bounded caller equivalence, not whole-scene, GPU, rendering or gameplay evidence.

The first null-camera fixture exposed a harness mistake: comparing callee-saved registers at the fault PC. Return ABI obligations apply to normal returns, not interrupted frames. The harness now keeps those checks on normal returns and compares fault behavior separately. No source, original bytes or expected call behavior was changed to fix that harness issue.

## Preservation against the pristine gate

The coordinator's pristine `86104d96a7` baseline passed a clean build and all 767 prior roots / 787 actual definitions in 596.502 seconds. Baseline report SHA-256 is `a692f9a29d92c3f8021b62a2254b06fb44776f34cfa2f51849364bde66dee374`.

After this lane's final clean build, all 625 prior tracked file contents remain identical to that frozen base. Every one of the 179 prior canonical C++ objects has identical allocated bytes, symbols and relocations, including objects with no accepted checks. Both checkouts pass the official build-provenance verifier for those objects; recorded compiler identity, flags, dependency hashes and stable-input evidence agree after normalizing the checkout path. This covers every actual accepted definition and its old inputs by equivalence to the already passed baseline. It is not a fresh run of 787 project checks.

The generated C scaffold is reported separately. New U references add 110 generated-source lines, 55 allocated scaffold sections and 198 symbols. No prior allocated section or symbol is removed or changed. Its nonallocated debug-frame relocation grouping changes as new functions are generated; this object is not claimed to be byte-identical or accepted C++ source. The scaffold has no allocated relocation sections. No scaffold source or build tool is committed. Full per-input/per-object evidence is in `course-select-scene-init/preservation.json`; the reproduction script reports this distinction explicitly.

## Reproduction and seals

Run from the frozen-base worktree after applying this family unchanged, with the owner's verified private EU inputs and approved compiler/tool symlinks present:

```sh
. ./development_environment.sh
export DEVKITARM=/usr
python project/dot_reports/course-select-scene-init/verify.py --output build/course-scene-final --clean
python project/dot_reports/course-select-scene-init/replay.py --directory build/course-scene-final
python project/dot_reports/course-select-scene-init/preserve.py --baseline /path/to/pristine-861-worktree --output build/course-scene-final
```

The baseline path must contain the coordinator's passed `build/dot-baseline-861/report.json` and its fresh canonical objects. The scripts derive imports from unchanged map rows and verify source provenance. They store generated binaries only under ignored build storage. `fixtures.json` contains synthetic fixture data; the original executable, compiler binaries, generated objects and linked images are not part of this proposal.

Final source SHA-256: `c0b308b4db931357e9b191c1cc839039a2a9ebee038962ea44baddb6c42eb29d`.

Unchanged checker SHA-256: `e177e0abe130e645e75c5f0dc24abb19dbe83310de55f1f099ec8fc385e07317`. Unchanged exact-byte implementation SHA-256: `aef81a3a21c49bb17b8a19f139d02607899257e04dbe461ece2992cc726fc2e2`.

Final compiler object SHA-256: `fb6faff260b9121f990e0546353d6b198999afe3acd2c412271732fd9485a59a`. Diagnostic body SHA-256: `9086e14f1511f166643020e015a50612b7cdf27164dbeacee57dfe974aede8f3`.

`course-select-scene-init/results.json` seals compiler, linker, wibo, checker, configuration, scripts and fixture hashes, exact check output, attempts, measured timings and verification summaries. At the original verification checkpoint, publication was held because repository visibility conflicted with the then-current private-only brief. The owner later lifted that hold as recorded below. Canonical acceptance remains with the integrator.

## Publication status — 2026-10-02 07:52 UTC owner decision

The owner confirmed that `exgota/super-mario-3d-land-browser` is public and authorized publication of source/header/report proposals on `dot/*`. The previous visibility hold is lifted under the updated `AGENTS.md`, `project/BRIEF.md` rule 13 and `project/DOT_BRIEF.md` at `56e9b4b89fe3c5d391c2948f1ce2b2c50706638e`. Game files, extracted assets, credentials, private download URLs and leaked SDK material remain excluded. Earlier private/publication-hold wording describes the historical checkpoint.

This remains a proposal for the integrator with zero exact-match claims. All build, checker, replay and preservation evidence above concerns frozen base `86104d96a7f570383bbfefc5fffad998e134496c`; it is not current-main acceptance. The recorded failures, replay limits and unresolved type/identity constraints still apply. This publication correction changes notes only: source/header blobs and retained objects are unchanged, and no build or replay was repeated. Only the integrator may accept the proposal, set committed ranks, write the ledger or move `main`.
