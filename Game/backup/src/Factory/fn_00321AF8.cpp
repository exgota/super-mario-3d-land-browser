namespace {
struct HitSensor;
struct Nerve;
struct AttackState;
struct Actor {
    char padding[0x64];
    AttackState* state;
    Nerve* response;
};
}

extern "C" {
extern Nerve dat_003F2370;
extern Nerve dat_003F2374;
bool fn_003227E0(AttackState*);
void fn_003227F4(AttackState*);
void fn_00322634(AttackState*, unsigned int);
bool _ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(const Actor*, const Nerve*);
bool _ZN2al12isSensorNameEPNS_9HitSensorEPKc(HitSensor*, const char*);
bool fn_0027B768(unsigned int, HitSensor*, HitSensor*, Nerve*, const Nerve*);
bool _ZN2al27isMsgPlayerInvincibleAttackEj(unsigned int);
bool _ZN2al6isMsg9Ej(unsigned int);
}

extern "C" bool fn_00321AF8(Actor* actor, unsigned int message, HitSensor* sender, HitSensor* receiver) {
    if (!fn_003227E0(actor->state))
        return false;
    if (_ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F2370) ||
        _ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F2374))
        return false;
    if (!_ZN2al12isSensorNameEPNS_9HitSensorEPKc(receiver, "Body"))
        return false;
    if (!fn_0027B768(message, sender, receiver, actor->response, &dat_003F2370))
        return false;
    if (actor->state) {
        fn_003227F4(actor->state);
        if (_ZN2al27isMsgPlayerInvincibleAttackEj(message) || _ZN2al6isMsg9Ej(message))
            fn_00322634(actor->state, message);
    }
    return true;
}
