namespace {
struct IUseNerve {};
struct LiveActor : IUseNerve {};
struct Nerve {};
}

extern "C" bool _ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(const IUseNerve*, const Nerve*);
extern "C" void _ZN2al18invalidateClippingEPNS_9LiveActorE(LiveActor*);
extern "C" void _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(IUseNerve*, const Nerve*);
extern "C" Nerve dat_003F2CCC;
extern "C" Nerve dat_003F2CD0;
extern "C" Nerve dat_003F2CD4;

extern "C" void fn_001A1420(LiveActor* actor)
{
    if (_ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F2CCC) ||
        _ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F2CD0)) {
        _ZN2al18invalidateClippingEPNS_9LiveActorE(actor);
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F2CD4);
    }
}
