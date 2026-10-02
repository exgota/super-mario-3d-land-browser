# Root 00324DD4: Telescope exit ABI prerequisite

Base/source snapshot: `e2336037d4f709acc082b3322daedadf9500564a`.
Branch: `dot/root-324dd4`; final note commit is recorded in the handoff manifest.
Target: `[0x00324DD4, 0x00324FF4)`, 544 bytes; instruction/pool boundary `0x00324FC8`.
Status: U, ABI-blocked before implementation. Zero source forms, build attempts, checker attempts, or exact-byte credit.

## Module and object evidence

The routine belongs with the Game MapObj Telescope family. Its local sound name is `SeSyTelescopeGoOut`; it restores the controlled actor's position/orientation, ends telescope-related state, and selects the idle nerve. The original exact method name is not established.

Constructor `00324FF4` calls the existing `al::MapObjActor` constructor `00280428`, installs primary dispatch `003D569C`, and preserves its established 0x60-byte LiveActor base. It installs effect/audio/stage interface tables at object offsets +4/+8/+0xC and initializes owned fields through +0x9C. This supports using the existing `al::MapObjActor` declaration, rather than inventing an overlapping base.

Dispatch row `[003D569C,003D5734)` contains initializer `00324AAC` and message handler `00323FC8`. The handler calls this root at `003240EC` with its unchanged actor pointer after testing the incoming message. Initialization stores the result of `002D182C` at +0x70. The root passes that field to `0026AA60` and then calls `00273800`.

## Blocking contracts

The existing C-linkage `fn_0026AA60` has incompatible declarations across source already present at this base:

- `Factory/group_0014240C.cpp`: `void(void*)`; owning root `0014240C` is O
- `Factory/group_00142424.cpp`: `void(uint32_t)`; owning root `00142424` is O
- `Factory/fn_002737E4.cpp`: `int(int)`; owning root `002737E4` is O
- `MapObj/TrickHintPanel.cpp`: `int(u32)`

`fn_002737E4` also declares `fn_00273800` as `int(int)` and chains the former's return into it. Binary `00273800` obtains a scene object internally and sets a byte; it does not read its incoming argument. That machine behavior cannot resolve the existing incompatible C++ declarations or justify a new false return/argument contract.

There is an additional interface conflict: `Enemy/Bubble.cpp` declares `fn_00279B64(al::IUseStageSwitch*)`, while accepted `Factory/fn_001A0350.cpp` declares it against its anonymous-namespace `const IUseNerve*`. This root passes its +0xC stage-switch interface, consistent with the existing actor header and the callee's first virtual call.

No single compatible established declaration covers these imports. Repair requires coordinated owner reconciliation of accepted Factory declarations, which is excluded from this worker's scope. No casts, alternate aliases, false imports, private-type duplicates, or shared-header changes were attempted.

## Data preflight

- Direct unit-Z data `004305E0` already has its 12-byte `sead::Vector3<float>::ez` row
- SafeString dispatch address `003D9C3C` lies inside existing row `[003D9C34,003D9C50)`
- Final nerve `003F2C3C` already has a four-byte row; initializer `00381AF4` installs its dispatch
- That installed dispatch starts at `003BF6D0`, already covered by `[003BF6D0,003BF6E0)`
- All direct branch targets in the root have function rows; local strings/scalars remain inside its mapped interval

No missing-row blocker was found. No data owners, extents, or map partitions were invented.

## Verification and handoff

Read-only preflight used the unchanged map, existing source, and owner's EU binary. Verified binary SHA-256: `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.

`python make.py eu`: not run; no source was introduced.
`python tools/check.py <symbol> --object <object>`: not run; no project-built root object exists.
No checker output, generated object, or build provenance is claimed. No replay, full preservation gate, or repeated audit ran.

Map remained byte-for-byte unchanged, SHA-256 `8e490e86394ddd3b867c5a217297f8c441c2ca5eb06bc0d64ec4551ddaaaeabc`; no scratch symbols were required. Checker, compiler configuration, Factory, game sources, headers, ranks, and ledger were not changed. The note-only patch and manifest are frozen outside the repository for coordinator publication.
