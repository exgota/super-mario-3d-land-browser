namespace {
typedef unsigned int u32;
typedef unsigned char u8;
}

extern "C" void fn_001F2BC0(void *object) {
    volatile u32 *words = static_cast<volatile u32 *>(object);
    words[0] = 0;
    words[1] = 0;
    words[2] = 0;
    static_cast<volatile u8 *>(object)[12] = 0;
}

extern "C" void fn_002984BC(void *object) {
    volatile u32 *words = static_cast<volatile u32 *>(object);
    words[0] = 0;
    words[1] = 0;
    words[2] = 0;
    static_cast<volatile u8 *>(object)[12] = 0;
}
