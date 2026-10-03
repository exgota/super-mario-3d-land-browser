namespace {
struct Vec3 {
    float x, y, z;
};

extern "C" const unsigned char _ZTVN4sead14SafeStringBaseIcEE[];

struct SafeString {
    const void* vtable;
    const char* text;
    SafeString(const char* value) : vtable(_ZTVN4sead14SafeStringBaseIcEE + 8), text(value) {}
};

struct State {
    unsigned char unknown[8];
    void* actor;
    const char* effect;
    unsigned char unknown10[4];
    const char* impactEffect;
    Vec3 position;
    float velocity;
    float height;
    unsigned char unknown2c[4];
    float gravity;
    float ceiling;
    float restitution;
    float damping;
};

extern "C" void fn_00278F44(void*, const char*, float);
extern "C" void fn_00278BF4(void*, const SafeString&, const Vec3&);
}

extern "C" void fn_00320DA8(State* self) {
    self->velocity -= self->gravity;
    self->velocity *= self->damping;
    self->height += self->velocity;
    if (self->height > self->ceiling) {
        self->velocity = 0.0f;
        self->height = self->ceiling;
    }
    if (self->height < 0.0f) {
        self->height = 0.0f;
        if (self->velocity < 0.0f) {
            float speed = self->velocity > 0.0f ? self->velocity : -self->velocity;
            fn_00278F44(self->actor ? static_cast<unsigned char*>(self->actor) + 4 : 0,
                       self->impactEffect, speed);
            self->velocity = -self->velocity * self->restitution;
        }
    }
    if (self->effect) {
        Vec3 position = self->position;
        position.y += self->height;
        fn_00278BF4(self->actor, SafeString(self->effect), position);
    }
}
