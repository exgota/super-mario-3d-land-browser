extern "C" unsigned int fn_00147D48(unsigned int);

extern "C" unsigned int fn_00147D40(unsigned int* p) {
    return fn_00147D48(p[0x90 / sizeof(unsigned int)]);
}
