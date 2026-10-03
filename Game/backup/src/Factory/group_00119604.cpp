namespace {
struct Component;
typedef void (*ComponentMethod)(Component*);

struct ComponentVTable {
    ComponentMethod slots[7];
};

struct Component {
    ComponentVTable* vtable;
};

struct Actor {
    unsigned char padding[0x98];
    Component* component;
};
}

extern "C" void _ZN2al9LiveActor13makeActorDeadEv(Actor*);
extern "C" void fn_00119624(Component*);
extern "C" void fn_0032463C(Component*);

extern "C" void fn_00119604(Actor* actor) {
    _ZN2al9LiveActor13makeActorDeadEv(actor);
    Component* component = actor->component;
    return component->vtable->slots[6](component);
}

extern "C" void fn_0032461C(Actor* actor) {
    _ZN2al9LiveActor13makeActorDeadEv(actor);
    Component* component = actor->component;
    return component->vtable->slots[6](component);
}
