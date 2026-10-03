namespace {
struct Keeper;
struct State;
struct Object {
    unsigned char unknown[0x3c];
    Keeper* keeper;
};

extern "C" State* fn_00214A2C(Keeper*);
extern "C" void fn_002149F4(State*);
extern "C" void fn_0033F820(State*);
}

extern "C" void fn_0033FF5C(Object* object) {
    State* state = fn_00214A2C(object->keeper);
    if (state)
        fn_002149F4(state);
}

extern "C" void fn_00340198(Object* object) {
    State* state = fn_00214A2C(object->keeper);
    if (state)
        fn_0033F820(state);
}
