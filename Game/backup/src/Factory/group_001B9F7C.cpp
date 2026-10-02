namespace {
struct VTable {
    unsigned int pad[9];
    void (*slot)(void*);
};
struct Interface {
    VTable* vtable;
};
struct Object {
    unsigned int pad;
    Interface* interface;
};
}

extern "C" void fn_001B9F8C(void*);
extern "C" void fn_002D281C(void*);
extern "C" void fn_002D3CC4(void*);

extern "C" void fn_001B9F7C(Object* self) {
    return self->interface->vtable->slot(self->interface);
}

extern "C" void fn_002D280C(Object* self) {
    return self->interface->vtable->slot(self->interface);
}

extern "C" void fn_002D3CB4(Object* self) {
    return self->interface->vtable->slot(self->interface);
}
