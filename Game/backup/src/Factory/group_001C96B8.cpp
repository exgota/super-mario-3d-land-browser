namespace {
struct Object {
    unsigned char padding[0x24];
    void* field_24;
};
}

extern "C" void* fn_001C96C0(void*);
extern "C" void* fn_001D9444(void*);

extern "C" void* fn_001C96B8(Object* self) {
    return fn_001C96C0(self->field_24);
}

extern "C" void* fn_001D943C(Object* self) {
    return fn_001D9444(self->field_24);
}
