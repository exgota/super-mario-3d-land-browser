extern "C" const void* _ZTVN4sead14SafeStringBaseIcEE[];

namespace {
struct ActorInitInfo;
struct Nerve;
struct SafeString {
    const void* vtable;
    const char* text;
    SafeString(const char* value) : vtable(_ZTVN4sead14SafeStringBaseIcEE + 2), text(value) {}
};
struct Vector3 { float x, y, z; };
struct Actor {
    void (**vtable)(Actor*);
    char padding[0x60];
    Actor* demo;
    Vector3 position;
    Vector3 rotation;
    char unknown[8];
    bool flag;
};
struct Demo { char storage[0x6c]; };
}

extern "C" {
extern const Nerve dat_003F1D94;
void _ZN2al24initActorWithArchiveNameEPNS_9LiveActorERKNS_13ActorInitInfoERKN4sead14SafeStringBaseIcEEPKc(Actor*, const ActorInitInfo&, const SafeString&, const char*);
void fn_00272DFC(Vector3*, Vector3*, const ActorInitInfo&, int);
Actor* fn_00255D9C(Demo*, const SafeString&);
void _ZN2al30initCreateActorNoPlacementInfoEPNS_9LiveActorERKNS_13ActorInitInfoE(Actor*, const ActorInitInfo&);
bool fn_0021E100();
void _ZN2al9initNerveEPNS_9LiveActorEPKNS_5NerveEi(Actor*, const Nerve*, int);
void* _ZnwjRKSt9nothrow_t(unsigned int, ...);
}

namespace {
inline Actor* createDemo(const char* name, const ActorInitInfo& info) {
    Demo* allocated = static_cast<Demo*>(_ZnwjRKSt9nothrow_t(0x6c));
    Actor* demo;
    if (allocated) {
        SafeString actorName(name);
        demo = fn_00255D9C(allocated, actorName);
    } else demo = 0;
    _ZN2al30initCreateActorNoPlacementInfoEPNS_9LiveActorERKNS_13ActorInitInfoE(demo, info);
    return demo;
}
}

extern "C" {
void fn_0031ED34(Actor* actor, const ActorInitInfo& info) {
    _ZN2al24initActorWithArchiveNameEPNS_9LiveActorERKNS_13ActorInitInfoERKN4sead14SafeStringBaseIcEEPKc(actor, info, SafeString("KoopaThirdLastGate"), 0);
    fn_00272DFC(&actor->position, &actor->rotation, info, 0);
    Actor* demo = createDemo("DemoKoopaLv3Stage", info);
    actor->demo = demo;
    actor->flag = !fn_0021E100();
    _ZN2al9initNerveEPNS_9LiveActorEPKNS_5NerveEi(actor, &dat_003F1D94, 0);
    actor->vtable[4](actor);
}
}
