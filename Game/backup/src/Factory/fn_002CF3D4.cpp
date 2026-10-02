namespace {
struct VTable {
    void* slots[13];
};
struct Object {
    VTable* vtable;
};
}

extern "C" void* fn_002CF3D4(Object* self) {
    typedef void* (*Method)(Object*);
    Method method = reinterpret_cast<Method>(self->vtable->slots[13]);
    return method(self);
}
