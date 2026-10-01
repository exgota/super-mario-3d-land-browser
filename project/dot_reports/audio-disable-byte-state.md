# Audio disable byte-state follow-up

Zero exact credit. Target fn_0024AD94 remains32 bytes with two differing bytes. One new ordinary representation hypothesis, after the packet's two forms, did not improve it. The experimental source was withdrawn; this branch proposes notes only.

Base/checker commit:5025a6cd5ec8531fb1bc40ff4b570a0c01ef201c. The original EU hash remains e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64. Canonical check.py and checkExactBytes.py are unchanged blobs7c0ccd93b7387a2f9afa9b304b5254f34149b318 and ce8fc0a5d747c1521d84a1ca1fbeaeab09e3bb47.

## New form and outcome

The packet called the byte at offset0 a bool. The new private view uses an unsigned byte of unknown semantic role, opaque storage through+0x0B, and the independently observed controller pointer at+0x0C. It preserves the existing void extern-C declaration used by accepted LiveActor callers. Committed source5053b97 was compiled by the unchanged project build; fresh make.py eu -ca linked and exported successfully.

Canonical command: `python tools/check.py fn_0024AD94 --object build/eu/obj/lib/al/src/Audio/alAudioKeeperDisable.o`.

Output: `U -> m: The linked candidate differs from the unchanged original interval.` Complete extent32 bytes; differences2. The original clears R0 then stores R0 at receiver+0; the candidate clears R1 then stores R1. All other instructions and the direct imported call agree. Original SHA-256 a4561c1e52e5379ecb5750099cca46e6bf7f22be1a863e902f74b155816474dd; candidate0f3c5ae89d6ca897610dc4732a2c3f26262fa38dc641c72fdd607786be4f2e64.

The actual candidate was:

```cpp
#ifdef NON_MATCHING
#include <Audio/alAudioKeeper.h>
namespace {
struct AudioKeeperDisableFields {
    unsigned char mStateByte;
    unsigned char mUnrecovered01[11];
    void* mSoundController;
};
}
extern "C" void fn_001D9F10(void*, int);
extern "C" void fn_0024AD94(al::AudioKeeper* keeper) {
    AudioKeeperDisableFields* fields = reinterpret_cast<AudioKeeperDisableFields*>(keeper);
    fn_001D9F10(fields->mSoundController, 0);
    fields->mStateByte = 0;
}
#endif
```

The local existing target row was named fn_0024AD94 without changing its bounds/pool/type. Only the checker assigned m. No map, source, flags, tools or tracking-file changes are in the final proposal. The failed code remains recoverable in the local source history; no NonMatching behavioral claim is made.

## Independent return-contract audit

A full aligned ARM branch scan finds3 direct branches to the target and13 to its tail-wrapper0026B948. The two accepted LiveActor call sites overwrite R0 immediately. The wrapper is a multi-path actor deactivation path and does not establish a typed return contract. Most inspected wrapper callers overwrite R0; two immediately following call chains also replace it before use:00346170 calls00272A9C→00189160→0028E688, whose first real data operation loads a global intoR0;00354918 calls00255DE8, which immediately setsR0 to13. Two outer tail branches remain untyped. These are negative observations, not proof of an original bool/int/void return type.

Changing the root's return type solely to force the final zero into R0 is therefore not justified. The next useful input is independent original API/return-contract or source-lowering evidence, not another zero-store spelling. Cumulative distinct root forms:3. No new helper address or original boundary was invented.
