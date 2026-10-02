# fn_002CFBBC

Written by factory job 6720 (gpt-6.1-sol high, run finished).

Address: 0x002CFBBC

## Class guess

Free player-position helper: high confidence; `void fn_002CFBBC(nn::math::VEC3*)`; output type evidenced by `_ZN2rp12getPlayerPosEv` and `sead::Vector3CalcCtr<float>::add`.

Player component class identity: unknown, low confidence; pointer offsets +0x40 and +0x98.

## Struct offsets

- Output +0x0: `float x`; high confidence, `nn::math::VEC3`.
- Output +0x4: `float y`; high confidence, `nn::math::VEC3`.
- Output +0x8: `float z`; high confidence, `nn::math::VEC3`.
- Player +0x74: `PlayerComponent*`; high confidence.
- PlayerComponent +0x40: `Scale*`; medium confidence, parameter to `fn_001721DC`.
- PlayerComponent +0x98: `ActiveState*`; high confidence.
- ActiveState +0x0: `ActiveStateVTable*`; high confidence.
- ScalarProvider +0x0: `ScalarProviderVTable*`; high confidence.
- Matrix34 +0x4, +0x14, +0x24: `float`; high confidence, second column of `float[3][4]`.

## Callees

- 0x001721DC: `float fn_001721DC(Scale*)`.
- 0x00189160: `Player* rp::getPlayerActor()`; `_ZN2rp14getPlayerActorEv`.
- 0x0026E1DC: `ScalarProvider* fn_0026E1DC()`.
- 0x00277B94: `Matrix34 fn_00277B94(Player*)`; medium confidence, `Matrix34` inferred as `float[3][4]`.
- 0x0027A5CC: `const nn::math::VEC3& rp::getPlayerPos()`; `_ZN2rp12getPlayerPosEv`; pointer/reference distinction uncertain.
- 0x0027CB48: `void sead::Vector3CalcCtr<float>::add(nn::math::VEC3&, const nn::math::VEC3&, const nn::math::VEC3&)`; `_ZN4sead14Vector3CalcCtrIfE3addERN2nn4math4VEC3ERKS4_S7_`.
- 0x0027CB64: `void sead::Vector3CalcCtr<float>::sub(nn::math::VEC3&, const nn::math::VEC3&, const nn::math::VEC3&)`; `_ZN4sead14Vector3CalcCtrIfE3subERN2nn4math4VEC3ERKS4_S7_`.
- 0x0027CC64: `void sead::Vector3CalcCtr<float>::multScalar(nn::math::VEC3&, const nn::math::VEC3&, float)`; `_ZN4sead14Vector3CalcCtrIfE10multScalarERN2nn4math4VEC3ERKS4_f`.

## Vtable slots

- ActiveStateVTable +0x10: `bool ActiveState::active()`; medium confidence, `bool` or `int` return.
- ScalarProviderVTable +0x0: `float ScalarProvider::value()`; high confidence.

## Data references

- 0x004305D4: `dat_004305D4`, `const nn::math::VEC3`; high confidence, source argument to `sead::Vector3CalcCtr<float>::multScalar`.
