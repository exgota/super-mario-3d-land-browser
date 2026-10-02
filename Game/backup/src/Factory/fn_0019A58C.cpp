namespace {
struct ByamlIter;

struct CameraParameters {
    unsigned char base[0x4c];
    float distance;
    float angleV;
    float angleH;
    float upOffset;
    float sideOffset;
    float sideDegree;
    float cameraOffset;
    float targetLookRate;
};
}

extern "C" void _ZN2al6Camera4loadEPKNS_9ByamlIterE(CameraParameters*, const ByamlIter*);
extern "C" bool _ZNK2al9ByamlIter16tryGetFloatByKeyEPfPKc(const ByamlIter*, float*, const char*);

extern "C" void fn_0019A58C(CameraParameters* camera, const ByamlIter* iter) {
    _ZN2al6Camera4loadEPKNS_9ByamlIterE(camera, iter);
    _ZNK2al9ByamlIter16tryGetFloatByKeyEPfPKc(iter, &camera->distance, "Distance");
    _ZNK2al9ByamlIter16tryGetFloatByKeyEPfPKc(iter, &camera->angleV, "AngleV");
    _ZNK2al9ByamlIter16tryGetFloatByKeyEPfPKc(iter, &camera->angleH, "AngleH");
    _ZNK2al9ByamlIter16tryGetFloatByKeyEPfPKc(iter, &camera->upOffset, "UpOffset");
    _ZNK2al9ByamlIter16tryGetFloatByKeyEPfPKc(iter, &camera->sideOffset, "SideOffset");
    _ZNK2al9ByamlIter16tryGetFloatByKeyEPfPKc(iter, &camera->sideDegree, "SideDegree");
    _ZNK2al9ByamlIter16tryGetFloatByKeyEPfPKc(iter, &camera->cameraOffset, "CameraOffset");
    _ZNK2al9ByamlIter16tryGetFloatByKeyEPfPKc(iter, &camera->targetLookRate, "TargetLookRate");
}
