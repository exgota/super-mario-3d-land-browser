#include <nn/init/init_StartUp.h>
#include <nn/os/CTR/os_ErrorHandler.h>
#include <nn/applet/CTR/applet_Initialize.h>

extern "C" void nninitSetup() {
    nninitSetupDefault();
    nn::os::CTR::detail::SetInternalErrorHandlingMode(false);
    nn::applet::CTR::detail::Initialize(0);
    nninitSetupDaemons();
}
