namespace {
struct Forwarder {
    unsigned char pad[0x1C];
    void* value;
};
}

extern "C" void* fn_0025BC6C(void*);
extern "C" void* fn_0026B434(void*);
extern "C" void* fn_0026B04C(void*);

extern "C" void* fn_00183F38(Forwarder* self) {
    return fn_0025BC6C(self->value);
}

extern "C" void* fn_0025BA74(Forwarder* self) {
    return fn_0026B434(self->value);
}

extern "C" void* fn_0025BA98(Forwarder* self) {
    return fn_0026B04C(self->value);
}
