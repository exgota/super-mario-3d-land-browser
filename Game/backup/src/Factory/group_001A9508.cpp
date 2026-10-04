namespace {
struct Object {
    const void* vtable;
    unsigned int field4;
    unsigned int field8;
    unsigned int fieldC;
};

extern "C" const unsigned char dat_003D0B8C;
extern "C" const unsigned char dat_003D72DC;
}

extern "C" void fn_001A9508(Object* self) {
    self->field4 = 0;
    self->vtable = &dat_003D0B8C;
    self->field8 = 0;
    self->fieldC = 0;
}

extern "C" void fn_001E2BCC(Object* self) {
    self->field4 = 0;
    self->vtable = &dat_003D72DC;
    self->field8 = 0;
    self->fieldC = 0;
}
