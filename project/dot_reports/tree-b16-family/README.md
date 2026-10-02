# TreeNode proposal: frozen-b16 family revalidation

This is a carried-ready proposal for main's **actor_group** lane, which owns the
TreeNode family in b16 STATE. It adds no main acceptance credit. The submitted C++
is byte-identical to source commit `37844b080b6f7469fb06558c4d272883bc3b6ee6`
from the earlier `dot/tree-push-front` proposal published at
`ecf593ca65702f1ff0eceb3247c3ff89726b820b`. No new source form was investigated.

Frozen base: `b16e2a0cc32e0cdacac5ba94feabe67432433391`.
Branch: `dot/tree-b16-family`.
Committed source checkpoint: `db3a3e78abf816c0c39b775f935524b5621284d6`.
Base tree: `7afbde0712147755a41f29294e49e4612f790ad0`.
The base was reconstructed with all 563 tracked blobs verified by the coordinator.
Main may have advanced since this snapshot; these results apply only to this
base and final source. Its owner must revalidate any later integrated checkpoint.

## Result and scope

The project clean build compiled 44 Game, 128 al and 1 SDK translation units,
linked and exported successfully. The unchanged project checker then preserved
**all 667 prior accepted roots / 37,224 complete bytes at all 686 actual emitted
canonical definitions**. The inventory checks every defined STT_FUNC occurrence
of those accepted roots, including weak copies; it does not prefer strong copies
and silently omit the others. Only after all 686 checks passed did the driver
check the existing 64-byte pushFrontChild candidate. It passed with zero differing
bytes. All four physical TreeNode bodies pass the complete-interval byte check.

The family is **252 complete bytes: 188 previously accepted plus 64 proposed**.
Only 64 bytes would be new credit if main independently accepts this proposal.
The 667-root map was restored byte-for-byte after all checker runs. The ordinary
compact build remains a scaffold; these results make no whole-image or runtime
claim. No game data is included in this submission.

| Canonical symbol | Original interval | Complete bytes | Different bytes |
|---|---|---:|---:|
| `_ZN4sead8TreeNode14pushFrontChildEPS0_` | `0x002F2AD4..0x002F2B14` | 64 | 0 |
| `_ZN4sead8TreeNode13detachSubTreeEv` | `0x00222978..0x002229DC` | 100 | 0 |
| `_ZN4sead8TreeNode27clearChildLinksRecursively_Ev` | `0x002F2B14..0x002F2B54` | 64 | 0 |
| `_ZN4sead8TreeNodeC2Ev` | `0x0028CC00..0x0028CC18` | 24 | 0 |

The five actual ELF function symbol entries cover four bodies. Constructor C1
has size 24 in `i._ZN4sead8TreeNodeC1Ev`; C2 is its zero-size address alias in that
same section. The already accepted C2 map row explicitly names that C1 section,
so its canonical check covers the entire 24-byte constructor. C1 is not a new
row or a second body/credit. Every emitted family entry is listed in
[inventory.json](inventory.json).

## Source and helper closure

Only `lib/al/src/Util/seadTreeNode.cpp` differs from b16. The established proposal
adds the ordinary push-front body and marks the existing detach definition
`__attribute__((noinline))`, preserving the retail out-of-line detach call. The
accepted detach body itself is unchanged. The TU retains its private four-link
class and uses no shared TreeNode header. No shared header, table, ABI, boundary,
name or tool changes are proposed. All Pro reservations remain untouched.

The unchanged source has only the four physical bodies above. There is no new
unmapped helper that must be eliminated, no added inline-closure object and no
invented address alias. Its byte checks resolve these original named imports:

- `_ZN4sead8TreeNode14pushFrontChildEPS0_`: `_ZN4sead8TreeNode13detachSubTreeEv` at `0x00222978`
- `_ZN4sead8TreeNode13detachSubTreeEv`: no external imports
- `_ZN4sead8TreeNode27clearChildLinksRecursively_Ev`: no external imports
- `_ZN4sead8TreeNodeC2Ev`: no external imports

The provenance-checked Tree compilation has four repository inputs: this source,
`Game/project_globals.h`, `lib/CtrSDK/include/nn/types.h`, and `data/config.json`.
The latter three are unchanged from b16. It uses the configured ARMCC **4.1/791**,
without per-file or global flag changes. The exact command and input hashes are
in [tree-object-provenance.json](tree-object-provenance.json).

[final-source.patch](final-source.patch) is the complete source-only diff from
b16: 19 inserted lines and 1 removed line. `git apply --check` succeeded against
the clean b16 worktree. Relative to the previously published Tree source, the
source patch is explicitly empty, and the SHA below is unchanged. The branch's
remaining changes are these review notes and machine-readable evidence.

## Timing and status

Assignment/revalidation window began 2026-10-01T23:20:54Z. This is revalidation
and packaging of carried-ready inventory, not a new matching search.

- Clean build: `2026-10-01T23:22:11.749905+00:00` to `2026-10-01T23:22:48.103271+00:00`, **36.353 seconds**
- Checker driver after inventory, covering prior checks, candidate and four detailed byte checks: `2026-10-01T23:23:43.037016+00:00` to `2026-10-01T23:32:05.318844+00:00`, **502.242 seconds**
- Prior gate finished at `2026-10-01T23:32:04.052461+00:00`, before the candidate checker
- Measured build plus checker driver time: **538.596 seconds**, excluding inventory, preparation/packaging and the gap between commands

The new accepted bytes on main from this revalidation remain **0**. No accepted
throughput is claimed; only main's acceptance gate can establish it.

## Main intake

[acceptance-manifest.json](acceptance-manifest.json) names prior checkpoint b16,
main owner `actor_group`, candidate symbol and source. The row already has its
established name and is U in the frozen base. Main owns enrolling it as M in a
separate committed input before invoking its unmodified `tools/acceptance_batch.py`.
This dot check uses plain `tools/check.py --object`, which permits the original U
row, and restores the entire map afterward. The manifest is an intake input,
not a record that the main acceptance tool was run here.

Main must apply the final source patch unchanged or take the source commit,
commit its intended inputs and candidate enrollment, and run its canonical build
and prior-definition gate before candidate acceptance. If the current accepted
set differs from b16, use a newly agreed frozen checkpoint and renewed preservation
evidence. If it conflicts with the owning lane's current source family, return
it to that owner; this report does not authorize root to assemble fragments.

## SHA-256 identities

- `lib/al/src/Util/seadTreeNode.cpp`: `7baffec4b181a3f1ad57356bc67a46ca099fe864e861dfd62aa01a83ce9983b4`
- `build/eu/obj/lib/al/src/Util/seadTreeNode.o`: `383b2aa4563612409c401b1ae2c1bf5f10052337ae2c7b1b80cf2f5acfb3efd0`
- `data/ver/eu/code.bin`: `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`
- `data/ver/eu/map.csv`: `e77614ca3a67b34978ecbeb88fcaf668b5b0a181c0af62baec2bf5f3925faee5`
- `data/config.json`: `5d41e617eb5b26374ed60b5383a0940ce0ce7d046344f7f815b7564144822c45`
- `make.py`: `1d5ba9628e78a4aaa23d6bbbea40131101a46ee41ca43e3df124eaa97d55241a`
- `tools/check.py`: `e177e0abe130e645e75c5f0dc24abb19dbe83310de55f1f099ec8fc385e07317`
- `tools/acceptance_batch.py`: `63014a5993b2281221d0ea9bd67de2ce8a39bc1a5f69e43fcc9e4d95a0f384df`
- `tools/low/checkExactBytes.py`: `aef81a3a21c49bb17b8a19f139d02607899257e04dbe461ece2992cc726fc2e2`
- `tools/low/buildProvenance.py`: `343d1a0954aba9a4805e71420be9a99d91185db4c8b329d1363d11efcee53529`
- `data/compilers/4.1/791/bin/armcc.exe`: `d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d`
- `final-source.patch`: `2a0eb98f96f61354637ac425ea22bdf4b79cb6c76446a88528d2be45b0bf7e28`
- `clean_build.log`: `c69434844148138461888ad8028b30b1dacb3b7c3c73090d5f6ad443e9d38ddb`

## Reproduction and detailed evidence

[checks.json](checks.json) records all 686 prior object checks, the candidate
check, four full byte comparisons, hashes and timing. [inventory.json](inventory.json)
records every checked accepted-symbol definition and every emitted family symbol.
The original target and all generated objects/ELFs stay in ignored local paths.
The clean build log remains local at `build/tree_b16_family/clean_build.log` and
is fingerprinted above. Its final stages report link and export success.

[recipe.md](recipe.md) contains the executed clean-build wrapper and complete
executed verification driver. Run it at this submitted source in a repository
with the approved ignored EU inputs and compiler/venv installed. The normal
build must create fresh local objects; no object or archive is shared here.
