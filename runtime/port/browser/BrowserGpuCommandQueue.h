// Independently authored browser runtime pipeline.
#pragma once

#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <exception>
#include <map>
#include <memory>
#include <mutex>
#include <span>
#include <thread>
#include <vector>

namespace Port {

enum class BrowserGpuCommandKind : std::uint32_t {
    PicaCommandList = 1,
    DrawState,
    VertexBatch,
    TextureUpload,
    MemoryFill,
    MemoryTransfer,
    FramebufferReadback,
    CacheInvalidation,
    Presentation,
    Barrier,
    TriangleBatch,
    VertexProgramPreflight,
    SurfaceDeletion,
    RendererDiagnostics,
    RendererShutdown,
    DisplayTransfer,
};

struct BrowserGpuQueueLimits {
    std::size_t maximum_records;
    std::size_t maximum_bytes;
    std::size_t maximum_frames;
};

struct BrowserGpuCommandHeader {
    std::uint32_t schema_version = 1;
    BrowserGpuCommandKind kind;
    std::uint64_t sequence;
    std::uint64_t frame_identifier;
    std::uint64_t submission_ticks;
    std::size_t payload_bytes;
    std::size_t result_capacity_bytes;
};

struct BrowserGpuQueueStatistics {
    std::uint64_t published_sequence;
    std::uint64_t completed_sequence;
    std::uint64_t retired_sequence;
    std::size_t retained_records;
    std::size_t retained_bytes;
    std::size_t retained_frames;
    std::size_t peak_records;
    std::size_t peak_bytes;
    std::size_t peak_frames;
    std::uint64_t admission_waits;
};

// A command owns every input byte. Its result storage has a reserved, finite extent.
// Neither a mutable guest pointer nor a callback into the guest belongs in a payload.
class BrowserGpuCommandRecord {
public:
    const BrowserGpuCommandHeader& Header() const { return header; }
    std::span<const std::uint8_t> Payload() const { return payload; }
    std::span<std::uint8_t> ResultStorage() { return result; }
    std::span<const std::uint8_t> Result() const { return {result.data(), result_bytes}; }

private:
    friend class BrowserGpuCommandQueue;
    enum class State : std::uint32_t { Published, Executing, Complete, Failed };
    BrowserGpuCommandHeader header;
    std::vector<std::uint8_t> payload;
    std::vector<std::uint8_t> result;
    std::size_t result_bytes = 0;
    std::atomic<State> state{State::Published};
};

// One CPU producer and one GPU consumer. Allocation and ownership remain bounded
// until the CPU retires a completion, rather than just until the GPU dequeues it.
class BrowserGpuCommandQueue {
public:
    explicit BrowserGpuCommandQueue(BrowserGpuQueueLimits limits);

    // Returns zero only for backpressure. Copies bytes before release publication.
    std::uint64_t TryPublish(BrowserGpuCommandKind kind, std::uint64_t frame_identifier,
                             std::uint64_t submission_ticks,
                             std::span<const std::uint8_t> payload,
                             std::size_t result_capacity_bytes);
    std::shared_ptr<BrowserGpuCommandRecord> Acquire();
    void MarkConsumerReady();
    void WaitForConsumerReady();
    void Complete(const std::shared_ptr<BrowserGpuCommandRecord>& record,
                  std::size_t result_bytes);
    void Fail(const std::shared_ptr<BrowserGpuCommandRecord>& record, std::exception_ptr error);
    std::shared_ptr<BrowserGpuCommandRecord> RetireCompleted();
    void WaitThrough(std::uint64_t sequence);
    std::uint64_t OldestSequence() const;
    std::uint64_t PublishedSequence() const;
    BrowserGpuQueueStatistics Statistics() const;
    void Close();
    void Abort(std::exception_ptr error);
    void RethrowFailure() const;

private:
    void RequireProducer() const;
    void RequireConsumer();
    void RethrowFailureLocked() const;

    BrowserGpuQueueLimits limits;
    const std::thread::id producer;
    std::thread::id consumer;
    mutable std::mutex mutex;
    std::condition_variable condition;
    std::deque<std::shared_ptr<BrowserGpuCommandRecord>> pending;
    std::deque<std::shared_ptr<BrowserGpuCommandRecord>> retained;
    BrowserGpuCommandRecord* executing_record = nullptr;
    std::map<std::uint64_t, std::size_t> frames;
    std::exception_ptr failure;
    bool closed = false, consumer_ready = false;
    std::size_t retained_bytes = 0, peak_records = 0, peak_bytes = 0, peak_frames = 0;
    std::uint64_t retired_sequence = 0, last_frame_identifier = 0, admission_waits = 0;
    std::atomic<std::uint64_t> published_sequence{0}, completed_sequence{0};
};

} // namespace Port
