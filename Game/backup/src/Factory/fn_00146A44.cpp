namespace {
struct AreaObjFactory;
struct AreaObjFactoryEntry;

struct LiveActorKit {
    unsigned char reserved[0x38];
    AreaObjFactory* areaObjFactory;
};
}

namespace al {
extern LiveActorKit* getLiveActorKit();
}

extern "C" void fn_001CC448(AreaObjFactory*, const AreaObjFactoryEntry*, int);
extern "C" const AreaObjFactoryEntry dat_003B896C[];

extern "C" void fn_00146A44() {
    fn_001CC448(al::getLiveActorKit()->areaObjFactory,
               dat_003B896C, 36);
}
