# Native block scheduling and port milestone 1

Root owns this source-only family on root/native-block-scheduling, frozen base 00986a9698620c3e92aa62d3ccda72907552928f. It changes no Game/lib source, map cell, compiler configuration, ledger or Factory definition. Claims are empty and matching credit is zero. All original inputs, generated translations, timing tables, libraries, movies, user state and captures stay ignored.

## Observed exit check

Port milestone 1 passes the complete startup prefix through the first top-screen GPU::SetBufferSwap. Final native captures static_execution_block_schedule_2 and _3 replay the same historical movie and pre-CPU user snapshot used by the corrected stock reference reference_updated_floating_point_control_0. Both return zero. The unchanged strict comparator reports events_exact, payloads_exact, both_complete and passed true, with zero Movie errors. All 528 raw events including ticks match, as do eight PICA command lists totaling 79,936 bytes. Counts are 348 GSP commands, 68 hardware register writes, 102 VBlanks and one swap. Event SHA256 is ed7a39625518e559be6c0bc8f5a50d05f088161bc9256b6a8a169eab114f9859. The final captures also match each other exactly.

Each final native run executes 226,978,430 guest instructions on CPU0 and zero on CPU1, with no interpreter/JIT fallback. Linked original function instruction coverage remains 2,437,712 bytes across 15,874 functions. This is recompilation coverage, not byte-exact decompilation or native source-binding coverage. The prior instruction-timed run's 473 divergent event timestamps are gone. The separate accepted oracle FPSCR correction removed the historical 16 PICA word differences; the earlier report of 18 words was corrected in native_platform_evidence.md.

The observed boundary is startup through the first swap. A visibly presented image, input, audio, World 1-1 replay and browser build remain unverified. Only the clean priority function is linked as an accepted-source replacement. The other rank-O source bindings remain incomplete and do not gain source-port credit from this checkpoint.

## Change and independent checks

The generation helper invokes the pinned public Dynarmic A32 frontend and unmodified Azahar cost function before linking. It retains stock conditions, block cycles, full end/failure/return descriptors and terminal trees. The two scalar FPSCR modes yield 1,285,328 records for 642,664 resumable instruction addresses and 1,290,170 terminal nodes. The inventory equals the linked Entry table, including all 397 Thumb entries. There are 166 explicitly unsupported records. Generated metadata contains no runtime guest decoder or code emitter.

Runtime reproduces block charges after execution, prior-block flushing before SVC, current-block charging after HLE, checked and unchecked links, and the eight-entry return stack. It tracks logical stock-cache visitation rather than treating every statically linked target as stock-visited. It resolves a completed terminal before dispatch lookup, so an expired slice can stop before an unbound next address. Native timing ABI revision 2 refuses old callback ordering.

Seven independent original-stock controls agree with native execution on stop PC, ticks, SVC count and prior-SVC charge. The generic stock observer and static native observer are public source-only tools. The helper's bounded format audit covers 49 records, 33 supported and 16 refused. Four malformed requests refuse. An additional ignored priority audit compares 14 signed boundary inputs against stock original execution, with equal results, flags, PCs, instruction counts and ticks. It observes 60 explicit internal-slice refusals and 22 permitted final-block overshoots, with no fallback. This last diagnostic reads literals from the unchanged original image; its earlier zero-filled-literal fixture mistake was fixed only in the probe.

Fresh native semantic checks preserve startup's 188,695 instructions, registers, flags, first SVC and 1,384,448 writable bytes; 2,048 bounded integer cases; 4,105 priority AAPCS cases; and 88,064 floating-point cases with all VFP words/FPSCR/CPU registers/flags. No interpreter executes in these checks. The full capture additionally proves the final scheduling path. The exact public capture/I/O/native-platform patch sequence applies cleanly to a separate temporary index at the pinned Azahar revision.

## Reproduction and receipts

Follow tools/static_recompiler/NATIVE_PLATFORM.md and azahar_reference/BUILD.md. The reference must use the accepted public ARM64 FPSCR correction and the documented deterministic file delay. Generate translations, derive the schedule before runtime, copy its sidecar beside the sealed timed library, build the native adapter, record/replay a stable reference and supply both its movie and initial snapshot to native execution. Preserve all failed observations. compare_gpu_capture.py accepts only untouched event bytes, payloads, complete boundaries and no movie errors.

Primary ignored inputs/captures live under build/root_port_reference. This worktree's ignored build contains scheduled_capture_2_receipt.json, scheduled_capture_3_receipt.json, their strict comparison reports, scheduled_capture_final_repeat_comparison.json, native_scheduling_report.json, block_scheduled_native/execution_report.json, block_scheduled_native/floating_point_execution_report.json, native_block_schedule_full/block_schedule.json, native_block_schedule/version_two_provenance.json, native_block_schedule/priority_adapter_report.json, schedule_patch_sequence_report.json and native_schedule_source_receipt.json. These receipts bind actual final sources and commands; they are not checked-in game material.

Final host executable SHA256: cb5719b6177d6c19c9852b30dba9efb5efaee2a8f8c1d43985d640500093239c. Final timed library SHA256: e79538983be80ccd0b730fc3985162a6fef299f94bab7bcb8a3a1905541e6131. Schedule SHA256: fbe0bdfdf8479deacac0c113c8d975b0094abe9e2eb8cfa7ce5cbcf9d6a9c10c. Frontend archive SHA256: fbeaf46d4cceae7f1429e73459bfeeb3118b5551520f336216751daca4a57d01. Approved code.bin SHA256 remains e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64. Azahar revision is 662d412123305a9f4be94dd3dc73ddf91a18c55e; Dynarmic revision is e77b1ba0b7da7cbe93021b01a663acfe7c4dd516.

## Explicit limits

The table supports only the two observed scalar FPSCR modes. Unsupported IT/big-endian state, vector modes, exception/interpreter/check-bit terminals and dynamic CPSR changes are refused. Logical visitation does not model stock cache-capacity eviction. Guest executable writes and single stepping are unsupported. The priority source binding is atomic and explicitly refuses a slice ending inside its body. Additional accepted-source bindings need guest-memory/call lowering and resumable scheduling contracts. The Pro answer is an uncompiled proposal. No rendering or gameplay completion follows from GPU command equality.

Final source hashes:

- `runtime/port/NativeBlockSchedule.cpp`: `b499412be9153f9b7809a635cdcf1de83e3c03260451222a686756e3f0a7057c`
- `runtime/port/NativeBlockSchedule.h`: `6dec039ad285eb52b9410c974b56c6a8b4d291945ab8f81b522722554d8a8c45`
- `runtime/port/StaticArmBackend.cpp`: `33f04f07d2e932240e0adb7c24a9ef4a8b379c8a8a5d97eb411fa30808458b38`
- `runtime/port/StaticArmBackend.h`: `7d1f6156fb5505a1cc29ff00acd3f3da2e7d4803bb588c0e0ad610c99e257c44`
- `runtime/port/NativeTiming.h`: `746017dbe1534d399bd0ce9b1eaa1c19722a3d6c590a98943a5a8ca17146c90a`
- `runtime/port/NativeTiming.c`: `ce10474ee98360171a62ad1525ada48c9e11811da5bb30448c77f0643fdc3359`
- `tools/static_recompiler/build_native_block_schedule.py`: `2ea26a64c18f67eafcc8d9953c2b369b86cae802074ec1f7d79cf1048031d540`
- `tools/static_recompiler/derive_native_block_schedule.cpp`: `d59c196f5267e9ce526524a4479fc55885489b21aecbe2e56ae8fdc7e93c7b55`
- `tools/static_recompiler/verify_native_block_scheduling.cpp`: `3849d3f9f7bc638f598336029548b710e7619cd42dc475589110ea1b7b5ebb62`
- `tools/static_recompiler/verify_stock_block_scheduling.cpp`: `a2462401e052563e029ccce24850ddd84eb04dd731cb3b406d0d1071ef53c73e`
- `tools/static_recompiler/instrument_timing.py`: `eca1479d0a86ed3a93f8d9fae2b44f875a4669ecd3a472124ee9838a3d719572`
