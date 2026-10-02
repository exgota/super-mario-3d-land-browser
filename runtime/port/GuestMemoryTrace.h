#pragma once
#include <cstdint>
#include <fstream>
#include <memory>
#include <set>
#include "recomp.h"

namespace Port {
// Optional diagnostic output. Values and output paths are supplied at runtime;
// the source contains no game-specific addresses or captured state.
class GuestMemoryTrace {
public:
    static std::shared_ptr<GuestMemoryTrace> FromEnvironment();
    void RecordWrite(std::uint32_t processor, std::uint32_t instruction, std::uint32_t address, std::uint32_t value, const Context& context);
private:
    GuestMemoryTrace(const char* filename, const char* values);
    std::ofstream output;
    std::set<std::uint32_t> selected_values;
};
}
