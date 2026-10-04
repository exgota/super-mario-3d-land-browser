namespace {
struct Value {
    char pad[0x38];
    float value;
};

struct Owner {
    char pad[0x10];
    Value* value;
};
}

extern "C" float fn_0026252C(Owner* owner) {
    return owner->value->value;
}
