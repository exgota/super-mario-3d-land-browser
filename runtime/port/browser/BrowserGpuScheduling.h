// Independently authored browser runtime pipeline.
#pragma once
#include "BrowserGpuPipeline.h"
#include <array>

namespace Port {

enum class BrowserGpuBarrier : std::size_t {
    GuestMemoryRead,
    MemoryFill,
    MemoryTransfer,
    CacheInvalidation,
    CommandOrResourceReuse,
    Presentation,
    Stop,
    Count,
};

struct BrowserGpuMemorySlice {
    std::uint32_t physical_address;
    std::span<const std::uint8_t> bytes;
};

// Directory and bytes use a versioned little-endian wire format. Empty or
// overlapping physical ranges are refused. A missing range never reads live RAM.
std::vector<std::uint8_t> CaptureBrowserGpuMemorySnapshot(
    std::span<const BrowserGpuMemorySlice> slices, std::size_t maximum_bytes);
std::span<const std::uint8_t> ReadBrowserGpuMemorySnapshot(
    std::span<const std::uint8_t> snapshot, std::uint32_t physical_address,
    std::size_t bytes);

// Validate once per job, then resolve bounded physical reads by binary search.
// The view borrows the immutable queue payload and cannot outlive its record.
class BrowserGpuMemorySnapshotView {
public:
    explicit BrowserGpuMemorySnapshotView(std::span<const std::uint8_t> snapshot);
    std::span<const std::uint8_t> Read(std::uint32_t physical_address, std::size_t bytes) const;

private:
    struct Range {
        std::uint32_t physical_address, bytes, payload_offset;
    };
    std::span<const std::uint8_t> snapshot;
    std::vector<Range> ranges;
};

class BrowserGpuScheduling {
public:
    explicit BrowserGpuScheduling(BrowserGpuPipeline& pipeline) : pipeline(pipeline) {}

    std::uint64_t Submit(BrowserGpuCommandKind kind, std::uint64_t frame_identifier,
                         std::uint64_t submission_ticks, std::span<const std::uint8_t> snapshot,
                         std::size_t result_capacity_bytes = 0);
    void DrainCompletions();
    void Barrier(BrowserGpuBarrier reason, std::uint64_t through_sequence = 0);
    void Stop();
    const std::array<std::uint64_t, static_cast<std::size_t>(BrowserGpuBarrier::Count)>&
    BarrierCounts() const { return barrier_counts; }

private:
    BrowserGpuPipeline& pipeline;
    std::array<std::uint64_t, static_cast<std::size_t>(BrowserGpuBarrier::Count)> barrier_counts{};
};

} // namespace Port
