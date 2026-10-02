# Actor state update: integer receiver prerequisite

- Branch `dot/root-163054`; base `6b0e2a1385b814b70344023d9816424edcc7d832`.
- Target `0x00163054..0x00163290`, 572 bytes, remains U.
- Incoming r0 is an object receiver: the target increments its +0x18C word and passes the preserved receiver to named actor/nerve helpers.
- Accepted Factory group_00152284 declares `unsigned int fn_00163054(unsigned int)`; wrapper 16304C loads its argument as an integer word. No established typed ordinary alias exists for reuse.
- A natural object-pointer implementation would conflict with that accepted integer declaration. Shared repair is integrator-owned; no integer adapter or new export alias was introduced.
- Return semantics are not claimed here. No source/build/check attempt; payload is this short note only.
