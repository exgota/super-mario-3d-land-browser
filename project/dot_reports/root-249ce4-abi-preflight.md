# Collision projection: shared vector prerequisite

- Branch `dot/root-249ce4`; base `6b0e2a1385b814b70344023d9816424edcc7d832`.
- Target `0x00249CE4..0x00249F1C`, 568 bytes, remains U.
- Calls at 249D84 and later projection paths require mapped Vector3CalcCtr<float>::sub at 27CB64; multScalarAdd at 27CCA0 consumes the resulting vectors and scalar projection.
- Published root-1359d4-abi.md and root-15826c-abi.md independently record the current prerequisite: accepted Factory imports of the same mangled vector operations use distinct private vector reference types, while no coherent public Vector3CalcCtr/nn::math::VEC3 declaration exists.
- This target's receiver supplies a collision resource at +0x11C; a compact record supplies indices at +0x6/+0x8/+0xA/+0xC. These observations do not establish the receiver's complete class identity or extent.
- No repeated full ABI audit, new private alias, cast adapter or Factory repair. The integrator must establish the shared vector interface before source work resumes.
- No source/build/check attempt; zero exact claim. Only this short note is included.
