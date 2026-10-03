# Connected guest execution observation evidence

`root/guest-execution-observation` adds an optional finite native diagnostic window around an externally selected guest address. Root independently built the final source and compared three actual native replays against the preserved original Azahar movie. All three full comparisons pass.

Ownership was recorded at `c9df36ac`, before implementation, against frozen `origin/main` `80c334855e991839cd1f4ca69d30babe5b73f74e`. Scope is the observer, backend/native frontend integration, separate CMake patch and documentation. The scheduler, translated module, original writer observation, matching inputs, map, ranks, ledger and oracle remain unchanged. Matching claims/newly translated bytes are zero. Existing linked translated bytes: 2437712.

## Final source and isolated build

| Final source | SHA256 |
| --- | --- |
| `runtime/port/GuestExecutionObservation.h` | `c292636caa2cc07a89b120369bbd9a8ec2e730b3baaaaca7601d1970dee63c7c` |
| `runtime/port/GuestExecutionObservation.cpp` | `de28f8fb34ba6e99bc9c2f51b6971819996dcf40066ed1ed82df4e5251fb8850` |
| `runtime/port/StaticArmBackend.h` | `f68dd1c84c41fdcd5c8e8addbc1477cc1499dcecc063e87c74cb32617728bfb9` |
| `runtime/port/StaticArmBackend.cpp` | `3d693e58670f5ef7adda726877653351461810e599b49a58dfa8104ee9869957` |
| `runtime/port/AzaharStaticExecution.cpp` | `c70c05d4bae641c6979c51372dd349a8070c1e1e80614f6a7c155ca23f4c5d2a` |
| `tools/static_recompiler/azahar_reference/azahar_guest_execution_observation.patch` | `1477882585e847870fe75efb7ebf137dfa18cf63e0ef6606ed341e72fe259191` |
| `tools/static_recompiler/azahar_reference/GUEST_EXECUTION_OBSERVATION.md` | `af11d7bfbc7e61f06e0af7f52389de78ae75a217a79a4120d9989a8b6ecbde02` |

The fresh isolated builder is local ignored `build/build_isolated_guest_execution_provider_revision_2.py`, SHA256 `c6880a10e0daa409d588dcc4f2ff9fec3915bd15b0cf93942d211809a5225aaa`. It compiles the changed backend/frontend and new observer, reuses the unchanged writer and six original objects, and links the accepted archives. It checks the separate CMake patch on an isolated copy after the accepted writer patch. Original provider source/build outputs are never overwritten.

`build/guest_execution_provider_revision_2/build_receipt.json`, SHA256 `53677a86dbccdc362b4610e0a43f6a1a1840d7d02dc5afd411cbff9ac95ddd76`, records 11 successful stages, 3457 preserved inputs and 22 sealed outputs. Measured build interval: 7.472301 seconds, after prerequisite identity checks. Native executable: 31833944 bytes, SHA256 `cdc7bbd777d5dd35b9c6638456c29c3f1b5d5a224a8c1a6dc8c634a7882c36c9`. No failed/revision 1 changed object is reused. Installed SDK aliases receive only the receipt's explicit allowance. The compiled-module frontend and implicit system framework/linker closure remain outside this verified native build scope.

Two actual failures informed the final source. The first compile rejected a 64-slot CP15 array because the clean provider exposes 84 slots. Capacity now follows its enum and exact array type/extent. Revision 1's enabled run failed before its selected trigger because repeated invalidating boundaries remained attached to a stale current charge. Revision 2 records the first such boundary, then ends that association. History remains bounded; later outside-charge events are counted gaps. No scheduler, memory operation, timing or limit changed. Failed receipts remain retained: `40196bac6890d9fc739d3f3cd9b8dbe01f1e4b118d40abb9a4a43df2d4f0de4c` and `a6441f24f26a120453b9f0b53075450116ddb6d43aa56b7f72a7ece90afc122d`.

## Actual original-movie checks

Local cases are under `build/guest_execution_preparation/`. Each uses the same original 360-presentation movie and snapshot, independently pinned to the separately recorded original audio reference. Unchanged comparer SHA256: `739a407d7965e7fffd4cfed32cef9ba31bc7866a8f8b1ed960e50967f74434c0`.

| Actual native case | Full comparison SHA256 | Post-comparison preservation SHA256 |
| --- | --- | --- |
| `native_disabled_revision_2_360` | `e60d98ce0d481e6288f796d5b352bb3eabc4268c21e43bea1bf379bbf16486e4` | `a4a4d9a635c928feab1629bb4b17d180cec03556a5235ffa2ce889e0c383fd90` |
| `native_enabled_writer_entry_revision_2_360` | `b129c8c10826df2af18d615253170b4124b29bd5bd367421b4e20566e5218dcf` | `a321d7d4eba8834f0611e96e07baafa698ed5cb555b07fdc6957054c201114a4` |
| `native_unmapped_trigger_revision_2_360` | `8461c3150d9eddf18089d62acf94e0505823e7275548ed7ad0e58e1c7143cf2e` | `c94026a3d850dd13499c4b387d32c7c423406334797d93abadd2e95692ff19f1` |

All comparisons pass input events/polls, GPU events/ticks, PICA payloads, both screens/framebuffers and audio events/PCM. Each executes 393977876 CPU 0 guest instructions; CPU 1 executes zero, both report zero interpreter/JIT fallback. All declared provider/source/module/schedule/dump/movie/snapshot/reference inputs remain equal before execution, afterward and after comparison. Original reference tree SHA256 `0d480ec01a02390b164b037e965116aceeab2ace0f1d2cae6f72e6407c230dde`; original snapshot tree SHA256 `2f6b36e5a6ede765cbc4906a4ccf61c826b0ddc04f9bffa07ae32fd75cbc3597`.

Disabled interval: 54.284196 seconds. Selected window: 273.285651 seconds. Unselected trigger: 462.490479 seconds. These measured diagnostic intervals include finishing identity work on this host, and establish no production performance or per-instruction benchmark. History costs time before a selected trigger. No warmup or timing optimization was introduced.

The selected complete stream is 563688 bytes, SHA256 `5d8ded12d70cfa0dc1b8d3382807506bc76a5c0424cb3ac11d91be3f52003d52`. One completed window contains 49 charged entries, 43 actual callbacks and 194 window records, with no omissions/failed memory operations. Independent original-code proof `connected_instruction_validation.json`, SHA256 `6b2e9dd0bd364c556f1e8f2be2294722ba1116c34df6071d6c84ea67edb4f537`, checks 189 valid instruction RAM samples and retains CPU/context/segment/table/admission/callback chronology. The observed producer/getter/store/successor/read/use chain is queued privately to Pro at `.integrator/pro_queue/layout/world-one-connected-application-slot-window.md`, SHA256 `945ddcae616e6dfe54a342eba16d9224eef39b3ccb23cd8d127ddc0390a130a5`. Root reconstructs no layout or player identity, and does not retrospectively join the older independent writer recording.

The out-of-range trigger completes with zero windows, zero omissions, zero failed memory operations and explicit unselected history. Its 1992-byte stream SHA256 is `a080f7d2ca4c296053a5b33b856bad9d3c7ab137f9d0fa3772df04b7b8e69f5c`. It proves only declared zero-window scope, never a selected function witness.

Fourteen real configuration/output checks pass in `build/guest_execution_controls_revision_2/validation_receipt.json`, SHA256 `98b96e9fbdd1f1579516e0190594e3bf5426d0c4173f98960b0a607b63033068`. They cover missing/invalid/signed/misaligned settings, excessive/insufficient bounds, existing output, linked parents and actual event/byte exhaustion. Every refusal has its intended reason; exhausted streams are incomplete. All protected inputs and the existing marker preserve. An earlier private script's directory-name collision remains an incomplete 11-case run without final control credit.

## Repetition, limits and remaining gaps

Public usage: `tools/static_recompiler/azahar_reference/GUEST_EXECUTION_OBSERVATION.md`. Frozen local replay controller: `build/replay_guest_execution_movie_revision_2.py`, SHA256 `cf7db84cdd67ded35d7dd04cd5f226d5b48cd96aa4341f7b41f24e881181d311`. Post-comparison verifier: `build/verify_guest_execution_preservation_revision_2.py`, SHA256 `c7b4cd9606b9452c81bfced9d62ebb3eeb6f0587b900c3ab069c596dce1fffc9`. Replay to a new ignored case, run the unchanged full comparer with the pinned original reference/movie and new capture, then rehash/seal that case with the verifier. Existing outputs refuse overwrite. Original dump/captures/profiles and generated binaries stay private and ignored.

Full replay caps: 360 presentations, 600/660 seconds child/outer (disabled 120/180), 128 MiB PICA/per file, 2560 MiB monitored total, 720000 files, 15 GiB free floor, two-second polling. Selected observation: 16 prehistory/32 subsequent charge entries, one window, 20000 events and 16 MiB including footer. Callbacks, retained records, contexts and page-table identities are also bounded. These monitored ceilings are not filesystem quotas. Actual controls use shorter finite caps.

Charge entry/callback completion do not prove architectural retirement, saved-register ancestry, uninterrupted mappings/lifetime or a completed gameplay update. HLE/direct memory paths and unsampled remaps remain gaps. An adjacent named map fragment does not establish a typed function's semantic identity. Pro answers remain proposals. Milestones 5/6, complete Section 7 semantic state, continuous audio, saves and complete rank-O address replacements remain open.
