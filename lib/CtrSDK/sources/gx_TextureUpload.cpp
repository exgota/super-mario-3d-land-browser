// Retail EU 0x0020FC34. The original public/internal function name is unknown.
// Clean-room reconstruction from this routine, __srf_initSurface, and its
// glCompressedTexImage2D caller. These are observed prefixes, not SDK headers.
namespace {
struct TextureUploadOwner {
    unsigned char unknown[0x2c];
    unsigned int storageMode;
    unsigned char generateMipmaps;
};
struct TextureUploadSurface {
    unsigned char* resident;
    unsigned char* staging;
    const unsigned char* input;
    int width;
    int height;
    int format;
    int type;
    unsigned int pixelFormat;
    int totalBytes;
    unsigned char unknown24[0x10];
    int levels;
    int bitsPerPixel;
    unsigned int allocationRegion;
    unsigned int alignment;
};
typedef void* (*TextureAllocate)(unsigned int, unsigned int, unsigned int, int);
typedef void (*TextureRelease)(unsigned int, unsigned int, unsigned int, void*);
}
extern "C" {
extern TextureAllocate dat_003E2654;
// Only the leading callback cell of the existing 16-byte row is referenced.
extern TextureRelease dat_003E2658;
void __srf_initSurface(int, int, int, int, int, int, TextureUploadSurface*);
void fn_0028B068(unsigned int, TextureUploadSurface*);
int fn_00205FA4(unsigned int, TextureUploadSurface*);
void __rt_memcpy(void*, const void*, unsigned int);
void fn_002106F0(void*, const void*, unsigned int);
void fn_00391030(void*, unsigned int);
void fn_00205EE8(void*, void*, int, int, int);
}

// NonMatching: original boundaries and public-name uncertainty are retained.
#ifdef NON_MATCHING
extern "C" void fn_0020FC34(TextureUploadOwner* owner,
                             TextureUploadSurface* surface, int levels,
                             int width, int height, int format, int type,
                             const unsigned char* data, int initFlags,
                             bool nativeOrder, unsigned int storageMode)
{
    if (surface->resident) {
        if (surface->width != width || surface->height != height ||
            surface->format != format || owner->storageMode != storageMode ||
            surface->levels != levels || surface->type != type)
            fn_0028B068(owner->storageMode, surface);
    }
    if (!surface->resident) {
        __srf_initSurface(width, height, format, type, levels, initFlags, surface);
        if (!fn_00205FA4(storageMode, surface))
            return;
    }
    if (data) {
        int uploadLevels = levels;
        int uploadBytes = surface->totalBytes;
        if (owner->generateMipmaps) {
            uploadBytes = surface->bitsPerPixel * (width * height) / 8;
            uploadLevels = 1;
        }
        surface->input = data;
        switch (storageMode) {
        case 0x01010000:
        case 0x01020000:
        case 0x01030000:
            if (nativeOrder) {
                __rt_memcpy(surface->staging, data, uploadBytes);
            } else {
                int offset = 0;
                for (int level = 0; level < uploadLevels; ++level) {
                    const unsigned char* source = data + offset;
                    unsigned char* destination = surface->staging + offset;
                    unsigned int pixelFormat = surface->pixelFormat;
                    int bytesPerPixel = 4;
                    bool halfByte = false;
                    switch (pixelFormat) {
                    case 1: bytesPerPixel = 3; break;
                    case 2: case 3: case 4: case 5: case 6:
                        bytesPerPixel = 2; break;
                    case 7: case 8: case 9: bytesPerPixel = 1; break;
                    case 10: case 11:
                        bytesPerPixel = 1; halfByte = true; break;
                    }
                    if (pixelFormat == 12 || pixelFormat == 13) {
                        // The binary reuses its format scratch slot for the zero
                        // block origin. Later mip selection overrides it only
                        // for pixel formats 0..4.
                        format = 0;
                        unsigned int blockWidth = width / 4;
                        unsigned int blockHeight = height / 4;
                        for (unsigned int y = 0; y < blockHeight; ++y) {
                            for (unsigned int x = 0; x < blockWidth; ++x) {
                                unsigned int tiled =
                                    ((x >> 1) + (((y >> 1) * blockWidth) >> 1)) * 4 +
                                    (x & 1) + ((y & 1) << 1);
                                unsigned int linear = y * blockWidth + x;
                                if (tiled < blockWidth * blockHeight &&
                                    linear < blockWidth * blockHeight) {
                                    const unsigned int* words = reinterpret_cast<const unsigned int*>(source);
                                    unsigned int* output = reinterpret_cast<unsigned int*>(destination);
                                    if (pixelFormat == 12) {
                                        output[tiled * 2] = words[linear * 2];
                                        output[tiled * 2 + 1] = words[linear * 2 + 1];
                                    } else {
                                        output[tiled * 4] = words[linear * 4];
                                        output[tiled * 4 + 1] = words[linear * 4 + 1];
                                        output[tiled * 4 + 2] = words[linear * 4 + 2];
                                        output[tiled * 4 + 3] = words[linear * 4 + 3];
                                    }
                                }
                            }
                        }
                    } else {
                        int temporaryBytes = width * height * bytesPerPixel;
                        if (halfByte) temporaryBytes /= 2;
                        unsigned char* temporary = dat_003E2654 ?
                            static_cast<unsigned char*>(dat_003E2654(0x10000, 0x100, 0, temporaryBytes)) : 0;
                        int count = width * height;
                        switch (pixelFormat) {
                        case 0:
                            for (int i = 0; i < count; ++i) {
                                unsigned int value = reinterpret_cast<const unsigned int*>(source)[i];
                                reinterpret_cast<unsigned int*>(temporary)[i] =
                                    (value >> 24) | ((value >> 8) & 0xff00) |
                                    ((value << 8) & 0xff0000) | (value << 24);
                            }
                            break;
                        case 1:
                            for (int i = 0; i < count; ++i) {
                                unsigned char blue = source[i * 3 + 2];
                                unsigned char green = source[i * 3 + 1];
                                unsigned char red = source[i * 3];
                                temporary[i * 3] = blue;
                                temporary[i * 3 + 1] = green;
                                temporary[i * 3 + 2] = red;
                            }
                            break;
                        case 5: case 6:
                            for (int i = 0; i < count; ++i) {
                                unsigned short value = reinterpret_cast<const unsigned short*>(source)[i];
                                reinterpret_cast<unsigned short*>(temporary)[i] = (value >> 8) | (value << 8);
                            }
                            break;
                        case 2: case 3: case 4:
                            for (int i = 0; i < count; ++i)
                                reinterpret_cast<unsigned short*>(temporary)[i] =
                                    reinterpret_cast<const unsigned short*>(source)[i];
                            break;
                        case 7: case 8: case 9:
                            for (int i = 0; i < count; ++i) temporary[i] = source[i];
                            break;
                        case 10: case 11:
                            for (int i = 0; i < count / 2; ++i) temporary[i] = source[i];
                            break;
                        }
                        for (unsigned int y = 0; y < static_cast<unsigned int>(height); ++y) {
                            unsigned int sourceY = y * height / static_cast<unsigned int>(height);
                            unsigned int destinationY = height - y - 1;
                            // Preserve the original scratch selector for formats
                            // outside the five explicit mip-format cases below.
                            format = destinationY;
                            for (unsigned int x = 0; x < static_cast<unsigned int>(width); ++x) {
                                unsigned int tiled =
                                    ((x >> 3) + (((destinationY >> 3) * width) >> 3)) << 6;
                                unsigned int sourceX = x * width / static_cast<unsigned int>(width);
                                for (int bit = 0; (1 << bit) < 8; ++bit) {
                                    unsigned int mask = 1 << bit;
                                    tiled += ((x & mask) + ((destinationY & mask) << 1)) * mask;
                                }
                                unsigned int linear = sourceY * width + sourceX;
                                if (tiled < static_cast<unsigned int>(width * height) &&
                                    linear < static_cast<unsigned int>(width * height)) {
                                    if (halfByte) {
                                        unsigned char* to = destination + tiled * bytesPerPixel / 2;
                                        const unsigned char* from = temporary + linear * bytesPerPixel / 2;
                                        if (x & 1) *to = (*from & 0xf0) | (*to & 0x0f);
                                        else *to = (*from & 0x0f) | (*to & 0xf0);
                                    } else {
                                        unsigned char* to = destination + tiled * bytesPerPixel;
                                        const unsigned char* from = temporary + linear * bytesPerPixel;
                                        for (int byte = 0; byte < bytesPerPixel; ++byte)
                                            to[byte] = from[byte];
                                    }
                                }
                            }
                        }
                        if (dat_003E2658) dat_003E2658(0x10000, 0x100, 0, temporary);
                    }
                    offset += surface->bitsPerPixel * (width * height) / 8;
                    width >>= 1;
                    height >>= 1;
                }
            }
            break;
        case 0x02010000:
            surface->resident = const_cast<unsigned char*>(data);
            break;
        }
        switch (storageMode) {
        case 0x01020000: case 0x01030000:
            fn_002106F0(surface->resident, surface->staging, uploadBytes);
            break;
        case 0x02020000: case 0x02030000:
            fn_002106F0(surface->resident, surface->input, uploadBytes);
            break;
        }
        if (storageMode == 0x01010000 || storageMode == 0x02010000)
            fn_00391030(surface->resident, uploadBytes);
        if (owner->generateMipmaps) {
            width = surface->width;
            height = surface->height;
            unsigned char* source = surface->resident;
            switch (surface->pixelFormat) {
            case 0: format = 0; break;
            case 1: format = 1; break;
            case 2: format = 3; break;
            case 3: format = 2; break;
            case 4: format = 4; break;
            }
            for (int level = 1; level < surface->levels; ++level) {
                unsigned char* destination = source + surface->bitsPerPixel * (width * height) / 8;
                fn_00205EE8(source, destination, width, height, format);
                source = destination;
                width >>= 1;
                height >>= 1;
            }
        }
    }
    owner->storageMode = storageMode;
}
#endif
