// Derive generation-time metadata with the pinned public Dynarmic frontend.
// Link the unchanged Azahar arm_tick_counts.cpp implementation for its costs.
// No CPU backend, executable code generation, or original opcode output is used.
#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iterator>
#include <limits>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

#include "core/arm/dynarmic/arm_tick_counts.h"
#include "dynarmic/frontend/A32/a32_ir_emitter.h"
#include "dynarmic/frontend/A32/a32_location_descriptor.h"
#include "dynarmic/frontend/A32/translate/a32_translate.h"
#include "dynarmic/frontend/A32/translate/translate_callbacks.h"
#include "dynarmic/interface/A32/config.h"
#include "dynarmic/ir/basic_block.h"
#include "dynarmic/ir/opcodes.h"

namespace {
constexpr std::uint32_t OriginalCodeAddress = 0x00100000;
constexpr std::size_t OriginalCodeSize = 3096576;
constexpr std::uint32_t UnsupportedTerminal = 0x80000000;
constexpr std::uint32_t SupervisorCallFlag = 0x40000000;
constexpr std::uint32_t VectorModeMask = 0x00370000;
constexpr std::uint32_t AbsentNode = 0xffffffff;

// Low bits describe the actual root terminal. The high bit requires refusal.
enum class TerminalKind : std::uint32_t {
    Invalid = 0,
    CheckedLink = 1,
    UncheckedForwardLink = 2,
    ReturnStackPop = 3,
    ReturnToDispatcher = 4,
    FastDispatch = 5,
    CheckHalt = 6,
    ConditionalFlags = 8,
    ConditionalCheckBit = 9,
    Interpreter = 10,
};

// Version two: eighteen prefix words followed by eight words per local node.
// All descriptor low/high halves are serialized little endian explicitly.
struct TerminalNode {
    std::uint32_t kind = 0;
    std::uint32_t condition = AbsentNode;
    std::uint64_t next_descriptor = 0;
    std::uint32_t then_index = AbsentNode;
    std::uint32_t else_index = AbsentNode;
    std::uint32_t interpreter_count = 0;
};

struct BlockSchedule {
    std::array<std::uint32_t, 18> prefix;
    std::vector<TerminalNode> nodes;
};

std::uint32_t ReadLittleEndian(const unsigned char* data) {
    return std::uint32_t(data[0]) | (std::uint32_t(data[1]) << 8) |
           (std::uint32_t(data[2]) << 16) | (std::uint32_t(data[3]) << 24);
}

std::uint32_t Narrow(std::uint64_t value) {
    if (value > std::numeric_limits<std::uint32_t>::max())
        throw std::runtime_error("metadata exceeds the field width");
    return static_cast<std::uint32_t>(value);
}

std::uint32_t TaggedAddress(const Dynarmic::IR::LocationDescriptor& descriptor) {
    const Dynarmic::A32::LocationDescriptor location{descriptor};
    return location.PC() | std::uint32_t(location.TFlag());
}

bool DescriptorRepresentable(const Dynarmic::IR::LocationDescriptor& descriptor) {
    const Dynarmic::A32::LocationDescriptor location{descriptor};
    return (location.CPSR().Value() & ~std::uint32_t(0x20)) == 0 &&
           !(location.FPSCR().Value() & VectorModeMask) && !location.SingleStepping();
}

class TranslationCallbacks final : public Dynarmic::A32::TranslateCallbacks {
public:
    explicit TranslationCallbacks(const std::vector<unsigned char>& image) : code(image) {}

    std::optional<std::uint32_t> MemoryReadCode(std::uint32_t address) override {
        if ((address & 3) || address < OriginalCodeAddress ||
            std::uint64_t(address) + 4 > OriginalCodeAddress + code.size())
            return std::nullopt;
        return ReadLittleEndian(code.data() + address - OriginalCodeAddress);
    }

    bool PreCodeReadHook(bool, std::uint32_t, Dynarmic::A32::IREmitter&) override {
        if (translated_addresses.size() >= 65536)
            throw std::runtime_error("block exceeds the bounded translation limit");
        return true;
    }

    void PreCodeTranslationHook(bool, std::uint32_t address,
                                Dynarmic::A32::IREmitter&) override {
        translated_addresses.push_back(address);
    }

    std::uint64_t GetTicksForCode(bool is_thumb, std::uint32_t,
                                 std::uint32_t instruction) override {
        return Core::TicksForInstruction(is_thumb, instruction);
    }

    std::vector<std::uint32_t> translated_addresses;

private:
    const std::vector<unsigned char>& code;
};

TerminalKind RootKind(const Dynarmic::IR::Terminal& terminal) {
    return boost::apply_visitor([](const auto& node) -> TerminalKind {
        using Type = std::decay_t<decltype(node)>;
        using namespace Dynarmic::IR::Term;
        if constexpr (std::is_same_v<Type, LinkBlock>) return TerminalKind::CheckedLink;
        if constexpr (std::is_same_v<Type, LinkBlockFast>) return TerminalKind::UncheckedForwardLink;
        if constexpr (std::is_same_v<Type, PopRSBHint>) return TerminalKind::ReturnStackPop;
        if constexpr (std::is_same_v<Type, ReturnToDispatch>) return TerminalKind::ReturnToDispatcher;
        if constexpr (std::is_same_v<Type, FastDispatchHint>) return TerminalKind::FastDispatch;
        if constexpr (std::is_same_v<Type, CheckHalt>) return TerminalKind::CheckHalt;
        if constexpr (std::is_same_v<Type, If>) return TerminalKind::ConditionalFlags;
        if constexpr (std::is_same_v<Type, CheckBit>) return TerminalKind::ConditionalCheckBit;
        if constexpr (std::is_same_v<Type, Interpret>) return TerminalKind::Interpreter;
        return TerminalKind::Invalid;
    }, terminal);
}

bool TerminalRepresentable(const Dynarmic::IR::Terminal& terminal) {
    return boost::apply_visitor([&](const auto& node) -> bool {
        using Type = std::decay_t<decltype(node)>;
        using namespace Dynarmic::IR::Term;
        if constexpr (std::is_same_v<Type, LinkBlock> || std::is_same_v<Type, LinkBlockFast>) {
            return DescriptorRepresentable(node.next);
        } else if constexpr (std::is_same_v<Type, CheckHalt>) {
            return TerminalRepresentable(node.else_);
        } else if constexpr (std::is_same_v<Type, If>) {
            return TerminalRepresentable(node.then_) && TerminalRepresentable(node.else_);
        } else if constexpr (std::is_same_v<Type, PopRSBHint> ||
                             std::is_same_v<Type, ReturnToDispatch> ||
                             std::is_same_v<Type, FastDispatchHint>) {
            return true;
        }
        // CheckBit needs native internal-bit state. Interpreter and invalid refuse.
        return false;
    }, terminal);
}

std::uint32_t FlattenTerminal(const Dynarmic::IR::Terminal& terminal,
                              std::vector<TerminalNode>& nodes) {
    if (nodes.size() >= 128) throw std::runtime_error("terminal tree exceeds bounded node count");
    const auto index = Narrow(nodes.size());
    nodes.emplace_back();
    nodes[index].kind = static_cast<std::uint32_t>(RootKind(terminal));
    boost::apply_visitor([&](const auto& node) {
        using Type = std::decay_t<decltype(node)>;
        using namespace Dynarmic::IR::Term;
        if constexpr (std::is_same_v<Type, LinkBlock> ||
                      std::is_same_v<Type, LinkBlockFast> || std::is_same_v<Type, Interpret>)
            nodes[index].next_descriptor = node.next.Value();
        if constexpr (std::is_same_v<Type, Interpret>)
            nodes[index].interpreter_count = Narrow(node.num_instructions);
        if constexpr (std::is_same_v<Type, If>)
            nodes[index].condition = static_cast<std::uint32_t>(node.if_);
        if constexpr (std::is_same_v<Type, If> || std::is_same_v<Type, CheckBit>) {
            const auto child = FlattenTerminal(node.then_, nodes);
            nodes[index].then_index = child;
        }
        if constexpr (std::is_same_v<Type, If> || std::is_same_v<Type, CheckBit> ||
                      std::is_same_v<Type, CheckHalt>) {
            const auto child = FlattenTerminal(node.else_, nodes);
            nodes[index].else_index = child;
        }
    }, terminal);
    return index;
}

std::string DescribeTerminal(const Dynarmic::IR::Terminal& terminal) {
    return boost::apply_visitor([](const auto& node) -> std::string {
        using Type = std::decay_t<decltype(node)>;
        using namespace Dynarmic::IR::Term;
        std::ostringstream output;
        output << "{\"kind\":" << static_cast<std::uint32_t>(RootKind(Terminal{node}));
        if constexpr (std::is_same_v<Type, LinkBlock> ||
                      std::is_same_v<Type, LinkBlockFast> || std::is_same_v<Type, Interpret>) {
            output << ",\"next_descriptor\":" << node.next.Value();
        }
        if constexpr (std::is_same_v<Type, Interpret>)
            output << ",\"interpreter_instruction_count\":" << node.num_instructions;
        if constexpr (std::is_same_v<Type, If>)
            output << ",\"condition\":" << static_cast<std::uint32_t>(node.if_);
        if constexpr (std::is_same_v<Type, If> || std::is_same_v<Type, CheckBit>)
            output << ",\"then\":" << DescribeTerminal(node.then_);
        if constexpr (std::is_same_v<Type, If> || std::is_same_v<Type, CheckBit> ||
                      std::is_same_v<Type, CheckHalt>)
            output << ",\"else\":" << DescribeTerminal(node.else_);
        output << '}';
        return output.str();
    }, terminal);
}

BlockSchedule Derive(const std::vector<unsigned char>& code,
                     std::uint32_t address, std::uint32_t mode, bool diagnostics) {
    const auto program_counter = address & ~std::uint32_t(1);
    const bool thumb = (address & 1) != 0;
    if ((!thumb && (program_counter & 3)) || program_counter < OriginalCodeAddress ||
        std::uint64_t(program_counter) + (thumb ? 2 : 4) > OriginalCodeAddress + code.size())
        throw std::runtime_error("request address is outside aligned approved code");
    if (mode & ~Dynarmic::A32::LocationDescriptor::FPSCR_MODE_MASK)
        throw std::runtime_error("request FPSCR contains status or unsupported mode bits");

    Dynarmic::A32::PSR status{thumb ? 0x20U : 0U};
    const Dynarmic::A32::LocationDescriptor location{
        program_counter, status, Dynarmic::A32::FPSCR{mode}};
    TranslationCallbacks callbacks{code};
    // These are the actual pinned Azahar MakeJit defaults, not hardware guesses.
    const Dynarmic::A32::UserConfig configuration{};
    const Dynarmic::A32::TranslationOptions options{
        configuration.arch_version, true, configuration.hook_hint_instructions};
    auto block = Dynarmic::A32::Translate(location, &callbacks, options);
    const Dynarmic::A32::LocationDescriptor end{block.EndLocation()};
    const auto count = std::count_if(callbacks.translated_addresses.begin(),
        callbacks.translated_addresses.end(), [&](auto instruction_address) {
            // A lookahead can request translation, then break before emitting it.
            return instruction_address >= program_counter && instruction_address < end.PC();
        });

    const auto kind = RootKind(block.GetTerminal());
    bool supervisor = false;
    bool exception = false;
    bool dynamic_status_mode = false;
    bool unsupported = !TerminalRepresentable(block.GetTerminal()) ||
                       !DescriptorRepresentable(block.EndLocation());
    std::vector<std::uint64_t> push_descriptors;
    for (const auto& instruction : block) {
        if (instruction.GetOpcode() == Dynarmic::IR::Opcode::A32CallSupervisor) supervisor = true;
        if (instruction.GetOpcode() == Dynarmic::IR::Opcode::A32ExceptionRaised) exception = true;
        // Unlike flag-only writes, full CPSR writes can change omitted E/IT state.
        if (instruction.GetOpcode() == Dynarmic::IR::Opcode::A32SetCpsr) {
            dynamic_status_mode = true;
            unsupported = true;
        }
        if (instruction.GetOpcode() == Dynarmic::IR::Opcode::PushRSB) {
            const auto value = instruction.GetArg(0);
            if (!value.IsImmediate()) {
                unsupported = true;
            } else {
                push_descriptors.push_back(value.GetU64());
                unsupported |= !DescriptorRepresentable(
                    Dynarmic::IR::LocationDescriptor{value.GetU64()});
            }
        }
    }
    if (push_descriptors.size() > 1) unsupported = true;
    if (exception) {
        unsupported = true;
    }
    if (block.HasConditionFailedLocation())
        unsupported |= !DescriptorRepresentable(block.ConditionFailedLocation());

    const auto failed_count = block.HasConditionFailedLocation() ?
        std::count_if(callbacks.translated_addresses.begin(), callbacks.translated_addresses.end(),
            [&](auto instruction_address) {
                const Dynarmic::A32::LocationDescriptor failed{block.ConditionFailedLocation()};
                return instruction_address >= program_counter && instruction_address < end.PC() &&
                       instruction_address < failed.PC();
            }) : 0;
    const auto end_descriptor = block.EndLocation().Value();
    const auto failed_descriptor = block.HasConditionFailedLocation() ?
        block.ConditionFailedLocation().Value() : 0;
    const auto push_descriptor = push_descriptors.empty() ? 0 : push_descriptors.front();
    BlockSchedule result;
    FlattenTerminal(block.GetTerminal(), result.nodes);

    result.prefix = {
        address, mode, TaggedAddress(block.EndLocation()),
        static_cast<std::uint32_t>(block.GetCondition()),
        block.HasConditionFailedLocation() ? TaggedAddress(block.ConditionFailedLocation()) : 0,
        Narrow(block.CycleCount()), Narrow(block.ConditionFailedCycleCount()), Narrow(count),
        static_cast<std::uint32_t>(kind) | (unsupported ? UnsupportedTerminal : 0) |
            (supervisor ? SupervisorCallFlag : 0),
        push_descriptors.empty() ? 0 : TaggedAddress(
            Dynarmic::IR::LocationDescriptor{push_descriptors.front()}),
        static_cast<std::uint32_t>(end_descriptor), static_cast<std::uint32_t>(end_descriptor >> 32),
        static_cast<std::uint32_t>(failed_descriptor), static_cast<std::uint32_t>(failed_descriptor >> 32),
        static_cast<std::uint32_t>(push_descriptor), static_cast<std::uint32_t>(push_descriptor >> 32),
        Narrow(failed_count), Narrow(result.nodes.size())};
    const auto& metadata = result.prefix;

    if (diagnostics) {
        std::ostringstream output;
        output << "{\"address\":" << address << ",\"fpscr_mode\":" << mode
               << ",\"start_descriptor\":" << location.UniqueHash()
               << ",\"end_descriptor\":" << block.EndLocation().Value()
               << ",\"entry_condition\":" << metadata[3]
               << ",\"condition_failed_descriptor\":"
               << (block.HasConditionFailedLocation() ? block.ConditionFailedLocation().Value() : 0)
               << ",\"pass_cycles\":" << metadata[5] << ",\"failure_cycles\":" << metadata[6]
               << ",\"instruction_count\":" << metadata[7]
               << ",\"failure_instruction_count\":" << metadata[16]
               << ",\"terminal_node_count\":" << metadata[17]
               << ",\"terminal_kind\":" << metadata[8]
               << ",\"unsupported\":" << (unsupported ? "true" : "false")
               << ",\"supervisor\":" << (supervisor ? "true" : "false")
               << ",\"exception\":" << (exception ? "true" : "false")
               << ",\"dynamic_status_mode\":" << (dynamic_status_mode ? "true" : "false")
               << ",\"push_descriptors\":[";
        for (std::size_t index = 0; index < push_descriptors.size(); ++index) {
            if (index) output << ',';
            output << push_descriptors[index];
        }
        output << "],\"terminal\":" << DescribeTerminal(block.GetTerminal()) << "}\n";
        const auto text = output.str();
        if (std::fwrite(text.data(), 1, text.size(), stderr) != text.size())
            throw std::runtime_error("diagnostic output failed");
    }
    return result;
}

void WriteField(std::uint32_t field) {
    const std::array<unsigned char, 4> bytes{
        static_cast<unsigned char>(field), static_cast<unsigned char>(field >> 8),
        static_cast<unsigned char>(field >> 16), static_cast<unsigned char>(field >> 24)};
    if (std::fwrite(bytes.data(), 1, bytes.size(), stdout) != bytes.size())
        throw std::runtime_error("binary metadata output failed");
}
}  // namespace

int main(int argument_count, char** arguments) {
    try {
        if (argument_count < 2 || argument_count > 3 ||
            (argument_count == 3 && std::string(arguments[2]) != "--diagnostics"))
            throw std::runtime_error("usage: derive_native_block_schedule code.bin [--diagnostics]");
        std::ifstream stream(arguments[1], std::ios::binary);
        if (!stream) throw std::runtime_error("cannot read local original code.bin");
        const std::vector<unsigned char> code{
            std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>()};
        if (code.size() != OriginalCodeSize)
            throw std::runtime_error("original code.bin extent is invalid; verify its approved SHA256 before invocation");

        std::array<unsigned char, 8> request;
        for (;;) {
            const auto read = std::fread(request.data(), 1, request.size(), stdin);
            if (read == 0 && std::feof(stdin)) break;
            if (read != request.size()) throw std::runtime_error("incomplete binary request");
            const auto metadata = Derive(code, ReadLittleEndian(request.data()),
                                         ReadLittleEndian(request.data() + 4), argument_count == 3);
            for (const auto field : metadata.prefix) WriteField(field);
            for (const auto& node : metadata.nodes) {
                WriteField(node.kind);
                WriteField(node.condition);
                WriteField(static_cast<std::uint32_t>(node.next_descriptor));
                WriteField(static_cast<std::uint32_t>(node.next_descriptor >> 32));
                WriteField(node.then_index);
                WriteField(node.else_index);
                WriteField(node.interpreter_count);
                WriteField(0);  // Reserved version-two field, always zero.
            }
        }
        if (std::fflush(stdout) != 0) throw std::runtime_error("binary metadata flush failed");
        return 0;
    } catch (const std::exception& error) {
        std::fprintf(stderr, "derive_native_block_schedule: %s\n", error.what());
        return 1;
    }
}
