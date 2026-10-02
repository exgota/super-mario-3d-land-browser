# 0x002E4BE4: incompatible accepted return declaration

Base: `6b0e2a1385b814b70344023d9816424edcc7d832`, checked against remote main
on 2026-10-02 at 23:31 UTC. Branch: `dot/root-2e4be4`.
Target remains U: `[0x002E4BE4, 0x002E4E18)`, 564 complete bytes.
Zero source forms, builds, or canonical checks; zero exact credit.

The existing declaration in `Game/backup/src/Factory/group_00129414.cpp`
is `extern "C" bool fn_002E4BE4(void*)`. Its accepted wrapper
`fn_002E4BDC` returns that value. I checked compatibility rather than
treating the declaration alone as a blocker.

The target clears two 1024-byte regions at object offsets 0x1C and 0x428.
It then clears one word in every entry of four arrays. The array pointer,
signed count, stride, and cleared field offsets are respectively:

- 0x41C, 0x83C, 0xB4, 0xA4
- 0x420, 0x840, 0x124, 0x114
- 0x424, 0x844, 0x2688, 0x2614
- 0x10, 0x14, 0x1D0, 0x4

Finally it clears fields 0x864 and 0x870 and copies count 0x83C to 0x868.
The full count remains in the return register, without bool normalization.
The initialization routine at 0x002E5C94 writes `1 << config_byte[9]`
to 0x83C at 0x002E5D1C. This field is a capacity, not a bool.
The direct wrapper caller at 0x001C0D9C immediately replaces the returned
register with its own object pointer. The original wrapper is an object
pointer load followed by fallthrough into this target.

A void reset is supported by these uses; an integer return cannot be
excluded solely from the observed register residue. A bool return is
not supported. Returning a converted count or a constant bool adds return
normalization or overwrites the observed count. Omitting a return from a
bool function would invoke undefined behavior and is not a valid match.

The accepted private Factory return contract therefore needs owner review
before implementing this body. No adapter, alias rename, shared-header edit,
Factory edit, map edit, rank edit, or build/oracle change was made.
No missing-row claim follows: the target and both direct clear-call rows exist.
No module identity beyond this adjacent object family is claimed.

Original code SHA-256 verified:
`e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.
Evidence was read only from the authorized original and current source.
There is no object or build provenance because compilation was not attempted.
