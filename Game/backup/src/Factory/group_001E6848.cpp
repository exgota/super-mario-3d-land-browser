namespace {
struct Actor {
    unsigned char pad[0x68];
    void* next;
};
}

extern "C" void _ZN2al9LiveActor8calcAnimEv(void*);
extern "C" void* fn_001E6860(void*);
extern "C" void fn_0012D370(void*);
extern "C" void* fn_0025F258(void*);

extern "C" void* fn_001E6848(Actor* self) {
    _ZN2al9LiveActor8calcAnimEv(self);
    return fn_001E6860(self->next);
}

extern "C" void* fn_0025F240(Actor* self) {
    fn_0012D370(self);
    return fn_0025F258(self->next);
}
