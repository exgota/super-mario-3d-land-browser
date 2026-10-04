// Independently authored browser runtime pipeline.
#include "BrowserGpuCommandQueue.h"
#include <algorithm>
#include <limits>
#include <stdexcept>

namespace Port {

BrowserGpuCommandQueue::BrowserGpuCommandQueue(BrowserGpuQueueLimits limits_)
    : limits(limits_), producer(std::this_thread::get_id()) {
    if (!limits.maximum_records || !limits.maximum_bytes || !limits.maximum_frames)
        throw std::invalid_argument("GPU queue limits must be positive");
}

void BrowserGpuCommandQueue::RequireProducer() const {
    if (std::this_thread::get_id() != producer)
        throw std::logic_error("GPU queue producer operation requires the CPU thread");
}

void BrowserGpuCommandQueue::RequireConsumer() {
    const auto current = std::this_thread::get_id();
    if (current == producer)
        throw std::logic_error("GPU queue consumer must be a separate worker");
    if (consumer == std::thread::id{}) consumer = current;
    if (consumer != current)
        throw std::logic_error("GPU queue has exactly one consumer");
}

void BrowserGpuCommandQueue::RethrowFailureLocked() const {
    if (failure) std::rethrow_exception(failure);
}

void BrowserGpuCommandQueue::RethrowFailure() const {
    std::lock_guard lock(mutex);
    RethrowFailureLocked();
}

std::uint64_t BrowserGpuCommandQueue::TryPublish(
    BrowserGpuCommandKind kind, std::uint64_t frame_identifier, std::uint64_t submission_ticks,
    std::span<const std::uint8_t> payload, std::size_t result_capacity_bytes) {
    RequireProducer();
    if (kind < BrowserGpuCommandKind::PicaCommandList || kind > BrowserGpuCommandKind::Barrier)
        throw std::invalid_argument("Unknown GPU command kind");
    if (payload.size() > limits.maximum_bytes ||
        result_capacity_bytes > limits.maximum_bytes - payload.size())
        throw std::length_error("GPU command exceeds the queue byte capacity");
    const auto bytes = payload.size() + result_capacity_bytes;
    std::lock_guard lock(mutex);
    RethrowFailureLocked();
    if (closed) throw std::logic_error("GPU queue is closed");
    if (frame_identifier < last_frame_identifier)
        throw std::invalid_argument("GPU frame identifiers must be monotonic");
    const bool new_frame = !frames.contains(frame_identifier);
    if (retained.size() >= limits.maximum_records || bytes > limits.maximum_bytes - retained_bytes ||
        (new_frame && frames.size() >= limits.maximum_frames)) {
        ++admission_waits;
        return 0;
    }
    const auto previous_sequence = published_sequence.load(std::memory_order_relaxed);
    if (previous_sequence == std::numeric_limits<std::uint64_t>::max())
        throw std::overflow_error("GPU command sequence exhausted");
    auto record = std::make_shared<BrowserGpuCommandRecord>();
    record->header = {1, kind, previous_sequence + 1, frame_identifier, submission_ticks,
                      payload.size(), result_capacity_bytes};
    record->payload.assign(payload.begin(), payload.end());
    record->result.resize(result_capacity_bytes);
    // Roll back all containers if an allocation fails before publication.
    auto [frame, inserted] = frames.try_emplace(frame_identifier, 0);
    try {
        retained.push_back(record);
        try { pending.push_back(record); }
        catch (...) { retained.pop_back(); throw; }
    } catch (...) {
        if (inserted) frames.erase(frame);
        throw;
    }
    ++frame->second;
    retained_bytes += bytes;
    last_frame_identifier = frame_identifier;
    peak_records = std::max(peak_records, retained.size());
    peak_bytes = std::max(peak_bytes, retained_bytes);
    peak_frames = std::max(peak_frames, frames.size());
    published_sequence.store(record->header.sequence, std::memory_order_release);
    condition.notify_all();
    return record->header.sequence;
}

std::shared_ptr<BrowserGpuCommandRecord> BrowserGpuCommandQueue::Acquire() {
    std::unique_lock lock(mutex);
    RequireConsumer();
    if (executing_record) throw std::logic_error("GPU worker already owns an executing record");
    condition.wait(lock, [&] { return failure || closed || !pending.empty(); });
    if (failure || pending.empty()) return {};
    auto record = pending.front();
    pending.pop_front();
    // The acquire matches publication of the owned snapshot, including its bytes.
    if (published_sequence.load(std::memory_order_acquire) < record->header.sequence)
        throw std::logic_error("GPU command was not published");
    record->state.store(BrowserGpuCommandRecord::State::Executing, std::memory_order_release);
    executing_record = record.get();
    return record;
}

void BrowserGpuCommandQueue::MarkConsumerReady() {
    std::lock_guard lock(mutex);
    RequireConsumer();
    RethrowFailureLocked();
    consumer_ready = true;
    condition.notify_all();
}

void BrowserGpuCommandQueue::WaitForConsumerReady() {
    RequireProducer();
    std::unique_lock lock(mutex);
    condition.wait(lock, [&] { return failure || consumer_ready; });
    RethrowFailureLocked();
}

void BrowserGpuCommandQueue::Complete(const std::shared_ptr<BrowserGpuCommandRecord>& record,
                                      std::size_t result_bytes) {
    std::lock_guard lock(mutex);
    RequireConsumer();
    if (!record || record.get() != executing_record || result_bytes > record->result.size() ||
        record->header.sequence != completed_sequence.load(std::memory_order_relaxed) + 1 ||
        record->state.load(std::memory_order_acquire) != BrowserGpuCommandRecord::State::Executing)
        throw std::logic_error("Invalid GPU command completion");
    record->result_bytes = result_bytes;
    record->state.store(BrowserGpuCommandRecord::State::Complete, std::memory_order_release);
    completed_sequence.store(record->header.sequence, std::memory_order_release);
    executing_record = nullptr;
    condition.notify_all();
}

void BrowserGpuCommandQueue::Fail(const std::shared_ptr<BrowserGpuCommandRecord>& record,
                                  std::exception_ptr error) {
    std::lock_guard lock(mutex);
    RequireConsumer();
    if (!error) error = std::make_exception_ptr(std::runtime_error("GPU worker failed"));
    if (!failure) failure = error;
    if (record) record->state.store(BrowserGpuCommandRecord::State::Failed, std::memory_order_release);
    closed = true;
    condition.notify_all();
}

std::shared_ptr<BrowserGpuCommandRecord> BrowserGpuCommandQueue::RetireCompleted() {
    RequireProducer();
    std::lock_guard lock(mutex);
    RethrowFailureLocked();
    if (retained.empty() || retained.front()->state.load(std::memory_order_acquire) !=
                                BrowserGpuCommandRecord::State::Complete)
        return {};
    auto record = retained.front();
    retained.pop_front();
    retained_bytes -= record->header.payload_bytes + record->header.result_capacity_bytes;
    auto frame = frames.find(record->header.frame_identifier);
    if (--frame->second == 0) frames.erase(frame);
    retired_sequence = record->header.sequence;
    condition.notify_all();
    return record;
}

void BrowserGpuCommandQueue::WaitThrough(std::uint64_t sequence) {
    RequireProducer();
    std::unique_lock lock(mutex);
    if (sequence > published_sequence.load(std::memory_order_acquire))
        throw std::invalid_argument("GPU fence references an unpublished sequence");
    condition.wait(lock, [&] {
        return failure || completed_sequence.load(std::memory_order_acquire) >= sequence;
    });
    RethrowFailureLocked();
}

std::uint64_t BrowserGpuCommandQueue::OldestSequence() const {
    std::lock_guard lock(mutex);
    return retained.empty() ? 0 : retained.front()->header.sequence;
}

std::uint64_t BrowserGpuCommandQueue::PublishedSequence() const {
    return published_sequence.load(std::memory_order_acquire);
}

BrowserGpuQueueStatistics BrowserGpuCommandQueue::Statistics() const {
    std::lock_guard lock(mutex);
    return {published_sequence.load(std::memory_order_acquire),
            completed_sequence.load(std::memory_order_acquire), retired_sequence,
            retained.size(), retained_bytes, frames.size(), peak_records, peak_bytes,
            peak_frames, admission_waits};
}

void BrowserGpuCommandQueue::Close() {
    RequireProducer();
    std::lock_guard lock(mutex);
    closed = true;
    condition.notify_all();
}

void BrowserGpuCommandQueue::Abort(std::exception_ptr error) {
    RequireProducer();
    std::lock_guard lock(mutex);
    if (!error) error = std::make_exception_ptr(std::runtime_error("GPU completion failed"));
    if (!failure) failure = error;
    closed = true;
    condition.notify_all();
}

} // namespace Port
