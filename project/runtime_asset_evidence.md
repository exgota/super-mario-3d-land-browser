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

## Fresh root inline input construction, 2026-10-02

Root applies the lane's complete AssetReader patch unchanged and validates the committed df034a8e source in an absent directory. Only the frozen eight support files and exact full source are copied, with no reports, outputs, fixtures or executables. Fresh original producer/packing replay, debug and optimized native builds, and every previous native regression pass. All863sealed paths verify before and after execution; original instructions stay unchanged/read-execute, and owner model input stays read-only.

The137owner meshes provide561source assignments,28inline uploads,94source words,112encoded words and168packet words. Both native configurations pass1100conversion/packing fixtures and six mapping controls. Retail overflow conversion uses unsigned007F0000 and discards sign;512negative-overflow fixtures explicitly differ from the pinned public libctru routine. This is measured original behavior. Unsupported descending mappings remain unavailable. All1083missing fixed values remain unavailable, with no default assignment.

Prior uniform1289fixtures/15468words plus137owner packets, context645/face393/root1276/palette481/math3422bits/selection39members and49asset/archive outputs remain equal. Evidence: build/runtime/native_inline_attribute_root_validation/root_summary.json and its fresh reports. No active shader, scalar sampling, final vertex, GPU rendering, level or gameplay equivalence is claimed.

## Supplied fixed upload history, fresh root validation 2026-10-02

The unchanged122-line patch04d043b6 produces committed complete source4260601f. Root verifies all2500authoritative hashes before and after replay, then copies exactly ten frozen support files plus the committed source into an absent validation directory. Fixtures, reports and executables are generated afresh. Original instruction/source guards remain unchanged.

Debug with address/undefined sanitizers and O3 each pass10025ordered history cases,2818available values,81017unavailable values and11272encoded words, plus17status controls and38malformed rejections. All137owner meshes are read afresh. Without supplied history,1083missing owner values and provenance remain null. A separately supplied original setup prefix resolves controlled inputs only. Corrupt or partial histories provide no known values transactionally. Prior inline/uniform/context/face/root/palette/math/selection and49asset/scene outputs remain identical.

Evidence: build/runtime/native_fixed_attribute_history_root_validation/root_summary.json and original/native/preservation reports. Actual scene startup/reset submission, GPU sampled float24 values, active shader execution, final vertices, rendering and gameplay remain unverified. No level has run.
