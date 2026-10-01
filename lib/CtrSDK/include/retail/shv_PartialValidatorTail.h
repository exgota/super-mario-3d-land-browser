// Partial-only state selection recovered from EU 00379388..00379FC0.
#pragma once
#include <shv_ValidatorTail.h>

namespace ShvReconstruction {

// Unlike the full entry's 189-slot sweep, the partial entry visits only the
// selected category's sentinel-terminated register lists, in this order.
inline void uploadPartialRegisterList(Byte* shader, Byte* validator,
                                      const Word* list) {
    for (Word i = 0; list[i] != 0xbd; ++i) {
        Word index = list[i];
        Word bit = 1u << (index & 31);
        Word dirtyOffset = 0x7a8 + (index >> 5) * 4;
        if (!(w(shader, dirtyOffset) & bit))
            continue;
        Word byteMask = b(shader, 0x3f6 + index);
        if (byteMask && w(shader, 0x4b4 + index * 4) !=
                        w(validator, 0x100c + index * 4)) {
            emit(w(shader, 0x4b4 + index * 4),
                 registerNumbers()[index] | (byteMask << 16));
            // The shadow advances even if the command buffer is full.
            w(validator, 0x100c + index * 4) = w(shader, 0x4b4 + index * 4);
        }
        w(shader, dirtyOffset) &= ~bit;
    }
}

inline void uploadPartialRegisters(Word category, Byte* shader, Byte* validator) {
    if (category & 0x20) {
        if ((w(shader, 0x5f4) & 7) == 7) {
            if (b(shader, 0xdf4)) {
                w(validator, 0x1158) = w(shader, 0x600);
                w(shader, 0x7b0) &= ~0x80000u;
            } else {
                w(validator, 0x1158) = ~w(shader, 0x600);
                w(shader, 0x7b0) |= 0x80000;
            }
        }
        if (w(shader, 0x560) & 1) {
            // A retained read-only light scan occurs at 00379B80. Its result
            // is discarded by this entry; the full entry has a separate path.
            Word index = 0;
            const volatile Byte* observed = shader;
            while (index < 8 && !observed[0x9a0 + index * 112])
                ++index;
        }
    }
    if (category & 0x10) {
        uploadPartialRegisterList(shader, validator, dat_003E2EC8);
        uploadPartialRegisterList(shader, validator, dat_003E2EB0);
    }
    if (category & 2)
        uploadPartialRegisterList(shader, validator, dat_003E2E50);
    if (category & 0x20)
        uploadPartialRegisterList(shader, validator, dat_003E2EE0);
}

inline void validatePartialShaderTail(Byte* flags, Word category,
                                     Byte* context, Byte* shader, Byte* validator) {
    invalidateShaderState(context, shader, validator, category);
    if (category & 0x10)
        uploadUniforms(context, shader, true);
    if (category & 0x400)
        validateDepth(flags, context, shader, validator, true, true);
    uploadPartialRegisters(category, shader, validator);
    if (category & 0x40) {
        bool uploaded = false;
        Word rgba[514];
        validateLightingLuts(flags, context, shader, uploaded, dat_003A485C, dat_003A4874);
        validateTextureLuts(flags, context, shader, rgba, uploaded, dat_003A2F6C + 2);
        validateFogLut(flags, context, shader, uploaded);
        validateGasLut(flags, context, shader, uploaded);
    }
    if (category & 0x800)
        validateFramebufferAccess(flags, context, shader, true);
}

} // namespace ShvReconstruction
