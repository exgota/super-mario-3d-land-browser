// Independently authored browser render packet format.
#include "BrowserGpuRenderPacket.h"
#include <algorithm>
#include <cstring>
#include <limits>
#include <stdexcept>

namespace {
constexpr std::uint32_t PacketIdentifier = 0x50525047; // "GPRP", little-endian.
constexpr std::size_t HeaderBytes = 40, DirectoryEntryBytes = 16, SectionAlignment = 8;

bool ValidKind(Port::BrowserGpuCommandKind kind) {
    return kind >= Port::BrowserGpuCommandKind::PicaCommandList &&
           kind <= Port::BrowserGpuCommandKind::RendererShutdown;
}

std::size_t PaddedBytes(std::size_t bytes, std::size_t maximum_bytes) {
    const auto padding = (SectionAlignment - bytes % SectionAlignment) % SectionAlignment;
    if (bytes > maximum_bytes || padding > maximum_bytes - bytes)
        throw std::length_error("Render section exceeds its byte capacity");
    return bytes + padding;
}

void WriteWord(std::span<std::uint8_t> packet, std::size_t offset, std::uint32_t value) {
    for (unsigned index = 0; index < 4; ++index)
        packet[offset + index] = static_cast<std::uint8_t>(value >> (index * 8));
}

std::uint32_t ReadWord(std::span<const std::uint8_t> packet, std::size_t offset) {
    if (offset > packet.size() || packet.size() - offset < 4)
        throw std::invalid_argument("Render packet word is outside its extent");
    std::uint32_t value = 0;
    for (unsigned index = 0; index < 4; ++index)
        value |= static_cast<std::uint32_t>(packet[offset + index]) << (index * 8);
    return value;
}

void WriteLongWord(std::span<std::uint8_t> packet, std::size_t offset, std::uint64_t value) {
    WriteWord(packet, offset, static_cast<std::uint32_t>(value));
    WriteWord(packet, offset + 4, static_cast<std::uint32_t>(value >> 32));
}

std::uint64_t ReadLongWord(std::span<const std::uint8_t> packet, std::size_t offset) {
    return ReadWord(packet, offset) | (static_cast<std::uint64_t>(ReadWord(packet, offset + 4)) << 32);
}
} // namespace

namespace Port {

BrowserGpuRenderPacketBuilder::BrowserGpuRenderPacketBuilder(
    BrowserGpuRenderCommandMetadata metadata_, std::size_t maximum_bytes_)
    : metadata(metadata_), maximum_bytes(maximum_bytes_) {
    if (!ValidKind(metadata.kind)) throw std::invalid_argument("Unknown render packet command kind");
    if (maximum_bytes < HeaderBytes || maximum_bytes > std::numeric_limits<std::uint32_t>::max())
        throw std::length_error("Render packet capacity cannot represent its header and extent");
}

void BrowserGpuRenderPacketBuilder::AddSection(std::uint32_t tag,
                                               std::span<const std::uint8_t> bytes) {
    if (std::any_of(sections.begin(), sections.end(),
                    [tag](const auto& section) { return section.tag == tag; }))
        throw std::invalid_argument("Render packet section tags must be unique");
    if (sections.size() >= (maximum_bytes - HeaderBytes) / DirectoryEntryBytes)
        throw std::length_error("Render packet directory exceeds its byte capacity");
    const auto padded_bytes = PaddedBytes(bytes.size(), maximum_bytes);
    const auto directory_end = HeaderBytes + (sections.size() + 1) * DirectoryEntryBytes;
    if (padded_payload_bytes > maximum_bytes - directory_end ||
        padded_bytes > maximum_bytes - directory_end - padded_payload_bytes)
        throw std::length_error("Render packet sections exceed its byte capacity");
    sections.push_back({tag, std::vector<std::uint8_t>(bytes.begin(), bytes.end())});
    padded_payload_bytes += padded_bytes;
}

std::vector<std::uint8_t> BrowserGpuRenderPacketBuilder::Build() const {
    const auto directory_end = HeaderBytes + sections.size() * DirectoryEntryBytes;
    std::vector<std::uint8_t> packet(directory_end + padded_payload_bytes, 0);
    WriteWord(packet, 0, PacketIdentifier);
    WriteWord(packet, 4, 1);
    WriteWord(packet, 8, static_cast<std::uint32_t>(packet.size()));
    WriteWord(packet, 12, static_cast<std::uint32_t>(sections.size()));
    WriteWord(packet, 16, static_cast<std::uint32_t>(metadata.kind));
    WriteWord(packet, 20, DirectoryEntryBytes);
    WriteLongWord(packet, 24, metadata.frame_identifier);
    WriteLongWord(packet, 32, metadata.submission_ticks);
    std::size_t offset = directory_end;
    for (std::size_t index = 0; index < sections.size(); ++index) {
        const auto& section = sections[index];
        const auto entry_offset = HeaderBytes + index * DirectoryEntryBytes;
        WriteWord(packet, entry_offset, section.tag);
        WriteWord(packet, entry_offset + 4, static_cast<std::uint32_t>(offset));
        WriteWord(packet, entry_offset + 8, static_cast<std::uint32_t>(section.bytes.size()));
        if (!section.bytes.empty())
            std::memcpy(packet.data() + offset, section.bytes.data(), section.bytes.size());
        offset += PaddedBytes(section.bytes.size(), maximum_bytes);
    }
    return packet;
}

BrowserGpuRenderPacketView::BrowserGpuRenderPacketView(std::span<const std::uint8_t> packet_)
    : packet(packet_) {
    if (packet.size() < HeaderBytes || packet.size() > std::numeric_limits<std::uint32_t>::max() ||
        ReadWord(packet, 0) != PacketIdentifier || ReadWord(packet, 4) != 1 ||
        ReadWord(packet, 8) != packet.size() || ReadWord(packet, 20) != DirectoryEntryBytes)
        throw std::invalid_argument("Invalid render packet header");
    metadata = {static_cast<BrowserGpuCommandKind>(ReadWord(packet, 16)),
                ReadLongWord(packet, 24), ReadLongWord(packet, 32)};
    if (!ValidKind(metadata.kind)) throw std::invalid_argument("Unknown render packet command kind");
    const auto count = ReadWord(packet, 12);
    if (count > (packet.size() - HeaderBytes) / DirectoryEntryBytes)
        throw std::invalid_argument("Invalid render packet directory extent");
    const auto directory_end = HeaderBytes + static_cast<std::size_t>(count) * DirectoryEntryBytes;
    sections.reserve(count);
    for (std::size_t index = 0; index < count; ++index) {
        const auto entry_offset = HeaderBytes + index * DirectoryEntryBytes;
        Entry entry{ReadWord(packet, entry_offset), ReadWord(packet, entry_offset + 4),
                    ReadWord(packet, entry_offset + 8)};
        if (ReadWord(packet, entry_offset + 12) != 0 || entry.offset % SectionAlignment != 0 ||
            entry.offset < directory_end || entry.offset > packet.size() ||
            entry.bytes > packet.size() - entry.offset)
            throw std::invalid_argument("Invalid render packet section extent or alignment");
        sections.push_back(entry);
    }
    std::sort(sections.begin(), sections.end(), [](const auto& first, const auto& second) {
        return first.tag < second.tag;
    });
    for (std::size_t index = 1; index < sections.size(); ++index)
        if (sections[index - 1].tag == sections[index].tag)
            throw std::invalid_argument("Render packet section tags must be unique");
    auto extents = sections;
    std::sort(extents.begin(), extents.end(), [](const auto& first, const auto& second) {
        return first.offset == second.offset ? first.bytes < second.bytes : first.offset < second.offset;
    });
    std::size_t previous_end = directory_end;
    for (const auto& entry : extents) {
        if (entry.bytes && entry.offset < previous_end)
            throw std::invalid_argument("Render packet sections overlap");
        previous_end = std::max(previous_end, static_cast<std::size_t>(entry.offset) + entry.bytes);
    }
}

std::span<const std::uint8_t> BrowserGpuRenderPacketView::Section(std::uint32_t tag) const {
    const auto found = std::lower_bound(sections.begin(), sections.end(), tag,
        [](const auto& section, std::uint32_t sought) { return section.tag < sought; });
    if (found == sections.end() || found->tag != tag)
        throw std::out_of_range("Required render packet section is missing");
    return packet.subspan(found->offset, found->bytes);
}

} // namespace Port
