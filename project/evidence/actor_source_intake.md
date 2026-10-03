# ActorTimer, WoodBox and WarpDoor source intake

Frozen integrator base: `b275e09b1a9d41b48b1c57c2ec9854c61a04254b`. Final committed source: `b683e4e36de330614f1ba1e44d1fae05adac2eb7`. Separate reconstruction-name evidence commit: `e29f0f3e5e49eee1189c068bfa4f752ed8dac459`. All seven imported source/header files are byte-identical to the fetched supplier tips below. The report-named earlier source commit objects were not present in the fetched repository; the pushed tips and their actual blobs are the reproducible provenance.

## Canonical results

| Function | Original start | Complete bytes | Driver result |
| --- | --- | ---: | --- |
| `_ZN7WoodBox10receiveMsgEjPN2al9HitSensorES2_` | `00314430` | 560 | Complete interval exact |
| `_ZN8WarpDoor4initERKN2al13ActorInitInfoE` | `003195F0` | 556 | Complete interval exact |
| `_ZN2al10ActorTimer6updateEv` | `001BFE60` | 556 | Complete interval exact |
| `_ZN8WarpDoorC1ERKN4sead14SafeStringBaseIcEE` | `00319ADC` | 136 | Complete interval exact |

The normal project build succeeded using the existing module compiler settings. The four sequential `tools/check.py --object` invocations took 2.778049 seconds and passed 1,808 complete bytes. The supplier reports claimed only the three larger roots, 1,672 bytes. The additional 136-byte WarpDoor constructor was found in the ordinary object and independently passed the same canonical checker. Its complete interval includes the pool, which exceeds the 124-byte function symbol size. No new implementation form or tuning was needed.

A second normal build linked with all four proposed rows active. All five inspected object hashes remained identical to the ones used for the four checks. The unchanged shared LiveActorFlag constructor also passed its complete 48-byte canonical interval. The final global duplicate-definition audit reported zero conflicts, accounting for normal COMDAT ownership. Scratch ranks and the entire map were restored byte-for-byte to the committed named map. No rank or ledger change is submitted.

These are driver checks, not integrator acceptance. The integrator must independently build the submitted source and preserve every accepted root before awarding any of the 1,808 proposed bytes. This intake does not assert a fresh whole-map driver regression pass.

## Emitted definitions and interface limits

The ordinary WoodBox and WarpDoor objects emit 152-byte class tables, and ActorTimer emits a 12-byte class table. They are not given independent exact credit. In the final compact linked image, none of those genuine class-table sections remains. The only retained listed table symbol is the existing weak eight-byte WarpDoor ABI-prefix scaffold. WoodBox is a storage/receiveMsg view; it does not reconstruct all original overrides. WarpDoor receiveMsg and control are declarations only.

WarpDoor emits both constructor aliases, C1 and C2, for the same constructor body. Only the existing complete C1 row receives a claim. The source-defined `readActorTimerDeadFlag` and the two used unchanged flag readers inline and leave no separately retained sections in the final compact image. They add no independent function count. Canonical object closure checks determine their contribution to the root. Shared weak base/SafeString support is ordinary compiler output and receives no separate claim.

The new declarations reuse accepted public LiveActor, ActorInitInfo, sensor and audio interfaces. The driver and independent helper found no newly introduced direct import-signature collision. Two older private Factory variants of fn_0026a9fc and fn_001C96B8 already coexist with the public contracts; this intake leaves those unrelated files unchanged. The imported WoodBox nerve is an incomplete external interface view, not a definition of an abstract object. Original startup/table evidence grounds its zero-offset nerve interface. No source or game data is copied from an unapproved external input.

The independently verified identities, storage extents and limitations are in `actor_source_intake_names.md`. This work provides byte-exact function evidence only, with no new rendering, browser, runtime-equivalence or gameplay claim.

## Frozen source inventory

| Path | Supplier tip | SHA-256 |
| --- | --- | --- |
| `lib/al/include/LiveActor/alActorTimer.h` | `f4d665a26118591ce070f35a18259c0270aa4299` | `f409dc1c792495be7fdc885dd2e7e4ad92e61ea429651854c18a174ea4f0d234` |
| `lib/al/src/LiveActor/alActorTimer.cpp` | `f4d665a26118591ce070f35a18259c0270aa4299` | `b57c87d4692539f80c334e06fb5720d952d79f378d343bcea88dc4ede85a7bde` |
| `lib/al/src/LiveActor/alActorTimerFlag.cpp` | `f4d665a26118591ce070f35a18259c0270aa4299` | `5d95085aaaa0221d6e61736956d9c76ee3d32f4b8eae5d180e035120aa31beb4` |
| `Game/backup/include/MapObj/WoodBox.h` | `e37becbc14d13e0d59d18cd4545d9e5156ac117d` | `de2c9a004fce949f32b95e7ce68934f0368b7d6b95f316e97bd0c2b9cffb2b08` |
| `Game/backup/src/MapObj/WoodBox.cpp` | `e37becbc14d13e0d59d18cd4545d9e5156ac117d` | `89bef91e6d3d9f9c3ab7ffb826035ef5165c3407132055a25c178c427047ff45` |
| `Game/backup/include/MapObj/WarpDoor.h` | `2066af391502783383f857eec21f2c40079dd16f` | `4de193ad4808af32003897d9907be6a698bfc3b16fdf06b2199f651670dc45e0` |
| `Game/backup/src/MapObj/WarpDoor.cpp` | `2066af391502783383f857eec21f2c40079dd16f` | `18ed7dbc68b57c440a1af4a015406e9f1ce865482eaa6fcac6d37ede6d55d12e` |
