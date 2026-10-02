namespace {
struct LiveActor;
}

extern "C" void _ZN2al21startHitReactionDeathEPKNS_9LiveActorE(const LiveActor*);
extern "C" void _ZN2al25startHitReactionDisappearEPKNS_9LiveActorE(const LiveActor*);
extern "C" void _ZN2al9LiveActor4killEv(LiveActor*);
extern "C" void _ZN2al9LiveActor17makeActorAppearedEv(LiveActor*);
extern "C" void fn_001BC728(LiveActor*);

extern "C" void fn_0012C82C(LiveActor* actor)
{
    _ZN2al21startHitReactionDeathEPKNS_9LiveActorE(actor);
    _ZN2al9LiveActor4killEv(actor);
}

extern "C" void fn_001BCA5C(LiveActor* actor)
{
    fn_001BC728(actor);
    _ZN2al9LiveActor17makeActorAppearedEv(actor);
}

extern "C" void fn_0031F2F8(LiveActor* actor)
{
    _ZN2al25startHitReactionDisappearEPKNS_9LiveActorE(actor);
    _ZN2al9LiveActor4killEv(actor);
}

extern "C" void fn_00325AC0(LiveActor* actor)
{
    _ZN2al21startHitReactionDeathEPKNS_9LiveActorE(actor);
    _ZN2al9LiveActor4killEv(actor);
}
