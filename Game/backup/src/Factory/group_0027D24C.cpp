extern "C" void _ZN2al9LiveActor13makeActorDeadEv(void*);
extern "C" void _ZN2nn5gxlow3CTR6UnlockEv();
extern "C" void _ZN2nn3svc11ExitProcessEv();
extern "C" void _ZN4sead21ControllerWrapperBaseD1Ev(void*);

#define TAIL_WRAPPER(name, target) \
    extern "C" void target(void*); \
    extern "C" void name(void* self) { return target(self); }

extern "C" void fn_0027D24C(void* self) {
    return _ZN2al9LiveActor13makeActorDeadEv(self);
}
TAIL_WRAPPER(fn_00280414, fn_00280418)
TAIL_WRAPPER(fn_002877F0, fn_002877F4)
extern "C" void nngxlowUnlock() { return _ZN2nn5gxlow3CTR6UnlockEv(); }
TAIL_WRAPPER(fn_0028A814, fn_0028A818)
TAIL_WRAPPER(fn_0028EBCC, fn_0028EBD0)

namespace nn {
namespace dbg {
enum BreakReason { BreakReason_Default = 0 };
}
}
extern "C" void _ZN2nn3svc5BreakENS_3dbg11BreakReasonE(nn::dbg::BreakReason);
namespace nn {
namespace dbg {
void Break(BreakReason reason) {
    return _ZN2nn3svc5BreakENS_3dbg11BreakReasonE(reason);
}
}
}

extern "C" void fn_00293480();
extern "C" void nninitSystem() { return fn_00293480(); }

namespace _GLOBAL__N__16_init_Default_cpp_c3fc63fe {
class ExitHandler {
public:
    void HandleNotification(unsigned int);
};
void ExitHandler::HandleNotification(unsigned int) {
    return _ZN2nn3svc11ExitProcessEv();
}
}

TAIL_WRAPPER(fn_00296048, fn_0029604C)
TAIL_WRAPPER(fn_00297020, fn_00297024)
TAIL_WRAPPER(fn_0029BE9C, fn_0029BEA0)
TAIL_WRAPPER(fn_002BE054, fn_002BE058)
TAIL_WRAPPER(fn_002C16D4, fn_002C16D8)
TAIL_WRAPPER(fn_002C31D0, fn_002C31D4)
TAIL_WRAPPER(fn_002C623C, fn_002C6240)
TAIL_WRAPPER(fn_002CA4A4, fn_002CA4A8)
TAIL_WRAPPER(fn_002CC83C, fn_002CC840)
TAIL_WRAPPER(fn_002D1950, fn_002D1954)
TAIL_WRAPPER(fn_002D5664, fn_002D5668)
TAIL_WRAPPER(fn_002DAB00, fn_002DAB04)
TAIL_WRAPPER(fn_002DBF74, fn_002DBF78)
extern "C" void fn_002DF7C8(void* self) {
    return _ZN4sead21ControllerWrapperBaseD1Ev(self);
}
TAIL_WRAPPER(fn_002EEB6C, fn_002EEB70)
TAIL_WRAPPER(fn_002F188C, fn_002F1890)

#undef TAIL_WRAPPER
