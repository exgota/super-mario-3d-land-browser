namespace {
struct Actor {
    unsigned char padding[0x8a];
    unsigned char flag;
    unsigned char padding2[5];
    int mode;
};
struct Spine { Actor* actor; };
struct Nerve {};
}

extern "C" {
bool fn_00279ED4(Actor*, int);
void fn_00279e8c(Actor*, float);
void fn_00257D10(Actor*, float, float);
bool _ZN2al11isFirstStepEPKNS_9IUseNerveE(const Actor*);
void _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(Actor*, const Nerve*);
void fn_0027285C(Actor*);
void fn_002728C0(Actor*, const char*);
void fn_00272B60(Actor*, int);
extern Nerve dat_003F24E4;
extern const char dat_003BCC44[];
extern Nerve dat_003F24F4;
}

namespace {
inline void startSelected(Actor* actor, int mode) {
    if (mode == 4)
        fn_00272B60(actor, 2);
    else
        fn_002728C0(actor, dat_003BCC44);
}
}

extern "C" void fn_0035548C(const Nerve*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (!fn_00279ED4(actor, 3))
        fn_00279e8c(actor, 1.0f);
    fn_00257D10(actor, 0.5f, 1.0f);
    if (actor->flag) {
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F24E4);
        return;
    }
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor)) {
        fn_0027285C(actor);
        startSelected(actor, actor->mode);
        if (actor->mode != 1)
            fn_00272B60(actor, 2);
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F24F4);
    }
}
