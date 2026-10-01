# Partial shader validator reconstruction

**Integration qualification:** shared graphics C-symbol declarations differ between the state and shader branches. Do not claim combined integration until they are reconciled and regression checked. See [the exact conflict, common layout evidence, and proposed declaration contract](graphics-shared-declarations.md).

Target: `__shv_partialValidateShaderValidator`, EU interval 0x00377FD0..0x0037B818 (14,408 bytes), based on main 5025a6cd5ec8531fb1bc40ff4b570a0c01ef201c. This source is NonMatching; it adds zero exact-byte credit. Compiler configuration is the existing CtrSDK ARMCC 4.0/902 configuration. No map boundary, rank, tool, compiler flag, ledger or state-file change is proposed.

The entry takes a dirty-state pointer and a category mask, returns void, and loads the current control pointer from 0x003E3154. The 16-byte global at 0x003E2E40 contains the current validator pointer at +8 and geometry-mode cache at +12. Validator+0 points to the current shader and +4 caches the shader whose attribute layout was emitted. The direct retail caller is 0x0024CFAC, inside mapped interval 0x0024CEF0..0x0024D0AC. At 0x0024CEFC..0x0024CF00 it loads the same control pointer into r4, then passes r0=r4 at 0x0024CFA4; the real dirty-state argument therefore aliases global control. The caller masks requested categories against control+8 before this call and clears the selected dirty bits and force bits after return. Command cursor and limit are the existing four-byte globals at 0x003E2E30 and 0x003E2E34.

The map's pool marker 0x00378F1C is embedded in the routine. Execution skips the literal data and resumes at 0x00378F78. Other embedded literal blocks also occur. The complete mapped function interval remains unchanged and must be checked in full.

The front source reconstructs scissor bounds, geometry mode changes, shader-code uploads, uniform-program fragments, and twelve input attributes. Attributes are insertion-sorted by signed data address while the maximum-address check is unsigned. Active-array layouts are compared against cached indices, relative addresses, strides and packed formats. The rebuild path stores a persistent linked list, packs up to twelve attribute selectors per loader, encodes inter-attribute padding, and falls back from combined addressing when stride/alignment checks fail. Fixed attributes are emitted after the array inputs.

The tail reconstructs category-selected invalidation, dirty-uniform runs, depth conversion, register shadow emission, six lookup-table cache formats and framebuffer access. Shared ordinary C++ access and tail headers were jointly reconstructed with the full-validator lane from the owner-supplied executable. They contain no copied function instructions, assembled bodies or game-data definitions. Original-name/API recovery remains unproven; offsets retain neutral names.

Partial-only behavior is kept separate: it iterates register-category lists rather than the full validator's dirty-bit sweep. The order is 0x003E2EC8 then 0x003E2EB0 for category 0x10, 0x003E2E50 for category 2 and 0x003E2EE0 for category 0x20. Those existing whole map intervals contain 6, 6, 24 and 157 words respectively, each with a 0xBD sentinel. Partial depth updates also set control dirty bit 0x80000. Partial lookup selectors use distinct whole six-word tables at 0x003A485C and 0x003A4874, even though their contents equal the full entry's tables at 0x003A482C and 0x003A4844. No binary data is added to source.

## Front reconstruction diagnostics

Two front source formulations were built by normal `make.py eu`; both project builds linked/exported. The second removes a compiler-synthesized unmapped memclr helper by retaining explicit stores for the observed loader-clear loop. It contains only the root definition and original command-buffer imports, with no surviving source helper bodies.

An ignored local ARM1176 diagnostic compared original execution from 0x00377FD0 stopped at 0x00379388 against the source-built front-only entry. Both formulations passed 910 fixtures with two invocations each (1,820 prefix comparisons). Fixtures varied category/dirty bits, scissor ranges, program lengths, geometry mode and empty, fixed, mixed, planar, client-memory and interleaved attribute layouts. Shader, control, validator, flags, command-buffer bytes, cursor and global geometry cache snapshots agreed. Actual original command-buffer callees were executed. The first formulation's compiler memclr call was modeled; the second formulation has no such call. This is a prefix diagnostic, not whole-root functional or exact-byte proof.

## Final committed-source result

Final source commit: `b37cf54`. Five distinct source formulations were used, below the eight-attempt cap. The entry is wrapped in the project's `NON_MATCHING` convention. Shared tail SHA-256 is `81ec5643766d16ccb02431da0a36156655a808b949e199c4c9672288f316fa99`; it is identical to the full-validator lane's frozen tail.

A clean `python make.py eu -ca` completed, linked and exported under the unchanged SDK ARMCC 4.0/902 configuration. The canonical command was:

```sh
. ./development_environment.sh
export DEVKITARM=/usr
python tools/check.py __shv_partialValidateShaderValidator --object build/eu/obj/lib/CtrSDK/sources/shv_PartialValidator.o
```

The checker accepted committed C++ provenance and reported: `U -> M: The complete compiled section, including its literal pool, has a different size from the original interval.` The canonical object has one function symbol of 16,260 bytes and a complete root section of **16,276 bytes**, including its 16-byte trailing literal pool, versus the unchanged 14,408-byte target interval. The original report used the symbol size for this comparison; this distinction is corrected here. It has no remaining source helper bodies. The only external function imports are the three original command-buffer helpers and original texture-LUT lookup. Generated object SHA-256: `1f99e7a1c1eec27189c4ae5e3b7a8696eeb7d0c3a0eec1702dd4709c6c146297`. This is an explicitly nonmatching proposal, not an exact match.

The final frozen source passed the same 2,370 complete-root fixtures under two input arrangements: separate dirty/control buffers and the caller-proven dirty/control alias. Each fixture is invoked twice, for 4,740 fixtures across arrangements and 9,480 original/source comparisons, with zero snapshot differences. Every final run entered 0x00377FD0 and returned normally; none stopped the original at an internal boundary. The six suite counts below are per input arrangement; each row passed independently in both arrangements:

| Suite | Fixtures | Original/source comparisons |
| --- | ---: | ---: |
| Scissor, programs and attribute layouts | 910 | 1,820 |
| Uniform runs, registers, depth and framebuffer | 540 | 1,080 |
| Complete LUT conversions and combined states | 440 | 880 |
| Cached LUT partial ranges | 300 | 600 |
| NaN, infinity, subnormal, boundary and tiny-buffer cases | 120 | 240 |
| Allocation callback and newly allocated cache storage | 60 | 120 |

Snapshots compare the shader, control, validator, dirty-state input, complete command storage, cursor and relevant globals; LUT cases also compare all selected LUT storage. Cache, numeric-edge and allocation suites additionally check stack and callee-saved integer register restoration. Allocation fixtures compare callback arguments and allocated bytes. The actual original command-buffer and texture lookup functions execute unchanged. Six FPSCR modes are exercised where stated in the scripts, but FPSCR exception/status-bit equality is not claimed. Register-number BSS and runtime object relationships are controlled fixtures; they do not establish actual game initialization, rendering or replay.

The padding fix has independent retail evidence. The command helper copies an additional source word for even-length payloads. The original partial RGBA workspace starts at SP+4; the two adjacent selector words at SP+0x804/+0x808 come from 0x003A2F6C+8/+12. The gas workspace starts at SP+0x7CC; its adjacent word at SP+0x80C is the previous-upload flag. The C++ workspaces materialize those observed padding words explicitly. The shared unsigned conversion helper checks sign/NaN/overflow bits before the hardware conversion to preserve original results under ARMCC's fast floating mode.

The exact six scripts, deterministic alias transformation, setup recipe and hash manifest are preserved in [the replay appendix](shader-partial-validator-replay.md). Local frozen outputs remain under ignored `build/research/`; the manifest is `build/research/final_manifest.json`. No generated object, disassembly, executable bytes or game data is committed. The EU executable still has SHA-256 `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`. Local checker rank changes are discarded before delivery; root owns acceptance.

Remaining work is byte-exact code generation and coverage outside these bounded synthetic fixtures. The main lane must independently import/build/check this proposal; this branch adds zero accepted exact functions and zero exact bytes.
