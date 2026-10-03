namespace {
struct Vec3 { float x, y, z; };
struct Actor {
    char pad00[8];
    void* audio;
    Vec3 position;
    char pad18[0x68];
    int timer;
    Vec3 playerPosition;
    int pad90;
    void* counter;
};
struct Context { Actor* actor; };
struct SafeString {
    const void* vtable;
    const char* text;
    SafeString(const char* value);
};
}

extern "C" {
extern const char dat_003BFCA0[];
extern const char dat_003BFCBC[];
extern const void* _ZTVN4sead14SafeStringBaseIcEE[];
void fn_00257860(void*, int);
void fn_0027109C(void*, const SafeString&);
void fn_0027798C(Actor*);
void fn_00266C14(float);
void fn_002788B8(Vec3*);
}
namespace rp { const Vec3& getPlayerPos(); }

namespace {
SafeString::SafeString(const char* value)
    : vtable(&_ZTVN4sead14SafeStringBaseIcEE[2]), text(value) {}
}

extern "C" void fn_0035478C(void*, const Context* context) {
    Actor* actor = context->actor;
    fn_00257860(actor->counter, (actor->timer + 29) / 30);
    if (actor->timer > 0)
        --actor->timer;
    if (actor->timer == 90 || actor->timer == 60 || actor->timer == 30) {
        SafeString sound(dat_003BFCA0);
        fn_0027109C(&actor->audio, sound);
    }
    if (actor->timer == 1)
        fn_0027798C(actor);
    if (actor->timer == 0) {
        fn_00266C14(1.0f);
        SafeString sound(dat_003BFCBC);
        fn_0027109C(&actor->audio, sound);
        fn_002788B8(&actor->position);
        actor->playerPosition = rp::getPlayerPos();
    }
}
