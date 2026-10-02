namespace {
struct Actor {
    char base[0x60];
    void* actionKeeper;
    float arg0;
    int arg1;
    float arg2;
};
struct ActorInitInfo;
struct Nerve;
}

extern "C" {
void _ZN2al17initActorPoseTFSVEPNS_9LiveActorE(Actor*);
void fn_00270AB0(Actor*, const ActorInitInfo*);
void fn_0026F5A8(Actor*, const ActorInitInfo*);
void fn_0026F56C(Actor*, const ActorInitInfo*, int);
void fn_00277de0(Actor*, const ActorInitInfo*);
void* _ZnwjRKSt9nothrow_t(unsigned int, const void*);
void* fn_0025A840(void*, Actor*, const ActorInitInfo*, const char*, int);
void fn_002794F8(float*, const ActorInitInfo*);
void fn_0027D1DC(int*, const ActorInitInfo*);
void fn_0027D180(float*, const ActorInitInfo*);
void fn_00280538(void*, const ActorInitInfo*);
void fn_0027FAB8(Actor*);
void _ZN2al9initNerveEPNS_9LiveActorEPKNS_5NerveEi(Actor*, const Nerve*, int);
extern Nerve dat_003F1DA8;
extern Nerve dat_003F1DB0;
}

extern "C" void fn_0018938C(Actor* actor, const ActorInitInfo* info) {
    _ZN2al17initActorPoseTFSVEPNS_9LiveActorE(actor);
    fn_00270AB0(actor, info);
    fn_0026F5A8(actor, info);
    fn_0026F56C(actor, info, 16);
    fn_00277de0(actor, info);
    void* keeper = _ZnwjRKSt9nothrow_t(24, actor);
    if (keeper)
        keeper = fn_0025A840(keeper, actor, info, "KoopaFire", 10);
    actor->actionKeeper = keeper;
    fn_002794F8(&actor->arg0, info);
    fn_0027D1DC(&actor->arg1, info);
    fn_0027D180(&actor->arg2, info);
    fn_00280538(reinterpret_cast<char*>(actor) + 12, info);
    fn_0027FAB8(actor);
    if (actor->arg1 > 0)
        _ZN2al9initNerveEPNS_9LiveActorEPKNS_5NerveEi(actor, &dat_003F1DA8, 0);
    else
        _ZN2al9initNerveEPNS_9LiveActorEPKNS_5NerveEi(actor, &dat_003F1DB0, 0);
}
