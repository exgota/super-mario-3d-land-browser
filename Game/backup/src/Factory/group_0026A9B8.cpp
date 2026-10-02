namespace {
struct Owner {
    unsigned char pad[0x24];
    void* value;
};
}

extern "C" Owner* fn_002281D0(void*);
extern "C" int fn_0019A960(void*, void*);
extern "C" int fn_002534B8(void*, void*);

extern "C" int fn_0026A9B8(void* self) {
    Owner* owner = fn_002281D0(self);
    return fn_0019A960(owner->value, self);
}

extern "C" int fn_0026AA60(void* self) {
    Owner* owner = fn_002281D0(self);
    return fn_002534B8(owner->value, self);
}
