// Independently authored browser runtime pipeline.
#include "BrowserGpuScheduling.h"
#include <algorithm>
#include <cstring>
#include <limits>
#include <stdexcept>

namespace {
constexpr std::uint32_t SnapshotIdentifier = 0x53555047; // "GPUS", little-endian.
constexpr std::size_t HeaderBytes = 16, DirectoryEntryBytes = 12;

void WriteWord(std::span<std::uint8_t> bytes, std::size_t offset, std::uint32_t value) {
    for (unsigned index = 0; index < 4; ++index)
        bytes[offset + index] = static_cast<std::uint8_t>(value >> (8 * index));
}

std::uint32_t ReadWord(std::span<const std::uint8_t> bytes, std::size_t offset) {
    if (offset > bytes.size() || bytes.size() - offset < 4)
        throw std::invalid_argument("GPU snapshot word is outside its extent");
    std::uint32_t value = 0;
    for (unsigned index = 0; index < 4; ++index)
        value |= static_cast<std::uint32_t>(bytes[offset + index]) << (8 * index);
    return value;
}
} // namespace

namespace Port {

std::vector<std::uint8_t> CaptureBrowserGpuMemorySnapshot(
    std::span<const BrowserGpuMemorySlice> slices, std::size_t maximum_bytes) {
    if (maximum_bytes < HeaderBytes || maximum_bytes > std::numeric_limits<std::uint32_t>::max() ||
        slices.size() > (maximum_bytes - HeaderBytes) / DirectoryEntryBytes)
        throw std::length_error("GPU snapshot directory exceeds its byte capacity");
    std::vector<BrowserGpuMemorySlice> ordered(slices.begin(), slices.end());
    std::sort(ordered.begin(), ordered.end(), [](const auto& first, const auto& second) {
        return first.physical_address < second.physical_address;
    });
    std::size_t total_bytes = HeaderBytes + ordered.size() * DirectoryEntryBytes;
    std::uint64_t previous_end = 0;
    for (const auto& slice : ordered) {
        const auto address_end = static_cast<std::uint64_t>(slice.physical_address) + slice.bytes.size();
        if (slice.bytes.empty() || slice.bytes.size() > std::numeric_limits<std::uint32_t>::max() ||
            address_end > (std::uint64_t{1} << 32) || slice.physical_address < previous_end)
            throw std::invalid_argument("GPU snapshot needs disjoint valid physical ranges");
        if (slice.bytes.size() > maximum_bytes - total_bytes)
            throw std::length_error("GPU snapshot payload exceeds its byte capacity");
        total_bytes += slice.bytes.size();
        previous_end = address_end;
    }
    std::vector<std::uint8_t> snapshot(total_bytes);
    WriteWord(snapshot, 0, SnapshotIdentifier);
    WriteWord(snapshot, 4, 1);
    WriteWord(snapshot, 8, static_cast<std::uint32_t>(ordered.size()));
    WriteWord(snapshot, 12, static_cast<std::uint32_t>(total_bytes));
    std::size_t payload_offset = HeaderBytes + ordered.size() * DirectoryEntryBytes;
    for (std::size_t index = 0; index < ordered.size(); ++index) {
        const auto& slice = ordered[index];
        const auto directory_offset = HeaderBytes + index * DirectoryEntryBytes;
        WriteWord(snapshot, directory_offset, slice.physical_address);
        WriteWord(snapshot, directory_offset + 4, static_cast<std::uint32_t>(slice.bytes.size()));
        WriteWord(snapshot, directory_offset + 8, static_cast<std::uint32_t>(payload_offset));
        std::memcpy(snapshot.data() + payload_offset, slice.bytes.data(), slice.bytes.size());
        payload_offset += slice.bytes.size();
    }
    return snapshot;
}

std::span<const std::uint8_t> ReadBrowserGpuMemorySnapshot(
    std::span<const std::uint8_t> snapshot, std::uint32_t physical_address, std::size_t bytes) {
    return BrowserGpuMemorySnapshotView(snapshot).Read(physical_address, bytes);
}

BrowserGpuMemorySnapshotView::BrowserGpuMemorySnapshotView(std::span<const std::uint8_t> snapshot_)
    : snapshot(snapshot_) {
    if (snapshot.size() < HeaderBytes || ReadWord(snapshot, 0) != SnapshotIdentifier ||
        ReadWord(snapshot, 4) != 1 || ReadWord(snapshot, 12) != snapshot.size())
        throw std::invalid_argument("Invalid GPU snapshot header");
    const auto range_count = ReadWord(snapshot, 8);
    if (range_count > (snapshot.size() - HeaderBytes) / DirectoryEntryBytes)
        throw std::invalid_argument("Invalid GPU snapshot directory");
    const auto directory_end = HeaderBytes + static_cast<std::size_t>(range_count) * DirectoryEntryBytes;
    ranges.reserve(range_count);
    std::uint64_t previous_end = 0;
    std::size_t previous_payload_end = directory_end;
    for (std::size_t index = 0; index < range_count; ++index) {
        const auto directory_offset = HeaderBytes + index * DirectoryEntryBytes;
        const auto address = ReadWord(snapshot, directory_offset);
        const auto length = ReadWord(snapshot, directory_offset + 4);
        const auto offset = ReadWord(snapshot, directory_offset + 8);
        const auto address_end = static_cast<std::uint64_t>(address) + length;
        if (!length || address_end > (std::uint64_t{1} << 32) || address < previous_end ||
            offset != previous_payload_end || offset > snapshot.size() || length > snapshot.size() - offset)
            throw std::invalid_argument("Invalid GPU snapshot range");
        previous_end = address_end;
        previous_payload_end = static_cast<std::size_t>(offset) + length;
        ranges.push_back({address, length, offset});
    }
    if (previous_payload_end != snapshot.size())
        throw std::invalid_argument("GPU snapshot has unowned trailing bytes");
}

std::span<const std::uint8_t> BrowserGpuMemorySnapshotView::Read(
    std::uint32_t physical_address, std::size_t bytes) const {
    if (bytes > (std::uint64_t{1} << 32) - physical_address)
        throw std::invalid_argument("GPU snapshot read exceeds physical address space");
    auto found = std::upper_bound(ranges.begin(), ranges.end(), physical_address,
        [](std::uint32_t address, const Range& range) { return address < range.physical_address; });
    if (found == ranges.begin())
        throw std::out_of_range("GPU read is absent from the immutable snapshot");
    --found;
    const auto range_end = static_cast<std::uint64_t>(found->physical_address) + found->bytes;
    if (static_cast<std::uint64_t>(physical_address) + bytes > range_end)
        throw std::out_of_range("GPU read is absent from the immutable snapshot");
    return snapshot.subspan(static_cast<std::size_t>(found->payload_offset) +
                            physical_address - found->physical_address, bytes);
}

std::uint64_t BrowserGpuScheduling::Submit(
    BrowserGpuCommandKind kind, std::uint64_t frame_identifier, std::uint64_t submission_ticks,
    std::span<const std::uint8_t> snapshot, std::size_t result_capacity_bytes) {
    return pipeline.Submit(kind, frame_identifier, submission_ticks, snapshot, result_capacity_bytes);
}

void BrowserGpuScheduling::DrainCompletions() {
    pipeline.DrainCompletions();
}

void BrowserGpuScheduling::Barrier(BrowserGpuBarrier reason, std::uint64_t through_sequence) {
    const auto index = static_cast<std::size_t>(reason);
    if (index >= barrier_counts.size()) throw std::invalid_argument("Unknown GPU barrier reason");
    pipeline.WaitThrough(through_sequence ? through_sequence : pipeline.PublishedSequence());
    ++barrier_counts[index];
}

void BrowserGpuScheduling::Stop() {
    try { Barrier(BrowserGpuBarrier::Stop); }
    catch (...) {
        const auto error = std::current_exception();
        try { pipeline.Shutdown(); } catch (...) {}
        std::rethrow_exception(error);
    }
    pipeline.Shutdown();
}

} // namespace Port
