namespace {
struct VTable {
    int reserved0;
    int reserved1;
    int reserved2;
    void (*callback)(void*, int);
};

struct Object {
    VTable* vtable;
};
}

extern "C" int fn_002CB4E0(void*, int*);

extern "C" int fn_002CB4B0(Object* object, int* data) {
    object->vtable->callback(object, 0);
    return fn_002CB4E0(object, data + 1);
}
