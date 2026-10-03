# Bounded stage capture range evidence

Root owns `root/stage-capture-range`, frozen `origin/main` `cded24cc509242a7373aa281c084b63de77bfae0`. The submitted family permits an explicitly selected finite capture through 8,400 presentations and 3 GiB of PICA payload. The rendered loader domain ends at index 8,399. Its remaining assertions and strict equality policy are unchanged. Defaults, scheduler, backend, memory, translated module, input, audio, state observation and guest execution are unchanged. No matching build input changes, claims or new recompiled bytes. Linked recompiled bytes remain 2,437,712. Only the integrator moves main.

## Submitted source

| Path | SHA-256 |
| --- | --- |
| `tools/static_recompiler/azahar_reference/azahar_gameplay_capture_bounds.patch` | `c96aaf072c882e7f5b8ab1ca4a5fa767687cbf4d1d96357c4bd4fdc7af96b086` |
| `tools/static_recompiler/azahar_reference/STAGE_CAPTURE_RANGE.md` | `3d6ee97928635530cb4e3427b2340496ec6e53cf4770d9203f03538363daa7ce` |
| `tools/static_recompiler/compare_rendered_capture.py` | `db0e6b29ed9e71628f26c9b5596d01c1fec49d45cada99647652797cf3062d20` |

The public range document is the frozen source-preparation record consumed by this build. Its pending-build wording records that earlier preparation stage. The measured build and checks below supersede that preparation status. Historical `GAMEPLAY_CAPTURE_BOUNDS.md` remains the preceding 7,200/2 GiB family.

## Actual isolated build and controls

The isolated build completed with return code zero in 8.888767333 seconds. It sealed 3,373 explicit inputs, retained 34 outputs and 19 successful command receipts, and preserved its original inputs. It compiled new GPU capture and stock/native headless objects, replaced exactly one member in each of two copied sealed archives, and relinked both frontends. All unchanged archive members came from the accepted archive identities. The execution-observer provider is outside this closure. Installed toolchain alias bytes/paths/links are retained. Implicit system linker/runtime closure and the compiled-module frontend are unverified.

The six actual compiled controls passed with original inputs preserved. Stock and native each rejected presentation 8,401 and PICA 3,221,225,473 bytes. Both accepted the exact configured upper selections, 8,400 and 3,221,225,472, with a one-second wall selection, then returned failure with an incomplete wall-time footer. No control claims a completed 8,400-presentation capture. Core files were disabled, individual output files capped at 16 MiB, each direct child bounded to 90 seconds, and the free-space floor kept at 15 GiB.

| Local proof | SHA-256 |
| --- | --- |
| `build/stage_capture_provider/build_receipt.json` | `a35e26b55ba30abedaa7939a3ad6325da0608d6c5ab13280935d2c94f5fff847` |
| `build/build_isolated_stage_capture_provider.py` | `08f4396434ffb3c4c6eb7b331918251b41300313646c3269a855cd7c3181d4a1` |
| `build/stage_capture_source_handoff.json` | `d8dde6fba593897311321c041bc3552c030dae99607124e8a0f8e1aae6d9e503` |
| `build/replay_stage_capture_movie.py` | `e5cb2b48b523f7f58d43ccd44c6d4d41351e0d3daced8961257091b7d60ef02a` |
| `build/verify_stage_capture_preservation.py` | `debd1d5952cae39ecc4db26cfa7d63085401539cd1b08f1fe3b1ebd007267855` |
| `build/check_stage_capture_bounds.py` | `30d3d2a5862c16c86a4806150ddd1608a2c1582cd6db7cd9f4d4cc91cb940dba` |
| `build/stage_capture_preparation/compiled_range_controls/result.json` | `58a893df468b5524be4824f9fdf7e24561b8869bce78dabcd07cd44da638aae4` |

## Actual canonical movie checks

The established original 360-presentation movie and initial snapshot were replayed independently through both new frontends. Native completed in 44.231347542 seconds and stock in 30.172308416 seconds. Native reported CPU0 393,977,876 guest instructions, CPU1 zero, and zero fallback on both. The unchanged full movie comparer passed all GPU events/ticks/PICA payloads, both rendered screens/framebuffers, input polls, audio events and PCM bytes. Both captures had complete footers. Separate post-comparison seals rehashed every declared original/provider/source/library/schedule/movie/snapshot/reference input and passed.

The original movie is `build/root_port_input/reference_scripted_360/input_movie.ctm`, SHA-256 `2c3f2c18a7222b3c6af7e29c9cb64df5f6a7e0b7d11b8c4485b8a25236753898`. Its paired original audio capture tree is `0d480ec01a02390b164b037e965116aceeab2ace0f1d2cae6f72e6407c230dde`; its paired initial snapshot tree is `2f6b36e5a6ede765cbc4906a4ccf61c826b0ddc04f9bffa07ae32fd75cbc3597`. No new capture borrows that movie or initial state without this specific identity check.

| Local proof | SHA-256 |
| --- | --- |
| `build/stage_capture_preparation/native_former_range_360/execution_receipt.json` | `d33e8c621c7d4c572770fccd1b996da61ef33ff4a24f99dc7c438a590f1c77ff` |
| `build/stage_capture_preparation/native_former_range_360/full_movie_comparison.json` | `1ab989626ba8ea4e2c26f923916d8a4d776fb3191d4fbca0568728efdc4237eb` |
| `build/stage_capture_preparation/native_former_range_360/post_comparison_original_preservation.json` | `929bb993ddbb7f2662b114fedb1969fb005ff77172c75a9952a9fe2ed1dd7bd0` |
| `build/stage_capture_preparation/stock_former_range_360/execution_receipt.json` | `23e956a375b9a3174d3a200ef1eee9d764ad5d6ea3cbed832244055b87158e77` |
| `build/stage_capture_preparation/stock_former_range_360/full_movie_comparison.json` | `e192b1167020c69efc2da7d2824262319c9d4f2eba7215efcf99a3a68023d106` |
| `build/stage_capture_preparation/stock_former_range_360/post_comparison_original_preservation.json` | `9b363793d5652e7b2e24870213569dbfcb5e7c3325b54bda10b2451db3236d73` |

Run from the primary repository after `. ./development_environment.sh`. The frozen private builder is `build/root_stage_capture_range/build/build_isolated_stage_capture_provider.py`; it requires its sealed local source handoff. `build/replay_stage_capture_movie.py --help` provides exact paired reference/movie/snapshot selection and finite resource bounds. The execution receipts retain the actual frontend arguments and environment. `tools/static_recompiler/compare_movie_replay.py ORIGINAL REPLAY --movie MOVIE --report REPORT --preview-directory PREVIEWS` reruns the unchanged strict comparison; `build/verify_stage_capture_preservation.py CASE` checks the post-comparison preservation. `build/check_stage_capture_bounds.py` records six compiled controls. Those private tools require absent ignored output paths; retained artifacts stay immutable. All game data, traces, PCM, pixels, generated code and tool binaries remain ignored.

## Storage and remaining work

Owner-authorized cleanup retired only completed own scratch. Every removed file was SHA-256 sealed to a flushed manifest before unlink; inode/device/mtime/size were checked before removal. Original dump/source/active comparison inputs and sealed receipts/hashes were retained. Historical comparisons whose raw payloads were retired require regeneration before rerunning. Six reclamation passes removed 2459099 files, measured 23503396864 allocated bytes before removal. Actual free space at this report is 20044218368 bytes. This measured difference includes other concurrent writers and retained manifests.

| Reclamation receipt, relative to `build/root_scratch_reclamation` | SHA-256 |
| --- | --- |
| `reclamation_receipt.json` | `de23be3b73bcfa775326b2116973dfb191dd01019846fe895beaf2c57a378d0d` |
| `native_7200_retirement/reclamation_receipt.json` | `65034a1228134ffb21481d92bfcbc4cd5425b31180f222560dfd87fb29acff22` |
| `failed_navigation_retirement/reclamation_receipt.json` | `2c3ef12c6c2670201acb1b93654679b682d35b06049d2a88fdc2de815560b6eb` |
| `entry_native_retirement/reclamation_receipt.json` | `6e28a13419f90ed5770f5d2cfe2d8f3175d266072f99d0fad54967295380ebc6` |
| `navigation_native_retirement/reclamation_receipt.json` | `e0f1a386f21f2c77d4c0af7cb42a30beceea44858ebe62b8cb6f051595d622e7` |
| `earlier_replay_retirement/reclamation_receipt.json` | `617cf47a67eabc74af03dcca769d121ffdb900595b488687addc6075aede11fa` |

A prospective World 1-1 continuation will explicitly select 7,800 presentations, 2,400 provider seconds, 2,460 outer seconds, 3 GiB PICA, 3,584 MiB monitored total bytes, 900,000 monitored files, 256 MiB per file, a 15 GiB free-disk floor and two-second polling. Monitored aggregate thresholds are not filesystem quotas. It has not run in this submission. Current accepted 7,200 evidence shows the World 1-1 start card with four lives. Playable stage/control/update/goal, whole Section 7 replay, sustained browser execution, sound continuity, physical/mobile input and complete rank-O replacement adapters remain open. Milestones 5 and 6 remain in progress.

The Pro answer `world-one-connected-application-slot-window.md` arrived. Its proposal accepts invocation-local opaque offset/dataflow and local return-to-dependent-use continuity, based on supplied receipts. It does not establish application/player types, allocation lifetime, unique global write provenance or a completed-update boundary. It did not independently obtain or rehash the private trace. Root will seek upstream identity and update-phase evidence, rather than repeating the connected downstream window. No layout is reconstructed or promoted by this branch.
