// Native platform adapter for the public GPLv2-or-later Azahar interface.
#pragma once
#include <array>
#include <filesystem>
#include <vector>
#include "core/arm/arm_interface.h"
#include "GuestMemoryTrace.h"
#include "recomp.h"

namespace Kernel { class SVCContext; }
namespace Core { class System; }

namespace Port {
class StaticArmBackend final : public Core::ARM_Interface {
public:
    StaticArmBackend(Core::System& system, Memory::MemorySystem& memory, u32 id,
                     std::shared_ptr<Core::Timing::Timer> timer, const std::filesystem::path& library,
                     std::shared_ptr<GuestMemoryTrace> trace = nullptr);
    ~StaticArmBackend() override;
    void Run() override;
    void Step() override;
    void ClearInstructionCache() override;
    void InvalidateCacheRange(u32 address, std::size_t length) override;
    void ClearExclusiveState() override;
    void SetPageTable(const std::shared_ptr<Memory::PageTable>& table) override;
    void SetPC(u32 value) override;
    u32 GetPC() const override;
    u32 GetReg(int index) const override;
    void SetReg(int index, u32 value) override;
    u32 GetVFPReg(int index) const override;
    void SetVFPReg(int index, u32 value) override;
    u32 GetVFPSystemReg(VFPSystemRegister reg) const override;
    void SetVFPSystemReg(VFPSystemRegister reg, u32 value) override;
    u32 GetCPSR() const override;
    void SetCPSR(u32 value) override;
    u32 GetCP15Register(CP15Register reg) const override;
    void SetCP15Register(CP15Register reg, u32 value) override;
    void SaveContext(ThreadContext& output) override;
    void LoadContext(const ThreadContext& input) override;
    void PrepareReschedule() override;
    bool HasSingleInstructionBreakAccuracy() override { return false; }
    void ChargeBlock(u32 address, u32 instructions, u64 ticks);
    u64 InstructionsExecuted() const { return instructions_executed; }
protected:
    std::shared_ptr<Memory::PageTable> GetPageTable() const override;
private:
    static u8 Read8(Context*, u32);
    static u16 Read16(Context*, u32);
    static u32 Read32(Context*, u32);
    static void Write8(Context*, u32, u8);
    static void Write16(Context*, u32, u16);
    static void Write32(Context*, u32, u32);
    static void RefuseInterpretation(Context*, u32, u32);
    static Code Lookup(Context*, u32);
    Code FindCode(u32 address) const;
    Memory::MemorySystem& memory;
    std::unique_ptr<Kernel::SVCContext> svc;
    std::shared_ptr<Memory::PageTable> page_table;
    void* library = nullptr;
    const Entry* entries = nullptr;
    u32 entry_count = 0;
    Context context{};
    std::array<u32, 64> floating_registers{};
    std::array<u32, CP15_REGISTER_COUNT> coprocessor_registers{};
    u32 fpscr = 0, fpexc = 0, cpsr_control = 0x10;
    bool reschedule = false;
    std::vector<u8*> callback_pages;
    u64 instructions_executed = 0;
    u64 supervisor_ticks = 0;
    u32 current_instruction = 0;
    std::shared_ptr<GuestMemoryTrace> trace;
    static const Host callbacks;
};
}
