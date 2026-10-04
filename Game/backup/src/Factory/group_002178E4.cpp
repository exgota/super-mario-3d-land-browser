namespace {
struct Object;
typedef void (*Method)(Object*);

struct MethodTable {
    Method slot0;
    Method slot1;
    Method refresh;
};

struct Object {
    MethodTable* methods;
    unsigned int value;
};
}

extern "C" unsigned int fn_002178E4(Object* object) {
    object->methods->refresh(object);
    return object->value;
}

extern "C" unsigned int fn_00263828(Object* object) {
    object->methods->refresh(object);
    return object->value;
}
