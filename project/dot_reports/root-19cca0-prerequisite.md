# 0019CCA0: accepted declaration prerequisite

Base: `3d69bf00a769676c77f07465163def56a559b48f`.
Branch: `dot/root-19cca0`.
Target: `[0x0019CCA0, 0x0019CED0)`, 560 bytes, rank U at this base.
Outcome: stopped at ABI preflight; zero source forms and zero exact-byte credit.

The target initializes an al::LiveActor, captures its virtual base matrix at
object+0x8C, and creates a linked actor through the public al::ActorFactory
contract. That creator returns al::LiveActor* from a const char* name.
The target stores that result at object+0xBC and initializes it using the
linked placement. If its name equals DotCoinTailLiftUpA, the target passes
that same linked actor and the string MarioColor to 0x00271330. It ignores
the returned register value. This target therefore establishes a live-actor
receiver and a required string argument, but does not establish a C++ return type.

The accepted definition in
`Game/backup/src/Factory/group_0026ACDC.cpp:13` instead declares
`void* fn_00271330(RootObject*)`, where RootObject is translation-unit private.
Its one-argument callee declaration also omits the incoming name.
The accepted sibling `Game/backup/src/Factory/fn_0019CED8.cpp:33` declares
`void fn_00271330(void*, const char*)`; that is already inconsistent with
the accepted definition and is not a coherent public API to reuse.
No declaration for this symbol exists in Game or al public headers.

Binary inspection confirms 0x00271330 forwards the receiver through offsets
0x28, 0, 0x28 and preserves the second argument into 0x0024E9A8.
That routine passes the name into 0x0024E5E0, whose comparison uses it, then
updates animation state. This is not a receiver-only getter. The observed
machine return value does not justify the accepted pointer return declaration.

Required prerequisite: the owning lane must reconcile the accepted wrapper,
its downstream declaration, and existing callers into one evidenced shared
contract. No Factory edits, private-type adapters, or competing declarations
were added. The target's dispatch entry is inside existing data row
`[0x003CFA28, 0x003CFAC0)`; the nerve rows at 0x003F2814 and 0x003F281C
and SafeString vtable row are also present. No missing-row request is warranted.

Canonical `make.py eu` and `tools/check.py --object` were not run because
the prerequisite prevents a coherent source candidate. No object exists.
No scratch symbol names or ranks were used; the complete map is unchanged.
The owner code SHA256 was verified as
`e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.
No binary data, disassembly, source implementation, or shared headers are included.
