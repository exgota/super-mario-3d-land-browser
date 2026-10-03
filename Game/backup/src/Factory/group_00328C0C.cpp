namespace {
struct State {
    unsigned char padding[0x58];
    float value;
};
}

extern "C" State dat_003EFEE4;
extern "C" State dat_003EFAE4;

extern "C" float fn_00328C0C() {
    return dat_003EFEE4.value;
}

extern "C" float fn_0032A06C() {
    return dat_003EFAE4.value;
}
