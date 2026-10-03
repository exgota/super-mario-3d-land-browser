namespace {
struct Dispatch {
    virtual void unused0() {}
    virtual void unused1() {}
    virtual void unused2() {}
    virtual void unused3() {}
    virtual void invoke() {}
};
struct Inner { unsigned char pad[16]; Dispatch *dispatch; };
struct Outer { unsigned char pad[8]; Inner *inner; };
}

extern "C" void fn_00181570(void *self)
{
    reinterpret_cast<Outer *>(self)->inner->dispatch->invoke();
}

extern "C" void fn_0018EC1C(void *self)
{
    reinterpret_cast<Outer *>(self)->inner->dispatch->invoke();
}
