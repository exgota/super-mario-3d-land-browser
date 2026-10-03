# Bug initialization and integer argument ABI

This individual-family record is retained for interface evidence. The current combined submission and final-source verification are recorded in [initializer_intake_batch.md](initializer_intake_batch.md). Its nineteen files are byte-identical to the reviewed family snapshots below.

Codex driver owns this four-file intake family from main `b7cc487b59aa2409b062a226938afcadb9da634d`. Bug.cpp, Bug.h and EnemyStateHipDropDown.h are byte-identical to `dot/root-2d4544` at `6d2d3e45cb2c47f49e6452b6029e86b8d14b461c`. The only additional game-source correction is the existing caller `Game/backup/src/Factory/fn_0018938C.cpp`.

## Independent interface evidence

Original helper `002794F8` uses the established Byaml integer reader, leaves the output unchanged when lookup fails or yields the integer sentinel -1, and returns zero or one. Original Bug initialization tests the return and converts the result from signed integer to float before updating a vector. This supports the existing `bool(int*, const al::ActorInitInfo&)` contract used by KoopaPillar and FallMapParts.

The existing Factory caller instead typed the output field as floating point and discarded the return through an incompatible declaration. Its independently linked constructor at `00189480` initializes receiver offset `0x64` with integer 300. The initializer at `0018938C` passes that exact field to the integer helper. The correction makes the field int, uses the canonical initialization-info type, and derives the required reference from the incoming pointer. Ignoring the helper result remains valid. No alias or pointer cast conceals the mismatch.

The other six address imports in Bug have no additional direct declaration conflict in the scoped current-main Game/lib search. This is a bounded audit. It does not establish all repository interfaces or original C++ pointer/reference spellings. The imported HipDropDown constructor declaration takes the three arguments consumed by its original body. No other source in the frozen main includes that header.

## Canonical verification

Committed source `6c59fa81185be4e141ea78b772b59f7a47ac7972` built with normal `python make.py eu` under the existing configured toolchain. Both complete intervals passed `tools/check.py --object` against their normal project-built objects:

| Function | Complete bytes | Result |
| --- | ---: | --- |
| Existing caller `fn_0018938C` | 244 | Exact, preserved |
| Bug initialization | 532 | Exact proposal |

Targeted checks took 1.049 seconds. The original map was restored after checking. No map name, boundary, rank, compiler flag, oracle, binary or ledger change is committed in this family. The separate `integrator/bug-row-names` evidence is a dependency before intake. The integrator must still verify the submitted final source and preserve all accepted roots before awarding the 532 proposed new bytes.

The three imported source hashes are unchanged from the supplier. Final source hashes:

| File | SHA-256 |
| --- | --- |
| `Game/backup/src/Factory/fn_0018938C.cpp` | `156e46a711de88272f5e43cb2665d0b0d4c298042ca8feac70472004ece64a58` |
| `Game/backup/src/Enemy/Bug.cpp` | `5d47ee35fefd82c9262e4c7cc525d105d369beb8a404077866d67e698c2fba34` |
| `Game/backup/include/Enemy/Bug.h` | `26e9ff9206a1d481ab56c636e317ab2ff44faf5cb402d657ea5174aeaec26d33` |
| `Game/backup/include/Enemy/EnemyStateHipDropDown.h` | `4ca853056122c4ed0d5103f778c5dfe756471ea6be8280fe9dc676c52daf251a` |

The obsolete helper declaration and private initialization-info forward declaration have zero remaining hits in the four-file intake family. Other unrelated declarations in the existing caller remain outside this correction. No runtime or gameplay result is claimed.
