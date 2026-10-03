namespace {
struct Receiver {
    void** vtable;
};
typedef void (*Dispatch)(void*);
}

extern "C" void fn_00273094(void* self)
{
    Dispatch dispatch = (Dispatch)static_cast<Receiver*>(self)->vtable[7];
    dispatch(self);
}

extern "C" void fn_002F0B2C(void* self)
{
    Dispatch dispatch = (Dispatch)static_cast<Receiver*>(self)->vtable[7];
    dispatch(self);
}
