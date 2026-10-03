Claims: none. No source form or canonical check attempted.
Closest attempt: preflight only; 752-byte target remains unimplemented.
The accepted helper at 0x00229370 declares float(float*, int), but its implementation
addresses actual float members at receiver + 0x34 + 4 * index.
The target receiver is a mixed-layout track object, not an established float array.
A cast solely to accommodate this misleading public signature is not admissible.
Next idea: integrator-owned shared declaration repair backed by those member accesses.
Do not introduce a second incompatible export or count this as a checker failure.
