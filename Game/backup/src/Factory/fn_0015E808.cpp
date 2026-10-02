namespace {
struct State {
    char padding[0xa4];
    int count;
    int capacity;
};
struct Actor;
}

extern "C" const void* _ZN2al8getTransEPKNS_9LiveActorE(const Actor*);
extern "C" void fn_002CD200(State*, const void*, const void*);

extern "C" bool fn_0015E808(State* self, const Actor* actor, const void* value) {
    if (self->count >= self->capacity)
        return false;
    fn_002CD200(self, _ZN2al8getTransEPKNS_9LiveActorE(actor), value);
    ++self->count;
    return true;
}
