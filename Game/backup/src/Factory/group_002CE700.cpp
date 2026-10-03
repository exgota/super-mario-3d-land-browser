extern "C" unsigned char dat_003EFEE4;
extern "C" unsigned char dat_003EFAE4;

extern "C" void* fn_002CE700() {
    return *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(&dat_003EFEE4) + 0x60);
}

extern "C" void* fn_0032A50C() {
    return *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(&dat_003EFAE4) + 0x60);
}
