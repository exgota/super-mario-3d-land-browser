namespace {
struct Interface {
    virtual void f0() {}
    virtual void f1() {}
    virtual void f2() {}
    virtual void f3() {}
    virtual int call() { return 0; }
};
}

extern "C" int fn_00334658(Interface* self) {
    return self->call();
}
