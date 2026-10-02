namespace al {
void* getLiveActorKit();
}

extern "C" void* fn_001CC314(void*, void*);
extern "C" void* fn_003328AC(void*, void*);

namespace {
struct LiveActorKit {
    unsigned char pad[0x38];
    void* actor;
};
}

extern "C" void* fn_001CC2F8(void* actor) {
    return fn_001CC314(static_cast<LiveActorKit*>(al::getLiveActorKit())->actor, actor);
}

extern "C" void* fn_00332890(void* actor) {
    return fn_003328AC(static_cast<LiveActorKit*>(al::getLiveActorKit())->actor, actor);
}
