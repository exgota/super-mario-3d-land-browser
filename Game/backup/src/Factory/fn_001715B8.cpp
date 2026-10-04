namespace {
struct NoteObjGenerator {
    unsigned char unknown[0x78];
    unsigned char active;
};
}

extern "C" void fn_001D5684(NoteObjGenerator*);
extern "C" void _ZN2al9LiveActor4killEv(NoteObjGenerator*);

extern "C" void fn_001715B8(NoteObjGenerator* self) {
    fn_001D5684(self);
    self->active = 0;
    _ZN2al9LiveActor4killEv(self);
}
