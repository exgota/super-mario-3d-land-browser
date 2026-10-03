namespace {
struct Interface {
    virtual void a() = 0;
    virtual void b() = 0;
    virtual void c() = 0;
    virtual void d() = 0;
    virtual void e() = 0;
    virtual void f() = 0;
};
}
extern "C" void fn_0018B1E0(Interface* self) { self->f(); }
extern "C" void fn_001A52D4(Interface* self) { self->f(); }
extern "C" void fn_001B5FB8(Interface* self) { self->f(); }
