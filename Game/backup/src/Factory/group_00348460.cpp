namespace {
class Actor {
public:
    virtual void slot0() = 0;
    virtual void slot1() = 0;
    virtual void slot2() = 0;
    virtual void slot3() = 0;
    virtual void slot4() = 0;
    virtual void finish() = 0;
};
}

extern "C" bool _ZN2al16updateNerveStateEPNS_9IUseNerveE(Actor*);
extern "C" bool _ZN2al11isActionEndEPKNS_9LiveActorE(const Actor*);
extern "C" bool _ZN2al11isFirstStepEPKNS_9IUseNerveE(const Actor*);
extern "C" void fn_0025F41C(Actor*);
extern "C" void fn_00214374(Actor*);
extern "C" void _ZN2al25startHitReactionDisappearEPKNS_9LiveActorE(const Actor*);

extern "C" void fn_00348460(void*, Actor** context) {
    Actor* actor = *context;
    if (_ZN2al16updateNerveStateEPNS_9IUseNerveE(actor)) {
        fn_0025F41C(actor);
        actor->finish();
    }
}

extern "C" void fn_00357190(void*, Actor** context) {
    Actor* actor = *context;
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor)) {
        fn_00214374(actor);
        actor->finish();
    }
}

extern "C" void fn_0036A654(void*, Actor** context) {
    Actor* actor = *context;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor)) {
        _ZN2al25startHitReactionDisappearEPKNS_9LiveActorE(actor);
        actor->finish();
    }
}
