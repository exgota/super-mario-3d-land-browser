// Independently authored browser render packet format.
#pragma once
#include "BrowserGpuCommandQueue.h"

namespace Port {

struct BrowserGpuRenderCommandMetadata {
    BrowserGpuCommandKind kind;
    std::uint64_t frame_identifier;
    std::uint64_t submission_ticks;
};

// Tags are assigned by the integration layer. Each section owns arbitrary bytes,
// not a guest pointer or callback. Every serialized integer is little endian.
class BrowserGpuRenderPacketBuilder {
public:
    BrowserGpuRenderPacketBuilder(BrowserGpuRenderCommandMetadata metadata,
                                  std::size_t maximum_bytes);
    void AddSection(std::uint32_t tag, std::span<const std::uint8_t> bytes);
    std::vector<std::uint8_t> Build() const;

private:
    struct Section {
        std::uint32_t tag;
        std::vector<std::uint8_t> bytes;
    };
    BrowserGpuRenderCommandMetadata metadata;
    std::size_t maximum_bytes, padded_payload_bytes = 0;
    std::vector<Section> sections;
};

// Validate once per immutable record. Returned spans borrow that record's bytes.
class BrowserGpuRenderPacketView {
public:
    explicit BrowserGpuRenderPacketView(std::span<const std::uint8_t> packet);
    const BrowserGpuRenderCommandMetadata& Metadata() const { return metadata; }
    std::span<const std::uint8_t> Section(std::uint32_t tag) const;

private:
    struct Entry {
        std::uint32_t tag, offset, bytes;
    };
    std::span<const std::uint8_t> packet;
    BrowserGpuRenderCommandMetadata metadata;
    std::vector<Entry> sections;
};

} // namespace Port
