namespace {
struct Data003EFAE4 {
    unsigned char padding[0x1c4];
    float value;
};
}

extern "C" {
extern Data003EFAE4 dat_003EFAE4;
float fn_00329F3C() {
    return dat_003EFAE4.value;
}
}
