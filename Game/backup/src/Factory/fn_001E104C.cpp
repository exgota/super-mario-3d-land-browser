namespace {
extern "C" unsigned int dat_003B2A98;
extern "C" void nngxAdd3DCommand(unsigned int*, int, int);
}

extern "C" void fn_001E104C() {
    nngxAdd3DCommand(&dat_003B2A98, 0x10, 1);
}
