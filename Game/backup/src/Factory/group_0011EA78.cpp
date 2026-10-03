namespace {
struct ChildActor {
    void (**vtable)(ChildActor*);
};

struct Actor {
    unsigned char padding[0x60];
    ChildActor* child;
};
}

extern "C" void _ZN2al9LiveActor13makeActorDeadEv(Actor*);

extern "C" void fn_0011EA78(Actor* actor) {
    _ZN2al9LiveActor13makeActorDeadEv(actor);
    ChildActor* child = actor->child;
    return child->vtable[6](child);
}

extern "C" void fn_0012FB3C(Actor* actor) {
    _ZN2al9LiveActor13makeActorDeadEv(actor);
    ChildActor* child = actor->child;
    return child->vtable[6](child);
}
