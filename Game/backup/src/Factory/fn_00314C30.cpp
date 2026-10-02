namespace {
struct Nerve {};
struct RailHelper {};
struct ClippingHelper {};
struct ActorInitInfo {
    void* unknown;
    void* placement;
};
struct Actor {
    unsigned char base[0x68];
    RailHelper* rail;
    ClippingHelper* clipping;
    float first[12];
    float second[3];
    bool hasArgument;
};

extern "C" {
void _ZN2al9initActorEPNS_9LiveActorERKNS_13ActorInitInfoE(Actor*, const ActorInitInfo&);
void _ZN2al9initNerveEPNS_9LiveActorEPKNS_5NerveEi(Actor*, const Nerve*, int);
void* _ZnwjRKSt9nothrow_t(unsigned int, const void*);
RailHelper* fn_0027A83C(void*, void*);
bool fn_00136548(Actor*, const ActorInitInfo&, float*, int);
void fn_001366B0(Actor*, const ActorInitInfo&, float*, int);
void fn_00136624(Actor*, const ActorInitInfo&, float*, int);
bool _ZN2al10tryGetArg3EPiRKNS_13ActorInitInfoE(int*, const ActorInitInfo&);
bool fn_0027ECA4(Actor*);
ClippingHelper* fn_00270F00(void*, Actor*, int, int);
void _ZN2al18invalidateClippingEPNS_9LiveActorE(Actor*);
void _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(Actor*, const Nerve*);
extern Nerve dat_003F26AC;
extern Nerve dat_003F26A8;
}
}

extern "C" void fn_00314C30(Actor* actor, const ActorInitInfo& info) {
    _ZN2al9initActorEPNS_9LiveActorERKNS_13ActorInitInfoE(actor, info);
    _ZN2al9initNerveEPNS_9LiveActorEPKNS_5NerveEi(actor, &dat_003F26AC, 0);
    void* rail = _ZnwjRKSt9nothrow_t(24, actor);
    if (rail)
        rail = fn_0027A83C(rail, info.placement);
    actor->rail = static_cast<RailHelper*>(rail);
    if (fn_00136548(actor, info, actor->first, 0))
        fn_001366B0(actor, info, actor->second, 2);
    else
        fn_00136624(actor, info, actor->second, 2);
    int arg = -1;
    _ZN2al10tryGetArg3EPiRKNS_13ActorInitInfoE(&arg, info);
    actor->hasArgument = arg != -1;
    if (fn_0027ECA4(actor)) {
        void* clipping = reinterpret_cast<void* (*)(unsigned int)>(_ZnwjRKSt9nothrow_t)(24);
        if (clipping)
            clipping = fn_00270F00(clipping, actor, 1, 0);
        actor->clipping = static_cast<ClippingHelper*>(clipping);
        _ZN2al18invalidateClippingEPNS_9LiveActorE(actor);
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F26A8);
    }
}
