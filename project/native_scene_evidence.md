# Native scene definition and fixed collision instances

Verified on 2026-10-01 with Apple Clang 21.0.0, C++20, AddressSanitizer and UndefinedBehaviorSanitizer. The standalone reader remains separate from Game/al and the ARMCC matching build. Scene mode preserves every placement field, encoded string, rail and layer record. It resolves only the fixed map-part initialization path recovered below. Other actor initializers and placement categories remain explicitly unresolved.

Retail `initPlacementMap` at 0x00274EF0 reads `StageData` -> `AllInfos` -> the requested category, walks the array in order, resolves its object name through the actor factory, and calls the actor initializer. This entry path has no layer-name filter. The factory lookup at 0x00268EB0 reads `ObjectName` and `ClassName` from its conversion table and returns the first match. Scene mode applies that lookup to `ObjInfo`; it preserves `LayerInfos` without interpreting other selection paths.

The factory's fixed map-part creator at 0x00396404 calls the constructor at 0x0012C328. Its vtable points to initialization at 0x0012C228, which creates the quaternion pose keeper and calls the map-part helper at 0x002D5664/0x002D5668. The helper uses the placement name and `ShapeModelNo`: absent or -1 uses the unnumbered name and `ObjectData`; a nonnegative number appends at least three decimal digits and uses `MapPartsData`. A negative value other than -1 retains `ObjectData` while numbering the model name. The 16 owner placements validated here all use -1. Positive and other-negative branches have synthetic checks, rather than owner-resource coverage.

Initialization at 0x002417E8 loads `InitActor`. Its `Model` key requests a model, including the observed null-valued key. Its `Collision` key supplies an optional `Name`; 0x00242014 defaults that name to the resource basename. Collision loading at 0x0024F8CC requests the `.kcl` member and calculates the initial actor matrix through 0x00266EF4. Joint transforms and pose overrides remain unresolved. CGFX catalogs verify the requested model identity; mesh and material decoding remain outside this slice.

The quaternion pose update at 0x001DB88C multiplies degree fields by float32 constant 0x3C8EFA35, then calls quaternion construction at 0x0026E674. Its formula corresponds to Rz * Ry * Rx. Base-matrix calculation at 0x00334F90 writes rotation and translation; 0x002DDC68/0x002DDC70 multiplies the three rotation columns by their scale components. Scene mode preserves the float32 inputs and the observed operation order, including quaternion intermediates, matrix subtraction order and separate column scaling.

Ordinary C++ now reconstructs the bounded finite branches of retail sine at 0x00287908 and cosine at 0x00287AD0. Scalar coefficients retain their observed binary32 representations. Range reduction follows the signed rounding-bias operation and four sequential remainder corrections. Arguments whose absolute bit representation reaches 0x46490E49 require the unreconstructed large-argument reducer at 0x00211F48 and are rejected explicitly. The calculation requires round-to-nearest-even. A scoped `#pragma clang fp contract(off)` around this native math block preserves the separate multiply/add rounding observed in VFPv2; ARMCC Game/al flags remain unchanged.

Independent Python comparison validates all 298 placements, five layer records and 26 rails, including 7,947 float32 values by their bits, 13,693 integers and 2,171 encoded strings across the preserved records. All 16 fixed resource bindings resolve. Six initial collision instances contain 1,666 triangles. Independent geometry composition differs by at most 1.82e-12 in double arithmetic; the maximum matrix difference from an independent ideal-trigonometry Euler formula is 1.20e-7. The retail polynomial approximation accounts for that remaining analytical difference. These checks establish decoding and composition, not gameplay.

Unicorn 2.1.4 executes the unchanged retail degree/quaternion, sine/cosine, matrix and column-scaling routines with ARM1176, FPSCR=0, and a read/execute-only code mapping. All 192 matrix components from the 16 owner placements now match the reference by bits. A three-axis, nonuniform-scale fixture also matches every component. A broader bounded check compares 2,018 sine/cosine results from 1,009 inputs and 1,404 matrix components from 117 poses, including those owner placements, signed zero, threshold neighbors, negative scale and randomized angles. All 3,422 values match under sanitized debug and `-O3` builds. Emulator arithmetic and this narrow routine check do not establish hardware or frame-by-frame replay parity.

Correction to the initial report: its 12 apparent signed-zero differences came from parsing JSON `-0` as integer zero. Direct raw-bit comparison of the earlier source at commit 4197223 finds zero signed-zero differences and four numerical matrix-component differences, with a maximum of 5.97e-8. The reconstruction removes those four measured differences. Scene output now includes `matrix_bits` beside the numeric matrix, so JSON round trips retain signed-zero evidence.

Seven malformed scene-input checks and one unsupported-angle check pass without partial reports or sanitizer failures. The malformed cases cover duplicate resource keys, a wrong resource directory, a missing factory object-name key, a malformed factory root, a missing rotation component, a nonfloat rotation component and a missing actor initialization table. The previous 49-file inspection reproduces every recorded numeric, geometry and catalog result. All 50 extracted file hashes, the working-copy hash and the original dump hash remain unchanged. No assets or generated scene diagnostics are committed.

Reproduce scene assembly after local extraction:

```sh
. ./development_environment.sh
clang++ -std=c++20 -Wall -Wextra -Wpedantic -Werror -fsanitize=address,undefined -g runtime/AssetReader.cpp -o build/runtime/asset_reader
python - <<'PY'
import json
from pathlib import Path
import subprocess
directory = Path('data/runtime/romfs')
manifest = json.loads((directory / 'world_one_extraction.json').read_text())
factory = json.loads((directory / 'scene_factory_extraction.json').read_text())['files'][0]['path']
course = json.loads((directory / 'world_one_discovery.json').read_text())['world_one_course']
stage = f"StageData/{course['Stage']}Map{course['Scenario']}.szs"
paths = [entry['path'] for entry in manifest['files'] if entry['path'].startswith(('ObjectData/', 'MapPartsData/'))]
arguments = ['build/runtime/asset_reader', '--scene']
arguments += [str(Path('data/runtime/extracted') / path) for path in [factory, stage, *paths]]
with (directory / 'native_scene_report.json').open('w') as report:
    subprocess.run(arguments, stdout=report, check=True)
PY
```

Ignored `native_scene_validation.json` and `retail_math_production_validation.json` record the comparisons, source hash and reference settings. The local driver `build/runtime/validate_scene.py` reproduces the detailed comparisons and damaged-input checks. Its math harness includes the production C++ file and is built both with the flags above and with `-O3`, without a global floating-point override. Its reference writes only scratch pose, angle, output and stack memory: pose at 0x00500000, angles at 0x00501000, output at 0x00502000, SP=0x0070FF00 and return sentinel 0x00503FFC. It calls 0x001DB88C with pose/angles, 0x00334F90 with pose/output, then 0x002DDC68 with output/pose scale at +0x20. Direct trigonometry inputs and results use S0 at 0x00287908/0x00287AD0. The original executable hash is checked before execution.

Large-argument reduction, other floating-point modes, real-hardware parity, stage-switch collision activation, dynamic actors, collision-attribute meaning, rendering, gameplay and differential replay remain open. The bit agreement applies to the bounded routines and tested inputs. No M3 claim follows.
