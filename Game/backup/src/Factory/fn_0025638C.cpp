#include <cstdint>

extern "C" std::uint32_t fn_00256394(void* self, std::uint32_t value);

extern "C" std::uint32_t fn_0025638C(void* self, std::uint32_t value) {
    *reinterpret_cast<std::uint32_t*>(static_cast<unsigned char*>(self) + 0x34) = value;
    return fn_00256394(self, value);
}
