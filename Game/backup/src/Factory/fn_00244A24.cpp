namespace {
    extern "C" void fn_0011091C(void*, unsigned short);
}

extern "C" void fn_00244A24(void* self, unsigned int value) {
    fn_0011091C(*reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(self) + 4),
                static_cast<unsigned short>(value));
}
