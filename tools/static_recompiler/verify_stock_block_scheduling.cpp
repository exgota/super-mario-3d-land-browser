// Observe the pinned stock CPU independently of native scheduling metadata.
// Guest instructions come exclusively from the caller's approved local code.bin.
// Each stdin request contains four little-endian uint32 fields: tagged address,
// tick budget, initial CPSR, initial FPSCR. Output contains scalar JSON only.
// Data memory and registers are synthetic zero-filled fixtures. This program
// establishes bounded scheduling contracts, not original game-state replay.
#include <array>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iterator>
#include <optional>
#include <stdexcept>
#include <vector>

#include "core/arm/dynarmic/arm_tick_counts.h"
#include "dynarmic/interface/A32/a32.h"

namespace {
constexpr std::uint32_t OriginalCodeAddress = 0x00100000;
constexpr std::size_t OriginalCodeSize = 3096576;
constexpr std::uint32_t MaximumTickBudget = 1000000;
constexpr std::size_t MaximumRequests = 64;

std::uint32_t ReadLittleEndian(const unsigned char* bytes) {
    return std::uint32_t(bytes[0]) | (std::uint32_t(bytes[1]) << 8) |
           (std::uint32_t(bytes[2]) << 16) | (std::uint32_t(bytes[3]) << 24);
}

class StockSchedulingCallbacks final : public Dynarmic::A32::UserCallbacks {
public:
    explicit StockSchedulingCallbacks(const std::vector<unsigned char>& image,
                                      std::uint64_t budget)
        : code(image), remaining_ticks(budget) {}

    std::optional<std::uint32_t> MemoryReadCode(std::uint32_t address) override {
        if (address < OriginalCodeAddress ||
            std::uint64_t(address) + 4 > OriginalCodeAddress + code.size())
            return std::nullopt;
        return ReadLittleEndian(code.data() + address - OriginalCodeAddress);
    }
    std::uint8_t MemoryRead8(std::uint32_t) override { return 0; }
    std::uint16_t MemoryRead16(std::uint32_t) override { return 0; }
    std::uint32_t MemoryRead32(std::uint32_t) override { return 0; }
    std::uint64_t MemoryRead64(std::uint32_t) override { return 0; }
    void MemoryWrite8(std::uint32_t, std::uint8_t) override {}
    void MemoryWrite16(std::uint32_t, std::uint16_t) override {}
    void MemoryWrite32(std::uint32_t, std::uint32_t) override {}
    void MemoryWrite64(std::uint32_t, std::uint64_t) override {}
    void InterpreterFallback(std::uint32_t, std::size_t) override {
        ++fallback_count;
        processor->HaltExecution();
    }
    void CallSVC(std::uint32_t) override {
        ++supervisor_count;
        supervisor_prior_charge = charged_ticks;
    }
    void ExceptionRaised(std::uint32_t, Dynarmic::A32::Exception) override {
        throw std::runtime_error("stock scheduling probe raised an exception");
    }
    void AddTicks(std::uint64_t ticks) override {
        charged_ticks += ticks;
        remaining_ticks = ticks >= remaining_ticks ? 0 : remaining_ticks - ticks;
    }
    std::uint64_t GetTicksRemaining() override { return remaining_ticks; }
    std::uint64_t GetTicksForCode(bool thumb, std::uint32_t,
                                std::uint32_t instruction) override {
        return Core::TicksForInstruction(thumb, instruction);
    }

    Dynarmic::A32::Jit* processor = nullptr;
    std::uint64_t charged_ticks = 0;
    std::uint64_t supervisor_prior_charge = 0;
    std::uint32_t supervisor_count = 0;
    std::uint32_t fallback_count = 0;

private:
    const std::vector<unsigned char>& code;
    std::uint64_t remaining_ticks;
};

void Observe(const std::vector<unsigned char>& code,
             const std::array<unsigned char, 16>& request) {
    const auto tagged_address = ReadLittleEndian(request.data());
    const auto address = tagged_address & ~std::uint32_t(1);
    const auto budget = ReadLittleEndian(request.data() + 4);
    const auto status = ReadLittleEndian(request.data() + 8);
    const auto floating_point_status = ReadLittleEndian(request.data() + 12);
    if (address < OriginalCodeAddress ||
        std::uint64_t(address) + 4 > OriginalCodeAddress + code.size() ||
        (!(tagged_address & 1) && (address & 3)) ||
        ((status >> 5) & 1) != (tagged_address & 1) ||
        budget == 0 || budget > MaximumTickBudget)
        throw std::runtime_error("invalid stock scheduling request");

    StockSchedulingCallbacks callbacks(code, budget);
    Dynarmic::A32::UserConfig configuration{};
    configuration.callbacks = &callbacks;
    configuration.define_unpredictable_behaviour = true;
    configuration.code_cache_size = 8 * 1024 * 1024;
    Dynarmic::A32::Jit processor(configuration);
    callbacks.processor = &processor;
    processor.Regs().fill(0);
    processor.ExtRegs().fill(0);
    processor.Regs()[15] = address;
    processor.SetCpsr(status);
    processor.SetFpscr(floating_point_status);
    processor.Run();
    if (callbacks.fallback_count)
        throw std::runtime_error("stock scheduling probe required interpreter fallback");

    std::printf("{\"address\":%u,\"budget\":%u,\"initial_status\":%u,"
                "\"initial_fpscr\":%u,\"next_pc\":%u,\"charged_ticks\":%llu,"
                "\"supervisors\":%u,\"supervisor_prior_charge\":%llu,\"fallbacks\":%u}\n",
                tagged_address, budget, status, floating_point_status, processor.Regs()[15],
                static_cast<unsigned long long>(callbacks.charged_ticks),
                callbacks.supervisor_count,
                static_cast<unsigned long long>(callbacks.supervisor_prior_charge),
                callbacks.fallback_count);
}
} // namespace

int main(int count, char** values) {
    try {
        if (count != 2)
            throw std::runtime_error("usage: verify_stock_block_scheduling approved-code.bin < requests.bin");
        std::ifstream stream(values[1], std::ios::binary);
        if (!stream)
            throw std::runtime_error("cannot open original code image");
        const std::vector<unsigned char> code{std::istreambuf_iterator<char>(stream),
                                             std::istreambuf_iterator<char>()};
        if (code.size() != OriginalCodeSize)
            throw std::runtime_error("original code image extent differs");

        std::array<unsigned char, 16> request{};
        std::size_t request_count = 0;
        for (;;) {
            const auto size = std::fread(request.data(), 1, request.size(), stdin);
            if (size == 0 && !std::ferror(stdin)) break;
            if (size != request.size())
                throw std::runtime_error("incomplete stock scheduling request");
            if (++request_count > MaximumRequests)
                throw std::runtime_error("stock scheduling request limit exceeded");
            Observe(code, request);
        }
        return 0;
    } catch (const std::exception& error) {
        std::fprintf(stderr, "%s\n", error.what());
        return 1;
    }
}
