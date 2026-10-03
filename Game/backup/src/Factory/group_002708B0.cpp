namespace {
struct Owner {
    unsigned char padding[0x34];
    void* target;
};
}

extern "C" void* fn_0015A9A4(void*);
extern "C" void* fn_00328948(void*);

extern "C" void* fn_002708B0(Owner* self) {
    void* target = self->target;
    if (target == 0)
        return target;
    return fn_0015A9A4(target);
}

extern "C" void* fn_0032C8C8(Owner* self) {
    void* target = self->target;
    if (target == 0)
        return target;
    return fn_00328948(target);
}
