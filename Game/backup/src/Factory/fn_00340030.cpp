namespace {
struct Object {
    unsigned char padding[0x3c];
    void* field_3c;
};
}

extern "C" void* fn_0033F0AC(void*);

extern "C" void* fn_00340030(Object* self) {
    return fn_0033F0AC(self->field_3c);
}
