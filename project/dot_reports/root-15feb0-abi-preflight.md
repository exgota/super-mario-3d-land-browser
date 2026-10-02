# Actor state boundary requires shared declaration review

Branch: dot/root-15feb0. Base: f8e79c19657c5dd4d57631b6456bd353e8fe1726.
Target: 0015FEB0, 532 bytes, rank U. Exact claims: none.
Accepted 8-byte caller: 0015FEA8, rank O, in Game/backup/src/Factory/group_00152284.cpp.
Its current callee declaration is `unsigned int fn_0015FEB0(unsigned int)`.
The target treats argument0 as an actor/object pointer and passes it through actor/nerve interfaces; a typed reconstruction must preserve that pointer contract.
The existing integer placeholder declaration is a C++ declaration-level compatibility issue, not a measured failure of the accepted ARM call sequence.
The complete return contract and any unused extra argument require reconciliation; no guessed return value or placeholder-compatible false signature is supplied here.
Factory source is integrator-owned and remains untouched. No demotion, shared-source repair, build, or checker claim.
No missing data row is asserted. Park this root until the integrator establishes one consistent caller/callee declaration and revalidates the accepted caller.
No replay or broad preservation gate was run.
