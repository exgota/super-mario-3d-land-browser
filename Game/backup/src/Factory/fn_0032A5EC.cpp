namespace {
struct Data {
    unsigned char pad[0x218];
    float value;
};
}
extern "C" Data dat_003EFAE4;
extern "C" float fn_0032A5EC() {
    return dat_003EFAE4.value;
}
