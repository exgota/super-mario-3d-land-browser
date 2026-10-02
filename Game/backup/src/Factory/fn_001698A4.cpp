namespace {
struct Vec3Data {
    float x, y, z;
};

struct Vec3 : Vec3Data {
    Vec3& operator=(const Vec3& other) {
        static_cast<Vec3Data&>(*this) = static_cast<const Vec3Data&>(other);
        return *this;
    }
};

struct Actor {
    unsigned char base[0x60];
    Vec3 savedTrans;
    unsigned char gap6c[4];
    int current;
    int threshold;
    unsigned char gap78[6];
    bool enabled;
    bool floorTouchEnabled;
};

struct Nerve;
}

extern "C" bool _ZN2al21isMsgPlayerFloorTouchEj(unsigned int);
extern "C" bool fn_0027F1A8(unsigned int);
extern "C" bool _ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(const Actor*, const Nerve*);
extern "C" void _ZN2al18invalidateClippingEPNS_9LiveActorE(Actor*);
extern "C" void _ZN2al16startNerveActionEPNS_9LiveActorEPKc(Actor*, const char*);
extern "C" const Vec3& _ZN2al8getTransEPKNS_9LiveActorE(const Actor*);
extern "C" Nerve dat_003F26E0;
extern "C" Nerve dat_003F2700;
extern "C" const char dat_003BD81C[];

extern "C" bool fn_001698A4(Actor* actor, unsigned int message) {
    if (_ZN2al21isMsgPlayerFloorTouchEj(message) && actor->floorTouchEnabled &&
        _ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F26E0)) {
        _ZN2al18invalidateClippingEPNS_9LiveActorE(actor);
        if (actor->current > actor->threshold)
            _ZN2al16startNerveActionEPNS_9LiveActorEPKc(actor, "Touch");
        else
            _ZN2al16startNerveActionEPNS_9LiveActorEPKc(actor, "FallSign");
        return true;
    }
    if (fn_0027F1A8(message) && actor->enabled &&
        !_ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F2700)) {
        actor->savedTrans = _ZN2al8getTransEPKNS_9LiveActorE(actor);
        _ZN2al16startNerveActionEPNS_9LiveActorEPKc(actor, dat_003BD81C);
        _ZN2al18invalidateClippingEPNS_9LiveActorE(actor);
    }
    return false;
}
