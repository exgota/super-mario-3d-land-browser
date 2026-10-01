// NonMatching reconstruction of retail EU 0x0020ACAC..0x0020F690.
// See project/dot_reports/packed-state-root.md for evidence and verification.
#include <retail/FloatState.h>

#ifdef NON_MATCHING
namespace retail_float_state {

union FloatWord { float value; u32 bits; s32 signedBits; };

static inline u32 bits(float value)
{
    FloatWord word;
    word.value = value;
    return word.bits;
}

static inline s32 signedBits(float value)
{
    FloatWord word;
    word.value = value;
    return word.signedBits;
}

static inline float fromBits(u32 value)
{
    FloatWord word;
    word.bits = value;
    return word.value;
}

// The retail encoders truncate the mantissa and retain overflowing exponents.
// These are deliberately not IEEE half/float24 conversion routines.
static inline u32 pack16(float value)
{
    u32 word = bits(value);
    s32 exponent = (word & 0x7fffffff) ? s32((word >> 23) & 255) - 112 : 0;
    u32 sign = word >> 31;
    if (exponent < 0)
        return sign << 15;
    return ((word >> 13) & 1023) | (u32(exponent) << 10) | (sign << 15);
}

static inline u32 pack24(float value)
{
    u32 word = bits(value);
    s32 exponent = (word & 0x7fffffff) ? s32((word >> 23) & 255) - 64 : 0;
    u32 sign = word >> 31;
    if (exponent < 0)
        return sign << 23;
    return ((word >> 7) & 65535) | (u32(exponent) << 16) | (sign << 23);
}

static inline u32 pack20(float value)
{
    u32 word = bits(value);
    s32 exponent = (word & 0x7fffffff) ? s32((word >> 23) & 255) - 64 : 0;
    u32 sign = word >> 31;
    if (exponent < 0)
        return sign << 19;
    return ((word >> 11) & 4095) | (u32(exponent) << 12) | (sign << 19);
}

static inline u32 packPositive24(float value, float scale)
{
    if (value <= 0.0f || ((bits(value) >> 23) & 255) == 255)
        return 0;
    float scaled = value * scale;
    return signedBits(scaled) < 0x4b800000 ? u32(scaled) : 0x00ffffff;
}

static inline u32 packSigned412(float value)
{
    if (value == 0.0f || ((bits(value) >> 23) & 255) == 255)
        return 0;
    float scaled = (value + 8.0f) * 4096.0f;
    if (scaled < 0.0f)
        scaled = 0.0f;
    else if (signedBits(scaled) >= 0x47800000)
        scaled = 65535.0f;
    scaled = signedBits(scaled) < 0x47000000 ? scaled + 32768.0f : scaled - 32768.0f;
    return u32(scaled);
}

static inline u32 packDirection(float value)
{
    if (value == 0.0f || ((bits(value) >> 23) & 255) == 255)
        return 0;
    float scaled = (-value + 2.0f) * 2048.0f;
    if (scaled < 0.0f)
        scaled = 0.0f;
    else if (signedBits(scaled) >= 0x46000000)
        scaled = 8191.0f;
    scaled = signedBits(scaled) >= 0x45800000 ? scaled - 4096.0f : scaled + 4096.0f;
    return u32(scaled);
}

static inline float upperOne(float value)
{
    return signedBits(value) > 0x3f800000 ? 1.0f : value;
}

static inline u32 colorByte(float value)
{
    return u32(0.5f + 255.0f * value);
}

static inline u32 colorRgb(float r, float g, float b)
{
    return colorByte(r) | (colorByte(g) << 8) | (colorByte(b) << 16);
}

static inline u32 colorRgba(const float* color)
{
    return colorRgb(color[0], color[1], color[2]) | (colorByte(color[3]) << 24);
}

static inline u32 colorBgr10(float r, float g, float b)
{
    return colorByte(upperOne(b)) | (colorByte(upperOne(g)) << 10)
        | (colorByte(upperOne(r)) << 20);
}

// The retail forced path evaluates the field directly; the conditional path
// evaluates it for comparison and again on change. Keeping the expression at
// each use also preserves reads from the current source fields between writes.
#define writeWord(context, control, index, value, mask, enables) \
    do { \
        State* cache = (context)->state; \
        cache->byteEnables[index] |= (enables); \
        if ((control)->forceShadow) { \
            cache->packedRegisters[index] = (cache->packedRegisters[index] & ~(mask)) | ((value) & (mask)); \
            cache->packedDirty[(index) >> 5] |= 1u << ((index) & 31); \
            (control)->dirty |= 0x80000; \
            (context)->complementedShadow[index] = ~cache->packedRegisters[index]; \
        } else if ((cache->packedRegisters[index] & (mask)) != ((value) & (mask))) { \
            cache->packedRegisters[index] = (cache->packedRegisters[index] & ~(mask)) | ((value) & (mask)); \
            cache->packedDirty[(index) >> 5] |= 1u << ((index) & 31); \
            (control)->dirty |= 0x80000; \
        } \
    } while (0)

static inline void writePreparedWord(Context* context, Control* control, unsigned index,
                                      u32 value, u32 mask, u8 enables)
{
    writeWord(context, control, index, value, mask, enables);
}

static inline void copy4(float* destination, const float* source)
{
    // Retail's pair-unrolled loop keeps one source element ahead of stores.
    float a = source[0];
    float b = source[1];
    destination[0] = a;
    a = source[2];
    destination[1] = b;
    b = source[3];
    destination[2] = a;
    destination[3] = b;
}

static inline void copy3(float* destination, const float* source)
{
    destination[0] = source[0];
    destination[1] = source[1];
    destination[2] = source[2];
}

static inline bool replace4(float* destination, const float* source)
{
    bool changed = false;
    for (int component = 0; component < 4; ++component) {
        if (destination[component] != source[component])
            changed = true;
        destination[component] = source[component];
    }
    return changed;
}

template <int Kind>
static inline void writeLightColor(Context* context, Control* control, int light,
                                   const float* color)
{
    const int kind = Kind;
    State* state = context->state;
    const float* material = state->materialColor[kind];
    unsigned base = kind == 0 ? 95 : kind == 1 ? 94 : kind == 2 ? 92 : 93;
    if (kind == 3 && !state->multiplyFourthColor) {
        writeWord(context, control, base + light * 11,
                  colorBgr10(color[0], color[1], color[2]), 0xffffffff, 15);
    } else {
        float r = color[0] * material[0];
        float g = color[1] * material[1];
        float b = color[2] * material[2];
        writeWord(context, control, base + light * 11,
                  colorBgr10(r, g, b), 0xffffffff, 15);
    }
}

static inline void writeGlobalColor(Context* context, Control* control)
{
    State* state = context->state;
    const float* ambient = state->globalColor;
    const float* material = state->materialColor[0];
    const float* offset = state->colorOffset;
    float r = offset[0] + ambient[0] * material[0];
    float g = offset[1] + ambient[1] * material[1];
    float b = offset[2] + ambient[2] * material[2];
    writeWord(context, control, 180, colorBgr10(r, g, b), 0xffffffff, 15);
}

static inline void writeFloatRegisters(State* state, Control* control, u32 location,
                                       const float* values, int width, int count,
                                       int transpose, int matrix)
{
    LocationRecord record = state->locations[(location >> 7) & 2047];
    unsigned element = location & 127;
    unsigned stride = matrix ? 1 + ((record.packed >> 13) & 3) : 1;
    control->dirty |= 0x10000;
    u32 (*registers)[4] = record.packed & 0x400 ? state->floatRegisters1 : state->floatRegisters0;
    u32* dirty = record.packed & 0x400 ? state->floatDirty1 : state->floatDirty0;
    for (int i = 0; i < int(stride * count); ++i) {
        unsigned index = (record.packed >> 24) + stride * element + i;
        dirty[index >> 5] |= 1u << (index & 31);
    }
    unsigned component = (record.packed >> 11) & 3;
    unsigned base = record.packed >> 24;
    if (matrix) {
        for (int i = 0; i < count; ++i) {
            for (int row = 0; row < width; ++row) {
                for (int column = 0; column < width; ++column) {
                    u32 value = bits(values[(i * width + row) * width + column]);
                    if (transpose)
                        registers[base + stride * (element + i) + column][3 - component - row] = value;
                    else
                        registers[base + stride * (element + i) + row][3 - component - column] = value;
                }
            }
        }
    } else {
        for (int i = 0; i < count; ++i)
            for (int c = 0; c < width; ++c)
                registers[base + element + i][3 - component - c] = bits(values[i * width + c]);
    }
}

} // namespace retail_float_state

extern "C" void fn_0020ACAC(u32 location, const float* values, int width,
                           int count, int transpose, int matrix)
{
    using namespace retail_float_state;
    if (location == 0xffffffff || count == 0)
        return;
    Context* context = dat_003E2E40.current;
    State* state = context->state;
    Control* control = dat_003E3154;
    if (!(location & 0x40000)) {
        writeFloatRegisters(state, control, location, values, width, count, transpose, matrix);
        return;
    }

    unsigned id = (location >> 2) & 65535;
    switch (id) {
    case 1:
        state->scalar1 = values[0];
        writePreparedWord(context, control, 42, packPositive24(values[0], 16777216.0f), 0x00fffffe, 7);
        break;
    case 2:
        state->scalar2 = values[0];
        writePreparedWord(context, control, 42, ((bits(values[0]) >> 23) - 127) << 24, 0xff000000, 8);
        break;
    case 19: {
        state->scalar19 = values[0];
        u32 value = pack16(values[0]);
        writePreparedWord(context, control, 44, value << 20, 0x0ff00000, 12);
        writePreparedWord(context, control, 48, (value >> 8) << 19, 0x07f80000, 12);
        break;
    }
    case 21: {
        writePreparedWord(context, control, 45, (pack16(values[1]) << 16) | packSigned412(values[2]), 0xffffffff, 15);
        writePreparedWord(context, control, 47, pack16(values[0]), 0x0000ffff, 3);
        copy3(state->vector21, values);
        break;
    }
    case 22: {
        writePreparedWord(context, control, 46, (pack16(values[1]) << 16) | packSigned412(values[2]), 0xffffffff, 15);
        writePreparedWord(context, control, 47, pack16(values[0]) << 16, 0xffff0000, 12);
        copy3(state->vector22, values);
        break;
    }
    case 31:
        state->scalar31 = values[0];
        writePreparedWord(context, control, 91, pack16(state->scalar32 + values[0]) | (pack16(-values[0]) << 16), 0xffffffff, 15);
        break;
    case 32:
        state->scalar32 = values[0];
        writePreparedWord(context, control, 91, pack16(state->scalar31 + values[0]) | (pack16(-state->scalar31) << 16), 0xffffffff, 15);
        break;
    case 33: {
        writePreparedWord(context, control, 39, values[0] == 0.0f ? 1 : 0, 0xffffffff, 15);
        float scale;
        float offset;
        if (values[0] == 0.0f) {
            offset = control->range0;
            scale = control->range0 - control->range1;
        } else {
            scale = -values[0];
            offset = 0.0f;
        }
        if (control->biasEnabled && control->bias != 0.0f)
            offset += control->bias * fromBits(control->format ? 0x33800001 : 0x37800080);
        writePreparedWord(context, control, 23, pack24(scale), 0xffffffff, 15);
        writePreparedWord(context, control, 24, offset == 0.0f ? 0 : pack24(offset), 0xffffffff, 15);
        state->scalar33 = values[0];
        break;
    }
    case 35:
        for (int i = 0; i < 4; ++i)
            writePreparedWord(context, control, 34 + i, pack24(values[i]), 0xffffffff, 15);
        copy4(state->vector35, values);
        break;
    case 38:
        state->scalar38 = values[0];
        writeWord(context, control, 90, colorByte(values[0]) << 8, 0x0000ff00, 2);
        break;
    case 39:
        writeWord(context, control, 85, colorRgb(values[0], values[1], values[2]), 0x00ffffff, 7);
        copy3(state->vector39, values);
        break;
    case 40:
        writeWord(context, control, 86, colorRgb(values[0], values[1], values[2]), 0x00ffffff, 7);
        writeWord(context, control, 87, colorByte(upperOne(values[3])), 0x000000ff, 1);
        copy4(state->vector40, values);
        break;
    case 41:
        writePreparedWord(context, control, 88, packPositive24(values[0], 256.0f), 0x00ffffff, 7);
        state->scalar41 = values[0];
        break;
    case 42:
        writePreparedWord(context, control, 83, pack16(values[0] == 0.0f ? 0.0f : values[0]), 0xffffffff, 15);
        state->scalar42 = values[0];
        break;
    case 44:
        writePreparedWord(context, control, 82, pack16(values[0]), 0xffffffff, 15);
        state->scalar44 = values[0];
        break;
    case 51:
        copy4(state->globalColor, values);
        writeGlobalColor(context, control);
        break;
    case 52:
        copy4(state->colorOffset, values);
        writeGlobalColor(context, control);
        break;
    case 53:
        if (!replace4(state->materialColor[0], values))
            return;
        for (int light = 0; light < 8; ++light)
            writeLightColor<0>(context, control, light, state->lights[light].color[0]);
        writeGlobalColor(context, control);
        break;
    case 54:
        if (!replace4(state->materialColor[1], values))
            return;
        for (int light = 0; light < 8; ++light)
            writeLightColor<1>(context, control, light, state->lights[light].color[1]);
        break;
    case 55:
        if (!replace4(state->materialColor[2], values))
            return;
        for (int light = 0; light < 8; ++light)
            writeLightColor<2>(context, control, light, state->lights[light].color[2]);
        break;
    case 56:
        if (!replace4(state->materialColor[3], values))
            return;
        for (int light = 0; light < 8; ++light)
            writeLightColor<3>(context, control, light, state->lights[light].color[3]);
        break;
    case 65: case 66: case 67: case 68: case 69: case 70: case 71: case 72: {
        unsigned light = id - 65;
        copy4(state->lights[light].color[0], values);
        writeLightColor<0>(context, control, light, values);
        break;
    }
    case 73: case 74: case 75: case 76: case 77: case 78: case 79: case 80: {
        unsigned light = id - 73;
        copy4(state->lights[light].color[1], values);
        writeLightColor<1>(context, control, light, values);
        break;
    }
    case 81: case 82: case 83: case 84: case 85: case 86: case 87: case 88: {
        unsigned light = id - 81;
        copy4(state->lights[light].color[2], values);
        writeLightColor<2>(context, control, light, values);
        break;
    }
    case 89: case 90: case 91: case 92: case 93: case 94: case 95: case 96: {
        unsigned light = id - 89;
        copy4(state->lights[light].color[3], values);
        writeLightColor<3>(context, control, light, values);
        break;
    }
    case 97: case 98: case 99: case 100: case 101: case 102: case 103: case 104: {
        unsigned light = id - 97;
        copy4(state->lights[light].vector97, values);
        writePreparedWord(context, control, 96 + 11 * light, pack16(values[0]) | (pack16(values[1]) << 16), 0xffffffff, 15);
        writePreparedWord(context, control, 97 + 11 * light, pack16(values[2]), 0xffffffff, 15);
        writePreparedWord(context, control, 100 + 11 * light, values[3] == 0.0f ? 1 : 0, 1, 1);
        break;
    }
    case 105: case 106: case 107: case 108: case 109: case 110: case 111: case 112: {
        unsigned light = id - 105;
        copy3(state->lights[light].vector105, values);
        writePreparedWord(context, control, 98 + 11 * light, packDirection(values[0]) | (packDirection(values[1]) << 16), 0xffffffff, 15);
        writePreparedWord(context, control, 99 + 11 * light, packDirection(values[2]), 0xffffffff, 15);
        break;
    }
    case 161: case 162: case 163: case 164: case 165: case 166: case 167: case 168: {
        unsigned light = id - 161;
        state->lights[light].scalar161 = values[0];
        writePreparedWord(context, control, 101 + 11 * light, pack20(values[0]), 0x000fffff, 7);
        break;
    }
    case 169: case 170: case 171: case 172: case 173: case 174: case 175: case 176: {
        unsigned light = id - 169;
        state->lights[light].scalar169 = values[0];
        writePreparedWord(context, control, 102 + 11 * light, pack20(values[0]), 0x000fffff, 7);
        break;
    }
    case 207: case 208: case 209: case 210: case 211: case 212: case 213: {
        u32 code;
        switch (s32(values[0] * 4.0f)) {
        case 1: code = 6; break;
        case 2: code = 7; break;
        case 4: code = 0; break;
        case 8: code = 1; break;
        case 16: code = 2; break;
        case 32: code = 3; break;
        default: return;
        }
        unsigned index = id - 207;
        writePreparedWord(context, control, 187, code << (4 * index), 15u << (4 * index), u8(1u << (index / 2)));
        break;
    }
    case 270: case 271: case 272: case 273: case 274: case 275:
    case 276: case 277: case 278: case 279: case 280: case 281: {
        u32 code;
        switch (s32(values[0])) {
        case 1: code = 0; break;
        case 2: code = 1; break;
        case 4: code = 2; break;
        default: return;
        }
        if (id < 276)
            writePreparedWord(context, control, 54 + (id - 270) * 5, code, 3, 1);
        else
            writePreparedWord(context, control, 54 + (id - 276) * 5, code << 16, 0x30000, 4);
        break;
    }
    case 282: case 283: case 284: case 285: case 286: case 287: {
        unsigned index = id - 282;
        copy4(state->vector282[index], values);
        writeWord(context, control, 53 + index * 5, colorRgba(values), 0xffffffff, 15);
        break;
    }
    case 288:
        copy4(state->vector288, values);
        writeWord(context, control, 84, colorRgba(values), 0xffffffff, 15);
        break;
    case 294:
        copy3(state->vector294, values);
        writeWord(context, control, 81, colorRgb(values[0], values[1], values[2]), 0xffffffff, 15);
        break;
    default:
        return;
    }
}
#undef writeWord
#endif // NON_MATCHING
