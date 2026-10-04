namespace {
struct Nerve;

struct State {
    unsigned char padding0[8];
    bool active;
    unsigned char padding9[23];
    unsigned int count;
};
}

extern "C" {
extern const Nerve dat_003F35A4;
void _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(State*, const Nerve*);

void fn_0015EFF0(State* self) {
    unsigned int count = self->count;
    self->active = false;
    self->count = count + 1;
    _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(self, &dat_003F35A4);
}
}
