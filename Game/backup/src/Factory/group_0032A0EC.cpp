namespace {
struct FloatAtC {
    unsigned char prefix[12];
    float value;
};
}

extern "C" FloatAtC dat_003EFAE4;
extern "C" FloatAtC dat_003EFEE4;

extern "C" float fn_0032A0EC() {
    return dat_003EFAE4.value;
}

extern "C" float fn_0032A5CC() {
    return dat_003EFEE4.value;
}
