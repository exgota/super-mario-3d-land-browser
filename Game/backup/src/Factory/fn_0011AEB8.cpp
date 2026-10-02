namespace {
struct State {
    unsigned char padding[0x9c];
    int value;
};
}

extern "C" int fn_0011AEB8(State *state) {
    int value = state->value;
    if (value >= 3)
        return 0;
    state->value = value + 1;
    return 1;
}
