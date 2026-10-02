namespace {
struct IUseNerve {
    virtual void nerveSlot() = 0;
};

struct ActorBase {
    virtual void slot0() = 0;
    virtual void slot1() = 0;
    virtual void slot2() = 0;
    virtual void appear() = 0;
    unsigned int unknown4;
    unsigned int unknown8;
};

struct LiveActor : ActorBase, IUseNerve {};

struct Controller {
    virtual void slot0() = 0;
    virtual void slot1() = 0;
    virtual void slot2() = 0;
    virtual void slot3() = 0;
    virtual void slot4() = 0;
    virtual void finish() = 0;
    unsigned int unknown4;
    unsigned int unknown8;
    LiveActor* actor;
    unsigned int handle;
    LiveActor* optionalActor;
    LiveActor* secondaryActor;
};
}

extern "C" {
bool _ZN2al11isFirstStepEPKNS_9IUseNerveE(const Controller*);
bool _ZN2al11isActionEndEPKNS_9LiveActorE(const LiveActor*);
void _ZN2al11startActionEPNS_9LiveActorEPKc(LiveActor*, const char*);
void fn_0026A9B8(LiveActor*);
void fn_0026a9fc(LiveActor*);
void fn_00272918(LiveActor*, const char*);
void fn_002728C0(LiveActor*, const char*);
LiveActor* fn_0024CC94(LiveActor*, const char*);
bool fn_00279B64(const IUseNerve*);
void fn_00256BE8(IUseNerve*);
void fn_00273848(unsigned int*);
extern const char dat_003C04E8[];
extern const char dat_003C04F0[];
extern const char dat_003C04DC[];
}

extern "C" void fn_001A0350(Controller* self) {
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(self)) {
        if (self->optionalActor)
            fn_0026A9B8(self->optionalActor);
        fn_0026a9fc(self->actor);
        fn_00272918(self->actor, dat_003C04E8);
        fn_002728C0(self->actor, dat_003C04F0);
        self->secondaryActor->appear();
        _ZN2al11startActionEPNS_9LiveActorEPKc(self->actor, dat_003C04DC);
        _ZN2al11startActionEPNS_9LiveActorEPKc(self->secondaryActor, dat_003C04DC);
        _ZN2al11startActionEPNS_9LiveActorEPKc(
            fn_0024CC94(self->secondaryActor, dat_003C04E8), dat_003C04DC);
    }
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(self->actor)) {
        if (fn_00279B64(self->actor))
            fn_00256BE8(self->actor);
        fn_00273848(&self->handle);
        fn_002728C0(self->actor, dat_003C04E8);
        self->finish();
    }
}
