namespace {
struct NerveStateHolder {
    void* state;
};
}

extern "C" bool _ZN2al16updateNerveStateEPNS_9IUseNerveE(void*);
extern "C" void _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(void*, const void*);
extern "C" char dat_003F20E0;

extern "C" void fn_00347E64(void*, const NerveStateHolder* holder) {
    void* state = holder->state;
    if (_ZN2al16updateNerveStateEPNS_9IUseNerveE(state))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(state, &dat_003F20E0);
}
