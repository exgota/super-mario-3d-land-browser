// Clean-room semantic reconstruction from the owner's EU executable.
// Partial-validator ranges 00377FD0..00379388. Not byte-exact.
// All offsets are observed accesses, not imported SDK declarations.
#pragma once
#include "shv_ValidatorAccess.h"

namespace ShvReconstruction {

struct AttributeLink {
    int index;
    AttributeLink* next;
};

inline void validateScissor(Byte* flags, Word category, Byte* context) {
    if (!(category & 0x1000) || !(w(flags, 0) & 0x200))
        return;
    int left, bottom, right, top;
    Word mode;
    if (!b(context, 0x578)) {
        left = bottom = 0;
        right = static_cast<int>(w(context, 0x5c8)) - 1;
        top = static_cast<int>(w(context, 0x5cc)) - 1;
        mode = 0;
    } else {
        left = static_cast<int>(w(context, 0x514));
        bottom = static_cast<int>(w(context, 0x518));
        right = static_cast<int>(w(context, 0x51c)) + left - 1;
        top = static_cast<int>(w(context, 0x520)) + bottom - 1;
        int width = static_cast<int>(w(context, 0x5c8));
        int height = static_cast<int>(w(context, 0x5cc));
        if (width <= left) left = width - 1;
        else if (left < 0) left = 0;
        if (height <= bottom) bottom = height - 1;
        else if (bottom < 0) bottom = 0;
        if (width < right) right = width - 1;
        else if (right < 0) right = 0;
        if (height < top) top = height - 1;
        else if (top < 0) top = 0;
        mode = 3;
    }
    emit(mode, 0x000f0065);
    emit(static_cast<Word>(left) | (static_cast<Word>(bottom) << 16), 0x000f0066);
    emit(static_cast<Word>(right) | (static_cast<Word>(top) << 16), 0x000f0067);
}

inline Word attributeEncoding(Byte* context, int index) {
    Byte* attribute = context + 0x3e8 + index * 24;
    Word type = w(attribute, 8);
    Word packedType = type == 0x1401 ? 1 : type == 0x1402 ? 2 : type == 0x1406 ? 3 : 0;
    return ((w(attribute, 4) - 1) << 2) | packedType;
}

inline void putAttributeNibble(Word* words, Word position, Word value) {
    if (position < 8)
        words[0] |= value << (position * 4);
    else
        words[1] |= value << (position * 4 - 32);
}

inline void emitAttributeRegisters(Byte* shader, bool sameShader) {
    if (sameShader) {
        __cb_writeRegs(0x200, w(shader, 0x8bc) * 3 + 1,
                       reinterpret_cast<Word*>(shader + 0x8c0));
        return;
    }
    __cb_writeRegs(0x200, 39, reinterpret_cast<Word*>(shader + 0x8c0));
    emit(w(shader, 0x95c), 0x000f02bb);
    emit(w(shader, 0x960), 0x000f02bc);
    if (b(shader, 0x3f4)) {
        emit(w(shader, 0x964), 0x000f028b);
        emit(w(shader, 0x968), 0x000f028c);
    }
}

// Sort active arrays by their data address, cache the shape, and construct the
// PICA attribute-loader register block. Disabled arrays use fixed attributes.
inline void validateAttributes(Byte* flags, Byte* context, Byte* shader,
                               Byte* validator) {
    if (w(flags, 0) & 0x8042) {
        AttributeLink nodes[12];
        int fixed[12];
        AttributeLink* head = nodes;
        int arrayCount = 0;
        int fixedCount = 0;
        Word highestAddress = 0;
        bool sameShader = p(validator, 4) == shader;
        p(validator, 4) = shader;
        b(context, 0x71c) = 1;
        nodes[0].next = 0;
        for (int index = 11; index >= 0; --index) {
            if (w(shader, 0x358 + index * 12) == ~0u)
                continue;
            Byte* attribute = context + 0x3e8 + index * 24;
            if (!b(attribute, 0x15)) {
                fixed[fixedCount++] = index;
                continue;
            }
            if (!w(attribute, 0x10)) {
                b(context, 0x71c) = 0;
            } else if (w(context, 0x5d4 + index * 4) > highestAddress) {
                highestAddress = w(context, 0x5d4 + index * 4);
            }
            AttributeLink* added = nodes + arrayCount;
            added->index = index;
            if (arrayCount) {
                int address = static_cast<int>(w(context, 0x5d4 + index * 4));
                if (address <= static_cast<int>(w(context, 0x5d4 + head->index * 4))) {
                    added->next = head;
                    head = added;
                } else {
                    AttributeLink* previous = head;
                    AttributeLink* current = head->next;
                    while (current && address > static_cast<int>(w(context, 0x5d4 + current->index * 4))) {
                        previous = current;
                        current = current->next;
                    }
                    previous->next = added;
                    added->next = current;
                }
            }
            ++arrayCount;
        }
        if (b(context, 0x66c))
            b(context, 0x71c) = 0;
        if (arrayCount) {
            Word lowestAddress = w(context, 0x5d4 + head->index * 4) & ~15u;
            w(context, 0x604) = lowestAddress;
            if (b(context, 0x71c)) {
                if (!b(context, 0x19)) {
                    Word indexAddress = w(context, 0x5d0);
                    if (indexAddress < lowestAddress) lowestAddress = indexAddress;
                    if (indexAddress > highestAddress) highestAddress = indexAddress;
                }
                if (highestAddress - lowestAddress >= 0x10000000)
                    b(context, 0x71c) = 0;
            }
        } else {
            b(context, 0x71c) = 0;
        }

        bool rebuild = w(shader, 0x884) != static_cast<Word>(arrayCount) ||
                       w(shader, 0x8b8) != static_cast<Word>(fixedCount) ||
                       (w(context, 4) & 0x200);
        bool sameArrays = true;
        if (!rebuild) {
            AttributeLink* current = head;
            AttributeLink* cached = reinterpret_cast<AttributeLink*>(p(shader, 0x820));
            for (int i = 0; i < arrayCount; ++i, current = current->next, cached = cached->next) {
                int index = current->index;
                if (index != cached->index ||
                    w(context, 0x5d4 + index * 4) - w(context, 0x5d4 + head->index * 4) !=
                        w(shader, 0x824 + i * 4) - w(shader, 0x824) ||
                    w(context, 0x3f4 + index * 24) != w(shader, 0x854 + i * 4)) {
                    sameArrays = false;
                    break;
                }
                Word encoding = i < 8 ? w(shader, 0x8c4) >> (i * 4) : w(shader, 0x8c8) >> (i * 4 - 32);
                if ((attributeEncoding(context, index) & 0xff) != (encoding & 15)) {
                    sameArrays = false;
                    break;
                }
            }
            for (int i = 0; i < fixedCount; ++i) {
                if (static_cast<Word>(fixed[i]) != w(shader, 0x888 + i * 4)) {
                    rebuild = true;
                    break;
                }
            }
            if (!sameArrays || !b(context, 0x71c))
                rebuild = true;
        }
        if (!rebuild) {
            Word base = w(context, 0x604);
            if (!b(context, 0x19) && static_cast<int>(w(context, 0x5d0)) < static_cast<int>(base))
                base = w(context, 0x5d0) & ~15u;
            if (w(context, 0x604) != base || b(shader, 0x3f5) ||
                w(context, 0x5d4 + head->index * 4) - base != w(shader, 0x8cc)) {
                Word offset = w(context, 0x5d4 + head->index * 4) - base;
                for (int i = 1; i < static_cast<int>(w(shader, 0x8bc)); ++i)
                    w(shader, 0x8cc + i * 12) = w(shader, 0x8cc + i * 12) - w(shader, 0x8cc) + offset;
                w(shader, 0x8cc) = offset;
                sameArrays = false;
                if (w(context, 0x604) == base)
                    b(shader, 0x3f5) = 0;
            } else {
                sameArrays = sameShader;
            }
            w(context, 0x604) = base;
            w(shader, 0x8c0) = base >> 3;
            if (sameArrays) {
                emit(base >> 3, 0x000f0200);
            } else {
                emitAttributeRegisters(shader, sameShader);
                if (!sameShader) {
                    w(context, 0x720) = arrayCount;
                    w(context, 0x724) = arrayCount + fixedCount;
                    int i = 0;
                    AttributeLink* node = reinterpret_cast<AttributeLink*>(p(shader, 0x820));
                    for (; i < arrayCount; ++i, node = node->next)
                        w(context, 0x728 + i * 4) = node->index;
                    for (int j = 0; j < fixedCount; ++j, ++i)
                        w(context, 0x728 + i * 4) = fixed[j];
                }
            }
        } else {
            AttributeLink* current = head;
            AttributeLink* cached = reinterpret_cast<AttributeLink*>(shader + 0x7c0);
            for (int i = 0; i < arrayCount; ++i) {
                int index = current->index;
                cached[i].index = index;
                w(shader, 0x824 + i * 4) = w(context, 0x5d4 + index * 4);
                w(shader, 0x854 + i * 4) = w(context, 0x3f4 + index * 24);
                if (i != arrayCount - 1) {
                    cached[i].next = cached + i + 1;
                    current = current->next;
                } else {
                    cached[i].next = 0;
                }
            }
            for (int i = 0; i < fixedCount; ++i)
                w(shader, 0x888 + i * 4) = fixed[i];
            w(shader, 0x884) = arrayCount;
            w(shader, 0x8b8) = fixedCount;
            p(shader, 0x820) = reinterpret_cast<Byte*>(cached);
            if (!w(context, 0x508) && !b(context, 0x19))
                b(context, 0x71c) = 0;
            if (b(context, 0x19)) {
                w(shader, 0x8c0) = static_cast<int>(w(context, 0x604)) >> 3;
                b(shader, 0x3f5) = 0;
            } else {
                if (static_cast<int>(w(context, 0x5d0)) < static_cast<int>(w(context, 0x604))) {
                    w(context, 0x604) = w(context, 0x5d0) & ~15u;
                    b(shader, 0x3f5) = 1;
                } else {
                    b(shader, 0x3f5) = 0;
                }
                w(shader, 0x8c0) = static_cast<int>(w(context, 0x604)) >> 3;
            }
            if (b(context, 0x71c)) {
                Word* registers = reinterpret_cast<Word*>(shader + 0x8c0);
                w(shader, 0x8bc) = 0;
                for (int i = 1; i < 39; ++i)
                    reinterpret_cast<volatile Word*>(registers)[i] = 0;
                Word* loader = registers + 3;
                Word loaderComponents = 0;
                Word packedBytes = 0;
                Word alignment = 1;
                Word stride = 0;
                Word groupEnd = 0;
                current = head;
                for (int i = 0; i < arrayCount; ++i) {
                    int index = current->index;
                    AttributeLink* next = current->next;
                    Byte* attribute = context + 0x3e8 + index * 24;
                    Word offset = w(context, 0x5d4 + index * 4) - w(context, 0x604);
                    Word type = w(attribute, 8);
                    Word typeSize = type == 0x1400 || type == 0x1401 ? 1 : type == 0x1402 ? 2 : type == 0x1406 ? 4 : 0;
                    putAttributeNibble(registers + 1, i, attributeEncoding(context, index));
                    if (!w(attribute, 12)) {
                        loader[0] = offset;
                        loader[1] |= i;
                        loader[2] |= 0x10000000 | ((w(attribute, 4) * typeSize) << 16);
                        loader += 3;
                        ++w(shader, 0x8bc);
                    } else {
                        if (alignment < typeSize) alignment = typeSize;
                        packedBytes = (packedBytes + typeSize - 1) & ~(typeSize - 1);
                        Word size = w(attribute, 4) * typeSize;
                        packedBytes += size;
                        if (!loaderComponents) {
                            loader[0] = offset;
                            stride = w(attribute, 12);
                            groupEnd = w(context, 0x5d4 + index * 4) + stride;
                        }
                        putAttributeNibble(loader + 1, loaderComponents++, i);
                        bool endGroup = i + 1 == arrayCount;
                        if (!endGroup) {
                            Word nextAddress = w(context, 0x5d4 + next->index * 4);
                            endGroup = nextAddress - w(context, 0x604) <= offset ||
                                       w(attribute, 12) != w(context, 0x3f4 + next->index * 24) ||
                                       static_cast<int>(nextAddress) >= static_cast<int>(groupEnd) ||
                                       loaderComponents == 12;
                        }
                        Word nextOffset = endGroup ? loader[0] + stride :
                            w(context, 0x5d4 + next->index * 4) - w(context, 0x604);
                        Word paddingWords = (nextOffset - offset - size) >> 2;
                        if ((paddingWords >> 2) + ((paddingWords & 3) != 0) > 12 - loaderComponents) {
                            b(context, 0x71c) = 0;
                            break;
                        }
                        if (paddingWords) {
                            packedBytes = ((packedBytes + 3) & ~3u) + paddingWords * 4;
                            if (paddingWords & 3) {
                                putAttributeNibble(loader + 1, loaderComponents++, (paddingWords & 3) + 11);
                                paddingWords &= ~3u;
                            }
                            while (paddingWords) {
                                putAttributeNibble(loader + 1, loaderComponents++, 15);
                                paddingWords -= 4;
                            }
                        }
                        if (endGroup) {
                            if (((packedBytes + alignment - 1) & ~(alignment - 1)) != stride) {
                                b(context, 0x71c) = 0;
                                break;
                            }
                            loader[2] |= (stride << 16) | (loaderComponents << 28);
                            alignment = 1;
                            loaderComponents = 0;
                            packedBytes = 0;
                            loader += 3;
                            ++w(shader, 0x8bc);
                        }
                    }
                    current = next;
                }
                if (b(context, 0x71c)) {
                    for (int i = 0; i < fixedCount; ++i)
                        registers[2] |= 0x10000u << (i + arrayCount);
                    if (arrayCount)
                        registers[2] |= 0xf0000000u + ((arrayCount + fixedCount) << 28);
                }
            }
            w(context, 0x720) = arrayCount;
            w(context, 0x724) = arrayCount + fixedCount;
            w(shader, 0x95c) = 0;
            w(shader, 0x960) = 0;
            w(shader, 0x964) = 0x76543210;
            w(shader, 0x968) = 0xfedcba98;
            current = reinterpret_cast<AttributeLink*>(p(shader, 0x820));
            int i = 0;
            for (; i < arrayCount; ++i, current = current->next) {
                int index = current->index;
                putAttributeNibble(reinterpret_cast<Word*>(shader + 0x95c), i,
                                   w(shader, 0x358 + index * 12) & 15);
                w(context, 0x728 + i * 4) = index;
            }
            for (int j = 0; j < fixedCount; ++j, ++i) {
                putAttributeNibble(reinterpret_cast<Word*>(shader + 0x95c), i,
                                   w(shader, 0x358 + fixed[j] * 12) & 15);
                w(context, 0x728 + i * 4) = fixed[j];
            }
            emitAttributeRegisters(shader, false);
        }
    }
    if (w(flags, 0) & 0x80c2) {
        for (Word i = w(context, 0x720); i < w(context, 0x724); ++i) {
            Word index = w(context, 0x728 + i * 4);
            emit(i, 0x000f0232);
            emit(w(context, 0x358 + index * 12), 0x000f0233);
            emit(w(context, 0x35c + index * 12), 0x000f0234);
            emit(w(context, 0x360 + index * 12), 0x000f0235);
        }
    }
}

inline void validateShaderFront(Byte* flags, Word category, Byte* context,
                                Byte* shader, Byte* validator, bool full = false) {
    if (category & 4) {
        Word geometry = b(shader, 0x3f4) != 0;
        if (dat_003E2E40[3] != geometry || (w(context, 4) & 4)) {
            __cb_addDummyWrite(0x251, 10);
            __cb_addDummyWrite(0x200, 30);
            emit(geometry ? 2 : 0, 0x00010229);
            __cb_addDummyWrite(0x200, 30);
            dat_003E2E40[3] = geometry;
            emit(geometry ? 1 : 0, 0x00010244);
        }
    }
    // The full validator separately clears the geometry-input mode and marks
    // its byte mask before the program transfer. The partial entry omits it.
    if (full && b(shader, 0x3f4)) {
        w(shader, 0x4b8) &= ~0x8000u;
        w(validator, 0x1010) |= 0x8000;
        b(shader, 0x3f7) |= 2;
        w(shader, 0x7a8) |= 2;
        if ((category & 4) && (w(flags, 0) & 0x800000)) {
            for (Word i = 1; i <= 8; ++i)
                w(validator, 0x100c + i * 4) = ~w(shader, 0x4b4 + i * 4);
        }
    }
    if ((category & 1) && (w(flags, 0) & 0x100000)) {
        Byte* binary = p(shader, 0x3e4);
        if (b(shader, 0x3f4)) {
            emit(0, 0x000f029b);
            __cb_multiWriteReg(0x29c, w(binary, 4), reinterpret_cast<Word*>(p(binary, 0)));
            emit(1, 0x000f028f);
            emit(0, 0x000f02a5);
            __cb_multiWriteReg(0x2a6, w(binary, 12), reinterpret_cast<Word*>(p(binary, 8)));
        } else if (w(binary, 4) > 512) {
            emit(512, 0x000f029b);
            __cb_multiWriteReg(0x29c, w(binary, 4) - 512, reinterpret_cast<Word*>(p(binary, 0)) + 512);
            emit(1, 0x000f028f);
        }
        emit(0, 0x000f02cb);
        Word length = w(binary, 4);
        if (length > 512) length = 512;
        __cb_multiWriteReg(0x2cc, length, reinterpret_cast<Word*>(p(binary, 0)));
        emit(1, 0x000f02bf);
        emit(0, 0x000f02d5);
        __cb_multiWriteReg(0x2d6, w(binary, 12), reinterpret_cast<Word*>(p(binary, 8)));
    }
    if (category & 4) {
        if (!full && b(shader, 0x3f4) && (w(flags, 0) & 0x800000)) {
            for (Word i = 1; i <= 8; ++i)
                w(validator, 0x100c + i * 4) = ~w(shader, 0x4b4 + i * 4);
        }
        if (w(flags, 0) & 0x1000000) {
            for (Word i = 9; i <= 16; ++i)
                w(validator, 0x100c + i * 4) = ~w(shader, 0x4b4 + i * 4);
        }
    }
    if ((category & 8) && (w(flags, 0) & 0x600000)) {
        Byte* binary = p(shader, 0x3e4);
        if (b(shader, 0x3f4) && (w(flags, 0) & 0x400000)) {
            Byte* program = p(binary, 0x10) + w(shader, 0x3ec) * 232;
            for (Word i = 0; i < w(program, 0x34); ++i)
                __cb_writeRegs(0x290, 4, reinterpret_cast<Word*>(p(program, 0x30)) + i * 4);
        }
        if (w(flags, 0) & 0x200000) {
            Byte* program = p(binary, 0x10) + w(shader, 0x3e8) * 232;
            for (Word i = 0; i < w(program, 0x34); ++i)
                __cb_writeRegs(0x2c0, 4, reinterpret_cast<Word*>(p(program, 0x30)) + i * 4);
        }
    }
    if (category & 0x200)
        validateAttributes(flags, context, shader, validator);
}

} // namespace ShvReconstruction
