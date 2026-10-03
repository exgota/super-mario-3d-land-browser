namespace {
struct Inner {
    unsigned char pad[0x10];
    void *value;
};
struct Outer {
    unsigned char pad[0x68];
    Inner *inner;
};
}

extern "C" void *fn_00326834(void *);
namespace al {
struct LiveActor;
void *getTrans(const LiveActor *);
}

extern "C" void *fn_00280280(const Outer *p) {
    return al::getTrans(static_cast<const al::LiveActor *>(p->inner->value));
}

extern "C" void *fn_00326828(const Outer *p) {
    return fn_00326834(p->inner->value);
}
