namespace {
struct Object {
    unsigned char pad[0x6C];
    short value;
};
}

extern "C" void fn_001FA9E0(void*);

extern "C" void fn_001FA9D4(Object* self) {
    self->value = -1;
    return fn_001FA9E0(self);
}
