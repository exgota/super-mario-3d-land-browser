namespace {
struct IUseNerve;
struct Nerve;
}

extern "C" bool _ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(
    const IUseNerve*, const Nerve*);
extern "C" const Nerve dat_003F0354;

extern "C" bool fn_002556F8(const IUseNerve* self) {
    return _ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(
        self, &dat_003F0354);
}
