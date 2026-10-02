namespace {
struct Dispatch {
    virtual void invoke() {}
};
struct Holder { unsigned int reserved; Dispatch* dispatch; };
}

#define BODY(name) extern "C" void name(Holder* self) { self->dispatch->invoke(); }
BODY(fn_001B9984)
BODY(fn_001B9DA8)
BODY(fn_001B9EB8)
BODY(fn_001BABB8)
BODY(fn_001BAC38)
BODY(fn_001D44E0)
BODY(fn_002D27C4)
BODY(fn_002D2BF8)
BODY(fn_002D2C1C)
BODY(fn_002D36D0)
BODY(fn_003573A0)
BODY(fn_003573B0)
