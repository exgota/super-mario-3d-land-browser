# Finite GPU observation window

This optional capture mode executes the complete natural guest path and exports
GPU diagnostics only within a declared presentation interval. It does not skip
startup, seek the movie, change guest memory, inject a saved RAM state, or alter
GPU execution, scheduling, input, audio or services. An omitted GPU prefix is
missing evidence. A successful window comparison never earns whole-prefix GPU,
completed-update, Section 7 game-state or milestone credit.

Apply `azahar_gpu_observation_window.patch` after the accepted
`azahar_gameplay_capture_bounds.patch` at its 8,400-presentation/3 GiB extent.
The new patch changes only private `gpu_command_capture.cpp` and
`headless_capture.cpp`. It adds no headers, CMake sources or guest addresses.
Use new isolated source/build copies. Preserve all previous sources, providers,
movies, snapshots and captures.

Set `ROOT_PORT_CAPTURE_BEGIN_PRESENTATION` to a positive decimal, 1-based
inclusive beginning. Window mode also requires explicit decimal
`ROOT_PORT_CAPTURE_PRESENTATION` (inclusive end),
`ROOT_PORT_CAPTURE_WALL_SECONDS` and `ROOT_PORT_CAPTURE_PICA_BYTES`. Require
`1 <= begin <= end <= 18000`, wall seconds from 1 through 3,600, and recorded
PICA bytes from 1 through 3,221,225,472. Empty, signed, malformed, missing or
out-of-range selections fail. File, total-output, outer-time and free-disk bounds
remain the external controller's responsibility. There are no new SDK flags for
those host resource controls.

When the begin option is unset, the accepted capture behavior remains unchanged:
the original `gpu_events.jsonl`, first top-screen swap when presentation is
unset, presentation selection at most 8,400, PICA default 128 MiB/maximum 3 GiB,
and wall default 120 seconds/maximum 3,600 seconds. The old event format,
payload names, completion paths and complete-capture comparers are retained.
Actual preservation still requires root's unchanged-mode controls.

For beginning N greater than 1, export arms immediately after the natural
presentation N-1 callback. Commands observed afterward, including commands for
the next presentation interval, are exported through the selected final
presentation. Beginning 1 arms before guest load. Its arming guest tick and
renderer frame are explicitly `null`; the coverage/initial-arm events use zero
as a metadata placeholder, not an observed guest tick. Later arming records use
the actual N-1 tick and renderer frame. The final presentation and closing event
identify the actual end tick. These are diagnostic interval boundaries, not
proof of instruction retirement, completed gameplay updates, or ownership of
each asynchronous GPU command by one rendered frame.

Window mode writes `gpu_window_events.jsonl`, and never creates
`gpu_events.jsonl`. Existing whole GPU/rendered/movie comparers therefore cannot
accept a window dataset. Its separate contiguous event sequence begins at zero
with a coverage declaration and arming record. Retained GSP command indices,
PICA list indices/names, VBlank indices, addresses, words and guest ticks stay
absolute. No recorded trace is normalized, rewritten or spliced.

The arming record supplies counters for observed but unexported startup. The
closing record and complete footer supply `recorded` and `total_observed`
counters for GSP commands, PICA lists/bytes, buffer swaps, VBlanks, hardware
register writes and color fills. For a complete window, total observed equals
unexported prefix plus recorded for every counter. PICA's 3 GiB budget applies
to exported payload bytes; total observed bytes can exceed it. Legacy footer
`gsp_commands`, `pica_lists` and `vblanks` mean total observed in window mode;
`payload_bytes` means recorded. The explicit counter objects disambiguate them.
After the final presentation closes export, subsequent callbacks are outside
this diagnostic scope. Failure footers remain incomplete, including runs that
never arm, fail a payload/event write, exceed bounds, or miss the selected end.

When enabled, existing `input_events.jsonl`, original movie and HLE stereo
`audio_events.jsonl`/PCM span the complete natural run to the selected end,
including startup. The optional existing state-observation callback remains
unchanged and is not gated by the GPU export window. Full input/audio equality
does not replace the omitted GPU prefix. Only the final selected presentation's
two existing RGBA/framebuffer pairs are exported; this does not add intermediate
screen captures or provide a self-contained rendering replay without startup
GPU state.

`compare_window_movie_replay.py` is a separate strict validator. It requires a
coverage header, matching configuration, one correctly positioned arming event,
contiguous absolute callback indices, exact counter joins, a final software
presentation/VBlank, matching closing event, and complete final outcome. It
checks all PICA/RGBA/framebuffer extents and rejects unreferenced payloads. It
then compares actual event and payload bytes, original movie delivery, full-run
input, full-run stereo PCM and their timing. No timing or trace normalization
participates in acceptance. Only the declared stock/native backend identity
differs in configuration; every other configuration field must agree. Existing
input/movie validators and pure payload/pixel helpers are reused without edits.
The audio validator retains the existing block, rate, channel, extent and final
boundary checks against the explicit window boundary, without pretending an
old whole-prefix GPU stream exists.

The validator limits GPU logs to 256 MiB, individual JSON lines to 64 KiB, events
to 2,000,000, PICA files to 1,000,000 and aggregate recorded PICA to the selected
bound/3 GiB maximum. Existing 64 MiB input/audio and 16 MiB movie limits remain.
Reference logs are bounded to 64 MiB. Raw PICA equality uses chunked byte reads,
and seals ensure validated files do not change during comparison. Optional PNG
previews preserve the captured pixel bytes in an absent ignored directory.
Reports always state `full_gpu_prefix_compared: false` and
`game_state_semantics_compared: false`.

Root must independently review/build the ignored isolated recipe, verify
unchanged first-swap and original 360-presentation full captures, exercise begin
1/later/exact-end and refusal/incomplete controls, and replay each new window's
own original movie and recorded `initial_user_state`. Each recording/replay
controller must require absent owned output, protect original dump/profile,
provider/module/schedule/source and copied input identities before/after, clear
inherited `ROOT_PORT_*` variables in its child, select only the intended options,
monitor finite wall/outer/PICA/file/per-file/total/free-disk bounds, preserve all
failures, require every full input/audio/GPU-window footer, and check zero native
fallback on both CPUs. Neither this patch nor its comparer supplies those
controllers or changes their inputs. No saved profile's file presence establishes
its loaded phase, actor identity, timer semantics, control or goal.

This handoff is source preparation only. Compilation, unchanged-mode output
preservation, actual window alignment, exact original/native window replay,
control, goal and Section 7 comparable state remain for root's real checks.
