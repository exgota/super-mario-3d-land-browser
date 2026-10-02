namespace {
class Actor {
public:
    virtual void slot0() = 0;
    virtual void slot1() = 0;
    virtual void slot2() = 0;
    virtual void slot3() = 0;
    virtual void slot4() = 0;
    virtual void slot5() = 0;
    char padding[0x64];
    void* state;
};
}

extern "C" bool _ZN2al16updateNerveStateEPNS_9IUseNerveE(Actor*);
extern "C" void fn_00279158(Actor*, void*);

extern "C" void fn_00348FE4(void*, Actor** argument) {
    Actor* actor = *argument;
    if (_ZN2al16updateNerveStateEPNS_9IUseNerveE(actor)) {
        fn_00279158(actor, actor->state);
        actor->slot5();
    }
}

extern "C" void fn_0034F1A0(void*, Actor** argument) {
    Actor* actor = *argument;
    if (_ZN2al16updateNerveStateEPNS_9IUseNerveE(actor)) {
        fn_00279158(actor, actor->state);
        actor->slot5();
    }
}

extern "C" void fn_00350294(void*, Actor** argument) {
    Actor* actor = *argument;
    if (_ZN2al16updateNerveStateEPNS_9IUseNerveE(actor)) {
        fn_00279158(actor, actor->state);
        actor->slot5();
    }
}
