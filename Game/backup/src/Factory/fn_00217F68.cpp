namespace {
struct Actor;
struct Nerve;
}

extern "C" bool fn_00279ED4(const Actor*, int);
extern "C" bool _ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(const Actor*, const Nerve*);
extern "C" void fn_00279e8c(Actor*, float);
extern "C" void fn_00279e5c(Actor*, float);
extern "C" Nerve dat_003F1E5C;
extern "C" Nerve dat_003F1E80;
extern "C" Nerve dat_003F1E70;

extern "C" void fn_00217F68(Actor* actor) {
    float first;
    if (fn_00279ED4(actor, 0))
        first = 0.5f;
    else if (_ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F1E5C))
        first = 2.4f;
    else if (_ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F1E80))
        first = 6.0f;
    else if (_ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F1E70))
        first = 2.0f;
    else
        first = 6.4f;

    float second;
    if (fn_00279ED4(actor, 0))
        second = 0.4f;
    else if (_ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F1E80))
        second = 0.998f;
    else if (_ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F1E70))
        second = 0.998f;
    else
        second = 0.998f;

    fn_00279e8c(actor, first);
    fn_00279e5c(actor, second);
}
