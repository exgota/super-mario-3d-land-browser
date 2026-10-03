# Finite stage capture range

The completed original/native navigation comparison reached the World 1-1 start
card at 7,200 selected presentations. Playable stage pixels and control remain
unverified. This source family permits a finite longer capture without changing
guest execution or supplying game state.

`azahar_gameplay_capture_bounds.patch` now permits
`ROOT_PORT_CAPTURE_PRESENTATION` from 1 through 8,400 and
`ROOT_PORT_CAPTURE_PICA_BYTES` from 1 through 3,221,225,472 bytes (3 GiB).
Both existing PICA validation sites have the same ceiling and error text.
`compare_rendered_capture.py` permits presentation indices through 8,399. All
other loader assertions and equality checks remain unchanged.

The defaults remain the first top-screen swap when presentation selection is
unset, 128 MiB of aggregate PICA payload, and a 120-second wall limit. Strict
decimal parsing, payload copying, events, completion/failure behavior, movie,
input, audio, observation, timing and scheduling remain unchanged. The existing
wall selector already accepts 1 through 3,600 seconds; this family changes no
wall source code. The proposed longer run selects 2,400 seconds explicitly.

Apply this patch to the accepted private provider source closure after the
existing `STATE_OBSERVATION.md` patch sequence, in place of the earlier
7,200/2 GiB version of this same patch. Do not stack both versions. The existing
`GAMEPLAY_CAPTURE_BOUNDS.md` records the earlier family and is retained unchanged.

The ignored `build/build_isolated_stage_capture_provider.py` prepares an isolated
build from the sealed successful bounds provider receipt. It checks and applies
this patch only to new private source copies, compiles GPU capture plus the
stock and native headless variants, replaces exactly one member in each of two
copied bounds archives, and relinks the two original frontends. It preserves the
previous provider, explicit source/header/object/archive/link inputs, installed
toolchain aliases, and the original translated module and block schedule. The
independent execution-observer provider is outside this build closure. The
compiled-module frontend and implicit system linker/runtime closure are not
verified by this recipe.

Root must build the isolated provider, check an original 360-presentation
stock/native movie with the full comparers, then record and replay a new finite
original movie. Root's initial proposed selection is 7,800 presentations with
the existing navigation input script continuing naturally at its final neutral
state. The proposed capture selections are
`ROOT_PORT_CAPTURE_PRESENTATION=7800`,
`ROOT_PORT_CAPTURE_WALL_SECONDS=2400`, and
`ROOT_PORT_CAPTURE_PICA_BYTES=3221225472`, with a 2,460-second monitored outer
limit. Input scripts, output resource limits,
original recording and exact replay remain separately selected and checked by
root. This source-only preparation establishes no successful build, capture,
playable stage, Section 7 comparable state, or milestone completion.
