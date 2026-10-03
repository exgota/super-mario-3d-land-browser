namespace {
struct Owner {
    unsigned char pad[0xC];
    void* target;
};
}

extern "C" void* fn_0019C02C(void*);
extern "C" void* fn_001D9EC0(void*);
extern "C" void* fn_001E3ED0(void*);
extern "C" void* fn_00256828(void*);

extern "C" void* fn_0019C024(Owner* self) { return fn_0019C02C(self->target); }
extern "C" void* fn_001D9EB8(Owner* self) { return fn_001D9EC0(self->target); }
extern "C" void* fn_001E3EC8(Owner* self) { return fn_001E3ED0(self->target); }
extern "C" void* fn_00256820(Owner* self) { return fn_00256828(self->target); }
