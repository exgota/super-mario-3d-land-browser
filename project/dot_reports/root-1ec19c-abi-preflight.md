# Actor initialization shared init-info prerequisite

Branch: dot/root-1ec19c. Base: e2336037d4f709acc082b3322daedadf9500564a.
Target: 001EC19C, 544 bytes, U. Exact claims: none; concrete actor class name remains unproved.
This root passes its same initialization-info argument through argument getters, including fn_0027D1DC at 001EC20C.
The already recorded root-13d41c-abi-preflight.md identifies this helper's declaration in accepted Factory fn_0018938C.cpp: void(int*, const anonymous ActorInitInfo*).
The TU-private incomplete type has no usable shared declaration. A new typed al::ActorInitInfo import would not reconcile the accepted caller.
This is another dependent target for the same integrator-owned signature cleanup, not a new ABI audit or runtime fault claim.
No source/header/Factory changes, build, or canonical check were attempted. No exact or demotion claim.
Other layout, data, and source prerequisites were not exhaustively screened after this established dependency was found.
