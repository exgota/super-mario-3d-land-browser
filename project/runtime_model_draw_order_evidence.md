# Resource-local initial model draw order

Branch `root/runtime-model-draw-order` reconstructs initial cached model ordering from the owner's original executable and local CGFX members. It adds a separately invoked native interface and a public local command. Matching Game/al sources, compiler flags, map rows, and all original data stay unchanged. This family makes no exact game-function claim.

The complete historical proposal is adopted unchanged: AssetReader.cpp SHA-256 `7ef892c0e9141785fe8841711fcaeb51066f37937088a252e6aeea0def8b96e3`, baseline `0d30436f8efc83a619cd78d2eef533f41c64caa2ba1f2c39c8e3de85e2892ab0`, patch `12cc27b01996f032b3eafd217be5edd6877e22a317bdd00346af9ee7061332a7`. Its source was committed as `3d86009` before fresh root replay. The additional standalone ModelDrawSchedule.cpp command is `53bae75f4f07f64087667c65a604814be8e77525c433977dfe75c3c6e903d975`, committed as `133047b` before its own checks. There is no copied game payload or instruction array in either source.

Original execution of the cached-model initializer, comparator, and mesh/material/shape visibility consumer establishes the supported path. The initializer sorts by the material's low order byte, mesh order byte, and unsigned bytes of the case-folded material name. Its nested pair-exchange loop can move equal keys indirectly; a stable standard-library sort would change behavior. Original C-locale tolower establishes ASCII folding for all 256 input bytes. The visibility consumer treats the node index as signed 16-bit. A negative index uses the mesh's visibility byte. A nonnegative index needs caller node state, which this interface does not invent.

The source validates catalog identity and every model, list, mesh, material, shape, name, and DATA/IMAG extent before returning a descriptor. Callers must explicitly provide both initialized/unmodified resource material identity and serialized mesh-list identity. Missing identities, material overrides, and replaced mesh lists retain unavailable status and an empty order. Any unresolved node-dependent decision leaves the complete visible sequence unavailable, even when some individual entries are known.

## Fresh root results

Root prepared an absent directory with exactly 20 frozen support files plus the complete committed AssetReader.cpp. No prior reports, fixtures, or executables were copied. All 8,678 historical sealed paths, including owner data, verify before and after the new execution. The original executable retains SHA-256 `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`; original code instructions and callees are unchanged. Controlled metadata fixtures are private modified copies.

Each sanitized debug and optimized native build agrees with original execution and independent raw-field decoding on 39 owner models / 137 meshes. Fifteen models have an order different from serialized ordinal order. Eight controlled models / 36 meshes cover sorting and alias behavior. Checks include 255 nonzero original case-fold outputs, 24 signed-index controls, 6 unavailable controls, 2 unsupported controls, 38 malformed refusals, and 4 missing/override path controls. Complete debug and optimized output agrees.

Prior material replay preserves 127 materials / 4,320 packet words, 64 caller controls / 1,952 words, 142 synthetic cases / 3,548 words, and all availability/alignment refusals. Prior command replay preserves 1,514 streams / 11,900 packets / 131,076 words / 3,456 uploads, plus 240 original copy cases. Full previous interface replay passes, including fixed-history 10,025 cases / 11,272 encoded words, inline values, raw uniforms, context, face/root/palette, bounded retail math, scene assembly, and earlier CLI outputs. The existing 1,083 missing owner fixed values remain unavailable.

The committed public command additionally agrees byte-for-byte with the fresh validated native driver in 235 cases per build, across 47 local members/controlled fixtures and five path-identity combinations. Six malformed/argument/input controls return failure with zero partial stdout. It buffers the report until every model succeeds. Every compiler and replay stderr is empty on successful runs.

Fresh timing: original draw reference 1.462 seconds; native draw validation 25.213 seconds. Subsequent preservation commands total 202.815 seconds, including material reference 2.236, material native 65.278, command reference 3.219, command native 14.977, and prior interfaces 117.103. Nested regressions are included, not added again. These observations establish tested component parity, not sustained throughput.

Local mechanical evidence: build/root_runtime_model_draw_order/{draw_schedule_reference_report.json,draw_schedule_native_report.json,material_native_report.json,native_report.json,preservation_report.json,regression_report.json,public_command_report.json,seal_before.json,seal_after.json,preservation_commands.json}. The copied validators and exact invocations remain beside those reports. Historical failure directories remain untouched.

## Local command

Build from public source, then pass an uncompressed local CGFX member and explicit path identities:

```sh
clang++ -std=c++20 -ffp-contract=off -Wall -Wextra -Werror -O3 runtime/ModelDrawSchedule.cpp -o build/model_draw_schedule
build/model_draw_schedule <local.bcmdl> true true
```

Each identity accepts `true`, `false`, or `missing`. Only pass true when the caller has established that runtime path. JSON names are encoded byte strings, so host locale and text decoding cannot change comparison. Keep asset members and output under ignored data/build paths. Existing AssetReader command modes retain their previous output.

## Limits

This is initial resource-local scheduling, before active actor, camera/frustum, node-state, scene submission, shader arithmetic, GPU state, final vertices, or rendering. No level has run and no differential gameplay replay has passed. The native command is available locally; browser execution remains unverified. Emscripten is not installed on the inspected PATH. Integrator acceptance is pending and adds no matching byte credit.

## Integrator follow-up

The original root-runtime-model-draw-order-7a2fcaef0 submission was rejected at 05:08:52 ET on project/STATE.md only. The revision branch starts from current main and changes the state handoff. AssetReader and ModelDrawSchedule content remain at the exact verified hashes above. The complete source and mechanical evidence are unchanged; no fragments were combined. No acceptance is inferred from the earlier local checks.
