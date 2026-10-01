#ifndef RETAIL_JPEG_DECODER_ROOT_H
#define RETAIL_JPEG_DECODER_ROOT_H

// Layout views recovered from the owner's EU executable. This is not a claim
// about the names or complete declaration of the original SDK context.
namespace nn { namespace jpeg { namespace CTR {
struct JpegMpDecoderContext {
    unsigned int inputAddress, inputSize, outputCapacity;
    unsigned int restartCountdown, restartInterval, outputAddress;
    unsigned short width, height, maximumWidth, maximumHeight;
    unsigned short requiredWidth, requiredHeight;
    short predictorY, predictorCb, predictorCr;
    signed char bufferedBits;
    unsigned char lastByte, samplingX, samplingY, blockWidth, blockHeight;
    unsigned char luminanceBlocks, unknown31, metadataPresent, unknown33;
    unsigned int metadataAddress, outputWidth, outputHeight, outputBlockAddress;
    union Status { unsigned int all; unsigned char bytes[4]; } status;
    unsigned char outputFormat, unknown49, app1Seen, headerOnly, secondImageRequested, scale;
    unsigned char unknown4e[6];
    unsigned int cursor, inputEndAddress, presentMarkers, requiredMarkers, options;
    unsigned char unknown68[0x18];
    unsigned char workspace[0x22f0];
};
typedef char JpegContextObservedSize[(sizeof(JpegMpDecoderContext) == 0x2370) ? 1 : -1];
namespace detail {
void JpegMpDecoderAsmConvertWorkToAsm(JpegMpDecoderContext*);
void JpegMpDecoderAsmConvertWorkToC(JpegMpDecoderContext*);
void JpegMpDecoderAsmGetMatrix(JpegMpDecoderContext*, int, short*);
void JpegMpDecoderAsmDecodeBlock(JpegMpDecoderContext*, unsigned char*, int);
}
}}}

namespace RetailJpeg {
typedef unsigned char Byte;
typedef unsigned short Half;
typedef unsigned int Word;
typedef nn::jpeg::CTR::JpegMpDecoderContext Context;
typedef void (*OutputBlock)(Context*, Word, Word);
inline Byte& byte(Context* c, Word at) { return reinterpret_cast<Byte*>(c)[at]; }
inline signed char& signedByte(Context* c, Word at) { return *reinterpret_cast<signed char*>(reinterpret_cast<Byte*>(c) + at); }
inline Half& half(Context* c, Word at) { return *reinterpret_cast<Half*>(reinterpret_cast<Byte*>(c) + at); }
inline Word& word(Context* c, Word at) { return *reinterpret_cast<Word*>(reinterpret_cast<Byte*>(c) + at); }
inline Byte* buffer(Context* c, Word at) { return reinterpret_cast<Byte*>(c) + at; }
inline Word bigEndianHalf(const Byte* p) { return (Word(p[0]) << 8) | p[1]; }
inline void error(Context* c, int value) { if (!c->status.bytes[0]) c->status.bytes[0] = Byte(value); }
inline Word alignWidth(Word width, Byte format) {
    switch (format) {
    case 0: return (width + 1) & ~1u;
    case 1: case 3: case 5: case 7: case 8: return width;
    case 2: case 4: case 6: return (width + 7) & ~7u;
    default: return 0;
    }
}
inline Word alignHeight(Word height, Byte format) {
    switch (format) {
    case 0: case 1: case 3: case 5: case 7: case 8: return height;
    case 2: case 4: case 6: return (height + 7) & ~7u;
    default: return 0;
    }
}
}

extern "C" {
void fn_002397A8(RetailJpeg::Context*);
int fn_001FDCE8(RetailJpeg::Context*, unsigned int);
int fn_001FE5E0(RetailJpeg::Context*, unsigned int);
unsigned int fn_002396F0(unsigned int, unsigned int, unsigned int);
void __aeabi_memcpy4(void*, const void*, unsigned int);
extern const RetailJpeg::OutputBlock dat_003B4A30[36];
extern const RetailJpeg::OutputBlock dat_003B4AC0[36];
extern const RetailJpeg::OutputBlock dat_003B4B50[36];
extern const unsigned char dat_003B2BE0[];
extern const unsigned char dat_003B3A39[];
extern const short dat_003B49A0[];
}
#endif
