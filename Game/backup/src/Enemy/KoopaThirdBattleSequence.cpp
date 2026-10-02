#include <Nerve/alNerveExecutor.h>
#include <Nerve/alNerveFunction.h>

// Inferred layout of the third Koopa battle sequence; the retail class name is unknown.
struct KoopaThirdBattleSequence {
    const void* vtable;
    void* nerveKeeper;
    bool dead;
    unsigned char padding[3];
    void* actor;
    bool initialized;
};

extern "C" KoopaThirdBattleSequence* fn_0025A678(KoopaThirdBattleSequence*, const char*, void*);
extern "C" const unsigned char dat_003CBF8C[];
extern "C" al::NerveStateBase* fn_0025A650(void*);
extern "C" al::NerveStateBase* fn_0025A628(void*);
extern "C" al::NerveStateBase* fn_0025A600(void*);
extern "C" al::NerveStateBase* fn_0025A4D0(void*);
extern "C" al::NerveStateBase* fn_0025A5CC(void*);
extern "C" al::NerveStateBase* fn_0025A598(void*);
extern "C" al::NerveStateBase* fn_00142600(void*);
extern "C" al::NerveStateBase* fn_0025A570(void*);
extern "C" al::NerveStateBase* fn_0025A548(void*);
extern "C" void fn_0025A4F8(void*);
extern "C" const al::Nerve dat_003F3338;
extern "C" const al::Nerve dat_003F333C;
extern "C" const al::Nerve dat_003F3340;
extern "C" const al::Nerve dat_003F3344;
extern "C" const al::Nerve dat_003F3348;
extern "C" const al::Nerve dat_003F3354;
extern "C" const al::Nerve dat_003F3350;
extern "C" const al::Nerve dat_003F334C;
extern "C" const al::Nerve dat_003F3358;
extern "C" const al::Nerve dat_003F335C;

extern "C" KoopaThirdBattleSequence* fn_0016F0D8(KoopaThirdBattleSequence* state, void* actor) {
    state = fn_0025A678(state, "\x83\x4e\x83\x62\x83\x70\x8e\x4f\x89\xf1\x90\xed\x96\xda\x83\x56\x81\x5b\x83\x50\x83\x93\x83\x58", actor);
    state->vtable = dat_003CBF8C;
    state->initialized = false;
    reinterpret_cast<al::NerveExecutor*>(state)->initNerve(&dat_003F3338, 9);
    al::initNerveState(reinterpret_cast<al::IUseNerve*>(state), fn_0025A650(state->actor), &dat_003F333C, "\x90\xed\x93\xac\x8a\x4a\x8e\x6e\x83\x66\x83\x82");
    al::initNerveState(reinterpret_cast<al::IUseNerve*>(state), fn_0025A628(state->actor), &dat_003F3340, "\x90\xed\x93\xac\x92\x86\x83\x66\x83\x82");
    al::initNerveState(reinterpret_cast<al::IUseNerve*>(state), fn_0025A600(state->actor), &dat_003F3344, "\x90\xed\x93\xac\x8f\x49\x97\xb9\x83\x66\x83\x82");
    al::initNerveState(reinterpret_cast<al::IUseNerve*>(state), fn_0025A4D0(state->actor), &dat_003F3348, "\x82\xb5\x82\xc1\x82\xdb\x8d\x55\x8c\x82");
    al::initNerveState(reinterpret_cast<al::IUseNerve*>(state), fn_0025A5CC(state->actor), &dat_003F3354, "\x89\x8a\x94\xad\x8e\xcb\x28\x89\x93\x8b\x97\x97\xa3\x29");
    al::initNerveState(reinterpret_cast<al::IUseNerve*>(state), fn_0025A598(state->actor), &dat_003F3350, "\x89\x8a\x94\xad\x8e\xcb\x28\x8b\xdf\x8b\x97\x97\xa3\x29");
    al::initNerveState(reinterpret_cast<al::IUseNerve*>(state), fn_00142600(state->actor), &dat_003F334C, "\x83\x71\x83\x62\x83\x76\x83\x68\x83\x8d\x83\x62\x83\x76");
    al::initNerveState(reinterpret_cast<al::IUseNerve*>(state), fn_0025A570(state->actor), &dat_003F3358, "\x83\x57\x83\x83\x83\x93\x83\x76");
    al::initNerveState(reinterpret_cast<al::IUseNerve*>(state), fn_0025A548(state->actor), &dat_003F335C, "\x83\x5e\x81\x5b\x83\x93");
    fn_0025A4F8(state->actor);
    return state;
}
