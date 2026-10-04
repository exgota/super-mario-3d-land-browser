// Independent GPU bridge using existing platform interfaces. No renderer code is copied.
#include "BrowserWebGlBridge.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <map>
#include <memory>
#include <stdexcept>
#include <vector>
#include <emscripten/emscripten.h>
#include "core/memory.h"
#include "video_core/pica/output_vertex.h"
#include "video_core/pica/pica_core.h"
#include "video_core/renderer_software/sw_framebuffer.h"

namespace {
struct Surface {
    std::uint32_t identifier, color_address, depth_address, width, height;
    Pica::FramebufferRegs registers;
    Memory::MemorySystem* memory;
    std::vector<std::uint8_t> color;
    std::vector<std::uint32_t> depth_stencil;
};
std::map<std::array<std::uint32_t, 5>, std::unique_ptr<Surface>> surfaces;
std::map<std::array<std::uint32_t, 4>, std::uint32_t> texture_revisions;
std::uint32_t next_surface_identifier = 1;
bool draw_open = false;

std::uint32_t Hash(const void* bytes, std::size_t size) {
    const auto* data = static_cast<const std::uint8_t*>(bytes);
    std::uint32_t result = 2166136261u;
    for (std::size_t index = 0; index < size; ++index) result = (result ^ data[index]) * 16777619u;
    return result;
}

EM_JS(void, BeginDraw, (const std::uint32_t* registers, const float* viewport,
                       std::uint32_t identifier, std::uint32_t width, std::uint32_t height,
                       const std::uint8_t* color, const std::uint32_t* depth,
                       const std::uint32_t* textures, const float* lighting,
                       std::uint32_t lighting_revision, const float* fog,
                       std::uint32_t fog_revision), {
    if (!globalThis.browserWebGlRenderer) {
        globalThis.browserWebGlRenderer = globalThis.createBrowserWebGlRenderer({
            convertTexture: globalThis.convertPicaTexture,
        });
    }
    const renderer = globalThis.browserWebGlRenderer;
    const descriptors = [];
    for (let index = 0; index < 3; ++index) {
        const offset = (textures >>> 2) + index * 8;
        const words = HEAPU32.subarray(offset, offset + 8);
        descriptors.push({enabled: words[0] !== 0,
            key: [words[1], words[2], words[3], words[4], words[7]].join(':'),
            width: words[2], height: words[3], format: words[4],
            data: HEAPU8.subarray(words[5], words[5] + words[6]),
            wrapS: 0, wrapT: 0});
    }
    const state = HEAPU32.slice(registers >>> 2, (registers >>> 2) + 512);
    for (let index = 0; index < 3; ++index) {
        const parameter = state[[0x83, 0x93, 0x9b][index]];
        descriptors[index].wrapS = (parameter >>> 12) & 7;
        descriptors[index].wrapT = (parameter >>> 8) & 7;
        if (!descriptors[index].enabled) {
            descriptors[index].wrapS = 0; descriptors[index].wrapT = 0;
        }
    }
    const geometry = HEAPF32.slice(viewport >>> 2, (viewport >>> 2) + 6);
    const key = String(identifier);
    renderer.beginDraw({registers: state, viewport: Array.from(geometry.subarray(0, 4)),
        depthConfiguration: geometry.subarray(4, 6),
        surface: {key, width, height,
            color: renderer.surfaces.has(key) ? null : HEAPU8.slice(color, color + width * height * 4),
            depthStencil: renderer.surfaces.has(key) ? null : HEAPU32.slice(depth >>> 2, (depth >>> 2) + width * height)},
        textures: descriptors,
        lightingLookup: {revision: lighting_revision,
            data: renderer.lightingRevision === lighting_revision ? null : HEAPF32.slice(lighting >>> 2, (lighting >>> 2) + 256 * 24 * 2)},
        fogLookup: {revision: fog_revision,
            data: renderer.fogRevision === fog_revision ? null : HEAPF32.slice(fog >>> 2, (fog >>> 2) + 128 * 2)}});
});

EM_JS(void, AppendTriangle, (const float* vertices), {
    globalThis.browserWebGlRenderer.appendTriangle(HEAPF32.subarray(vertices >>> 2, (vertices >>> 2) + 66));
});
EM_JS(void, EndDraw, (), {
    globalThis.browserWebGlRenderer?.endDraw();
});
EM_JS(int, ReadColor, (std::uint32_t identifier, std::uint8_t* destination), {
    const pixels = globalThis.browserWebGlRenderer?.readSurface(String(identifier));
    if (!pixels) return 0;
    HEAPU8.set(pixels, destination);
    return 1;
});
EM_JS(void, DeleteSurface, (std::uint32_t identifier), {
    const renderer = globalThis.browserWebGlRenderer;
    if (!renderer) return;
    const key = String(identifier), surface = renderer.surfaces.get(key);
    if (!surface) return;
    if (surface.dirty) throw new Error('GPU surface must be synchronized before invalidation');
    renderer.gl.deleteFramebuffer(surface.framebuffer);
    renderer.gl.deleteTexture(surface.colorTexture);
    renderer.gl.deleteTexture(surface.depthTexture);
    renderer.surfaces.delete(key);
    if (renderer.currentSurface === surface) renderer.currentSurface = null;
});

Surface& GetSurface(Memory::MemorySystem& memory, const Pica::FramebufferRegs& registers) {
    const auto& framebuffer = registers.framebuffer;
    const std::array<std::uint32_t, 5> key{framebuffer.GetColorBufferPhysicalAddress(),
        framebuffer.GetDepthBufferPhysicalAddress(), framebuffer.GetWidth(), framebuffer.GetHeight(),
        static_cast<std::uint32_t>(framebuffer.color_format.Value()) |
            (static_cast<std::uint32_t>(framebuffer.depth_format.Value()) << 8)};
    if (auto found = surfaces.find(key); found != surfaces.end()) return *found->second;
    const auto width = key[2], height = key[3];
    if (surfaces.size() >= 16 || width == 0 || height == 0 || width > 1024 || height > 1024)
        throw std::runtime_error("GPU surface exceeds bounded first-play domain");
    auto surface = std::make_unique<Surface>(Surface{next_surface_identifier++, key[0], key[1],
        width, height, registers, &memory, std::vector<std::uint8_t>(width * height * 4),
        std::vector<std::uint32_t>(width * height)});
    const bool depth_available = key[1] && memory.GetPhysicalPointer(key[1]);
    if (!key[0] || !memory.GetPhysicalPointer(key[0]))
        throw std::runtime_error("GPU color target physical pointer is unavailable");
    if (!depth_available && (registers.output_merger.depth_test_enable ||
        registers.output_merger.depth_write_enable || registers.output_merger.stencil_test.enable))
        throw std::runtime_error("GPU depth target physical pointer is unavailable");
    SwRenderer::Framebuffer reader(memory, surface->registers);
    reader.Bind();
    for (std::uint32_t y = 0; y < height; ++y) {
        for (std::uint32_t x = 0; x < width; ++x) {
            const auto index = y * width + x;
            const auto pixel = reader.GetPixel(x, y);
            for (unsigned component = 0; component < 4; ++component) surface->color[index * 4 + component] = pixel[component];
            std::uint32_t depth = depth_available ? reader.GetDepth(x, y) : 0xffffff;
            if (framebuffer.depth_format == Pica::FramebufferRegs::DepthFormat::D16) depth = (depth << 8) | (depth >> 8);
            const auto stencil = depth_available && framebuffer.depth_format == Pica::FramebufferRegs::DepthFormat::D24S8 ? reader.GetStencil(x, y) : 0;
            surface->depth_stencil[index] = (depth << 8) | stencil;
        }
    }
    return *surfaces.emplace(key, std::move(surface)).first->second;
}

bool Overlaps(std::uint32_t first, std::uint32_t bytes, std::uint32_t address, std::uint64_t size) {
    return static_cast<std::uint64_t>(first) < static_cast<std::uint64_t>(address) + size &&
           static_cast<std::uint64_t>(address) < static_cast<std::uint64_t>(first) + bytes;
}
}

void Port::DrawBrowserWebGlTriangle(Memory::MemorySystem& memory, Pica::PicaCore& pica,
                                   const Pica::OutputVertex& first,
                                   const Pica::OutputVertex& second,
                                   const Pica::OutputVertex& third) {
    const auto& registers = pica.regs.internal;
    if (!draw_open) {
        auto& surface = GetSurface(memory, registers.framebuffer);
        const auto& rasterizer = registers.rasterizer;
        const std::array<float, 6> viewport{static_cast<float>(rasterizer.viewport_corner.x),
            static_cast<float>(rasterizer.viewport_corner.y),
            Pica::f24::FromRaw(rasterizer.viewport_size_x).ToFloat32() * 2,
            Pica::f24::FromRaw(rasterizer.viewport_size_y).ToFloat32() * 2,
            Pica::f24::FromRaw(rasterizer.viewport_depth_range).ToFloat32(),
            Pica::f24::FromRaw(rasterizer.viewport_depth_near_plane).ToFloat32()};
        std::array<std::uint32_t, 24> textures{};
        const auto configurations = registers.texturing.GetTextures();
        constexpr std::array<unsigned, 14> nibbles{8, 6, 4, 4, 4, 4, 4, 2, 2, 2, 1, 1, 1, 2};
        for (unsigned index = 0; index < 3; ++index) {
            const auto& texture = configurations[index];
            const auto width = static_cast<std::uint32_t>(texture.config.width);
            const auto height = static_cast<std::uint32_t>(texture.config.height);
            const auto format = static_cast<unsigned>(texture.format);
            const auto address = texture.config.GetPhysicalAddress();
            const bool enabled = texture.enabled && address && !(index == 0 && texture.config.type == Pica::TexturingRegs::TextureConfig::Disabled);
            if (!enabled) continue;
            if (format >= nibbles.size() || !width || !height || width > 2048 || height > 2048 || width % 8 || height % 8)
                throw std::runtime_error("Unsupported GPU first-play texture extent");
            const auto bytes = width * height * nibbles[format] / 2;
            const auto* data = memory.GetPhysicalPointer(address);
            if (!data) throw std::runtime_error("GPU texture physical pointer is unavailable");
            const std::array<std::uint32_t, 4> key{address, width, height, format};
            auto [revision, inserted] = texture_revisions.try_emplace(key, 0);
            if (inserted) revision->second = Hash(data, bytes);
            const std::array<std::uint32_t, 8> descriptor{1, address, width, height, format,
                static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(data)), bytes, revision->second};
            for (unsigned word = 0; word < 8; ++word) textures[index * 8 + word] = descriptor[word];
        }
        static std::array<float, 24 * 256 * 2> lighting{};
        static std::array<float, 128 * 2> fog{};
        const auto lighting_revision = Hash(pica.lighting.luts.data(), sizeof(pica.lighting.luts));
        const auto fog_revision = Hash(pica.fog.lut.data(), sizeof(pica.fog.lut));
        for (unsigned sampler = 0; sampler < 24; ++sampler) {
            for (unsigned index = 0; index < 256; ++index) {
                const auto& entry = pica.lighting.luts[sampler][index];
                lighting[(sampler * 256 + index) * 2] = entry.ToFloat();
                lighting[(sampler * 256 + index) * 2 + 1] = entry.DiffToFloat();
            }
        }
        for (unsigned index = 0; index < 128; ++index) {
            fog[index * 2] = pica.fog.lut[index].ToFloat();
            fog[index * 2 + 1] = pica.fog.lut[index].DiffToFloat();
        }
        BeginDraw(registers.reg_array.data(), viewport.data(), surface.identifier, surface.width,
            surface.height, surface.color.data(), surface.depth_stencil.data(), textures.data(),
            lighting.data(), lighting_revision, fog.data(), fog_revision);
        draw_open = true;
    }
    std::array<float, 66> data{};
    const std::array<const Pica::OutputVertex*, 3> vertices{&first, &second, &third};
    for (unsigned index = 0; index < 3; ++index) {
        const auto& vertex = *vertices[index];
        auto* output = data.data() + index * 22;
        for (unsigned component = 0; component < 4; ++component) output[component] = vertex.pos[component].ToFloat32();
        for (unsigned component = 0; component < 4; ++component) output[4 + component] = vertex.color[component].ToFloat32();
        output[8] = vertex.tc0[0].ToFloat32(); output[9] = vertex.tc0[1].ToFloat32(); output[10] = vertex.tc0_w.ToFloat32();
        output[11] = vertex.tc1[0].ToFloat32(); output[12] = vertex.tc1[1].ToFloat32();
        output[13] = vertex.tc2[0].ToFloat32(); output[14] = vertex.tc2[1].ToFloat32();
        float orientation = 0;
        for (unsigned component = 0; component < 4; ++component)
            orientation += vertex.quat[component].ToFloat32() * first.quat[component].ToFloat32();
        for (unsigned component = 0; component < 4; ++component)
            output[15 + component] = vertex.quat[component].ToFloat32() * (orientation < 0 ? -1 : 1);
        for (unsigned component = 0; component < 3; ++component) output[19 + component] = vertex.view[component].ToFloat32();
    }
    AppendTriangle(data.data());
}

void Port::FinishBrowserWebGlDraw() {
    EndDraw();
    draw_open = false;
}

void Port::SynchronizeBrowserWebGlColor() {
    FinishBrowserWebGlDraw();
    for (auto& [key, surface] : surfaces) {
        if (!ReadColor(surface->identifier, surface->color.data())) continue;
        SwRenderer::Framebuffer writer(*surface->memory, surface->registers);
        writer.Bind();
        for (std::uint32_t y = 0; y < surface->height; ++y) {
            for (std::uint32_t x = 0; x < surface->width; ++x) {
                const auto offset = (y * surface->width + x) * 4;
                writer.DrawPixel(x, y, {surface->color[offset], surface->color[offset + 1],
                    surface->color[offset + 2], surface->color[offset + 3]});
            }
        }
    }
    texture_revisions.clear();
}

void Port::InvalidateBrowserWebGlSurfaces(std::uint32_t address, std::uint32_t bytes) {
    if (draw_open) throw std::runtime_error("GPU invalidation requires a closed draw batch");
    for (auto iterator = surfaces.begin(); iterator != surfaces.end();) {
        const auto& surface = *iterator->second;
        const auto pixels = static_cast<std::uint64_t>(surface.width) * surface.height;
        if (Overlaps(address, bytes, surface.color_address, pixels * 4) ||
            Overlaps(address, bytes, surface.depth_address, pixels * 4)) {
            DeleteSurface(surface.identifier);
            iterator = surfaces.erase(iterator);
        } else ++iterator;
    }
    texture_revisions.clear();
}
