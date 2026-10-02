extern "C" {
extern int _ZN2nn3cfg3CTR6detail10InitializeEv();
extern int _ZN2nn3ndm19SetupDaemonsDefaultEv();
extern int _ZN2nn3ndm3CTR6detail9Interface16SuspendSchedulerEb(bool);
extern int fn_0010415C(unsigned int);
extern int fn_001056A0(void*, unsigned int, unsigned int);
extern int _ZN2nn5gxlow3CTR10InitializeEv();
extern int _ZN2nn5gxlow3CTR11YieldThreadEv();
extern int _ZN2nn5gxlow3CTR19WriteHWRegsWithMaskEjPKvS3_j();
extern int _ZN2nn5gxlow3CTR21IsFirstInitializationEv();
extern int _ZN2nn5gxlow3CTR24RegisterInterruptHandlerEPFvvE16nngxlowInterrupt();
extern int _ZN2nn5gxlow3CTR25GetNumSpeculativeRequestsEv();
extern int _ZN2nn5gxlow3CTR10ReadHWRegsEjPvj();
extern int _ZN2nn5gxlow3CTR14SetCommandlistEPvjbb();
extern int _ZN2nn5gxlow3CTR15GetPhysicalAddrEj();
extern wchar_t* wcsncpy(wchar_t*, const wchar_t*, unsigned int);
extern int fn_00118C84();
extern int fn_00118CEC();
extern int fn_00118D74();
extern int fn_00118DA4();
extern int fn_0013E238();
extern int fn_001492AC();
extern int fn_0015C95C();
extern int fn_0015C9D4();
}

namespace nn {
namespace cfg { namespace CTR {
int Initialize() { return _ZN2nn3cfg3CTR6detail10InitializeEv(); }
namespace detail {
int GetConfig(void* buffer, unsigned int size, unsigned int id) {
    return fn_001056A0(buffer, size, id);
}
}
} }
namespace ndm {
int SuspendScheduler(bool suspend) {
    return _ZN2nn3ndm3CTR6detail9Interface16SuspendSchedulerEb(suspend);
}
int OverrideDefaultDaemons(unsigned int daemons) {
    return fn_0010415C(daemons);
}
}
}

namespace sead {
class Heap;
class ThreadMgr {
public:
    void initialize(Heap*);
    void initMainThread_(Heap*);
};
void ThreadMgr::initialize(Heap* heap) { return initMainThread_(heap); }
}

extern "C" {
int nninitSetupDaemons() { return _ZN2nn3ndm19SetupDaemonsDefaultEv(); }
int nngxlowInitialize() { return _ZN2nn5gxlow3CTR10InitializeEv(); }
int nngxlowYieldThread() { return _ZN2nn5gxlow3CTR11YieldThreadEv(); }
int nngxlowWriteHWRegsWithMask() {
    return _ZN2nn5gxlow3CTR19WriteHWRegsWithMaskEjPKvS3_j();
}
int nngxlowIsFirstInitialization() {
    return _ZN2nn5gxlow3CTR21IsFirstInitializationEv();
}
int nngxlowRegisterInterruptHandler() {
    return _ZN2nn5gxlow3CTR24RegisterInterruptHandlerEPFvvE16nngxlowInterrupt();
}
int nngxlowGetNumSpeculativeRequests() {
    return _ZN2nn5gxlow3CTR25GetNumSpeculativeRequestsEv();
}
int nngxlowReadHWRegs() { return _ZN2nn5gxlow3CTR10ReadHWRegsEjPvj(); }
int nngxlowSetCommandlistEx() { return _ZN2nn5gxlow3CTR14SetCommandlistEPvjbb(); }
int nngxGetPhysicalAddr() { return _ZN2nn5gxlow3CTR15GetPhysicalAddrEj(); }
wchar_t* fn_0010C740(wchar_t* dst, const wchar_t* src, unsigned int count) {
    return wcsncpy(dst, src, count);
}
int fn_00118C80() { return fn_00118C84(); }
int fn_00118CE8() { return fn_00118CEC(); }
int fn_00118D70() { return fn_00118D74(); }
int fn_00118DA0() { return fn_00118DA4(); }
int fn_0013E234() { return fn_0013E238(); }
int fn_001492A8() { return fn_001492AC(); }
int fn_0015C958() { return fn_0015C95C(); }
int fn_0015C9D0() { return fn_0015C9D4(); }
}
