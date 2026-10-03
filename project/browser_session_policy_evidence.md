# Owned browser session cleanup

Branch `root/browser-session-policy` freezes integrator main `9c93ef3674b20f8c98760529196e7ff999c2819c`. Source commit `4d84c439860a600b7351cc65fe239902a87a44f1` implements the accepted owner browser lifecycle policy in BRIEF Section 5. Only the integrator moves main, ranks and the ledger. This port/tools family has no matching claims or new translated instruction bytes. `recompiled_bytes` stays 2437712; milestones 5 and 6 remain in progress.

## Change and authority

All six browser verifiers now default to headless Chrome and use one shared session manager. Each test gets a fresh ignored workspace, random name, owned persistent profile, explicit configuration and prospective registration before launch. The Git common directory shares the registry and one-visible-preview lease across worktrees. A second headed request refuses while preserving the first; headless tests can coexist with the active visible fixture.

A detached supervisor starts before browser launch and holds the visible lease independently. It watches the owner's private pipe and closes the exact owned session after success, failure, timeout or owner death. Recovery of registered abandoned contexts precedes new admission. Current UID, PID birth/boot identity and exact CLI/profile arguments authorize fallback signals. Unknown ownership, changed birth or lost ownership refuses a signal. Same-birth terminal processes and confirmed absence are recorded without signaling. Cleanup progress retains actual signal/error receipts. No label, age, global browser inventory or broad close/kill command authorizes cleanup.

Passing verifier results are written only after a cleanup receipt establishes absence of live owned client, daemon and browser processes. Profiles, persistent metadata and evidence remain local. Personal Chrome and unregistered contexts are outside this cleanup scope. Historical `--keep-open` requests refuse before launch. The existing flag is retained solely to explain that refusal.

The manager verifies CLI0.1.22/core1.64.0-alpha-1790635538000 source pins. A child-only upstream configuration hook selects an empty owned global configuration directory; inherited Playwright overrides and Node injection settings are removed for that child. Frozen manager/OS-reader copies bind each running supervisor to its original source. Parent and personal CLI configuration are unchanged. The README supplies the installation, lifecycle and ordinary capture commands.

The OS reader uses public Apple SDK/libproc/sysctl interfaces and Linux proc interfaces. It exposes three-valued identity results and no cleanup action. Tool source derives from public SDK/CLI inspection and original project implementation, with no game data or leaked source.

## Final source and actual lifecycle controls

Commands ran from `build/root_browser_session_policy`, after sourcing the primary checkout's `development_environment.sh`. Python compilation and `git diff --check` passed. Private proofs and providers remain ignored.

```sh
python tools/static_recompiler/verify_browser_session_policy.py \
  build/session_policy_controls_final \
  --peer-repository /Users/exgota/super-mario-3d-land-browser/build/root_browser_touch_input
```

The final command exited 0. Its result is `build/session_policy_controls_final/result.json`, SHA256 `3fece37a037bc0d53ab4d91cc53b7accbd2e4eafdd119a817db6627b451f0231`. It seals these three primary sources:

| Source | SHA256 |
| --- | --- |
| browser_session_policy.py | def64bb446a2c658b7df4244167915bb4def67c6a8ca3bdf6a3ff8241965e53b |
| browser_process_identity.py | bf1d95a8c283d0112b1c97a5c6407f1e4a71e7fc777b68fce812231870d45735 |
| verify_browser_session_policy.py | 5017518d22d93dc568b16dae6aad4a1072b44aa3674acd222a43b8489c11c3c5 |

Eight actual lifecycle controls passed on this Darwin host with installed Chrome154:

- Headless normal completion and persistent metadata retention.
- Original fixture failure preserved while its owned context closed.
- Actual `run-code` timeout, required by its `timed_out=true` command receipt, followed by cleanup.
- Owner SIGKILL with a live owned persistent daemon/browser, followed by independent cleanup.
- Owner SIGKILL during actual launch. The saved observation contained a live client and daemon before session metadata; cleanup removed both. No metadata existed to retain in this partial case.
- Supervisor and owner SIGKILL with a detached context still alive, followed by next-launch recovery before a new context opened.
- Second-visible refusal from the Touch worktree, then headless admission there while the original visible owner and browser births remained intact.
- Dead visible owner cleanup before replacement visible admission from the other worktree.

All fixture signals require the stored PID/UID/platform/birth/boot identity, argv-bound identity and exact fixture output or supervisor snapshot, followed by another birth check. At most one visible fixture existed at a time. Six actual verifier invocations also refused `--keep-open` before creating their output. The primary result seals only its three primary sources; the independent source review separately seals the six invoked verifiers.

Opened-context cleanup receipts retain persistent metadata and report no live owned client/daemon/browser afterward. Ordinary opened cases required no fallback signal. The partial-launch receipt records one SIGTERM to its starting owned client; the daemon then finished without a further signal. These observations are scoped live-resource checks, not atomic machine-wide absence proofs.

## Independent source and OS evidence

Primary `build/root_browser_platform_investigation/browser_session_policy_review.md`, SHA256 `292497feb10d329effabdeac36301b4d0a6a3671c55f94a4dcea0261b808b2d2`, records the independent read-only audit of final source. It found no remaining blocking ownership/cleanup issue. It mechanically preserved every original verifier assertion/comparison contract; the stdout error check moved into the common manager. Circle/touch JavaScript differs only by indentation and normalizes exactly to its original literal values. This review supplies source evidence, not six new capture executions.

`build/process_identity_verification/result.json`, SHA256 `c5668a3935c43816caf08473060d3bea667dd661ff6e2e643091f029201b744b`, records 20 actual Darwin/SDK checks and 63 independent synthetic format/failure/ownership contracts. Independent SDK inspection agreed on PID/UID/parent/birth and the 136-byte struct ABI. A naturally exiting owned Python child demonstrated live identity, unreaped zombie identity and confirmed absence after reaping. Permission, malformed argv, conflicting profile options, ownership loss and PID reuse controls remain synthetic contract evidence. The helper launched no browser and sent no signal.

## Unchanged browser replay preservation

The accepted Touch module remains unchanged at `build/root_browser_touch_input/build/browser_touch_module_final`; its manifest SHA256 is `4f9ad2c2c49161701a39ca926d720dee6f16ad08b2ceef17da22ce968497e5cd`. The wasm SHA256 remains `63e495ab072a9a48eb2052344e263bc0df458ac7f63563b7131ffe11c1526b6d`. The server uses the original presentation60 reference, owned dump and block schedule with 22 sealed served inputs. The owned dump is not served, and no game data is committed.

```sh
python tools/static_recompiler/serve_browser_execution.py \
  /Users/exgota/super-mario-3d-land-browser/build/root_browser_touch_input/build/browser_touch_module_final \
  build/session_policy_browser_server \
  --block-schedule /Users/exgota/super-mario-3d-land-browser/build/root_native_block_scheduling/build/native_block_schedule_full/block_schedule.bin \
  --reference /Users/exgota/super-mario-3d-land-browser/build/root_port_rendering/reference_presentation_60 \
  --dump-sha256 c83f9175208b3d60898d3a6d60451bee3108521e409cace362f63b46dda8c976 \
  --dump-bytes 536870912 --port 8805
# In a second terminal with the same checkout and environment:
python tools/static_recompiler/verify_browser_input_identity.py \
  http://127.0.0.1:8805 build/session_policy_identity_final
python tools/static_recompiler/verify_browser_execution.py \
  http://127.0.0.1:8805 build/session_policy_guest_final \
  --dump /Users/exgota/super-mario-3d-land-browser/build/root_port_reference/owned_dump.3ds \
  --server-output build/session_policy_browser_server \
  --reference /Users/exgota/super-mario-3d-land-browser/build/root_port_rendering/reference_presentation_60 \
  --exercise-controls
```

Both final commands exited 0. The identity result SHA256 is `f52b03448872307a523b40991356b3fc1c9e44cc9481eb9ccc6b6f88a5a10d50`: ten actual small ABI fixture results, readonly write refusal and unchanged bytes. It loads no game main. Its cleanup passed.

The full guest result SHA256 is `8d4e4ae7937a2351a66a9728d005a5762fd909964e1a9d31e0b5f3f2da2876de`. Its capture is `build/session_policy_browser_server/capture_a5f3d2bf332d4de699a84ae7b1a3784a`. The unchanged rendered comparator SHA256 is `49ef688c02c8f0b4f8a619213054f6d72348a31433b09e3c88027984de344c3f`; comparison SHA256 `4d2ca7992ab6e8843f2496542e35bdf69544ac4a215c1e6dd252fd18db15c01c` passes all 1430 event bytes/ticks, 95 PICA files totaling 765728 bytes, screen metadata, 691200 RGBA bytes and 345600 raw framebuffer bytes at presentation60. Both visible canvas hashes match the original screen payloads. Wrong-size rejection, Stop and clean retry pass. Exported 24 directories and 8 guest files match; the preexisting host-log difference remains separately retained and explained by the original verifier. Input/audio observations were disabled for this default profile, so this rerun earns no new live A/Circle/Touch/audio or intermediate-frame delivery claim.

The final identity and guest registrations bind the same final manager/OS-reader hashes above. Their cleanup receipts passed before result publication. `build/session_policy_final_closure.json`, SHA256 `0a4eb84086522502b94fa202a42c962768a13edc0424c30af6322b6367f6f617`, rechecked25 registered contexts and found no live owned resource. It also matched independent review, final control and final guardian source seals. It did not use a global CLI inventory or inspect personal tabs.

Use absent output names and your local owned provider paths when reproducing these commands. Original completed receipts are retained; no command overwrites them.

Earlier prototype/controls_1/first identity/guest receipts remain historical, without final-source execution credit. Their copied sources and outputs were preserved. The final-source reruns above replace their acceptance role.

## Limits

Linux execution, actual permission-denied ownership, adversarial scheduler/PID reuse races, forced SIGKILL cleanup fallback, cleanup-error failure injection, and a command history above64 KiB remain unexecuted here. The source audit covers their refusal/bounds logic. The 1-second scoped quiet observation supplies no atomic global absence guarantee. Admission uses a shared blocking transaction and bounded work per retained registration, without one total admission deadline. Changes to pinned executable/source identities refuse admission and require investigation.

This family changes no matching build input, protected oracle, Game/backup/src/Factory source, rank cell or ledger row. It adds no ARMCC/full-map result claim. World1-1 entry/goal/state replay, continuous browser sessions/audio, sustained frame rate, physical mobile, saves and complete rank-O source adapters remain open.
