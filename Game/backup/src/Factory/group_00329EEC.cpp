namespace {
struct Data {
    unsigned char pad[0x38];
    float value;
};
}

extern "C" Data dat_003EFAE4;
extern "C" Data dat_003EFEE4;

extern "C" float fn_00329EEC() {
    return dat_003EFAE4.value;
}

extern "C" float fn_0032A6AC() {
    return dat_003EFEE4.value;
}
