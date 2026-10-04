extern "C" void fn_00293480();

namespace nn {
namespace os {
extern unsigned int GetAppMemorySize();
extern unsigned int GetUsingMemorySize();
extern void SetDeviceMemorySize(unsigned int);
}
}

extern "C" void nninitStartUp()
{
    fn_00293480();
    unsigned int appMemorySize = nn::os::GetAppMemorySize();
    unsigned int usingMemorySize = nn::os::GetUsingMemorySize();
    return nn::os::SetDeviceMemorySize(appMemorySize - usingMemorySize);
}
