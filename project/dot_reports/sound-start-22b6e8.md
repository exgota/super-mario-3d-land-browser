# Sound start root 0022B6E8

NonMatching reconstruction only. The unchanged checker rejects all three committed forms by complete section size. The final source compiles to 2224 bytes against the unchanged 2364-byte interval; accepted complete bytes and accepted roots are both zero. The final bounded replay passes 862 returning original/candidate pairs. Three invalid controls are excluded, with the precise differences below. This is neither audio playback validation nor gameplay evidence.

## Frozen ownership and source

Base `754f99a30a337756df5aa01c28b4ed6a977fecd5`; branch `dot/sound-start-22b6e8`. Only new `lib/al/src/Audio/SoundArchiveStart.cpp` and these notes belong to this proposal. The source uses the existing `lib/al` Actor module and ARMCC 4.1 build 791. This is the established module/default compiler, not proof of this unnamed library root's original compiler. No 894 was available or installed, and no 902 comparison was attempted. Shared source/header files, config, map, tools, ledger, STATE and binary inputs remain untouched. No data/ABI prerequisite, table split or address alias is proposed.

The symbol is the existing row's neutral address spelling `fn_0022B6E8`. Its original class and method remain unidentified. The archive-backed sound-start interpretation is inferred from independent caller wrappers 002B9BA4, 002B9C88 and 002BC730; archive type-specific metadata, priority-sorted allocation, sound-command generation and final handle binding corroborate it. The field names sequence, stream and wave are descriptive deductions from the three metadata/preparation paths, not imported SDK names. All implementation evidence came from the owner's verified EU executable and existing clean project sources/tools. No public or removed SDK source was used.

Root: 0x0022B6E8..0x0022C024, including the four-byte literal pool at 0x0022C020 (`0x3C010204`, rounded binary32 1/127). The source's inline allocation/preparation helpers produce no separate compiled function entry: actual compiled closure is this one 2224-byte root. Its 28 undefined imports all resolve to genuine existing unchanged function rows. The final standard-map link succeeds with the target still U; that scaffold link does not establish that this source body is linked into the normal image. Strict object checking and the separate diagnostic link operate on the actual source-generated object.

## Attempts and timing

Investigation began 2026-10-02 03:31:02 UTC. Three source forms were committed before their normal project builds and strict object checks:

| UTC source commit time | Commit | Meaningful form | Complete compiled bytes | Strict result |
| --- | --- | --- | ---: | --- |
| 03:35:56 | 1d53b432bd5d91230c75b7b1a870b4f951e27ae1 | Typed pool allocation and attachment together | 2148 | U -> M, size mismatch |
| 03:37:53 | 64458befdd5f465cc112654ab51aaa68ef9ffdf6 | Separate instance allocation/attachment and stream preparation | 2224 | U -> M, size mismatch |
| 03:50:28 | 626fc34b6c73011db1f53ca1309ff82b53fe5fdd | Full-word hold argument, grounded by 002BC730 forwarding a word without boolean normalization | 2224 | U -> M, size mismatch |

The third form is a substantive ABI correction and counts as a source form even though its object bytes equal form two. The register-preserving wrappers also load the flag as a signed byte in two paths; the root tests the whole incoming word for zero. Final replay explicitly covers 2, 127, 128, 255, 256, 0xffffffff and 0x80000000. No fourth form was spent without a credible new structural hypothesis. Size movement is not exact-byte improvement. The target's inlined list empty/iterator checks and stack organization still differ. No strict byte-distance number is claimed because the checker stops at size mismatch.

The final clean project build (`. ./development_environment.sh; python make.py eu -ca`) started at 03:50:28 UTC and completed before the 03:51:52 UTC final checker/replay batch. Source and compiler files stayed fixed afterward. Logs remain in ignored `build/sound-start/`. The exact checker output for every form is:

```text
U -> M: The complete compiled section, including its literal pool, has a different size from the original interval.
```

Each check exits 1. Only a temporary Symbol-column spelling was supplied for the already-existing unnamed root row; the entire map was restored byte for byte in a finally block. No Rank/Type/boundary/pool changes are in this proposal. The source is guarded by the project `NON_MATCHING` convention. The interval remains U in the submitted tree.

## Recovered behavior and layout limits

The player view has archive at +4, 72-byte player slots through the pointer at +0x24, then paired active/free lists for type 1 at +0x28, type 3 at +0x40 and type 2 at +0x58. A list is a count followed by a two-pointer sentinel. Allocation uses intrusive nodes at sound +0xD4, checks a combined priority saturated to 0..127, stops the lowest-priority active sound when needed, initializes a free instance, and inserts it in priority order. The source's Sound size assertion describes only the recovered 0xDC-byte prefix, not the complete original object.

The options flags independently select start offset/type, player ID, priority, actor-player index and sequence override data. The player index masks its high byte; an actor index outside 0..3 returns 14. Priority addition/subtraction explicitly wraps in unsigned 32-bit arithmetic before signed interpretation and clamping. The sound priority field itself is a byte. Preparation return words for types 1 and 3 are masked to their low byte. Type 2 emits queue commands and has an always-zero successful status. Allocation and error cleanup have separate observable paths; final success optionally records the actor, restores hold priority and binds the handle.

Partial structs and function-pointer slots represent independently observed offsets/ABI use only. They do not establish SDK class identities, full sizes, ownership outside this root, or source compatibility with an unseen implementation. There are no competing existing declarations of the 28 address imports in this tree.

## Bounded whole-root replay

The final source-built object is independently relinked at 0x0022B6E8 using the unchanged checker's normal isolation helper and established import rows, after strict checking has already rejected its size. The compiler object is never edited. The diagnostic image has one 2224-byte allocated code section. It supplies the complete root on the candidate side; all reached non-modeled callees execute from the unmodified original binary on both sides. The decoder/emulator code and exact reproduction commands are in [sound-start-22b6e8-replay.md](sound-start-22b6e8-replay.md).

862 returning pairs pass: 711 primary cases, including 600 seeded combinations, plus 151 extended cases. The extended set covers restricted handle/options and handle/actor aliasing, all four FP rounding modes, flush/default-NaN and sticky-status settings, archive optional metadata, and the 21 nonboolean hold cases. The ordinary set covers null/not-ready/missing archives, invalid IDs/types, handle detach, actor/player admission rejection, empty pools, priority replacement, list ordering, signed wrap/clamp edges, all options bits, ambient callback/allocator success and failure, type-specific metadata failure, preparation failure/status truncation, rollback and hold paths.

Observed return counts across those 862 passing pairs: 0: 509, 1: 44, 2: 2, 3: 12, 4: 2, 5: 2, 7: 2, 8: 2, 9: 2, 11: 2, 13: 123, 14: 97, 15: 2, 52: 2, 255: 59. These status counts include explicitly modeled preparation responses.

Each passing pair compares return value, exact modeled-boundary arguments, every byte of the 393216-byte fixture region, the original 512-byte command-manager state, full FPSCR, restored SP and preserved r4-r11/d8-d15. An emulator write guard rejects any executed store outside the observed fixture/manager/stack ranges. Stack scratch and padding are excluded from memory equivalence, and arbitrary aliasing is not established. The maximum observed stack depth is 216 original / 232 candidate bytes. The primary set's maximum instruction counts are 1362 / 1340; the 20000-instruction budget is a failure limit, not a termination proof. Total instructions across all returning pairs are 806496 original / 792743 candidate. These are emulator instruction counts, not ARM hardware cycles.

The original root visits 584 of its 590 instruction addresses. The six unvisited instructions at 0x0022BF28..0x0022BF40 implement cleanup after the stream status initialized to zero in r6; all intervening conforming calls preserve that register. No return/status/register was forged to claim coverage of this branch.

The only modeled whole-function boundaries are resource preparation at 002BCAF8 and 002BC8C0. Each receives real original-reader output; the fixture records all defined fields and arguments, writes a fixed observable marker and supplies the selected status. Those preparations, audio resource loading/decoding and their downstream behavior remain unverified. Six synthetic vtable endpoints model initialize, release, engine-address query, priority-reorder notification, ambient-priority callback and ambient allocation. Initialize clears the tested sound prefix; release performs fixture list detach/free effects; reorder only records its invocation. These synthetic addresses exist only in the emulator, never in the C++ imports or canonical map. Capacity failure after eligibility and invalidated metadata are deliberate fixture mutations at call boundaries, not claims about real concurrent scheduling.

Retail archive lookup/metadata conversion, list erase/insert, allocation stop, global/actor admission and final list attachment, priority update, handle operations, ambient memcpy, command-ring allocation/append, stream command construction and volume conversion execute as original code. The original mapped callee entries reached, excluding the root and two modeled preparation entries, are:

`0x002149F4` `0x00214A2C` `0x00214A60` `0x0022ABC0` `0x0022AC0C` `0x0022AC18` `0x0022AC28` `0x0022AED0` `0x0022B188` `0x0022B1E0` `0x0022B204` `0x0022B58C` `0x0022B5E4` `0x0022C024` `0x00232390` `0x002323E8` `0x0024C8C0` `0x0028BA44` `0x0028F0A0` `0x002B9E34` `0x002B9EA0` `0x002B9EC8` `0x002B9F40` `0x002B9FE4` `0x002BA0C4` `0x002BD7DC` `0x002BD8B8` `0x002BD8E8` `0x002BD928` `0x002BE7AC` `0x002BE80C` `0x002C0E28` `0x002C0F58` `0x002C11BC` `0x0033F4CC` `0x0033F4F4` `0x0033F684` `0x0033F6E0` `0x0033F700` `0x0033F728` `0x0033F7D4` `0x0033F7F8` `0x0033F844` `0x0033F890` `0x0033F8B8` `0x0033F8C4` `0x0033F8EC` `0x0033F8F8` `0x0033FF54` `0x0033FF5C` `0x00340064` `0x0034006C` `0x003401B8` `0x003401C0` `0x00340278` `0x00340280` `0x0034042C` `0x00340434`

Three deliberately invalid controls are outside the 862 passes. A malformed active-list cycle exhausts 3000 instructions on both runs, at original PC 0x0022B9C4 versus candidate PC 0x0022B940. Null free-list nodes fault at 0x0022C028 in both runs; type 2 takes 479 original / 476 candidate instructions and type 3 takes 484 / 480. The driver reports their fault/budget outcomes and traces only; post-failure memory was not compared, so these are not equivalent-state or safe-recovery claims. There are zero faults/budget exhaustions among the 862 passing cases. Concurrent access, invalid object/vtable pointers, arbitrary list corruption, arbitrary overlap, asynchronous hardware behavior and physical audio output remain unverified.

## Preservation and reproducibility

The coordinator's pristine-base gate is `mario-main754/build/dot-baseline-754/report.json`, SHA256 `c208f39fadf49e66c5c98c97b445881ba2e51ade07b5724610cf0309175ec028`: clean build exit 0, 733 roots/all 753 actual accepted definitions pass, 582.169390522 seconds. This lane did not duplicate that 753-check run.

After the final clean build, every one of the baseline's 178 canonical Game/lib C++ objects was compared, including objects with no accepted-definition entry. Allocated section bytes/flags/alignment/sizes, ARM attributes, all substantive symbols and relocation records, normalized build commands/compiler hashes and every provenance input agree. All 370 baseline input hashes and 469 tracked Game/lib/tools/config/map blob files agree; the asm-differ gitlink is unchanged. The only extra object is SoundArchiveStart.o. The first strict raw-symbol comparison correctly reported 178 absolute STT_FILE path differences between workspaces; the final comparison verifies and normalizes only that known prefix, one file symbol per object. It does not normalize function symbols, bytes or relocations. The raw diagnostic failure is retained locally. The final preservation report has zero object/input/tracked-source differences. This is exhaustive equivalence to the proven baseline, explicitly not a second full canonical acceptance gate.

All proposal sources/notes apply unchanged to frozen 754f99a. No publication was performed by this lane. Zero accepted bytes means zero accepted-byte throughput throughout this investigation; replay and clean linking add no M2 credit. The remaining packet is [0022B6E8.md](../pro_requests/0022B6E8.md).

## Frozen hashes

- `lib/al/src/Audio/SoundArchiveStart.cpp`: `50a808b2c293d6d398a4f19a338b9762a715057e014d811db62a45dcb322d775`
- `build/eu/obj/lib/al/src/Audio/SoundArchiveStart.o`: `bfb1413ebcfc77876841e1ea8bb48473a71775fef0c8d28be328cf5d14a81439`
- `build/sound-start/candidate.bin`: `95baab7363a066ac708f3af9bb0d0c7b025293fc85635bbd2431144772abab93`
- `data/ver/eu/code.bin`: `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`
- `data/ver/eu/map.csv`: `b70e4f37358ebc14aa0c0d522c83172901f23998510a9a2a4e1a36d6cd811881`
- `data/config.json`: `5d41e617eb5b26374ed60b5383a0940ce0ce7d046344f7f815b7564144822c45`
- `tools/check.py`: `e177e0abe130e645e75c5f0dc24abb19dbe83310de55f1f099ec8fc385e07317`
- `tools/low/checkExactBytes.py`: `aef81a3a21c49bb17b8a19f139d02607899257e04dbe461ece2992cc726fc2e2`
- `tools/low/buildProvenance.py`: `343d1a0954aba9a4805e71420be9a99d91185db4c8b329d1363d11efcee53529`
- `data/compilers/4.1/791/bin/armcc.exe`: `d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d`
- `data/compilers/4.1/791/bin/armlink.exe`: `b9cffb71d7b58f3fd33fa8c4c8af884b75689bb5a9e9014309a08146bba676dc`
- `data/compilers/wibo`: `aee836280b3d7c80c2031410219c907c9824c11be6b128d0744655de0f22280b`

Compiler flags below are copied from final project provenance; include, preinclude, output, dependency and source paths use this isolated worktree. No flags changed:

```text
-DVERSION=EU -DNN_SWITCH_DISABLE_ASSERT_WARNING_FOR_SDK=1 -DNN_SWITCH_DISABLE_DEBUG_PRINT_FOR_SDK=1 -DNON_MATCHING=1 --cpu=MPCore --fpmode=fast --apcs=/interwork --depend_format=unix_quoted --diag_suppress=1608 --arm_only --no_exceptions --diag_style=gnu --arm_only --no_exceptions --diag_style=gnu --signed_chars --dollar --force_new_nothrow --no_rtti --no_debug_macros --no_depend_system_headers -O3 -Otime --gnu --split_sections --force_new_nothrow --multibyte_chars --enum_is_int --signed_chars --no_rtti_data --forceinline --remove_unneeded_entities --no_debug --preinclude=/workspace/scratch/73cdb2c524af/mario-root-22b6e8/Game/project_globals.h --sys_include -c -D__BASE_FILE_NAME__="SoundArchiveStart.cpp" -o --depend
```

Evidence sealing time: 2026-10-02T03:59:44.800456+00:00, 28.713 wall minutes since assignment, including investigation, source/build/check iterations, replay, preservation and report preparation. Publication/intake time is not included and no acceptance has occurred.
