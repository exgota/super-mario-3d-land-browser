# PackunFlower initialization shared declaration prerequisite

Branch: dot/root-13d41c. Base: e2336037d4f709acc082b3322daedadf9500564a.
Target: 0013D41C, 544 bytes, U. Exact claims: none.
The root selects PackunFlowerDokan/PackunFlower archives and forwards the same ActorInitInfo argument to public al initialization/argument helpers.
At 0013D530 it passes a local integer output and that info object to unnamed helper 0027D1DC.
Accepted Factory fn_0018938C.cpp declares that helper void(int*, const anonymous-namespace ActorInitInfo*), and likewise declares fn_00280538 against its private info type.
Current public placement headers provide float/bool argument overloads but no shared declaration for this integer helper.
A new typed al::ActorInitInfo declaration would conflict with the accepted caller's inaccessible type; a local duplicate type or placeholder signature would not resolve it.
Integrator-owned declaration reconciliation is needed before a coherent new class implementation. Return semantics are not asserted from this ignored-result call alone.
No source/header/Factory edits, build, or canonical check were attempted. No runtime failure or demotion is claimed.
Direct data references have containing rows; no new data-row request. Natural allocations observed here are 0x34, 0x70, and 0x28.
