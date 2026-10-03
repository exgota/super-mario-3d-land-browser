namespace {
struct Dispatch {
    virtual void slot0() = 0;
    virtual void slot1() = 0;
    virtual void slot2() = 0;
    virtual void slot3() = 0;
};
}

extern "C" void fn_002DE904(void*, Dispatch* self) {
    self->slot3();
}

extern "C" void fn_002DFD74(void*, Dispatch* self) {
    self->slot3();
}
