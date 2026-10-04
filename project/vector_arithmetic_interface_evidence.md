# Vector arithmetic interface repair

## Scope and evidence

Frozen base: `cd0c022599a9f0eca95dd64af1b66db1e5cc28d8`. Checked source commit: `39de2af57f2dbe887b015586cd9f53fce0cec118`. The existing Pro answer is applied as one cluster across 15 accepted factory owners and the shared calculator header. No helper body, rank, map boundary, compiler option, or game data changes.

The shared `sead::Vector3CalcCtr<float>` specialization declares `void add(output, left, right)` and `void multScalar(output, vector, scalar)` using the existing `nn::math::VEC3`. Callers use actual VEC3 subobjects. Existing private record identities, construction and copy order remain because changing those can alter ARMCC code generation. Size and offset assertions preserve the required storage. The curve owner also replaces its local subtraction import through the existing shared subtraction declaration.

The original pinned reference independently declares these void signatures: [RE-Pepper sead calculator](https://github.com/RE-Pepper/sead_ctr/blob/c0f95fb9c697ffc5f82fcf0b4046459a82cb5fb6/include/math/seadVectorCalcCtr.h). Source provenance follows the owner decision of 2026-10-03. Reference agreement does not grant acceptance.

## Verification

Both baseline and candidate pass the normal repository build after sourcing development_environment.sh. Provenance records identify 17 affected object files: the 15 changed owners and two unchanged header consumers. ELF definitions resolve to 42 previously accepted canonical functions. tools/check.py --object preserves all 42 complete source-generated function intervals in both builds. Objects are unmodified normal ARMCC outputs of committed source. Scratch map ranks were restored after each verification.

The new shared-type guard reports zero violations. Search of the 15 changed owner files finds zero references to the two removed mangled add and multScalar imports. Other private declarations belong to later clusters.

Integration must run alone with preserve_exact and operator approval. Any accepted-root regression rejects the entire cluster. This repair claims zero new matching bytes. Six downstream roots are potential beneficiaries; their complete build and matching status remain unverified. Local preservation is not a main acceptance verdict.

## Checked source hashes

| Path | SHA-256 |
|---|---|
| `Game/backup/src/Factory/fn_0016D830.cpp` | `99001aac5f12e0ea56f41a02f7500ee6aedd4aef53edc049cdaffc004f030758` |
| `Game/backup/src/Factory/fn_00171BE0.cpp` | `1b5eec5a115771d87bcc3666d2375e1c31bf1d3f95422379d88e315a43fad037` |
| `Game/backup/src/Factory/fn_00173698.cpp` | `482c225abc1148dfa3d049d2c7e9e55cc33dd1175e3a8fae4db94e98126a48da` |
| `Game/backup/src/Factory/fn_001BAD90.cpp` | `8c638759c46891f03272d55eb0bfa0c3930e423dbd433d406efd53c79af88e8c` |
| `Game/backup/src/Factory/fn_001BD204.cpp` | `387062a75e20082eb52fe99dd740df9e12e16506e14db6bfe50b2079f1bcdb06` |
| `Game/backup/src/Factory/fn_00213854.cpp` | `4542ef197cb470df136775b01d95c730fac3fe2c2f553d9ac80610d08c187efd` |
| `Game/backup/src/Factory/fn_00213B74.cpp` | `7d12c27b8f43060577b9988b1a5b6ee28bb7b8929c3f43e803d33a80e68c9368` |
| `Game/backup/src/Factory/fn_0030E468.cpp` | `a5b015a829429bbcee2c6d87be0c0ef85bc7b00971da8855cdd31386be210b76` |
| `Game/backup/src/Factory/fn_0032787C.cpp` | `d7a95767ecade4f2ce08ea05cee3ac8387453db6993c8606c50933ab5ab62272` |
| `Game/backup/src/Factory/fn_003449B4.cpp` | `95ccfc7c9d915d3ba7b84772ecb1b42f761c9aca261efa07c7c48f1e597b89f7` |
| `Game/backup/src/Factory/fn_0034D9AC.cpp` | `2a4bae509b501ad9d83bb4c82479c64bec2b8a9ee580820d50ba836db7d3fade` |
| `Game/backup/src/Factory/fn_00355820.cpp` | `e364ba2f1c6d08fc66fd5be2ef13b3c37e52e9e410ae128e77db87f9b2d5b849` |
| `Game/backup/src/Factory/fn_003600F8.cpp` | `8b8dc8f0eb29c605c750d9b123a876248c9abc49f0ca9bf76568a6f435234a3e` |
| `Game/backup/src/Factory/fn_00360DC0.cpp` | `4d70d7838365eaa0092941346232ec84e2db3a77abd2c4628e29c921856e623f` |
| `Game/backup/src/Factory/fn_0036844C.cpp` | `6598ea2824ee87781cd04daa3ef4d56b30d7d1c5f95a935ff3143fb340038afd` |
| `lib/sead/include/math/seadVectorCalcCtr.h` | `78850d7b038cf28ad4bdae92901bcee844297cd99442819b10e446c81bf9fcba` |

## Canonical preservation closure

- `fn_0011D7A8`
- `fn_0016D830`
- `fn_00171BE0`
- `fn_00173698`
- `fn_001BAD90`
- `fn_001BD204`
- `fn_00213854`
- `fn_00213B74`
- `fn_002438D4`
- `fn_00252B00`
- `fn_0025C040`
- `fn_0026CC14`
- `fn_0026EFD8`
- `fn_00277224`
- `fn_002775AC`
- `fn_00279ABC`
- `fn_0027DFE8`
- `fn_0028058C`
- `fn_00289C1C`
- `fn_002A7828`
- `fn_002A7B94`
- `fn_002CD5DC`
- `fn_002D67C0`
- `fn_002DAB20`
- `fn_002DB2A8`
- `fn_002DDA14`
- `fn_002DDAA4`
- `fn_002DE418`
- `fn_002DF460`
- `fn_002DF9F4`
- `fn_002E00EC`
- `fn_002E2FC8`
- `fn_00307F90`
- `fn_0030E468`
- `fn_0032787C`
- `fn_003449B4`
- `fn_0034D9AC`
- `fn_00355820`
- `fn_003600F8`
- `fn_00360DC0`
- `fn_0036844C`
- `nngxlowWriteHWRegs`
