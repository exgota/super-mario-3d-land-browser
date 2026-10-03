namespace {
struct DataRecord {
    unsigned int unused;
    void* value;
};
}

extern "C" DataRecord dat_003E2594;
extern "C" DataRecord dat_003E23A8;

extern "C" void* fn_0023AAB4() {
    return dat_003E2594.value;
}

extern "C" void* fn_00293078() {
    return dat_003E23A8.value;
}
