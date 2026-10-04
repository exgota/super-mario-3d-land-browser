namespace {
struct State {
    unsigned char padding[0x50];
    int value;
    int count;
    bool initialized;
};
}

extern "C" void fn_0017EB34(State* state, int value) {
    if (!state->initialized) {
        state->value = value;
        state->count = 1;
        state->initialized = true;
    }
}
