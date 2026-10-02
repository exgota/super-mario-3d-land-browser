namespace {
struct Rail;
struct Vector3 { float x, y, z; };
struct Quaternion { float x, y, z, w; };
struct Actor {
    char base[0x60];
    Rail* rail;
    char unknown64[12];
    float speed;
    int duration;
    int unknown78;
    int currentDuration;
};
}

extern "C" {
bool _ZN2al11isFirstStepEPKNS_9IUseNerveE(const Actor*);
void fn_0027063c(Actor*, const char*);
float fn_0027F4D4(const Rail*);
float fn_0027F488(const Rail*);
float fn_0027F43C(const Rail*);
float fn_0027F3D0(const Actor*, int);
Vector3* _ZN2al11getTransPtrEPNS_9LiveActorE(Actor*);
void fn_0026974C(Vector3*, const Rail*, float);
Quaternion* _ZN2al10getQuatPtrEPNS_9LiveActorE(Actor*);
void fn_002695C4(Quaternion*, const Rail*, float);
bool _ZN2al18isGreaterEqualStepEPKNS_9IUseNerveEi(const Actor*, int);
void fn_001C5BD4(Rail*);
void _ZN2al16startNerveActionEPNS_9LiveActorEPKc(Actor*, const char*);
extern const char dat_003BDD7C[];
extern const char dat_003BDD84[];
extern const char dat_003BDD94[];
extern const char dat_003BDDA0[];
}

extern "C" void fn_0016C1B8(Actor* actor) {
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor)) {
        fn_0027063c(actor, dat_003BDD7C);
        int duration = static_cast<int>(fn_0027F4D4(actor->rail));
        if (duration > 0)
            actor->duration = duration;
        float speed = fn_0027F488(actor->rail);
        if (speed > 0.0f)
            actor->speed = speed;
        if (actor->duration > 0) {
            actor->currentDuration = actor->duration;
        } else if (actor->speed > 0.0f) {
            actor->currentDuration = static_cast<int>(fn_0027F43C(actor->rail) / actor->speed);
            if (actor->currentDuration <= 0)
                actor->currentDuration = 1;
        }
    }
    float fraction = fn_0027F3D0(actor, actor->currentDuration);
    fn_0026974C(_ZN2al11getTransPtrEPNS_9LiveActorE(actor), actor->rail, fraction);
    fn_002695C4(_ZN2al10getQuatPtrEPNS_9LiveActorE(actor), actor->rail, fraction);
    if (_ZN2al18isGreaterEqualStepEPKNS_9IUseNerveEi(actor, actor->currentDuration)) {
        fn_001C5BD4(actor->rail);
        _ZN2al16startNerveActionEPNS_9LiveActorEPKc(actor, dat_003BDD84);
    }
}

extern "C" void fn_0016C34C(Actor* actor) {
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor)) {
        fn_0027063c(actor, dat_003BDD94);
        int duration = static_cast<int>(fn_0027F4D4(actor->rail));
        if (duration > 0)
            actor->duration = duration;
        float speed = fn_0027F488(actor->rail);
        if (speed > 0.0f)
            actor->speed = speed;
        if (actor->duration > 0) {
            actor->currentDuration = actor->duration;
        } else if (actor->speed > 0.0f) {
            actor->currentDuration = static_cast<int>(fn_0027F43C(actor->rail) / actor->speed);
            if (actor->currentDuration <= 0)
                actor->currentDuration = 1;
        }
    }
    float fraction = fn_0027F3D0(actor, actor->currentDuration);
    fn_0026974C(_ZN2al11getTransPtrEPNS_9LiveActorE(actor), actor->rail, fraction);
    fn_002695C4(_ZN2al10getQuatPtrEPNS_9LiveActorE(actor), actor->rail, fraction);
    if (_ZN2al18isGreaterEqualStepEPKNS_9IUseNerveEi(actor, actor->currentDuration)) {
        fn_001C5BD4(actor->rail);
        _ZN2al16startNerveActionEPNS_9LiveActorEPKc(actor, dat_003BDDA0);
    }
}
