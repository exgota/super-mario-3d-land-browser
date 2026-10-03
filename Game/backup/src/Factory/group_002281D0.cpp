namespace al {
extern void* getSceneObj(int);
}
namespace {
extern "C" int fn_0011158C(int);
extern "C" int _ZN2nn3svc5BreakENS_3dbg11BreakReasonE(int);
extern "C" int __rt_SIGPVFN(int);
}

extern "C" void* fn_002281D0() {
    return al::getSceneObj(0);
}

extern "C" int fn_002877F4() {
    return fn_0011158C(0);
}

extern "C" int nndbgPanic() {
    return _ZN2nn3svc5BreakENS_3dbg11BreakReasonE(0);
}

extern "C" int __cxa_pure_virtual() {
    return __rt_SIGPVFN(0);
}
