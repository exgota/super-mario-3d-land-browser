# Resource construction shared context signature prerequisite

Branch: dot/root-2b89f4. Base: 3d69bf00a769676c77f07465163def56a559b48f.
Target: 002B89F4, 556 bytes, U. Exact claims: none.
At entry, this root forwards its r2 input unchanged to base constructor0022C8EC. It preserves that input, later dereferences it at +0x8, and copies its first sixteen bytes for resource-binding construction.
This independently establishes pointer/context use, rather than an integer capacity, for the forwarded value in this caller.
Accepted Factory group_00140B54.cpp instead declares fn_0022C8EC as private Storage*(Storage*,const char*,int), used by rank-O wrapper002B9378.
A new context-pointer declaration would conflict with that accepted shared contract. No pointer-to-integer adapter, invented alias, or Factory change is proposed.
Integrator-owned declaration reconciliation is required before an ordinary typed implementation. The context's original name and complete layout remain unresolved.
Dispatch row003D8898 is present; no data-row or table-boundary claim. No source, build, canonical check, or replay was attempted, and no runtime failure/demotion is claimed.
