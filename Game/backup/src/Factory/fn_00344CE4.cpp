namespace {
struct Vector3 { float x, y, z; };
struct Controller {
    void (**vtable)(Controller*);
    char padding04[5];
    bool finished;
    char padding0a[2];
    int frame;
};
struct Actor;
struct LiveActor {
    void (**vtable)(Actor*);
    char padding04[0x5c];
};
struct Actor : LiveActor {
    int field60;
    void* field64;
    const char* animation;
    Controller* controller;
    Vector3 vector70;
    int field7c;
    bool flag80;
    bool flag81;
    char padding82[6];
    Vector3 translation;
    bool isFlag80() const { return flag80; }
    bool isFlag81() const { return flag81; }
    const Vector3& getTranslation() const { return translation; }
    void updateAlpha();
    void updateVelocity();
    void updateTranslation();
};
struct Spine { Actor* actor; };
struct Nerve {};
}

extern "C" {
void fn_00279e8c(LiveActor*, float);
void fn_00214650(LiveActor*, const Vector3&, float);
void fn_0026C728(LiveActor*, const Vector3&, float);
void _ZN2al8setTransEPNS_9LiveActorERKN4sead7Vector3IfEE(LiveActor*, const Vector3&);
void fn_002791F8(LiveActor*, const char*);
void fn_00145048(bool);
void fn_00145100();
void _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(LiveActor*, const Nerve*);
void fn_00141098(void*, int*);
extern Nerve dat_003F2664;
}

void Actor::updateAlpha() {
    if (!flag80)
        fn_00279e8c(this, 0.5f);
}

void Actor::updateVelocity() {
    if (flag81)
        fn_00214650(this, vector70, 0.0f);
    else
        fn_0026C728(this, vector70, 0.0f);
}

void Actor::updateTranslation() {
    _ZN2al8setTransEPNS_9LiveActorERKN4sead7Vector3IfEE(this, translation);
}

extern "C" void fn_00344CE4(void*, const Spine* spine) {
    Actor* actor = spine->actor;
    actor->updateAlpha();
    actor->updateVelocity();
    if (actor->animation) {
        actor->updateTranslation();
        fn_002791F8(actor, actor->animation);
    }
    if (actor->controller) {
        fn_00145048(actor->controller->frame <= 90);
        actor->controller->vtable[0](actor->controller);
        if (actor->controller->finished) {
            fn_00145100();
            _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F2664);
            if (actor->field64)
                fn_00141098(actor->field64, &actor->field60);
            actor->vtable[5](actor);
        }
    }
}
