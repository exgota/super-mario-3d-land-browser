# Resource/particle family on frozen main 8991

This is a bounded NonMatching integration proposal on `dot/resource-particle-family-8991`, based on `8991bec8cb27ff1a2dac0003f059d270a0b17493`. Source freezes at `be5bee8bc7d698e6d3a98160861b6c5a736cf754`. It adds three isolated SDK translation units and two private headers; no existing main source/header, map, boundary, type, rank, ledger, STATE, configuration, tool or original input changes are submitted. No algorithm or layout was changed and no additional code-generation form was attempted. New exact roots, exact bytes and accepted bytes/hour remain **zero**.

## Frozen inputs and independent histories

The caller package was read from local `mario-resource-298c58` at `0a3ea889700c86b90551aabbd90ab29d4e55d829`; its final source originally froze at `499daf8`. Its immutable published head is `0ee18252c9d108987d6da68a9a321dec10aa2509` on `dot/resource-construction-298c58`, as verified by the publishing coordinator.

The particle package was read from local `mario-particle-shape-2a451c` at `d65414a32464f5d68cf920d6fdfb6c66a884e645`, source freeze `bdfc1a3`. The coordinator verified published head `d1ef5caf319e44eb332b26f48c6ba43ff7e4eac3` on `dot/particle-shape-2a451c`, with exact tree `f860f03654163af6835f917648d0a845f8023888` equal to the local package. Both independent branches and their attempt histories remain unchanged. This branch starts directly from frozen main 8991 and does not rebase or replace them.

The original EU executable hash is `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`; exheader hash is `94e61359c80498495dd77bb2df16f0def6fca94fca736dc8c3c929279b44d2c8`. Configured ARMCC 4.0/902 hash is `e6aca4ebc280a54065406bcc3a0ae3d9f4d0ac5a4032c3a193adb836c72e70fe`. The manifest records full original/final source hashes and unchanged checker/script hashes. Private binary and toolchain files stay ignored and uncommitted.

## One C++ declaration and the retail ABI

The previous caller declared `extern "C" Bindings* fn_002A451C(Object*, void*, Allocator*, void*, void*)` using TU-private `resource298c58` types. The callee defined `extern "C" void* fn_002A451C(void*, const void*, void*, void*, void*)`. Those declarations have the same observed machine calling convention but incompatible C++ types.

New private `retail/ResourceParticleFamily.h` declares the callee's existing opaque signature, and both caller and callee include it. The caller's private declaration is removed and its return value is explicitly converted with `static_cast<Bindings*>`. The callee definition is unchanged. The other private header, both sizing definitions, layouts, arithmetic, branches, imports and data identities are copied unchanged. The header makes the exact declaration visible at both the call and definition; the normal compiler accepts both.

Retail 002995A8..002995CC independently establishes the boundary: the resource pointer is loaded and field+30 resolved into r1; the object allocator is loaded into r2; outer argument 5 becomes r3; outer argument 6 is stored at stack[0]; newly constructed object r7 becomes r0; BL 002A451C executes at 002995CC. Retail 002995D0 uses r0 as the returned nullable binding pointer. Thus owner/resource/object allocator/stream allocator/ParticleShape occupy r0/r1/r2/r3/stack[0], with the return in r0. Conversions to void pointers and back add no arithmetic or ownership operation. Adding const to the resource view grants no new mutation. This establishes the private interface, not an original class or method name.

The allocator dispatch+8/+C, binding swap+D0, enabled+D1 and pointer pairs+E4/+E8 remain the independently recovered structural views. ParticleShape's +30 swap and +1F8/+1FC buffers remain unchanged. No claim of complete original class identity, arbitrary alias safety or native portability follows from making the function declaration compatible.

## Unchanged compiled family

Every allocated section byte and attribute, every relocation resolved to symbol identity, and every non-file symbol compares equal before/after integration for all three normal project objects. Both sides pass the unchanged project provenance verifier against their own committed sources and configured compiler. Raw object hashes differ; path-bearing file/comment records are outside this comparison, and raw object equality is not asserted.

The four substantive definitions are fn_00298C58 (2,984 bytes), fn_002A451C (2,692 bytes), AddVertexStreamSize (60 bytes) and AddVertexParamSize (32 bytes). The particle object's local zero-size `__switch$$` markers are recorded and preserved separately; they are jump-table labels, not additional source helper functions. No separately emitted source helper survives. Import identities, tables, literal pools and data references remain unchanged. The codegen JSON contains all definitions, imports, allocated-section hashes and original/final object hashes.

Fresh canonical checks of the committed-source objects preserve original Type f and complete boundaries: caller 00298C58 is 2,984/2,928 bytes; particle 002A451C is 2,692/2,796 bytes; stream sizing 0022F554 is 60/60 bytes with 31 differing bytes; parameter sizing 0022F590 is 32/32 bytes with 10 differing bytes. The two roots reject on complete extent; the helpers reject on bytes. The unchanged checker reports:

- fn_00298C58: U -> M: The complete compiled section, including its literal pool, has a different size from the original interval.
- fn_002A451C: U -> M: The complete compiled section, including its literal pool, has a different size from the original interval.
- _ZN2nw3gfx13ParticleShape19AddVertexStreamSizeEjiii: U -> m: The linked candidate differs from the unchanged original interval.
- _ZN2nw3gfx13ParticleShape18AddVertexParamSizeEjii: U -> m: The linked candidate differs from the unchanged original interval.

Only the unchanged project checker produced temporary ranks. The mapped family comprises 5,816 complete retail bytes; the combined compiler sections comprise 5,768 bytes. Neither count is exact-match credit.

## Fresh current-baseline preservation

The normal committed-source clean build compiles 45 Game, 129 Actor and 4 SDK translation units, archives, links and exports successfully. The fresh canonical preservation gate checks **all 717 accepted roots and all 736 actual canonical definitions**, with zero failures, in 713.336953 seconds including the clean build. The complete [gate JSON](resource-particle-family-8991-gate.json) records every source object and checker result. No baseline objects or archives were copied into this worktree.

The unchanged `tools/acceptance_batch.py` uses an empty candidate manifest with prior_checkpoint 8991. Its accepted:true describes the preservation-only empty candidate gate, never acceptance of these four NonMatching functions. This is a fresh full canonical run on the combined committed source, not inherited 709/c2e or 707/45a credit and not object-equivalence extrapolation. The four family functions receive separate canonical checks afterward. The map is restored byte-for-byte after those diagnostic names/checker-owned ranks; all original types, pools and boundaries are retained throughout.

## Caller-to-particle execution

The auxiliary executable links all three unchanged project-generated objects together. It imports only remaining original functions/data, executes fn_00298C58 from the compiled object, and resolves its call directly to compiled fn_002A451C and both compiled sizing helpers. No code bytes or symbols are patched. The original side executes the unmodified owner executable. The diagnostic link is replay evidence, not canonical matching evidence.

The existing caller replay produces **655/655 agreeing pairs: 620 returns, 30 agreeing faults and 5 instruction-budget cases**, in 120.037156 seconds. Its unmodeled-call group has 510 pairs/475 returns, of which **445 returning pairs actually execute compiled caller and compiled particle**; the other 65 pairs stop or return before that call. In this group the compiled caller executes 510 times, particle 445 times and parameter sizing 4,005 times, with zero stream-sizing entries. The corresponding original entry/body counts agree exactly. The remaining 125 binding-provider and 20 initializer-result controls are reported separately in the [summary](resource-particle-family-8991-summary.json).

The 125 binding-provider controls intercept the particle entry on both sides and execute **zero particle bodies or sizing helpers**. Twenty initializer-result controls remain separate; ten reach a particle body and 90 parameter-sizing calls, but their modeled initializer excludes them from the unmodeled-call closure claim. A compiled entry being reached before model interception is not counted as a body execution.

The existing 131 fixtures are unchanged and run at FPSCR 0,00400000,00800000,00C00000 and 01000000. They cover flag/null guards, allocation failures, parent ownership/capacity modes, resource record counts/copy types, nested references and binding-provider outcomes. Their empty attribute resources never enter stream sizing. Root instruction coverage remains 572/730 (78.356%); the 158 uncovered instructions are the previously reported retired-storage destruction, growth/reallocation and null-object fallback paths. Coverage is not all-path proof.

The separately labeled nonempty-attribute integration fixture produces **5/5 returning agreements** in 0.619792 seconds, one per caller FPSCR setting. It executes compiled caller 5 times, particle 5 times, stream sizing 5 times and parameter sizing 40 times, with identical original counts. All four source definitions therefore participate in an observed caller-initiated chain. The existing 445 actual caller/particle closure pairs plus these 5 synthetic pairs total 450 returning closure observations; they are not combined with modeled controls.

The added fixture derives exclusively from recovered layouts already exercised by the independent suites: capacity 0, one non-fixed attribute at index 0, one self-relative attribute-array entry, and an allocator with the observed dispatch slots. It uses a distinct stream-allocator object backed by the same modeled allocation dispatch. Zero capacity is already a returning case in the independent particle replay. The original caller accepts and returns from this composed fixture under all five FPSCR inputs. This is synthetic integration evidence, not a real resource/scene fixture.

Both replay roots run in Unicorn ARM mode with the ARM1176 CPU model and enabled VFP; this is an instruction-level model, not the original hardware. The original executable occupies the mapped 0x100000..0x4FFFFF region, compiled code occupies 0x600000..0x60FFFF, and the modeled stack/control region is 0x700000..0x7FFFFF. Caller tests additionally map 0x800000..0x9FFFFF and 0x1000000..0x10FFFFF; they compare 0x800000..0x88FFFF,0x1000000..0x100FFFF, and each retained allocation at its requested size. Particle tests compare the entire 0x800000..0x9FFFFF arena.

The caller allocator returns deterministic aligned arenas or prescribed failures and records releases. Original constructor/initializer (except explicit controls), recursion, copy/destructor, shape builders/reset/memory functions and command builder execute normally. Hardware endpoints 0028E280 and 0010B2BC remain explicit no-op flush/physical-address models; each endpoint clobbers modeled caller-saved integer registers. Retained allocations, source/owner memory and the observed command-buffer ranges are compared. Three indeterminate padding bytes after each record ownership Boolean are masked; all defined record fields, including the full trailing 28-byte-record flag, remain compared. Freed storage and stack scratch bytes are excluded. Every returning execution has restored r4..r11,d8..d15 and SP canaries and equal FPSCR. Fault controls compare access kind/address/size, preceding external events and FPSCR, not full fault-state equality. The five cyclic-parent cases reach the 500,000-instruction budget on both sides; those are bounded nontermination observations, not successful returns or proofs of termination.

## Particle and helper regressions

With the same three compiled family objects linked, the particle-root regression produces **2,262 returning agreements, 42 agreeing fault pairs and 6 known failing fault-address controls** out of 2,310 pairs in 235.607518 seconds. Actual compiled entries are particle 2,310, parameter sizing 17,202 and stream sizing 3,438, identical to the corresponding original counts. All 628 executable retail particle instructions are covered; the 68 remaining pre-pool words are the six previously enumerated jump tables. No particle case reaches its instruction budget without a reported fault.

The particle suite uses 385 fixtures at each of six FPSCR settings: 0,00400000,00800000,00C00000,01000000,02000000. It includes all 16 indices, fixed/non-fixed types, mixed/reversed/duplicate attribute lists, capacities 0..127 plus 200 seeded random fixtures per setting, owner/slot behavior, swap controls, allocation failures, float bit patterns and explicit null controls. It compares every byte of its 2 MiB writable data arena, allocation/release events, return/stop status, invalid access and FPSCR; returning executions restore integer/VFP callee-saved registers and SP. Stack scratch is excluded. The 100,000-instruction budget is not a general termination proof.

The six null-attribute controls deliberately retain the known divergence: retail reads four bytes at address 4 while the compiled particle reads four bytes at address 0. They are reported as failures, never counted as passes. No algorithm correction was attempted. The valid replay domain excludes null attribute entries. Agreeing faults are distinguished from returning pairs.

Direct helper replay additionally passes 1,848 parameter-size and 24,024 stream-size pairs (25,872 total) in 4.700251 seconds. It uses 12 type values, widths-1/0/1/2/3/4/16, 22 used offsets around 32-byte boundaries and 13 stream counts-8..4096. Both entries are real original/compiled instructions with FPSCR 0 and callee-saved register checks; there are no modeled callees. The output fingerprints equal the independently frozen particle helper report.

Only virtual allocation/release endpoints are modeled in the particle suite. Remaining original AddVertexParam/Stream, reset, memory copy/clear/fill run normally; both sizing helpers execute their compiled definitions. Direct helper replay uses no modeled callees and stays inside signed 32-bit arithmetic bounds. Root invalid non-null offsets, out-of-range indices, arbitrary aliasing, negative/overflow capacities, real heap behavior, arbitrary FPSCR combinations, concurrent mutation and persistent nontermination remain unproven. The caller's unexercised growth/destructor paths remain unvalidated. No native portability, GPU output, real assets, rendering, gameplay or whole-image exactness is claimed.

## Handoff and timing

The source-only patch has SHA256 `fcfa48ceb4febb1edbee6ec1d9a87c63f6cf905303fad38a7bc07738722ba705` and passes `git apply --cached --check` in a fresh temporary index loaded from exact 8991. It adds only the five family files. The full family patch additionally includes reports and executable recipe notes. The coordinator owns publication and later intake.

Assignment began 2026-10-02T01:34:01+00:00; source committed at 2026-10-02T01:35:48Z; verification packaged at 2026-10-02T01:49:28.192008+00:00, a 927.192-second verification window; report review and the final notes commit follow it. It includes source packaging, normal clean build, the fresh full gate, four canonical checks, three-object byte/relocation/definition comparison, combined caller/particle/helper replay, the separately labeled synthetic closure fixture and evidence packaging. Instrumentation was corrected once before caller replay because the diagnostic ELF intentionally omits its symbol table; actual compiled entry addresses are read from the linker map and the ELF entry. No source code or algorithm changed in that correction. No canonical acceptance occurred: 0 accepted complete bytes/hour.

The [full executable recipe](resource-particle-family-8991-replay.md) includes the names-only diagnostic map, normal build/gate commands, canonical checks/restoration, auxiliary link, unchanged-fixture instrumented caller/particle replays, the separately labeled synthetic fixture, helper replay and codegen comparison. Scripts are included as notes, not changes under tools/. All replay images/objects and owner inputs remain local and ignored.

## Exact source hashes

- lib/CtrSDK/sources/AddressResourceFactory.cpp: `2f7779fbc2184d5101b7b17471d5a5d127fd71dcf1e1b45d41ae36ddcd76a975`
- lib/CtrSDK/sources/retail_ParticleBinding.cpp: `fd3764bec403a36674e67c4fcb08eb59d35e76911649aa876d5e88e671493a06`
- lib/CtrSDK/sources/retail_ParticleShapeSize.cpp: `d6ebb737bf4344f302329c960ecc7ad3d63ed1d8bc93ab3bbe8836b55acdd73a`
- lib/CtrSDK/include/retail/ParticleShape.h: `e1104ed6525d6dae9ebaa62657286cabb75b3c49704c388038b56069c24f7d33`
- lib/CtrSDK/include/retail/ResourceParticleFamily.h: `4ee8ec2d9b23cd121e1795850b8549ba21af96a43bb7aba7677ca424253ec808`

Original caller source SHA256: `85d0460855fe94644d96a5ced443fd92cca4905d6c076119f0a74313890e9aa9`. Original particle source SHA256: `94d39656f793974eac59cc0966ce383e9f7c2511ec4bd3ef157e6a0a18ef390f`. The sizing source and ParticleShape header hashes are unchanged. Restored 8991 map SHA256: `2dde00f86cc0bc6f174f3a281853a4faacea1f16b6b628d602bea687b4cdb805`.
