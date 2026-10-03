namespace {
struct Dispatch {
    virtual void a() = 0;
    virtual void b() = 0;
    virtual void c() = 0;
    virtual void d() = 0;
};
struct Owner {
    unsigned pad;
    Dispatch *dispatch;
};
}

extern "C" void fn_001A9544(Owner *self) {
    self->dispatch->d();
}

extern "C" void fn_001B99B4(Owner *self) {
    self->dispatch->d();
}
