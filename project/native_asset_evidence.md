# Native C++ asset reader

Verified on 2026-10-01 with Apple Clang 21.0.0 (`clang-2100.1.1.101`), targeting arm64 macOS. `runtime/AssetReader.cpp` is a standalone C++20 reader outside Game/al and the ARMCC build. It loads verified local assets, reads Yaz0/NARC/BYAML, creates typed placement and collision records, and catalogs CGFX resources. It preserves encoded text bytes and reports float32 bit patterns.

The independent Python comparison passed for all 49 selected files. Three nested layout archives remain explicitly unsupported for inspection. Four placement tables contain 328 records, including the level map's 298 placements, 26 rails, six starts and one goal. All 2,952 float fields match by their original 32-bit representation. All 5,646 integer or boolean fields match by value.

All 18 collision resources use the observed ordered, little-endian 0x38-byte KCL header. The complete octree traversal references all 3,853 prism records, with valid position and normal indices. The 3DS leaf offset points two bytes before the first index; the skipped value can be nonzero. The reader validates this variant rather than requiring the zero prefix described for Wii files. Triangle reconstruction has no degenerate denominator. Independent coordinates differ by at most 3.64e-12, and the maximum independently checked face-plane residual is 4.47e-13, using double arithmetic. Collision attributes remain raw identifiers.

All 40 CGFX containers have valid section bounds and self-relative catalog pointers. Their catalogs contain 39 model entries and 145 texture entries. Nine damaged-input cases reject without partial reports: invalid graphics pointers, invalid collision indices, an octree self-reference, a BYAML cycle, a nonfinite float, an invalid compression reference, invalid archive bounds and a truncated header. AddressSanitizer and UndefinedBehaviorSanitizer report no failures.

Reproduce the native read after the verified extraction described in `runtime_asset_evidence.md`:

```sh
. ./development_environment.sh
mkdir -p build/runtime
clang++ -std=c++20 -Wall -Wextra -Wpedantic -Werror -fsanitize=address,undefined -g runtime/AssetReader.cpp -o build/runtime/asset_reader
python - <<'PY'
import json
from pathlib import Path
import subprocess
manifest = json.loads(Path('data/runtime/romfs/world_one_extraction.json').read_text())
arguments = ['build/runtime/asset_reader']
arguments += [str(Path('data/runtime/extracted') / entry['path']) for entry in manifest['files']]
with Path('data/runtime/romfs/native_asset_report.json').open('w') as report:
    subprocess.run(arguments, stdout=report, check=True)
PY
```

Detailed comparisons, identities and geometry remain in ignored `data/runtime/romfs/native_asset_report.json` and `native_asset_validation.json`. The original dump, working copy and all 49 extracted files retain their recorded SHA-256 values. The dump/copy hash remains `c83f9175208b3d60898d3a6d60451bee3108521e409cace362f63b46dda8c976`.

This slice establishes format decoding. It does not recover collision response, attribute-table meaning, active-layer selection, actor/resource binding, transform composition, CGFX mesh/material/texture decoding, rendering, gameplay or replay. No M3 claim follows. BYAML v1 is validated on the owner data; the implemented v2 scalar rules have no owner-data sample in this validation.

Public format evidence: [BYAML](https://nintendo-formats.com/libs/common/byaml.html), [Yaz0](https://wiki.cloudmodding.com/oot/Yaz_%28File_Compression%29), [NARC](https://loveemu.hatenablog.com/entry/20091002/nds_formats), [KCL](https://mkwiiki.org/wiki/KCL), [CGFX](https://www.3dbrew.org/wiki/CGFX). Variant details come from the owner's files. No Nintendo SDK or removed library source was used.
