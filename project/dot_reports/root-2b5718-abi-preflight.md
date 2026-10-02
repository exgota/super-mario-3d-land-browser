# Resource initializer 002B5718: shared declaration prerequisite

Base: `3d69bf00a769676c77f07465163def56a559b48f`.
Branch: `dot/root-2b5718`.
Target: `[0x002B5718, 0x002B5948)`, 560 bytes, rank U.
Outcome: prerequisite-only; zero source forms, builds, or canonical checks.
No exact claim or new reconstructed source is supplied.

The initializer selects four resource representations by mode. Mode 0 allocates
and copies a byte buffer; mode 1 initializes the embedded record at +8 and stores
its address at +0x130. Modes 2 and 3 construct shader programs and store their
handles at +0x154 and +0x158. The final argument controls release of input data.
The independent caller in 001D0C04 supplies this five-argument call shape.
The neighboring 002B5608 consumer reads +0x154 for named uniform lookup.

Both shader modes call 0022D81C with one unsigned shader-handle word. That callee
immediately overwrites its incoming second register and indexes the shader
registry using the first word. The independent 002CBA08 initializer likewise
passes only a shader handle. Existing accepted Factory source instead declares
`extern "C" void fn_0022D81C(void*, void*);` in
`Game/backup/src/Factory/group_0022D800.cpp:14`.
Its accepted 0022D800 definition remains O in this base.

A coherent one-argument unsigned-handle declaration must first be established
across the accepted source. Per the current workflow, this lane does not edit
that Factory file or add a conflicting declaration, renamed alias, or adapter.
This is the only blocking prerequisite found in the target preflight.

Other imports have existing function rows. The uniform lookup and integer
setter signatures agree with `Graphics/TextureEnvironmentSetup.cpp`.
The byte-copy import already owns the `__rt_memcpy` map name. The allocator
wrapper takes size and alignment; the input release wrapper accepts a pointer.
The target uses local scalar constants and natural string literals, and has no
external table or vtable reference needing a new data row. Embedded shader
record extent remains inferred from its surrounding fields, not a missing-row
claim. No shared record definition was introduced.

The owner binary SHA-256 was verified as
`e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.
The map was never changed. `make.py eu` and unchanged `tools/check.py --object`
were not run because compilation was stopped at the accepted-declaration gate.
There is no project-built target object or object provenance to freeze.
