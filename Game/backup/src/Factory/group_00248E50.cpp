namespace {
struct Owner {
    unsigned char pad[0x24];
    void* target;
};
}

extern "C" void fn_001D83DC(void*, int, void*);
extern "C" void fn_001D839C(void*, int, void*);

extern "C" void fn_00248E50(Owner* self, void* arg) {
    fn_001D83DC(self->target, 1, arg);
}

extern "C" void fn_002490CC(Owner* self, void* arg) {
    fn_001D839C(self->target, 1, arg);
}
