namespace {
struct StateAt68 {
    unsigned char pad[0x10];
    int enabled;
};
struct Owner {
    unsigned char pad[0x68];
    StateAt68* state;
};
}

extern "C" void fn_0026F9E4();
extern "C" void fn_0012E654();

extern "C" void fn_001426B8(Owner* self) {
    if (self->state->enabled)
        fn_0026F9E4();
}

extern "C" void fn_0014291C(Owner* self) {
    if (self->state->enabled)
        fn_0012E654();
}
