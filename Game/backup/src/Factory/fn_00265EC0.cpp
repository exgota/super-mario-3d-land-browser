namespace {
struct Value {
    char padding[0x14];
    float value;
};

struct Object {
    char padding[0x20];
    Value* value;
};
}

extern "C" float fn_00265EC0(Object* object) {
    return object->value->value;
}
