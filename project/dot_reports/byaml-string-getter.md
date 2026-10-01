# Byaml string getter: bounded semantic evidence, no new exact match

Branch: `dot/byaml-string-getter`. Source/base head: `3de056e0dcb33619c32f8b46ef64f74c90d28934`. The source and header stay byte-for-byte unchanged. This branch adds notes only; root owns acceptance and publication.

## Outcome

The unchanged project build (`python make.py eu`, ARMCC 4.1/791) started without a build/eu directory, clean-built, linked and exported. The documented recipe was then extracted verbatim and rerun successfully; its retained build.log records that later incremental build. The strict canonical checks on its committed-source `alByamlStringTableIter.o` report:

- `_ZNK2al20ByamlStringTableIter9getStringEi`, entire `0x0028CA30..0x0028CA48`: `m -> m: The linked candidate differs from the unchanged original interval.` Exit 1, 24 bytes, 15 different bytes
- `_ZNK2al20ByamlStringTableIter15findStringIndexEPKc`, entire `0x0028B518..0x0028B584`: `O -> O: The complete source-generated function interval matches byte for byte.` Exit 0, all 108 bytes preserved

No source form was added. The four forms in `project/pro_requests/0028CA30.md` remain the complete cumulative search. No map, ledger, STATE, flags, tools, source, or headers changed. No Pro-reserved target was inspected or changed. This work adds zero exact bytes and makes no new source-intake proposal.

## Grounded hypothesis review

The constructor stores one pointer at iterator offset zero. The accepted lookup reads its count from the high 24 bits of that pointed-to header and accesses u32 relative offsets after the four-byte header. Original callers at `0x0029101C` (string value by key) and `0x0033753C` (data and key-name by index) independently construct the one-pointer iterator from the file header's string-table or hash-key-table offset, then call the getter with the payload/key index. Their caller/callee chains were exercised intact below.

This supports the current class and serialized layout. There is no new evidence for a wrapper, alternate member type, or expanded class layout. Local offset/base spelling is already exhausted by the packet. Further register-allocation or local-order permutations would not be a grounded source hypothesis, so none was compiled. The remaining mismatch is the existing arithmetic/load scheduling and temporary-register allocation described by the packet.

## Bounded original-machine replay

The complete [reproduction recipe](byaml-string-getter-replay.md) executes 3,208 constructed cases as 6,416 paired ARM1176 runs (1,168,984 total instructions). One side uses the unchanged verified EU executable. The other overlays only the 24-byte canonical getter plus the canonical accepted 108-byte lookup, at their original addresses. Every other caller and callee is actual original code; no callee is stubbed or replaced. The accepted lookup overlay is independently full-interval equal before execution.

All pairs agree with each other and the expected return values/output changes. The harness compares all 64 KiB of input/output memory, output-write extents, the stack pointer, and callee-saved registers. Direct getter runs also assert the exact two memory reads (iterator pointer and indexed offset word) and absence of writes. Scratch registers and condition flags are not caller contracts and are not compared.

- 1,143 direct getter cases: eight counts (1, 2, 3, 7, 16, 31, 64, 257), all valid indices, three physical layouts with padding/reverse string storage and varied aligned table locations
- 15 direct alias/empty-string cases
- 1,239 complete accepted-lookup cases, including actual original `strcmp`, plus one empty-table lookup
- 336 original string-by-key caller cases over seven data tags and three layouts
- 336 original data-and-key caller cases over the same tags/layouts
- 63 missing-key cases, 63 invalid pair-index cases and 12 invalid-root caller cases

The strings include empty strings, long common prefixes, and high bytes. Missing keys, wrong value tags and guarded invalid pair indices/roots leave outputs unchanged. Getter entry is actually reached in 3,084 executions across the full run; lookup in 3,278; original `strcmp` in 3,276. These are executions visiting the entry, not dynamic call counts.

Result digest: `9ecacc664db2c9d9d40130157c8d1d7f958a1238807232a3c6aa05a6b7f2b154`.

Scope: constructed serialized BYAML buffers and explicit caller guards only. No malformed getter-index contract, unmapped-input safety, original-asset ingestion, concurrent mutation, performance, whole-game behavior, or gameplay is established. This semantic evidence does not change the `m` result or the strict exact-byte count. Build/replay wall times were not measured for this pass.

## Frozen hashes

- EU executable: `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`
- Canonical getter interval: `403e01035d63d72e733a43fa345285a1dabf649d3335b68097d02688d15d7ef3`
- Retail getter interval: `86c4c404be666ca12416c4e3379d5bfc0a66ba69872ed471e67d511c653e8c7d`
- Canonical/retail lookup interval: `60c8973d0f7f1441bc4485cd6958d3af4dc52d58ba234280bbf60b08addf11d2`
- Unlinked lookup section: `74fae41a7c3bf8b7caf37cf33a65553ce6802c6e681e5ab6df4e5df441e4e4d7`; its sole relocation is offset 64, `R_ARM_CALL` (28), `strcmp`
- Whole canonical object: `e93120a7323982005ed92851e75c91ac7605cd8ae4b428b3c88bfedf20e2f477`
- Unchanged map: `514c3ec6f2343e8a40fc73996df78fc95b2e33f4897fbdda6bb0311df6cf40fa`

Local ignored evidence: `build/dot_byaml_string_getter/{build.log,canonical_results.json,getString_check.log,findStringIndex_check.log,replay_result.json}` plus checker-generated candidate directories. Generated objects, game bytes and candidate AXFs are not committed.
