# Shared vector operation family

The Codex driver owns this family on `cleanup/vector-operation-family`. Accepted base: `3985a149b6cefcae0a30ef4fbac24d91b59b75e8`. The first subtraction/scaled-add stage remains recorded in `project/evidence/vector_operation_imports.md`; its branch `cleanup/vector-operation-imports` was locally checked but never submitted. This extension carries that stage and uses source commit `35f7dc7bf2838f3fb484d877732056c65d166f04`. Nine source/header files are in the complete family. No new exact function or helper implementation is claimed.

## Normalization interface

Accepted `fn_00279ABC` is a four-byte tail wrapper for the mapped normalizer at `0x0027CCBC`. The normalizer reads three consecutive floats, computes their length in s0, compares it to zero and conditionally writes the three normalized components. It leaves the computed length in s0. The parent independently read both complete original bodies from the unchanged owned EU executable.

The accepted wrapper already used float return and a private vector reference. Its only active source caller instead declared void return and used another private vector pointer. The new shared neutral declaration uses `nn::math::VEC3&` and retains the wrapper's existing float return. The caller continues to ignore the result. This retains a consistent reconstruction contract; it does not prove the original return spelling. A bounded original-call scan did not establish an original caller consuming the result, so no such claim follows.

The caller's private Vec3 now holds a real VEC3 first member. Assignment copies that member, and normalization receives its reference. The unchanged unrelated helpers continue to receive the wrapper. Compile-time checks establish 12-byte vector storage and the existing Motion offsets 12, 24, 36 and 108, with total size 120. No inheritance, reinterpret cast, arithmetic helper body or global compiler change is introduced. `sead::Vector3<float>` remains a separate nominal type; this family does not infer an original inheritance relationship.

The shared `seadVectorCalcCtr.h` now declares normalize beside subtraction and scalar-add. Original private normalization imports disappear from both affected translation units. The stale void signature in `docs/facts/00173698.md` is corrected. Unrelated add/multScalar declarations and the other 24 tail wrappers are unchanged and are not newly certified source contracts.

## Whole-family local checks

The normal project ARMCC build passed on the committed source. The compiled header dependency set contains exactly six caller translation units. Every actual emitted function was mapped and previously O. The canonical checker passes all 31 complete intervals in 14.325939 seconds, preserving 1,820 bytes: the earlier five vector caller roots/1,476 bytes, the normalization caller/244 bytes, and all 25 four-byte wrappers/100 bytes. Complete intervals include their literal pools. The scratch map is restored byte-for-byte to HEAD; no ranks are committed.

The whole normal-object strong duplicate-definition audit reports zero. Only one VEC3 definition exists in tracked Game/lib. Three old literal mangled imports and the incompatible private normalization signatures have zero occurrences across tracked Game/lib. The actual object import remains the expected canonical normalizer symbol, now generated from the named C++ declaration. The new normalizer implementation count is zero.

Independent read-only review found no source-level blocker or required source change. It independently verified all nine committed source/header hashes, all six object hashes, the original normalizer and caller, and the 31-definition coverage. Review receipt sha256: `b1b61ff2586882f56339607ae7acbd0b497e50766cca1451dfc816a534b62b85`.

The integrator still must run its complete prior-root gate. This family remains held while ready dot work waits, because the current cleanup selection can otherwise outrun it. This proposal contributes zero new exact bytes. Local build/checks do not establish acceptance of this cleanup or of the report-only downstream hard target.

## Final source hashes

- `Game/backup/src/Factory/fn_00171BE0.cpp`: `862d81e072179d2823c7eaba8b59d7a18640f910d96e95a868d2cddedeb54a90`
- `Game/backup/src/Factory/fn_00173698.cpp`: `56bd0f864037d1a3d35ecf6461ed85c50a15cb9b09e84b4537f0b3b02c5b65c5`
- `Game/backup/src/Factory/fn_001BAD90.cpp`: `ed4e9d9856c718d2ffab6697be50f9d10a6149fd771ece5a6891ce01d1619720`
- `Game/backup/src/Factory/fn_00355820.cpp`: `89fa5d9adf3d09bf166de92b6c963b95a4981992b2fa6b78a9d6821189d79b03`
- `Game/backup/src/Factory/group_0011D7A8.cpp`: `12092404be39269503075ca8232e692be47e993291760ed248ba13a2ae5124a6`
- `Game/backup/src/Factory/group_002438D4.cpp`: `08eb617f34d87269c88bf0bce67af0894a37df85de3de962c9de03eee3cf497b`
- `lib/CtrSDK/include/nn/math/math_Vector3.h`: `02f79dd26e8182a78a0310c2e14cb4b9f219319bdaf8e25c265a8f8c9b0188fc`
- `lib/al/include/Math/alVectorNormalizationImports.h`: `0d7746251d4449f7cee3658afa3fde028eb84a97d4085eb60066595ec79a7813`
- `lib/sead/include/math/seadVectorCalcCtr.h`: `532e172af28128f287b2ae46d0cd5bb20af2a9e0e7f29970d715c0c3cc4608a1`
