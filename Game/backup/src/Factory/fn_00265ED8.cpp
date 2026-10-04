namespace {
struct Holder {
    unsigned char padding[0x20];
    void* object;
};
}

extern "C" void* fn_00265ED8(Holder* self) {
    return static_cast<char*>(self->object) + 0x124;
}
