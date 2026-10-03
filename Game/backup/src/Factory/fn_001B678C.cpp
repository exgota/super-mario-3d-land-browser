namespace {

struct ByamlIter;

struct Vector3f {
    float x;
    float y;
    float z;
};

struct CameraParameters {
    unsigned char base[0x4c];
    void* targetParameters;
    float distance;
    float angleV;
    float upOffset;
    float targetRadius;
    float targetLookRate;
    Vector3f limitBoxMin;
    Vector3f limitBoxMax;
};

extern "C" void _ZN2al6Camera4loadEPKNS_9ByamlIterE(CameraParameters*, const ByamlIter*);
extern "C" bool _ZNK2al9ByamlIter16tryGetFloatByKeyEPfPKc(const ByamlIter*, float*, const char*);
extern "C" bool fn_0025BE14(Vector3f*, const ByamlIter*, const char*);
extern "C" void fn_0025BDB0(void*, const ByamlIter*);

}

extern "C" void fn_001B678C(CameraParameters* camera, const ByamlIter* iter) {
    _ZN2al6Camera4loadEPKNS_9ByamlIterE(camera, iter);
    _ZNK2al9ByamlIter16tryGetFloatByKeyEPfPKc(iter, &camera->distance, "Distance");
    _ZNK2al9ByamlIter16tryGetFloatByKeyEPfPKc(iter, &camera->angleV, "AngleV");
    _ZNK2al9ByamlIter16tryGetFloatByKeyEPfPKc(iter, &camera->upOffset, "UpOffset");
    _ZNK2al9ByamlIter16tryGetFloatByKeyEPfPKc(iter, &camera->targetRadius, "TargetRadius");
    _ZNK2al9ByamlIter16tryGetFloatByKeyEPfPKc(iter, &camera->targetLookRate, "TargetLookRate");
    fn_0025BE14(&camera->limitBoxMin, iter, "LimitBoxMin");
    fn_0025BE14(&camera->limitBoxMax, iter, "LimitBoxMax");
    fn_0025BDB0(camera->targetParameters, iter);
}
