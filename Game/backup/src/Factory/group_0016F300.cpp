namespace {
struct Nerve;
struct NerveUser {
    unsigned char unknown[8];
    bool flag;
};
}

extern "C" void _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(NerveUser*, const Nerve*);
extern "C" Nerve dat_003F3360;
extern "C" Nerve dat_003F3564;
extern "C" Nerve dat_003F30D8;

extern "C" void fn_0016F300(NerveUser* self) {
    _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(self, &dat_003F3360);
    self->flag = false;
}

extern "C" void fn_00189C8C(NerveUser* self) {
    _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(self, &dat_003F3564);
    self->flag = false;
}

extern "C" void fn_0018B5AC(NerveUser* self) {
    _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(self, &dat_003F30D8);
    self->flag = false;
}
