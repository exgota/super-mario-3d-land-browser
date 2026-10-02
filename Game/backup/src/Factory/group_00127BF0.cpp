extern "C" {
extern const unsigned char _ZTVN4sead14SafeStringBaseIcEE[];
extern const unsigned char dat_003F30EC[];
extern const unsigned char dat_003F1AEC[];
}

namespace {
class ActorInitInfo;
class Nerve;

class LiveActor {
public:
    virtual void destructorSlot0() = 0;
    virtual void destructorSlot1() = 0;
    virtual void init(const ActorInitInfo&) = 0;
    virtual void initAfterPlacement() = 0;
    virtual void makeActorAppeared() = 0;
};

class SafeString {
public:
    SafeString(const char* text)
        : vptr(_ZTVN4sead14SafeStringBaseIcEE + 8), text(text) {}
    ~SafeString() {}

private:
    const void* vptr;
    const char* text;
};
}

extern "C" {
void _ZN2al24initActorWithArchiveNameEPNS_9LiveActorERKNS_13ActorInitInfoERKN4sead14SafeStringBaseIcEEPKc(
    LiveActor*, const ActorInitInfo&, const SafeString&, const char*);
void _ZN2al9initNerveEPNS_9LiveActorEPKNS_5NerveEi(
    LiveActor*, const Nerve*, int);
void fn_00267F2C(LiveActor*, const ActorInitInfo&, const SafeString&, const char*);
}

extern "C" void fn_00127BF0(LiveActor* self, const ActorInitInfo& info)
{
    _ZN2al24initActorWithArchiveNameEPNS_9LiveActorERKNS_13ActorInitInfoERKN4sead14SafeStringBaseIcEEPKc(
        self, info, "WanwanRoot", 0);
    _ZN2al9initNerveEPNS_9LiveActorEPKNS_5NerveEi(
        self, reinterpret_cast<const Nerve*>(dat_003F30EC), 0);
    self->makeActorAppeared();
}

extern "C" void fn_0016A328(LiveActor* self, const ActorInitInfo& info)
{
    fn_00267F2C(self, info, "CourseRoad", 0);
    _ZN2al9initNerveEPNS_9LiveActorEPKNS_5NerveEi(
        self, reinterpret_cast<const Nerve*>(dat_003F1AEC), 0);
    self->makeActorAppeared();
}
