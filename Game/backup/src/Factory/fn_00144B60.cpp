namespace {
struct Component;

struct ComponentVtable {
    void (*slot0)(Component*);
    void (*slot1)(Component*);
    void (*slot2)(Component*);
    void (*appear)(Component*);
};

struct Component {
    ComponentVtable* vtable;
};

struct Actor {
    char unknown[0x70];
    Component* component;
};
}

extern "C" void _ZN2al9LiveActor6appearEv(Actor*);

extern "C" void fn_00144B60(Actor* actor) {
    _ZN2al9LiveActor6appearEv(actor);
    Component* component = actor->component;
    return component->vtable->appear(component);
}
