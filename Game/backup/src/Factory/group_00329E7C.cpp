namespace {
struct DataObject {
    unsigned char padding[0x30];
    float value;
};
}

extern "C" DataObject dat_003EFAE4;
extern "C" DataObject dat_003EFEE4;

extern "C" float fn_00329E7C() {
    return dat_003EFAE4.value;
}

extern "C" float fn_0032A89C() {
    return dat_003EFEE4.value;
}
