namespace {
extern "C" unsigned char dat_003EFEE4;
extern "C" unsigned char dat_003EFAE4;
}

extern "C" void *fn_00328C1C() {
    return *reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(&dat_003EFEE4) + 0x5C);
}

extern "C" void *fn_0032A02C() {
    return *reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(&dat_003EFAE4) + 0x5C);
}
