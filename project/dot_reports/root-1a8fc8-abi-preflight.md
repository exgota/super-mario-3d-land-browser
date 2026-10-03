# Actor state update needs one entry/stub contract

Branch: dot/root-1a8fc8. Base: 64d1bb9f3c97318481de9baa08de473608b9e042.
Target 001A8FC8..001A921C, 596 bytes, U. No source forms or build/check claims.
The root dereferences its actor-state pointer and uses actor, pose and nerve interfaces.
Accepted Factory/group_001A0348.cpp declares this exact C-linkage entry as uint32_t fn_001A8FC8(uint32_t), while group_0036B90C.cpp declares a two-argument void*-returning form over void* parameters.
Both forward nerve-keeper host words, but these source signatures disagree with each other. No established ordinary public class-method alias was found in current Game/lib/header inspection.
Choosing one placeholder or synthesizing a return value would not reconcile the accepted callers; the body alone does not establish the wrappers' return contract.
Stop under the accepted-contract exclusion; the integrator owns the shared entry/stub repair.
The quaternion unit and directly referenced nerve rows are present. No accepted source, Factory, map, tools, ranks, configuration, ledger or STATE changed. No exact credit.
