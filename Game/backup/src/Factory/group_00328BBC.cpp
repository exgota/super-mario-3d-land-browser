namespace {
struct RefHolder {
    unsigned char pad[0x44];
    void* value;
};
}

extern "C" RefHolder dat_003EFEE4;
extern "C" RefHolder dat_003EFAE4;

extern "C" void* fn_00328BBC() {
    return dat_003EFEE4.value;
}

extern "C" void* fn_0032A3DC() {
    return dat_003EFAE4.value;
}
