namespace {
struct Holder {
    unsigned char pad[0x28];
    void* value;
};
}

extern "C" void* fn_00272414(void*);
namespace al {
class NerveExecutor {
public:
    void updateNerve();
};
class LiveActor;
const void* getTrans(const LiveActor*);
}

extern "C" void fn_001B5B34(Holder* self) {
    static_cast<al::NerveExecutor*>(self->value)->updateNerve();
}

extern "C" void* fn_001CDA40(Holder* self) {
    return fn_00272414(self->value);
}

extern "C" const void* fn_0027A5C0(Holder* self) {
    return al::getTrans(static_cast<const al::LiveActor*>(self->value));
}
