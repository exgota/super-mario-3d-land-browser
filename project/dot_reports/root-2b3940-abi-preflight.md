# Index command builder shared physical-address signature

Branch: dot/root-2b3940. Base: e2336037d4f709acc082b3322daedadf9500564a.
Target: 002B3940, 544 bytes, U. Exact claims: none.
Accepted Factory group_001012C8.cpp defines nngxGetPhysicalAddr as int() with no arguments and forwards to a likewise zero-argument declaration of the mapped nn::gxlow::CTR::GetPhysicalAddr(unsigned).
This target calls nngxGetPhysicalAddr at 002B3980 and 002B39B4 with an address in r0, then subtracts the physical base from the index-data physical address.
The mapped underlying mangled helper explicitly carries an unsigned argument; the current no-argument C++ definition is not a coherent declaration for these calls.
A typed import would conflict with that accepted definition. Integrator-owned Factory signature reconciliation is required; no alternate alias or untyped callable is introduced.
No source/header changes, build, or canonical check were attempted. No runtime fault, byte mismatch, or demotion claim.
The related local 2B3B60 attribute-reset routine does not call this helper and was not changed.
Data rows 003AF1C0 (12 bytes), 003E2E30, and 003E2E34 exist. Native mesh evidence documents this builder's topology semantics but supplies no source implementation or exact claim.
