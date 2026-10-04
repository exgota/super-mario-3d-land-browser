namespace {
struct Holder {
    char padding[0x20];
    void* object;
};
}

extern "C" void fn_00242E1C(void*);

extern "C" void fn_00265ECC(Holder* self) {
    fn_00242E1C(static_cast<char*>(self->object) + 0xC4);
}
