namespace {
struct Owner {
    unsigned char padding[0x1C];
    void* field_1C;
};
}

extern "C" void* fn_00248E50(void*);
extern "C" void* fn_002738B0(void*);

extern "C" void* fn_00248E48(Owner* self) {
    return fn_00248E50(self->field_1C);
}

extern "C" void* fn_002738A8(Owner* self) {
    return fn_002738B0(self->field_1C);
}
