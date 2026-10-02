namespace {
struct StateActor {
    unsigned char padding[0x6c];
    int cachedState;
};
struct Nerve {};
}

extern "C" int fn_0025AB80(StateActor*);
extern "C" int fn_0025AB3C(StateActor*, int);
extern "C" void _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(StateActor*, const Nerve*);
extern "C" Nerve dat_003F3214;
extern "C" Nerve dat_003F3218;
extern "C" Nerve dat_003F3220;
extern "C" Nerve dat_003F3224;
extern "C" Nerve dat_003F3228;
extern "C" Nerve dat_003F322C;

extern "C" bool fn_0016DABC(StateActor* actor) {
    int state = fn_0025AB80(actor);
    if (actor->cachedState == state)
        return false;
    actor->cachedState = state;
    int index = fn_0025AB3C(actor, actor->cachedState);
    bool changed = true;
    if (index == 0) {
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F3214);
        return changed;
    }
    if (index == 1) {
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F3218);
        return changed;
    }
    if (index == 2) {
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F3220);
        return changed;
    }
    if (index == 3) {
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F3224);
        return changed;
    }
    if (index == 4) {
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F3228);
        return changed;
    }
    if (index == 5) {
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F322C);
        return changed;
    }
    return false;
}
