namespace {
struct Component {
    virtual void slot0() = 0;
    virtual void slot1() = 0;
    virtual void slot2() = 0;
    virtual void slot3() = 0;
    virtual void appear() = 0;
};

struct Actor {
    unsigned char padding[0x98];
    Component* component;
};
}

extern "C" void _ZN2al9LiveActor17makeActorAppearedEv(Actor*);

extern "C" void fn_00119624(Actor* actor) {
    _ZN2al9LiveActor17makeActorAppearedEv(actor);
    return actor->component->appear();
}

extern "C" void fn_0032463C(Actor* actor) {
    _ZN2al9LiveActor17makeActorAppearedEv(actor);
    return actor->component->appear();
}
