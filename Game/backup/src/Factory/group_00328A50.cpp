namespace {
struct Wrapper {
    unsigned char pad[0x6c];
    void* value;
};
}

extern "C" void* fn_00328A60(void*, void*);
extern "C" void* fn_00328ABC(void*, void*);

extern "C" void* fn_00328A50(void* arg, Wrapper* self) {
    return fn_00328A60(self->value, arg);
}

extern "C" void* fn_00328AAC(void* arg, Wrapper* self) {
    return fn_00328ABC(self->value, arg);
}
