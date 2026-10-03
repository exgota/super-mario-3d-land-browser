namespace al {
class HitSensorKeeper {
public:
    void validate();
    void invalidate();
};
}

namespace {
struct HitSensorKeeperLayout {
    unsigned char pad[0x30];
    void *sensor;
};
}

extern "C" void fn_0027798C(al::HitSensorKeeper *self) {
    al::HitSensorKeeper *sensor = static_cast<al::HitSensorKeeper *>(reinterpret_cast<HitSensorKeeperLayout *>(self)->sensor);
    if (sensor)
        sensor->validate();
}

extern "C" void fn_00277AF0(al::HitSensorKeeper *self) {
    al::HitSensorKeeper *sensor = static_cast<al::HitSensorKeeper *>(reinterpret_cast<HitSensorKeeperLayout *>(self)->sensor);
    if (sensor)
        sensor->invalidate();
}
