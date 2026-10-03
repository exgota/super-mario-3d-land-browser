namespace {
struct Object {
    unsigned char padding[0x10];
    unsigned char flag;
};
}

extern "C" void fn_0014A81C(Object* self) {
    self->flag = 1;
}
