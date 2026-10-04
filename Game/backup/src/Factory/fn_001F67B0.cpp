namespace {
struct State {
    char pad[0x27c];
    int value;
};
}

extern "C" void fn_001F67B0(State* self) {
    self->value = -1;
}
