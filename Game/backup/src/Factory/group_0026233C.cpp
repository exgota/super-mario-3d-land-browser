namespace {
struct Nerve {};

struct Actor {
    virtual void slot0() = 0;
    virtual void slot1() = 0;
    virtual void slot2() = 0;
    virtual void slot3() = 0;
    virtual void makeActorAppeared() = 0;
};
}

extern "C" {
bool _ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(const Actor*, const Nerve*);
void _ZN2al14onDrawClippingEPNS_9LiveActorE(Actor*);
void _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(Actor*, const Nerve*);
extern Nerve dat_003F253C;
extern Nerve dat_003F2540;
extern Nerve dat_003F2544;
extern Nerve dat_003F2548;
}

extern "C" void fn_0026233C(Actor* actor)
{
    if (_ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F2540) ||
        _ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F2544)) {
        _ZN2al14onDrawClippingEPNS_9LiveActorE(actor);
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F2548);
        actor->makeActorAppeared();
    }
}

extern "C" void fn_002623BC(Actor* actor)
{
    if (_ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F2548) ||
        _ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F253C)) {
        _ZN2al14onDrawClippingEPNS_9LiveActorE(actor);
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F2540);
        actor->makeActorAppeared();
    }
}
