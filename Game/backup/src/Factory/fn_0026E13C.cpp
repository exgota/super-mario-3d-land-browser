namespace al {
class HitSensorKeeper {
public:
    void update();
};
}

namespace {
struct Owner {
    char pad[0x30];
    al::HitSensorKeeper* keeper;
};
}

extern "C" void fn_0026E13C(Owner* owner) {
    owner->keeper->update();
}
