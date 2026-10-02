namespace {
struct Context {
    void* actor;
};

extern "C" {
bool _ZN2al11isFirstStepEPKNS_9IUseNerveE(const void*);
bool _ZN2al16updateNerveStateEPNS_9IUseNerveE(void*);
bool _ZN2al11isActionEndEPKNS_9LiveActorE(const void*);
bool fn_00333BF0(void*);
void _ZN2al11startActionEPNS_9LiveActorEPKc(void*, const char*);
void _ZN2al16startNerveActionEPNS_9LiveActorEPKc(void*, const char*);
void _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(void*, const void*);
void fn_00214224(void*, const char*);
void fn_0027063c(void*, const char*);

extern char dat_003BE984;
extern char dat_003BDBE4;
extern char dat_003B6F4C;
extern char dat_003B7144;
extern char dat_003B6A60;
extern char dat_003BF7B8;
extern char dat_003C0594;
extern char dat_003C062C;
extern char dat_003B6C28;
extern char dat_003BDC48;
extern char dat_003BDC5C;
extern char dat_003EF540;
extern char dat_003C1250;
extern char dat_003B8B10;
extern char dat_003B8B18;
extern char dat_003B7414;
extern char dat_003C10F8;
extern char dat_003BE47C;
extern char dat_003BE1FC;
extern char dat_003BE1B4;
extern char dat_003BE1BC;
extern char dat_003F1348;
extern char dat_003E2E08;
}
}

#define DEFINE_BODY(name, check, action, data) \
extern "C" void name(void*, Context* context) { \
    void* actor = context->actor; \
    if (check(actor)) \
        action(actor, &data); \
}

DEFINE_BODY(fn_00357160, _ZN2al11isFirstStepEPKNS_9IUseNerveE, _ZN2al11startActionEPNS_9LiveActorEPKc, dat_003BE984)
DEFINE_BODY(fn_00358EA0, _ZN2al11isFirstStepEPKNS_9IUseNerveE, _ZN2al11startActionEPNS_9LiveActorEPKc, dat_003BDBE4)
DEFINE_BODY(fn_0035963C, _ZN2al11isFirstStepEPKNS_9IUseNerveE, fn_00214224, dat_003B6F4C)
DEFINE_BODY(fn_00359CD8, _ZN2al11isFirstStepEPKNS_9IUseNerveE, _ZN2al11startActionEPNS_9LiveActorEPKc, dat_003B7144)
DEFINE_BODY(fn_0035B374, _ZN2al11isFirstStepEPKNS_9IUseNerveE, _ZN2al11startActionEPNS_9LiveActorEPKc, dat_003B6A60)
DEFINE_BODY(fn_0035B9A4, _ZN2al11isFirstStepEPKNS_9IUseNerveE, _ZN2al11startActionEPNS_9LiveActorEPKc, dat_003BF7B8)
DEFINE_BODY(fn_0035E060, _ZN2al11isFirstStepEPKNS_9IUseNerveE, _ZN2al11startActionEPNS_9LiveActorEPKc, dat_003C0594)
DEFINE_BODY(fn_0035E0E4, _ZN2al11isFirstStepEPKNS_9IUseNerveE, _ZN2al11startActionEPNS_9LiveActorEPKc, dat_003C062C)
DEFINE_BODY(fn_0035E170, _ZN2al11isFirstStepEPKNS_9IUseNerveE, fn_00214224, dat_003B6C28)
DEFINE_BODY(fn_0035EF90, _ZN2al11isFirstStepEPKNS_9IUseNerveE, _ZN2al11startActionEPNS_9LiveActorEPKc, dat_003BDC48)
DEFINE_BODY(fn_0035F020, _ZN2al11isFirstStepEPKNS_9IUseNerveE, _ZN2al11startActionEPNS_9LiveActorEPKc, dat_003BDC5C)
DEFINE_BODY(fn_003604B4, _ZN2al16updateNerveStateEPNS_9IUseNerveE, _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE, dat_003EF540)
DEFINE_BODY(fn_00361018, _ZN2al11isFirstStepEPKNS_9IUseNerveE, fn_0027063c, dat_003C1250)
DEFINE_BODY(fn_003618C8, _ZN2al11isFirstStepEPKNS_9IUseNerveE, _ZN2al11startActionEPNS_9LiveActorEPKc, dat_003B8B10)
DEFINE_BODY(fn_003618F8, _ZN2al11isFirstStepEPKNS_9IUseNerveE, _ZN2al11startActionEPNS_9LiveActorEPKc, dat_003B8B18)
DEFINE_BODY(fn_003647FC, _ZN2al11isFirstStepEPKNS_9IUseNerveE, fn_00214224, dat_003B7414)
DEFINE_BODY(fn_00365C04, _ZN2al11isFirstStepEPKNS_9IUseNerveE, _ZN2al11startActionEPNS_9LiveActorEPKc, dat_003C10F8)
DEFINE_BODY(fn_00366174, _ZN2al11isFirstStepEPKNS_9IUseNerveE, fn_0027063c, dat_003BE47C)
DEFINE_BODY(fn_0036A3DC, _ZN2al11isFirstStepEPKNS_9IUseNerveE, fn_0027063c, dat_003BE1FC)
DEFINE_BODY(fn_0036A5CC, _ZN2al11isActionEndEPKNS_9LiveActorE, _ZN2al16startNerveActionEPNS_9LiveActorEPKc, dat_003BE1B4)
DEFINE_BODY(fn_0036A610, _ZN2al11isActionEndEPKNS_9LiveActorE, _ZN2al16startNerveActionEPNS_9LiveActorEPKc, dat_003BE1BC)
DEFINE_BODY(fn_00372638, fn_00333BF0, _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE, dat_003F1348)
DEFINE_BODY(fn_00373714, fn_00333BF0, _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE, dat_003E2E08)

#undef DEFINE_BODY
