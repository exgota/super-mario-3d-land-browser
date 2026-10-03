namespace {
struct Actor {
    virtual void slot0() = 0;
    virtual void slot1() = 0;
    virtual void slot2() = 0;
    virtual void slot3() = 0;
    virtual void slot4() = 0;
    virtual void slot5() = 0;
    virtual void kill() = 0;
    unsigned char padding[0x86];
    bool flag;
};
struct Context { Actor* actor; };
struct Nerve {};
}

extern "C" bool fn_00279ED4(const Actor*, int);
extern "C" void fn_00279e8c(Actor*, float);
extern "C" void fn_00257D10(Actor*, float, float);
extern "C" bool _ZN2al11isFirstStepEPKNS_9IUseNerveE(const Actor*);
extern "C" void fn_00272D78(Actor*, float);
extern "C" void fn_00272D50(Actor*, float);
extern "C" void _ZN2al11startActionEPNS_9LiveActorEPKc(Actor*, const char*);
extern "C" void fn_00272B60(Actor*, int);
extern "C" bool _ZN2al11isActionEndEPKNS_9LiveActorE(const Actor*);
extern "C" void _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(Actor*, const Nerve*);
extern "C" const char dat_003BCC64[];
extern "C" const Nerve dat_003F24E0;

extern "C" void fn_00354D84(const void*, const Context* context) {
    Actor* actor = context->actor;
    if (!fn_00279ED4(actor, 3))
        fn_00279e8c(actor, 1.0f);
    fn_00257D10(actor, 0.5f, 1.0f);
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor)) {
        fn_00272D78(actor, 0.0f);
        fn_00272D50(actor, 0.0f);
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003BCC64);
        fn_00272B60(actor, 3);
    }
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor)) {
        if (actor->flag)
            actor->kill();
        else
            _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F24E0);
    }
}
