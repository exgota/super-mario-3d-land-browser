namespace {
struct Actor;
struct Nerve { const void* vtable; };
struct State {
    unsigned char unknown[12];
    Actor* actor;
    bool triggered;
    State* setTriggered(bool value) {
        triggered = value;
        return this;
    }
};

extern "C" bool fn_00267A60(Actor*);
extern "C" bool fn_00263A84(Actor*);
extern "C" bool fn_0025A36C(Actor*);
extern "C" bool fn_0025A324(Actor*);
extern "C" bool fn_0026400C(Actor*);
extern "C" void _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(State*, const Nerve*);
extern "C" Nerve dat_003F334C;
extern "C" Nerve dat_003F3340;
extern "C" Nerve dat_003F3348;
extern "C" Nerve dat_003F3350;
extern "C" Nerve dat_003F335C;
extern "C" Nerve dat_003F3354;
}

extern "C" void fn_0016EEA0(State* self) {
    if (fn_00267A60(self->actor) && !self->triggered) {
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(self->setTriggered(true), &dat_003F3340);
        return;
    }
    if (fn_00263A84(self->actor)) {
        if (fn_0025A36C(self->actor))
            _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(self, &dat_003F334C);
        else if (fn_0025A324(self->actor))
            _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(self, &dat_003F3348);
        else if (fn_0026400C(self->actor))
            _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(self, &dat_003F3350);
        else
            _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(self, &dat_003F335C);
    } else {
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(self, &dat_003F3354);
    }
}
