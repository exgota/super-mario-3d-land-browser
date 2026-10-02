namespace {
struct Actor {};
struct Spine { Actor* actor; };
}

extern "C" bool _ZN2al11isActionEndEPKNS_9LiveActorE(Actor*);
extern "C" bool _ZN2al11isFirstStepEPKNS_9IUseNerveE(Actor*);
extern "C" void _ZN2al11startActionEPNS_9LiveActorEPKc(Actor*, const char*);
extern "C" void _ZN2al16startNerveActionEPNS_9LiveActorEPKc(Actor*, const char*);
extern "C" bool _ZN2al16updateNerveStateEPNS_9IUseNerveE(Actor*);
extern "C" void _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(Actor*, const char*);
extern "C" bool fn_001C2EE8(Actor*);
extern "C" void fn_00214224(Actor*, const char*);
extern "C" void fn_0027063c(Actor*, const char*);
extern "C" const char dat_003B6E38[];
extern "C" const char dat_003B6FBC[];
extern "C" const char dat_003B71AC[];
extern "C" const char dat_003B8B20[];
extern "C" const char dat_003BBFEC[];
extern "C" const char dat_003BC30C[];
extern "C" const char dat_003BC31C[];
extern "C" const char dat_003BD19C[];
extern "C" const char dat_003BD3F0[];
extern "C" const char dat_003BD3FC[];
extern "C" const char dat_003BDD6C[];
extern "C" const char dat_003BDD78[];
extern "C" const char dat_003BDF18[];
extern "C" const char dat_003BE084[];
extern "C" const char dat_003BE194[];
extern "C" const char dat_003BE1A0[];
extern "C" const char dat_003BE2C4[];
extern "C" const char dat_003BE2D0[];
extern "C" const char dat_003BE2D8[];
extern "C" const char dat_003BE30C[];
extern "C" const char dat_003BE318[];
extern "C" const char dat_003BE900[];
extern "C" const char dat_003BE98C[];
extern "C" const char dat_003BEC90[];
extern "C" const char dat_003BF04C[];
extern "C" const char dat_003BF058[];
extern "C" const char dat_003BF0B0[];
extern "C" const char dat_003C0048[];
extern "C" const char dat_003C0054[];
extern "C" const char dat_003C059C[];
extern "C" const char dat_003C0634[];
extern "C" const char dat_003F1608[];
extern "C" const char dat_003F163C[];
extern "C" const char dat_003F169C[];
extern "C" const char dat_003F1AEC[];
extern "C" const char dat_003F22C8[];
extern "C" const char dat_003F234C[];
extern "C" const char dat_003F2354[];
extern "C" const char dat_003F25E4[];
extern "C" const char dat_003F281C[];
extern "C" const char dat_003F2848[];
extern "C" const char dat_003F2A00[];
extern "C" const char dat_003F2A08[];
extern "C" const char dat_003F2A7C[];
extern "C" const char dat_003F2B34[];
extern "C" const char dat_003F2DBC[];
extern "C" const char dat_003F2F64[];
extern "C" const char dat_003F2F7C[];

extern "C" void fn_00352E20(const void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003B71AC);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, dat_003F169C);
}

extern "C" void fn_00353244(const void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003BF0B0);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, dat_003F2B34);
}

extern "C" void fn_003566C0(const void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        fn_00214224(actor, dat_003B6E38);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, dat_003F1608);
}

extern "C" void fn_00356808(const void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        fn_00214224(actor, dat_003B6FBC);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, dat_003F163C);
}

extern "C" void fn_00356F34(const void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003BE900);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, dat_003F2A00);
}

extern "C" void fn_003571C8(const void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003BE98C);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, dat_003F2A08);
}

extern "C" void fn_00359360(const void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003BE084);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, dat_003F2848);
}

extern "C" void fn_00359B74(const void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003BBFEC);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, dat_003F22C8);
}

extern "C" void fn_0035A1D4(const void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003BD19C);
    if (_ZN2al16updateNerveStateEPNS_9IUseNerveE(actor))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, dat_003F25E4);
}

extern "C" void fn_0035A74C(const void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        fn_0027063c(actor, dat_003BD3F0);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al16startNerveActionEPNS_9LiveActorEPKc(actor, dat_003BD3FC);
}

extern "C" void fn_0035B6F4(const void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003C0054);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, dat_003F2DBC);
}

extern "C" void fn_0035B748(const void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003C0048);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, dat_003F2DBC);
}

extern "C" void fn_0035E090(const void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003C059C);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, dat_003F2F64);
}

extern "C" void fn_0035E114(const void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003C0634);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, dat_003F2F7C);
}

extern "C" void fn_0035F5FC(const void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        fn_0027063c(actor, dat_003BE2C4);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al16startNerveActionEPNS_9LiveActorEPKc(actor, dat_003BE2D0);
}

extern "C" void fn_0035F664(const void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003BE2D8);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al16startNerveActionEPNS_9LiveActorEPKc(actor, dat_003BE2D0);
}

extern "C" void fn_0035F6CC(const void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003BE30C);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        fn_0027063c(actor, dat_003BE318);
}

extern "C" void fn_00361928(const void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003B8B20);
    if (fn_001C2EE8(actor))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, dat_003F1AEC);
}

extern "C" void fn_00362394(const void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        fn_0027063c(actor, dat_003BDD6C);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al16startNerveActionEPNS_9LiveActorEPKc(actor, dat_003BDD78);
}

extern "C" void fn_00364444(const void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003BC31C);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, dat_003F234C);
}

extern "C" void fn_00364498(const void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003BC30C);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, dat_003F2354);
}

extern "C" void fn_00364700(const void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        fn_0027063c(actor, dat_003BF04C);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al16startNerveActionEPNS_9LiveActorEPKc(actor, dat_003BF058);
}

extern "C" void fn_00366C44(const void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003BEC90);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, dat_003F2A7C);
}

extern "C" void fn_0036A564(const void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        fn_0027063c(actor, dat_003BE194);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al16startNerveActionEPNS_9LiveActorEPKc(actor, dat_003BE1A0);
}

extern "C" void fn_0036CCF8(const void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003BDF18);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, dat_003F281C);
}
