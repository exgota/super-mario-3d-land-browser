# Shader lookup and mask identity evidence

This is a separate, unapplied metadata proposal for main. It does not add a
map row, allocate BSS, rename a function, change a boundary or establish exact
source credit. Frozen base: `754f99a30a337756df5aa01c28b4ed6a977fecd5`.
The only oracle was the owner's verified EU executable, SHA256
`e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.

## Correct producer and independent consumer

`__shv_initializeShaderValidator`, `00108690..00108F74`, consumes the register
lookup. It does not produce the lookup or exclusion masks. Its pool word at
`00108F70` contains `00420F4C`; instruction `00108E08` loads that base into ip.
Instruction `00108E4C` reads `[ip + index*4]`. The loop initializes index to zero,
increments at `00108E60` and stops at 189 (`00108E64`). No instruction in this
whole root stores through the lookup base; no exclusion-mask base occurs.

The actual producer is `_shm_initializeShaderManager`, `001064E4..00106B74`.
At `0010655C` it loads the pair table at `003A421C`; `00106570` loads the lookup
base `00420F4C`. Each iteration reads an index/register pair, writes the register
with `00106580: STR lr,[r3,r2,LSL#2]`, and tests the next index against -1.
Reading the verified input gives exactly 189 pairs, exactly one occurrence of
each index 0 through 188. The sentinel pair starts at `003A4804` and ends at
`003A480C`. This source pair table crosses existing map rows; no source-table
boundary edit is proposed here.

Together the producer's complete index set and the independent initializer's
bounded reads establish 756 accessed bytes at `00420F4C..00421240`.
Full/partial validators are additional lookup consumers at `0037B8D0` and
`00377FD0`; the initializer evidence does not depend on their reconstructed
headers or their earlier ownership claims.

## Three independently based exclusion masks

The same manager loads mask bases at `001069D0`, `00106A28` and `00106ABC`.
Each is first cleared by a three-iteration loop writing two words per iteration.
The following loops use `index >> 5` and `1 << (index & 31)` to set bits from
sentinel-terminated source lists. All list indices are in 0..188.

| Accessed range | Size | Input list(s) | Non-sentinel entries |
| --- | ---: | --- | ---: |
| `00421240..00421258` | 24 | `003E2E50` | 23 |
| `00421258..00421270` | 24 | `003E2EB0`, then `003E2EC8` | 5 + 5 |
| `00421270..00421288` | 24 | `003E2EE0` | 156 |

Full-validator literal words at `0037D614/18/1C` independently refer to those
three bases. Its six-word filtering loops consume the same spans. Lookup and
masks are contiguous, but each mask has its own producer base and six-word clear.
This supports four separate observed data ranges; it does not recover original
source-level variable names, linkage, translation-unit ownership or allocation
syntax. The descriptive symbol `dat_00420F4C` is an address-derived proposal.

## Original execution cross-check

The evidence script executes the untouched original manager four times, with
initial lookup/mask bytes filled with 00, 5A, A6 and FF. All four runs return
normally and restore the ARM callee-saved registers and stack. They perform
396 writes covering exactly the 828-byte union `00420F4C..00421288`; the eight
bytes immediately after it remain unchanged. The resulting union equals the
pair/list-derived expectation in each run and has SHA256
`6b6f7f6179d3505726d2de7e3a7168f7d451f44b9095c13b21115f5495827c76`.
This checks observed write extents, not whether an undocumented owner allocates
a larger encompassing structure. Adjacent source bytes are not uploaded.

The current map contains none of these BSS rows. Main can review neutral data
identities with the four ranges above separately from the source proposal.
The initializer imports only `dat_00420F4C[189]`; it deliberately leaves that
reference unresolved under the current canonical map. No absolute-address
accessor is substituted in submitted source. Diagnostic replay explicitly binds
the missing symbol outside the normal build/checker and makes no closure claim.

## Correction to prior proposal comments

The earlier `shv_ValidatorTail.h` comment in the full, partial and graphics
integration proposals attributed lookup/mask initialization to `00108690`.
That comment was incorrect. The coordinator corrected the comments and report
qualifications after receiving the instruction-address evidence above. This
proposal does not modify those headers or claim their whole-root matches.

Reproduction and measured results are in
`shader-validator-initializer/metadata_audit.py` and `metadata.json`.
