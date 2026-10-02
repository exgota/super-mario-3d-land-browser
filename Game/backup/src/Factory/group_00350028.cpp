namespace {
struct Actor;
struct Nerve;
}

extern "C" bool _ZN2al11isFirstStepEPKNS_9IUseNerveE(const Actor*);
extern "C" void _ZN2al11startActionEPNS_9LiveActorEPKc(Actor*, const char*);
extern "C" void fn_00258774(Actor*, unsigned int);
extern "C" bool _ZN2al11isActionEndEPKNS_9LiveActorE(const Actor*);
extern "C" void _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(Actor*, const Nerve*);
extern "C" const char dat_003BC7C0[];
extern "C" const char dat_003BC800[];
extern "C" const char dat_003BC82C[];
extern "C" const char dat_003BADC0[];
extern "C" const char dat_003BADD0[];
extern "C" const Nerve dat_003F2420;
extern "C" const Nerve dat_003F2450;
extern "C" const Nerve dat_003F1FF4;
extern "C" const Nerve dat_003F2010;
extern "C" const Nerve dat_003F1FF0;
extern "C" const Nerve dat_003F1FFC;
extern "C" const Nerve dat_003F1FF8;

#define BODY(NAME, ACTION, VALUE, NERVE) \
extern "C" void NAME(void*, Actor** holder) { \
    Actor* actor = *holder; \
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor)) \
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, ACTION); \
    fn_00258774(actor, VALUE); \
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor)) \
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &NERVE); \
}

BODY(fn_00350028, dat_003BC7C0, 0x0042FA14, dat_003F2420)
BODY(fn_00350564, dat_003BC800, 0x0042FA14, dat_003F2450)
BODY(fn_00350684, dat_003BC82C, 0x0042FA14, dat_003F2420)
BODY(fn_003585B0, dat_003BADC0, 0x0042F534, dat_003F1FF4)
BODY(fn_003586DC, dat_003BADC0, 0x0042F534, dat_003F2010)
BODY(fn_0035885C, dat_003BADC0, 0x0042F534, dat_003F1FF0)
BODY(fn_00358B18, dat_003BADD0, 0x0042F534, dat_003F1FFC)
BODY(fn_00358BF0, dat_003BADD0, 0x0042F534, dat_003F1FF8)

#undef BODY
