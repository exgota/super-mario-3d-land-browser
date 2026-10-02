namespace {
struct FlagFields {
    char padding[0x40];
    signed char value;
};
}

extern "C" int fn_00326CC4(void* source) {
    return static_cast<FlagFields*>(source)->value;
}

extern "C" int fn_00377CD0(void* source) {
    return static_cast<FlagFields*>(source)->value;
}

extern "C" int fn_00377CD8(void* source) {
    return static_cast<FlagFields*>(source)->value;
}
