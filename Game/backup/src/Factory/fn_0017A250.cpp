namespace {
struct ByamlIter;
struct CameraState;
struct Vector3f {
    float x, y, z;
};
struct Camera {
    char base[0x4c];
    CameraState* state;
    float pushDistance;
    float pullDistance;
    float lowAngle;
    float highAngle;
    float upOffset;
    Vector3f limitBoxMin;
    Vector3f limitBoxMax;
};
}

extern "C" void _ZN2al6Camera4loadEPKNS_9ByamlIterE(Camera*, const ByamlIter*);
extern "C" bool _ZNK2al9ByamlIter16tryGetFloatByKeyEPfPKc(const ByamlIter*, float*, const char*);
extern "C" void fn_0025BE14(Vector3f*, const ByamlIter*, const char*);
extern "C" void fn_0025BDB0(CameraState*, const ByamlIter*);

extern "C" void fn_0017A250(Camera* self, const ByamlIter* iter) {
    _ZN2al6Camera4loadEPKNS_9ByamlIterE(self, iter);
    _ZNK2al9ByamlIter16tryGetFloatByKeyEPfPKc(iter, &self->pushDistance, "PushDistance");
    _ZNK2al9ByamlIter16tryGetFloatByKeyEPfPKc(iter, &self->pullDistance, "PullDistance");
    _ZNK2al9ByamlIter16tryGetFloatByKeyEPfPKc(iter, &self->lowAngle, "LowAngle");
    _ZNK2al9ByamlIter16tryGetFloatByKeyEPfPKc(iter, &self->highAngle, "HighAngle");
    _ZNK2al9ByamlIter16tryGetFloatByKeyEPfPKc(iter, &self->upOffset, "UpOffset");
    fn_0025BE14(&self->limitBoxMin, iter, "LimitBoxMin");
    fn_0025BE14(&self->limitBoxMax, iter, "LimitBoxMax");
    fn_0025BDB0(self->state, iter);
}
