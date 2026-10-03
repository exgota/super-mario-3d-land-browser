#include <cstdint>

extern "C" std::uint32_t fn_0033643C(std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t);

extern "C" std::uint32_t fn_00336434(std::uint32_t a0, std::uint32_t a1, std::uint32_t a2) {
    return fn_0033643C(a0, a1, a2, a0 + 0x20);
}
