namespace {
struct DispatchTarget {
    void** vtable;
};
}

extern "C" void fn_002587DC(DispatchTarget* self)
{
    reinterpret_cast<void (*)(DispatchTarget*)>(self->vtable[22])(self);
}
