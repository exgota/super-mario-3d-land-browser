// Independently authored browser runtime pipeline.
#pragma once
#include "BrowserGpuCommandQueue.h"
#include <array>
#include <functional>

namespace Port {

class BrowserGpuPipeline {
public:
    // Execute runs only on the GPU worker. It writes into the reserved result
    // extent and returns the written size, after required memory coherence.
    using Execute = std::function<std::size_t(const BrowserGpuCommandHeader&,
                                              std::span<const std::uint8_t>,
                                              std::span<std::uint8_t>)>;
    // Complete runs only on the CPU producer. GSP/timing/kernel actions go here.
    using Complete = std::function<void(const BrowserGpuCommandHeader&,
                                        std::span<const std::uint8_t>)>;

    BrowserGpuPipeline(BrowserGpuQueueLimits limits, Execute execute, Complete complete);
    // Lifecycle and completion operations, including destruction, belong to CPU.
    ~BrowserGpuPipeline();
    BrowserGpuPipeline(const BrowserGpuPipeline&) = delete;
    BrowserGpuPipeline& operator=(const BrowserGpuPipeline&) = delete;

    std::uint64_t Submit(BrowserGpuCommandKind kind, std::uint64_t frame_identifier,
                         std::uint64_t submission_ticks, std::span<const std::uint8_t> payload,
                         std::size_t result_capacity_bytes = 0);
    void DrainCompletions();
    void WaitThrough(std::uint64_t sequence);
    void Fence();
    void Shutdown();
    BrowserGpuQueueStatistics Statistics() const { return queue.Statistics(); }
    std::uint64_t PublishedSequence() const { return queue.PublishedSequence(); }
    std::array<std::uint32_t, 6> WorkerAdmission() const;

private:
    void RequireCpuThread() const;
    void WorkerLoop();
    BrowserGpuCommandQueue queue;
    const std::thread::id cpu_thread;
    Execute execute;
    Complete complete;
    std::thread worker;
    // Browser-only admission uses actual cells in the shared Wasm memory.
    std::array<std::atomic<std::uint32_t>, 6> worker_admission{{1, 0, 41, 0, 0, 0}};
    bool stopped = false, draining = false;
};

} // namespace Port
