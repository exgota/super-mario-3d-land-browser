namespace nn { namespace srv {
extern void Initialize();
extern void StartNotification();
}}
namespace nn { namespace os { namespace ARM { namespace detail {
extern void SaveThreadLocalRegionAddress();
}}}}
namespace nn { namespace os { namespace ThreadLocalStorage {
extern void ClearAllSlots();
}}}
namespace nn { namespace os { namespace CTR {
extern void SetupThreadCppExceptionEnvironment();
}}}
extern "C" void _fp_init();
extern "C" void fn_002913CC();
extern "C" void fn_0025BB50();
extern "C" void fn_00138F14();
extern "C" void fn_001F9DA0();
extern "C" void fn_001F2D24();
extern "C" void fn_001F32A0();
extern "C" void fn_001F2BD8();
extern "C" void fn_001F2D94();
extern "C" void fn_001F5F90();
extern "C" void fn_0025B5C0();
extern "C" void fn_0026C1C4();
extern "C" void fn_0026C268();
extern "C" void fn_0027BAD0();
extern "C" void fn_0027BB10();
extern "C" void fn_00326F48();
extern "C" void fn_003270A0();
extern "C" void fn_003271A0();

extern "C" void nninitSetupDefault() {
    nn::srv::Initialize();
    nn::srv::StartNotification();
    nn::os::ARM::detail::SaveThreadLocalRegionAddress();
}
namespace nn { namespace os { namespace detail {
void InitializeThreadEnvrionment() {
    nn::os::ThreadLocalStorage::ClearAllSlots();
    nn::os::CTR::SetupThreadCppExceptionEnvironment();
    _fp_init();
}
}}}
extern "C" void fn_00138F00() { fn_002913CC(); fn_0025BB50(); fn_00138F14(); }
extern "C" void fn_001F328C() { fn_001F9DA0(); fn_001F2D24(); fn_001F32A0(); }
extern "C" void fn_001F5F7C() { fn_001F2BD8(); fn_001F2D94(); fn_001F5F90(); }
extern "C" void fn_0025B5AC() { fn_002913CC(); fn_0025BB50(); fn_0025B5C0(); }
extern "C" void fn_0026C1B0() { fn_002913CC(); fn_0025BB50(); fn_0026C1C4(); }
extern "C" void fn_0026C254() { fn_002913CC(); fn_0025BB50(); fn_0026C268(); }
extern "C" void fn_0027BABC() { fn_002913CC(); fn_0025BB50(); fn_0027BAD0(); }
extern "C" void fn_0027BAFC() { fn_002913CC(); fn_0025BB50(); fn_0027BB10(); }
extern "C" void fn_00326F34() { fn_002913CC(); fn_0025BB50(); fn_00326F48(); }
extern "C" void fn_0032708C() { fn_002913CC(); fn_0025BB50(); fn_003270A0(); }
extern "C" void fn_0032718C() { fn_002913CC(); fn_0025BB50(); fn_003271A0(); }
