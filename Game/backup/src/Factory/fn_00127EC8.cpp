namespace {
struct Child {
    virtual void slot0() = 0;
    virtual void slot1() = 0;
    virtual void kill() = 0;
};

struct Actor {
    char padding[0x30];
    Child* child;
};
}

extern "C" void _ZN2al11LayoutActor4killEv(Actor* actor);

extern "C" void fn_00127EC8(Actor* actor) {
    _ZN2al11LayoutActor4killEv(actor);
    return actor->child->kill();
}
