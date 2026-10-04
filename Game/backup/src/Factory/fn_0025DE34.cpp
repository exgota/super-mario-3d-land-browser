namespace {
struct Object {
    unsigned char padding[0x54];
    unsigned int field_54;
};
}

extern "C" void fn_0025DE34(Object *self) {
    self->field_54 = 0;
}
