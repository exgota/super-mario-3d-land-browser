// Thread priority conversion between the kernel (svc) range and the library range.
// Reconstructed from the retail binary at 0x0010766C.

namespace nn { namespace os { namespace detail {

s32 ConvertSvcToLibraryPriority(s32 priority)
{
    if (priority >= 32)
    {
        return priority - 32;
    }

    s32 base;
    if (priority >= 24)
    {
        base = 0x5109D500;
        priority -= 24;
    }
    else
    {
        base = 0x6C8DA500;
    }
    return priority + base;
}

}}}
