#include "TranslatedFunctionModule.h"
#include <stdexcept>

namespace Port {
void TranslatedFunctionModule::Validate() const {
    if (abi_revision != RECOMP_ABI)
        throw std::runtime_error("static CPU ABI disagreement");
    // NativeTiming.c exports revision 2; NativeTiming.h has no constant revision.
    if (timing_revision != 2)
        throw std::runtime_error("static CPU timing ABI disagreement");
    if (!entries)
        throw std::runtime_error("missing static CPU entry table");
    if (entry_count == 0 || entry_count > MaximumEntryCount)
        throw std::runtime_error("invalid static CPU entry count");
    if (!timing_callback)
        throw std::runtime_error("missing static CPU timing callback storage");
    for (std::uint32_t index = 0; index < entry_count; ++index) {
        if (!entries[index].code)
            throw std::runtime_error("missing static CPU entry function");
        if (index && entries[index - 1].address >= entries[index].address)
            throw std::runtime_error("invalid static entry table");
    }
}
}
