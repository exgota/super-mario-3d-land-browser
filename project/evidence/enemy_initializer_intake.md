# PackunFlower and Tenten initializer intake

Owner: Codex driver, branch `dot/enemy-initializer-intake`, frozen integrator main `f2ac15774d869bdee33e9559b45143f598613fce`. Final source commit: `deeb6f0efa7fd039d374c562b87316f0fcbb3d02`. PackunFlower source/header comes from dot/root-13d41c-source at `5d00e61ea9c013995e9879c078565161812a67e0`. Tenten source/header comes from dot/root-30a070 at `a922916005fb6506ab52f5e415b7674249a0f0a4`. The driver preserves both init bodies and the PackunFlower header byte-for-byte. Its only source correction changes Tenten's new header to the MapObjActor base directly called by the original constructor, with an explicit init-only prefix qualification.

The normal project ARMCC4.1/791 build of that committed final source passed. The canonical object checker passes both complete intervals, including literal pools: PackunFlower::init at0013D41C,544bytes, and Tenten::init at0030A070,572bytes. Final checks took 1.190369 seconds. Total proposed family size is1,116bytes; accepted credit remains zero pending the integrator. Historical checks on the unchanged supplier pair are retained separately and do not replace these final-source results.

The two new headers reach only their two new translation units. The normal linked image retains exactly the two new initializer functions from those objects. Their inherited weak functions and ordinary152-byte actor vtables are discarded; the existing SafeString table remains separately mapped. The complete family definition audit found no duplicate strong definitions after normal COMDAT exclusions. The final Tenten header correction preserves the function inventory, and the PackunFlower object hash is unchanged. No existing source/header, build configuration, compiler flag or oracle changes. The integrator must still run its complete previous-root and actual-definition preservation gate.

The separate identity commit642c9dabb4ec5c5dd5b7a6a2bc4baea1937af22c changes only four previously blank symbol cells, with evidence in `project/evidence/enemy_initializer_identities.md` and the decision log. Every rank, interval, literal-pool boundary and type remains unchanged. Provisional descriptive class spellings are explicit. Tenten's complete allocation extent and both complete virtual implementations remain unrecovered. Helper source bodies and semantic gameplay are not newly certified. The scratch map is restored to its committed named form after checks; no ranks are submitted.

## Final source hashes

- `Game/backup/include/Enemy/PackunFlower.h`: `9b51dbd544c0b08149364d8a2fc8cd4cafd590f8fec90475ca71fd89633a4868`
- `Game/backup/src/Enemy/PackunFlower.cpp`: `bfaa414c589a843fcb93d7a2f8db72ebf2287462969bb8d417196e7747d2148a`
- `Game/backup/include/Enemy/Tenten.h`: `e1e716701e9fd6a57f2465085d8a96fba233807ab029bb733d815f47e7f0c1fe`
- `Game/backup/src/Enemy/Tenten.cpp`: `9f8994694e2ff1e0ae29d4c10b540d69a71a4fdf38d90e5a56612ddb3f1571b2`
