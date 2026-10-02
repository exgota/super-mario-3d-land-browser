# Graphics binding state, root 00282D20

This is a complete guarded NonMatching proposal against frozen main
`86104d96a7f570383bbfefc5fffad998e134496c`. It adds **zero exact roots, zero accepted
complete bytes, and zero accepted bytes/hour**. No publication or combined
integration is claimed. The lane owns only the new `gx_BindingState.cpp`, its
`GraphicsBindingState.h` observed views, and these reports.

The normal clean project build succeeds. The unchanged canonical checker rejects
its complete section: **1,924 candidate bytes versus 2,164 original bytes** at
00282D20..00283594. The function symbol covers 1,900 bytes and its section includes
24 further pool bytes. Only `fn_00282D20` is a nonempty function definition; all
source helpers inline. The exact checker result, exit status 1, is:

```text
U -> M: The complete compiled section, including its literal pool, has a different size from the original interval.
```

The existing U row is named temporarily only for this check, and its exact original
map bytes are restored in `finally`. No map, boundary, rank, ledger, STATE, tool,
compiler flag, resource byte, or other source/header family is changed.

## Observed difference on allocation failure

**Two fault diagnostics diverge.** With target 6600 and either a failed first
16-byte node allocation or a null allocator, the original faults on a four-byte
unmapped write at **0x00000004**; the candidate faults on a four-byte unmapped write
at **0x00000008**. Both report Unicorn access kind 20. The compared heap, state,
global bytes and ordered events agree up to these faults, but the fault addresses
do not. This is a known behavioral limit, not equivalent failure handling.

All **ten fault diagnostics are excluded** from the 624 returning-pair result.
Other observed failures include a four-byte unmapped read at 0x1C when a requested
binding-set payload is missing, and null-node writes at 0, 4, or 8 depending on the
target. They remain explicit diagnostics even where both sides happen to fault
identically. No source order, volatile access, padding, assembly, or compiler-flag
change was used to conceal the two divergent faults.

## Bounded execution result

The final clean-built source passes **624 returning ARM execution pairs**:
314 base cases and 310 extension cases. They compare 41,261,520 bytes of generated
state, registry, default binding set, node/payload allocations, and relevant global
cells, plus 909 ordered external/callee events. Maximum returning-run work is
1,516 instructions. Stack pointer, r4..r11, d8..d15, and final FPSCR agree on return.
The full root runs on both sides; this is not isolated helper testing.

The original binary's real implementations execute on both sides for:

- 00210850, sorted insertion into a low-nine-bit name bucket
- 0028B290 and 0028B1E0, 120-byte and 460-byte texture payload initialization
- 0028D1F0, zero-fill entry and its contiguous original tail through 0028D240
- 00282650, deferred removal with generated kind-2/kind-3 or absent payloads

The only external execution models are deterministic allocation and free
callbacks. They accept the observed `(0x10000, 0x100, 0, bytes-or-pointer)` words,
fill successful allocations with A5 before retail initialization, and record every
call. Callback returns poison r0..r3/r12 and s0..s15, then provide the required r0.
The free model records a release without recycling the memory. This does not
reconstruct an allocator, operating system, GPU, or graphics submission.

Cases cover all 35 supported targets; all three active texture units; new,
present, missing-payload, unchanged and zero bindings; empty/head/middle/tail hash
lookups and insertion; names around 2^31 and 2^32; all 38 fields restored by a
binding-set switch; found and missing referenced names; selected/default sets;
deferred deletion; and finite random set contents. Seven FPSCR settings cover
rounding, FZ and DN. Controlled callback cases change the active unit, clear the
allocator, or switch to a distinct copied registry during allocation. They are
bounded side-effect models, not a reentrancy proof.

Four cyclic missing-name chains are separately run on each side. All eight runs
exhaust a 200,000-instruction budget without returning, faulting, changing compared
data, or issuing calls. These are termination-limit diagnostics, not passing
terminating equivalence cases. Ordinary cases use finite chains and bounded,
distinct allocations in a 2 MiB test arena. Arbitrary overlap/aliasing, reentrant
callbacks, malformed pointers, invalid active-unit indices, exhausted address
space, arbitrary deletion payloads, and unbounded graphs remain unverified.
Unsigned low-nine-bit name hashing is tested across overflow boundaries; no signed
arithmetic overflow or out-of-bounds C++ array behavior is claimed.

The combined returning/fault suite reaches 514 of the 516 instruction positions
outside the original embedded data. The two unexecuted positions, 00282DEC and
00282E2C, are default exits below range checks whose enclosing comparisons already
restrict the unsigned index below nine. Coverage is descriptive evidence, not a
proof of equivalence or input safety. No rendering or gameplay claim follows.

## Binary role and identities

The approved private executable SHA-256 is
`e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.
The unchanged complete root hashes to
`9612526af131a50d90ab3599aeca05aa967b3c8c62bc88a55de62cb5c0070258`.
There are 516 instructions / 2,064 code bytes, two 36-byte interior dispatch tables
at 00282DF0..00282E14 and 00282E30..00282E54, and 28 trailing pool bytes at
00283578..00283594. No interior pool or tail was omitted from the extent check.
The root has 13 direct call sites to five existing mapped entries and eight
indirect allocator sites.

An independent full-image direct-call scan finds eight callers: 00112988,
00112A28, 001132AC and 0011334C in the earlier CFL texture root, plus 001D4CC4,
001D4E0C, 002B50B0 and 002CA09C. The first argument is a target word and the second
a name word; callers do not consume a return value. Independent callers generate
a name through 00283594, then bind it here. In particular, 001D4CC4 supplies 6610,
while 002B50B0 and 002CA09C supply DE1. This independently supports graphics
binding management beyond the prior CFL caller. The existing CFL declaration
`void fn_00282D20(unsigned, unsigned)` is preserved. No original API name is claimed.

Targets DE1 and 8513 update a pair of three-unit texture-binding arrays. Target
6600 switches an aggregate binding set; targets 6610..662F update 32 auxiliary
bindings. Other target words leave the observed state unchanged. Nodes are 16
bytes: payload pointer, kind, unsigned name and next pointer. Missing nodes are
allocated and inserted; a present node with a null payload is initialized. Existing
non-null payloads are used without checking their recorded kind, as in the binary.

The local state view reads active unit at 58, texture names at 5C and 68,
auxiliary names at 74, and selected set name at F4. The prefix array is an
access-offset view, not evidence that every preceding word is a dirty bitset.
Normal active units set bits 10..12 of the first word; auxiliary changes set bit
14. The registry view has defaults at 8, deferred-delete name at C, 512 buckets
at 10, node caches at 810/81C/828, and selected-set node at 8A8. A 9C-byte set has
its name followed by the three, three and 32 binding names. Kind-3 payloads are
820 bytes, initialized to zero with a final -1 sentinel.

Every import already has an unchanged map row. Global pointer 003E3154 has a
four-byte row; 003E3180 has an eight-byte row whose first word is the registry
pointer; 003E2654 is the four-byte allocator callback cell. The second 003E3180
word is retained as opaque in this view, while original deletion updates it as a
name-allocation hint. No new allocation boundary, global definition or copied data
is introduced. The dedicated source uses the existing SDK module's ARMCC 4.0/902
configuration because this is graphics-library state management around existing
nngx/graphics entries, not an actor implementation. That is provisional module
classification, not proof of the original compiler build. No 894 compiler was
installed or alternative flags selected.

## Source history and declaration compatibility

One implementation form was committed as `8f42d7109ebd81855acf0c38ba2abf301d1980bd`
before the first normal project build. It was rejected for 1,924/2,164-byte extent.
Two later committed declaration corrections preserve established/proposed shared
C signatures: `e652b25bc47c6ac2afa72e1e20c6030c58ef5e0c` retains the existing CFL
unsigned-pointer deletion argument; final source
`95940f5796310ca800ff1c649fc8efe2ed14ed04` uses the completed graphics-integration
proposal's opaque RenderControl pointer and four-unsigned allocator callback.
The original and final compiled root sections are byte-identical. There was no
algorithmic/code-generation tuning after the initial form and no forced fourth
attempt. All intended source revisions were committed before their builds/checks.

The shared signature is **proposed**, absent from main 861. A fresh local scan
covers 116 worktrees. Host C++ syntax checking with the actual completed
`graphics-integration` headers accepts the final declarations. Historical
DisplayInitializer still declares 003E2654 as an AllocatorState object, and
TextureUpload declares its final callback parameter as signed int. Older control
proposals use other pointee types for 003E3154. Those remain incompatible until
their owners submit a coordinated correction. No such family was changed, and
there is no combined integration claim. Exact conflicts and the compatible
proposal are in `texture-binding-state-declarations.md`.

## Preservation and final validation

Final `make.py eu -ca` passes in **39.828167 seconds**, 05:54:40.467961 through
05:55:20.296098 UTC on 2026-10-02. Final unchanged checker, both replay suites,
cycle diagnostics, and preservation run afterward against that object. The replay
runs take 8.460887 and 5.062060 seconds respectively. A reproduction-note heredoc error at 05:53 UTC caused import failures outside the
venv and overwrote wrapper logs; its saved failure record remains explicit. The
final clean build and all checks above were rerun from the sourced environment.
No source or map change resulted from that packaging failure.
Work began 05:25:44 UTC;
report/package preparation and declaration/scaffold audits are included in the
wall interval, not hidden as matching throughput.

The final build has **181 physical objects**: 179 unchanged prior canonical C++
objects, the new C++ object, and one generated scaffold object. Every one of the
179 old canonical objects, including objects without checked definitions, retains
all allocated sections/data, attributes, symbols, relocations, compiler identity,
normalized command and all **372 recorded inputs**. All **625 tracked baseline
files** are also unchanged. Path prefixes in STT_FILE and nonallocated `.comment`,
and consequent string/symbol-table indices, are the only canonical-object
normalizations.

The generated scaffold is audited separately: it gains exactly six function
aliases (210850, 282650, 282D20, 28B1E0, 28B290, 28D1F0) and three zero-filled data
aliases (3E2654, 3E3154, 3E3180). Every old section, symbol and resolved relocation
is preserved. Per-function CFI bytes/relocations also preserve; generated frame-CIE
ordinal names normalize to their per-function association. Exactly six new CFI
entries accompany the new function aliases. These aliases are normal generated
scaffolding and receive no implementation or exact credit.

This is **equivalence-backed preservation** against pristine 861, whose existing
empty-batch audit passed 767 roots / all 787 actual definitions in 596.501791275
seconds. Its report hash is
`a692f9a29d92c3f8021b62a2254b06fb44776f34cfa2f51849364bde66dee374`.
This lane did not rerun 787 canonical checks. The integrator owns any future acceptance.

The ordinary compact exported image remains identical to pristine 861, SHA-256
`c4116692fde2c3415f6368c6885089bb9dd6626b46c9798f4458dcd3c7109d02`.
The new root remains U and normal linking may select its generated stub. The
actual source body is therefore linked separately for replay at its original
address, resolving every import to its existing row. That diagnostic link is never
submitted as a canonical object. Its SHA-256 is
`9c9afaf1985c5a69fb8bdf01cd07425a1ffd16afdde05c7d95b0a664cc9b1ec2`.

Reproduction scripts and exact fixture construction are in
`texture-binding-state-reproduction.md`. Ignored local evidence is under
`build/texture-binding-state/`; no game or generated binary is in this proposal.

## Final hashes

- `lib/CtrSDK/include/retail/GraphicsBindingState.h`: `18b0093d99ca5b28afb5c3da00e25f416a1cf06d8de42daf737c664f4c4f3373`
- `lib/CtrSDK/sources/gx_BindingState.cpp`: `ef6f65ee654252c463c37f37e23b4c32a119568c9cb6ff1d291a31f54192f32b`
- `data/ver/eu/map.csv`: `94ac5673e907db521bf6b4b606d02be64ee7bae8a8fd2f08328b0c30d85b3db9`
- `data/config.json`: `5d41e617eb5b26374ed60b5383a0940ce0ce7d046344f7f815b7564144822c45`
- `tools/check.py`: `e177e0abe130e645e75c5f0dc24abb19dbe83310de55f1f099ec8fc385e07317`
- `tools/pypstem/stepBuild.py`: `b7be5977a0a1afc339eb4c9087b447cdc25b1c42daa3fe23af4b765008b0aac0`
- `build/eu/obj/lib/CtrSDK/sources/gx_BindingState.o`: `0ae3db6c8d5fdc3e51fd177e09920173a0324175edd90e7d83ce2b814aec9dcb`
- `build/eu/obj/lib/CtrSDK/sources/gx_BindingState.provenance.json`: `1a06f2839ccaf5e5a5e88e7ae270158a6ec63fe20014d63b01e7d4929db34d48`
- `build/eu/code.bin`: `c4116692fde2c3415f6368c6885089bb9dd6626b46c9798f4458dcd3c7109d02`
- ARMCC 4.0/902 executable: `e6aca4ebc280a54065406bcc3a0ae3d9f4d0ac5a4032c3a193adb836c72e70fe`

## Publication status — 2026-10-02 07:52 UTC owner decision

The owner confirmed that `exgota/super-mario-3d-land-browser` is public and authorized publication of source/header/report proposals on `dot/*`. The previous visibility hold is lifted under the updated `AGENTS.md`, `project/BRIEF.md` rule 13 and `project/DOT_BRIEF.md` at `56e9b4b89fe3c5d391c2948f1ce2b2c50706638e`. Game files, extracted assets, credentials, private download URLs and leaked SDK material remain excluded. Earlier private/publication-hold wording describes the historical checkpoint.

This remains a proposal for the integrator with zero exact-match claims. All build, checker, replay and preservation evidence above concerns frozen base `86104d96a7f570383bbfefc5fffad998e134496c`; it is not current-main acceptance. The recorded failures, replay limits and unresolved type/identity constraints still apply. This publication correction changes notes only: source/header blobs and retained objects are unchanged, and no build or replay was repeated. Only the integrator may accept the proposal, set committed ranks, write the ledger or move `main`.
