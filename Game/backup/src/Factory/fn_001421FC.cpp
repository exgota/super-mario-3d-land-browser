namespace {
struct KeeperResult {
    unsigned char padding[0x68];
    void* value;
};

struct Receiver {
    unsigned char padding[0x6C];
    void* keeper;
};
}

extern "C" KeeperResult* fn_00267BFC(void* keeper);

extern "C" bool fn_001421FC(Receiver* self) {
    return fn_00267BFC(self->keeper)->value == 0;
}
