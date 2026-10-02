namespace {
struct Spine {
    void* actor;
};
}

extern "C" bool _ZN2al11isFirstStepEPKNS_9IUseNerveE(const void*);
extern "C" void fn_0027063c(void*, const char*);
extern "C" void _ZN2al11startActionEPNS_9LiveActorEPKc(void*, const char*);
extern "C" void _ZN2al16validateClippingEPNS_9LiveActorE(void*);
extern "C" void fn_0026FB1C(void*);

extern "C" char dat_003BD82C;
extern "C" char dat_003BE474;
extern "C" char dat_003BEC88;
extern "C" char dat_003BCE14;
extern "C" char dat_003BF164;
extern "C" char dat_003BD7A8;
extern "C" char dat_003BF134;

extern "C" void fn_0036157C(void*, const Spine* spine) {
    void* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor)) {
        fn_0027063c(actor, &dat_003BD82C);
        _ZN2al16validateClippingEPNS_9LiveActorE(actor);
    }
}

extern "C" void fn_0036623C(void*, const Spine* spine) {
    void* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor)) {
        fn_0027063c(actor, &dat_003BE474);
        _ZN2al16validateClippingEPNS_9LiveActorE(actor);
    }
}

extern "C" void fn_00366C98(void*, const Spine* spine) {
    void* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor)) {
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, &dat_003BEC88);
        _ZN2al16validateClippingEPNS_9LiveActorE(actor);
    }
}

extern "C" void fn_0036AF88(void*, const Spine* spine) {
    void* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor)) {
        fn_0027063c(actor, &dat_003BCE14);
        fn_0026FB1C(actor);
    }
}

extern "C" void fn_0036C1B4(void*, const Spine* spine) {
    void* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor)) {
        fn_0027063c(actor, &dat_003BF164);
        _ZN2al16validateClippingEPNS_9LiveActorE(actor);
    }
}

extern "C" void fn_0036C804(void*, const Spine* spine) {
    void* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor)) {
        fn_0027063c(actor, &dat_003BD7A8);
        _ZN2al16validateClippingEPNS_9LiveActorE(actor);
    }
}

extern "C" void fn_003707BC(void*, const Spine* spine) {
    void* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor)) {
        fn_0027063c(actor, &dat_003BF134);
        _ZN2al16validateClippingEPNS_9LiveActorE(actor);
    }
}
