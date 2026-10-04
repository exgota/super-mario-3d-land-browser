// Independently authored browser runtime pipeline.
#include "BrowserGpuPipeline.h"
#include "BrowserGpuRenderPacket.h"
#include <limits>
#include <stdexcept>
#include <utility>
#if defined(__EMSCRIPTEN__)
#include <emscripten/emscripten.h>

EM_JS(int, AdmitBrowserGpuCpuWorker, (std::uint32_t* control), {
    try {
        globalThis.requireBrowserRuntimeWorkerIsolation(HEAPU8.buffer);
        if (globalThis.browserWebGlRenderer)
            throw new Error('GPU pipeline must start before CPU renderer creation');
        Atomics.store(HEAPU32, (control >>> 2) + 1, 1);
        return 1;
    } catch (error) {
        console.error('GPU CPU-worker admission failed:', String(error));
        return 0;
    }
});

EM_JS(int, AdmitBrowserGpuRenderWorker, (std::uint32_t* control), {
    try {
        globalThis.initializeBrowserGpuWorker({
            memoryBuffer: HEAPU8.buffer, controlByteOffset: control,
            createRenderer: globalThis.createBrowserWebGlRenderer,
            rendererOptions: {convertTexture: globalThis.convertPicaTexture},
        });
        return 1;
    } catch (error) {
        console.error('GPU render-worker admission failed:', String(error));
        return 0;
    }
});

EM_JS(int, ReleaseBrowserGpuRenderWorker, (), {
    try {
        globalThis.releaseBrowserGpuWorker?.();
        return 1;
    } catch (error) {
        console.error('GPU render-worker release failed:', String(error));
        return 0;
    }
});

EM_JS(int, ExecuteBrowserGpuRenderPacketJavaScript,
      (const std::uint32_t* metadata, const std::uint8_t* payload, std::uint32_t payload_bytes,
       std::uint8_t* result, std::uint32_t result_bytes), {
    try {
        return globalThis.executeBrowserGpuRenderCommand({
            memoryBuffer: HEAPU8.buffer, metadataByteOffset: metadata,
            payloadByteOffset: payload, payloadBytes: payload_bytes,
            resultByteOffset: result, resultBytes: result_bytes,
        });
    } catch (error) {
        console.error('GPU render packet execution failed:', String(error));
        return -1;
    }
});
#endif

namespace Port {

std::size_t ExecuteBrowserGpuRenderPacket(const BrowserGpuCommandHeader& header,
                                        std::span<const std::uint8_t> payload,
                                        std::span<std::uint8_t> result) {
    const BrowserGpuRenderPacketView packet(payload);
    const auto& metadata = packet.Metadata();
    if (header.schema_version != 1 || header.payload_bytes != payload.size() ||
        header.result_capacity_bytes != result.size() || metadata.kind != header.kind ||
        metadata.frame_identifier != header.frame_identifier ||
        metadata.submission_ticks != header.submission_ticks)
        throw std::invalid_argument("GPU packet metadata differs from its queue record");
#if defined(__EMSCRIPTEN__)
    if (payload.size() > std::numeric_limits<std::uint32_t>::max() ||
        result.size() > static_cast<std::size_t>(std::numeric_limits<int>::max()))
        throw std::length_error("GPU JavaScript packet exceeds its bounded bridge extent");
    const std::array<std::uint32_t, 7> words{
        static_cast<std::uint32_t>(header.kind),
        static_cast<std::uint32_t>(header.sequence), static_cast<std::uint32_t>(header.sequence >> 32),
        static_cast<std::uint32_t>(header.frame_identifier), static_cast<std::uint32_t>(header.frame_identifier >> 32),
        static_cast<std::uint32_t>(header.submission_ticks), static_cast<std::uint32_t>(header.submission_ticks >> 32)};
    const int written = ExecuteBrowserGpuRenderPacketJavaScript(words.data(), payload.data(),
        static_cast<std::uint32_t>(payload.size()), result.data(), static_cast<std::uint32_t>(result.size()));
    if (written < 0) throw std::runtime_error("GPU render packet adapter failed");
    return static_cast<std::size_t>(written);
#else
    throw std::runtime_error("GPU renderer packet execution requires the browser shared-Wasm module");
#endif
}

BrowserGpuPipeline::BrowserGpuPipeline(BrowserGpuQueueLimits limits, Complete complete_)
    : BrowserGpuPipeline(limits, ExecuteBrowserGpuRenderPacket, std::move(complete_)) {}

BrowserGpuPipeline::BrowserGpuPipeline(BrowserGpuQueueLimits limits, Execute execute_,
                                       Complete complete_)
    : queue(limits), cpu_thread(std::this_thread::get_id()), execute(std::move(execute_)),
      complete(std::move(complete_)) {
    if (!execute || !complete) throw std::invalid_argument("GPU pipeline needs both handlers");
#if defined(__EMSCRIPTEN__)
    static_assert(sizeof(std::atomic<std::uint32_t>) == sizeof(std::uint32_t));
    if (!AdmitBrowserGpuCpuWorker(reinterpret_cast<std::uint32_t*>(worker_admission.data())))
        throw std::runtime_error("GPU pipeline requires an isolated CPU worker and shared Wasm");
#endif
    worker = std::thread([this] { WorkerLoop(); });
    try { queue.WaitForConsumerReady(); }
    catch (...) {
        queue.Close();
        worker.join();
        throw;
    }
}

BrowserGpuPipeline::~BrowserGpuPipeline() {
    // Explicit Shutdown reports failures. Valid CPU-owned destruction joins.
    try { Shutdown(); } catch (...) {}
}

void BrowserGpuPipeline::RequireCpuThread() const {
    if (std::this_thread::get_id() != cpu_thread)
        throw std::logic_error("GPU pipeline control requires the CPU thread");
    if (draining) throw std::logic_error("GPU completion handlers must not reenter the pipeline");
}

void BrowserGpuPipeline::WorkerLoop() {
    std::shared_ptr<BrowserGpuCommandRecord> record;
    try {
#if defined(__EMSCRIPTEN__)
        if (!AdmitBrowserGpuRenderWorker(reinterpret_cast<std::uint32_t*>(worker_admission.data())))
            throw std::runtime_error("GPU worker isolation, shared Wasm or WebGL ownership failed");
#endif
        queue.MarkConsumerReady();
        while ((record = queue.Acquire())) {
            const auto bytes = execute(record->Header(), record->Payload(), record->ResultStorage());
            queue.Complete(record, bytes);
            record.reset();
        }
    } catch (...) {
        queue.Fail(record, std::current_exception());
    }
#if defined(__EMSCRIPTEN__)
    if (!ReleaseBrowserGpuRenderWorker())
        queue.Fail({}, std::make_exception_ptr(std::runtime_error("GPU context release failed")));
#endif
}

std::uint64_t BrowserGpuPipeline::Submit(
    BrowserGpuCommandKind kind, std::uint64_t frame_identifier, std::uint64_t submission_ticks,
    std::span<const std::uint8_t> payload, std::size_t result_capacity_bytes) {
    RequireCpuThread();
    if (stopped) throw std::logic_error("GPU pipeline is stopped");
    for (;;) {
        DrainCompletions();
        if (const auto sequence = queue.TryPublish(kind, frame_identifier, submission_ticks,
                                                   payload, result_capacity_bytes))
            return sequence;
        const auto oldest = queue.OldestSequence();
        if (!oldest) throw std::logic_error("GPU admission cannot make progress");
        queue.WaitThrough(oldest);
    }
}

void BrowserGpuPipeline::DrainCompletions() {
    RequireCpuThread();
    draining = true;
    try {
        while (const auto record = queue.RetireCompleted())
            complete(record->Header(), record->Result());
    } catch (...) {
        draining = false;
        queue.Abort(std::current_exception());
        throw;
    }
    draining = false;
}

std::array<std::uint32_t, 6> BrowserGpuPipeline::WorkerAdmission() const {
    std::array<std::uint32_t, 6> result;
    for (std::size_t index = 0; index < result.size(); ++index)
        result[index] = worker_admission[index].load(std::memory_order_acquire);
    return result;
}

void BrowserGpuPipeline::WaitThrough(std::uint64_t sequence) {
    RequireCpuThread();
    queue.WaitThrough(sequence);
    DrainCompletions();
}

void BrowserGpuPipeline::Fence() {
    WaitThrough(queue.PublishedSequence());
}

void BrowserGpuPipeline::Shutdown() {
    RequireCpuThread();
    if (stopped) { queue.RethrowFailure(); return; }
    queue.Close();
    if (worker.joinable()) worker.join();
    stopped = true;
    DrainCompletions();
}

} // namespace Port

#if defined(__EMSCRIPTEN__)
// Preparation entry point in the selected shared-Wasm module. It exercises this
// same renderer pipeline and releases its owner before guest startup. Root owns
// browser admission and calls it only before an active guest renderer exists.
extern "C" EMSCRIPTEN_KEEPALIVE std::uint32_t BrowserGpuPipelineAdmission() {
    try {
        const auto cpu = std::this_thread::get_id();
        bool completed_on_cpu = false;
        Port::BrowserGpuPipeline pipeline({2, 4096, 1},
            [&](const auto& header, auto result) {
                completed_on_cpu = std::this_thread::get_id() == cpu &&
                    header.kind == Port::BrowserGpuCommandKind::Barrier && result.empty();
            });
        const auto admission = pipeline.WorkerAdmission();
        if (admission != std::array<std::uint32_t, 6>{1, 1, 42, 1, 1, 1})
            throw std::runtime_error("GPU worker admission cells did not cross the actual Wasm heap");
        Port::BrowserGpuRenderPacketBuilder builder({Port::BrowserGpuCommandKind::Barrier, 1, 0}, 4096);
        const auto packet = builder.Build();
        const auto sequence = pipeline.Submit(Port::BrowserGpuCommandKind::Barrier, 1, 0, packet);
        pipeline.WaitThrough(sequence);
        const auto statistics = pipeline.Statistics();
        const bool retired = statistics.published_sequence == 1 && statistics.completed_sequence == 1 &&
            statistics.retired_sequence == 1 && statistics.retained_records == 0;
        pipeline.Shutdown();
        return completed_on_cpu && retired ? 1 : 0;
    } catch (...) {
        return 0;
    }
}
#endif
