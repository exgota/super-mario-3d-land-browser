# Actor motion shared vector prerequisite

Branch: dot/root-160770. Base: 3d69bf00a769676c77f07465163def56a559b48f.
Target: 00160770, 556 bytes, U. Exact claims: none.
The target calls fn_00279ABC at 001607F0 with a local three-float vector after obtaining actor direction.
As documented by root-17231c-abi-preflight.md, accepted Factory group_002438D4.cpp defines that wrapper using inaccessible anonymous Vec3&; the newer fn_00173698.cpp additionally declares a distinct private Vec3* and void return.
These are existing shared declaration inconsistencies, not a reason to introduce another incompatible prototype. Reconciliation remains integrator-owned.
Direct data row 003EF238 exists. No missing-row claim or return semantics are inferred from this ignored-result call.
No source, build, canonical check, or replay was attempted. No runtime failure or demotion claim.
