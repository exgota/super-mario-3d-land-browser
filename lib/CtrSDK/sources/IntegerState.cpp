// NonMatching reconstruction of retail EU 0x00206474..0x0020ACAC.
// Descriptive names only. See project/dot_reports/integer-state-root.md.
#include <retail/IntegerState.h>

#ifdef NON_MATCHING
namespace retail_integer_state {
using namespace retail_float_state;

static inline u32 bit(unsigned shift)
{
    // ARM register shifts use the low byte, and produce zero at 32 and above.
    shift &= 255;
    return shift < 32 ? 1u << shift : 0;
}

static inline u8 byteMask(u32 mask)
{
    return (mask & 0xff ? 1 : 0) | (mask & 0xff00 ? 2 : 0)
         | (mask & 0xff0000 ? 4 : 0) | (mask & 0xff000000 ? 8 : 0);
}

#define WRITE(index, value, mask, enables, extra) \
    do { \
        state->byteEnables[index] |= (enables); \
        if (control->forceShadow) { \
            state->packedRegisters[index] = (state->packedRegisters[index] & ~(mask)) | ((value) & (mask)); \
            state->packedDirty[(index) >> 5] |= 1u << ((index) & 31); \
            control->dirty |= 0x80000 | (extra); \
            context->complementedShadow[index] = ~state->packedRegisters[index]; \
        } else if ((state->packedRegisters[index] & (mask)) != ((value) & (mask))) { \
            state->packedRegisters[index] = (state->packedRegisters[index] & ~(mask)) | ((value) & (mask)); \
            state->packedDirty[(index) >> 5] |= 1u << ((index) & 31); \
            control->dirty |= 0x80000 | (extra); \
        } \
    } while (0)

static inline void writePrepared(Context* context, Control* control, unsigned index,
                                 u32 value, u32 mask, u8 enables, u32 extra = 0)
{
    State* state = context->state;
    WRITE(index, value, mask, enables, extra);
}

static inline void replaceEnum(u32& destination, u32& cache, u32 value,
                               Control* control, u32 dirty)
{
    if (control->forceShadow) {
        destination = value;
        control->dirty |= dirty;
        cache = 0;
    } else if (destination != value) {
        destination = value;
        control->dirty |= dirty;
    }
}

static inline void setControlByte(u8& destination, u8 value, Control* control, u32 dirty)
{
    if (destination != value) {
        destination = value;
        control->dirty |= dirty;
    }
}

static inline int decodeColorMode(u32 value)
{
    switch (value) {
    case 0x609a: return 0;
    case 0x609c: return 1;
    case 0x609b: return 2;
    case 0x609d: return 3;
    case 0x609e: return 4;
    case 0x609f: return 5;
    case 0x60a1: return 6;
    case 0x60a2: return 7;
    case 0x60a3: return 8;
    case 0x60a4: return 9;
    default: return -1;
    }
}

static inline int decodeWrap(u32 value)
{
    switch (value) {
    case 0x60c0: return 0;
    case 0x812f: return 1;
    case 0x60c1: return 2;
    case 0x8370: return 3;
    case 0x60c2: return 4;
    default: return -1;
    }
}

static inline int decodeFilter(u32 value)
{
    switch (value) {
    case 0x2600: return 0;
    case 0x2601: return 1;
    case 0x2700: return 2;
    case 0x2701: return 3;
    case 0x2702: return 4;
    case 0x2703: return 5;
    default: return -1;
    }
}

static inline int decodeCombine(u32 value, bool alpha)
{
    switch (value) {
    case 0x1e01: return 0;
    case 0x2100: return 1;
    case 0x0104: return 2;
    case 0x8574: return 3;
    case 0x8575: return 4;
    case 0x84e7: return 5;
    case 0x86ae: return alpha ? -1 : 6;
    case 0x86af: return 7;
    case 0x6401: return 8;
    case 0x6402: return 9;
    default: return -1;
    }
}

static inline int decodeSource(u32 value)
{
    switch (value) {
    case 0x8577: return 0;
    case 0x6210: return 1;
    case 0x6211: return 2;
    case 0x84c0: return 3;
    case 0x84c1: return 4;
    case 0x84c2: return 5;
    case 0x84c3: return 6;
    case 0x8579: return 13;
    case 0x8576: return 14;
    case 0x8578: return 15;
    default: return -1;
    }
}

static inline int decodeOperand(u32 value, bool alpha)
{
    if (alpha) {
        switch (value) {
        case 0x0302: return 0;
        case 0x0303: return 1;
        case 0x8580: return 2;
        case 0x8583: return 3;
        case 0x8581: return 4;
        case 0x8584: return 5;
        case 0x8582: return 6;
        case 0x8585: return 7;
        default: return -1;
        }
    }
    switch (value) {
    case 0x0300: return 0;
    case 0x0301: return 1;
    case 0x0302: return 2;
    case 0x0303: return 3;
    case 0x8580: return 4;
    case 0x8583: return 5;
    case 0x8581: return 8;
    case 0x8584: return 9;
    case 0x8582: return 12;
    case 0x8585: return 13;
    default: return -1;
    }
}

union FloatWord { float value; s32 bits; };
static inline float upperOne(float value)
{
    FloatWord word;
    word.value = value;
    return word.bits > 0x3f800000 ? 1.0f : value;
}
static inline u32 colorByte(float value)
{
    return u32(0.5f + 255.0f * upperOne(value));
}
static inline u32 colorBgr(float r, float g, float b)
{
    return colorByte(b) | (colorByte(g) << 10) | (colorByte(r) << 20);
}
static inline void refreshFourthColor(Context* context, Control* control, int light, bool multiply)
{
    State* state = context->state;
    const float* color = state->lights[light].color[3];
    unsigned index = 93 + light * 11;
    if (multiply) {
        float r = color[0] * state->materialColor[3][0];
        float g = color[1] * state->materialColor[3][1];
        float b = color[2] * state->materialColor[3][2];
        WRITE(index, colorBgr(r, g, b), 0xffffffff, 15, 0);
    } else {
        WRITE(index, colorBgr(color[0], color[1], color[2]), 0xffffffff, 15, 0);
    }
}

} // namespace retail_integer_state

extern "C" void fn_00206474(u32 location, const s32* values, int, int count)
{
    using namespace retail_float_state;
    using namespace retail_integer_state;
    if (location == 0xffffffff || count == 0)
        return;
    Context* context = dat_003E2E40.current;
    State* state = context->state;
    dat_0042013C.unknown0 = location;
    Control* control = dat_003E3154;
    if (!(location & 0x40000)) {
        unsigned element = location & 127;
        dat_0042013C = state->locations[(location >> 7) & 2047];
        u32 packed = dat_0042013C.packed;
        unsigned kind = (packed >> 13) & 3;
        if (kind == 0) {
            unsigned index = packed & 0x400 ? 1 : 9;
            for (int i = 0; i < count; ++i) {
                unsigned component = element + (dat_0042013C.packed >> 24) + i;
                u32 mask = bit(component);
                if (values[i]) state->packedRegisters[index] |= mask;
                else state->packedRegisters[index] &= ~mask;
            }
            if (control->forceShadow)
                context->complementedShadow[index] = ~state->packedRegisters[index];
            state->packedDirty[index >> 5] |= 1u << (index & 31);
        } else if (kind == 2) {
            for (int i = 0; i < count; ++i) {
                u32 meta = dat_0042013C.packed;
                unsigned index = (meta & 0x400 ? 2 : 10) + (meta >> 24) + element + i;
                state->packedRegisters[index] = (u32(values[i*3]) & 255)
                    | ((u32(values[i*3+1]) & 255) << 8)
                    | ((u32(values[i*3+2]) & 255) << 16);
                state->packedDirty[index >> 5] |= 1u << (index & 31);
                if (control->forceShadow)
                    context->complementedShadow[index] = ~state->packedRegisters[index];
            }
        } else {
            return;
        }
        control->dirty |= 0x10000;
        return;
    }

    unsigned id = (location >> 2) & 65535;
    switch (id) {
    case 0: WRITE(42, u32(values[0] == 0), 1, 1, 0); return;
    case 3: {
        switch (values[0]) {
        case 0:
            setControlByte(control->textureEnabled[0], 0, control, 0x400);
            setControlByte(control->cubeEnabled, 0, control, 0x400);
            break;
        case 0x0de1: case 0x6e00: case 0x6e01:
            setControlByte(control->textureEnabled[0], 1, control, 0x400);
            setControlByte(control->cubeEnabled, 0, control, 0x400);
            break;
        case 0x6e02: case 0x8513:
            setControlByte(control->textureEnabled[0], 0, control, 0x400);
            setControlByte(control->cubeEnabled, 1, control, 0x400);
            break;
        default: return;
        }
        state->enum3[0] = values[0];
        if (control->textureEnums[0] != u32(values[0])) {
            control->textureEnums[0] = values[0];
            control->dirty |= 0x400;
        }
        return;
    }
    case 4: case 5: {
        if (values[0] != 0 && values[0] != 0x0de1) return;
        unsigned i = id - 3;
        setControlByte(control->textureEnabled[i], values[0] != 0, control, 0x400 << i);
        state->enum3[i] = values[0];
        if (control->textureEnums[i] != u32(values[0])) {
            control->textureEnums[i] = values[0];
            control->dirty |= 0x400 << i;
        }
        return;
    }
    case 6:
        if (values[0] != 0 && values[0] != 0x6e03) return;
        WRITE(41, values[0] ? 0x400u : 0, 0x400, 2, 0x20);
        state->enum6 = values[0]; return;
    case 7:
        if (values[0] != 0x84c1 && values[0] != 0x84c2) return;
        WRITE(41, values[0] == 0x84c1 ? 0x2000u : 0, 0x2000, 2, 0); return;
    case 8:
        if (u32(values[0]) - 0x84c0 > 2) return;
        WRITE(41, (u32(values[0]) - 0x84c0) << 8, 0x300, 2, 0); return;
    case 9: case 10: {
        int value = decodeColorMode(values[0]);
        if (value < 0) return;
        unsigned shift = id == 9 ? 6 : 10;
        writePrepared(context, control, 44, u32(value) << shift, 15u << shift, id == 9 ? 3 : 2);
        return;
    }
    case 11: WRITE(44, values[0] ? 0x4000u : 0, 0x4000, 2, 0x20); return;
    case 12: case 13: {
        int value = decodeWrap(values[0]);
        if (value < 0) return;
        unsigned shift = id == 12 ? 0 : 3;
        writePrepared(context, control, 44, u32(value) << shift, 7u << shift, 1);
        return;
    }
    case 14: case 15: {
        u32 value = u32(values[0]) - 0x60d0;
        if (value > 2) return;
        unsigned shift = id == 14 ? 16 : 18;
        writePrepared(context, control, 44, value << shift, 3u << shift, 4);
        return;
    }
    case 16: {
        int value = decodeFilter(values[0]);
        if (value < 0) return;
        WRITE(48, u32(value), 7, 1, 0); return;
    }
    case 17: WRITE(48, (u32(values[0]) & 255) << 11, 0x7f800, 6, 0); return;
    case 18: WRITE(49, u32(values[0]), 255, 1, 0); return;
    case 20: WRITE(44, values[0] ? 0x8000u : 0, 0x8000, 2, 0x20); return;
    case 23: case 24: case 25: case 26: case 27: case 28: case 29:
        replaceEnum(state->enum23[id-23], control->enum23Cache[id-23], values[0], control, 0x20); return;
    case 30: {
        u32 value;
        switch (values[0]) {
        case 0x6030: value = 0xe40000; break;
        case 0x6048: value = 0xe40003; break;
        case 0x6051: value = 0xe40001; break;
        default: return;
        }
        state->enum30 = values[0];
        WRITE(89, value, 0xffff00ff, 13, 0);
        control->dirty |= 0x100;
        return;
    }
    case 34: WRITE(33, u32(values[0] != 0), 1, 1, 0); return;
    case 36: WRITE(90, u32(values[0] != 0), 1, 1, 0); return;
    case 37: {
        int value;
        switch (values[0]) {
        case 0x200: value = 0; break;
        case 0x207: value = 1; break;
        case 0x202: value = 2; break;
        case 0x205: value = 3; break;
        case 0x201: value = 4; break;
        case 0x203: value = 5; break;
        case 0x204: value = 6; break;
        case 0x206: value = 7; break;
        default: return;
        }
        WRITE(90, u32(value) << 4, 0x70, 1, 0); return;
    }
    case 43: state->flag43 = values[0] != 0; return;
    case 45:
        if (values[0] != 0x6060 && values[0] != 0x6061) return;
        WRITE(87, values[0] == 0x6061 ? 0x100u : 0, 0x100, 2, 0); return;
    case 46:
        if (values[0] != 0x605e && values[0] != 0x605f) return;
        WRITE(80, values[0] == 0x605f ? 8u : 0, 8, 1, 0); return;
    case 47: case 48: case 49:
        replaceEnum(state->enum47[id-47], control->enum47Cache[id-47], values[0], control, 0x2000000); return;
    case 50:
        if (state->packedRegisters[43] & 1) {
            if (!values[0]) { WRITE(182, 0, 0xf0, 1, 0); }
        } else if (values[0]) {
            // The retail path leaves its encoding uninitialized if enum224
            // is absent from this table. Valid states use one of eight words.
            unsigned encoding;
            for (int i = 0; i < 8; ++i) {
                if (dat_003A480C[i] == state->enum224) {
                    encoding = i == 7 ? 8 : i;
                    break;
                }
            }
            WRITE(182, encoding << 4, 0xf0, 1, 0);
        }
        WRITE(184, u32(values[0] == 0), 0xffffffff, 15, 0);
        WRITE(43, u32(values[0] != 0), 1, 1, 8);
        return;
    case 57: case 58: case 59: case 60: case 61: case 62: case 63: case 64: {
        unsigned light = id - 57;
        if (state->lights[light].enabled == u32(values[0])) return;
        state->lights[light].enabled = values[0] != 0;
        unsigned enabled = 0;
        u32 order = 0;
        for (unsigned i = 0; i < 8; ++i) {
            if (state->lights[i].enabled) {
                order |= i << (4 * enabled);
                ++enabled;
            }
        }
        WRITE(181, enabled ? enabled - 1 : 0, 0xffffffff, 15, 0);
        WRITE(188, order, 0xffffffff, 15, 0);
        if (values[0] && (!(state->packedRegisters[183] & (1u << (light + 8)))
                       || !(state->packedRegisters[183] & (1u << (light + 24)))))
            control->dirty |= 8;
        return;
    }
    case 113: case 114: case 115: case 116: case 117: case 118: case 119: case 120: {
        u32 mask = 1u << (id - 113);
        WRITE(183, values[0] ? 0 : mask, mask, byteMask(mask), 0); return;
    }
    case 121: case 122: case 123: case 124: case 125: case 126: case 127: case 128:
        writePrepared(context, control, 100 + (id - 121) * 11, values[0] ? 4 : 0, 4, 1); return;
    case 129: case 130: case 131: case 132: case 133: case 134: case 135: case 136:
        writePrepared(context, control, 100 + (id - 129) * 11, values[0] ? 8 : 0, 8, 1); return;
    case 137: case 138: case 139: case 140: case 141: case 142: case 143: case 144:
        writePrepared(context, control, 100 + (id - 137) * 11, values[0] ? 2 : 0, 2, 1); return;
    case 145: case 146: case 147: case 148: case 149: case 150: case 151: case 152:
        replaceEnum(state->lights[id-145].enum145, control->light145Cache[id-145], values[0], control, 8); return;
    case 153: case 154: case 155: case 156: case 157: case 158: case 159: case 160: {
        u32 mask = 1u << (id - 153 + 8);
        WRITE(183, values[0] ? 0 : mask, mask, byteMask(mask), 8); return;
    }
    case 177: case 178: case 179: case 180: case 181: case 182: case 183: case 184: {
        u32 mask = 1u << (id - 177 + 24);
        WRITE(183, values[0] ? 0 : mask, mask, byteMask(mask), 8); return;
    }
    case 185: case 186: case 187: case 188: case 189: case 190: case 191: case 192:
        replaceEnum(state->lights[id-185].enum185, control->light185Cache[id-185], values[0], control, 8); return;
    case 193: case 194: case 195: case 196: case 197: case 198: case 199: {
        u32 mask = 1u << ((id - 193) * 4 + 1);
        WRITE(185, values[0] ? 0 : mask, mask, byteMask(mask), 0); return;
    }
    case 200: case 201: case 202: case 203: case 204: case 205: case 206: {
        unsigned shift = (id - 200) * 4;
        u32 mask = 7u << shift;
        WRITE(186, (u32(values[0]) & 7) << shift, mask, byteMask(mask), 0); return;
    }
    case 214: case 215: case 216: case 217: case 218: case 219:
        replaceEnum(state->enum214[id-214], control->enum214Cache[id-214], values[0], control, 8); return;
    case 220: WRITE(182, (u32(values[0]) + 0x40) << 24, 0x03000000, 8, 0); return;
    case 221: WRITE(182, (u32(values[0]) << 22) + 0xd0000000, 0x00c00000, 4, 0); return;
    case 222: {
        u32 value = (u32(values[0]) << 28) + 0x80000000;
        if (values[0] != 0x62c8 && !state->flag223) value |= 0x40000000;
        WRITE(182, value, 0x70000000, 8, 0); return;
    }
    case 223: {
        unsigned mode = (state->packedRegisters[182] >> 28) & 3;
        WRITE(182, !values[0] && mode ? 0x40000000u : 0, 0x40000000, 8, 0);
        state->flag223 = values[0] != 0; return;
    }
    case 224: {
        unsigned index = u32(values[0]) - 0x62b0;
        if (index >= 8) return;
        unsigned encoding = index == 7 ? 8 : index;
        state->enum224 = dat_003A480C[index];
        if (state->packedRegisters[43] & 1) { WRITE(182, encoding << 4, 0xf0, 1, 0); }
        return;
    }
    case 225: WRITE(182, values[0] ? 0x40000u : 0, 0x40000, 4, 0); return;
    case 226: case 227: case 228: {
        state->aggregateEnables[id-226] = values[0] != 0;
        u32 mask = id == 226 ? 0x10000 : id == 227 ? 0x20000 : 0x80000;
        WRITE(182, values[0] ? mask : 0, mask, 4, 0);
        WRITE(182, u32(state->aggregateEnables[0] || state->aggregateEnables[1] || state->aggregateEnables[2]), 1, 1, 0);
        return;
    }
    case 229:
        WRITE(182, u32(values[0]) << 2, 12, 1, 0);
        WRITE(183, values[0] == 0x62c0 ? 0x80000u : 0, 0x80000, 4, 8); return;
    case 230: WRITE(182, values[0] ? 0x8000000u : 0, 0x8000000, 8, 0); return;
    case 231: WRITE(183, values[0] ? 0 : 0x10000u, 0x10000, 4, 8); return;
    case 232: WRITE(183, values[0] ? 0 : 0x20000u, 0x20000, 4, 8); return;
    case 233:
        if (state->multiplyFourthColor == u32(values[0] == 0)) return;
        WRITE(183, values[0] ? 0 : 0x700000u, 0x700000, 4, 8);
        state->multiplyFourthColor = values[0] == 0;
        for (int light = 0; light < 8; ++light)
            refreshFourthColor(context, control, light, values[0] == 0);
        return;
    case 234: case 235: case 236: case 237: case 238: case 239:
    case 240: case 241: case 242: case 243: case 244: case 245: {
        bool alpha = id >= 240;
        int value = decodeCombine(values[0], alpha);
        if (value < 0) return;
        unsigned stage = id - (alpha ? 240 : 234);
        unsigned shift = alpha ? 16 : 0;
        writePrepared(context, control, 52 + stage * 5, u32(value) << shift, 15u << shift, alpha ? 4 : 1);
        return;
    }
    case 246: case 247: case 248: case 249: case 250: case 251:
    case 252: case 253: case 254: case 255: case 256: case 257: {
        bool alpha = id >= 252;
        unsigned shift = alpha ? 16 : 0;
        u32 packed = 0;
        for (unsigned component = 0; component < 3; ++component) {
            int value = decodeSource(values[component]);
            if (value < 0) return;
            packed |= u32(value) << (shift + component * 4);
        }
        unsigned stage = id - (alpha ? 252 : 246);
        writePrepared(context, control, 50 + stage * 5, packed, 0xfffu << shift, alpha ? 12 : 3);
        return;
    }
    case 258: case 259: case 260: case 261: case 262: case 263:
    case 264: case 265: case 266: case 267: case 268: case 269: {
        bool alpha = id >= 264;
        unsigned shift = alpha ? 12 : 0;
        u32 packed = 0;
        for (unsigned component = 0; component < 3; ++component) {
            int value = decodeOperand(values[component], alpha);
            if (value < 0) return;
            packed |= u32(value) << (shift + component * 4);
        }
        unsigned stage = id - (alpha ? 264 : 258);
        writePrepared(context, control, 51 + stage * 5, packed, alpha ? 0x777000 : 0xfff, alpha ? 6 : 3);
        return;
    }
    case 289: case 290: case 291: case 292: {
        unsigned stage = id - 289;
        u32 packed = 0;
        if (values[0] == 0x8578) packed |= 1u << (stage + 8);
        else if (values[0] != 0x8579) return;
        if (values[1] == 0x8578) packed |= 1u << (stage + 12);
        else if (values[1] != 0x8579) return;
        u32 mask = 0x1100u << stage;
        WRITE(80, packed, mask, byteMask(mask), 0); return;
    }
    case 293:
        switch (values[0]) {
        case 0: WRITE(80, 0, 7, 1, 0); return;
        case 0xb60: WRITE(80, 5, 7, 1, 0x10); return;
        case 0x6050: WRITE(80, 7, 7, 1, 0x2000010); return;
        default: return;
        }
    case 295: WRITE(80, values[0] ? 0x10000u : 0, 0x10000, 4, 0); return;
    case 296: replaceEnum(state->enum296, control->enum296Cache, values[0], control, 0x10); return;
    default: return;
    }
}
#undef WRITE
#endif
