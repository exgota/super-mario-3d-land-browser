namespace al {
struct LiveActorKit {
    unsigned char pad[0x28];
    void* actor;
};
LiveActorKit* getLiveActorKit();
}

extern "C" void* fn_001D43DC(void*, void*);
extern "C" void* fn_0025C9E8(void*, void*);

extern "C" void* fn_001D43C0(void* arg) {
    return fn_001D43DC(al::getLiveActorKit()->actor, arg);
}

extern "C" void* fn_0025C9CC(void* arg) {
    return fn_0025C9E8(al::getLiveActorKit()->actor, arg);
}
