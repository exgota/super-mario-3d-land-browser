namespace al {
struct HitSensorKeeper {
    void *getSensor(const char *) const;
};
}

namespace {
struct Wrapper {
    unsigned char pad[0x30];
    al::HitSensorKeeper *keeper;
};
}

extern "C" void *fn_001EB9C0(void *, void *);
extern "C" void *fn_001EB9EC(void *, void *);

extern "C" void *fn_001EB9A4(const Wrapper *self, const char *name, void *arg)
{
    return fn_001EB9C0(self->keeper->getSensor(name), arg);
}

extern "C" void *fn_001EB9D0(const Wrapper *self, const char *name, void *arg)
{
    return fn_001EB9EC(self->keeper->getSensor(name), arg);
}
