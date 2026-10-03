namespace {
struct Holder {
    unsigned char pad[0x30];
    void* value;
};
}

extern "C" Holder* fn_00277658();
extern "C" void* fn_0012D0A4(void*);
extern "C" Holder* fn_0025BB60();
extern "C" void* fn_0025B4F4(void*);

extern "C" void* fn_0012D090() {
    return fn_0012D0A4(fn_00277658()->value);
}

extern "C" void* fn_0025B4E0() {
    return fn_0025B4F4(fn_0025BB60()->value);
}
