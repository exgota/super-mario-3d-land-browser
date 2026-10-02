namespace {
struct Data {
    char padding[0x2ec];
    void* value;
};
}

extern "C" Data dat_003EFAE4;

extern "C" void* fn_0032A4AC() {
    return dat_003EFAE4.value;
}
