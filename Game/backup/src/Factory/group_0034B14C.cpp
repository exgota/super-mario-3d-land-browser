namespace {
struct Object;
struct VTable {
    void* slots[5];
    void (*action)(Object*);
};
struct Object {
    VTable* vtable;
};
struct State {
    Object* object;
};
}

extern "C" bool _ZN2al11isFirstStepEPKNS_9IUseNerveE(const Object*);
extern "C" bool _ZN2al16updateNerveStateEPNS_9IUseNerveE(Object*);
extern "C" bool _ZN2al11isActionEndEPKNS_9LiveActorE(const Object*);

#define BODY(name, predicate) \
extern "C" void name(void*, State* state) { \
    Object* object = state->object; \
    if (predicate(object)) \
        object->vtable->action(object); \
}

BODY(fn_0034B14C, _ZN2al11isFirstStepEPKNS_9IUseNerveE)
BODY(fn_0034B17C, _ZN2al11isFirstStepEPKNS_9IUseNerveE)
BODY(fn_0034E248, _ZN2al16updateNerveStateEPNS_9IUseNerveE)
BODY(fn_0034E280, _ZN2al11isFirstStepEPKNS_9IUseNerveE)
BODY(fn_0034E2B0, _ZN2al11isFirstStepEPKNS_9IUseNerveE)
BODY(fn_0035A1A4, _ZN2al16updateNerveStateEPNS_9IUseNerveE)
BODY(fn_0035A7A0, _ZN2al11isFirstStepEPKNS_9IUseNerveE)
BODY(fn_0035C3AC, _ZN2al16updateNerveStateEPNS_9IUseNerveE)
BODY(fn_0035C62C, _ZN2al11isFirstStepEPKNS_9IUseNerveE)
BODY(fn_0035C65C, _ZN2al11isFirstStepEPKNS_9IUseNerveE)
BODY(fn_0035D804, _ZN2al11isFirstStepEPKNS_9IUseNerveE)
BODY(fn_00360658, _ZN2al11isFirstStepEPKNS_9IUseNerveE)
BODY(fn_003606E0, _ZN2al16updateNerveStateEPNS_9IUseNerveE)
BODY(fn_00360710, _ZN2al11isFirstStepEPKNS_9IUseNerveE)
BODY(fn_00360750, _ZN2al11isFirstStepEPKNS_9IUseNerveE)
BODY(fn_003607A0, _ZN2al16updateNerveStateEPNS_9IUseNerveE)
BODY(fn_00360A80, _ZN2al11isFirstStepEPKNS_9IUseNerveE)
BODY(fn_00360B08, _ZN2al16updateNerveStateEPNS_9IUseNerveE)
BODY(fn_00360B38, _ZN2al11isFirstStepEPKNS_9IUseNerveE)
BODY(fn_00360B78, _ZN2al11isFirstStepEPKNS_9IUseNerveE)
BODY(fn_00360BC8, _ZN2al16updateNerveStateEPNS_9IUseNerveE)
BODY(fn_00362AB0, _ZN2al11isActionEndEPKNS_9LiveActorE)
BODY(fn_003650FC, _ZN2al11isFirstStepEPKNS_9IUseNerveE)

#undef BODY
