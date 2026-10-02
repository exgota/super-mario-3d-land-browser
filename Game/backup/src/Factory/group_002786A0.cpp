namespace {
struct Dispatch {
    virtual void f0() {}
    virtual void f1() {}
    virtual void f2() {}
    virtual void f3() {}
    virtual void f4() {}
    virtual void f5() {}
    virtual void invoke() {}
};
}

extern "C" void fn_002786A0(Dispatch* self) { self->invoke(); }
extern "C" void fn_00336098(Dispatch* self) { self->invoke(); }
extern "C" void fn_0033625C(Dispatch* self) { self->invoke(); }
extern "C" void fn_00336978(Dispatch* self) { self->invoke(); }
