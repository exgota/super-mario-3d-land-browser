namespace {
struct Child;
struct ChildVtable {
    void (*slots[5])(Child*);
    void (*kill)(Child*);
};

struct Child {
    ChildVtable* vtable;
};

struct Actor {
    unsigned char reserved[0x78];
    Child* child;
};
}

extern "C" void _ZN2al9LiveActor4killEv(Actor*);

extern "C" void fn_00152AB4(Actor* actor) {
    _ZN2al9LiveActor4killEv(actor);
    Child* child = actor->child;
    return child->vtable->kill(child);
}
