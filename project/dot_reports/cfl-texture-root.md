# CFL expression-texture root

## Result and reservation

Branch `dot/cfl-texture-root` reconstructs only unnamed `fn_001119EC`, interval
`0x001119EC..0x0011379C` (7,600 bytes), from main
`57f902421f6874f132d5ea971d1aa5b224c5a528`. STATE was checked at 20:50 UTC on
2026-10-01; local targets 171334 and 1E37D8 were excluded. The original reservation
was commit `88cb9216abc83e5e1ab6faea26af4e49b7a9ee9c`.

The retained, complete ordinary C++ body and explicit field views are guarded
NonMatching proposals. There is no exact match and no full functional-equivalence
claim. The source has no assembly, instruction interpreter, copied function bytes,
or invented callee addresses. The source/helper names are descriptive; no original
name for the root has been established.

Canonical clean `make.py eu -ca` compiles, links and exports with this source.
The final root is 7,684 bytes including its literal pools, versus 7,600 retail.
The unchanged canonical checker rejects its unresolved external data dependency:

```text
Source closure rejected: An unresolved source helper is referenced by a non-branch relocation.
```

The dependency is the observed program-context BSS base `0x004244A8`, absent from
this base map. Its declaration is intentionally unresolved. No substitute address,
new map row, boundary change, checker patch, or rank change is committed. Local
checking named the existing unnamed root `fn_001119EC` without changing its interval
or U rank. The unmodified checker otherwise rejects an unnamed root before checking
with `The object check requires one established function symbol.`

## Attempt history

There were two distinct source forms, within the eight-form cap.

1. `e768289` compiled to 7,700 bytes under ARMCC 4.0/902. Canonical checking rejected
   the unmapped data dependency above. Bounded ARM replay passed its first 13 cases,
   then found an incorrect tiled occupancy scan on sparse 128-pixel input. The
   first draft mistakenly carried the tile index into the pixel counter. At retail
   0x112B74 the occupancy load resets r2 to zero before entry to the sample loop.
2. `d218368` resets the 64-sample counter for each tile. It also replaces pointer
   arithmetic before external objects with existing map-anchored data declarations:
   command template at 3A81CC+18 and draw indices at 3EF070. This form compiles to
   7,684 bytes; the same canonical rejection remains. All inline source helpers
   disappear: the object contains only the root as a nonempty function definition.

The remaining code-generation mismatch is not hidden by the data blocker. For
example, the candidate saves 13 integer registers and uses a 0x614-byte local
frame; retail saves 14 and uses 0x600 bytes. No further forms were spent tuning
code generation while canonical linking is blocked on independently reviewed data.

## Independent binary audit

The executable SHA-256 is the approved
`e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.
The unchanged root interval SHA-256 is
`42272e0476a4c58ae953e3e3b6d91e0fe8f9ce731dd115ba93c5470aa163afe9`.
A fresh control-flow walk, separate from the coordinator's scout, reaches 1,845
instructions / 7,380 code bytes. The skipped literal pools are 111FC4..111FF8
(52 bytes), 112470..1124B8 (72), and 113470..1134D0 (96).

There are 205 direct call sites to 36 distinct addresses, plus ten indirect calls
through the allocator's free callback. Counting the indirect-call bucket as a
37th target obscures the distinction. All-image aligned direct-call scanning finds
only 10EFA8, in named `CFLi_InitCharModel`. Its arguments are output block in r0,
original character-info pointer in r1, model receiver in r2, and resolution/flags
in r3. The output block is then passed to `CFLi_InitResCharModel` at 115B1C.
The caller and root have no recovered public API name for this internal routine.

## Body and layouts

The root clears a 256-byte output and accumulates occupied cells across up to 16
expression textures. It selects size 64/128/256/512/1024 from the resolution value;
60/E0/1E0 request two/three/four mip levels. Bits 5..10 select the staging allocation
size. Resource flag bit 28 disables occupancy, bit 30 selects a color override for
one pair of parts, and bit 31 selects the other override. Occupancy is a 16-by-16
union of nonzero low nibbles in two-byte texture pixels (their alpha/coverage role is inferred). The 64-pixel path
first calls the retail untile routine; larger sizes scan complete 8-by-8 tiles.

`Model+6C` is the resource pointer and `Model+70` is the 16-pointer expression
array. The fields consumed here from the resource are allocation memory kind at
67C and flags at 68C. The character-info copy is exactly 0x120 (288) bytes. The retained
header deliberately uses offset-indexed words rather than pretending to establish
all character-feature names.

A texture descriptor is 0x34 (52) bytes: owner at 00, width/height at 04/08, storage
width/height at 0C/10, level count at 14, additional fields 18/1C, source at 20,
format at 24, pixels at 28, memory kind at 2C, and ownership flag at 30. These fields
are independently visible in 285238 (initialization), 283CC4 (image header adapter),
28820C (release), and the root's inlined release paths. A quad is 0x4C (76) bytes: x, y,
width, height, angle, and mirror occupy 0x18 (24) bytes before its texture descriptor.
There are eight quads, drawn in the recovered order in two passes.

The source preserves the signed bit-pattern comparison used for special minimum
heights. It retains exact binary32 constants using round-trip decimal literals,
the original expression adjustment table, alternate original eye image selection
for expressions 12..15, the 13-entry shared image cache, all command templates,
RAM/GPU readback paths, mip conversion, final memory-kind conversion, and cleanup.
No resource bytes or graphics assets are copied into source.

The direct callees remain genuine mapped imports. In particular, 283D1C is the
image-size/load adapter (null destination queries size); 2848F4/2847BC allocate and
free the temporary images/staging; 283CC4 adapts an 8-byte image header to the
52-byte texture; 283A44 binds/clears a framebuffer; 2841C0 creates a projection
matrix through six VFP float arguments; 283700 consumes a quad; 283FAC consumes
five VFP floats plus the context and two integer flags. 285238 initializes a
texture from ten arguments. The free callback obtained from `nngxGetAllocator`
receives `(memoryKind, 0x101, owner, pixels)`. The source declarations record every
remaining call argument shape; unobserved return values are not assigned semantics.

## The unmapped program context

The exact 4244A8 literal is referenced by named `CFL_SetProgramContext`
(10DB50..10DCE0), `CFL_MakeModelIcon`, `CFLi_InitResCharModel`, this root, and the
shared drawing/projection helpers. `CFL_SetProgramContext` writes the base argument
fields at 00..20, a byte at 14, fields at 24..4C, and the final mode word at 50.
Thus the base identity and program-context role are established, with at least
0x54 (84) bytes of extent; a complete global-object boundary still needs a separate
review before a map entry is accepted. This root reads its mode at +50 and passes
the base to draw/binding calls. It neither reconstructs nor initializes that global.

## Verification limits and reproduction

Final clean-build replay passes all **74 pairs**: 50 baseline cases and 24
additional cases. Both runs compare output bytes, texture descriptors, complete
mip payload hashes, and ordered external events. The additional cases include
39,564 external events, 4,608 quad calls, and 288 absent-image quad calls. The
largest retail run executes 34,129,506 instructions. The diagnostic linked
candidate is 7,684 bytes, SHA-256
`c3ab3e0ca0774805a1edea3df2dcb72dfce426082c226a260961a72f1e9b5fc8`.
These measurements are bounded equivalence under the documented callee models;
they are not exact credit or a full functional validation.

The bounded replay and its exact fixture/model contract are in
[cfl-texture-root-replay.md](cfl-texture-root-replay.md). It executes the complete
retail root and the source-built ARM root. It also executes the unchanged retail
projection and untile helpers, and models resource loading, allocation, GPU work,
readback, and the remaining external calls. Agreement under these models does not
establish actual GPU output, system-service behavior, allocation failures,
invalid/aliased/reentrant inputs, full character-model initialization, or gameplay.
It adds no exact coverage.

Compiler provenance: the configured SDK module uses ARMCC **4.0 build 902**, not
the game's 4.1/791 module. Compiler executable SHA-256:
`e6aca4ebc280a54065406bcc3a0ae3d9f4d0ac5a4032c3a193adb836c72e70fe`.
The canonical build records its command in the object's `.provenance.json`.
No compiler flags or module configuration changed. The locally observed SDK
configuration is not a proof of the retail root's original compiler version.

```sh
. ./development_environment.sh
export DEVKITARM=/usr
python make.py eu -ca
python tools/check.py fn_001119EC --object build/eu/obj/lib/CtrSDK/sources/cfl_TextureRoot.o
```

Only source, headers, and these notes belong to this branch proposal. The map,
ledger, STATE, build scripts, flags, binaries, and local replay products are excluded.
