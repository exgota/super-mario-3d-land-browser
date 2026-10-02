# Actor-message player-position entry prerequisite

Branch: dot/root-3112e4. Base: 3d69bf00a769676c77f07465163def56a559b48f.
Target: 003112E4, 556 bytes, U. Exact claims: none.
At 00311454, this target calls entry0027A5C8 after a message predicate; no object pointer is prepared. The returned pointer supplies player-position Y.
Entry0027A5C8 is a four-byte no-op prefix immediately before mapped rp::getPlayerPos() at0027A5CC, whose public declaration takes no arguments and returns const sead::Vector3f&.
Accepted Factory group_00223D40.cpp instead defines the prefix as void* fn_0027A5C8(void*) and declares the underlying mangled position function with that same extra pointer parameter.
A typed caller cannot match this entry by passing an invented dummy object or by substituting the different +4 call target. A coherent prefix declaration/identity requires integrator-owned reconciliation.
No boundary change, alias substitution, Factory edit, or byte/demotion claim is proposed. The observed prefix is not declared to be a map error.
The existing proper fn_0027D760 message/sensor declaration from accepted3135E0/GhostPlayer was reusable and was not treated as a blocker.
All three direct nerve data rows exist. No source, build, canonical check, or replay was attempted.
