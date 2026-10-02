namespace {
extern "C" const unsigned int _ZTVN4sead14SafeStringBaseIcEE[];
struct ActorInitInfo { unsigned int words[6]; };
struct Vector3 { float x, y, z; };
struct Matrix34 { float values[12]; };
struct SafeString {
    const void* vtable;
    const char* text;
    SafeString(const char* value)
        : vtable(_ZTVN4sead14SafeStringBaseIcEE + 2), text(value) {}
};
struct Nothrow {};
struct LiveActor {
    virtual void slot0() = 0;
    virtual void slot1() = 0;
    virtual void slot2() = 0;
    virtual void appear() = 0;
    unsigned char fields[0x5c];
};
struct ArchiveActor : LiveActor {
    unsigned int extra[2];
    const char* archive;
    int frame;
};

extern "C" {
void _ZN2al13ActorInitInfoC1Ev(ActorInitInfo*);
void fn_001C325C(ActorInitInfo*, const ActorInitInfo*, const LiveActor*);
void* _ZnwjRKSt9nothrow_t(unsigned int, const Nothrow&);
LiveActor* _ZN2al9LiveActorC1EPKc(LiveActor*, const char*);
void _ZN2al24initActorWithArchiveNameEPNS_9LiveActorERKNS_13ActorInitInfoERKN4sead14SafeStringBaseIcEEPKc(LiveActor*, const ActorInitInfo&, const SafeString&, const char*);
void _ZN2al9calcUpDirEPN4sead7Vector3IfEEPKNS_9LiveActorE(Vector3*, const LiveActor*);
void fn_0027012C(Matrix34*, const Vector3*, const Vector3*, const Vector3*);
void _ZN2al13updatePoseMtxEPNS_9LiveActorEPKN4sead8Matrix34IfEE(LiveActor*, const Matrix34*);
bool _ZN2al22tryStartMclAnimIfExistEPNS_9LiveActorEPKc(LiveActor*, const char*);
void fn_002687D0(LiveActor*, float);
extern const char dat_003B1254[];
extern const char dat_003B126C[];
}
}

extern "C" void fn_0013EC2C(ArchiveActor* source, const ActorInitInfo* init, const Vector3* position, const Vector3* direction) {
    if (source->archive) {
        ActorInitInfo info;
        _ZN2al13ActorInitInfoC1Ev(&info);
        fn_001C325C(&info, init, source);
        LiveActor* storage = static_cast<LiveActor*>(_ZnwjRKSt9nothrow_t(0x60, *reinterpret_cast<const Nothrow*>(init)));
        LiveActor* actor = storage ? _ZN2al9LiveActorC1EPKc(storage, dat_003B1254) : 0;
        _ZN2al24initActorWithArchiveNameEPNS_9LiveActorERKNS_13ActorInitInfoERKN4sead14SafeStringBaseIcEEPKc(actor, info, SafeString(source->archive), 0);
        Vector3 up;
        _ZN2al9calcUpDirEPN4sead7Vector3IfEEPKNS_9LiveActorE(&up, source);
        Matrix34 matrix;
        fn_0027012C(&matrix, &up, direction, position);
        _ZN2al13updatePoseMtxEPNS_9LiveActorEPKN4sead8Matrix34IfEE(actor, &matrix);
        if (_ZN2al22tryStartMclAnimIfExistEPNS_9LiveActorEPKc(actor, dat_003B126C))
            fn_002687D0(actor, static_cast<float>(source->frame));
        actor->appear();
    }
}
