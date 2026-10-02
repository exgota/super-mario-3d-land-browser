namespace {
struct Actor;
struct Vector3 {
    float x, y, z;
};
}

extern "C" {
void fn_00267E50(Actor*);
bool fn_0026B924(const Actor*);
const Vector3& _ZN2al11getVelocityEPKNS_9LiveActorE(const Actor*);
const Vector3& fn_00265ECC(const Actor*);
void fn_0026FE44(Vector3*, const Vector3&, float, float);
const Vector3& _ZN2al10getGravityEPKNS_9LiveActorE(const Actor*);
bool fn_0027A724(const Vector3&, const Vector3&, float);
void _ZN2al11setVelocityEPNS_9LiveActorERKN4sead7Vector3IfEE(Actor*, const Vector3&);
bool fn_0027D5C4(Vector3*);
Vector3* _ZN2al11getFrontPtrEPNS_9LiveActorE(Actor*);
void fn_0027C000(Actor*, const Vector3*, const Vector3&, float);
}

extern "C" void fn_0017E4B8(Actor* actor) {
    fn_00267E50(actor);
    if (fn_0026B924(actor)) {
        Vector3 velocity(_ZN2al11getVelocityEPKNS_9LiveActorE(actor));
        Vector3 adjusted(velocity);
        fn_0026FE44(&adjusted, fn_00265ECC(actor), 1.0f, 0.0f);
        if (!fn_0027A724(velocity, _ZN2al10getGravityEPKNS_9LiveActorE(actor), 0.01f))
            _ZN2al11setVelocityEPNS_9LiveActorERKN4sead7Vector3IfEE(actor, adjusted);
    }
    Vector3 direction(_ZN2al11getVelocityEPKNS_9LiveActorE(actor));
    if (!fn_0027D5C4(&direction))
        fn_0027C000(actor, _ZN2al11getFrontPtrEPNS_9LiveActorE(actor), direction, 15.0f);
}
