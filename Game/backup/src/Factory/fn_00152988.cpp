namespace {
struct ClippingComponent;
struct ClippingVTable {
    void (*slots[11])(ClippingComponent*);
};
struct ClippingComponent {
    ClippingVTable* vtable;
};
struct Actor {
    unsigned char base[0x78];
    ClippingComponent* component;
};
}

extern "C" void _ZN2al9LiveActor12startClippedEv(Actor*);

extern "C" void fn_00152988(Actor* actor) {
    _ZN2al9LiveActor12startClippedEv(actor);
    ClippingComponent* component = actor->component;
    return component->vtable->slots[10](component);
}
