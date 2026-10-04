// Independently authored display-transfer address and guest-read integration.
#include "BrowserWebGlDisplaySurface.h"
#include "BrowserWebGlPresentation.h"
#include "BrowserWebGlBridge.h"
#include "common/color.h"
#include "core/memory.h"
#include "video_core/pica/pica_core.h"
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <limits>
#include <map>
#include <stdexcept>
#include <vector>
#include <emscripten/emscripten.h>

namespace {
struct DisplaySurface {
    std::uint32_t identifier, address, width, height, format, bytes;
    Memory::MemorySystem* memory;
    std::vector<std::uint8_t> rgba;
    bool materialized = false;
};
std::map<std::uint32_t, DisplaySurface> display_surfaces;
std::map<std::uint32_t, std::uint32_t> cached_pages;
std::uint32_t next_identifier = 0x40000000;
std::uint64_t handled_frame;
constexpr std::uint32_t page_bytes = 4096;

EM_JS(int, PresentationEnabled, (), {
    return globalThis.browserGpuPresentationEnabled === true &&
        globalThis.browserGpuPipelineEnabled === true;
});
EM_JS(int, CopyDisplaySurface, (std::uint32_t source, std::uint32_t destination,
    std::uint32_t width, std::uint32_t height, std::uint32_t format, int flip), {
    return globalThis.browserWebGlRenderer?.copyDisplaySurface?.({
        sourceSurfaceIdentifier: source, destinationSurfaceIdentifier: destination,
        width, height, colorFormat: format, flipVertically: Boolean(flip)}) ?? 0;
});
EM_JS(int, ReadDisplaySurface, (std::uint32_t identifier, std::uint8_t* destination), {
    const data = globalThis.browserWebGlRenderer?.readSurface(String(identifier));
    if (!data) return 0;
    HEAPU8.set(data, destination);
    return 1;
});
EM_JS(void, DeleteDisplaySurface, (std::uint32_t identifier), {
    globalThis.browserWebGlRenderer?.deleteSurface(String(identifier));
});

bool Overlaps(const DisplaySurface& surface, std::uint32_t address, std::uint32_t bytes) {
    return std::uint64_t(surface.address) < std::uint64_t(address) + bytes &&
        std::uint64_t(address) < std::uint64_t(surface.address) + surface.bytes;
}
void ChangeCachedPages(const DisplaySurface& surface, bool retain) {
    const std::uint64_t end = std::uint64_t(surface.address) + surface.bytes;
    for (std::uint64_t address = surface.address & ~(page_bytes - 1); address < end; address += page_bytes) {
        const auto page = static_cast<std::uint32_t>(address);
        if (retain) {
            auto& references = cached_pages[page];
            if (references++ == 0) surface.memory->RasterizerMarkRegionCached(page, page_bytes, true);
        } else {
            const auto found = cached_pages.find(page);
            if (found == cached_pages.end() || !found->second)
                throw std::logic_error("Display-surface page ownership is unavailable");
            if (--found->second == 0) {
                surface.memory->RasterizerMarkRegionCached(page, page_bytes, false);
                cached_pages.erase(found);
            }
        }
    }
}
void Materialize(DisplaySurface& surface) {
    if (surface.materialized) return;
    if (!ReadDisplaySurface(surface.identifier, surface.rgba.data()))
        throw std::runtime_error("Display-surface GPU readback is unavailable");
    auto* destination = surface.memory->GetPhysicalPointer(surface.address);
    if (!destination) throw std::runtime_error("Display-surface physical memory is unavailable");
    const auto format = static_cast<Pica::PixelFormat>(surface.format);
    const auto bytes_per_pixel = Pica::BytesPerPixel(format);
    for (std::uint32_t y = 0; y < surface.height; ++y) {
        for (std::uint32_t x = 0; x < surface.width; ++x) {
            const auto offset = (std::size_t(y) * surface.width + x) * 4;
            const Common::Vec4<std::uint8_t> color(surface.rgba[offset], surface.rgba[offset + 1],
                surface.rgba[offset + 2], surface.rgba[offset + 3]);
            auto* pixel = destination + (std::size_t(surface.height - 1 - y) * surface.width + x) * bytes_per_pixel;
            switch (format) {
            case Pica::PixelFormat::RGBA8: Common::Color::EncodeRGBA8(color, pixel); break;
            case Pica::PixelFormat::RGB8: Common::Color::EncodeRGB8(color, pixel); break;
            case Pica::PixelFormat::RGB565: Common::Color::EncodeRGB565(color, pixel); break;
            case Pica::PixelFormat::RGB5A1: Common::Color::EncodeRGB5A1(color, pixel); break;
            case Pica::PixelFormat::RGBA4: Common::Color::EncodeRGBA4(color, pixel); break;
            }
        }
    }
    surface.materialized = true;
}
}

bool Port::TransferBrowserWebGlDisplaySurface(Memory::MemorySystem& memory,
    const Pica::DisplayTransferConfig& configuration) {
    static unsigned diagnostic_transfers = 0;
    if (diagnostic_transfers++ < 8) std::fprintf(stderr,
        "browser display transfer enabled=%d input=%08x output=%08x extent=%ux%u:%ux%u format=%u:%u linear=%u swizzle=%u crop=%u block=%u texture=%u scale=%u flip=%u\n",
        PresentationEnabled(), configuration.GetPhysicalInputAddress(), configuration.GetPhysicalOutputAddress(),
        unsigned(configuration.input_width.Value()), unsigned(configuration.input_height.Value()),
        unsigned(configuration.output_width.Value()), unsigned(configuration.output_height.Value()),
        unsigned(configuration.input_format.Value()), unsigned(configuration.output_format.Value()),
        unsigned(configuration.input_linear.Value()), unsigned(configuration.dont_swizzle.Value()),
        unsigned(configuration.crop_input_lines.Value()), unsigned(configuration.block_32.Value()),
        unsigned(configuration.is_texture_copy.Value()), unsigned(configuration.scaling.Value()),
        unsigned(configuration.flip_vertically.Value()));
    if (!PresentationEnabled() || configuration.input_linear || configuration.dont_swizzle ||
        configuration.block_32 || configuration.is_texture_copy ||
        configuration.scaling != Pica::DisplayTransferConfig::NoScale ||
        configuration.input_width != configuration.output_width ||
        configuration.input_height < configuration.output_height ||
        configuration.output_format.Value() > Pica::PixelFormat::RGBA4) return false;
    const std::uint32_t width = configuration.output_width, height = configuration.output_height;
    const auto address = configuration.GetPhysicalOutputAddress();
    const auto format = static_cast<std::uint32_t>(configuration.output_format.Value());
    const std::uint64_t bytes = std::uint64_t(width) * height * Pica::BytesPerPixel(configuration.output_format);
    const auto end = std::uint64_t(address) + bytes;
    const bool cacheable = (address >= Memory::VRAM_PADDR && end <= Memory::VRAM_PADDR_END) ||
        (address >= Memory::FCRAM_PADDR && end <= Memory::FCRAM_N3DS_PADDR_END);
    if (!width || !height || width > 1024 || height > 1024 || bytes > 4 * 1024 * 1024 ||
        !cacheable ||
        std::uint64_t(address) + bytes > std::numeric_limits<std::uint32_t>::max() ||
        !memory.IsValidPhysicalAddress(address) || !memory.IsValidPhysicalAddress(address + bytes - 1)) return false;
    std::uint32_t source;
    if (!ResolveBrowserWebGlRenderSurface(configuration.GetPhysicalInputAddress(),
        configuration.input_width, configuration.input_height,
        static_cast<std::uint32_t>(configuration.input_format.Value()), source)) return false;
    FinishBrowserWebGlDraw();
    auto found = display_surfaces.find(address);
    if (found != display_surfaces.end() && (found->second.width != width ||
        found->second.height != height || found->second.format != format)) {
        InvalidateBrowserWebGlDisplayRegion(address, found->second.bytes);
        found = display_surfaces.end();
    }
    if (found == display_surfaces.end()) {
        if (display_surfaces.size() >= 8 || next_identifier == std::numeric_limits<std::uint32_t>::max()) return false;
        InvalidateBrowserWebGlDisplayRegion(address, static_cast<std::uint32_t>(bytes));
    }
    const auto identifier = found == display_surfaces.end() ? next_identifier++ : found->second.identifier;
    if (CopyDisplaySurface(source, identifier, width, height, format, configuration.flip_vertically) != 1) return false;
    if (found == display_surfaces.end()) {
        auto [entry, inserted] = display_surfaces.emplace(address, DisplaySurface{identifier, address,
            width, height, format, static_cast<std::uint32_t>(bytes), &memory,
            std::vector<std::uint8_t>(std::size_t(width) * height * 4)});
        if (!inserted) throw std::logic_error("Display-surface publication collided");
        ChangeCachedPages(entry->second, true);
    } else found->second.materialized = false;
    return true;
}

bool Port::PresentBrowserWebGlDisplayFrame(Memory::MemorySystem&, Pica::PicaCore& pica,
    std::uint64_t renderer_frame, std::uint64_t sampled_ticks) {
    static const bool diagnostic_termination = [] {
        std::set_terminate([] {
            try { if (std::current_exception()) std::rethrow_exception(std::current_exception()); }
            catch (const std::exception& error) { std::fprintf(stderr, "browser native termination: %s\n", error.what()); }
            catch (...) { std::fprintf(stderr, "browser native termination: non-standard exception\n"); }
            std::abort();
        });
        return true;
    }();
    (void)diagnostic_termination;
    if (!PresentationEnabled() || pica.regs_lcd.color_fill_top.is_enabled ||
        pica.regs_lcd.color_fill_bottom.is_enabled) return false;
    std::array<BrowserWebGlPresentationScreen, 2> screens;
    static unsigned diagnostic_frames = 0;
    static std::array<std::uint32_t, 4> last_framebuffer_addresses;
    const std::array<std::uint32_t, 4> addresses{pica.regs.framebuffer_config[0].address_left1,
        pica.regs.framebuffer_config[0].address_left2, pica.regs.framebuffer_config[1].address_left1,
        pica.regs.framebuffer_config[1].address_left2};
    if (addresses != last_framebuffer_addresses && diagnostic_frames++ < 12) {
        last_framebuffer_addresses = addresses;
        for (unsigned index = 0; index < 2; ++index) {
            const auto& fb = pica.regs.framebuffer_config[index];
            std::fprintf(stderr, "browser display framebuffer screen=%u active=%u address=%08x:%08x height=%u stride=%u format=%u aliases=%zu\n",
                index, unsigned(fb.active_fb), unsigned(fb.address_left1), unsigned(fb.address_left2),
                unsigned(fb.height.Value()), unsigned(fb.stride), unsigned(fb.color_format.Value()), display_surfaces.size());
        }
    }
    for (std::uint32_t index = 0; index < 2; ++index) {
        const auto& framebuffer = pica.regs.framebuffer_config[index];
        const auto address = framebuffer.active_fb == 0 ? framebuffer.address_left1 : framebuffer.address_left2;
        const auto found = display_surfaces.find(address);
        if (found == display_surfaces.end()) return false;
        const auto& surface = found->second;
        if (framebuffer.color_format.Value() != static_cast<Pica::PixelFormat>(surface.format) ||
            framebuffer.stride != surface.width * Pica::BytesPerPixel(framebuffer.color_format) ||
            framebuffer.height != surface.height) return false;
        screens[index] = {index == 0 ? 0u : 2u, surface.identifier, 0, 0, surface.width,
            surface.height, surface.height, surface.width, 3, 0, surface.format};
    }
    const auto result = TryPresentBrowserWebGlFrame(screens.data(), screens.size(), renderer_frame, sampled_ticks);
    if (result == BrowserWebGlPresentationResult::Unsupported) return false;
    handled_frame = renderer_frame;
    return true;
}
bool Port::BrowserWebGlDisplayFrameHandled(std::uint64_t renderer_frame) {
    return renderer_frame != 0 && renderer_frame == handled_frame;
}
void Port::FlushBrowserWebGlDisplayRegion(std::uint32_t address, std::uint32_t bytes) {
    if (!bytes) return;
    for (auto& [key, surface] : display_surfaces) if (Overlaps(surface, address, bytes)) Materialize(surface);
}
void Port::InvalidateBrowserWebGlDisplayRegion(std::uint32_t address, std::uint32_t bytes) {
    if (!bytes) return;
    for (auto iterator = display_surfaces.begin(); iterator != display_surfaces.end();) {
        if (!Overlaps(iterator->second, address, bytes)) { ++iterator; continue; }
        Materialize(iterator->second);
        DeleteDisplaySurface(iterator->second.identifier);
        ChangeCachedPages(iterator->second, false);
        iterator = display_surfaces.erase(iterator);
    }
}
void Port::FlushBrowserWebGlDisplaySurfaces() {
    for (auto& [key, surface] : display_surfaces) Materialize(surface);
}
