#ifndef SHV_VALIDATOR_TAIL_H
#define SHV_VALIDATOR_TAIL_H
#include "shv_ValidatorAccess.h"

namespace ShvReconstruction {

// The six dirty words cover 189 cached PICA register values. Root 0x00108690
// consumes the lookup; it is not its producer. Shader manager 0x001064E4 writes
// the lookup and exclusion masks. Whole map ownership remains unaccepted.
// These are data references, never copied code.
inline Word* registerNumbers() { return reinterpret_cast<Word*>(0x00420F4C); }
inline Word* exclusionMask(Word address) { return reinterpret_cast<Word*>(address); }

inline void invalidateRegisters(Byte* shader, Byte* validator, const Word* list) {
    for (Word i = 0; list[i] != 0xbd; ++i) {
        const Word index = list[i];
        if (b(shader, 0x3f6 + index)) {
            w(validator, 0x100c + 4 * index) = ~w(shader, 0x4b4 + 4 * index);
            w(shader, 0x7a8 + 4 * (index >> 5)) |= 1u << (index & 31);
        }
    }
}

inline void invalidateShaderState(Byte* context, Byte* shader, Byte* validator, Word category = ~0u) {
    if (w(context, 4)) {
        if (w(context, 4) & category & 0x10) {
            w(shader, 0x1bc) = w(shader, 0x1b8) = w(shader, 0x1b4) = ~0u;
            invalidateRegisters(shader, validator, dat_003E2EB0);
            if (b(shader, 0x3f4)) {
                w(shader, 0x350) = w(shader, 0x34c) = w(shader, 0x348) = ~0u;
                invalidateRegisters(shader, validator, dat_003E2EC8);
            }
        }
        if (w(context, 4) & category & 0x20)
            invalidateRegisters(shader, validator, dat_003E2EE0);
        if (w(context, 4) & category & 2)
            invalidateRegisters(shader, validator, dat_003E2E50);
    }
}

inline Word firstUniformDirty(Byte* shader, Word maskOffset, Word index) {
    while (!(w(shader, maskOffset + 4 * (index >> 5)) & (1u << (index & 31)))) {
        if (w(shader, maskOffset + 4 * (index >> 5)) >> (index & 31)) ++index;
        else index = (index & ~31u) + 32;
    }
    return index;
}
inline Word nextUniformDirty(Byte* shader, Word maskOffset, Word index, Word end) {
    for (;;) {
        const Word mask = w(shader, maskOffset + 4 * (index >> 5));
        if ((mask & (1u << (index & 31))) || index >= end) return index;
        if (mask >> (index & 31)) ++index;
        else index = (index & ~31u) + 32;
    }
}

inline void uploadUniformBank(Byte* shader, Word dataOffset, Word mappingOffset,
                              Word countOffset, Word maskOffset, Word reg) {
    Byte* data = p(shader, dataOffset);
    if (!data || !(w(shader, maskOffset) || w(shader, maskOffset + 4) ||
                   w(shader, maskOffset + 8))) return;
    const Word* runData = 0;
    Word runStart = 0, runCount = 0;
    Word index = firstUniformDirty(shader, maskOffset, 0);
    if (index >= w(shader, countOffset)) return;
    do {
        const Word destination = w(shader, mappingOffset + 4 * index);
        if (runData && destination - runStart != runCount) {
            emit(runStart | 0x80000000, 0x000f0000 | reg);
            __cb_multiWriteReg(reg + 1, runCount * 4, runData);
            runData = 0;
        }
        if (!runData) {
            runStart = destination;
            runData = reinterpret_cast<const Word*>(data + 16 * index);
            runCount = 1;
        } else ++runCount;
        index = nextUniformDirty(shader, maskOffset, index + 1, w(shader, countOffset));
    } while (index < w(shader, countOffset));
    if (runData) {
        emit(runStart | 0x80000000, 0x000f0000 | reg);
        __cb_multiWriteReg(reg + 1, runCount * 4, runData);
    }
}

inline void uploadUniforms(Byte* context, Byte* shader, bool ignoreDisabled = false) {
    if (!ignoreDisabled && (w(context, 8) & 0x10)) return;
    if (!(w(shader, 0x1b4) || w(shader, 0x1b8) || w(shader, 0x1bc) ||
          w(shader, 0x348) || w(shader, 0x34c) || w(shader, 0x350))) return;
    uploadUniformBank(shader, 0x1c0, 0x1c4, 0x344, 0x348, 0x290);
    uploadUniformBank(shader, 0x2c, 0x30, 0x1b0, 0x1b4, 0x2c0);
    w(shader, 0x1b4) = w(shader, 0x1b8) = w(shader, 0x1bc) = 0;
    w(shader, 0x348) = w(shader, 0x34c) = w(shader, 0x350) = 0;
}

inline Word float24(float value) {
    const Word raw = bits(value);
    const Word magnitude = raw & 0x7fffffff;
    const int exponent = magnitude ? static_cast<int>((raw << 1) >> 24) - 64 : 0;
    const Word sign = raw >> 31;
    if (exponent < 0) return sign << 23;
    return ((raw << 9) >> 16) | (static_cast<Word>(exponent) << 16) | (sign << 23);
}
inline void setShaderRegister(Byte* context, Byte* shader, Byte* validator,
                              Word index, Word value, bool markContext = false) {
    b(shader, 0x3f6 + index) |= 15;
    if (markContext && (b(context, 0xc) || w(shader, 0x4b4 + index * 4) != value))
        w(context, 0) |= 0x80000;
    if (b(context, 0xc)) {
        w(shader, 0x4b4 + index * 4) = value;
        w(shader, 0x7a8 + (index >> 5) * 4) |= 1u << (index & 31);
        w(validator, 0x100c + index * 4) = ~value;
    } else if (w(shader, 0x4b4 + index * 4) != value) {
        w(shader, 0x4b4 + index * 4) = value;
        w(shader, 0x7a8 + (index >> 5) * 4) |= 1u << (index & 31);
    }
}
inline void validateDepth(Byte* flags, Byte* context, Byte* shader, Byte* validator,
                          bool ignoreDisabled = false, bool markContext = false) {
    if (!(w(flags, 0) & 4) || (!ignoreDisabled && (w(context, 8) & 0x400))) return;
    float offset, scale;
    if (f(shader, 0xdcc) == 0.0f) {
        offset = f(context, 0x4c);
        scale = offset - f(context, 0x50);
    } else { scale = -f(shader, 0xdcc); offset = 0.0f; }
    if (b(context, 0x54) && f(context, 0x44) != 0.0f)
        offset += f(context, 0x44) * (w(context, 0x5bc) == 0 ?
                  floatBits(0x37800080) : floatBits(0x33800001));
    setShaderRegister(context, shader, validator, 23, float24(scale), markContext);
    setShaderRegister(context, shader, validator, 24, offset == 0.0f ? 0 : float24(offset), markContext);
}

inline Word findRegisterDirty(Byte* shader, Word index) {
    while (index < 192) {
        Word mask = w(shader, 0x7a8 + (index >> 5) * 4);
        if (mask >> (index & 31)) {
            while (!(mask & (1u << (index & 31)))) ++index;
            return index;
        }
        index = (index & ~31u) + 32;
    }
    return index;
}

inline void uploadRegisterState(Byte* context, Byte* shader, Byte* validator) {
    if ((w(shader, 0x5f4) & 7) == 7) {
        if (b(shader, 0xdf4)) {
            w(validator, 0x1158) = w(shader, 0x600);
            w(shader, 0x7b0) &= ~0x80000u;
        } else {
            w(validator, 0x1158) = ~w(shader, 0x600);
            w(shader, 0x7b0) |= 0x80000;
        }
    }
    if (w(context, 8)) {
        if (w(context, 8) & 0x20)
            for (Word i=0; i<6; ++i) w(shader, 0x7a8+4*i) &= ~exclusionMask(0x00421270)[i];
        if (w(context, 8) & 2)
            for (Word i=0; i<6; ++i) w(shader, 0x7a8+4*i) &= ~exclusionMask(0x00421240)[i];
        if (w(context, 8) & 0x10)
            for (Word i=0; i<6; ++i) w(shader, 0x7a8+4*i) &= ~exclusionMask(0x00421258)[i];
    }
    bool restoreLighting = false;
    if ((w(shader, 0x560) & 1) && !(w(context, 8) & 0x20)) {
        Word i=0;
        while (i<8 && !b(shader, 0x9a0+112*i)) ++i;
        if (i==8) {
            w(shader, 0x7b0) &= ~0xf0000000u;
            w(validator, 0x117c) = w(validator, 0x1180) =
                w(validator, 0x1184) = w(validator, 0x1188) = 0;
            __cb_fillRegs(0x140, 4, 0);
            restoreLighting = true;
            if (w(shader, 0x78c) & 0xf0) {
                const Word value = w(shader, 0x78c) & ~0xf0u;
                w(shader, 0x7bc) &= ~0x400000u;
                w(validator, 0x12e4) = value;
                emit(value, 0x000f01c3);
            }
        }
    }
    Word index = findRegisterDirty(shader, 0);
    const Word* runData = 0;
    Word runStart = 0, runCount = 0;
    if (index < 0xbd) {
        do {
            const Word value = w(shader, 0x4b4 + 4 * index);
            const Word mask = b(shader, 0x3f6 + index);
            const Word destination = registerNumbers()[index];
            if (mask && value != w(validator, 0x100c + 4 * index)) {
                if (mask != 15) {
                    if (runData) { __cb_writeRegs(runStart, runCount, runData); runData = 0; }
                    emit(value, destination | (mask << 16));
                } else {
                    if (runData && destination - runStart != runCount) {
                        __cb_writeRegs(runStart, runCount, runData); runData = 0;
                    }
                    if (runData) ++runCount;
                    else { runData = reinterpret_cast<const Word*>(shader + 0x4b4 + 4*index);
                           runCount = 1; runStart = destination; }
                }
                w(validator, 0x100c + 4 * index) = value;
            } else if (runData) {
                __cb_writeRegs(runStart, runCount, runData); runData = 0;
            }
            index = findRegisterDirty(shader, index+1);
        } while (index < 0xbd);
        if (runData) __cb_writeRegs(runStart, runCount, runData);
        for (Word i=0; i<6; ++i) w(shader, 0x7a8 + 4*i) = 0;
    }
    if (restoreLighting) {
        w(shader, 0x7b0) |= 0xf0000000;
        if (w(shader, 0x78c) & 0xf0) w(shader, 0x7bc) |= 0x400000;
    }
}

// LUT payloads are cached beside 512 source floats. The conversions below are
// reconstructed from the validator's VFP operations, including exponent tests,
// truncation, saturation thresholds, and its distinct signed encodings.
inline bool finiteFloat(float value) { return ((bits(value) << 1) >> 24) != 255; }
inline Word truncUnsigned(float value) {
    // VCVT.U32.F32 saturates at the unsigned endpoints and maps NaNs to zero.
    const Word raw = bits(value);
    if ((raw & 0x80000000) || raw > 0x7f800000) return 0;
    if (raw >= 0x4f800000) return ~0u;
    return static_cast<Word>(value);
}
inline Word positiveFixed(float value, float scale, Word ceiling, float threshold) {
    if (!(value > 0.0f) || !finiteFloat(value)) return 0;
    value *= scale;
    return static_cast<int>(bits(value)) >= static_cast<int>(bits(threshold)) ?
           ceiling : truncUnsigned(value);
}
inline Word signedMagnitude12(float value) {
    if (value == 0.0f || !finiteFloat(value)) return 0;
    value *= 2048.0f;
    const Word sign = value < 0.0f ? 0x800 : 0;
    if (value < 0.0f) value = -value;
    if (static_cast<int>(bits(value)) >= 0x45000000) value = 2047.0f;
    return sign | truncUnsigned(value);
}
inline Word wrappedSigned(float value, float offset, float scale,
                          float threshold, float maximum) {
    if (value == 0.0f || !finiteFloat(value)) return 0;
    value = (value + offset) * scale;
    if (value < 0.0f) value = 0.0f;
    else if (static_cast<int>(bits(value)) >= static_cast<int>(bits(threshold))) value = maximum;
    if (static_cast<int>(bits(value)) >= static_cast<int>(bits(threshold * 0.5f)))
        return truncUnsigned(value - threshold * 0.5f);
    return truncUnsigned(value + threshold * 0.5f);
}
inline void allocateLut(Byte* lut, Word offset, Word bytes) {
    if (!p(lut, offset))
        p(lut, offset) = dat_003E2654 ?
            static_cast<Byte*>(dat_003E2654(0x10000, 0x100, 0, bytes)) : 0;
}
inline void rebuildLightingLut(Byte* lut) {
    if (!(w(lut, 0x81c) & 1)) return;
    allocateLut(lut, 0x804, 0x400);
    for (Word i=0; i<256; ++i) {
        const Word value = positiveFixed(f(lut, 4+4*i), 4096.0f, 4095, 4096.0f);
        w(p(lut, 0x804), 4*i) = value;
        const Word difference = signedMagnitude12(f(lut, 0x404+4*i));
        w(p(lut, 0x804), 4*i) = value | (difference << 12);
    }
    w(lut, 0x81c) &= ~1u;
}
inline void rebuildTextureLut(Byte* lut, Word cacheOffset, Word dirtyBit) {
    if (!(w(lut, 0x81c) & dirtyBit)) return;
    allocateLut(lut, cacheOffset, 0x200);
    for (Word i=0; i<128; ++i) {
        const Word value = positiveFixed(f(lut, 4+4*i), 4096.0f, 4095, 4096.0f);
        w(p(lut, cacheOffset), 4*i) = value;
        const Word difference = wrappedSigned(f(lut, 0x204+4*i), 1.0f, 2048.0f,
                                              4096.0f, 4095.0f);
        w(p(lut, cacheOffset), 4*i) = value | (difference << 12);
    }
    w(lut, 0x81c) &= ~dirtyBit;
}
inline void rebuildColorLut(Byte* lut) {
    if (!(w(lut, 0x81c) & 0x20)) return;
    allocateLut(lut, 0x818, 0x200);
    Word i=0;
    for (; i<256; ++i) {
        float value=f(lut, 4+4*i);
        if (static_cast<int>(bits(value)) > 0x3f800000) value = 1.0f;
        b(p(lut, 0x818), i) = static_cast<Byte>(truncUnsigned(0.5f + value * 255.0f));
    }
    for (; i<511; ++i)
        b(p(lut, 0x818), i) = static_cast<Byte>(wrappedSigned(f(lut, 4+4*i),
                                                1.0f, 128.0f, 256.0f, 255.0f));
    b(p(lut, 0x818), i) = 0;
    w(lut, 0x81c) &= ~0x20u;
}
inline void rebuildFogLut(Byte* lut) {
    if (!(w(lut, 0x81c) & 2)) return;
    allocateLut(lut, 0x808, 0x200);
    for (Word i=0; i<128; ++i) {
        const Word difference = wrappedSigned(f(lut, 0x204+4*i), 2.0f, 2048.0f,
                                              8192.0f, 8191.0f);
        w(p(lut, 0x808), 4*i) = difference;
        const Word value = positiveFixed(f(lut, 4+4*i), 2048.0f, 2047, 2048.0f);
        w(p(lut, 0x808), 4*i) |= value << 13;
    }
    w(lut, 0x81c) &= ~2u;
}
inline void rebuildGasLut(Byte* lut) {
    if (!(w(lut, 0x81c) & 4)) return;
    allocateLut(lut, 0x80c, 0x40);
    for (Word i=0; i<8; ++i) {
        const float value=f(lut, 4+4*i)*255.0f;
        w(p(lut, 0x80c), 4*i) = positiveFixed(value, 1.0f, 255, 256.0f);
        float difference=f(lut, 0x24+4*i)*127.0f;
        if (difference < 0.0f) difference = -difference;
        Word encoded = truncUnsigned(difference) & 127;
        w(p(lut, 0x80c), 0x20+4*i) = encoded;
        if (f(lut, 0x24+4*i) < 0.0f)
            w(p(lut, 0x80c), 0x20+4*i) = encoded | 128;
    }
    w(lut, 0x81c) &= ~4u;
}

inline void validateLightingTable(Byte* context, Byte* lut, Word identity,
                                  Word cachedIdentityOffset, Word countOffset,
                                  Word firstOffset, Word selector, bool& uploaded) {
    if (w(context, cachedIdentityOffset) == identity) {
        if (w(context, countOffset)) {
            const Word first=w(context, firstOffset);
            emit(selector | first, 0x000f01c5);
            __cb_multiWriteReg(0x1c8, w(context, countOffset),
                              reinterpret_cast<Word*>(p(lut, 0x804)) + first);
            w(context, countOffset)=0;
        }
    } else {
        if (w(context, countOffset)) w(context, countOffset)=0;
        w(context, cachedIdentityOffset)=identity;
        rebuildLightingLut(lut);
        emit(selector, 0x000f01c5);
        __cb_multiWriteReg(0x1c8, 256, reinterpret_cast<Word*>(p(lut, 0x804)));
        uploaded=true;
    }
}
inline void validateLightingLuts(Byte* flags, Byte* context, Byte* shader, bool& uploaded,
                                 const Word* disabledBits = dat_003A482C,
                                 const Word* selectors = dat_003A4844) {
    if (!(w(shader, 0x560)&1) || !(w(flags,0)&0x4008)) return;
    for (Word i=0; i<6; ++i) {
        if (!((w(shader,0x98c)>>i)&1) ||
            ((w(shader,0x790)>>disabledBits[i])&1)) continue;
        const Word slot=w(shader,0x974+4*i);
        if (slot==~0u || !w(context,0x74+4*slot)) break;
        const Word identity=w(context,0x74+4*slot);
        Byte* lut=__tx_getBoundTextureLut(slot);
        if (!lut) break;
        validateLightingTable(context,lut,identity,0x10c+4*i,0x190+4*i,
                              0x214+4*i,selectors[i]<<8,uploaded);
    }
    for (Word i=0; i<8; ++i) {
        if (!b(shader,0x9a0+112*i)) continue;
        if ((w(shader,0x98c)&0x40) && !((w(shader,0x790)>>(i+8))&1)) {
            const Word slot=w(shader,0xa00+112*i);
            const Word identity=w(context,0x74+4*slot);
            Byte* lut=__tx_getBoundTextureLut(slot);
            validateLightingTable(context,lut,identity,0x124+4*i,0x1a8+4*i,
                                  0x22c+4*i,(i+8)<<8,uploaded);
        }
        if (!((w(shader,0x790)>>(i+24))&1)) {
            const Word slot=w(shader,0xa0c+112*i);
            const Word identity=w(context,0x74+4*slot);
            Byte* lut=__tx_getBoundTextureLut(slot);
            validateLightingTable(context,lut,identity,0x144+4*i,0x1c8+4*i,
                                  0x24c+4*i,(i+16)<<8,uploaded);
        }
    }
}

inline void validateTextureTable(Byte* context, Byte* lut, Word identity,
                                 Word cachedIdentityOffset, Word countOffset,
                                 Word firstOffset, Word selector, Word cacheOffset,
                                 Word dirtyBit, bool& uploaded) {
    if (w(context,cachedIdentityOffset)==identity) {
        if (w(context,countOffset)) {
            const Word first=w(context,firstOffset);
            emit(selector|first,0x000f00af);
            __cb_multiWriteReg(0xb0,w(context,countOffset),
                              reinterpret_cast<Word*>(p(lut,cacheOffset))+first);
            w(context,countOffset)=0;
        }
    } else {
        if (w(context,countOffset)) w(context,countOffset)=0;
        w(context,cachedIdentityOffset)=identity;
        rebuildTextureLut(lut,cacheOffset,dirtyBit);
        emit(selector,0x000f00af);
        __cb_multiWriteReg(0xb0,128,reinterpret_cast<Word*>(p(lut,cacheOffset)));
        uploaded=true;
    }
}
inline void uploadColorRange(Word* rgba, Word first, Word last) {
    if (first<256) {
        emit(first|0x400,0x000f00af);
        if (last>=256) {
            __cb_multiWriteReg(0xb0,256-first,rgba+first);
            emit(0x500,0x000f00af);
            __cb_multiWriteReg(0xb0,last-255,rgba+256);
            return;
        }
    } else emit((first-256)|0x500,0x000f00af);
    __cb_multiWriteReg(0xb0,last-first+1,rgba+first);
}
inline void validateTextureLuts(Byte* flags, Byte* context, Byte* shader,
                                Word* rgba, bool& uploaded, const Word* textureSelectors = dat_003A2F6C) {
    if (!w(shader,0xd8c) || !(w(flags,0)&0x4020)) return;
    // The retail emitter copies one source word of padding per even packet.
    // Keep the two selector temporaries adjacent to the 512 color words.
    rgba[512] = textureSelectors[0];
    rgba[513] = textureSelectors[1];
    for (Word i=0; i<2; ++i) {
        if (i==1 && !(w(shader,0x564)&0x4000)) continue;
        const Word slot=w(shader,0xd70+4*i);
        const Word identity=w(context,0x74+4*slot);
        Byte* lut=__tx_getBoundTextureLut(slot);
        validateTextureTable(context,lut,identity,0x164+4*i,0x1e8+4*i,
                             0x26c+4*i,textureSelectors[i]<<8,0x810,8,uploaded);
    }
    if (w(shader,0x564)&0x8000) {
        const Word slot=w(shader,0xd78);
        const Word identity=w(context,0x74+4*slot);
        Byte* lut=__tx_getBoundTextureLut(slot);
        validateTextureTable(context,lut,identity,0x16c,0x1f0,0x274,0,0x814,0x10,uploaded);
    }
    Word channel=0;
    while (channel<4 && w(context,0x74+4*w(shader,0xd7c+4*channel))==
                         w(context,0x170+4*channel)) ++channel;
    if (channel==4) {
        if (!(w(context,0x1f4)||w(context,0x1f8)||w(context,0x1fc)||w(context,0x200))) return;
        Word first=512,last=0;
        for (Word i=0; i<4; ++i) {
            Word count=w(context,0x1f4+4*i);
            if (count) {
                Word start=w(context,0x278+4*i);
                w(context,0x1f4+4*i)=0;
                if (start<first) first=start;
                if (start+count-1>last) last=start+count-1;
            }
        }
        for (Word i=0; i<4; ++i) {
            Byte* lut=__tx_getBoundTextureLut(w(shader,0xd7c+4*i));
            for (Word j=first; j<=last; ++j)
                rgba[j]=(rgba[j]&~(255u<<(i*8))) | (Word(b(p(lut,0x818),j))<<(i*8));
        }
        uploadColorRange(rgba,first,last);
    } else {
        for (Word i=0; i<4; ++i) {
            Byte* lut=__tx_getBoundTextureLut(w(shader,0xd7c+4*i));
            if (w(context,0x1f4+4*i)) w(context,0x1f4+4*i)=0;
            rebuildColorLut(lut);
            for (Word j=0; j<512; ++j)
                rgba[j]=(rgba[j]&~(255u<<(i*8))) | (Word(b(p(lut,0x818),j))<<(i*8));
        }
        emit(0x400,0x000f00af);
        __cb_multiWriteReg(0xb0,256,rgba);
        emit(0x500,0x000f00af);
        __cb_multiWriteReg(0xb0,256,rgba+256);
        uploaded=true;
        for (Word i=0; i<4; ++i)
            w(context,0x170+4*i)=w(context,0x74+4*w(shader,0xd7c+4*i));
    }
}

inline void validateFogLut(Byte* flags, Byte* context, Byte* shader, bool& uploaded) {
    if (!(w(shader,0x5f4)&7) || !(w(flags,0)&0x4010)) return;
    const Word slot=w(shader,0xde4);
    const Word identity=w(context,0x74+4*slot);
    Byte* lut=__tx_getBoundTextureLut(slot);
    if (w(context,0x180)==identity) {
        if (w(context,0x204)) {
            Word first=w(context,0x288);
            emit(first,0x000f00e6);
            __cb_multiWriteReg(0xe8,w(context,0x204),
                              reinterpret_cast<Word*>(p(lut,0x808))+first);
            w(context,0x204)=0;
        }
    } else {
        if (w(context,0x204)) w(context,0x204)=0;
        w(context,0x180)=identity;
        rebuildFogLut(lut);
        emit(0,0x000f00e6);
        __cb_multiWriteReg(0xe8,128,reinterpret_cast<Word*>(p(lut,0x808)));
        uploaded=true;
    }
}
inline void validateGasLut(Byte* flags, Byte* context, Byte* shader, bool uploaded) {
    if ((w(shader,0x5f4)&7)!=7 || !(w(flags,0)&0x02004000)) return;
    Word channel=0;
    while (channel<3 && w(context,0x74+4*w(shader,0xdf8+4*channel))==
                         w(context,0x184+4*channel)) ++channel;
    if (channel==3) return;
    Word rgb[17];
    for (Word i=0; i<16; ++i) reinterpret_cast<volatile Word*>(rgb)[i]=0;
    // Original stack slot following the 16 words is the earlier-upload flag.
    rgb[16] = uploaded ? 1 : 0;
    for (Word i=0; i<3; ++i) {
        Byte* lut=__tx_getBoundTextureLut(w(shader,0xdf8+4*i));
        rebuildGasLut(lut);
        for (Word j=0; j<8; ++j) {
            rgb[j] |= w(p(lut,0x80c),0x20+4*j)<<(i*8);
            rgb[8+j] |= w(p(lut,0x80c),4*j)<<(i*8);
        }
    }
    if (!uploaded) __cb_addDummyWrite(0xc0,0x2d);
    emit(0,0x000f0123);
    __cb_multiWriteReg(0x124,16,rgb);
    emit(0,0x100);
    for (Word i=0; i<3; ++i)
        w(context,0x184+4*i)=w(context,0x74+4*w(shader,0xdf8+4*i));
}

inline void validateFramebufferAccess(Byte* flags, Byte* context, Byte* shader,
                                      bool ignoreDisabled = false) {
    if ((!ignoreDisabled && (w(context,8)&0x800)) || (!(w(flags,0)&0x100) && w(shader,0xdb8)==w(context,0x574))) return;
    Word colorMask=Word(b(context,0x584)) | (Word(b(context,0x585))<<1) |
                   (Word(b(context,0x586))<<2) | (Word(b(context,0x587))<<3);
    w(context,0x574)=w(shader,0xdb8);
    emit(1,0x000f0111);
    emit(1,0x000f0110);
    switch (w(context,0x574)) {
    case 0x6051:
        emit(15,0x000f0113); emit(15,0x000f0112);
        emit(3,0x000f0114); emit(0,0x000f0115);
        break;
    case 0x6048:
        emit(15,0x000f0113); emit(15,0x000f0112);
        emit(0,0x000f0114); emit(0,0x000f0115);
        break;
    case 0x6030: {
        Word mask=0;
        if (colorMask) {
            if (w(context,0x5b8)==1 || b(context,0x57c) || colorMask!=15 || b(context,0x57d)) mask=1;
            mask |= 2;
        }
        if (b(context,0x57b)) {
            mask |= 4;
            if (b(context,0x588)) mask |= 8;
        }
        if (b(context,0x57a)) {
            mask |= 16;
            if (w(context,0x58c)) mask |= 32;
        }
        emit((mask&1)?15:0,0x000f0112);
        emit((mask&2)?15:0,0x000f0113);
        const Word depthRead=((mask&4)&&(mask&10))?2:0;
        const Word stencilRead=((mask&16)&&(mask&34))?1:0;
        emit(depthRead|stencilRead,0x000f0114);
        emit(((mask&8)?2:0)|((mask&32)>>5),0x000f0115);
        break;
    }
    }
}

inline void validateShaderTail(Byte* flags, Byte* context, Byte* shader, Byte* validator) {
    invalidateShaderState(context,shader,validator);
    uploadUniforms(context,shader);
    validateDepth(flags,context,shader,validator);
    uploadRegisterState(context,shader,validator);
    if (!(w(context,8)&0x40)) {
        bool uploaded=false;
        Word rgba[514];
        validateLightingLuts(flags,context,shader,uploaded);
        validateTextureLuts(flags,context,shader,rgba,uploaded);
        validateFogLut(flags,context,shader,uploaded);
        validateGasLut(flags,context,shader,uploaded);
    }
    validateFramebufferAccess(flags,context,shader);
}

} // namespace ShvReconstruction
#endif
