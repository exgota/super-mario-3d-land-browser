# Placement initialization: bounded contract differential

Observed 2026-10-01T14:16:49.297558+00:00 against committed root source `a63bb8e82720790b2a2a2d841278f90d7bf52a46`. Dot source tip is recorded in source_snapshot.json.

## Measured result

**76/76 execution pairs agree**, 38 finite fixtures × two modeled-callback ABI modes. Each pair runs the complete retail initializer and the complete frozen canonical source-generated initializer under Unicorn 2.1.4, ARM11 MPCore with VFP enabled. Run time: 1.208631 seconds. Maximum instruction-hook count: 9865 on each side, under the 100000-instruction/run engine cap. Callback-entry hooks contribute to this observed count. Deepest actual stack write: 456 bytes, within the enforced 512-byte allowance.

The measured claim is equivalence of traversal and initialization handoff under explicit archive/creator contracts. It is not whole-game execution, full actor construction, exact byte equality or a gameplay/replay result. Root's canonical checker reports `M -> m`; 33 bytes still differ in the complete 344-byte interval. Exact-matched coverage does not increase.

## Frozen provenance

- Symbol: `_ZN2al16initPlacementMapEPNS_5SceneEPKNS_8ResourceERKNS_13ActorInitInfoEPKc`.
- Original interval: [0x00274EF0, 0x00275048), 344 bytes; pool at 0x0027503C contains three unchanged words. Original SHA-256: `378c14f999e25d0358479989939a90114deb3b6fcb3651a180817895ec049049`.
- Approved full executable SHA-256: `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.
- Canonical source equals dot proposal bytes, SHA-256 `fd5e23ceb5b24deac34b8249a499e901c3e18ffb8977d2bc0a7fd4a39bbddee7`.
- Direct canonical object: `build/eu/obj/lib/al/src/Scene/alSceneFunction.o`, SHA-256 `c7a40f4fcf21cc31f58fadbe8554aa5071cce9ba67e3958d702ce1bfd970d153`.
- Frozen original-address linked candidate: `build/exact_checks/eu/function_00274EF0_jhyx3mge/candidate.axf`.
- AXF SHA-256: `98b16711d2b415e6cfb6a4f7ffd982c7f68b4976a265d46cc3c278e7ce00aa4e`.
- Candidate code SHA-256: `4a69f3fef5d46caa676d022d18b01eb5d27c9251c938199e1c1ecd0d6349cf98`.
- The root canonical diagnostic and project compiler provenance are preserved verbatim as snapshots. The harness overlay uses only this code section and leaves all retail imports/data unchanged.

## Named models and original imports

The **modeled archive virtual file lookup** requires `StageData.byml` and returns the fixture Byaml pointer or zero. Archive storage behavior is untested. The **modeled actor creator return** records the actual selected creator address and returns a preallocated valid synthetic actor. It skips the selected creator's body, allocation and construction. Both models run once preserving caller-saved registers and once clobbering R1/R2/R3/R12 and NZCV, preserving SP and callee-saved state.

Resource::getByml, SafeString and formatting/CRT chain, all Byaml/header/string/hash/array helpers, full ActorFactory name conversion and 225-entry retail creator scan, record name lookup, ActorInitInfo constructor/copy/ViewId extraction, initCreateActorWithPlacementInfo and the original empty base LiveActor::init execute unmodified. No helper instruction body is stubbed or replaced. Hooks observe the base init arguments before its actual `bx lr` executes. The modeled callback address has no executed instruction implementation.

The observed retail footprint is recorded in retail_callee_footprint.json: 34 mapped function-entry addresses, including the root. Candidate overlays the root only; every other executed callee instruction is original. Internal labels and CRT code also execute within the unchanged original image. Execution rejects addresses outside the original image except the declared archive callback and final return.

## Observed state and order

Both sides have identical archive/creator/init event order, creator addresses and object-name sequences. The 24-byte ActorInitInfo observed during each actual base init has placement iterator contents from the corresponding child, copies base fields +0x04/+0x08/+0x10, retains constructor zero at +0x0C despite a nonzero template value there, and derives the signed ViewId. Missing or wrong-type ViewId yields -1. Integer extrema remain exact.

Mixed children skip absent/wrong/null names and unknown objects while retaining later valid actor order. Duplicate ObjectName conversion entries use the first matched conversion even when its class is unknown. Hash named children traverse by indexed hash-entry order. Empty object name can create when its conversion exists. The last of 225 static creator entries is reached and returned correctly. Repeated 2/4/8-record fixtures preserve actor index and ViewId order.

All 262144 arena bytes agree and remain unchanged. Every write stays in the allowed active stack window. All other 65024 stack bytes, R4–R11, S16–S31 and SP remain unchanged. Raw transient stack addresses are normalized by comparing the pointed-to placement iterator contents. Active stack frame contents, caller-saved register results and final NZCV are excluded from equality because the callback contract permits their changes.

Fixture definitions are finite and independent of the retained C++ implementation. Expected creation names and ViewIds are asserted on retail. Initial preflight found a fixture mistake: placement records use literal `name` at 0x003A2DAC, while conversion rows use `ObjectName`/`ClassName`. The fixtures were corrected before differential comparison. No candidate mismatch was concealed.

## Limits and blockers

This uses synthetic valid objects/Byaml, zero through eight placement records, conversion arrays of at most two entries, and a nearest-mode initial FPSCR. It checks no floating-point actor behavior. Missing-file behavior is a modeled zero return, not a claim about real archive error policy. Invalid-magic handling is tested; arbitrary malicious offsets/counts are not.

Creators do not allocate or construct actors. The actual init destination is the empty base LiveActor method, not any derived actor implementation. Persistent actor fields, scene registration, resource loading, initialization side effects and frame updates are untested. Every modeled creator returns nonnull, so allocation failure/null actor dereference is outside the supported fixture contract. Mutable factory/array state and concurrent lifecycle changes are untested. Software ARM11 emulation does not establish physical-hardware behavior.

Full unmodified archive/creator/derived initialization requires independently reconstructed heap/services/resource-interface state and real actor dependencies. Those contracts are the blocker to expanding this result into full initialization equivalence. BRIEF's gameplay differential replay remains unperformed. The guarded source stays NonMatching; the parent records any bounded behavior outcome separately.

## Reproduction and files

```sh
. ./development_environment.sh
python build/dot_proposals/placement-map/validation/verify_initialization.py \
  --candidate build/exact_checks/eu/function_00274EF0_jhyx3mge/candidate.axf \
  --source-commit a63bb8e \
  --expected-candidate-sha256 98b16711d2b415e6cfb6a4f7ffd982c7f68b4976a265d46cc3c278e7ce00aa4e \
  --output build/dot_proposals/placement-map/validation/evidence_repeat.json
```

`evidence.json` contains every fixture and both full observable results. `verify_initialization.py` is the one harness; `validation_plan.md` gives the contracts, safety constraints and fixture rationale. `hashes.json` freezes all relevant inputs and outputs. Initial retail-only preflight is preserved separately and has no candidate equivalence claim. Production source, map, project documentation, ledger, ranks and Git were untouched by this lane.
