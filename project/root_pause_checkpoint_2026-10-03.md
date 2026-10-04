# Root pause checkpoint, October 3, 2026

Paused by the owner for a Mac restart. Resume only after an explicit owner
instruction. No new replay, investigation, provider build or submission starts.

## Branch and integration

Worktree: `/Users/exgota/super-mario-3d-land-browser/build/root_browser_gameplay_session`.
Branch: `root/browser-gameplay-session`, frozen main
`96773182c9a4b23005dc39b9a661580619c841f2`.
Pre-checkpoint commit: `0ffa79f410b73aa419370942b5498975dbf0893d`, pushed.
The pause commit preserves the two formerly untracked public verifiers and these
notes. Final branch/status proof is in the private checkpoint directory below.
This family is held and unsubmitted because its wider exact comparison failed.
No claim, rank, ledger, Game/lib/config/map/Factory or matching-tool changes.

The prior native bridge submission `root-world-one-bridge-continuation-e431761ab`
was accepted at main `de8c5810ae96f694cdad35ffc03af6a7c8b338e4`. Verdict:
`.integrator/results/root-world-one-bridge-continuation-e431761ab.json`.
Nothing newly ready requires integration. Do not force this gameplay family into main.

## Actual browser result and first known differences

The browser runs static ARM/HLE/software PICA in WebAssembly and draws its own
RGBA with Canvas2D. The local server serves runtime/sidecars and receives exports.
It does not render or feed pixels. WebGPU is absent.

The9000presentation original-movie replay completed beside the World1-1 bridge
crates, CPU0 4313017982/CPU1zero, reached fallbacks0, renderer9101,
ticks44048217034. Original HID/audio timing and bottom RGBA/framebuffer are exact.
Top RGBA differs in22/96000pixels,22bytes, maximum channel difference17.
First known top final-image difference is byte81124. The first divergent rendered
frame has not been located because this mode exports final screens only.
PCM differs in exactly1/9884160channel samples, browser value one greater:
sample frame1099311, right channel, native -3628/browser -3627, block6870,
within-block offset111, ticks12567578138, renderer2075, PCM byte4397246.
This is the first known captured divergence, not a proven first differing guest state.

Reference: `build/root_world_one_bridge_continuation/build/world_one_bridge_preparation/reference_world_one_bridge_8400_9000/capture`.
Browser: `build/root_browser_gameplay_session/build/browser_world_one_server_9000/capture_fcab449c29cf44c78f97584f715770bb`.
First-difference report in this worktree:
`build/browser_world_one_verification_9000/first_difference_report.json`, SHA-256
`6ce92d93a6a39d85a055854973e272ac183908afbdd3dacd0fb6111feaa34772`.
Native PCM SHA-256 `4f6c7d1dcbaf5a84da2358a27f0d34b1edaa9b52807838809ad8e53c2b712242`.
Browser PCM SHA-256 `2380586cbf7a13023af801a44bf1e3eeb549068152edd1ffd931ef28ddfe26d2`.
Native top RGBA SHA-256 `2852ffe77e1c8991faaddf061e711935db0b8e847b9fa804eb1651a17078b276`.
Browser top RGBA SHA-256 `88bf2bfc05521874c4abdfad52a075c02c30f1624b2ceff1ad4982ba674cb276`.
Full raw captures, initial profiles, movies and provider sources remain local.
No World1-1 exact replay, full goal, Section7 or playable-speed credit.
Average9000prefix rate4.5085presentations/sec. First eligible frame53.17sec.

The current safe unit finished naturally: public360 verifier passed exact seven
raw files, original movie/snapshot delivery, Canvas output, source-packet identity
and consumption totals. Result `build/public_gameplay_replay_verification_360/result.json`,
SHA-256 `13e6104dd7db541d276a3e24f205a4fd79677397737b91449f4b9ef8b6fdad97`.
The public controls verifier is a preserved draft. Its corresponding private
actual all12-button/four-circle/touch/focus-release/Stop case passed2e2387f9,
but the final public controls command has not itself been run.
The earlier360 performance profile sampled startup file reads. No gameplay
CPU/rasterization cost split is credited. Corrected private frame gate is unrun.

## Arithmetic findings, proposals and next step

Scratch audio candidate and receipt are under `build/audio_numerical_diagnosis/`.
Candidate patch SHA-256 `c11793f730cdb97d9257c25bd24df914173a5ce420c32d86fa4395521eb5a21b`.
Diagnosis receipt SHA-256 `35e208e4b7f0539b330a0a92d993e194415132b48e82285c38e31663d4131bec`.
Actual native objects use fused gain ramps and front-product fused stereo downmix.
Synthetic1,048,576ramp and1,048,576stereo cases demonstrate native/Wasm rounding
changes; explicit std::fma with native operation order matches across targets.
The actual failing block's arithmetic operands are unrecorded. Attribution of
its1-unit error and real-provider correction remain unverified. No patch applied.

Graphics helper checkpoint: `build/graphics_numerical_diagnosis/pause_checkpoint.json`,
SHA-256 `ce0755e0e163f221f3327b3c1af98ab20525b07d19b095ef119828cda55c211f`.
Public sources match. Native object fog/lighting FMA is observed. Vec4<float>
ARM64 pairwise reduction differs from the scalar source order, but the relevant
clip/interpolation/shader paths also use f24, so that specialization is not a
proven explanation. No graphics numerical compile or correction patch exists.

After explicit resume, first read AGENTS/BRIEF/STATE and this checkpoint. Continue
step2 by proving the public graphics fog/reduction arithmetic in isolated
native/Wasm numerical cases, then compose a grounded provider overlay, rebuild
in a new ignored output and replay the unchanged original movie. Retain the
failed artifacts. Do not submit until the family's comparison gap is resolved.
Then continue WebGPU/performance, the natural goal route, typed Section7 state
through Pro, and whole-level browser acceptance, in the owner's recorded order.

The following exact commands reproduce the current failed9000comparison after
explicit resume. They do not incorporate any unverified arithmetic candidate.
Run the server in one terminal and the verifier in another. The server never
serves the dump. Both output directories must be absent.

```sh
cd /Users/exgota/super-mario-3d-land-browser
. ./development_environment.sh
python build/root_browser_gameplay_session/tools/static_recompiler/serve_browser_execution.py \
  build/root_browser_gameplay_session/build/browser_gameplay_module_final \
  build/root_browser_gameplay_session/build/resume_gameplay_server_9000 \
  --block-schedule build/root_native_block_scheduling/build/block_scheduled_native/block_schedule.bin \
  --reference build/root_browser_gameplay_session/build/browser_session_preparation/server_reference_9000 \
  --dump-sha256 c83f9175208b3d60898d3a6d60451bee3108521e409cace362f63b46dda8c976 \
  --dump-bytes 536870912 --observe-audio --stream-audio --frame-output \
  --gameplay-session-presentations 9000 --gameplay-input-mode replay \
  --wall-time-seconds 3600 --port 8766
```

```sh
cd /Users/exgota/super-mario-3d-land-browser
. ./development_environment.sh
python build/root_browser_gameplay_session/tools/static_recompiler/verify_browser_gameplay_session.py \
  http://127.0.0.1:8766/ \
  build/root_browser_gameplay_session/build/resume_gameplay_verification_9000 \
  --server-output build/root_browser_gameplay_session/build/resume_gameplay_server_9000 \
  --reference build/root_world_one_bridge_continuation/build/world_one_bridge_preparation/reference_world_one_bridge_8400_9000/capture \
  --movie build/root_world_one_bridge_continuation/build/world_one_bridge_preparation/reference_world_one_bridge_8400_9000/capture/input_movie.ctm \
  --dump build/root_port_reference/owned_dump.3ds --timeout-seconds 3600
```

## Preserved work and process disposition

Private sealed manifest: `build/browser_session_preparation/pause_checkpoint/evidence_manifest.json`.
Its source/parked patch inventories preserve pre-checkpoint uncommitted work.
The separate `root/guest-execution-tick-selection` worktree remains intentionally
uncommitted: GuestExecutionObservation.cpp/.h, StaticArmBackend.cpp, observation
note, and new GUEST_EXECUTION_TICK_SELECTION.md. Tracked binary patch and new note
copies are sealed in this checkpoint directory. No provider build/run used it.

Current360command83342 completed0 and its browser cleanup confirmed absence.
Current server24932 closed by SIGINT after exports, exit130. Earlier profile
server18768 and failed long server84235 were already closed. Long verifier31718
finished with failed equality and successful browser cleanup. All52 own root
browser registrations rechecked with zero active owner/client/daemon/browser/errors.
31 older root test servers, each with matching process identity and root worktree
cwd, received SIGINT; none retained the same identity afterward. Personal browser
sessions were left alone. All helpers completed; stop controls issued, none running.
Aquinas absent. No active root goal exists. No automatic resumption is authorized.

Original dump, code, all comparison inputs, final module/providers, raw evidence,
source and receipts are retained. Completed own cache retirement removed
275475387logical/270667776allocated bytes from two closed browser profiles;
required empty registry directories remain. SHA-256 receipt6450e23b.
The additional Node duplicate deletion was refused because an active module
manifest still binds that path. It remains intact. Free disk20.80GB at19:32UTC.
Capture limits remain finite:60000presentations,3600wall seconds,256MiBPCM,
160MiBfinal-screen budget and2GiBexports; selected mode creates no GPU/PICA files.
