namespace {
struct Status {
    char padding[8];
    bool active;
};

struct Actor {
    char padding0[0x150];
    void* component150;
    void* component154;
    void* component158;
    char padding15c[4];
    Status* status;
    char padding164[0x28];
    int counter18c;
    int counter190;
};
}

extern "C" {
void fn_0037575C(Actor*);
void fn_0027B1C0(void*);
bool fn_001CD64C(int);
void fn_00291510();
bool _ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(const Actor*, const void*);
bool fn_0032B85C(Status*);
bool fn_001DB5D4(int);
void fn_00273944(void*);
void fn_001E0E14(void*, int);
void fn_001A6FD4();
bool fn_00328F50(Actor*);
void _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(Actor*, const void*);
extern char dat_003EF560;
__attribute__((weak)) int factory_00163810_thresholdFactor();
}

extern "C" void fn_00163810(Actor* self) {
    fn_0037575C(self);
    fn_0027B1C0(self->component150);
    if (fn_001CD64C(0)) {
        ++self->counter190;
        const int factor = factory_00163810_thresholdFactor();
        const int count = self->counter190;
        const int factor31 = (factor << 5) - factor;
        const int factor225 = (factor << 8) - factor31;
        if (count > (factor225 << 3)) {
            fn_00291510();
            self->counter190 = 0;
        }
    } else {
        self->counter190 = 0;
    }

    if (_ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(self, &dat_003EF560))
        return;
    if (!self->status->active && !fn_0032B85C(self->status))
        return;
    if (self->counter18c <= 0) {
        if (!fn_001DB5D4(0))
            return;
        fn_00273944(self->component154);
        fn_001E0E14(self->component158, 0);
        fn_001A6FD4();
    }
    if (fn_00328F50(self))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(self, &dat_003EF560);
    ++self->counter18c;
}

extern "C" __attribute__((weak)) int factory_00163810_thresholdFactor() {
    return 6;
}
