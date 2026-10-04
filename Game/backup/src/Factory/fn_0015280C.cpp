namespace {
struct ClippedObject;

struct ClippedObjectVTable {
    void (*reserved[11])();
    void (*endClipped)(ClippedObject*);
};

struct ClippedObject {
    ClippedObjectVTable* vtable;
};

struct Actor {
    unsigned char reserved[0x78];
    ClippedObject* clippedObject;
};
}

extern "C" void _ZN2al9LiveActor10endClippedEv(Actor*);

extern "C" void fn_0015280C(Actor* actor) {
    _ZN2al9LiveActor10endClippedEv(actor);
    ClippedObject* object = actor->clippedObject;
    return object->vtable->endClipped(object);
}
