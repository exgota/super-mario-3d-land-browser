namespace {
struct FloatAt10 {
    unsigned char pad[0x10];
    float value;
};
}

extern "C" FloatAt10 dat_003EFAE4;
extern "C" FloatAt10 dat_003EFEE4;

extern "C" float fn_00329DEC() {
    return dat_003EFAE4.value;
}

extern "C" float fn_0032AAFC() {
    return dat_003EFEE4.value;
}
