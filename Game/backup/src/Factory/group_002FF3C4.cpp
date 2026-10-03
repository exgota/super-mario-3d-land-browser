namespace {
struct Dispatch {
    virtual void call() {}
};
}

extern "C" void fn_002FF3C4(void* p) {
    static_cast<Dispatch*>(p)->call();
}

extern "C" void fn_0034A670(void* p) {
    static_cast<Dispatch*>(p)->call();
}
