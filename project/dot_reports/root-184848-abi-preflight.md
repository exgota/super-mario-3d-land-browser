# Actor state shared wrapper declaration prerequisite

Branch: dot/root-184848. Base: 713975727c0447ea8cdea708a9ab13cc539a92d0.
Target: 00184848, 548 bytes, U. Exact claims: none.
The target treats its first input as a state-object pointer, reads its actor pointer at +0xC, and uses that actor with existing named actor helpers.
Accepted Factory group_0017C2D0.cpp declares fn_00184848 as unsigned(unsigned,unsigned*) and calls it from rank-O wrapper00184840.
No coherent shared state-pointer declaration is available. A placeholder-compatible integer API or duplicate alternate export would not recover the real class contract.
Integrator-owned Factory declaration reconciliation is required before a typed implementation; the ignored return contract is not asserted from these observations.
This is the same wrapper-declaration issue pattern as the separate1898D4 prerequisite, not a runtime fault or checker-demotion claim.
Direct data references 00430500 and003F35C0 have containing rows. No source/header changes, build, canonical check, or replay were attempted.
