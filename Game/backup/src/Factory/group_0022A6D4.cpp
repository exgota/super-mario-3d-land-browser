namespace {
typedef unsigned char *Ptr;
extern "C" void fn_002323E8(Ptr, Ptr, Ptr, Ptr);
}

extern "C" void fn_0022A6D4(Ptr a, Ptr b) {
    fn_002323E8(a, a + 4, b + 4, a + 4);
}

extern "C" void fn_002B5590(Ptr a, Ptr b) {
    fn_002323E8(a, a + 4, b + 4, a + 4);
}
