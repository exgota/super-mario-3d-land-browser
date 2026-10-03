namespace {
struct LiveActor;

struct ActionController {
    void* unknown0;
    LiveActor* actor;
    LiveActor* movementActor;
    void* unknownC;
    const char* actionName;
};
}

extern "C" {
bool fn_00267934(const LiveActor*, const char*);
float fn_00330844(const LiveActor*, int);
const char* fn_00262580(const LiveActor*, int);
bool _ZN2al13isEqualStringEPKcS1_(const char*, const char*);
void _ZN2al11startActionEPNS_9LiveActorEPKc(LiveActor*, const char*);
void fn_00262538(LiveActor*, int);
void fn_0024AC6C(LiveActor*, int);
void fn_0026243C(LiveActor*, int);
void fn_0024393C(LiveActor*, int);
void fn_00190228(ActionController*);
extern const char* dat_003F1C84;
}

extern "C" void fn_001B9150(ActionController* self) {
    if ((fn_00267934(self->movementActor, "Move") ||
         fn_00267934(self->movementActor, "LuigiMove")) &&
        (fn_00330844(self->movementActor, 2) >= 0.8f ||
         fn_00330844(self->movementActor, 4) >= 0.1f)) {
        if (!_ZN2al13isEqualStringEPKcS1_(
                fn_00262580(self->actor, 0), dat_003F1C84)) {
            _ZN2al11startActionEPNS_9LiveActorEPKc(self->actor, dat_003F1C84);
        }
        fn_00262538(self->movementActor, 2);
        fn_0024AC6C(self->actor, 0);
        fn_0026243C(self->movementActor, 2);
        fn_0024393C(self->actor, 0);
        self->actionName = dat_003F1C84;
    } else {
        fn_00190228(self);
    }
}
