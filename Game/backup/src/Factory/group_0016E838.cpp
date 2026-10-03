namespace {
struct IUseNerve {};
struct LiveActor : IUseNerve {};
struct Nerve {};
}

extern "C" void _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(IUseNerve*, const Nerve*);
extern "C" void fn_0025A834(LiveActor*);
extern "C" void _ZN2al16validateClippingEPNS_9LiveActorE(LiveActor*);
extern "C" const Nerve dat_003F32F4;
extern "C" const Nerve dat_003F3314;
extern "C" const Nerve dat_003F3338;
extern "C" const Nerve dat_003F2D9C;

extern "C" void fn_0016E838(LiveActor* actor) {
    _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F32F4);
    fn_0025A834(actor);
}

extern "C" void fn_0016EB48(LiveActor* actor) {
    _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F3314);
    fn_0025A834(actor);
}

extern "C" void fn_0016EF94(LiveActor* actor) {
    _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F3338);
    fn_0025A834(actor);
}

extern "C" void fn_002F87B4(LiveActor* actor) {
    _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F2D9C);
    _ZN2al16validateClippingEPNS_9LiveActorE(actor);
}
