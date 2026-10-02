namespace {
struct LiveActor;
struct ActionSource;
struct ActionController {
    void* unknown;
    LiveActor* actor;
    ActionSource* source;
    const char* defaultAction;
    const char* currentAction;
};
}

extern "C" const char* fn_00262580(ActionSource*, int);
extern "C" bool fn_0026625C(LiveActor*, const char*);
extern "C" bool _ZN2al13isEqualStringEPKcS1_(const char*, const char*);
extern "C" void _ZN2al11startActionEPNS_9LiveActorEPKc(LiveActor*, const char*);
extern "C" void fn_00262538(ActionSource*, int);
extern "C" void fn_0024AC6C(LiveActor*, int);
extern "C" void fn_0026243C(ActionSource*, int);
extern "C" void fn_0024393C(LiveActor*, int);

extern "C" void fn_00190228(ActionController* self) {
    const char* action = fn_00262580(self->source, 0);
    if (!action) {
        if (self->defaultAction && self->currentAction != self->defaultAction)
            _ZN2al11startActionEPNS_9LiveActorEPKc(self->actor, self->defaultAction);
        self->currentAction = self->defaultAction;
    } else {
        if (!self->currentAction || !_ZN2al13isEqualStringEPKcS1_(action, self->currentAction)) {
            if (fn_0026625C(self->actor, action)) {
                _ZN2al11startActionEPNS_9LiveActorEPKc(self->actor, action);
                self->currentAction = action;
            } else {
                if (self->defaultAction && self->currentAction != self->defaultAction)
                    _ZN2al11startActionEPNS_9LiveActorEPKc(self->actor, self->defaultAction);
                self->currentAction = self->defaultAction;
            }
        }
    }
    if (self->currentAction != self->defaultAction) {
        fn_00262538(self->source, 0);
        fn_0024AC6C(self->actor, 0);
        fn_0026243C(self->source, 0);
        fn_0024393C(self->actor, 0);
    }
}
