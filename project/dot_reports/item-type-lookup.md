# Item-name discriminator

## Verified result

`fn_00277EC4`, unchanged EU interval `0x00277EC4–0x00278320`, is exact: **1,116 bytes**, including the complete literal/string tail beginning at `0x00278198`. This newly reconstructed function was U when selected after main refresh to `a059251507b899d48a6ab58b564cfa8a6d3d360f`. Main's one new CourseList packet had already been partially addressed on dot/course-list, and all blocked families had prior checkpoints.

Canonical checker/build source base: `a5041a5091efbf53fdbf99c6950133f2ec41e998`; a0592515 changed notes only. Candidate1 was committed locally as `fbcc7a5` before the unchanged canonical project build.

```sh
. ./development_environment.sh
export DEVKITARM=/usr
python make.py eu
.venv/bin/python tools/check.py fn_00277EC4 --object build/eu/obj/lib/al/src/Util/alItemTypeLookup.o
```

Exact output:
```text
U -> O: The complete source-generated function interval matches byte for byte.
```

The full compact build also linked/exported successfully. One candidate only. The compiled object came from the normal project build and its provenance sidecar, not a scratch invocation. No tools, global flags, data boundaries, or oracle changes.

## Semantics and independent evidence

The complete target is an ordered string discriminator using established `al::isEqualString(const char*, const char*)` at `0x00292308`. Empty and unknown names return zero. The thirty equality checks return values0–29 for these names, in order:

0 empty; 1 Coin; 2 Coin10; 3 CoinRandom10; 4 CoinInfinity; 5 PopCoin; 6 PopCoin3; 7 PopCoin5; 8 PopCoin5High; 9 PopCoin10; 10 PopCoinRandom5; 11 OneUp; 12 OneUpFast; 13 KinokoSuper; 14 KinokoSuperFast; 15 FireFlower; 16 FireFlowerForce; 17 SuperLeaf; 18 SuperLeafForce; 19 SuperLeafNormal; 20 SuperLeafSpecial; 21 BoomerangFlower; 22 BoomerangFlowerForce; 23 SuperStar; 24 Poison; 25 PoisonFast; 26 PatapataWing; 27 AssistItem; 28 Propeller; 29 KickKoura.

The source uses ordinary string literals and an if/return chain. It does not emit instruction arrays, a custom section, copied machine code or an imported string-pool alias. ARMCC itself placed all strings and padding identically in the function interval. The target does not guard null input; this source preserves that contract.

Independent callers support the input/output roles: `0x001215D0` obtains a string from a Byaml indexed query and stores the returned integer in a per-record table; `0x00266224`, `0x002CC830`, and `0x002D1264` retrieve the input through SafeString and consume the result as an item discriminator. The original public API spelling and enum type are not independently established, so the C-linkage address name is retained rather than guessed.

## Integration

Only local map naming required: the existing unchanged unnamed function row at `0x00277EC4`, pool `0x00278198`, end `0x00278320`, is named `fn_00277EC4`. `al::isEqualString` was already mapped. No new data identities or boundary changes are required. Main must import the source, name/revalidate the row, and run its canonical checker; this branch does not edit ranks, map, ledger, STATE, tools, or game binaries.

The current worktree also contains independently reported source proposals and the approved clean SafeString header changes; this function includes only alStringUtil.h and has one established external equality import, so none of those unrelated changes supplies its byte result.

## Reproducibility hashes

- EU code SHA256: `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`
- Source SHA256: `0430d41ceefa8631cb5ad97d31bf939e1b33a2b8c1f23c4817e27644b2a4bf37`
- Canonical object SHA256: `7c7fccca9de47d42fe2dcd53d2a4cb50aad60b8f784b04a93cfea9ab1b2d695f`
- ARMCC4.1/791 SHA256: `d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d`
