# Browser streamed audio startup buffering

## Scope and source

Base: `0ae15a51bc76722dbffcff3b99f6864082d5d1b6`.
Branch: `root/browser-audio-pacing`.
T3 thread: `mcp:83f08952-217f-48a1-aefc-168a91d5aa7d`.
T3 and Git independently confirm the worktree binding at
`/Users/exgota/.t3/worktrees/super-mario-3d-land-browser/root-browser-audio-pacing`.

Only `BrowserStreamedAudio.mjs`, `BrowserAudioWorklet.mjs` and this report belong
to this lane. No existing page, worker, renderer, C++, STATE, daily report,
matching input, factory, ledger or rank changed. No game, Wasm, assets or replay
entered this lane. No migration, submission, push, deployment or schedule ran.

| Source | SHA256 |
|---|---|
| `runtime/port/browser/BrowserStreamedAudio.mjs` | `e8d253047ef458baaad95b66b5bcfddc6b8dfaee74c59780b38617f21a077ee4` |
| `runtime/port/browser/BrowserAudioWorklet.mjs` | `ab9dd0c0638df078a00e16646de5239cbcb3c88a4ab038e4b07fba131da694e6` |

The commit carries source and this report only. The final report hash is supplied
in the handoff outside this self-referential file.

## Opt-in behavior

Call `playback.enableStartupBuffer(minimumFrames)` while the stream is idle,
before `start()`. The permitted extent is 1 through 63,489 source stereo frames.
4,096 frames is the synthetic candidate, not a selected game setting.

The default remains zero and sends no new processor option or statistics field.
The enabled path reports `startup_buffer_frames` and passes that value to the
worklet. The worklet waits for the threshold before consuming its first source
frame. Startup output zeros remain counted as `startup_silence_frames`. A valid
`finish()` releases a short stream below threshold. After source consumption
begins, starvation remains `underrun_frames`; playback does not rebuffer, stretch,
drop, repeat, interpolate or manufacture source samples. Existing packet,
generation, notification, receipt, validation and terminal contracts remain.

The bound is `capacity - maximum_packet + 1`. Below that threshold, every legal
packet still fits. An initial 65,536-frame upper bound failed with variable packet
sizes: queued source stayed below the threshold while the next packet could not
fit, so neither producer nor consumer progressed. The failed receipt and exact
source/fixture remain under ignored `build/audio_pacing/`.

## Non-browser evidence

Final-source verification is
`build/audio_pacing/final_source_verification.json`. It records source hashes,
fixture hashes, process exit statuses and timestamped sanitized host snapshots.
The three checks ran October 4, 2026, around 10:38:19 UTC with requested nice 10. Snapshot
load averages were 3.531/3.379/3.505 on 10 logical processors; inspected full-width
process arguments found no factory. This is diagnostic context, not proof of
an uncontended machine. Process CPU values are lifetime averages from `ps`.

The sealed receipts are:

- `build/audio_pacing/sealed_buffer_boundary_result.json`: 416 synthetic cases
  pass. Thresholds include 0, 1, 127, 128, 129, 2,048, 4,096 and 63,489; source
  extents include empty, short, quantum, packet, capacity and ring-wrap boundaries.
  Output quanta are 128 and 256 frames. Burst and paced producers both pass.
  All 52 default-mode comparisons equal the frozen worklet's complete output
  quantum hash, notifications, receipts and source observation hash.
- `build/audio_pacing/sealed_final_playback_lifecycle_result.json`: the real
  controller and worklet modules run with mock AudioContext/MessagePort delivery.
  Default 216,000-frame output, complete receipt/event history and invalid packet
  errors equal the frozen base. Caller mutation after append does not change
  output. Pending start, full-ring capacity wait and drain cancellation close
  the graph; stale generation fails; suspended start reuses its graph; same-stream
  terminal restart refuses. A fresh short configured stream drains. Invalid
  option extents and configuration after start refuse.
- `build/audio_pacing/sealed_final_worklet_contract_result.json`: 216,000
  deterministic channel-distinct signed-16 little-endian stereo frames at
  32,728 Hz, 2,048-frame packets and bounded deterministic +/-20 ms virtual
  arrival jitter. Every emitted source float equals its input divided by 32,768.
  The source Float32LE hash is
  `1bd9f28316e77ee9b73271a71b4404caa8067f66bbe5404745f45db9dd01e466`.
  Default output and all notifications equal the base exactly. With the 4,096
  threshold, underflows fall from 768 frames to zero, while 2,048 startup zeros
  remain counted. First source output occurs at 62.576 ms of virtual time.
  A deliberate one-second source gap still reports 31,360 underrun frames.
  Empty/short finish, wrong-generation/duplicate refusal and stopped output pass.

`buffer_boundary_failure_001.json`, `capacity_failure_BrowserStreamedAudio.mjs`,
`capacity_failure_BrowserAudioWorklet.mjs` and the initial fixtures preserve the
failed capacity candidate. Earlier successful receipts remain separately retained.
Node syntax and diff whitespace checks pass. These are synthetic contracts, not
an actual software360 game replay or a browser/speaker/performance observation.

| Final non-browser receipt | SHA256 |
|---|---|
| `final_source_verification.json` | `c395dee099589bf2abaa35e2a315ef32a598dd304f5dcdf0dfdaf5d8f5124adb` |
| `sealed_buffer_boundary_result.json` | `fb42eb9aa188dac58f375b698273a2532f505dfababaa04be7d794a95fa3e40c` |
| `sealed_final_playback_lifecycle_result.json` | `25301bcef4ff99f193a5cdf3cab6a4cf1597d9f519842c79b22ca541fcaead3f` |
| `sealed_final_worklet_contract_result.json` | `63094edd5e9db822eeb7e2d7bad5503fc4ee74f0c39820d70f62b08dcfc2d56a` |

## Browser measurements

Root explicitly released October 4, 2026, 06:40:00 through 06:42:00 ET. No game
or other browser measurement was admitted by this lane. All own browser/server
helpers closed before the cutoff. The completion message's durable delivery
timestamp was 06:42:04.570 ET. An initially stated 06:41:32 handback timestamp was
incorrect and was corrected by a read-back-confirmed message. No extension or
further browser measurement occurred.

HeadlessChrome 154 measured 10:40:37.996 through 10:40:54.273 UTC. Three sequential
streams contain 98,304 synthetic channel-distinct stereo frames each. Every source
sample copied from the actual AudioWorklet output equals independently generated
signed-16 input divided by 32,768. Their common Float32LE SHA256 is
`6c8ecc776f8ba495c23bcb67724ef1218a25f944f9eb1526f0be2a5f057ff6fe`.

| Chrome scenario | Default | 4,096-frame buffer | Buffer plus 600 ms source gap |
|---|---:|---:|---:|
| Measured steady arrival, source frames/second | 32,874.317 | 32,861.971 | Deliberately interrupted |
| Maximum absolute arrival jitter, ms | 21.513 | 22.008 | 22.039 excluding deliberate gap |
| Start request to ready, ms | 169.100 | 3.200 | 7.900 |
| First observed consumption after producer begins, ms | 63.700 | 145.400 | 145.400 |
| Startup zero output frames | 0 | 0 | 0 |
| Underrun output frames | 0 | 0 | 14,464 |
| Source frames accepted/consumed | 98,304 | 98,304 | 98,304 |

The first consumption observation is an acknowledgement/progress measurement,
not an exact first audible sample timestamp. All values and offered/acknowledged
packet times remain in the receipt. Planned jitter was +/-20 ms; observed absolute
jitter was bounded below the fixture's predeclared 45 ms limit. Both uninterrupted
arrival rates are within 2% of 32,728 Hz. Cold first-context startup and later
context startup are not comparable latency experiments. Zero startup zeros means
none were emitted by the worklet, not zero wall delay. Both uninterrupted streams
have zero underruns, so this fixture establishes no Chrome improvement.

Real pending append and drain cancellation pass. Same-stream terminal restart
refuses; a fresh seven-frame buffered stream starts, drains below its threshold,
and closes. Every natural finish closes its AudioContext. No cancelled stream
manufactures an output observation or final receipt.

The Chrome launcher ran in its own detached session with a fresh Playwright-owned
headless context. It used the installed browser API directly to avoid shared
registry/configuration writes. Browser closure was awaited. The launcher PID was
54,380; sampled descendants were 54,381, 54,440, 54,441, 54,457, 54,459, 54,460 and
54,465. All are absent in the supplemental cleanup receipt. The launcher process
group was also absent after ordinary closure. These proofs cover recorded owned
PIDs and awaited closure; the sampled inventory does not prove that every possible
Chrome helper was enumerated. No personal browser process was controlled.
The Chrome local HTTP server ran in owned Python process 54,378, which is also
verified absent in `server_owner_absence.json`.

Seventeen resource snapshots record actual load and sanitized process PID,
parent PID, CPU/RSS/nice, with full-width command text inspected. Load began at
3.142/3.273/3.438 and ended at 3.509/3.355/3.465 on ten logical processors.
Chrome's sampled parent process has PID 54,381, nice 0, 236,512 KiB RSS and 5.1%
lifetime CPU. These values do not prove contention-free execution. The broad
`factory.py` text detector flagged the last two snapshots at 10:40:53.968 and
10:40:54.271 UTC. Those matches lie outside the retained top-30 process list;
their attribution cannot be reconstructed. They might be shell command text.
Factory presence/absence during those samples is therefore unknown. No factory
restart, stop or other control was attempted. The original flagged receipts stay
unchanged, and an attribution limitation is recorded separately.

Native Safari driver admission ran 10:41:12.928 through 10:41:13.982 UTC. Safari
refused session creation: “You must enable 'Allow remote automation' in the
Developer section of Safari Settings to control Safari via WebDriver.” No session
or Safari audio test started, and no setting changed. The owned driver exited and
local server closed. The failed receipt is retained in its own directory.

Browser evidence remains under ignored `build/audio_pacing/chrome_window_001/`
and `build/audio_pacing/safari_window_001/`. The served module hashes equal the
final source hashes before and after both runs. The Chrome actual receipt,
independent verification, resource window and supplemental cleanup are separate
files; Safari's refused admission is a failed result, not a browser pass.

| Browser receipt | SHA256 |
|---|---|
| `chrome_window_001/actual_browser_receipt.json` | `8e33e39cf0dae643c778039c33f83b93a0b646c141c59dac6477e3f8da988f54` |
| `chrome_window_001/result.json` | `90a28ba140bcda0b1d8cf3b15e31d7028ac87276b3f21ee8d05a53e93f3e5305` |
| `chrome_window_001/resource_window.json` | `5b059b5deceb6ad91575d9b2a57633f9ba9916ef28aedc6d84c23b1b1ca46777` |
| `chrome_window_001/supplemental_owned_cleanup.json` | `225a616e6151cca0a77fdc1dba47f5b74e98600cbcc78761b4f8de2833a4d0a2` |
| `safari_window_001/result.json` | `0647ddad0200155a8f9c785d5150e9f6a3f595c8fbcfe38e6fa376f6c3bc9422` |
| `resource_attribution_limitation.json` | `01be7cdf56abe881815ef957cfa75df4a5f852b6e21eb8a69fba273e0173c891` |

## Integration limits

Root must choose whether to enable the option and its extent. This source alone
changes no existing caller. A finite startup buffer absorbs bounded jitter but
cannot sustain a producer that supplies less than 32,728 source frames/second.
It also adds startup latency and does not establish audio/video synchronization.

Actual synthetic Chrome graph behavior passes within the scope above. Safari
graph behavior remains unobserved. Physical output, Safari device
resampling, actual game production, default software360 regression and human
World1-1 start-to-goal with sound remain Root's checks. M2 peer acceptance follows
that human M1 gate. Synthetic output supplies no performance or milestone credit.
