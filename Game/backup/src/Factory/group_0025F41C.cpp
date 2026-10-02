namespace {
namespace al {
struct LiveActor;
}
namespace sead {
template <typename T> struct Vector3;
}
}

extern "C" void* _ZN2al8getTransEPKNS_9LiveActorE(const al::LiveActor*);
extern "C" void* fn_0025F438(void*, void*);
extern "C" void* fn_0027D370(void*, void*);
extern "C" void* _ZN2rp14getPlayerActorEv();
extern "C" void* _ZN2al12calcFrontDirEPN4sead7Vector3IfEEPKNS_9LiveActorE(void*, const al::LiveActor*);

extern "C" void* fn_0025F41C(const al::LiveActor* actor) {
    void* trans = _ZN2al8getTransEPKNS_9LiveActorE(actor);
    return fn_0025F438(const_cast<al::LiveActor*>(actor), trans);
}

extern "C" void* fn_00278644(void* out) {
    const al::LiveActor* actor = static_cast<const al::LiveActor*>(_ZN2rp14getPlayerActorEv());
    return _ZN2al12calcFrontDirEPN4sead7Vector3IfEEPKNS_9LiveActorE(out, actor);
}

extern "C" void* fn_0027D354(const al::LiveActor* actor) {
    void* trans = _ZN2al8getTransEPKNS_9LiveActorE(actor);
    return fn_0027D370(const_cast<al::LiveActor*>(actor), trans);
}
