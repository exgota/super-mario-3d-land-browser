namespace {
struct Vec3 {
    float x, y, z;
};

struct Actor {
    char unknown[0x64];
    float referenceHeight;
};

struct Nerve {};
}

extern "C" {
bool fn_003750B0(const Actor*);
void fn_002592F8(Vec3*);
const Vec3& _ZN2al8getTransEPKNS_9LiveActorE(const Actor*);
bool _ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(const Actor*, const Nerve*);
extern Nerve dat_003F1F98;
extern Nerve dat_003F1F88;
extern Nerve dat_003F1F8C;
extern Nerve dat_003F1F94;
extern Nerve dat_003F1F9C;
}

extern "C" bool fn_00374FC0(const Actor* actor) {
    if (!fn_003750B0(actor))
        return false;
    if (_ZN2al8getTransEPKNS_9LiveActorE(actor).y - actor->referenceHeight > 300.0f)
        return false;
    Vec3 position;
    fn_002592F8(&position);
    if (position.y - actor->referenceHeight < -50.0f)
        return false;
    if (_ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F1F98))
        return false;
    if (_ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F1F88))
        return false;
    if (_ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F1F8C))
        return false;
    if (_ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F1F94))
        return false;
    if (_ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F1F9C))
        return false;
    return true;
}
