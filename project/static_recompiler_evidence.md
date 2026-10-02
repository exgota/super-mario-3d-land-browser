# Native static recompiler, first root family

`root/static-recompiler` adds a raw-executable adapter to the pinned public ARM-to-C generator and a strict native execution host. Its implementation stays under tools/static_recompiler and runtime/port. The matching Game/lib/config trees, map rows, ranks and ledger are unchanged. Generated game-derived C and native binaries remain ignored under build.

The dependency is https://github.com/fearkov/3dsrecomp at 83e6920784baff6ebc5888ed5cfdbf9735dfbf1c. Cargo.toml declares MIT and the README states that it contains no game code. Root inspected its discovery, generation, ABI, override and compilation source, and uses only the pinned dependency without vendoring it. This dependency reuse supplies ARM/Thumb/VFP lowering while leaving independent original execution and Azahar as the oracle.

## Observed native build

Fresh build/root_static_recompiler_verified starts absent and builds the committed generator against Rust 1.99.0 and Apple Clang 17. The declared executable segment pages agree with the raw binary extent. Original executable SHA-256 remains e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64; header SHA-256 is 94e61359c80498495dd77bb2df16f0def6fca94fca736dc8c3c929279b44d2c8. Its frozen main is 75eaf19ea813a4f3ce19ca37e12d8e521c9c79c2. The later accepted matches do not change the retail input or the replacement source.

Discovery receives 18,054 read-only map function seeds. All 40 generated C sources compile serially at native -O1, with floating-point contraction disabled. Native symbol inspection finds all 17,264 generated function definitions in the linked Mach-O library. The library SHA-256 is a4fc0129f21f6d7be3363f089ddc7b9ef91cee226497a435508a3a5cf6954a80. The approved exact priority source is also compiled unchanged by Clang and entered through the address override.

The conservative linked coverage is 2,400,572 unique map function instruction bytes. Literal pools, explicit interpreter sites and the exact-source replacement contribute zero. There are three generated fallback sites, none executed in the checks below. Coverage and checks do not establish correct execution of every linked function. No matching rank or exact byte credit is requested.

## Independent execution checks

Unicorn 2.1.4 executes the unchanged original using the ARM11MPCore CPU model. Native translated startup and original startup agree at the first supervisor call: SVC 0x21, next PC 0x00101B2C, all 16 general registers, NZCVQ/GE flags, and 1,384,448 writable bytes. The native host runs 251,587 translated instructions before that boundary, with no interpreter execution. This checks BSS initialization, calls, loops, literal reads, guest stack state and the first service boundary.

The priority replacement agrees in 4,105 inputs, including signed boundaries and reproducible random 32-bit values. It compiles the same clean source whose main row is rank O. Comparison follows the AAPCS contract: return r0, callee-saved registers, stack pointer and return PC. Original scratch registers and flags are caller-clobbered and do not form this contract. The generator redirects both direct calls and table dispatch to the same replacement address.

Another 2,048 full-register/flag comparisons pass across 64 reproducibly sampled bounded integer leaf functions throughout the original. They read only literal pools, have no pointer-argument memory access, external calls, services, backward branches or VFP operations. This checks more instruction forms without making layout assumptions or matching claims.

Evidence: build/root_static_recompiler_verified/{generator_build.log,generation.log,compile_*.log,replacement_build.log,host_build.log,link.log,native_symbols.txt,build_manifest.json,execution_validation.log,execution_report.json}. The initial failed Rust type-inference run is retained under build/root_static_recompiler_initial. The earlier symbol-audit implementation was interrupted after an unnecessarily slow repeated regex scan; its linked outputs remain under build/root_static_recompiler_native. The successful fresh run uses a set comparison for the audit.

Reproduce with the build/verify commands in tools/static_recompiler/README.md. Both source and payload hashes are recorded, so a different generated artifact cannot inherit this evidence.

## Still unverified

The host stops at its first supervisor call. It has no platform service implementation, calibrated Azahar timing, GPU submission or first-frame stream comparison. VFP edge cases and whole-game instruction behavior remain unverified. Milestone 1 remains in progress, and milestones 2 through 6 are not started. No rendering, input, audio, World 1-1 or browser claim follows from the native link.

The one helper builds an ignored pinned Azahar reference harness. It captures original GSP records, initial and chained PICA lists, and the first top-screen swap. Matching input/clock alone is insufficient because new user state includes random console identity; the reference must preserve its initial user snapshot for replay. No captured GPU frame exists at this submission.
