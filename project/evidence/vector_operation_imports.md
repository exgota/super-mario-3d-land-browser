# Shared vector operation imports

Owner: driver. Branch: `cleanup/vector-operation-imports`. Frozen integrator main: `3985a149b6cefcae0a30ef4fbac24d91b59b75e8`. Ownership commit: `555d51687008f529549fa731dae41aac33badc5c`. Final source: `b39d611af625285cbd18cccd30b9b9a2683f1819`. This six-file family introduces two declaration-only headers and reconciles all active callers of two existing vector operation symbols. It preserves five accepted roots, 1,476 complete bytes, and proposes zero new function credit.

## Original contracts and representation

The original EU code hash is `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`. The driver independently inspected both complete 28-byte bodies. At `0027CB64`, subtraction loads three float components from each input before storing left minus right into the three output components. At `0027CCA0`, scaled-add loads both input vectors before writing the addend plus the scaled vector. Its scalar arrives in the floating-point argument register. Both permit output/input aliasing; no restrict contract is introduced.

The accepted map identities encode `sead::Vector3CalcCtr<float>` static operations with `nn::math::VEC3&` output and const VEC3 references as inputs. The scaled-add scalar precedes those inputs. Existing void returns are retained: callers discard the unchanged integer return register, which does not independently prove the original source return spelling. No operation body is reconstructed or emitted, and both helper rows remain unaccepted U.

The new `nn/math/math_Vector3.h` owns one three-float VEC3 storage declaration. `math/seadVectorCalcCtr.h` declares only the float-specialized subtraction and scaled-add methods. Each of the four existing translation-unit-private vector wrappers now contains one real first-member VEC3 object. Calls use that member directly. This is a reconstruction choice that supplies typed subobjects without unrelated-reference casts or an unproved original inheritance claim.

The private wrapper names and every unrelated imported helper signature remain unchanged. The established `seadVector.h` is unchanged. Actual public sead::Vector3f objects remain a separate nominal type and require their own reviewed representation boundary in future callers. This family does not certify unrelated private actor views, normalize imports or a complete vector API.

## Canonical preservation

The normal `python make.py eu` build succeeded, linked and exported with unchanged configured Game ARMCC 4.1/791 settings. Compile-time checks establish VEC3 component offsets 0/4/8, size12, each wrapper's first-member offset0 and size12, and the enclosing Body/Actor offsets used by these callers. The normal compiler dependency records name exactly the four owned source files for the new headers.

All five canonical project-object checks pass, including complete literal-pool extents. Checker wall time: 1.932 seconds, sequentially. This is verification time, not accepted-byte throughput.

| Root | Start | Exclusive end | Complete bytes |
| --- | --- | --- | ---: |
| `fn_00171BE0` | `0x00171BE0` | `0x00171CDC` | 252 |
| `fn_001BAD90` | `0x001BAD90` | `0x001BAE88` | 248 |
| `fn_00355820` | `0x00355820` | `0x00355918` | 248 |
| `fn_0011D7A8` | `0x0011D7A8` | `0x0011D914` | 364 |
| `fn_00307F90` | `0x00307F90` | `0x003080FC` | 364 |

The four normal objects emit exactly these five function definitions. There are no extra inline wrapper bodies, vector operation bodies, implicit helper functions or data tables. The expected subtraction import appears in three objects and scaled-add in the fourth, with the original mangled identities unchanged. The canonical checker verifies each normal committed-source object. A global duplicate-definition audit found zero duplicates.

Search for the two obsolete literal-mangled declaration names across all tracked Game/ and lib/ source and headers returns zero hits. Named C++ declarations now generate those same external symbol identities. No generic implementation, alias shim, source address literal, map change, rank change, ledger edit or compiler/configuration change is introduced. The working map is restored byte-for-byte to HEAD after checks.

## Final source hashes

| File | SHA-256 |
| --- | --- |
| `lib/CtrSDK/include/nn/math/math_Vector3.h` | `02f79dd26e8182a78a0310c2e14cb4b9f219319bdaf8e25c265a8f8c9b0188fc` |
| `lib/sead/include/math/seadVectorCalcCtr.h` | `2f5bb1840d77fea95f6748fb596899ae0da3e846fb263b515c0a2bc05406bf77` |
| `Game/backup/src/Factory/fn_00171BE0.cpp` | `862d81e072179d2823c7eaba8b59d7a18640f910d96e95a868d2cddedeb54a90` |
| `Game/backup/src/Factory/fn_001BAD90.cpp` | `ed4e9d9856c718d2ffab6697be50f9d10a6149fd771ece5a6891ce01d1619720` |
| `Game/backup/src/Factory/fn_00355820.cpp` | `89fa5d9adf3d09bf166de92b6c963b95a4981992b2fa6b78a9d6821189d79b03` |
| `Game/backup/src/Factory/group_0011D7A8.cpp` | `12092404be39269503075ca8232e692be47e993291760ed248ba13a2ae5124a6` |

An independent read-only review of the final source found no source-level blocker. It confirms all five calls use real VEC3 subobjects, argument order is preserved, the two old declarations have no remaining Game/lib occurrences, and unrelated helper contracts stay unchanged. The parent separately completed the normal build, emitted-definition/dependency audit and five canonical checks reported above.

## Integration status

Local checks establish this bounded family only. The integrator must still run the full cleanup preservation gate before adoption. To preserve the owner's dot-first priority, keep this finished cleanup unsubmitted while the ready ActorTimer/WoodBox/WarpDoor dot source family is pending. Static review of the current scheduler shows that any queued cleanup/solo takes precedence over ordinary riders despite the dot-first sort. No production setting or restart is changed here.
