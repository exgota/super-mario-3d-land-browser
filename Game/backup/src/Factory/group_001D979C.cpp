namespace {
typedef void (*Dispatch)(void*);
struct VTable {
    unsigned char padding[0x4c];
    Dispatch entry;
};
struct Object {
    VTable* vtable;
};
}

extern "C" void fn_001D979C(void* self) {
    static_cast<Object*>(self)->vtable->entry(self);
}

extern "C" void fn_0024FC30(void* self) {
    static_cast<Object*>(self)->vtable->entry(self);
}

extern "C" void fn_00267E0C(void* self) {
    static_cast<Object*>(self)->vtable->entry(self);
}
