# Final ready-source intake before owner-requested pause

The owner requested integration of pending ready work before a clean machine pause. This family combines the unchanged source from dot/root-315c6c and dot/root-19df5c on frozen main `e499a54d61481811c464cb39beb2098ac07da063`. Compiled source commit: `ca31b50f5b857303567410ac72127514b6ac003b`. Proposed roots are fn_00315C6C (712 complete bytes) and fn_0019DF5C (720 complete bytes), total1432. No prior exact root or shared header is changed.

## Independent identity review

The original EU executable SHA-256 remains `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`. Registry0x003B9B10 pairs the Nokonoko label at0x003E031B with creator0x0039887C, which allocates0x88 and calls constructor0x003165D4. That constructor calls MapObjActor and lays out fields0x60 through0x84. Primary table0x003D4018 slot0x34 points to callback0x00315C6C. Helper0x0027A5FC reads the nerve step at0x0C and compares it <= the supplied threshold. Helper0x0031CD28 copies position and quaternion references and uses the float argument. Static constructor0x00380EBC initializes the existing four-byte nerve objects at0x003F2288/8C/90/94 with table identities0x003BBEE4/EF4/F04/F14. Only neutral missing names are added, with no table reconstruction or boundary change.

Original caller0x0019E4D4 passes the same receiver to the squat update0x0019DF5C and ignores the result. Constructor0x0019E5D4 allocates exactly eight bytes for the state record. Original code confirms the five-state dispatch, animator slots0x18/0x14, predicate slots0x08/0x24, service-prefix offsets and tail to0x0019E22C. The existing whole row0x003B1534..0x003B1540 contains SquatStart. Receiver/service layouts remain explicitly minimum observed prefixes, not recovered complete class identities. Existing imports have no competing declarations for the six neutral squat function names on the frozen main.

The driver verified14 map row changes are symbol-only: every rank, type, start, pool, end and other field is unchanged. Separate naming/evidence commits precede each source import. The final source and headers are byte-identical to the supplier branches. No original dump, checker, compiler flags, ledger or submitted rank is changed.

## Existing interface limitation

Nokonoko imports the existing void fn_0027D760(u32,const al::HitSensor*,const al::HitSensor*) contract already used by GhostPlayer and ReceiveMsg3135E0. Factory/group_00156F9C.cpp has a pre-existing bool/private-Sensor declaration whose calls discard the return; this proposal does not add a new signature or modify that existing code. Broader legacy declaration cleanup remains separate. This is a pre-existing source-contract cleanup candidate, not a new signature introduced by this family. The target uses the existing typed project interface and does not depend on the legacy declaration's return value.

## Verification

After sourcing development_environment.sh, the normal project build/link/export succeeded. The unmodified canonical tools/check.py reported O for fn_00315C6C (712bytes) and fn_0019DF5C (720bytes). The global compiled-definition audit found zero duplicate definitions. The complete scratch map was restored before this evidence commit; no ranks or ledger changes are submitted. Build/check ran from 2026-10-03T19:18:50.779840+00:00 through 2026-10-03T19:25:38.847046+00:00, 408.1 seconds. This verifies the two proposed intervals locally. The integrator's full preservation gate remains mandatory; accepted throughput remains zero until its verdict.

## Source SHA-256

- `Game/backup/include/Enemy/Nokonoko.h`: `1809dbde8460ae815cba303d031a523852e4a253fe457f305f2999dfd380ae8a`
- `Game/backup/src/Enemy/NokonokoReceiveMessage.cpp`: `d49cef9e46efc63a8754cb84d0fd747051bdc1251bcc4bfd39037df4f60798ca`
- `Game/backup/include/Player/ObservedSquatAction.h`: `74b99d9a0c169ce9d401122c820b40831d367c256a53770f8e7515af8456a3dc`
- `Game/backup/src/Player/ObservedSquatAction.cpp`: `9bd74e8c5a411b579cdf68d0046a96ea624fc7deec565ebb85fd5b8dd4d36c77`
