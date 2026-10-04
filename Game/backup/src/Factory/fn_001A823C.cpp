#include <cstdint>

extern "C" void fn_001A823C(void* self)
{
    *reinterpret_cast<std::uint8_t*>(static_cast<std::uint8_t*>(self) + 6) = 1;
}
