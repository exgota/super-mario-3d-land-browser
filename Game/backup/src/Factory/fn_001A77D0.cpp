extern "C" const unsigned char _ZTVN4sead14SafeStringBaseIcEE[];

namespace {
struct SafeString {
    const void* vtable;
    const char* text;
    SafeString(const char* value)
        : vtable(_ZTVN4sead14SafeStringBaseIcEE + 8), text(value) {}
};

struct Action {
    virtual void slot0() = 0;
    virtual void reset() = 0;
    virtual void slot2() = 0;
    virtual void activate() = 0;
};

struct Animation {
    virtual void slot0() = 0;
    virtual void reset() = 0;
    virtual void slot2() = 0;
    virtual void start(const char* name) = 0;
    virtual bool active() = 0;
};

struct Sound {
    virtual void slot0() = 0;
    virtual void slot1() = 0;
    virtual void slot2() = 0;
    virtual void play(const SafeString& name) = 0;
};

struct Actor {
    unsigned char padding[12];
    Sound* sound;
};

struct State {
    unsigned char padding[12];
    Actor* actor;
    Animation* animation;
    Action* action;
    unsigned char padding18[12];
    Action* controller;
};
}

extern "C" void fn_001A77D0(State* self) {
    self->controller->reset();
    if (!self->animation->active()) {
        self->animation->start("Transform");
        self->animation->reset();
        self->actor->sound->play(SafeString("SePmTurnBackStatue"));
    }
    self->action->activate();
    self->actor->sound->play(SafeString("SePvDummyForVoiceStop"));
}
