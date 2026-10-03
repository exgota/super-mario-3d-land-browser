namespace {
struct ObjectBEFE0 {
    unsigned char padding[0x4C];
    void* field_4C;
};

extern "C" void* fn_001BEFE8(void*);
extern "C" void* fn_001D2398(void*);
}

extern "C" void* fn_001BEFE0(ObjectBEFE0* self) {
    return fn_001BEFE8(self->field_4C);
}

extern "C" void* fn_001D2390(ObjectBEFE0* self) {
    return fn_001D2398(self->field_4C);
}
