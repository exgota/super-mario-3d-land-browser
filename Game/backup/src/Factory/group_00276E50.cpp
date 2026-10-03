namespace {
struct IUseNerve;
struct Nerve;
}

extern "C" bool al_isNerve(IUseNerve const*, Nerve const*)
    __asm("_ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE");

extern "C" {
extern unsigned char dat_003EFF78[];
extern unsigned char dat_003F2940[];
bool fn_00276E50(void*);
bool fn_00326834(void*);
}

bool fn_00276E50(void* useNerve) {
    return !al_isNerve(static_cast<IUseNerve const*>(useNerve),
                       reinterpret_cast<Nerve const*>(dat_003EFF78));
}

bool fn_00326834(void* useNerve) {
    return !al_isNerve(static_cast<IUseNerve const*>(useNerve),
                       reinterpret_cast<Nerve const*>(dat_003F2940));
}
