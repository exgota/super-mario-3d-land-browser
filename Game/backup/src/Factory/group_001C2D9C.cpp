namespace {
struct Holder { void* value; };
struct OffsetHolder { char pad[0x28]; void* value; };
}

extern "C" void* fn_001C2DA4(void*);
extern "C" void* fn_00212104(void*);
extern "C" void* fn_002519B0(void*);

extern "C" void* fn_001C2D9C(OffsetHolder* self) {
    return fn_001C2DA4(self->value);
}

extern "C" void* fn_002120FC(OffsetHolder* self) {
    return fn_00212104(self->value);
}

extern "C" void* fn_002519A8(OffsetHolder* self) {
    return fn_002519B0(self->value);
}

namespace al {
struct LiveActor;
struct Velocity { };
Velocity const* getVelocity(LiveActor const*);
}

extern "C" void* fn_00272DD0(OffsetHolder* self) {
    return const_cast<al::Velocity*>(al::getVelocity(
        static_cast<al::LiveActor const*>(self->value)));
}
