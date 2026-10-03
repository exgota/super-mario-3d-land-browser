namespace {
struct FloatValueAt8 {
    unsigned int unk_00;
    unsigned int unk_04;
    float value;
};
}

extern "C" {
extern FloatValueAt8 dat_003EFAE4;
extern FloatValueAt8 dat_003EFEE4;
float fn_00329E6C() {
    return dat_003EFAE4.value;
}
float fn_0032A75C() {
    return dat_003EFEE4.value;
}
}
