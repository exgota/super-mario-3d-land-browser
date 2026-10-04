namespace {
struct ActorService {
    unsigned char padding[0x8];
    void* context;
};

struct LiveActorKit {
    unsigned char padding[0x24];
    ActorService* service;
};
}

namespace al {
extern LiveActorKit* getLiveActorKit();
}

extern "C" void* fn_001DBF1C(void*, void*);
extern "C" void* fn_001DC058(void*, void*);
extern "C" void* fn_00244D24(void*, void*);

extern "C" void* fn_001DBEFC(void* argument) {
    return fn_001DBF1C(al::getLiveActorKit()->service->context, argument);
}

extern "C" void* fn_001DC038(void* argument) {
    return fn_001DC058(al::getLiveActorKit()->service->context, argument);
}

extern "C" void* fn_00244D04(void* argument) {
    return fn_00244D24(al::getLiveActorKit()->service->context, argument);
}
