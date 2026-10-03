namespace {
struct DispatchObject {
    void** vtable;
};
}

extern "C" DispatchObject* fn_00265E80(void*);
extern "C" DispatchObject* fn_0027BE70(void*);

extern "C" void fn_00277674(void* self) {
    DispatchObject* result = fn_00265E80(self);
    ((void (*)(DispatchObject*))result->vtable[7])(result);
}

extern "C" void fn_0027DF88(void* self) {
    DispatchObject* result = fn_0027BE70(self);
    ((void (*)(DispatchObject*))result->vtable[7])(result);
}
