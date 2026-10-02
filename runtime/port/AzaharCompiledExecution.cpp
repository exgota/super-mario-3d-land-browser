// SPDX-License-Identifier: GPL-2.0-or-later
#include "StaticArmBackend.h"
#include <iostream>
#include "root_port_capture/cpu_backend_factory.h"
#include "root_port_capture/headless_capture.h"

extern "C" {
extern const std::uint32_t recomp_abi;
extern const std::uint32_t recomp_entry_count;
extern const Entry recomp_entries[];
}

int main(int argc, char** argv) {
    if (argc < 4 || argc > 6) {
        std::cerr << "usage: azahar_compiled_execution <block-schedule> <dump-copy> <absent-output-directory> [movie] [initial-user-directory]\n";
        return 2;
    }
    const Port::TranslatedFunctionModule module{
        recomp_abi, native_timing_revision, recomp_entries, recomp_entry_count,
        &native_block_timing_callback
    };
    try {
        module.Validate();
        const auto schedule = std::filesystem::absolute(argv[1]);
        const auto trace = Port::GuestMemoryTrace::FromEnvironment();
        RootPortCapture::RegisterCpuBackendFactory(
            [module, schedule, trace](Core::System& system, Memory::MemorySystem& memory,
                                     u32 id, std::shared_ptr<Core::Timing::Timer> timer) {
                return std::make_shared<Port::StaticArmBackend>(
                    system, memory, id, std::move(timer), module, schedule, trace);
            });
        std::vector<char*> arguments{argv[0]};
        arguments.insert(arguments.end(), argv + 2, argv + argc);
        arguments.push_back(nullptr);
        return RootPortCapture::RunHeadlessCapture(argc - 1, arguments.data());
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << '\n';
        return 1;
    }
}
