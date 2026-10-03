namespace {
struct PlayerState {
    unsigned char padding[0x180];
    int state;
    void reset();
};

struct SceneContext {
    unsigned char padding[0x40];
    void* component;
    unsigned int unknown44;
    PlayerState* player;
};

struct StateOwner {
    unsigned char padding[0x0c];
    SceneContext* context;
    unsigned int unknown10;
    void* auxiliary;
    unsigned int unknown18;
    void* actor;
};

struct Spine {
    StateOwner* owner;
};
}

extern "C" {
bool _ZN2al11isFirstStepEPKNS_9IUseNerveE(const StateOwner*);
bool _ZN2al6isStepEPNS_9IUseNerveEi(StateOwner*, int);
bool _ZN2al13isGreaterStepEPKNS_9IUseNerveEi(const StateOwner*, int);
void _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(StateOwner*, const void*);
void fn_0025BA58(const char*);
void fn_002518C8(void*);
void fn_002556C4(void*);
void fn_0017C528(PlayerState*, bool);
void fn_002257D0(void*);
void fn_00213EDC(void*, bool);
void fn_0017C514(PlayerState*);
bool fn_002136E0(void*);
void fn_0025BCC0(void*);
void fn_00272AE4(void*);
void fn_00188F14(void*);
extern const char dat_003B75E4;
extern const unsigned char dat_003F1768;

__attribute__((weak)) void factory_003732B4_storeState(PlayerState*, int);
}

namespace {
inline void PlayerState::reset() {
    factory_003732B4_storeState(this, 0);
    fn_0017C514(this);
}
}

extern "C" void fn_003732B4(const void*, const Spine* spine) {
    StateOwner* owner = spine->owner;
    PlayerState* player = owner->context->player;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(owner)) {
        fn_0025BA58(&dat_003B75E4);
        fn_002518C8(owner->context->component);
        fn_002556C4(owner->auxiliary);
        fn_0017C528(player, false);
        fn_002257D0(owner->actor);
        fn_00213EDC(owner->actor, false);
    }
    if (_ZN2al6isStepEPNS_9IUseNerveEi(owner, 60)) {
        fn_00213EDC(owner->actor, true);
        player->reset();
    }
    if (_ZN2al13isGreaterStepEPKNS_9IUseNerveEi(owner, 60)
        && fn_002136E0(owner->actor)) {
        fn_0025BCC0(owner->actor);
        fn_00272AE4(owner->context->component);
        fn_00188F14(owner->context->component);
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(owner, &dat_003F1768);
    }
}

extern "C" __attribute__((weak))
void factory_003732B4_storeState(PlayerState* player, int value) {
    player->state = value;
}
