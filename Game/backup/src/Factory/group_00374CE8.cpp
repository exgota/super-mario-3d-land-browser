namespace {
typedef unsigned char Byte;
}

extern "C" void* fn_0021DEFC(void*);
extern "C" void* fn_0021DEC8(void*);

extern "C" void* fn_00374CE8(void* value) {
    return fn_0021DEFC(static_cast<Byte*>(value) + 8);
}

extern "C" void* fn_00374CF0(void* value) {
    return fn_0021DEC8(static_cast<Byte*>(value) + 8);
}
