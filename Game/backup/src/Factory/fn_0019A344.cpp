namespace {
struct Obj {
    unsigned char pad[0x6c];
    unsigned int value;
};
}

extern "C" void fn_0019A344(Obj* self, unsigned int value) {
    self->value = value;
}
