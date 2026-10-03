# Plant initializer intake from dot/root-2f9060

The driver prepared this apply-clean family on frozen main `a0cfc18b1b7ad6c9af7762e763abd15388f43d65`. Supplier tip `21e656e7bc8298bd433c731ce3baf16d3bba3293` and committed local source `a78d668ea82eb3388a98f485ec448b6ef0c093db` have identical Plant source/header and quaternion header. The proposed complete initializer interval is 0x002F9060 through 0x002F92C4, 612 bytes. This is proposed credit until integrator acceptance.

## Identity and ownership

The original EU executable has SHA-256 `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`. Operator inspection confirmed that the initialization slot at vtable 0x003D26B4 + 4 points to 0x002F9060. Constructor 0x002F92C4 calls 0x00280428 and initializes the quaternion at offset 0x60, pointers at 0x70/0x74 and integer fields at 0x78/0x7C. The initializer uses the Plant item/archive name. Plant identity is inferred from this evidence; this proposal makes no factory-registration or complete-allocation claim.

Separate evidence commit `348f5727` changes only the function symbol in one map row, plus its decision entry. It preserves the row rank, interval and other fields. Source commit `a78d668ea82eb3388a98f485ec448b6ef0c093db` owns Plant.h, Plant.cpp and the copy overload in seadQuat.h for this family. The quaternion overload expresses ordinary four-component assignment, grounded in the loads/stores at 0x002F90B0 through 0x002F90C0. No unrelated source definition is changed.

## Local canonical verification

After sourcing development_environment.sh, the normal `python make.py eu` initial build and a normal relink with only the Plant scratch rank changed to M both succeeded. The project's unmodified `tools/check.py` reported O for Plant::init (612 bytes), PackunFlower::init (544 bytes), and Tenten::init (572 bytes). The latter two retain their prior exact status. The global compiled-definition audit found zero duplicate definitions. The entire scratch map was restored before this evidence commit; no ranks or ledger edits are submitted.

These are three targeted local checks. They do not claim preservation of every quaternion-header dependent function. The integrator must run its full prior-root preservation gate before acceptance. The supplier report's broader preservation result remains supplier evidence, not an independently repeated driver result. Source/helper closure consists of the new Plant member, its new inline quaternion copy overload and existing named project interfaces; no private duplicate helper definitions were added.

Local relink/check interval: 2026-10-03T17:10:00.291020+00:00 through 2026-10-03T17:11:55.449865+00:00. The initial full build began during the preceding hourly wake; this interval excludes that build and the future integrator gate. Therefore no end-to-end accepted throughput is claimed.

## Final source SHA-256

- `Game/backup/include/MapObj/Plant.h`: `9c4b73b65889b661a6f212121a12b704870dbafe1cdfabb1f52fe5a1bff5db23`
- `Game/backup/src/MapObj/Plant.cpp`: `dd79fcd793f01fb5e6b7aa71ed5d0f99f008660b3f6d4bdc82a7e8dd28947f48`
- `lib/sead/include/math/seadQuat.h`: `affaf74298df076ac0b7b3cb439572a86865b6a109a633391bef131fa50672e2`
