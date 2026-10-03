namespace {
struct FloatHolder {
    unsigned char pad[0x50];
    float value;
};
}

extern "C" FloatHolder dat_003EFEE4;
extern "C" FloatHolder dat_003EFAE4;

extern "C" float fn_00328BAC() {
    return dat_003EFEE4.value;
}

extern "C" float fn_0032A5FC() {
    return dat_003EFAE4.value;
}
