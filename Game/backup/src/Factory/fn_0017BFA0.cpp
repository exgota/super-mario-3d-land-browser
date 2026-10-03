#include <cstdint>

namespace {
struct TargetFields {
    std::uint32_t pad[3];
    std::uint32_t first;
    std::uint32_t second;
};
}

extern "C" void fn_0017BFA0(void* object, std::uint32_t value) {
    TargetFields* fields = static_cast<TargetFields*>(object);
    fields->first = value;
    fields->second = value;
}
