namespace {
typedef unsigned char Byte;
}

extern "C" void *fn_0021DEC8(void *);
extern "C" void *fn_0021DEFC(void *);
extern "C" void *fn_0022AE34(void *);

extern "C" void *fn_0021DEC0(void *self) {
    return fn_0021DEC8(static_cast<Byte *>(self) + 8);
}

extern "C" void *fn_0021DEF4(void *self) {
    return fn_0021DEFC(static_cast<Byte *>(self) + 8);
}

extern "C" void *fn_0022AE2C(void *self) {
    return fn_0022AE34(static_cast<Byte *>(self) + 8);
}
