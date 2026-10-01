# HitSensorDirector constructor

## Result

`al::HitSensorDirector::HitSensorDirector()` (`_ZN2al17HitSensorDirectorC1Ev`) occupies `0x001D1F90`–`0x001D20FC`: 364 bytes (`0x16C`), including the literal pool beginning at `0x001D20BC`.

The parent lane ran the repository build under ARMCC 4.1 build 791 and then:

```sh
. ./development_environment.sh
python tools/check.py _ZN2al17HitSensorDirectorC1Ev --object build/eu/obj/lib/al/src/LiveActor/alHitSensorDirector.o
```

Reported checker result:

```text
U -> O: The complete source-generated function interval matches byte for byte.
```

The checked constructor source and headers were already committed and unchanged from refreshed main `9fba58fa1db1aaabad80d2b77d87139bd83b80e6`. This lane made no source or header edits, ran no concurrent build, and made no map, rank, tool, configuration, ledger, or STATE changes. The parent owns the build, exact check and rank update. The prior blocked entry was stale: `al::SensorHitGroup::SensorHitGroup(int, const char*)` is now independently mapped at `0x002496F8` and ranked O on main.

## Independent retail observations

The owner-provided executable has the approved SHA-256 `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`. Source inspection and Capstone disassembly established:

- The object is 28 bytes: one vtable pointer followed by six `SensorHitGroup*` fields at offsets `4`, `8`, `12`, `16`, `20`, and `24`.
- Each allocation requests 12 bytes through the already-mapped nothrow operator new at `0x002932B0`, then conditionally calls the now-mapped group constructor at `0x002496F8`.
- The six capacities and names, in order, are `16 / Player`, `128 / Ride`, `512 / Eye`, `2048 / Simple`, `1024 / MapObj`, and `1024 / Character`.
- The final call is the mapped `al::registerExecutorUser(al::IUseExecutor*, const char*)` at `0x001CC79C`.
- Its registration name at `0x001D20F0` is CP932 `83 5A 83 93 83 54 81 5B 00`, decoding to センサー. The source already contains the correct original CP932 bytes. A UTF-8 terminal displays replacement characters; that display is not evidence of source corruption. No conversion was needed or performed.
- The constructor stores `0x003D6920`, which is exactly the existing mapped `_ZTVN2al17HitSensorDirectorE` address `0x003D6918` plus the ordinary eight-byte ABI header. Unlike the separately blocked NerveExecutor/LayoutActor constructors, this data identity needs no map-boundary repair.

The canonical object's constructor section is exactly `0x16C` bytes. Independent inspection found zero differences across the 77 non-relocation words, including every string and padding byte. Its remaining 14 words are relocations: six allocations, six group constructors, one executor registration, and one vtable address. This inspection supported the diagnostic; only the parent lane's unchanged project checker established the match.

## Follow-on class evidence

The retail vtable has zero words at `0x003D6918` and `0x003D691C`, then function pointers:

- `0x003D6920 -> 0x001D1DA0`: the execute slot, currently an unnamed 496-byte (`0x1F0`) function. It clears/tests the six sensor groups, calls a pair-testing helper at `0x002497B0`, and performs character-group self-pair overlap checks using actor identity at sensor offset `0x28`, position at `0x08`, and radius at `0x14`.
- `0x003D6924 -> 0x00330450`: the draw slot, a single `bx lr` instruction.

These are independently grounded identities for later work, not additional matched functions. The current header still inherits the placeholder `IUseExecutor::execute()` and `draw()`. Accepting the constructor's reference to the independently mapped vtable does not establish that this C++ vtable or the class's execution behavior has been reconstructed. This report claims only the exact constructor interval above.

## Current canonical checker revalidation

On 2026-10-01, a clean worktree of main `a360142fbddd9ab875ab68314c55445ade05fd00` was reconstructed and its entire Git tree verified. The proposed source/header edits were applied and committed as local verification checkpoint `eace3c0`. The unchanged current project build (`python make.py eu`, ARMCC 4.1/791 for game code) compiled, linked, and exported successfully. The current canonical `tools/check.py --object` commands were then rerun on those freshly built objects. No checker/tool changes were made. The same independently evidenced local-only map identities were supplied; no existing function interval changed.

The unchanged constructor reports `U -> O: The complete source-generated function interval matches byte for byte.` All 364 bytes pass without extra identities specific to this function. This is revalidation of existing source and resolution of a stale blocker.
