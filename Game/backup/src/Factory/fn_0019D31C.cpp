namespace {
struct LiveActor;
struct Nerve;
struct Parameters {
    const int* firstDuration;
    void* unknown04;
    const float* speed;
    const int* secondDuration;
};
struct ActorParameters {
    LiveActor* actor;
    Parameters* parameters;
};
struct Controller {
    virtual void slot0() = 0;
    virtual void slot1() = 0;
    virtual void slot2() = 0;
    virtual void slot3() = 0;
    virtual void slot4() = 0;
    virtual void finish() = 0;
    void* unknown04;
    void* unknown08;
    ActorParameters bindings;
};
}

extern "C" {
bool _ZN2al11isFirstStepEPKNS_9IUseNerveE(const Controller*);
void _ZN2al11startActionEPNS_9LiveActorEPKc(LiveActor*, const char*);
int fn_0027A5C8();
void fn_00273E5C(LiveActor*, int, float);
bool _ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(const Controller*, const Nerve*);
bool _ZN2al18isGreaterEqualStepEPKNS_9IUseNerveEi(const Controller*, int);
void _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(Controller*, const Nerve*);
extern const char dat_003C08D8[];
extern const Nerve dat_003F3004;
extern const Nerve dat_003F2FF8;
extern const Nerve dat_003F3008;

void fn_0019D31C(Controller* self) {
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(self))
        _ZN2al11startActionEPNS_9LiveActorEPKc(self->bindings.actor, dat_003C08D8);
    int value = fn_0027A5C8();
    ActorParameters bindings = self->bindings;
    fn_00273E5C(bindings.actor, value, *bindings.parameters->speed);
    if (_ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(self, &dat_003F3004)) {
        if (_ZN2al18isGreaterEqualStepEPKNS_9IUseNerveEi(self, *self->bindings.parameters->firstDuration))
            _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(self, &dat_003F2FF8);
    } else if (_ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(self, &dat_003F3008)) {
        if (_ZN2al18isGreaterEqualStepEPKNS_9IUseNerveEi(self, *self->bindings.parameters->secondDuration))
            self->finish();
    }
}
}
