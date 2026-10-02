namespace {
struct Object;
typedef void (*Method)(Object*);
struct VTable {
    void* first;
    void* second;
    Method third;
};
struct Object {
    VTable* vtable;
};
}

extern "C" void fn_002FF3D0(Object* object) {
    object->vtable->third(object);
}
