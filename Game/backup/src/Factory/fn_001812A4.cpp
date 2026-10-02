namespace {
struct VTable {
    int (*slot0)(void*);
    int (*slot1)(void*);
    int (*slot2)(void*);
    int (*slot3)(void*);
    int (*slot4)(void*);
};

struct Object {
    char padding[0x14];
    void* child;
};
}

extern "C" int fn_001812A4(Object* object) {
    void* child = object->child;
    VTable* table = *reinterpret_cast<VTable**>(child);
    return table->slot4(child);
}
