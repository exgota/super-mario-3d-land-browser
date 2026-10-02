# Primitive draw setup: complete NonMatching proposal

The complete unnamed EU root `0x002E0D2C..0x002E1544` (2,072 bytes) is reconstructed as guarded C++ in source commit `37eeb40a1b42bcf9f0d42b9b2d8d3c5e00e3b986`, against frozen base `86104d96a7f570383bbfefc5fffad998e134496c`. This is one unsuccessful source form, retained as **NonMatching**, with **zero new exact roots and zero exact bytes**. No optimization forms were added during recovery.

The unchanged project checker reports:

> U -> M: The complete compiled section, including its literal pool, has a different size from the original interval.

ARMCC 4.1 build 791 emits 1,968 bytes, not the original 2,072. There is no equal-length differing-byte count: the checker rejects size first. The normal project build compiles the committed C++ and clean-links the existing scaffold; because the unchanged map leaves this root U, that link does not enroll the proposed root. The separate diagnostic replay link runs the real candidate object with its 22 imports resolved to existing retail identities. It is not a canonical acceptance link.

## Scope and classification

Only these reconstruction files are added:

- `lib/al/src/Graphics/retail_PrimitiveDrawSetup.cpp`
- `lib/al/include/Observed/PrimitiveDrawSetup.h`

The source covers the whole root: temporary shader construction, two shader configurations and command buffers, attribute lookups, box/line/sphere/disk/cylinder geometry, outline index buffers, material state, and temporary shader disposal. Vertex and index-buffer views describe observed 0x24-byte and 0x10-byte storage. Unknown receiver regions remain opaque.

**`lib/al` / Actor / 791 is the configured build carrier, not proof that this function belongs to al or that 791 was its original compiler.** The root remains unnamed in the base map. Its behavior and already named neighboring imports support a primitive-draw-manager interpretation; the private `primitive_draw_setup` namespace and filename are descriptive names, not recovered original ABI names. No module-wide compiler setting changed and build 894 was neither downloaded nor tested.

The implementation and new layout evidence come from the owner's hash-verified EU executable. Existing symbolic import spellings come from the frozen repository map; this lane did not consult removed SDK/sead/NintendoWare libraries, RedPepper Library material, leaked source, or additional symbol dumps. It neither certifies a new provenance for inherited map names nor uses them alone to establish root identity. No binary, asset, extracted shader, compiler, ELF, or gameplay image is part of the patch. The synthetic replay shader is constructed by the test harness, not extracted from game assets.

## Shared declaration conflict: integration prerequisite

**Do not combine this branch blindly with the LightData family.** Its external C declarations currently conflict at two shared identities:

- This proposal and skeletal-construction declare `extern void* dat_003E23B8` and `void* fn_00293088(void*)`; effect-resource-init also uses the pointer declaration for the data symbol
- LightDataDirector1C6410 declares `extern unsigned int dat_003E23B8[]` and `sead::Heap* fn_00293088(unsigned int)`, then passes `dat_003E23B8[0]`

These happen to read the same first 32-bit word under this ARM ABI, but they are not compatible C++ declarations. The 12-byte mapped data interval is not proven to be a pointer-sized object's complete type. Independent inspection of `00293088..002930A8` shows that it replaces r0 using its own global/TLS path, then returns the thread's +0x64 heap; its observed callers do not resolve the original parameter type. A shared declaration decision and cross-family preservation check belong to the owning integrator/lane. No other worktree or shared header was changed here.

The shader attribute parameter deliberately reuses the effect proposal's incomplete `effect2ea9e8::Attribute` type, matching its external C declaration. This branch dereferences none of that type's fields and does not duplicate its definition. The other shared allocator/shader imports use the same signatures inspected in the effect proposal. `recorded/diagnostic-link.json` enumerates all 22 imports, and the declaration audit from recovery remains local in `build/primitive-draw-setup/declaration-audit.txt`.

## Independent receiver and helper evidence

Caller `001019B8` allocates 0x48EC bytes through `002913E4`, conditionally invokes `0010349C`, stores the result at caller +0x20, then dispatches through the receiver's first virtual slot with heap and shader data arguments. Constructor `0010349C` installs table `003DA3B4`; that table's first word is `002E0D2C`. The constructor/table/caller chain is independent of the proposed setup source and establishes the minimum receiver extent and virtual dispatch identity.

The root requests a temporary 0x128E8-byte shader with alignment -32. Heap dispatch uses vtable offsets +0x14 (allocation), +0x20 (resize), and +0x38 (largest block). It resolves a null heap through the existing TLS helper. Private field names reflect uses and offsets, not a complete original class declaration.

The replay executes the retail shader constructor/parser/configuration, attribute lookup, command generation, shape allocation/copy, vertex constructors, geometry generators, arithmetic helpers, and the independent receiver constructor. These helpers are imports, not newly reconstructed C++ bodies. The package proposes one complete root, with no reconstructed helper-family credit.

## Differential replay and limits

The final preserved object passes 35 whole-root fixtures: **24 normal returns and 11 equal memory faults**. It executes all 485 root instruction addresses in the two executable ranges `002E0D2C..002E1124` and `002E1194..002E1530`. The intervening/final literal islands are not counted as instructions. This is instruction coverage, not exhaustive state or branch-path coverage.

Fixtures cover explicit/null heaps, in-place/moving resize, caller-saved register clobbering, shader command counts 1/3/8/16/31, attribute suffix lookup, six floating-point control modes, four receiver/allocation offsets, null receiver/shader inputs, and nine allocation-failure positions. The normal runs verify preserved r4-r11 and d8-d15, stack restoration, allocation/resize/free/cache events, attribute and shape call semantics, aggregate retail helper calls, masked FPSCR status, a 1 MiB receiver/data region, a 2 MiB allocation arena, and the observed global region. Stack scratch bytes and return r0 for this void function are not compared. Fault cases compare fault kind/address/width and observed state; they are not claims of graceful error handling.

The only runtime models are heap allocate/free/resize/largest-block vtable calls, TLS read `0028CF4C`, heap-owner query `0028E8F8`, and cache flush `00296048`. The original executable file is unchanged; the harness seeds mutable memory and intercepts those services. Retail helper instructions execute from the verified dump. Each root execution has a 2,000,000-instruction limit. Synthetic shader programs and constructed heap/TLS states do not cover arbitrary real shaders, allocation interleavings, hardware cache behavior, or live PICA execution. No scene, frame, rendering, full game, port milestone, or gameplay equivalence follows.

Two earlier baseline fixture failures are preserved. A zero shader-command count and then a zero swizzle count caused equal out-of-range faults in original and candidate, so the harness rejected them as invalid normal-return fixtures. The final synthetic inputs provide positive command/swizzle counts. No candidate source changed to hide either failure. See `recorded/fixture-zero-command-failure.json` and `recorded/fixture-zero-swizzle-failure.json`.

## Build, oracle, and preservation evidence

The pre-interruption clean build ran 2026-10-02 06:11:57.658400–06:12:36.519001 UTC (38.860796 seconds). Recovery inspected the existing source/evidence rather than restarting reconstruction. The fresh clean build ran 08:01:37.758274–08:01:56.957476 UTC (19.199223 seconds), exit 0. The 08:02 replay took 3.248416 seconds. The end-to-end historical investigation duration was not recovered, so these test windows are not accepted-bytes/hour or labor-time claims.

Frozen baseline evidence is the pristine 86104d9 report, SHA-256 `a692f9a29d92c3f8021b62a2254b06fb44776f34cfa2f51849364bde66dee374`: 767 prior exact roots and 787 actual canonical definitions passed its checks. This lane verifies **all 179 prior source objects and all 372 provenance inputs** against that baseline. Object comparison normalizes repository path prefixes in STT_FILE and nonallocated .comment information, plus resulting raw symbol/string indices; section bytes, attributes, symbol identities, resolved relocation identities, compiler hashes and normalized build commands otherwise agree. All 625 tracked files in the frozen base remain byte-identical.

Generated scaffold comparison preserves all 2,095 old allocated sections, 8,060 prior named non-file symbol records, and 1,934 relocation-section records. There are 14 extra allocated sections: thirteen 16-byte function placeholders and a 12-byte data placeholder. They contribute **zero reconstructed or matched bytes**. Numeric nonallocated debug-frame self-group names shift after inserted placeholders; the audit normalizes those names only after proving the symbol refers to that exact frame, and still compares frame bytes and referenced code identities. Two repeated-name dictionary issues and this debug-name distinction were found while repairing the interrupted diagnostic seal; `recorded/audit-recovery.json` records them. No project checker, target, map boundary, or reconstruction was altered for that audit.

This preservation proof transfers the independently checked baseline object result through normalized object equality. It is not a claim that 787 checker invocations were freshly rerun in this branch, or that a current-main integration has been tested. Only the integrator can accept new ranks after its own final build and checks.

`check.py` temporarily supplies address-based names to 14 already existing map rows so the unchanged project object checker can resolve the proposal. Their intervals and types stay fixed. The complete map is restored in a finally block and its SHA-256 is verified. These scratch names/ranks are not submitted.

### Final hashes

| Artifact | SHA-256 |
| --- | --- |
| C++ source | `d0ded71c767eae58d19699a715f4aecf3974c55791b4dd12d443ec994d67c052` |
| Header | `0e2bced934b826cce4fa6b4bbf1e6ec00d43486e00bdbe64ee7936e084c427ce` |
| Canonical source object | `898e30e6db4c63acaa17690b245f9cb498bf3d9b5f624921cc6d338ce88f4c4a` |
| Complete compiled root section | `9afd2b982c129178a7e5ed13ed181efddcf870d6d145c2da68efc577c6cead4e` |
| `tools/check.py` | `e177e0abe130e645e75c5f0dc24abb19dbe83310de55f1f099ec8fc385e07317` |
| `tools/low/checkExactBytes.py` | `aef81a3a21c49bb17b8a19f139d02607899257e04dbe461ece2992cc726fc2e2` |
| `tools/low/buildProvenance.py` | `343d1a0954aba9a4805e71420be9a99d91185db4c8b329d1363d11efcee53529` |
| Restored map | `94ac5673e907db521bf6b4b606d02be64ee7bae8a8fd2f08328b0c30d85b3db9` |
| ARMCC 4.1/791 | `d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d` |
| wibo | `aee836280b3d7c80c2031410219c907c9824c11be6b128d0744655de0f22280b` |
| Original EU executable, local only | `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64` |

The complete compiler flags, source/input hashes, per-object preservation records, and other toolchain hashes are in `primitive-draw-setup/recorded/`. These JSON records contain metadata and test results, not executable data.

The packaged reproduction runner was then executed end to end at 2026-10-02T08:06:24.160817+00:00 to 2026-10-02T08:06:52.677496+00:00 (28.516703 seconds), preserving the same source/object hashes and all results. Its second clean build, canonical mismatch check, replay, coverage and preservation stages all completed; see `primitive-draw-setup/recorded/reproduce-run.json`.

## Reproduction and handoff

Keep the owner's authorized binaries and already approved 791/902 toolchains local and gitignored. Python needs the existing project dependencies plus Unicorn and pyelftools/capstone. The archived preservation proof expects a pristine frozen-base checkout with its verified build and `build/dot-baseline-861/report.json`; set `PRIMITIVE_BASELINE` to its absolute directory if it is not the sibling `mario-main861` directory. This archived baseline report is a prerequisite, not generated by this family script.

From this branch's repository root:

```sh
. ./development_environment.sh
export DEVKITARM=/usr  # Linux binutils location; use the real platform path elsewhere
python project/dot_reports/primitive-draw-setup/run.py
```

The runner verifies the frozen inputs and toolchain hashes before execution, clean-builds committed source through `make.py eu -ca`, runs the unchanged project checker, links the untouched candidate object for diagnostic replay, replays all fixtures, checks instruction coverage, checks prior objects and generated scaffold, then verifies inputs again. It writes regenerated evidence only under ignored `build/primitive-draw-setup/`. The pinned checker result is expected to remain NonMatching; a runner pass means diagnostics reproduced, not an exact match.

Submit the whole branch/patch unchanged against the named frozen base. It changes only the two source/header files and these notes/reproduction records. The coordinator owns publication; the integrator owns current-main conflict resolution, shared declaration decisions, enrollment, ranks, ledger, and acceptance. This family has no exact claims; its proposed complete NonMatching scope is 2,072 original bytes. No map, ledger, STATE, tool, configuration, or oracle changes belong to it.

2026-10-02 authorization clarification: the owner's updated public-repository instructions were read at `56e9b4b89fe3c5d391c2948f1ce2b2c50706638e`. The coordinator reports explicit owner authorization at 07:52 UTC for public source/report dot branches. Historical test claims and original test timestamps above are unchanged. This recovery task performs local work only and publishes nothing itself.
