namespace {
struct Actor;
struct Spine { Actor* actor; };
}

extern "C" bool _ZN2al11isFirstStepEPKNS_9IUseNerveE(const Actor*);
extern "C" bool _ZN2al16updateNerveStateEPNS_9IUseNerveE(Actor*);
extern "C" void _ZN2al16validateClippingEPNS_9LiveActorE(Actor*);
extern "C" void fn_0026B948(Actor*);
extern "C" void fn_001C96B8(Actor*);
extern "C" void fn_0024ED7C(Actor*);
extern "C" void fn_00213658(Actor*);
extern "C" void fn_0016EEA0(Actor*);

extern "C" void fn_0034D530(void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al16validateClippingEPNS_9LiveActorE(actor);
}

extern "C" void fn_0034DB98(void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        fn_0026B948(actor);
}

extern "C" void fn_0034E0E8(void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        fn_001C96B8(actor);
}

extern "C" void fn_0034E110(void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al16validateClippingEPNS_9LiveActorE(actor);
}

extern "C" void fn_003565A0(void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        fn_0024ED7C(actor);
}

extern "C" void fn_00356698(void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        fn_0024ED7C(actor);
}

extern "C" void fn_0035C41C(void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al16updateNerveStateEPNS_9IUseNerveE(actor))
        fn_00213658(actor);
}

extern "C" void fn_0035C490(void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al16updateNerveStateEPNS_9IUseNerveE(actor))
        fn_00213658(actor);
}

extern "C" void fn_00362F74(void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al16updateNerveStateEPNS_9IUseNerveE(actor))
        fn_0016EEA0(actor);
}

extern "C" void fn_003630B0(void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al16updateNerveStateEPNS_9IUseNerveE(actor))
        fn_0016EEA0(actor);
}

extern "C" void fn_0036ACBC(void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al16validateClippingEPNS_9LiveActorE(actor);
}

extern "C" void fn_0036C2B4(void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al16validateClippingEPNS_9LiveActorE(actor);
}
