namespace {

struct AppearingComponent {
    virtual void slot0() = 0;
    virtual void slot1() = 0;
    virtual void slot2() = 0;
    virtual void slot3() = 0;
    virtual void appear() = 0;
};

struct Actor {
    unsigned char padding[0x60];
    AppearingComponent* component;
};

}

extern "C" void _ZN2al9LiveActor17makeActorAppearedEv(void* actor);
extern "C" void fn_0011EAB8(Actor* actor);
extern "C" void fn_0012FB7C(Actor* actor);

extern "C" void fn_0011EA98(Actor* actor) {
    _ZN2al9LiveActor17makeActorAppearedEv(actor);
    return actor->component->appear();
}

extern "C" void fn_0012FB5C(Actor* actor) {
    _ZN2al9LiveActor17makeActorAppearedEv(actor);
    return actor->component->appear();
}
