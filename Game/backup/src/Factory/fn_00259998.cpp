namespace {
struct Vector3 {
    float x, y, z;
};

struct LiveActor {
    char reserved[0x60];
    void* keeper;
};
}

extern "C" LiveActor* fn_0025A294(void*, LiveActor*);
extern "C" bool fn_00259890(void*, LiveActor*);
extern "C" const Vector3& _ZN2al8getTransEPKNS_9LiveActorE(const LiveActor*);
extern "C" Vector3* _ZN2al11getTransPtrEPNS_9LiveActorE(LiveActor*);
extern "C" const Vector3& _ZN2al11getVelocityEPKNS_9LiveActorE(const LiveActor*);
extern "C" Vector3* _ZN2al14getVelocityPtrEPNS_9LiveActorE(LiveActor*);

namespace {
LiveActor* getLinkedActor(void* keeper, LiveActor* actor) {
    return keeper ? fn_0025A294(keeper, actor) : 0;
}
}

extern "C" void fn_00259998(LiveActor* actor) {
    if (getLinkedActor(actor->keeper, actor)) {
        _ZN2al11getTransPtrEPNS_9LiveActorE(actor)->x =
            _ZN2al8getTransEPKNS_9LiveActorE(getLinkedActor(actor->keeper, actor)).x;
        _ZN2al11getTransPtrEPNS_9LiveActorE(actor)->z =
            _ZN2al8getTransEPKNS_9LiveActorE(getLinkedActor(actor->keeper, actor)).z;
        if (fn_00259890(actor->keeper, actor)) {
            _ZN2al11getTransPtrEPNS_9LiveActorE(actor)->y =
                _ZN2al8getTransEPKNS_9LiveActorE(getLinkedActor(actor->keeper, actor)).y + 110.0f;
            if (_ZN2al11getVelocityEPKNS_9LiveActorE(actor).y < -8.0f) {
                _ZN2al14getVelocityPtrEPNS_9LiveActorE(actor)->y =
                    _ZN2al11getVelocityEPKNS_9LiveActorE(actor).y * 0.0f;
            }
        }
    }
}
