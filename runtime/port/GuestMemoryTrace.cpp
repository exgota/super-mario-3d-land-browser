#include "GuestMemoryTrace.h"
#include <cstdlib>
#include <filesystem>
#include <sstream>
#include <stdexcept>

namespace Port {
std::shared_ptr<GuestMemoryTrace> GuestMemoryTrace::FromEnvironment() {
    const char* filename = std::getenv("ROOT_PORT_MEMORY_TRACE_PATH");
    const char* values = std::getenv("ROOT_PORT_MEMORY_TRACE_VALUES");
    if (!filename && !values) return nullptr;
    if (!filename || !values) throw std::runtime_error("memory tracing needs both path and value filters");
    return std::shared_ptr<GuestMemoryTrace>(new GuestMemoryTrace(filename, values));
}
GuestMemoryTrace::GuestMemoryTrace(const char* filename, const char* values) {
    if (std::filesystem::exists(filename)) throw std::runtime_error("memory trace output already exists");
    std::stringstream input(values);
    std::string value;
    while (std::getline(input, value, ',')) {
        std::size_t end;
        auto parsed = std::stoull(value, &end, 0);
        if (end != value.size() || parsed > UINT32_MAX) throw std::runtime_error("invalid memory trace value");
        selected_values.insert(static_cast<std::uint32_t>(parsed));
    }
    if (selected_values.empty()) throw std::runtime_error("memory trace filter is empty");
    output.open(filename);
    if (!output) throw std::runtime_error("cannot open memory trace output");
}
void GuestMemoryTrace::RecordWrite(std::uint32_t processor, std::uint32_t instruction, std::uint32_t address, std::uint32_t value, const Context& context) {
    if (!selected_values.contains(value)) return;
    output << "{\"processor\":" << processor << ",\"instruction\":" << instruction
           << ",\"address\":" << address << ",\"value\":" << value << ",\"registers\":[";
    for (unsigned index = 0; index < 16; ++index) output << (index ? "," : "") << context.r[index];
    output << "],\"floating_registers\":[";
    for (unsigned index = 0; index < 32; ++index) output << (index ? "," : "") << context.vfp[index];
    output << "],\"fpscr\":" << *context.fpscr << "}\n";
    output.flush();
    if (!output) throw std::runtime_error("cannot write memory trace output");
}
}
