namespace {
struct Actor;
struct Auxiliary;
struct Selection {
    virtual void slot0() = 0;
    virtual void slot1() = 0;
    virtual int getSelection() = 0;
};
struct Context {
    char padding[0x40];
    Actor* actor;
    Auxiliary* auxiliary;
};
struct Controller {
    char padding[0x0c];
    Context* context;
    Selection* selection;
    Auxiliary* auxiliary;
};
struct Spine {
    Controller* executor;
};
}

extern "C" {
bool _ZN2al11isFirstStepEPKNS_9IUseNerveE(const Controller*);
bool _ZN2al6isStepEPNS_9IUseNerveEi(Controller*, int);
void fn_002518C8(Actor*);
void fn_002556C4(Auxiliary*);
void fn_00272AE4(Actor*);
void fn_00258194(Actor*, const char*);
void fn_00188030(Auxiliary*);
void fn_00212208(Auxiliary*);
void fn_00273A28(const char*, int, int, bool);
Auxiliary* fn_0026AF18(Selection*);
void fn_0025DE7C(Auxiliary*, int);
void fn_00212224(Controller*);
extern const char dat_003B761C[];
extern const char dat_003B7604[];
}

extern "C" void fn_00372F94(void*, Spine* spine) {
    Controller* controller = spine->executor;
    Actor* actor = controller->context->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(controller)) {
        fn_002518C8(actor);
        fn_002556C4(controller->auxiliary);
    }
    if (_ZN2al6isStepEPNS_9IUseNerveEi(controller, 10)) {
        fn_00272AE4(actor);
        fn_00258194(actor, dat_003B761C);
        fn_00188030(controller->context->auxiliary);
    }
    if (_ZN2al6isStepEPNS_9IUseNerveEi(controller, 54)) {
        fn_00212208(controller->context->auxiliary);
    }
    fn_00273A28(dat_003B7604, 0, 10, true);
    if (_ZN2al6isStepEPNS_9IUseNerveEi(controller, 72)) {
        int selection = controller->selection->getSelection();
        Auxiliary* auxiliary = fn_0026AF18(controller->selection);
        fn_0025DE7C(auxiliary, selection);
        fn_00212224(controller);
    }
}
