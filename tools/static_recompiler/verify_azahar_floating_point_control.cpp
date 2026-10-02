#include <array>
#include <cfenv>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <vector>
#include "dynarmic/interface/A32/a32.h"

struct FloatingPointControlCallbacks final : Dynarmic::A32::UserCallbacks {
    std::array<std::uint32_t,4> instructions;
    Dynarmic::A32::Jit* processor = nullptr;
    std::uint64_t ticks = 64;
    std::uint64_t observed_floating_point_control = 0;
    unsigned fallbacks = 0;
    std::optional<std::uint32_t> MemoryReadCode(std::uint32_t address) override {
        if (address >= 0x1000 && address < 0x1010) return instructions[(address-0x1000)/4];
        return std::nullopt;
    }
    std::uint8_t MemoryRead8(std::uint32_t) override { throw std::runtime_error("unexpected read"); }
    std::uint16_t MemoryRead16(std::uint32_t) override { throw std::runtime_error("unexpected read"); }
    std::uint32_t MemoryRead32(std::uint32_t) override { throw std::runtime_error("unexpected read"); }
    std::uint64_t MemoryRead64(std::uint32_t) override { throw std::runtime_error("unexpected read"); }
    void MemoryWrite8(std::uint32_t,std::uint8_t) override { throw std::runtime_error("unexpected write"); }
    void MemoryWrite16(std::uint32_t,std::uint16_t) override { throw std::runtime_error("unexpected write"); }
    void MemoryWrite32(std::uint32_t,std::uint32_t) override { throw std::runtime_error("unexpected write"); }
    void MemoryWrite64(std::uint32_t,std::uint64_t) override { throw std::runtime_error("unexpected write"); }
    void InterpreterFallback(std::uint32_t,std::size_t) override { ++fallbacks; processor->HaltExecution(); }
    void CallSVC(std::uint32_t) override {
        fenv_t environment;
        if (fegetenv(&environment) != 0) throw std::runtime_error("host floating-point environment unavailable");
        observed_floating_point_control = environment.__fpcr;
        processor->HaltExecution();
    }
    void ExceptionRaised(std::uint32_t,Dynarmic::A32::Exception) override { throw std::runtime_error("guest exception"); }
    void AddTicks(std::uint64_t elapsed) override { ticks=elapsed>=ticks?0:ticks-elapsed; }
    std::uint64_t GetTicksRemaining() override { return ticks; }
};

int main(int argc,char** argv) {
    if(argc!=2) return 2;
    std::ifstream stream(argv[1],std::ios::binary);
    std::vector<char> code((std::istreambuf_iterator<char>(stream)),std::istreambuf_iterator<char>());
    if(code.size()!=3096576) return 3;
    const auto original=[&](std::uint32_t address) {
        std::uint32_t instruction; std::memcpy(&instruction,code.data()+address-0x100000,4); return instruction;
    };
    for(unsigned source_mode=0;source_mode<4;++source_mode) {
        for(unsigned target_mode=0;target_mode<4;++target_mode) {
            for(unsigned dimension : {240,320,400}) {
                for(bool split_run : {false,true}) {
                    FloatingPointControlCallbacks callbacks;
                    // Three original, register-only instructions composed into a diagnostic sequence.
                    // The original SVC is used only to end the diagnostic, without invoking game services.
                    callbacks.instructions={original(0x105208),original(0x38cff0),original(0x38cff4),original(0x101b28)};
                    Dynarmic::A32::UserConfig configuration;
                    configuration.callbacks=&callbacks;
                    configuration.arch_version=Dynarmic::A32::ArchVersion::v6K;
                    configuration.define_unpredictable_behaviour=true;
                    configuration.code_cache_size=8*1024*1024;
                    Dynarmic::A32::Jit processor(configuration);
                    callbacks.processor=&processor;
                    processor.Regs()[0]=0x03000000|(target_mode<<22);
                    processor.Regs()[15]=0x1000;
                    processor.SetCpsr(0x10);
                    processor.SetFpscr(0x03000000|(source_mode<<22));
                    float divisor=static_cast<float>(dimension);
                    std::memcpy(&processor.ExtRegs()[1],&divisor,4);
                    processor.ExtRegs()[17]=0x40000000;
                    if(split_run) processor.Step();
                    processor.Run();
                    std::printf("{\"initial_mode\":%u,\"requested_mode\":%u,\"dimension\":%u,\"split_run\":%s,\"result\":\"0x%08X\",\"fpscr\":\"0x%08X\",\"host_fpcr_at_svc\":\"0x%08llX\",\"fallbacks\":%u}\n",
                        source_mode,target_mode,dimension,split_run?"true":"false",processor.Regs()[0],processor.Fpscr(),
                        static_cast<unsigned long long>(callbacks.observed_floating_point_control),callbacks.fallbacks);
                }
            }
        }
    }
}
