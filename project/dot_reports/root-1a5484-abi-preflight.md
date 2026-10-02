# Course label formatting shared argument prerequisite

Branch: dot/root-1a5484. Base: e2336037d4f709acc082b3322daedadf9500564a.
Target: 001A5484, 544 bytes, U. Exact claims: none.
This routine formats a wide-character course label from a two-word identifier and two flag arguments; complete class and return-lifetime semantics remain unresolved.
At 001A550C it passes that identifier pointer to accepted fn_002568B4, which forwards the same r0 to 002568CC.
The latter callee saves that pointer and directly reads its first two words before course lookup, independently establishing a required argument.
Accepted Factory group_0024C7EC.cpp instead defines fn_002568B4 as int() and declares its forwarded fn_002568CC as int(), both without arguments.
A typed pointer import conflicts with those accepted definitions. Integrator-owned Factory signature cleanup is required; no alias or register-preservation trick is proposed.
The local capped 1A5BE4 family is the SaveDataAccessSequence constructor, a different routine and source family; it was inspected and remains untouched.
No missing data row was found in direct literal preflight. No source, build, canonical check, or replay was attempted; no runtime failure or demotion claim.
