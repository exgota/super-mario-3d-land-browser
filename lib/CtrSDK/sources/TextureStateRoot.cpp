// NonMatching, retail EU 003910C0..0039211C. See the branch report.
#include <retail/TextureStateRoot.h>

#ifdef NON_MATCHING
namespace retail_texture_state {
#define CHECK_OFFSET(type, field, value) \
    typedef char check_##type##_##field[(__builtin_offsetof(type, field) == value) ? 1 : -1]
CHECK_OFFSET(Texture, image, 0x34);
CHECK_OFFSET(Image, mipCount, 0x34);
CHECK_OFFSET(ControlView, textureEnabled, 0x104);
CHECK_OFFSET(ControlView, cache, 0x670);
CHECK_OFFSET(Cache, unit1, 0x44);
CHECK_OFFSET(Cache, unit2, 0x64);
CHECK_OFFSET(Cache, format0, 0x38);
#undef CHECK_OFFSET

static inline Word encodeBias(float bias)
{
    union { float f; Word u; int i; } value;
    value.f = bias;
    if (bias == 0.0f || ((value.u << 1) >> 24) == 255)
        return 0;
    value.f = (bias + 16.0f) * 256.0f;
    if (value.f < 0.0f)
        value.f = 0.0f;
    else if (value.i >= 0x46000000)
        value.f = 8191.0f;
    if (value.i >= 0x45800000)
        return static_cast<Word>(value.f - 4096.0f);
    return static_cast<Word>(value.f + 4096.0f);
}

static inline void setWrap(Word& parameters, Word wrap, unsigned shift)
{
    switch (wrap) {
    case 0x2901: parameters = (parameters & ~(7u << shift)) | (2u << shift); break;
    case 0x812d: parameters = (parameters & ~(7u << shift)) | (1u << shift); break;
    case 0x812f: parameters &= ~(7u << shift); break;
    case 0x8370: parameters = (parameters & ~(7u << shift)) | (3u << shift); break;
    }
}

static inline void setFilters(Unit& cache, Texture* texture)
{
    switch (texture->magnification) {
    case 0x2600: cache.parameters &= ~2u; break;
    case 0x2601: cache.parameters |= 2u; break;
    }
    switch (texture->minification) {
    case 0x2600:
    case 0x2601:
        if (texture->minification == 0x2600) cache.parameters &= ~4u;
        else cache.parameters |= 4u;
        cache.lod &= ~0x0f0f0000u;
        cache.parameters &= ~0x01000000u;
        break;
    case 0x2700:
    case 0x2701:
    case 0x2702:
    case 0x2703:
        if (texture->minification & 1) cache.parameters |= 4u;
        else cache.parameters &= ~4u;
        cache.lod = (cache.lod & ~0x000f0000u) | ((texture->image.mipCount - 1) << 16 & 0x000f0000u);
        cache.lod = (cache.lod & ~0x0f000000u) | (static_cast<Word>(texture->minimumLod < 0 ? 0 : texture->minimumLod) << 24 & 0x0f000000u);
        if (texture->minification >= 0x2702) cache.parameters |= 0x01000000;
        else cache.parameters &= ~0x01000000u;
        break;
    }
}

static inline void dimensions(Unit& cache, const Image& image)
{
    cache.dimensions = (cache.dimensions & ~0x07ff0000u) | (image.width << 16 & 0x07ff0000u);
    cache.dimensions = (cache.dimensions & ~0x000007ffu) | (image.height & 0x7ff);
}

static inline void commonSampling(Unit& cache, Texture* texture)
{
    setWrap(cache.parameters, texture->wrapS, 12);
    setWrap(cache.parameters, texture->wrapT, 8);
    setFilters(cache, texture);
}

static inline bool ordinaryFormat(Word format)
{
    switch (format) {
    case 0x1906: case 0x1907: case 0x1908: case 0x1909: case 0x190a:
    case 0x6700: case 0x675a: case 0x675b: return true;
    default: return false;
    }
}

template<unsigned Index> static inline unsigned secondary(Texture* texture, ControlView* control, Word border)
{
    Cache& all = control->cache;
    Unit& cache = Index == 1 ? all.unit1 : all.unit2;
    cache.lod = (cache.lod & ~0x1fffu) | (encodeBias(texture->lodBias) & 0x1fff);
    cache.parameters &= ~0xc0u;
    if (ordinaryFormat(texture->image.format)) {
        commonSampling(cache, texture);
        cache.border = border;
    } else if (texture->image.format == 0x6050) {
        cache.parameters &= ~6u;
        cache.lod &= ~0x0f0f1fffu;
        cache.border = 0;
        cache.parameters &= ~0x7700u;
    } else {
        return 0x502;
    }
    cache.parameters = (cache.parameters & ~0x30u) | (texture->image.format == 0x675a ? 0x20 : 0);
    dimensions(cache, texture->image);
    Word physical = fn_00289C1C(texture->image.data);
    cache.address = (cache.address & 0xc0000000u) | (physical >> 3);
    all.enabled |= 1u << Index;
    Word& format = Index == 1 ? all.format1 : all.format2;
    format = (format & ~15u) | (texture->image.packedFormat & 15);
    __cb_writeRegs(Index == 1 ? 0x91 : 0x99, 6, &cache.border);
    return 0;
}
}

extern "C" unsigned fn_003910C0(retail_texture_state::Texture* texture, int unit)
{
    using namespace retail_texture_state;
    ControlView* control = reinterpret_cast<ControlView*>(dat_003E3154);
    Cache& all = control->cache;
    unsigned error = texture->image.data == 0 ? 0x502 : 0;
    Byte* enabled = reinterpret_cast<Byte*>(control) + 0x104 + unit;
    if (*enabled == 0 || error != 0) {
        switch (unit) {
        case 0: all.enabled &= ~1u; break;
        case 1: all.enabled &= ~2u; break;
        case 2: all.enabled &= ~4u; break;
        default: return 0x501;
        }
        return *enabled ? error : 0;
    }
    Word border = static_cast<Word>(texture->border[0] * 255.0f)
                | static_cast<Word>(texture->border[1] * 255.0f) << 8
                | static_cast<Word>(texture->border[2] * 255.0f) << 16
                | static_cast<Word>(texture->border[3] * 255.0f) << 24;
    switch (unit) {
    case 1: return secondary<1>(texture, control, border);
    case 2: return secondary<2>(texture, control, border);
    case 0: break;
    default: return 0;
    }
    Unit& cache = all.unit0;
    cache.parameters &= ~0x100000u;
    cache.border = border;
    dimensions(cache, texture->image);
    cache.lod = (cache.lod & ~0x1fffu) | (encodeBias(texture->lodBias) & 0x1fff);
    all.format0 = (all.format0 & ~15u) | (texture->image.packedFormat & 15);
    if (ordinaryFormat(texture->image.format)) {
        cache.parameters = (cache.parameters & ~0x70000000u) | (control->textureMode[0] == 0x6e00 ? 0x30000000 : 0);
        commonSampling(cache, texture);
        cache.parameters = (cache.parameters & ~0x30u) | (texture->image.format == 0x675a ? 0x20 : 0);
    } else if (texture->image.format == 0x6040) {
        cache.parameters = (cache.parameters & ~0x71000000u) | 0x20100006;
        cache.lod &= ~0x0f0f1fffu;
        cache.parameters = (cache.parameters & ~0x7730u) | 0x1100;
    } else if (texture->image.format == 0x6050) {
        cache.parameters &= ~0x70000036u;
        cache.lod &= ~0x0f0f1fffu;
        cache.border = 0;
        cache.parameters &= ~0x7700u;
    } else {
        return 0x502;
    }
    Word physical = fn_00289C1C(texture->image.data);
    cache.address = (cache.address & 0xc0000000u) | (physical >> 3);
    all.enabled |= 1;
    __cb_writeRegs(0x81, 10, &cache.border);
    Word* out = dat_003E2E30;
    if (out < dat_003E2E34) {
        *out++ = all.format0;
        *out++ = 0x000f008e;
        dat_003E2E30 = out;
    }
    return 0;
}
#endif
