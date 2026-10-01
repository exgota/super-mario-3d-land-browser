# Native CGFX vertex and index records

Verified on 2026-10-01 with Apple Clang 21.0.0, C++20, AddressSanitizer and UndefinedBehaviorSanitizer. `runtime/AssetReader.cpp` remains outside Game/al and the ARMCC matching build. Its CGFX catalog now includes resource-local vertex attributes and index streams. It does not apply a skeleton, shape or world transform, select materials, infer primitive topology or render a model.

The owner's selected files use CGFX revision 0x05000000. Their CMDL shape lists point to SOBJ records with flags 0x10000001. The documented count, self-relative pointer and stride fields lead to 0x40000002 interleaved vertex groups, 0x40000001 component declarations, face groups and index descriptors. Metadata stays inside DATA; vertex and index buffers stay inside the IMAG payload. The reader bounds those regions before traversal and limits decoded components by the source file size.

The public [CGFX description](https://www.3dbrew.org/wiki/CGFX) establishes the record layout. Its compact scalar-type table differs from this owner's variant, which stores OpenGL scalar enums. The [Khronos registry header](https://github.com/KhronosGroup/OpenGL-Registry/blob/main/api/GLES/gl.h) establishes those enum values. The reader handles float32, signed/unsigned byte and signed/unsigned short components. Each attribute retains its raw component words, multiplier bits and scaled float32 bits. Semantic IDs remain numeric, including unresolved semantic 2.

Independent Python reads compare every exposed field and component against the original extracted member bytes. All 39 model catalogs in the 49 selected files pass: 137 shapes, 50,076 vertices, 621,631 raw components and 621,631 scaled components. The raw components comprise 412,824 float32 fields and 208,807 integer fields. All 133,872 indices from 137 streams match and lie inside their vertex groups. The comparison also verifies 164 bone references and every preserved shape-position field by bits. Short scalar types have six additional controlled signed/unsigned component checks; the owner components use float32 and byte types.

The 16 models already resolved by scene binding contribute 52 shapes, 18,333 vertices and 52,734 indices. Their face groups include references to bones 0 and 1. The reader therefore retains resource-local data and leaves bone transforms unresolved. Primitive fields and skinning fields remain raw numbers. No triangle count follows from the index arrays.

Twenty-eight 0x80000000 vertex groups remain opaque beyond their flags and adjacent raw semantic field. Their constant payloads are not decoded or replaced with defaults. Material-to-mesh mapping, shape translation semantics, skeleton transforms, animation, primitive topology and GPU vertex processing remain open. The owner assets retain those unresolved records intact.

Thirteen damaged-input checks reject with exit 1, no partial report and no sanitizer failure. They cover oversized lists, metadata/image pointers in the wrong region, zero stride, component offsets outside stride, unsupported scalar types, nonfinite vertex components, duplicate semantics, nonnull empty lists, invalid shape signatures, partial index scalars and indices outside the vertex count. The prior scene checks, 49-file inspection baseline and all 3,422 debug/optimized retail-math comparisons still pass after rebuilding from this source. All 50 extracted hashes and both original/working-copy dump hashes remain unchanged.

Reproduce inspection after local extraction:

```sh
. ./development_environment.sh
clang++ -std=c++20 -Wall -Wextra -Wpedantic -Werror -fsanitize=address,undefined -g runtime/AssetReader.cpp -o build/runtime/asset_reader
python - <<'PY'
import json
from pathlib import Path
import subprocess
directory = Path('data/runtime/romfs')
manifest = json.loads((directory / 'world_one_extraction.json').read_text())
paths = [str(Path('data/runtime/extracted') / item['path']) for item in manifest['files']]
with (directory / 'native_mesh_report.json').open('w') as report:
    subprocess.run(['build/runtime/asset_reader', *paths], stdout=report, check=True)
PY
```

The ignored local driver `build/runtime/validate_mesh.py` performs the independent raw-byte/float32 comparison and damaged-input checks. `native_mesh_validation.json` records counts, rejection results, source hash and asset hashes under ignored storage. The existing `build/runtime/validate_scene.py` checks scene and math regression. Detailed resource names, component data and generated reports remain ignored. No rendering, gameplay, M3 or differential replay claim follows.
