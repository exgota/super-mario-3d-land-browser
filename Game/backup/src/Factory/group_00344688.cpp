namespace {
struct Actor {};
struct Nerve {};
struct Spine { Actor* actor; };
}

extern "C" bool _ZN2al11isFirstStepEPKNS_9IUseNerveE(const Actor*);
extern "C" void _ZN2al11startActionEPNS_9LiveActorEPKc(Actor*, const char*);
extern "C" bool _ZN2al11isActionEndEPKNS_9LiveActorE(const Actor*);
extern "C" void _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(Actor*, const Nerve*);

extern "C" const char dat_003BC190[];
extern "C" const char dat_003BB478[];
extern "C" const char dat_003BB440[];
extern "C" const char dat_003BD8F0[];
extern "C" const char dat_003B9430[];
extern "C" const char dat_003BEDDC[];
extern "C" const char dat_003BD2CC[];
extern "C" const char dat_003BD2BC[];
extern "C" const char dat_003BABE8[];
extern "C" const char dat_003BB324[];
extern "C" const char dat_003BB32C[];
extern "C" const char dat_003BB6A0[];
extern "C" const char dat_003BC9EC[];
extern "C" const char dat_003BCB88[];
extern "C" const char dat_003BCB90[];
extern "C" const char dat_003BBDD4[];
extern "C" const char dat_003BDB44[];
extern "C" const char dat_003BECF0[];
extern "C" const char dat_003BC098[];
extern "C" const char dat_003BF6B0[];
extern "C" const char dat_003BF688[];
extern "C" const char dat_003BACA8[];
extern "C" const char dat_003BAC94[];
extern "C" const char dat_003BBB60[];
extern "C" const char dat_003BEA50[];
extern "C" const Nerve dat_003F2308;
extern "C" const Nerve dat_003F20E4;
extern "C" const Nerve dat_003F2708;
extern "C" const Nerve dat_003F1C68;
extern "C" const Nerve dat_003F2AB4;
extern "C" const Nerve dat_003F2614;
extern "C" const Nerve dat_003F261C;
extern "C" const Nerve dat_003F1F90;
extern "C" const Nerve dat_003F20BC;
extern "C" const Nerve dat_003F20C0;
extern "C" const Nerve dat_003F2148;
extern "C" const Nerve dat_003F2488;
extern "C" const Nerve dat_003F24C8;
extern "C" const Nerve dat_003F24CC;
extern "C" const Nerve dat_003F2258;
extern "C" const Nerve dat_003F2788;
extern "C" const Nerve dat_003F2A8C;
extern "C" const Nerve dat_003F22E0;
extern "C" const Nerve dat_003F2C3C;
extern "C" const Nerve dat_003F1FB8;
extern "C" const Nerve dat_003F1FB0;
extern "C" const Nerve dat_003F21F8;
extern "C" const Nerve dat_003F2A2C;

extern "C" void fn_00344688(void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003BC190);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F2308);
}

extern "C" void fn_0034507C(void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003BB478);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F20E4);
}

extern "C" void fn_00345220(void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003BB440);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F20E4);
}

extern "C" void fn_00345544(void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003BD8F0);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F2708);
}

extern "C" void fn_00346A8C(void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003B9430);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F1C68);
}

extern "C" void fn_00346FEC(void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003BEDDC);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F2AB4);
}

extern "C" void fn_00347508(void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003BD2CC);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F2614);
}

extern "C" void fn_003475B8(void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003BD2BC);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F261C);
}

extern "C" void fn_00347918(void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003BABE8);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F1F90);
}

extern "C" void fn_00347B24(void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003BB324);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F20BC);
}

extern "C" void fn_00347EFC(void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003BB32C);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F20C0);
}

extern "C" void fn_00347FDC(void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003BB6A0);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F2148);
}

extern "C" void fn_00349E68(void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003BC9EC);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F2488);
}

extern "C" void fn_0034A4EC(void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003BCB88);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F24C8);
}

extern "C" void fn_0034A540(void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003BCB90);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F24CC);
}

extern "C" void fn_0034BFB8(void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003BBDD4);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F2258);
}

extern "C" void fn_0034CE80(void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003BDB44);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F2788);
}

extern "C" void fn_0034D4DC(void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003BECF0);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F2A8C);
}

extern "C" void fn_0034EE24(void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003BC098);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F22E0);
}

extern "C" void fn_0034FB2C(void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003BF6B0);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F2C3C);
}

extern "C" void fn_0034FB80(void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003BF688);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F2C3C);
}

extern "C" void fn_00350EDC(void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003BACA8);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F1FB8);
}

extern "C" void fn_00350F88(void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003BAC94);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F1FB0);
}

extern "C" void fn_00351BD8(void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003BBB60);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F21F8);
}

extern "C" void fn_0035238C(void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003BEA50);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F2A2C);
}
