namespace {
struct KeeperHolder {
    char pad[0x30];
    void* keeper;
};
}

namespace al {
struct HitSensorKeeper {
    void* getSensor(char const*) const;
};
struct HitSensor {
    void* invalidate();
    void* validate();
};
}

extern "C" void* fn_001EB94C(KeeperHolder const* self, char const* name) {
    return static_cast<al::HitSensor*>(static_cast<al::HitSensorKeeper*>(self->keeper)->getSensor(name))->invalidate();
}

extern "C" void* fn_001EBB10(KeeperHolder const* self, char const* name) {
    return static_cast<al::HitSensor*>(static_cast<al::HitSensorKeeper*>(self->keeper)->getSensor(name))->validate();
}
