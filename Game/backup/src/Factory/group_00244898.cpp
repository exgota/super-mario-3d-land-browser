#include <stdint.h>

namespace al {
struct LiveActor {
    unsigned char pad[0x30];
    struct HitSensorKeeper* hitSensorKeeper;
};
struct HitSensorKeeper {
    void update();
};
}

extern "C" void* fn_002448A0(void*);

extern "C" void* fn_00244898(void* actor) {
    return fn_002448A0(*reinterpret_cast<void**>(static_cast<unsigned char*>(actor) + 0x30));
}

namespace alSensorFunction {
void updateHitSensorsAll(al::LiveActor* actor) {
    actor->hitSensorKeeper->update();
}
}
