# Native software presentation

The native static CPU now reaches a visible title-logo presentation through the pinned Azahar software GPU. Stock Dynarmic and native execution use the same movie, initial user snapshot, services and renderer. The comparator requires exact event bytes and ticks, PICA payloads, screen metadata, RGBA pixels and raw active framebuffer bytes. This establishes bounded native software rendering. WebGPU, browser presentation, input and gameplay are separate milestones.

## Build the observer

Follow [NATIVE_PLATFORM.md](NATIVE_PLATFORM.md) to generate the timed native library and obtain the pinned public Azahar source. After its reference capture, deterministic-I/O, native platform and Dynarmic floating-point control patches, apply the optional presentation observer:

```sh
. ./development_environment.sh
git -C build/port_tools/azahar_reproduction apply --check \
  "$PWD/tools/static_recompiler/azahar_reference/azahar_render_capture.patch"
git -C build/port_tools/azahar_reproduction apply \
  "$PWD/tools/static_recompiler/azahar_reference/azahar_render_capture.patch"
build/port_tools/azahar_reproduction_python_packages/cmake/data/bin/cmake \
  --build build/port_tools/azahar_reproduction_build \
  --target azahar_gpu_capture azahar_static_execution -j 1
```

The observer reads the existing software renderer after its natural VBlank `SwapBuffers`. It does not inject a presentation, change a framebuffer, process another command list or replace the renderer. With `ROOT_PORT_CAPTURE_PRESENTATION` unset, the historical first-top-screen-submission boundary remains unchanged. A positive decimal count from 1 through 3600 selects that many natural presentations after the first top-screen submission. Invalid selections fail.

## Record and replay

Every output directory must be absent and ignored. Use only the approved local dump copy. A fresh recording creates its own movie and pre-CPU user snapshot. Use that same pair for both stock replay and native replay:

```sh
ROOT_PORT_CAPTURE_PRESENTATION=60 \
build/port_tools/azahar_reproduction_build/bin/Release/azahar_gpu_capture \
  build/root_port_reference/owned_dump.3ds \
  build/root_port_rendering/reference_60
ROOT_PORT_CAPTURE_PRESENTATION=60 \
build/port_tools/azahar_reproduction_build/bin/Release/azahar_gpu_capture \
  build/root_port_reference/owned_dump.3ds \
  build/root_port_rendering/reference_60_replay \
  build/root_port_rendering/reference_60/input_movie.ctm \
  build/root_port_rendering/reference_60/initial_user_state
ROOT_PORT_CAPTURE_PRESENTATION=60 \
build/port_tools/azahar_reproduction_build/bin/Release/azahar_static_execution \
  build/platform_timed_translation/translated.dylib \
  build/root_port_reference/owned_dump.3ds \
  build/root_port_rendering/native_60 \
  build/root_port_rendering/reference_60/input_movie.ctm \
  build/root_port_rendering/reference_60/initial_user_state
python tools/static_recompiler/compare_rendered_capture.py \
  build/root_port_rendering/reference_60 \
  build/root_port_rendering/reference_60_replay \
  --report build/rendering_stock_repeat.json
python tools/static_recompiler/compare_rendered_capture.py \
  build/root_port_rendering/reference_60 \
  build/root_port_rendering/native_60 \
  --report build/rendering_native_comparison.json \
  --preview-directory build/rendering_native_preview
```

Repeat native execution into another absent directory, then compare the two native captures. The library suffix is `.so` on Linux. Only macOS arm64 has been observed. The platform executable refuses missing translations and interpreter/JIT fallback; retain its stderr instruction-count receipt.

Reports and previews must be absent paths under the invoking checkout's ignored `build`. Preview PNGs encode the captured RGBA bytes directly, using Python's standard library. They are owner-derived images and must never enter Git. Color-count diagnostics help inspect content, but cannot relax pixel equality or establish geometry. A black or color-fill-only screen is insufficient rendering evidence.

## Capture convention and observed scope

`frame_presentation` records the renderer frame, zero-based selected presentation index, first top-screen submission ticks and natural VBlank index. Its two screens are top-left (identifier 0) and bottom (identifier 2). Landscape RGBA dimensions are transposed from software framebuffer storage. Raw active left framebuffer bytes retain hardware stride, height, addresses, format, active-buffer register and LCD fill. The legacy `frame` field remains zero; it is not the selected presentation index.

The first presentation was black. At presentation 60, the top screen shows the title logo during a dark fade, with 742 RGB colors and no LCD color fill. Both final native replays agree exactly with stock on all 1,430 events/ticks, 95 PICA lists containing 765,728 bytes, 691,200 RGBA bytes and 345,600 raw framebuffer bytes. Each executes 343,255,755 guest instructions with zero interpreter/JIT fallback. A stock replay also agrees exactly. [The evidence note](../../project/rendered_frame_capture_evidence.md) records final source hashes and local receipts.

The bottom screen at this boundary is effectively one background color. The pinned stock decoder reads one pixel beyond its nominal extent, causing the observed corner pixel. This observer preserves the stock output and also retains the exact raw framebuffers; it does not silently correct the reference. Stereoscopic output, later scenes, long-run timing, standalone WebGPU rendering and browser execution remain unverified. This milestone adds no matching credit, and accepted-source bindings beyond the priority adapter remain incomplete.
