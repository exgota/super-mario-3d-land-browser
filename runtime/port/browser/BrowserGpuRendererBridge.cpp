// Independently authored integration of the GPU-owned renderer.
#include "BrowserGpuPipeline.h"
#include "BrowserGpuRenderPacket.h"
#include <emscripten/emscripten.h>
#include <cstdint>
#include <memory>
#include <span>
#include <stdexcept>
#include <vector>

namespace {
std::unique_ptr<Port::BrowserGpuPipeline> pipeline;
std::vector<std::uint8_t> synchronous_result;
constexpr std::size_t maximum_queue_bytes = 32 * 1024 * 1024;
}

extern "C" EMSCRIPTEN_KEEPALIVE void BrowserGpuRendererInitialize() {
    if (pipeline) throw std::logic_error("GPU renderer pipeline is already initialized");
    pipeline = std::make_unique<Port::BrowserGpuPipeline>(
        Port::BrowserGpuQueueLimits{128, maximum_queue_bytes, 2},
        [](const auto& header, std::span<const std::uint8_t> result) {
            if (header.result_capacity_bytes)
                synchronous_result.assign(result.begin(), result.end());
        });
}

extern "C" EMSCRIPTEN_KEEPALIVE std::uint32_t BrowserGpuRendererSubmit(
    const std::uint8_t* bytes, std::uint32_t byte_count,
    std::uint32_t result_capacity, std::uint32_t wait_for_completion) {
    if (!pipeline) throw std::logic_error("GPU renderer pipeline is unavailable");
    if (result_capacity > maximum_queue_bytes / 2 ||
        (result_capacity && !wait_for_completion))
        throw std::length_error("GPU renderer results require bounded synchronous retirement");
    const std::span<const std::uint8_t> payload(bytes, byte_count);
    const Port::BrowserGpuRenderPacketView packet(payload);
    const auto metadata = packet.Metadata();
    const auto sequence = pipeline->Submit(metadata.kind, metadata.frame_identifier,
        metadata.submission_ticks, payload, result_capacity);
    if (wait_for_completion) pipeline->WaitThrough(sequence);
    else pipeline->DrainCompletions();
    return wait_for_completion ? static_cast<std::uint32_t>(synchronous_result.size()) : 0;
}

extern "C" EMSCRIPTEN_KEEPALIVE const std::uint8_t* BrowserGpuRendererResultPointer() {
    return synchronous_result.data();
}

extern "C" EMSCRIPTEN_KEEPALIVE void BrowserGpuRendererFence() {
    if (pipeline) pipeline->Fence();
}

extern "C" EMSCRIPTEN_KEEPALIVE void BrowserGpuRendererShutdown() {
    if (!pipeline) return;
    pipeline->Shutdown();
    pipeline.reset();
    synchronous_result.clear();
}
