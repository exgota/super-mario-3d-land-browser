namespace {
struct Actor;
struct Nerve;
struct Item;
struct Vec3 { float x, y, z; Vec3(float a, float b, float c) { set(a, b, c); } void set(float a, float b, float c) { x = a; y = b; z = c; } };
struct VTable { void (*slots[5])(); void (*kill)(Actor*); };
struct Actor { VTable* vtable; char pad[0x5c]; Item* item; };
extern "C" {
bool _ZN2al11isFirstStepEPKNS_9IUseNerveE(const Actor*);
bool _ZN2al18isGreaterEqualStepEPKNS_9IUseNerveEi(const Actor*, int);
void _ZN2al9onCollideEPNS_9LiveActorE(Actor*);
void _ZN2al14onDrawClippingEPNS_9LiveActorE(Actor*);
void _ZN2al24startHitReactionOnGroundEPKNS_9LiveActorE(const Actor*);
void _ZN2al21startHitReactionDeathEPKNS_9LiveActorE(const Actor*);
void fn_0026F770(Actor*, float);
float fn_00279114(float, float);
void fn_00279AC0(Actor*, const Vec3&);
bool fn_00279ED4(const Actor*, unsigned int);
void fn_00279e8c(Actor*, float);
void fn_00279e5c(Actor*, float);
bool fn_002620A8(const Item*);
void fn_00261FFC(Item*, Actor*);
}
}
extern "C" void fn_001A8D50(Actor* self) {
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(self)) {
        fn_0026F770(self, 35.0f);
        const Vec3& velocity = Vec3(0.0f, fn_00279114(-3.0f, 3.0f), 0.0f);
        fn_00279AC0(self, velocity);
        _ZN2al9onCollideEPNS_9LiveActorE(self);
        _ZN2al14onDrawClippingEPNS_9LiveActorE(self);
    }
    if (fn_00279ED4(self, 0)) {
        fn_00279e8c(self, 3.5f);
        fn_00279e5c(self, 0.98f);
    } else {
        fn_00279e8c(self, 3.5f);
        fn_00279e5c(self, 0.98f);
    }
    if (fn_00279ED4(self, 0)) {
        _ZN2al24startHitReactionOnGroundEPKNS_9LiveActorE(self);
        _ZN2al21startHitReactionDeathEPKNS_9LiveActorE(self);
        if (fn_002620A8(self->item)) fn_00261FFC(self->item, self);
        self->vtable->kill(self);
    } else if (_ZN2al18isGreaterEqualStepEPKNS_9IUseNerveEi(self, 20)) {
        _ZN2al21startHitReactionDeathEPKNS_9LiveActorE(self);
        if (fn_002620A8(self->item)) fn_00261FFC(self->item, self);
        self->vtable->kill(self);
    }
}
