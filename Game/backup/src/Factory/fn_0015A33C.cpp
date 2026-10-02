namespace {
struct Actor {
    char padding[0x50];
    void* child;
};
}

extern "C" void _ZN2al11LayoutActor6appearEv(Actor*);
extern "C" void _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(Actor*, const void*);
extern "C" void fn_0027E73C(void*);
extern "C" void fn_00159fec(Actor*);
extern "C" char dat_003F1844;

extern "C" void fn_0015A33C(Actor* actor) {
    _ZN2al11LayoutActor6appearEv(actor);
    _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F1844);
    fn_0027E73C(actor->child);
    fn_00159fec(actor);
}
