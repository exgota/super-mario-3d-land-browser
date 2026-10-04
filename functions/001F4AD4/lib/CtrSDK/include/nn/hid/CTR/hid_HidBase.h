#pragma once
#include "nn/os/os_EventBase.h"

namespace nn {
namespace hid {
namespace CTR {

    class HidBase : nn::os::EventBase {
        void *m_pResource;
    public:
        // Adapted accessor; reference variants expose void* storage or uptr API.
        void* GetResource() const { return m_pResource; }
    };

} // namespace CTR
} // namespace hid
} // namespace nn
