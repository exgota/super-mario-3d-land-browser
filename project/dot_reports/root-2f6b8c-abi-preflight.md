# State motion: shared vector prerequisite

- Branch `dot/root-2f6b8c`; base `6b0e2a1385b814b70344023d9816424edcc7d832`.
- Target `0x002F6B8C..0x002F6DC4`, 568 bytes, remains U.
- Required call at 2F6C44 reaches mapped Vector3CalcCtr<float>::sub at 27CB64, using three-float state/position vectors.
- Published root-1359d4-abi.md and root-15826c-abi.md document the independently confirmed prerequisite: accepted Factory imports use distinct private vector types for the same mangled operation, without a coherent public Vector3CalcCtr/nn::math::VEC3 declaration.
- This target adds another dependent caller. No repeated full ABI audit or private alias/cast adapter is proposed; shared repair remains integrator-owned.
- The root wrapper's void-pointer return declaration is not independently resolved or used as the blocker here.
- No source/build/check attempt or exact claim. Payload is this short note only.
