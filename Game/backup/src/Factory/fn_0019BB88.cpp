namespace {
extern "C" char _ZTVN4sead14SafeStringBaseIcEE[];
struct SafeString {
    void (**vtable)(SafeString*);
    const char* text;
    SafeString() {}
    SafeString(const char* value) {
        text = value;
        vtable = reinterpret_cast<void (**)(SafeString*)>(_ZTVN4sead14SafeStringBaseIcEE + 8);
    }
    SafeString(const SafeString& value) {
        value.vtable[2](const_cast<SafeString*>(&value));
        text = value.text;
        vtable = reinterpret_cast<void (**)(SafeString*)>(_ZTVN4sead14SafeStringBaseIcEE + 8);
    }
    void set(const char* value) {
        text = value;
        vtable = reinterpret_cast<void (**)(SafeString*)>(_ZTVN4sead14SafeStringBaseIcEE + 8);
    }
    SafeString& set(const SafeString& value) {
        value.vtable[2](const_cast<SafeString*>(&value));
        text = value.text;
        vtable = reinterpret_cast<void (**)(SafeString*)>(_ZTVN4sead14SafeStringBaseIcEE + 8);
        return *this;
    }
};
struct FormatString {
    void* vtable;
    const char* text;
    int capacity;
    int length;
    char buffer[64];
};
struct Actor {
    void (**vtable)(Actor*);
    char fields[0x5c];
};
struct Owner {
    char fields[0x64];
    Actor* source;
    char gap[8];
    Actor* model;
};
struct ActorInitInfo;
extern "C" void* _ZnwjRKSt9nothrow_t(unsigned int, const ActorInitInfo&);
extern "C" Actor* _ZN2al11MapObjActorC1ERKN4sead14SafeStringBaseIcEE(void*, const SafeString&);
extern "C" SafeString* fn_0027AD3C(FormatString*, const char*, ...);
extern "C" void fn_00267F2C(Actor*, const ActorInitInfo&, const SafeString&, int);
extern "C" void _ZN2al8copyPoseEPNS_9LiveActorEPKS0_(Actor*, const Actor*);
extern "C" void _ZN2al11startActionEPNS_9LiveActorEPKc(Actor*, const char*);
}

extern "C" void fn_0019BB88(Owner* self, const ActorInitInfo& initInfo, const char* name, bool appear) {
    SafeString string;
    void* allocation = _ZnwjRKSt9nothrow_t(0x60, initInfo);
    Actor* actor = static_cast<Actor*>(allocation);
    if (allocation) {
        string.set("\x8a\xf8\x83\x82\x83\x66\x83\x8b");
        actor = _ZN2al11MapObjActorC1ERKN4sead14SafeStringBaseIcEE(allocation, string);
    }
    self->model = actor;
    FormatString formatted;
    fn_00267F2C(self->model, initInfo, string.set(*fn_0027AD3C(&formatted, "%sFlag", name)), 0);
    _ZN2al8copyPoseEPNS_9LiveActorEPKS0_(self->model, self->source);
    if (appear) {
        self->model->vtable[4](self->model);
        _ZN2al11startActionEPNS_9LiveActorEPKc(self->model, "Wait");
    } else {
        self->model->vtable[6](self->model);
    }
}
