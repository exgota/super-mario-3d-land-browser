namespace {
typedef void *Ptr;
}

extern "C" Ptr fn_002519DC();
extern "C" Ptr fn_0027AD14();

extern "C" Ptr fn_001B8828() {
    return *reinterpret_cast<Ptr *>(reinterpret_cast<char *>(fn_002519DC()) + 0x10);
}

extern "C" Ptr fn_0024FEB8() {
    return *reinterpret_cast<Ptr *>(reinterpret_cast<char *>(fn_0027AD14()) + 0x10);
}
