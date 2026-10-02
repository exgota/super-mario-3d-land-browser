# Passive raw RAM observations

The optional source-only `azahar_state_observation.patch` adds an external, bounded read plan to the configured software-renderer reference and native frontend. It contains no game addresses, layouts, generated guest code or state. Apply after the accepted replay payload extent patch and preceding default-applet, audio, input and rendering sequence. This dependency must land before submitting or reproducing the combined family.

```sh
. ./development_environment.sh
git -C build/port_tools/azahar_reproduction apply --check \
  "$PWD/tools/static_recompiler/azahar_reference/azahar_state_observation.patch"
git -C build/port_tools/azahar_reproduction apply \
  "$PWD/tools/static_recompiler/azahar_reference/azahar_state_observation.patch"
build/port_tools/azahar_reproduction_python_packages/cmake/data/bin/cmake \
  --build build/port_tools/azahar_reproduction_build \
  --target azahar_gpu_capture azahar_static_execution -j 1
```

Set `ROOT_PORT_STATE_PLAN` to an ordinary local file. Unset leaves the observer inactive, without files or payload reads. The plan uses printable ASCII, single spaces and a final LF. Names use bounded lower snake case. This generic example describes synthetic memory, not a game field:

```text
version 1
frames 120 240
field raw_field 12 0x40000000 0x0 0x4
```

A field gives name, byte count, base address and one through sixteen offsets. For every offset except the last, read four original little-endian pointer bytes at current address plus offset; that pointer becomes the next current address. The last offset selects raw bytes. A single offset therefore performs a direct read. All arithmetic and extents stay inside the 32-bit guest address space. The observer reacquires the current guest page table for each selected frame.

The implementation validates every required page before copying. It accepts only ordinary mapped RAM with nonnull page pointers. It does not invoke MMIO, memory fallbacks, rasterizer flushes, guest helpers or virtual dispatch, and does not change guest scheduling. Null pointers, cached/watched/unmapped pages and overflow produce explicit unavailable results. A readable zero field stays real zero bytes; an unreadable field never becomes fabricated zeros.

Bounds:1..1024strictly increasing renderer frames,1..128unique fields,1..256bytes per field,1..16offsets,64name bytes,256KiBplan input and16MiBaggregate output including its canonical receipt. A conservative worst-case preflight prevents oversized observations. Existing or symbolic-link observer outputs are refused.

The hook executes on the emulation thread in `GPU::VBlankCallback`, inside the existing presentation observer, after software `SwapBuffers` returns and before that callback reschedules its recurrent event. It stops observing after GPU capture completes. Records retain actual renderer frame, guest ticks and VBlank index; these counters remain separate. This is an operational RAM sampling edge, not a certified completed player/camera/timer update. The caller must preserve stopped guest execution and a stable page table during reads. Other renderers or concurrent guest-writing configurations are outside this validation.

The separate `state_plan.txt` and `state_events.jsonl` retain exact bytes and pointer-read traces. Skipped or unreached requested frames emit explicit unavailable-frame records. Duplicate callbacks do not reread a selected frame. Reversing frame/tick/VBlank metadata raises. The outcome counts missing/unavailable observations; complete requires successful capture termination, all requested frames observed and all fields readable. Invalid configuration returns2 before guest loading; failed loading/movie playback finalizes missing observations as incomplete. Existing callback exceptions may terminate the stock process and earn no success credit.

Record on stock and replay the original movie with its own pre-CPU user snapshot on native, using the same plan and capture boundary. Follow [AUDIO.md](../AUDIO.md) for the existing executable/library arguments. Compare with:

```sh
python tools/static_recompiler/compare_state_observation.py \
  build/root_port_state_observation/reference build/root_port_state_observation/native \
  --movie build/root_port_state_observation/reference/input_movie.ctm \
  --report build/state_observation_comparison.json
```

The comparer validates canonical plan coverage, exact schema/types, pointer bytes and addresses, complete outcomes and recorded VBlank/presentation anchors. It compares raw observation event bytes and reports the first different renderer frame. It also requires the unchanged movie/input/GPU/PICA/pixel/audio checks. Matching raw bytes does not establish field identity, live player/camera selection, RNG coverage, timer/coin semantics or completed gameplay-update phase. Missing observables do not satisfy Section7. All plans, captures, movies, snapshots, data and previews remain ignored.

[The evidence note](../../../project/state_replay_observation_evidence.md) records final source hashes,86mapped-memory controls,38comparer controls and actual stock/native preservation. No World1-1 or browser completion is claimed.
