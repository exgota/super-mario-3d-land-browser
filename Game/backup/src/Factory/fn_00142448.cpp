namespace {
struct StateObject {
    char padding[0x68];
    int state;
};

struct Actor {
    char padding[0x6c];
    void* keeper;
};
}

extern "C" StateObject* fn_00267BFC(void* keeper);

extern "C" bool fn_00142448(Actor* actor) {
    return fn_00267BFC(actor->keeper)->state == 2;
}
