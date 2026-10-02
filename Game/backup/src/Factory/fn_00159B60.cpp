namespace {
struct ByamlIter;

struct RailCamera {
    char base[0x50];
    int railId;
    int unknown54;
    float distance;
    float angleV;
    float angleH;
    float upOffset;
    float sideOffset;
    float sideDegree;
    bool isCalcStartPosUseLookAtPos;
};

extern "C" void _ZN2al6Camera4loadEPKNS_9ByamlIterE(RailCamera*, const ByamlIter*);
extern "C" bool _ZNK2al9ByamlIter14tryGetIntByKeyEPiPKc(const ByamlIter*, int*, const char*);
extern "C" bool _ZNK2al9ByamlIter16tryGetFloatByKeyEPfPKc(const ByamlIter*, float*, const char*);
extern "C" bool _ZNK2al9ByamlIter15tryGetBoolByKeyEPbPKc(const ByamlIter*, bool*, const char*);
}

extern "C" void fn_00159B60(RailCamera* self, const ByamlIter* iter) {
    _ZN2al6Camera4loadEPKNS_9ByamlIterE(self, iter);
    _ZNK2al9ByamlIter14tryGetIntByKeyEPiPKc(iter, &self->railId, "RailId");
    _ZNK2al9ByamlIter16tryGetFloatByKeyEPfPKc(iter, &self->distance, "Distance");
    _ZNK2al9ByamlIter16tryGetFloatByKeyEPfPKc(iter, &self->angleV, "AngleV");
    _ZNK2al9ByamlIter16tryGetFloatByKeyEPfPKc(iter, &self->angleH, "AngleH");
    _ZNK2al9ByamlIter16tryGetFloatByKeyEPfPKc(iter, &self->upOffset, "UpOffset");
    _ZNK2al9ByamlIter16tryGetFloatByKeyEPfPKc(iter, &self->sideOffset, "SideOffset");
    _ZNK2al9ByamlIter16tryGetFloatByKeyEPfPKc(iter, &self->sideDegree, "SideDegree");
    _ZNK2al9ByamlIter15tryGetBoolByKeyEPbPKc(iter, &self->isCalcStartPosUseLookAtPos, "IsCalcStartPosUseLookAtPos");
}
