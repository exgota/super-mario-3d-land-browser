# Sample-array initialization callback prerequisite

Branch: dot/root-1f4540. Base: 713975727c0447ea8cdea708a9ab13cc539a92d0.
Target: 001F4540, 556 bytes, U. Exact claims: none.
The root invokes mapped __aeabi_vec_ctor_nocookie_nodtor for 32 records of size 0x3C at receiver +4, using callback 001F47A4; it also initializes an equivalent temporary array later.
That runtime constructor callback receives each element address. Accepted Factory group_001CF7EC.cpp instead defines fn_001F47A4 as void() with no arguments, and no shared record-constructor identity exists.
An ordinary C++ record-array construction needs a coherent constructor/callback declaration; casting the no-argument function or inventing an alternate export would not resolve the shared type contract.
The empty four-byte callback body establishes no original class name by itself. Integrator-owned Factory/declaration reconciliation is required before this lane implements the constructor family.
Observed records, reader subobject, and timing setup suggest an input-sample reader; full receiver layout and time-conversion source form remain unresolved.
Counter storage 003EF688 and timing storage 003EF690 have separate existing eight-byte rows. No new data row is claimed.
No source, build, canonical check, or replay was attempted. No byte failure, runtime fault, or demotion claim; inline system-call handling was not attempted.
