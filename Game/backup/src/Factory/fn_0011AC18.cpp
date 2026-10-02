namespace {
struct Object;

struct VTable {
    void (*slots[7])(Object*);
};

struct Object {
    VTable* vtable;
};

struct Owner {
    unsigned int padding[2];
    Object* object;
};
}

extern "C" void fn_0011AC18(Owner* owner) {
    Object* object = owner->object;
    if (object)
        object->vtable->slots[6](object);
}
