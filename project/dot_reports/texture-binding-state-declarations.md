# Binding-root declaration compatibility

This is a declaration/integration note, separate from the root's matching result.
The root source is frozen at `95940f5796310ca800ff1c649fc8efe2ed14ed04` against main
`86104d96a7f570383bbfefc5fffad998e134496c`. No existing source/header family was edited.

The final read-only scan checks source/header text in 116 local worktrees, with
case-insensitive address spelling, distinct file hashes and worktree heads recorded
in ignored `build/texture-binding-state/declaration-audit.json`. It is local
inventory evidence, not proof that every remote or future proposal was inspected.
Main 861 contains none of the nine imported/defined address-name declarations
used by this family. Thus main's existing genuine C signatures were not replaced.

## Compatible proposed signatures

The completed graphics-integration proposal at
`2fe0650d8e667b59a450d11ae256799722a9562d` uses this shared declaration:

```cpp
namespace retail_graphics { struct RenderControl; }
extern "C" retail_graphics::RenderControl* dat_003E3154;
```

The new binding header forward-declares exactly that incomplete type. The TU
imports the same pointer and converts it to a local observed State view for its
field accesses. It does not define RenderControl or widen the shared header.
The pending graphics ShaderBinary, ProgramLink and validator headers consistently
use the following allocator and zero-fill signatures, also retained here:

```cpp
extern "C" {
extern void* (*dat_003E2654)(unsigned, unsigned, unsigned, unsigned);
void fn_0028D1F0(void*, unsigned);
}
```

`retail_graphics_binding::Allocate` is exactly that function-pointer type; aliases
named Word/u32 in the inspected headers are unsigned int. The callback's third
argument is a zero word here. No pointer ownership semantics are established by
renaming that word.

The completed CFL texture proposal independently declares:

```cpp
extern "C" {
void fn_00282D20(unsigned, unsigned);
void fn_00282650(int, unsigned*);
}
```

Both are preserved. `fn_00282650` was initially declared with a const pointer in
this lane, then corrected before final freezing to avoid introducing a conflicting
C declaration. The initial root also used its private State* as the global pointer
and a pointer third argument in Allocate; those were corrected to the shared
proposed signatures. All corrections are confined to this family's new files.
The generated ARM root section is unchanged after these type corrections.

A host C++ `-fsyntax-only` diagnostic includes the actual pending GraphicsGlobals,
ShaderBinary, ProgramLink and shv_ValidatorAccess headers with the new binding
header and final import declarations. It passes. This checks declaration
compatibility, not target layout, runtime correctness, or combined linking.
Its exact input and command are included in the reproduction note.

## Remaining incompatible historical proposals

These conflicts were found and remain explicit:

- `mario-display-init/lib/CtrSDK/include/retail/DisplayInitializer.h` declares
  `extern "C" retail_display::AllocatorState dat_003E2654`. Its aggregate includes
  allocator/release callbacks, flags and manager pointer. It is incompatible with
  a function-pointer variable at the same C name and crosses existing map rows.
  Keep that family held pending its own map/ownership and shared-declaration plan.
- `mario-20fc34/lib/CtrSDK/sources/gx_TextureUpload.cpp` declares
  `TextureAllocate` as `void* (*)(unsigned int, unsigned int, unsigned int, int)`.
  The signed final parameter differs from the four-unsigned proposed shared type.
  Its owner must resolve signed byte-count behavior before joint integration.
- Older FloatState/IntegerState headers declare 003E3154 as
  `retail_float_state::Control*`. Older TextureStateRoot uses `unsigned char*`;
  older ProgramLink/validator headers use Byte*, an unsigned-char alias. Those
  declarations conflict with the opaque RenderControl* type even though the ARM
  pointer width is identical. The completed graphics-integration proposal already
  changes its participating families to the opaque shared pointer plus local views.
  That does not silently fix all historical branches.

The inspected `fn_0028D1F0(void*, unsigned)` declarations in the effect-resource,
shader, program-link and display proposals are compatible (u32 is unsigned int).
No other source/header declaration of dat_003E3180, fn_00210850, fn_0028B1E0 or
fn_0028B290 was found in this local scan. Their new declarations therefore have no
observed peer-type conflict. They still need explicit review if another provider
or declaration is introduced later.

The new 003E3180 type is an eight-byte row view: a registry pointer and an opaque
word. It introduces no storage definition, row split or claim about the complete
pointed registry allocation. The helper pointer types for fn_00210850 and the
payload constructors express only fields and return values independently observed
in the original binary.

This note does not authorize adopting another family's fragments or overwriting
shared headers. A future combined proposal should use one owner for the agreed
shared declarations, adapt every affected family in an apply-clean patch, and run
its required preservation/replay gates. Current delivery proves only this isolated
family against main 861 and its stated compatibility with the completed proposed
shared signatures.

## Representative inspected file hashes

- `mario-graphics-integration/lib/CtrSDK/include/retail/GraphicsGlobals.h`: `48a02c17d8de9be153d4c21a4bc1a7cdbf1afcd28cd844add35c1e5303031ca5`
- `mario-graphics-integration/lib/CtrSDK/include/retail/ShaderBinary.h`: `b915c8b36fbb2c5a925da8c3c5a2e34e97647b4e67f9dcf9f98a46f36708b0df`
- `mario-graphics-integration/lib/CtrSDK/include/retail/ProgramLink.h`: `b37706e54916ffe2715bf501ad92cf6d21b7846a197b8199facbfd3a9c483f62`
- `mario-graphics-integration/lib/CtrSDK/include/shv_ValidatorAccess.h`: `e6662318ba73a7d96b736c8ca510e7b3c6e0c79335a7638d26f2d5622b54efd4`
- `mario-display-init/lib/CtrSDK/include/retail/DisplayInitializer.h`: `2fd0f0080f0ba716b60124b60c6d250b39354aea62b86f8639d62bd5e31a847f`
- `mario-20fc34/lib/CtrSDK/sources/gx_TextureUpload.cpp`: `5aab23ba7a1dcbd14b478d217c18d9daf1ef949ecc67a60e74e78de0674c8418`
- `mario-cfl-texture-root/lib/CtrSDK/sources/cfl_TextureRoot.cpp`: `d95d2be5784fd429cfb104570ba3dbeff6f53520cc6602b62ea7f739cdb6071d`
