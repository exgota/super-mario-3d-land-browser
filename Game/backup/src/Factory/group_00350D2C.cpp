extern "C" const char _ZTVN4sead14SafeStringBaseIcEE[];

namespace {
struct NerveContext { void* actor; };
struct SafeString {
    const void* vtable;
    const char* text;

    SafeString(const char* s) : text(s) {
        vtable = _ZTVN4sead14SafeStringBaseIcEE + 8;
    }
};
}

extern "C" bool _ZN2al11isFirstStepEPKNS_9IUseNerveE(const void*);
extern "C" void fn_00270CCC(void*, const SafeString&);
extern "C" const char dat_003B9680[];
extern "C" const char dat_003B7A1C[];
extern "C" const char dat_003B7A0C[];
extern "C" const char dat_003B7C24[];
extern "C" const char dat_003B7B24[];
extern "C" const char dat_003B7B9C[];
extern "C" const char dat_003C165C[];
extern "C" const char dat_003B857C[];
extern "C" const char dat_003B8570[];
extern "C" const char dat_003B8290[];
extern "C" const char dat_003C172C[];
extern "C" const char dat_003C16F4[];
extern "C" const char dat_003B7ACC[];
extern "C" const char dat_003B97C4[];
extern "C" const char dat_003B97CC[];
extern "C" const char dat_003B2AEC[];
extern "C" const char dat_003A2D60[];

extern "C" void fn_00350D2C(void*, const NerveContext* context)
{
    void* actor = context->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor)) {
        fn_00270CCC(actor, SafeString(dat_003B9680));
    }
}

extern "C" void fn_00355F98(void*, const NerveContext* context)
{
    void* actor = context->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor)) {
        fn_00270CCC(actor, SafeString(dat_003B7A1C));
    }
}

extern "C" void fn_00355FE0(void*, const NerveContext* context)
{
    void* actor = context->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor)) {
        fn_00270CCC(actor, SafeString(dat_003B7A0C));
    }
}

extern "C" void fn_00356130(void*, const NerveContext* context)
{
    void* actor = context->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor)) {
        fn_00270CCC(actor, SafeString(dat_003B7C24));
    }
}

extern "C" void fn_003576F8(void*, const NerveContext* context)
{
    void* actor = context->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor)) {
        fn_00270CCC(actor, SafeString(dat_003B7B24));
    }
}

extern "C" void fn_0035784C(void*, const NerveContext* context)
{
    void* actor = context->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor)) {
        fn_00270CCC(actor, SafeString(dat_003B7B9C));
    }
}

extern "C" void fn_0035A008(void*, const NerveContext* context)
{
    void* actor = context->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor)) {
        fn_00270CCC(actor, SafeString(dat_003C165C));
    }
}

extern "C" void fn_0035C2A8(void*, const NerveContext* context)
{
    void* actor = context->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor)) {
        fn_00270CCC(actor, SafeString(dat_003B857C));
    }
}

extern "C" void fn_0035C2F0(void*, const NerveContext* context)
{
    void* actor = context->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor)) {
        fn_00270CCC(actor, SafeString(dat_003B8570));
    }
}

extern "C" void fn_0035D674(void*, const NerveContext* context)
{
    void* actor = context->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor)) {
        fn_00270CCC(actor, SafeString(dat_003B8290));
    }
}

extern "C" void fn_0035DD34(void*, const NerveContext* context)
{
    void* actor = context->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor)) {
        fn_00270CCC(actor, SafeString(dat_003C172C));
    }
}

extern "C" void fn_00367634(void*, const NerveContext* context)
{
    void* actor = context->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor)) {
        fn_00270CCC(actor, SafeString(dat_003C16F4));
    }
}

extern "C" void fn_00367F28(void*, const NerveContext* context)
{
    void* actor = context->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor)) {
        fn_00270CCC(actor, SafeString(dat_003B7ACC));
    }
}

extern "C" void fn_00368D58(void*, const NerveContext* context)
{
    void* actor = context->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor)) {
        fn_00270CCC(actor, SafeString(dat_003B97C4));
    }
}

extern "C" void fn_00368E14(void*, const NerveContext* context)
{
    void* actor = context->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor)) {
        fn_00270CCC(actor, SafeString(dat_003B97CC));
    }
}

extern "C" void fn_003725F0(void*, const NerveContext* context)
{
    void* actor = context->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor)) {
        fn_00270CCC(actor, SafeString(dat_003B2AEC));
    }
}

extern "C" void fn_003736CC(void*, const NerveContext* context)
{
    void* actor = context->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor)) {
        fn_00270CCC(actor, SafeString(dat_003A2D60));
    }
}
