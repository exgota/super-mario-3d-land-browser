namespace {
extern "C" bool _ZN2al11isFirstStepEPKNS_9IUseNerveE(const void*);
extern "C" void fn_0027BEA0(void*, const void*, int);
extern "C" const unsigned int dat_003BD994;
}

extern "C" void fn_0035EB5C(void*, const void** nervePtr) {
    const void* nerve = *nervePtr;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(nerve))
        fn_0027BEA0(static_cast<char*>(const_cast<void*>(nerve)) + 4,
                    &dat_003BD994, 0);
}
