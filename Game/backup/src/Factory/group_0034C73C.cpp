namespace {
struct Actor {
    virtual void slot0() = 0;
    virtual void slot1() = 0;
    virtual void slot2() = 0;
    virtual void slot3() = 0;
    virtual void slot4() = 0;
    virtual void slot5() = 0;
    char padding[0x60];
    void* state;
};

struct Context {
    Actor* actor;
};
}

extern "C" bool _ZN2al16updateNerveStateEPNS_9IUseNerveE(Actor*);
extern "C" void fn_00279158(Actor*, void*);

extern "C" void fn_0034C73C(void*, Context* context) {
    Actor* actor = context->actor;
    if (_ZN2al16updateNerveStateEPNS_9IUseNerveE(actor)) {
        fn_00279158(actor, actor->state);
        actor->slot5();
    }
}

extern "C" void fn_0034D82C(void*, Context* context) {
    Actor* actor = context->actor;
    if (_ZN2al16updateNerveStateEPNS_9IUseNerveE(actor)) {
        fn_00279158(actor, actor->state);
        actor->slot5();
    }
}

extern "C" void fn_00352960(void*, Context* context) {
    Actor* actor = context->actor;
    if (_ZN2al16updateNerveStateEPNS_9IUseNerveE(actor)) {
        fn_00279158(actor, actor->state);
        actor->slot5();
    }
}

extern "C" void fn_0036454C(void*, Context* context) {
    Actor* actor = context->actor;
    if (_ZN2al16updateNerveStateEPNS_9IUseNerveE(actor)) {
        fn_00279158(actor, actor->state);
        actor->slot5();
    }
}
