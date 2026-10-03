namespace {
struct Object {
    unsigned char pad[0x6c];
    int value;
};
}

extern "C" void fn_0012EDF8(Object *self) {
    self->value = 10;
}
