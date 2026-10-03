namespace {
struct DispatchObject {
    void (**vtable)(DispatchObject*, int);
};
}

extern "C" void fn_002F0960(DispatchObject* self, int arg) {
    self->vtable[9](self, arg);
}

extern "C" void fn_00336A48(DispatchObject* self, int arg) {
    self->vtable[9](self, arg);
}
