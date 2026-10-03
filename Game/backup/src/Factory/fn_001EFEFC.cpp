#include <cstdint>

extern "C" std::uint32_t fn_001EFEFC(std::uint32_t value, std::uint32_t bits)
{
    return ((value & ~0x00f00000u) << 8) | bits;
}
