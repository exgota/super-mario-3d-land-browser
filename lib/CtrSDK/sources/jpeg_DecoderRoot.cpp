#include "retail/jpeg_DecoderRoot.h"

// NonMatching: full marker/scan driver at 002397A8..0023A854. Its five
// observed callers read context+44 after return; the return register is unused.
#ifdef NON_MATCHING
extern "C" void fn_002397A8(RetailJpeg::Context* c) {
    using namespace RetailJpeg;
    using namespace nn::jpeg::CTR::detail;
    const Byte* input = reinterpret_cast<const Byte*>(c->inputAddress);
    bool secondImage = false;
    Word cursor = c->cursor;
    if (cursor + 2 >= c->inputSize || input[cursor] != 0xff || input[cursor + 1] != 0xd8) {
        error(c, -60); return;
    }
    c->cursor = cursor + 2;
restart:
    c->restartCountdown = 0;
    c->restartInterval = 0;
    while (c->cursor + 1 < c->inputSize) {
        cursor = c->cursor++;
        if (input[cursor] != 0xff) continue;
        cursor = c->cursor++;
        Word marker = input[cursor];
        if (marker == 0xd9) break;
        cursor = c->cursor;
        if (cursor + 1 >= c->inputSize) { error(c, 0xa6); return; }
        Word length = bigEndianHalf(input + cursor);
        if (!secondImage && marker == 0xe1) {
            if (c->app1Seen) c->cursor = cursor + length;
            else {
                c->app1Seen = 1;
                if (!fn_001FDCE8(c, length)) { error(c, -30); return; }
            }
        } else if (!secondImage && marker == 0xe2) {
            if (!fn_001FE5E0(c, length)) { error(c, -32); return; }
            c->cursor += length;
        } else if (marker == 0xc0 || marker == 0xc1) {
            if (cursor + 9 >= c->inputSize) { error(c, -61); return; }
            if (c->secondImageRequested && !secondImage) { error(c, -31); return; }
            c->height = bigEndianHalf(input + cursor + 3);
            c->width = bigEndianHalf(input + cursor + 5);
            Word sampling = input[cursor + 9];
            c->cursor = cursor + length;
            if (c->headerOnly) continue;
            if (sampling != 0x11 && sampling != 0x21 && sampling != 0x12 && sampling != 0x22) {
                error(c, -62); return;
            }
            c->samplingX = sampling >> 4;
            c->samplingY = sampling & 15;
            c->blockWidth = c->samplingX * 8;
            c->blockHeight = c->samplingY * 8;
            if (!c->width || !c->height || c->width > c->maximumWidth || c->height > c->maximumHeight) {
                error(c, -20); return;
            }
            if ((c->requiredWidth && c->requiredWidth != c->width) || (c->requiredHeight && c->requiredHeight != c->height)) {
                error(c, -21); return;
            }
            Word shift = c->scale;
            Word rounding = (1u << shift) - 1;
            Word width = (int(c->width + rounding)) >> shift;
            Word height = (int(c->height + rounding)) >> shift;
            if (alignWidth(width, c->outputFormat) > c->outputWidth) c->outputWidth = alignWidth(width, c->outputFormat);
            if (alignHeight(height, c->outputFormat) > c->outputHeight) c->outputHeight = alignHeight(height, c->outputFormat);
            if (fn_002396F0(c->outputWidth, c->outputHeight, c->outputFormat) > c->outputCapacity) { error(c, -127); return; }
            if (!c->samplingX || c->samplingX > 2 || !c->samplingY || c->samplingY > 2) { error(c, -62); return; }
            c->luminanceBlocks = c->samplingX * c->samplingY;
            c->outputBlockAddress = 0;
            Word index = c->outputFormat * 4 + c->samplingY * 2 + c->samplingX - 3;
            if (!(c->width & (c->blockWidth - 1)) && !(c->height & (c->blockHeight - 1)))
                c->outputBlockAddress = reinterpret_cast<Word>(dat_003B4A30[index]);
            if (!c->outputBlockAddress) c->outputBlockAddress = reinterpret_cast<Word>(dat_003B4AC0[index]);
            if (!c->outputBlockAddress) { error(c, -127); return; }
            if (c->scale) {
                c->outputBlockAddress = reinterpret_cast<Word>(dat_003B4B50[index]);
                if (!c->outputBlockAddress) { error(c, -127); return; }
            }
            c->presentMarkers |= 1;
            c->requiredMarkers |= 0x10000;
        } else if (marker == 0xc4) {
            if (cursor + 2 >= c->inputSize) { error(c, -63); return; }
            Word next = cursor + 2;
            c->cursor = cursor + length;
            while (c->cursor > next) {
                if (c->inputSize <= next + 17) { error(c, -63); return; }
                Word selector = input[next];
                Word raw = (selector & 1) ? 0x138 : 0;
                Word derived = (selector & 1) ? 0x560 : 0;
                if (selector & 0x10) { raw += 0x24; derived += 0x2b0; }
                byte(c, raw + 0x500) = 0;
                int count = 0;
                for (int i = 1; i <= 16; ++i) {
                    byte(c, raw + 0x500 + i) = input[next + i];
                    count += input[next + i];
                }
                next += 17;
                Word symbolsEnd = next + count;
                if (symbolsEnd >= c->inputSize) { error(c, -63); return; }
                for (int i = 0; i < count; ++i) byte(c, raw + 0x514 + i) = input[next + i];
                next = symbolsEnd;
                Word total = 0;
                for (int bits = 1; bits <= 16; ++bits) {
                    for (int i = 1; i <= byte(c, raw + 0x500 + bits) && total <= 256; ++i)
                        byte(c, 0x1f02 + total++) = bits;
                }
                if (total > 256) { error(c, -63); return; }
                byte(c, 0x1f02 + total) = 0;
                Word code = 0;
                int bits = signedByte(c, 0x1f02);
                Word position = 0;
                if (bits) {
                    do {
                        while (signedByte(c, 0x1f02 + position) == bits) {
                            half(c, 0x1d00 + position * 2) = code;
                            ++position; ++code;
                            if (position > 256) { error(c, -63); return; }
                        }
                        code <<= 1;
                        ++bits;
                    } while (byte(c, 0x1f02 + position));
                }
                int first = 0;
                for (int bits = 1; bits <= 16; ++bits) {
                    int n = byte(c, raw + 0x500 + bits);
                    if (!n) word(c, derived + 0x7b4 + bits * 4) = ~0u;
                    else {
                        half(c, derived + 0x7fc + bits * 2) = first;
                        word(c, derived + 0x770 + bits * 4) = half(c, 0x1d00 + first * 2);
                        first += n;
                        if (first > 256) { error(c, -63); return; }
                        word(c, derived + 0x7b4 + bits * 4) = half(c, 0x1d00 + (first - 1) * 2);
                    }
                }
                word(c, derived + 0x7f8) = 0xfffff;
                Word symbol = 0;
                for (int bits = 1; bits <= 8; ++bits) {
                    int repeat = 1 << (8 - bits);
                    for (int n = 1; n <= byte(c, raw + 0x500 + bits); ++n) {
                        if (symbol > 256) { error(c, -63); return; }
                        int lookup = half(c, 0x1d00 + symbol * 2) << (8 - bits);
                        if (lookup < -3118 || lookup + repeat >= 3027) { error(c, -63); return; }
                        for (int j = 0; j < repeat; ++j) {
                            int index = derived + lookup;
                            if (index + 0x31e < 0 || Word(index + 0x41e) >= 0x1800) { error(c, -63); return; }
                            byte(c, index + 0x81e) = bits;
                            byte(c, index + 0x91e) = byte(c, raw + 0x514 + symbol);
                            ++lookup;
                        }
                        ++symbol;
                    }
                }
                Word flag;
                switch (selector) {
                case 0: flag = 0x10; break;
                case 1: flag = 0x20; break;
                case 0x10: flag = 0x40; break;
                case 0x11: flag = 0x80; break;
                default: error(c, -127); return;
                }
                c->presentMarkers |= flag;
            }
        } else if (!secondImage && marker == 0xd8) {
            if (c->secondImageRequested) {
                secondImage = true;
                c->presentMarkers = 0;
                c->requiredMarkers = 0;
                goto restart;
            }
        } else if (marker == 0xda) {
            if (c->headerOnly) {
                if (c->secondImageRequested && !secondImage) error(c, -31);
                return;
            }
            Word flags = c->presentMarkers;
            if (flags & 0x10000) { error(c, -64); return; }
            if (!(flags & 1)) { error(c, -68); return; }
            if (!(flags & 0x1000)) { error(c, -69); return; }
            if ((flags & 0xf0) != 0xf0) {
                if (!(c->options & 1)) { error(c, -70); return; }
                __aeabi_memcpy4(buffer(c, 0x500), dat_003B2BE0, 0x1800);
            }
            if (c->cursor + 12 >= c->inputSize) { error(c, -64); return; }
            c->cursor += 12;
            JpegMpDecoderAsmConvertWorkToAsm(c);
            if (!c->outputBlockAddress) { error(c, -127); return; }
            for (Word y = 0; y < c->height; y += c->blockHeight) {
                for (Word x = 0; x < c->width; x += c->blockWidth) {
                    if (c->restartInterval && --c->restartCountdown == 0) {
                        c->restartCountdown = c->restartInterval;
                        JpegMpDecoderAsmConvertWorkToC(c);
                        if (c->bufferedBits > 7) {
                            c->cursor -= 1 + (c->lastByte == 0xff);
                        }
                        c->cursor += 2;
                        c->bufferedBits = 0;
                        c->predictorCr = 0;
                        c->predictorCb = 0;
                        c->predictorY = 0;
                        JpegMpDecoderAsmConvertWorkToAsm(c);
                    }
                    for (Word block = 0; block < c->luminanceBlocks; ++block) {
                        JpegMpDecoderAsmGetMatrix(c, 0, reinterpret_cast<short*>(buffer(c, 0x24)));
                        JpegMpDecoderAsmDecodeBlock(c, buffer(c, 0x400 + block * 64), 0);
                    }
                    JpegMpDecoderAsmGetMatrix(c, 2, reinterpret_cast<short*>(buffer(c, 0x26)));
                    JpegMpDecoderAsmDecodeBlock(c, buffer(c, 0x380), 0x40);
                    JpegMpDecoderAsmGetMatrix(c, 2, reinterpret_cast<short*>(buffer(c, 0x28)));
                    JpegMpDecoderAsmDecodeBlock(c, buffer(c, 0x3c0), 0x40);
                    if (c->status.all) {
                        JpegMpDecoderAsmConvertWorkToC(c);
                        if (c->status.bytes[1]) error(c, -10);
                        return;
                    }
                    reinterpret_cast<OutputBlock>(c->outputBlockAddress)(c, x, y);
                }
            }
            JpegMpDecoderAsmConvertWorkToC(c);
            c->presentMarkers |= 0x10000;
            break;
        } else if (marker == 0xdb) {
            Word next = cursor + 2;
            c->cursor = cursor + length;
            while (c->cursor > next) {
                Word selector = input[next++];
                Word table = (selector & 15);
                if (table >= 3) { error(c, -65); return; }
                table *= 64;
                const Byte* order = dat_003B3A39 + 0x9a7;
                if (selector & 0xf0) {
                    if (c->inputSize <= next + 128) { error(c, -96); return; }
                    for (Word i = 0; i < 64; ++i) {
                        Word value = bigEndianHalf(input + next);
                        next += 2;
                        Word scaled = value * int(dat_003B49A0[i]);
                        half(c, 0x80 + (table + order[i]) * 2) = int((scaled << 5) + 0x8000) >> 16;
                    }
                } else {
                    if (c->inputSize <= next + 64) { error(c, -96); return; }
                    for (Word i = 0; i < 64; ++i) {
                        int scaled = input[next++] * int(dat_003B49A0[i]);
                        half(c, 0x80 + (table + order[i]) * 2) = (scaled + 0x400) >> 11;
                    }
                }
            }
            c->presentMarkers |= 0x1000;
        } else if (marker == 0xdd) {
            if (cursor + 3 >= c->inputSize) { error(c, -66); return; }
            Word interval = bigEndianHalf(input + cursor + 2);
            c->restartInterval = interval;
            c->restartCountdown = interval + 1;
            c->cursor = cursor + length;
        } else c->cursor = cursor + length;
    }
    if (c->cursor > c->inputSize) { error(c, -91); return; }
    if (!(c->presentMarkers & 0x10000)) { error(c, -67); return; }
    if (c->requiredMarkers & ~c->presentMarkers) error(c, -50);
}
#endif
