# Garigari identity enrollment for the composed acceptance family

Base: `75ac2e8fdb34f5c8d307dfeab78171b287bb1584`. This is an independent identity proposal for three existing function rows. The accompanying patch changes only their Symbol fields. Type `f`, rank `U`, starts, ends, pools, section names and every data row remain unchanged. No new object boundary or table name is proposed.

| Proposed symbol | Existing complete interval | Pool | Bytes |
| --- | --- | --- | ---: |
| `fn_00315598` | `00315598..003155CC` | `003155C4..003155CC` | 52 |
| `_ZN8Garigari7controlEv` | `003155CC..003156E0` | `003156B4..003156E0` | 276 |
| `_ZNK2al10FunctorV0MIP8GarigariMS1_FvvEEclEv` | `0039CC6C..0039CC94` | none | 40 |

## Class and control identity

The original factory record at `003B9DC0` pairs the literal `Garigari` at `003E00D8` with factory `00398844`. The original factory allocates `0x78` bytes and calls constructor `003156E0`. That constructor installs primary vptr `003D3E78` and initializes the retained fields at `60`, `64`, `68`, `6C`, `70` and `74`. The inherited `LiveActor::control` slot is offset `4C` from that address point. Its original word at `003D3EC4` is `003155CC`. The existing Garigari header declares this override and its six fields; no header change is needed. This establishes the method's identity independently of compiler equality.

The raw table prefix at `003D3E70` lies inside the current named `_ZTV8FireBall` map row. That row label is not used to establish Garigari ownership, and no table row is repaired or imported by this proposal. `garigari_retail_identity_records.json` retains the exact original words and current containing rows so the distinction is reviewable. Only the constructor's original class/factory coupling and observed slot establish the function identity.

The control body compares the existing signed integer fields `6C`, `70` and `74`, tests three state-object addresses, starts effect/audio at the first threshold, stops them at the second threshold, and increments `74`. It retains short-circuit order and repeated field loads. The literal pool contains the ordinary text `Cut` and `SeEmLvGarigariCutting`, both retained as ordinary source strings. Full control equality includes all 44 pool bytes.

The original effect calls use receiver `actor + 4`: entries `0027BEA0` and `001BFA04` receive the existing `IUseEffectKeeper` interface. The audio calls use `actor + 8`: entries `00267364` and `00273B1C` dispatch through the audio interface's existing getter slot `8`. They receive a two-word SafeString temporary; the final call also receives integer zero. These remain existing address-entry declarations. No public provider names are inferred or added. The five existing four-byte state rows `003F277C`, `003F2780`, `003F2784`, `003F2788` and `003F278C` remain address-named imports with no public nerve-name proposal.

## Callback entry and receiver ABI

The original eight-byte member-pointer record `003BDB34..003BDB3C` contains `00315598, 00000000`. Original initialization loads that pair, combines it with the actor pointer and the functor address point, then passes the temporary to the stage-switch listener. The callback's original complete body receives the actor in `R0`, calls the existing isNerve entry with state `003F277C`, and conditionally tail-calls setNerve with `003F2780`. This supports the receiver argument `Garigari*` and existing address-derived C entry `fn_00315598`.

The earlier carried package explicitly described `startMoving` as an inferred spelling. This submission removes that guess. It defines no public callback method and changes no Garigari declaration. Its final ordinary C body is paired exact for the full 52-byte row, including the eight-byte pool. The older review's statement that this callback has no pool is incorrect; its frozen interval JSON and current paired diagnostics retain the actual pool.

## Typed member-functor operator

Original initialization builds a functor from the actor and the direct member-pointer record. Operator `0039CC6C` loads the parent at `4`, the member-pointer words at `8` and `C`, applies the ARM C++ member-pointer adjustment/virtual test, and dispatches through the pointer. The adjacent clone independently allocates and copies a 16-byte object with the same field offsets. The existing `al::FunctorV0M<Garigari*, void (Garigari::*)()>` API defines exactly that non-owning callable interface and layout. The original address-point words at `003D5E1C` and `003D5E20` select this operator and clone respectively. The typed operator identity follows the factory/actor/member-pointer construction and interface dispatch, rather than matching its generic opcode shape alone.

Final source uses an ordinary explicit specialization of the existing operator body. It emits a native `i.` function section under both compilers, so existing Type `f` is retained. An earlier extraction using only explicit method instantiation emitted no operator definition; that attempt is preserved under `garigari/explicit_instantiation_attempt`. No global or semantic compiler flag was changed to force emission.

The proposed whole functor table `003D5E14..003D5E24` crosses existing anonymous boundaries. This remains a held observation. No whole-table ownership or boundary patch is submitted. The final source defines no clone or init body. ARMCC automatically emits the existing generic clone (64 bytes) and a 16-byte functor table alongside the specialized operator; those sections are unaccepted and are not included in this queue. The exact operator imports none of them. The earlier frozen copy-clone specialization remains one pool byte different from `0039CC2C`; init `003153FC` remains nonexact. The current automatic generic clone was not checked or claimed exact. Neither is included in the 368-byte ready family.

## Evidence and acceptance boundary

`original_interval_evidence.json` and `.txt` are immutable copies of the earlier original intervals and disassemblies. `garigari_retail_identity_records.json` adds the original class/factory/table words with unchanged map-row context. `garigari/diagnostics/4.1/{791,894}/evidence.json` records actual final commands, inputs, object hashes, full intervals, imports and original-address links. This proposal has no canonical rank or data credit. Root reviews and commits the three names separately, then builds committed source and applies its canonical checker.
