#include "StaticArmBackend.h"
#include <iostream>
#include <exception>
#include "root_port_capture/cpu_backend_factory.h"
#include "root_port_capture/headless_capture.h"

int main(int argc, char** argv) {
    if (argc < 4 || argc > 6) {
        std::cerr << "usage: azahar_static_execution <translated-library> <dump-copy> <absent-output-directory> [movie] [initial-user-directory]\n";
        return 2;
    }
    const std::filesystem::path library = std::filesystem::absolute(argv[1]);
    const auto trace = Port::GuestMemoryTrace::FromEnvironment();
    const auto write_observation = Port::GuestWriteObservation::FromEnvironment();
    const auto execution_observation = Port::GuestExecutionObservation::FromEnvironment();
    RootPortCapture::RegisterCpuBackendFactory([library, trace, write_observation, execution_observation](Core::System& system, Memory::MemorySystem& memory,
                                                        u32 id, std::shared_ptr<Core::Timing::Timer> timer) {
        return std::make_shared<Port::StaticArmBackend>(system, memory, id, std::move(timer), library, trace, write_observation, execution_observation);
    });
    std::vector<char*> arguments{argv[0]};
    arguments.insert(arguments.end(), argv + 2, argv + argc);
    arguments.push_back(nullptr);
    if (!write_observation && !execution_observation) return RootPortCapture::RunHeadlessCapture(argc - 1, arguments.data());
    int result;
    try {
        result = RootPortCapture::RunHeadlessCapture(argc - 1, arguments.data());
    } catch (...) {
        if (write_observation) { try { write_observation->Finish(false); } catch (...) {} }
        if (execution_observation) { try { execution_observation->Finish(false); } catch (...) {} }
        throw;
    }
    bool complete = true;
    std::exception_ptr finish_failure;
    if (write_observation) {
        try { complete = write_observation->Finish(result == 0); }
        catch (...) { finish_failure = std::current_exception(); }
    }
    if (execution_observation) {
        try { complete = execution_observation->Finish(result == 0) && complete; }
        catch (...) { if (!finish_failure) finish_failure = std::current_exception(); }
    }
    if (finish_failure) std::rethrow_exception(finish_failure);
    return result == 0 && !complete ? 1 : result;
}
