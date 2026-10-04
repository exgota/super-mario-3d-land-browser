namespace {
struct Object;

struct Vtable {
    void* entries[2];
    void* (*getObject)(Object*);
};

struct Object {
    Vtable* vtable;
};
}

extern "C" void* fn_001BF270(void*, void*);
extern "C" void* fn_00330040(void*, void*);

extern "C" void* fn_001BF250(Object* object, void* argument) {
    return fn_001BF270(object->vtable->getObject(object), argument);
}

extern "C" void* fn_00330020(Object* object, void* argument) {
    return fn_00330040(object->vtable->getObject(object), argument);
}
