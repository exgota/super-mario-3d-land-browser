# Local runtime asset foundation

Verified on 2026-10-01. `tools/romfs.py` uses the Python standard library and stays separate from decompiled Game/al code. It reads a repository-local, ignored copy of the owner's decrypted EU dump. It validates NCSD partition ranges, NCCH flags, the RomFS directory tree, metadata links and the complete IVFC SHA-256 chain before indexing, extracting or inspecting files.

The original and working copy both retain SHA-256 `c83f9175208b3d60898d3a6d60451bee3108521e409cace362f63b46dda8c976`. The application partition has title ID `0004000000053F00`, product code `CTR-P-AREP`, 34 directories including the root, and 1,771 files totaling 298,216,940 bytes. All 73,421 IVFC blocks passed: 5 at level 1, 570 at level 2 and 72,846 at level 3. A complete verification and image hash took 0.405 seconds on this machine.

World 1-1 is identified from the first normal course in the first world's course table, cross-checked against the corresponding world-map preload table. Its scenario is 1. The stage map contains 189 preload records: 46 archive paths and 143 sound-item identifiers. All 46 paths exist and total 2,630,195 bytes. The stage has six start records and one goal-pole record. These are data observations; loading or playing the level is unverified.

The selected stage, dependencies and discovery tables form 49 files totaling 2,659,548 bytes. All written-file SHA-256 values were rechecked. Repeat extraction preserved identical files. Nine rejection checks covered a changed asset byte, a cyclic directory, conflicting existing data, an out-of-range archive member, output outside ignored storage, direct use of the original dump, an invalid compression reference, a cyclic BYAML graph and a report replacing its input. All runtime files are ignored. Detailed indexes, decoded tables, selectors and validation results stay under `data/runtime/romfs/`; assets stay under `data/runtime/extracted/`.

Run from the repository root:

```sh
. ./development_environment.sh
python tools/romfs.py data/runtime/dump/eu.3ds index
python tools/romfs.py data/runtime/dump/eu.3ds verify
python tools/romfs.py --help
```

The reader supports decrypted NCSD images, this RomFS IVFC variant, Yaz0, flat named NARC archives and v1/v2 BYAML. This dump's Japanese BYAML strings require explicit `--string-encoding cp932`. Three layout archives use nested NARC directories and are extracted correctly as opaque files, but inspection rejects that unsupported directory variant. Sound-item resolution, complete runtime dependency closure, graphics, collision interpretation, gameplay and differential replay remain open. No milestone M3 claim follows from this work.

Format references: [NCSD](https://www.3dbrew.org/wiki/NCSD), [NCCH](https://www.3dbrew.org/wiki/NCCH), [RomFS](https://www.3dbrew.org/wiki/RomFS), [Yaz0](https://wiki.cloudmodding.com/oot/Yaz_%28File_Compression%29), [NARC](https://loveemu.hatenablog.com/entry/20091002/nds_formats), [BYAML](https://nintendo-formats.com/libs/common/byaml.html). The implementation uses those public descriptions and the owner's binary. It uses no SDK source or removed library history.
