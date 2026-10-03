namespace {
struct LayoutActor;
struct LiveActor;
struct IUseNerve;
struct Nerve;
}

extern "C" void _ZN2al11LayoutActor6appearEv(LayoutActor*);
extern "C" void _ZN2al9LiveActor6appearEv(LiveActor*);
extern "C" void _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(IUseNerve*, const Nerve*);
extern "C" const Nerve dat_003F1A5C;
extern "C" const Nerve dat_003F0354; // WipeSimpleNrvWait
extern "C" const Nerve dat_003F2E94;

extern "C" void fn_001BEFB4(LayoutActor* actor) {
    _ZN2al11LayoutActor6appearEv(actor);
    _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(
        reinterpret_cast<IUseNerve*>(actor), &dat_003F1A5C);
}

extern "C" void fn_002556C4(LayoutActor* actor) {
    _ZN2al11LayoutActor6appearEv(actor);
    _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(
        reinterpret_cast<IUseNerve*>(actor), &dat_003F0354);
}

extern "C" void fn_0031F310(LiveActor* actor) {
    _ZN2al9LiveActor6appearEv(actor);
    _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(
        reinterpret_cast<IUseNerve*>(actor), &dat_003F2E94);
}
