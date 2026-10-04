// Independent raw vertex submission using documented PICA register interfaces.
// No existing-platform vertex loader, shader generator or renderer is copied here.
#include "BrowserWebGlVertexSubmission.h"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <emscripten/emscripten.h>
#include "core/memory.h"
#include "video_core/pica/pica_core.h"

namespace {
using Word = std::uint32_t;
constexpr Word missing_buffer = std::numeric_limits<Word>::max();
Port::BrowserWebGlVertexSubmissionStatistics statistics;

struct AttributeDescription {
    Word fixed{}, format{}, components{}, buffer{missing_buffer}, offset{}, stride{}, index{}, reg{};
};
struct BufferDescription {
    Word address{}, pointer{}, bytes{}, reserved{};
};
static_assert(sizeof(AttributeDescription) == 8 * sizeof(Word));
static_assert(sizeof(BufferDescription) == 4 * sizeof(Word));

// Schema1 header: topology,count,index bytes,vertex offset,entry,output mask/count,
// input count,buffer count,clip,input-to-uniform,bool mask,program/swizzle lengths,
// program hash low/high,swizzle hash low/high. All extents are validated natively.
EM_JS(int, SubmitVertexBatch, (const Word* header, const Word* attributes,
                             const Word* buffers, const Word* indices,
                             const Word* program, const Word* swizzles,
                             const Word* input_mapping, const Word* output_mapping,
                             const float* float_uniforms, const Word* integer_uniforms,
                             const float* default_attributes), {
    const renderer = globalThis.browserWebGlRenderer;
    if (!renderer || typeof renderer.tryDrawVertexBatch !== 'function') return 0;
    const state = HEAPU32.subarray(header >>> 2, (header >>> 2) + 18);
    const inputCount = state[7], bufferCount = state[8];
    const inputAttributes = [];
    for (let index = 0; index < inputCount; ++index) {
        const offset = (attributes >>> 2) + index * 8;
        const words = HEAPU32.subarray(offset, offset + 8);
        inputAttributes.push({index:words[6], default:words[0] !== 0, format:words[1],
            components:words[2], bufferIndex:words[3], byteOffset:words[4], byteStride:words[5]});
    }
    const inputBuffers = [];
    for (let index = 0; index < bufferCount; ++index) {
        const offset = (buffers >>> 2) + index * 4;
        const words = HEAPU32.subarray(offset, offset + 4);
        inputBuffers.push({physicalAddress:words[0], data:HEAPU8.subarray(words[1], words[1] + words[2])});
    }
    const indexWords = HEAPU32.subarray(indices >>> 2, (indices >>> 2) + 4);
    const descriptor = {schemaVersion:1, primitiveType:state[0], vertexCount:state[1],
        indexFormat:state[2], vertexOffset:state[3], entryPoint:state[4],
        outputMask:state[5], outputCount:state[6], inputAttributeCount:inputCount,
        clipEnabled:state[9] !== 0, inputToUniform:state[10], booleanUniforms:state[11],
        programIdentity:Array.from(state.subarray(14, 18)).join(':'),
        programWords:HEAPU32.subarray(program >>> 2, (program >>> 2) + state[12]),
        swizzleWords:HEAPU32.subarray(swizzles >>> 2, (swizzles >>> 2) + state[13]),
        inputRegisterMapping:HEAPU32.subarray(input_mapping >>> 2, (input_mapping >>> 2) + 16),
        outputMapping:HEAPU32.subarray(output_mapping >>> 2, (output_mapping >>> 2) + 7),
        floatUniforms:HEAPF32.subarray(float_uniforms >>> 2, (float_uniforms >>> 2) + 384),
        integerUniforms:HEAPU32.subarray(integer_uniforms >>> 2, (integer_uniforms >>> 2) + 16),
        defaultAttributes:HEAPF32.subarray(default_attributes >>> 2, (default_attributes >>> 2) + 64),
        attributes:inputAttributes, buffers:inputBuffers,
        indexBuffer:state[2] ? {physicalAddress:indexWords[0],
            data:HEAPU8.subarray(indexWords[1], indexWords[1] + indexWords[2])} : null};
    // The renderer/cache consumes this synchronously and owns copies of retained data.
    // These heap views must never be published directly to an asynchronous GPU queue.
    return renderer.tryDrawVertexBatch(descriptor) ? 1 : 0;
});

EM_JS(void, InvalidateVertexResources, (Word address, Word bytes), {
    globalThis.browserWebGlRenderer?.vertexDrawCache?.invalidatePhysicalRange(address, bytes);
});

bool PhysicalRange(Memory::MemorySystem& memory, std::uint64_t address,
                   std::uint64_t bytes, BufferDescription& result) {
    if (!bytes || address > std::numeric_limits<Word>::max() ||
        bytes > std::numeric_limits<Word>::max() || address + bytes > (std::uint64_t{1} << 32)) {
        ++statistics.invalid_memory_ranges;
        return false;
    }
    auto reference = memory.GetPhysicalRef(static_cast<Word>(address));
    auto last = memory.GetPhysicalRef(static_cast<Word>(address + bytes - 1));
    if (!reference || reference.GetSize() < bytes || !last ||
        last.GetPtr() != reference.GetPtr() + bytes - 1) {
        ++statistics.invalid_memory_ranges;
        return false;
    }
    result = {static_cast<Word>(address),
        static_cast<Word>(reinterpret_cast<std::uintptr_t>(reference.GetPtr())),
        static_cast<Word>(bytes), 0};
    return true;
}

bool BuildVertexSubmission(Memory::MemorySystem& memory, Pica::PicaCore& pica, bool indexed) {
    const auto& registers = pica.regs.internal;
    const auto& pipeline = registers.pipeline;
    const auto& configuration = pipeline.vertex_attributes;
    const Word count = pipeline.num_vertices;
    const Word topology = static_cast<Word>(pipeline.triangle_topology.Value());
    if (pipeline.use_gs != Pica::PipelineRegs::UseGS::No || (topology != 0 && topology != 3) ||
        !count || count % 3 || count > static_cast<Word>(std::numeric_limits<int>::max()) ||
        registers.rasterizer.clip_enable || registers.vs.input_to_uniform) return false;
    const Word input_count = static_cast<Word>(registers.vs.max_input_attribute_index) + 1;
    if (input_count > configuration.GetNumTotalAttributes()) return false;
    const std::uint64_t base_address = configuration.GetPhysicalBaseAddress();
    BufferDescription indices{};
    Word index_bytes = 0;
    std::uint64_t maximum_vertex = static_cast<Word>(pipeline.vertex_offset);
    if (indexed) {
        index_bytes = static_cast<Word>(pipeline.index_array.format) == 1 ? 2 : 1;
        if (!PhysicalRange(memory, base_address + static_cast<Word>(pipeline.index_array.offset),
                           static_cast<std::uint64_t>(count) * index_bytes, indices)) return false;
        const auto* data = reinterpret_cast<const std::uint8_t*>(static_cast<std::uintptr_t>(indices.pointer));
        maximum_vertex = 0;
        // The only per-index CPU work is a little-endian extent scan. No attributes or shader
        // outputs are decoded, expanded, transformed or assembled on the CPU.
        for (Word index = 0; index < count; ++index) {
            const std::size_t offset = static_cast<std::size_t>(index) * index_bytes;
            Word value = data[offset];
            if (index_bytes == 2) value |= static_cast<Word>(data[offset + 1]) << 8;
            maximum_vertex = std::max(maximum_vertex, static_cast<std::uint64_t>(value));
        }
        statistics.scanned_index_bytes += indices.bytes;
    } else {
        maximum_vertex += count - 1;
        if (maximum_vertex > std::numeric_limits<Word>::max()) return false;
    }
    std::array<AttributeDescription, 16> attributes{};
    std::array<Word, 12> loader_offsets{};
    for (Word index = 0; index < input_count; ++index) {
        auto& attribute = attributes[index];
        attribute.fixed = configuration.IsDefaultAttribute(index);
        attribute.index = index;
        attribute.reg = registers.vs.GetRegisterForAttribute(index);
    }
    // Interpret each loader's component list and alignment once per draw. Later loaders own
    // repeated attribute declarations. Padding components never become shader inputs.
    for (Word loader = 0; loader < 12; ++loader) {
        const auto& source = configuration.attribute_loaders[loader];
        const Word component_count = source.component_count;
        if (component_count > 12) return false;
        Word offset = 0;
        loader_offsets[loader] = source.data_offset;
        for (Word component = 0; component < component_count; ++component) {
            const Word index = source.GetComponent(component);
            if (index >= 12) {
                offset = (offset + 3) & ~Word{3};
                offset += (index - 11) * 4;
                continue;
            }
            const Word size = configuration.GetElementSizeInBytes(index);
            offset = (offset + size - 1) & ~(size - 1);
            if (index < input_count && !attributes[index].fixed) {
                auto& attribute = attributes[index];
                attribute.format = static_cast<Word>(configuration.GetFormat(index));
                attribute.components = configuration.GetNumElements(index);
                attribute.buffer = loader;
                attribute.offset = offset;
                attribute.stride = source.byte_count;
            }
            offset += configuration.GetStride(index);
        }
    }
    std::array<BufferDescription, 12> buffers{};
    std::array<Word, 12> buffer_indices{};
    buffer_indices.fill(missing_buffer);
    std::array<std::uint64_t, 12> buffer_extents{};
    for (Word index = 0; index < input_count; ++index) {
        const auto& attribute = attributes[index];
        if (attribute.fixed) continue;
        if (attribute.buffer == missing_buffer || !attribute.components) return false;
        const Word size = attribute.format == 3 ? 4 : attribute.format == 2 ? 2 : 1;
        const auto extent = attribute.offset + maximum_vertex * attribute.stride + attribute.components * size;
        buffer_extents[attribute.buffer] = std::max(buffer_extents[attribute.buffer], extent);
    }
    Word buffer_count = 0;
    for (Word loader = 0; loader < 12; ++loader) {
        if (!buffer_extents[loader]) continue;
        if (!PhysicalRange(memory, base_address + loader_offsets[loader], buffer_extents[loader],
                           buffers[buffer_count])) return false;
        buffer_indices[loader] = buffer_count++;
    }
    for (Word index = 0; index < input_count; ++index)
        if (!attributes[index].fixed) attributes[index].buffer = buffer_indices[attributes[index].buffer];
    std::array<Word, 16> input_mapping{};
    for (Word index = 0; index < 16; ++index) input_mapping[index] = registers.vs.GetRegisterForAttribute(index);
    std::array<Word, 7> output_mapping{};
    for (Word index = 0; index < 7; ++index) output_mapping[index] = registers.rasterizer.vs_output_attributes[index].raw;
    std::array<float, 384> float_uniforms{};
    for (Word index = 0; index < 96; ++index)
        for (Word component = 0; component < 4; ++component)
            float_uniforms[index * 4 + component] = pica.vs_setup.uniforms.f[index][component].ToFloat32();
    std::array<Word, 16> integer_uniforms{};
    for (Word index = 0; index < 4; ++index)
        for (Word component = 0; component < 4; ++component)
            integer_uniforms[index * 4 + component] = pica.vs_setup.uniforms.i[index][component];
    Word boolean_uniforms = 0;
    for (Word index = 0; index < 16; ++index)
        if (pica.vs_setup.uniforms.b[index]) boolean_uniforms |= Word{1} << index;
    std::array<float, 64> default_attributes{};
    for (Word index = 0; index < 16; ++index)
        for (Word component = 0; component < 4; ++component)
            default_attributes[index * 4 + component] = pica.input_default_attributes[index][component].ToFloat32();
    const auto program_hash = pica.vs_setup.GetProgramCodeHash();
    const auto swizzle_hash = pica.vs_setup.GetSwizzleDataHash();
    const std::array<Word, 18> header{topology, count, index_bytes, static_cast<Word>(pipeline.vertex_offset),
        static_cast<Word>(registers.vs.main_offset), static_cast<Word>(registers.vs.output_mask),
        static_cast<Word>(registers.rasterizer.vs_output_total), input_count, buffer_count,
        static_cast<Word>(registers.rasterizer.clip_enable), static_cast<Word>(registers.vs.input_to_uniform),
        boolean_uniforms, Pica::MAX_PROGRAM_CODE_LENGTH, Pica::MAX_SWIZZLE_DATA_LENGTH,
        static_cast<Word>(program_hash), static_cast<Word>(program_hash >> 32),
        static_cast<Word>(swizzle_hash), static_cast<Word>(swizzle_hash >> 32)};
    Port::PrepareBrowserWebGlDraw(memory, pica);
    return SubmitVertexBatch(header.data(), reinterpret_cast<const Word*>(attributes.data()),
        reinterpret_cast<const Word*>(buffers.data()), reinterpret_cast<const Word*>(&indices),
        pica.vs_setup.GetProgramCode().data(), pica.vs_setup.GetSwizzleData().data(),
        input_mapping.data(), output_mapping.data(), float_uniforms.data(), integer_uniforms.data(),
        default_attributes.data()) != 0;
}
}

bool Port::TryDrawBrowserWebGlVertexBatch(Memory::MemorySystem& memory, Pica::PicaCore& pica,
                                       bool indexed) {
    if (!BuildVertexSubmission(memory, pica, indexed)) {
        ++statistics.fallback_draws;
        return false;
    }
    ++statistics.submitted_draws;
    statistics.submitted_vertices += static_cast<Word>(pica.regs.internal.pipeline.num_vertices);
    return true;
}

void Port::InvalidateBrowserWebGlVertexResources(Word address, Word bytes) {
    InvalidateVertexResources(address, bytes);
}

const Port::BrowserWebGlVertexSubmissionStatistics& Port::GetBrowserWebGlVertexSubmissionStatistics() {
    return statistics;
}
