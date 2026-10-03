namespace {
struct Fn00329490Object {
    unsigned char pad[0x68];
    unsigned int value;
};
}

extern "C" unsigned int fn_00329490(void* self) {
    return static_cast<Fn00329490Object*>(self)->value;
}
