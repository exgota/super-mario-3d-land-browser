namespace {
struct RestoreState {
    unsigned int unknown;
    int commandList;
    int x;
    int y;
    int width;
    int height;
    signed char mode;
    bool hasBuffer;
    unsigned short padding;
    void* buffer;
    void* bufferManager;
    void* viewportManager;
};

struct StringStorage {
    unsigned int words[11];
};
}

extern "C" void fn_00248CEC(void*);
extern "C" void fn_0024A19C();
extern "C" void fn_00281360(int*, int*, int*, int*);
extern "C" void fn_001D4704(void*, void*, signed char);
extern "C" void fn_001E4370(void*, int, int, int, int);
extern "C" void fn_0028F6F0(int);
extern "C" void fn_001D477C(void*);
extern "C" void _ZN2al9StringTmpILi32EEC1EPKcz(StringStorage*, const char*, ...);

extern "C" void fn_00251028(RestoreState* self) {
    if (!self->mode) {
        if (self->hasBuffer)
            fn_00248CEC(self->buffer);
        fn_0024A19C();
    }
    fn_00281360(&self->x, &self->y, &self->width, &self->height);
    if (self->mode) {
        if (!self->hasBuffer)
            goto finish;
        fn_00248CEC(self->buffer);
    }
    if (self->hasBuffer)
        fn_001D4704(self->bufferManager, self->buffer, self->mode);
    if (!self->mode)
        fn_001E4370(self->viewportManager, self->x, self->y, self->width, self->height);
finish:
    fn_0028F6F0(self->commandList);
    if (self->hasBuffer)
        fn_001D477C(self->bufferManager);
    StringStorage temporary;
    _ZN2al9StringTmpILi32EEC1EPKcz(&temporary, "BindCmdListRestore[%d]\n", self->commandList);
}
