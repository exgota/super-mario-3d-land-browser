# Full shader-validator reconstruction

**Integration qualification:** shared graphics C-symbol declarations differ between the state and shader branches. Do not claim combined integration until they are reconciled and regression checked. See [the exact conflict, common layout evidence, and proposed declaration contract](graphics-shared-declarations.md).

`dot/shader-full-validator`, based on `5025a6cd5ec8531fb1bc40ff4b570a0c01ef201c`.
Target: `__shv_validateShaderValidator`, unchanged EU interval
`0x0037B8D0..0x0037F0E0`, 14,352 bytes, originally rank U.

This is a complete **NonMatching source proposal**, not an exact match. The
configured SDK compiler is ARMCC 4.0 build 902. The final source checkpoint is
`af79aed`; `make.py eu` compiles, links and exports successfully. The canonical
committed-source object checker reports:

```
U -> M: The complete compiled section, including its literal pool, has a different size from the original interval.
```

The compiler emits one function section of 16,284 bytes, 1,932 bytes larger than
the unchanged interval. No source helper remains outside the root. No exact
bytes or functions are claimed. The checker accepts committed-input provenance
and stops on size before a linked-byte comparison. Its local rank write was
restored; this branch contains no map, rank, ledger, configuration, tool, state,
or binary changes. In this revision of the checker, `--object` writes the rank
even when `-s` is supplied; no checker change was made.

## Source and ownership

- `lib/CtrSDK/sources/shv_FullValidator.cpp` supplies the actual one-argument
  C ABI body under the project's `NON_MATCHING` convention
- `lib/CtrSDK/include/retail/shv_ValidatorFront.h` contains the initial shader,
  scissor and attribute-layout reconstruction developed with the partial lane
- `lib/CtrSDK/include/shv_ValidatorAccess.h` declares observed data/callee ABIs
- `lib/CtrSDK/include/shv_ValidatorTail.h` contains the shared state, uniform,
  register and LUT logic, plus the full-only register traversal

The shared headers are deliberately identical to the partial-validator lane's
final copies. Intake should retain one copy of each. The partial root uses its
own category/list traversal; it must not call the full tail unconditionally.
The accessors express observed offsets without claiming a complete original SDK
class definition. No removed SDK source or implementation reference was used.

## Grounded behavior and reusable evidence

The entry receives a pointer to dirty flags in r0 and returns no value. The
render context is `*0x003E3154`. `0x003E2E40+8` points to the validator state;
its first word points to the current shader. Caller `0x0038E4D4` loads the
context pointer from the same global, passes it in r0 at `0x0038E54C`, and clears
its dirty word after the call. Thus this observed caller aliases flags and
context; a dedicated fixture group checks that alias. A missing shader returns
after the scissor phase. Command cursor/end globals are `0x003E2E30/0x003E2E34`.

The map's pool marker `0x0037C84C` is embedded in the attribute phase. Branches
resume after it and after later pools. Executable paths continue through the
return at `0x0037F0D0`, with final literal words through `0x0037F0E0`. The source
covers the entire root; the interval was not shortened at the first pool.

The front sorts up to twelve enabled arrays by address, caches attribute shape,
coalesces compatible interleaved loaders, encodes fixed attributes, uploads
programs and constants, and emits scissor registers. The full root additionally
clears shader word `+0x4B8` bit 15, invalidates validator word `+0x1010`, and marks
shader byte `+0x3F7`/dirty word `+0x7A8` whenever a geometry shader is active. This
happens before the instruction upload, independently of category bit 4.

The tail uses six dirty words for 189 cached register values. It invalidates
category lists, batches contiguous destination registers and uniform runs,
converts depth values to the observed 24-bit representation, and handles six
LUT cache formats. These include lighting signed-magnitude differences,
texture signed wrapping, color byte tables, fog packed differences, and gas
three-channel tables. It also reconstructs partial cached-range transfers,
allocator calls, no-light handling, and framebuffer-access registers.

The full root reads six-entry lighting tables at `0x003A482C/0x003A4844`; the
partial root reads equal-content tables at `0x003A485C/0x003A4874`. Texture
selectors are the first two words at `0x003A2F6C` for full and words 2/3 for
partial. Existing whole data identities are preserved. The 189-entry register
lookup at `0x00420F4C` and six-word masks at `0x00421240/58/70` do not have map
identities here. They remain explicit data-address references, with no invented
map row, moved interval, copied executable code, or added BSS definition.

The retail `__cb_multiWriteReg` copies an extra source word as packet padding
for an even count. The original color workspace is followed by two selector
words; the gas workspace is followed by the earlier-upload flag. Both are
explicit ordinary C++ data in the proposal. Full color storage starts at sp;
its padding word is sp+0x800. Full gas storage starts at sp+0x7C8; its padding
word is sp+0x808. Partial has the corresponding +4-byte offsets. These are
observed adjacent data values, not arbitrary padding chosen to hide a mismatch.

## Verification and its bounds

The final canonical ARMCC object was linked separately at `0x00600000` only for
behavioral comparison. The original remains byte-for-byte unchanged and runs at
its original address under Unicorn's ARM1176 model. Both whole roots execute to
their real returns. The complete shader/context/validator/flags buffers,
command buffers including padding, cursor, and applicable LUT buffers agree:

| Final whole-root group | Fixtures | Calls per fixture | Matching original/candidate pairs |
| --- | ---: | ---: | ---: |
| Scissor/program/attribute layouts and cache reuse | 910 | 2 | 1,820 |
| Invalidation/uniform/register/depth/LUT/framebuffer combinations | 220 | 2 | 440 |
| NaN, infinity, subnormal, thresholds, partial updates, small buffers, six FPSCR modes | 120 | 2 | 240 |
| Successful modeled allocation callbacks and cache reuse | 60 | 2 | 120 |
| Caller-proven flags/context alias | 220 | 2 | 440 |
| Total | 1,530 | 2 | 3,060 |

All final groups also check stack-pointer and r4-r11 restoration. All command helpers (`__cb_writeRegs`, `__cb_multiWriteReg`,
`__cb_fillRegs`, `__cb_addDummyWrite`) and `__tx_getBoundTextureLut` execute their
untouched original bodies, including their original internal copy routine.
There are no modeled command or texture-lookup stubs. The allocation group alone
models the application callback referenced by `0x003E2654`: it returns distinct
aligned mapped memory for the requested byte count. It does not reproduce the
real allocator, allocator failure, reentrant callbacks, or game initialization.
Other groups preallocate their LUT caches.

The register-number table and exclusion masks are controlled fixtures shared by
both executions. Attribute/program/uniform/LUT contents are controlled data,
not a captured initialized game state. No GPU, rendering, gameplay, complete
initializer, asynchronous mutation, or general functional-equivalence claim
follows from these bounded comparisons. VFP result storage is compared; FPSCR
exception-status equality is not claimed.

Earlier prefix-only and tail-only diagnostics are superseded for the final
counts. They found two useful defects: packet-padding words differed despite
identical caches, and ARMCC fast floating comparisons mishandled NaNs in a
software unsigned-conversion guard. Explicit adjacent data and bit-based NaN
classification fixed those cases. A later front-harness adaptation initially
retained an obsolete memory-clear hook at the newly linked entry; removing that
hook corrected the test setup without changing source.

Five distinct compiled source forms were used, including the packet-padding
repair that initially introduced an unmapped compiler clear helper, its
explicit-store repair, selector parameterization, and the NaN fix. Two complete
committed-root forms were submitted to the canonical checker: 16,004 bytes at
`0445153`, then 16,284 bytes at `af79aed`. No cosmetic retry or per-file flag
change was used. Further exact matching needs control-flow/register allocation
and workspace-layout reconstruction rather than a changed oracle.

## Reproduction

Read the repository setup instructions, activate its environment, then run:

```
python make.py eu
python tools/check.py __shv_validateShaderValidator --object build/eu/obj/lib/CtrSDK/sources/shv_FullValidator.o
```

The checker rank update is local measurement; do not include it when taking this
dot source proposal. The replay appendix contains the original diagnostic
scripts as source, with a bootstrap link step. It requires the authorized
local EU binary and Unicorn/pyelftools and adds no game data to the repository.

Frozen final object SHA-256:
`8da371d5717b57b3807b8a6e753ee6baf8137fc5478ae67687c8862e6decd537`.
Final tail-header SHA-256:
`81ec5643766d16ccb02431da0a36156655a808b949e199c4c9672288f316fa99`.
The original executable retains SHA-256
`e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.
