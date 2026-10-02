namespace {
struct LiveActor;
struct Transition;

struct Vec3 {
    float x, y, z;
    Vec3& operator=(const Vec3& v) {
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
    }
};

struct Quat {
    float x, y, z, w;
    Quat& operator=(const Quat& q) {
        x = q.x;
        y = q.y;
        z = q.z;
        w = q.w;
        return *this;
    }
};

struct ActorArray {
    int count;
    int capacity;
    LiveActor** actors;
    LiveActor* get(int index) const {
        if (static_cast<unsigned>(index) < static_cast<unsigned>(count))
            return actors[index];
        return 0;
    }
};

struct ActorTransition {
    LiveActor* actor;
    Transition* transition;
    ActorArray* targets;
    int index;
    Vec3 startTrans;
    Quat startQuat;
    Vec3 endTrans;
    Quat endQuat;
};
}

extern "C" const Quat& _ZN2al7getQuatEPKNS_9LiveActorE(const LiveActor*);
extern "C" const Vec3& _ZN2al8getTransEPKNS_9LiveActorE(const LiveActor*);
extern "C" const unsigned char dat_004305D4[];
extern "C" void fn_0025F1C0(Transition*, const Vec3&, const Vec3&, const void*, float);

extern "C" void fn_0026759C(ActorTransition* self, int index) {
    self->index = index;
    self->startQuat = _ZN2al7getQuatEPKNS_9LiveActorE(self->actor);
    self->startTrans = _ZN2al8getTransEPKNS_9LiveActorE(self->actor);
    self->endQuat = _ZN2al7getQuatEPKNS_9LiveActorE(self->targets->get(index));
    self->endTrans = _ZN2al8getTransEPKNS_9LiveActorE(self->targets->get(index));
    fn_0025F1C0(self->transition, self->startTrans, self->endTrans,
               reinterpret_cast<const void*>(0x004305D4), 400.0f);
}
