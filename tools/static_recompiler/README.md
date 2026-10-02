# Native static translation

This port tool reads the owner's approved EU `code.bin` and `exh.bin`, seeds discovery from a frozen main function map, generates C, and links that C with the native compiler. It does not change matching source, compiler settings, ranks, the ledger, or the original executable.

The ARM/Thumb/VFP generator is the public `fearkov/3dsrecomp` dependency at commit `83e6920784baff6ebc5888ed5cfdbf9735dfbf1c`, pinned in Cargo.toml and Cargo.lock. Its Cargo metadata declares MIT, and its README states that it contains no game code. Downloaded sources and developer tools belong under ignored build directories. No dependency source is vendored here.

Generated C, objects, libraries, input maps and reports are game-derived local data. The builder requires an absent child directory of the ignored `build/` tree. It verifies the approved original executable digest before translation. Do not publish its generated directory or native library.

## Build and verify

Install Rust and a native C/C++ compiler. The tested local Rust installation lives in `build/port_tools/cargo` and `build/port_tools/rustup`. A different installation may be selected with `CARGO`, `CARGO_HOME` and `RUSTUP_HOME`.

From the repository root:

```sh
. ./development_environment.sh
python tools/static_recompiler/build_port.py --output build/native_port_run
python tools/static_recompiler/verify_execution.py build/native_port_run
build/native_port_run/native_execution data/ver/eu/code.bin data/ver/eu/exh.bin build/native_port_run/translated.dylib
```

The library extension is `.so` on Linux. Compilation uses one slot. `--optimization 0`, `1`, or `2` selects native optimization without changing any ARMCC setting. `--without-replacements` builds the generated code alone as a control.

The build manifest records original and input hashes, frozen main, generator revision, compiler, every generated C hash, native library hash, linked function count and conservative instruction coverage. Native symbol inspection confirms every generated C function is present in the link. Coverage counts unique translated instruction bytes inside map function code ranges, excluding literal pools, explicit interpreter fallback sites and functions replaced by decompiled source. This is linked coverage, not execution parity or matching credit.

`NativeExecution.cpp` maps the executable's declared segments, BSS, a local stack and TLS. It dispatches by original address, stops at the first supervisor call, and refuses interpreter fallback or missing translation. It implements no console services, so it cannot yet reach a GPU frame.

## Decompiled replacements

`ExactFunctionReplacements.cpp` compiles the unchanged clean priority-conversion source from `lib/CtrSDK/sources/os_Priority.cpp`. Its adapter replaces original address `0x0010766C` in both the generated direct calls and address table. The builder verifies the exact symbol, rank O and source identity on frozen main before enabling it. Additional registered native adapters can replace more exact functions without modifying decompiled game files. Being rank O alone does not make an arbitrary class or pointer layout portable.

The independent original-execution validator uses Unicorn ARM11MPCore, not the generator's associated interpreter. It compares translated startup registers, flags, supervisor-call boundary and writable memory. It also checks bounded integer leaf functions across the executable and the replacement's AAPCS return/callee-saved contract. Floating-point edge cases, services, timing, GPU commands and gameplay require further differential checks.
