# Capped Byaml component and metadata readers

Baseline is `a4f1579f9f0f13dacfe91d92501cf74154c131ad` (the 618-root main checkpoint). Work is on `dot/byaml-component-metadata`; the source result is commit `53b7c81`. The worktree is `/workspace/scratch/73cdb2c524af/mario-byaml-component`.

## Result

Four ordinary C++ reconstructions are retained under `NON_MATCHING` in the existing `lib/al/src/Placement/alPlacementFunction.cpp`. There are **zero exact additions**. All four canonical `tools/check.py --object` invocations return exit 1 and `U -> m: The linked candidate differs from the unchanged original interval.` The complete root sections have their expected sizes.

| Root | Complete bytes | Different bytes | Linked complete-interval SHA256 |
|---|---:|---:|---|
| fn_002253C8 | 152 | 18 | 4d5d748dbf42e729a95a61201c56ca32d8b477eab8a2aec3290994777f88ac73 |
| fn_0025BE14 | 192 | 12 | e14c4816a4ff45e2b4f938951f4734bb2ddc5283799de565b2b46c4705fa05dc |
| fn_00278C6C | 160 | 9 | 6b5c74293aa496241efdbc09c1e9387533974357bee844314ef332a447d05269 |
| fn_0032B89C | 64 | 8 | 1c278f6ff2a40872dea453d291b468a8e4afb2b79be56496137a5576a7dd9c14 |

The first three reproduce the packet's best 152/192/160-byte source behavior and copy-scheduling discrepancy. PowerUpItemNum retains its existing 64-byte/8-difference result: the output-pointer and PC-relative key preparations are exchanged. No instruction padding, fabricated helper ABI, volatile access, assembly, altered compiler flags or binary editing is used. All external calls bind to the established retail imports. The private pair helper disappears from the final root; no unknown callee survives.

`python make.py eu` builds, links and exports with committed source under the unchanged ARMCC 4.1/791 module configuration. A final `python make.py eu -ca` also completes successfully with the baseline map restored; the rebuilt object has the identical SHA256 and passes the unchanged provenance verifier. Canonical object checks preserve **54/54 previously accepted roots defined in this translation unit**. This is a focused preservation check, not a fresh audit of all 618 roots. Neither shared vector nor Byaml headers changed.

## Bounded source search

The packet already records four unsuccessful forms per target. This session compiled two candidate forms per root, then one final consolidation of their best bodies; it did not run a broad permutation search.

1. Recreated the best float component source inside its actual existing translation unit, rather than an isolated scratch translation unit. It gives the identical packet differences: 18/12/9 bytes. For PowerUpItemNum, used the already accepted `readSceneMetadataInteger` helper. That form hoists the default initialization and produces 56 bytes, so it fails the complete interval size check.
2. Tested ordinary POD float-component aggregates without changing the shared vector class, and an explicit metadata presence branch. XYZ grows to 156/196 bytes and retains an extra preserved register for its aggregate address; XY is 160 bytes/15 differences. The metadata reader returns to 64 bytes/8 differences.
3. Final source restores the first float bodies and retains the second metadata body, with explicit NonMatching guards. This is consolidation, not a third new hypothesis. It is committed, rebuilt, and checked again.

This closes the actual-translation-unit hypothesis for these bodies. The accepted metadata layout and helper are already present on main; these results do not justify changing the global vector layout or introducing a guessed API merely to move the last copy.

## Real-callee differential execution

The replay passes **752 constructed-BYAML cases, 1,504 ARM executions and 825,764 executed instructions**. Counts are 231 direct XYZ, 235 nested XYZ, 235 nested XY and 51 PowerUpItemNum. It compares the complete returned integer, output guard bytes, input buffer and metadata record between the unchanged retail root and the canonical source-generated root linked at its original address. It also checks SP restoration and bounds every non-stack write to the expected output extent.

Every lookup is real retail execution. The replay never intercepts or fabricates a callee return. The actual float lookup at `0x278C30`, integer lookup at `0x27E068`, data lookup at `0x28CAB4`, nested iterator lookup at `0x290FB0`, iterator constructor at `0x2910E8` and strcmp at `0x28AA60` all execute. The local machine loads the owner's verified retail executable into emulator memory, then overlays only the selected canonical compiled root for the source side; this does not modify the original executable file. No game data is committed.

The cases cover all 216 three-component combinations of absent, float, integer, Boolean, string and null entries; partial/all lookup success; invalid iterators; valid iterators with null roots; missing/wrong-typed outer entries; null/array outer entries; all tested integer extremes; positive and negative zero; subnormals; infinities; quiet and signaling NaN bit patterns. Component reads execute without short circuiting, and float payload bits are copied unchanged.

A useful distinction is independently confirmed: a present outer null or array value produces a valid iterator, after which component lookups fail and the vector is overwritten with zeros. A missing or integer outer value fails the outer lookup and preserves the vector. PowerUpItemNum uses 2 when its iterator pointer is null, its iterator is invalid, the key is missing, or the stored value is not an integer; every actual integer value, including negative bit patterns, is returned without clamping.

The aggregate output digest is `10d5d135b9a5e9e2ff6a4db1ee9032fbcf4c4e9cf51838b4899114727cf16fe9`. The final source objects are the same ones used for the canonical mismatch and 54-root preservation checks. This is bounded execution over constructed valid buffers, not arbitrary-buffer safety, an original asset-loading replay, hardware floating-point validation or gameplay equivalence.

## Identities and reproducibility

- Retail code: `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`
- Source SHA256: `28852dda531806dff679994e25685500d3f37e5ca4a1878d406d7ead8dc81015`
- Final ARMCC object SHA256: `f4c177637abac340cd8bae8f3be70d629e8ab8c3ef34d44e48f266db96df56b7`
- `tools/check.py`: `e177e0abe130e645e75c5f0dc24abb19dbe83310de55f1f099ec8fc385e07317`
- `tools/low/checkExactBytes.py`: `aef81a3a21c49bb17b8a19f139d02607899257e04dbe461ece2992cc726fc2e2`
- `tools/low/buildProvenance.py`: `343d1a0954aba9a4805e71420be9a99d91185db4c8b329d1363d11efcee53529`
- Baseline map SHA256: `ac7eef756278c86c41472bab66291afed57b6bcbed2336eb2b75aae157ff7cca`
- Vector header SHA256: `08547526705c7b2ee17679aaa95911d04a21a5fb85d886bfbfa8655b3a382a6e`
- Byaml header SHA256: `3ceeca89571abeb56431268928af2c13cf412debf7bc751a0b46ffeeabf6a4c4`

For local canonical checks only, the four original unnamed function rows were temporarily given their neutral `fn_` names. Their boundaries and pools were unchanged; only the check tool set the resulting mismatch ranks. The baseline map is restored before delivery. No map, tool, configuration, ledger, STATE or binary changes are in this branch.

The complete reproducer and original-address build/check setup are in [the replay recipe](byaml-component-metadata/replay_recipe.md). Ignored per-attempt sources/objects, canonical JSON, linker outputs and the replay result remain in `build/dot_byaml_components`. The timed session began at 2026-10-01 20:36 UTC; the final clean build finished at 20:47 UTC. The saved replay recipe was executed successfully from its Markdown code blocks before that clean build. These are shared session times, not separate per-root labor measurements.
