namespace {
struct ActorFields {
    unsigned char padding[0x68];
    void *holder;
};

struct HolderFields {
    unsigned char padding[0x0c];
    void *value;
};

typedef void *(*Forward)(void *);
}

extern "C" void *fn_0026F974(void *);
extern "C" void *_ZN2al8getTransEPKNS_9LiveActorE(void *);
extern "C" void *fn_00326834(void *);

extern "C" void *fn_00141D40(ActorFields *actor) {
    return fn_0026F974(static_cast<HolderFields *>(actor->holder)->value);
}

extern "C" void *fn_00255DDC(ActorFields *actor) {
    return _ZN2al8getTransEPKNS_9LiveActorE(static_cast<HolderFields *>(actor->holder)->value);
}

extern "C" void *fn_0025A744(ActorFields *actor) {
    return fn_00326834(static_cast<HolderFields *>(actor->holder)->value);
}
