// Independently authored browser runtime pipeline.
#include "BrowserGpuPipeline.h"
#include <stdexcept>
#include <utility>
#if defined(__EMSCRIPTEN__)
#include <emscripten/emscripten.h>

EM_JS(int, AdmitBrowserGpuCpuWorker, (std::uint32_t* control), {
    try {
        globalThis.requireBrowserRuntimeWorkerIsolation(HEAPU8.buffer);
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
#endif

namespace Port {

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
