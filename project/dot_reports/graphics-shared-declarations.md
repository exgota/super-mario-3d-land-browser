# Shared graphics declarations: integration requirement

The float/integer state branches and shader-validator branches describe the same two retail globals with different C++ external types. The state header declares `dat_003E2E40` as `retail_float_state::ContextSlots` and `dat_003E3154` as `retail_float_state::Control*`; the shader header declares them as `Word[4]` and `Byte*`. Their observed ARM32 storage and loads agree, but these declarations conflict if both headers are included in one translation unit. A combined source integration has not been verified. Per-branch builds and recorded comparisons do not establish combined-header compatibility.

## Common evidence

- The existing whole map row003E2E40..003E2E50 is16 bytes. Both families load its pointer at+8. The pointed object begins with the current shader/state pointer. The validators additionally read and write word+12 as the cached zero/one geometry-enabled state; the state writers leave it opaque.
- The existing whole map row003E3154..003E3158 is one four-byte pointer. State writers observe dirty word+0 and force-shadow byte+0C in the pointed render-control object. Shader callers independently pass this same pointer as their dirty-state argument, and both shader roots have been replayed with that alias.
- These are observed prefixes and accesses. They do not establish the full pointed allocations or original source type names.

## Proposed common declaration contract

Use one shared declaration header and neutral, explicitly descriptive opaque pointer types. For example, on the existing ARM32 target:

```cpp
namespace retail_graphics {
struct ProgramContext;
struct RenderControl;
struct ContextSlots {
    unsigned int unknown0[2];
    ProgramContext* current; // +8
    unsigned int geometryEnabled; // +12, observed validator cache
};
}
extern "C" retail_graphics::ContextSlots dat_003E2E40;
extern "C" retail_graphics::RenderControl* dat_003E3154;
```

The state code can adapt the shared opaque pointers to its independently recovered typed views; shader code can adapt them to byte-access views. Both headers must import the same external declarations instead of redeclaring those symbols. This is a compatible proposed type contract, not a new claim about original names or a committed source rewrite. Require ARM32 size/offset assertions, ordinary project rebuilds, canonical checks of affected roots, and the recorded alias/behavior replay after reconciliation. Do not treat the current separate-branch results as that integration check. Main owns coordination of any production declaration consolidation.
