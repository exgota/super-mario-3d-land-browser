namespace {
struct Wrapper {
    unsigned int vtable;
    void* value;
};
}

extern "C" void* fn_00216EC4(void*);
extern "C" void* fn_00216EB0(void*);
extern "C" void* fn_002CC83C(void*);
extern "C" void* fn_00255518(void*);
extern "C" void* fn_0018F458(void*);

extern "C" void* fn_0032EA84(Wrapper* self) {
    return fn_002CC83C(fn_00216EC4(self->value));
}

extern "C" void* fn_0032EA98(Wrapper* self) {
    return fn_00255518(fn_00216EC4(self->value));
}

extern "C" void* fn_0032EC50(Wrapper* self) {
    return fn_0018F458(fn_00216EB0(self->value));
}

extern "C" void* fn_0032EC64(Wrapper* self) {
    return fn_002CC83C(fn_00216EB0(self->value));
}

extern "C" void* fn_0032EC78(Wrapper* self) {
    return fn_00255518(fn_00216EB0(self->value));
}
