namespace {
typedef unsigned int u32;
struct Object {
    unsigned char pad[0x3c];
    void* value;
};
}

extern "C" void* fn_0033F49C(void*);
extern "C" void* fn_0033F4B4(void*);

extern "C" u32 fn_0033FEF8(Object* self) {
    return *static_cast<u32*>(fn_0033F49C(self->value));
}

extern "C" u32 fn_003400E8(Object* self) {
    return *static_cast<u32*>(fn_0033F4B4(self->value));
}
