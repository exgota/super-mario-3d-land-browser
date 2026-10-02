// Typed binding for generated functions. This descriptor does not own storage.
#pragma once
#include <cstdint>
#include "NativeTiming.h"

namespace Port {
struct TranslatedFunctionModule {
    static constexpr std::uint32_t MaximumEntryCount = 1u << 20;

    std::uint32_t abi_revision = 0;
    std::uint32_t timing_revision = 0;
    const Entry* entries = nullptr;
    std::uint32_t entry_count = 0;
    NativeBlockTimingCallback* timing_callback = nullptr;

    // Entries and callback storage must remain valid throughout backend use.
    // Entries must be immutable; the backend installs its timing callback.
    void Validate() const;
};
}
