#include <cfl/cfl_TextureRoot.h>

#ifdef NON_MATCHING
// NonMatching. Full root reconstructed from the EU executable, 0x001119EC.
// Imports keep established retail addresses/names. Helpers below express source
// operations, have no asserted retail addresses, and must be inlined for checking.
using namespace cfl_re;
extern "C" {
void* fn_002848F4(unsigned bytes, int alignment);
void fn_002847BC(void*);
int fn_00283D1C(void* image, int category, int index);
void fn_00283CC4(Texture*, const void* image, unsigned memoryKind);
void fn_00283A44(Texture**, int width, int height, int clear, int, int);
void fn_002841C0(float*, float left, float right, float bottom, float top, float nearPlane, float farPlane);
void fn_002873A0(const float*);
void fn_002870A4(const void*, int);
void fn_00283960(void* context, int, int);
void fn_00283700(const Quad*);
void fn_00284020(int, int, int, int, unsigned);
void fn_002840F4(int);
void fn_00283FAC(void* context, float left, float right, float bottom, float top, float depth, int, int);
void fn_00284894();
void fn_00284868();
void fn_002847E4();
void* fn_0028E280(unsigned region);
void* fn_0028E240(unsigned region);
void fn_00283594(int, unsigned*);
void fn_00282D20(unsigned, unsigned);
void fn_00282B84(unsigned, int, unsigned, int, int, int, int, int);
void fn_002874D0(unsigned, unsigned, void**);
void fn_00282650(int, unsigned*);
void fn_0011635C(void*, const void*, int, int, int, int);
void fn_00285238(Texture*, int width, int height, int levels, int, int, int format, const void* pixels, unsigned memoryKind, int);
void fn_002823B8(Texture*, void*, unsigned, int, int, int);
void fn_00282094(void*, int, int, const void*, const float*, const void*, const void*);
void fn_0028820C(Texture*);
void fn_00296048(void*, unsigned);
void nngxAdd3DCommand(const void*, unsigned, int);
void nngxSplitDrawCmdlist();
int glGetError();
typedef void (*FreeCallback)(unsigned, unsigned, void*, void*);
void nngxGetAllocator(void*, FreeCallback*);
void __rt_memclr(void*, unsigned);
void __rt_memcpy(void*, const void*, unsigned);
void __aeabi_memcpy4(void*, const void*, unsigned);
extern unsigned dat_003A7A74[9];
extern unsigned dat_003A7AB8[];
extern unsigned dat_003A7D38[];
extern unsigned dat_003A7E40[4];
extern unsigned dat_003A7E50[12];
extern unsigned dat_003A7E80[12];
extern unsigned char dat_003A8080[];
extern unsigned dat_003A81CC[24];
extern unsigned dat_003A8244[6];
extern unsigned dat_003A825C[10];
extern unsigned char dat_003A834C[];
extern unsigned char dat_003A838A[];
extern ExpressionAdjust dat_003A83A4[16];
extern unsigned dat_003EF06C;
extern unsigned char dat_003EF070[4];
extern unsigned char dat_003EF13C[32];
extern unsigned char dat_004244A8[]; // observed BSS; not yet present in map.csv
}

static inline void syncDraw()
{
    nngxSplitDrawCmdlist();
    glGetError();
    fn_00284894();
    fn_00284868();
    fn_002847E4();
    glGetError();
}

static inline void loadImage(void*& destination, int category, int index)
{
    if (!destination) {
        int size = fn_00283D1C(0, category, index);
        void* data = 0;
        if (size > 0) {
            data = fn_002848F4(size, 32);
            fn_00283D1C(data, category, index);
        }
        destination = data;
    }
}

static inline void releaseTexture(Texture& texture)
{
    if (texture.owner) {
        if (texture.ownsPixels) {
            unsigned kind = texture.memoryKind;
            void* owner = texture.owner;
            void* pixels = texture.pixels;
            FreeCallback freeCallback;
            nngxGetAllocator(0, &freeCallback);
            freeCallback(kind, 0x101, owner, pixels);
        }
        texture.height = 0;
        texture.owner = 0;
        texture.width = 0;
        texture.ownsPixels = 0;
    }
}

static inline void commandPair(unsigned value, unsigned header)
{
    unsigned command[2];
    command[0] = value;
    command[1] = header;
    nngxAdd3DCommand(command, 8, 1);
}

static inline void resetTextureCommands()
{
    unsigned command[4];
    command[0] = dat_003A7E40[0];
    command[1] = dat_003A7E40[1];
    command[2] = dat_003A7E40[2];
    command[3] = dat_003A7E40[3];
    nngxAdd3DCommand(command, 16, 1);
}

static inline bool isGpuMemory(void* pointer)
{
    unsigned address = reinterpret_cast<unsigned>(pointer);
    if (reinterpret_cast<unsigned>(fn_0028E280(0x20000)) <= address &&
        reinterpret_cast<unsigned>(fn_0028E240(0x20000)) >= address)
        return true;
    return reinterpret_cast<unsigned>(fn_0028E280(0x30000)) <= address &&
           reinterpret_cast<unsigned>(fn_0028E240(0x30000)) >= address;
}

static inline void readPixels(void* output, Texture* texture, int size, int bytes)
{
    void* pixels = texture->pixels;
    if (isGpuMemory(pixels)) {
        unsigned format = dat_003A7AB8[0x50 / 4] == 0x6754 ? 0x1907 : 0x1908;
        unsigned temporary;
        fn_00283594(1, &temporary);
        fn_00282D20(0xde1, temporary);
        fn_00282B84(0x10de1, 0, format, 0, 0, size, size, 0);
        glGetError();
        syncDraw();
        void* mapped;
        fn_002874D0(0xde1, 0x6790, &mapped);
        __rt_memcpy(output, mapped, bytes);
        fn_00282D20(0xde1, 0);
        fn_00282650(1, &temporary);
    } else {
        __rt_memcpy(output, pixels, bytes);
    }
    syncDraw();
}

static inline float minimumSpecialHeight(float height)
{
    // Retail compares signed IEEE-754 bit patterns, rather than VCMP.
    union { float f; int i; } bits;
    bits.f = height;
    return bits.i < 0x41400000 ? 12.0f : height;
}

static inline void setQuad(Quad& q, float x, float y, float width, float height, float angle, int mirror)
{
    q.x = x; q.y = y; q.width = width; q.height = height; q.angle = angle; q.mirror = mirror;
}

extern "C" void fn_001119EC(unsigned char* occupancy, const CharInfo* original, Model* model, int resolution)
{
    void* images[13] = {0};
    void** imagePairs[16][2] = {
        {&images[12], &images[6]}, {&images[11], &images[6]},
        {&images[12], &images[5]}, {&images[10], &images[4]},
        {&images[9], &images[6]}, {&images[8], &images[6]},
        {&images[12], &images[3]}, {&images[11], &images[3]},
        {&images[12], &images[3]}, {&images[10], &images[3]},
        {&images[9], &images[3]}, {&images[8], &images[3]},
        {&images[7], &images[6]}, {&images[7], &images[6]},
        {&images[7], &images[3]}, {&images[7], &images[3]}
    };
    Resource* resource = model->resource;
    int size;
    switch (resolution) {
    case 0x80: case 0xe0: size = 0x80; break;
    case 0x100: case 0x1e0: size = 0x100; break;
    case 0x200: size = 0x200; break;
    case 0x400: size = 0x400; break;
    default: size = 0x40; break;
    }
    int levels = resolution == 0x60 ? 2 : resolution == 0xe0 ? 3 : resolution == 0x1e0 ? 4 : 1;
    bool mipmapped = levels >= 2;
    bool overrideFirst = (resource->flags >> 30) & 1;
    bool overrideSecond = resource->flags >> 31;
    bool makeOccupancy = (1u & ~(resource->flags >> 28)) != 0;
    bool needReadback = makeOccupancy || mipmapped;
    unsigned bytes = 0;
    if (resolution & 0x20) bytes += 0x800;
    if (resolution & 0x40) bytes += 0x2000;
    if (resolution & 0x80) bytes += 0x8000;
    if (resolution & 0x100) bytes += 0x20000;
    if (resolution & 0x200) bytes += 0x80000;
    if (resolution & 0x400) bytes += 0x200000;
    unsigned char* staging = static_cast<unsigned char*>(fn_002848F4(bytes, 16));
    __rt_memclr(occupancy, 256);
    fn_002870A4(dat_003A8080, 1);

    for (int expression = 0; expression < 16; ++expression) {
        Texture* target = model->expression[expression];
        if (!target) continue;
        CharInfo info;
        __aeabi_memcpy4(&info, original, sizeof(info));
        const ExpressionAdjust& adjustment = dat_003A83A4[expression];
        int rotation = adjustment.firstRotation;
        info.word[0x54/4] += adjustment.secondY;
        if (adjustment.secondType >= 0) info.word[0x64/4] = adjustment.secondType;
        if (adjustment.firstType >= 0 && adjustment.firstType != info.word[0x20/4]) {
            rotation += 32 - dat_003A834C[info.word[0x20/4]];
            info.word[0x20/4] = adjustment.firstType;
            rotation -= 32 - dat_003A834C[adjustment.firstType];
        }
        if (rotation) {
            int value = rotation + info.word[0x30/4];
            if (value < 0) value = 0;
            else if (value > 7) value = 7;
            info.word[0x30/4] = value;
        }
        if (adjustment.secondRotation) {
            int value = info.word[0x4c/4] + adjustment.secondRotation;
            if (value < 0) value = 0;
            else if (value > 11) value = 11;
            info.word[0x4c/4] = value;
        }
        loadImage(*imagePairs[expression][0], 10, info.word[0x20/4]);
        if (expression == 12 || expression == 13 || expression == 14 || expression == 15)
            loadImage(images[12], 10, original->word[0x20/4]);
        loadImage(*imagePairs[expression][1], 17, info.word[0x64/4]);
        loadImage(images[2], 11, info.word[0x3c/4]);
        loadImage(images[1], 18, info.word[0x78/4]);
        loadImage(images[0], 16, info.word[0x9c/4]);
        int leftType = info.word[0x20/4];
        int rightType = leftType;
        void* leftImage = *imagePairs[expression][0];
        void* rightImage = leftImage;
        if (expression == 12 || expression == 14) {
            leftType = original->word[0x20/4];
            leftImage = images[12];
        } else if (expression == 13 || expression == 15) {
            rightType = original->word[0x20/4];
            rightImage = images[12];
        }
        Quad quads[8];
        fn_00283CC4(&quads[0].texture, leftImage, 0x10000);
        fn_00283CC4(&quads[1].texture, rightImage, 0x10000);
        fn_00283CC4(&quads[2].texture, images[2], 0x10000);
        fn_00283CC4(&quads[3].texture, images[2], 0x10000);
        fn_00283CC4(&quads[4].texture, *imagePairs[expression][1], 0x10000);
        fn_00283CC4(&quads[5].texture, images[1], 0x10000);
        fn_00283CC4(&quads[6].texture, images[1], 0x10000);
        fn_00283CC4(&quads[7].texture, images[0], 0x10000);
        Texture* targetBinding = target;
        fn_00283A44(&targetBinding, size, size, 1, 0, 0);
        if (target->memoryKind == 0x10000 && static_cast<unsigned>(size) >= 0x400) {
            __rt_memclr(target->pixels, size * size * 2);
            fn_00296048(target->pixels, size * size * 2);
        }
        float scale = static_cast<float>(size) * 0.015625f;
        float firstX = static_cast<float>(info.word[0x34/4]) * 0.8896146416664124f;
        float firstY = 18.451522827148438f + static_cast<float>(info.word[0x38/4]) * 1.0760942697525024f;
        float firstScale = 1.0f + static_cast<float>(info.word[0x28/4]) * 0.4f;
        float firstWidth = firstScale * 5.34375f * scale;
        float firstHeight = (firstScale * 4.5f) * (0.64f + static_cast<float>(info.word[0x2c/4]) * 0.12f) * scale;
        float firstAngle = static_cast<float>((32 - dat_003A834C[info.word[0x20/4]] + info.word[0x30/4]) % 32) * 11.25f;
        float secondX = static_cast<float>(info.word[0x50/4]) * 0.8896146416664124f;
        float secondY = 16.549806594848633f + static_cast<float>(info.word[0x54/4]) * 1.0760942697525024f;
        float secondScale = 1.0f + static_cast<float>(info.word[0x44/4]) * 0.4f;
        float secondWidth = secondScale * 5.0625f * scale;
        float secondHeight = (secondScale * 4.5f) * (0.64f + static_cast<float>(info.word[0x48/4]) * 0.12f) * scale;
        float secondAngle = static_cast<float>((32 - dat_003A838A[info.word[0x3c/4]] + info.word[0x4c/4]) % 32) * 11.25f;
        float thirdY = 29.25885009765625f + static_cast<float>(info.word[0x74/4]) * 1.0760942697525024f;
        float thirdScale = 1.0f + static_cast<float>(info.word[0x6c/4]) * 0.4f;
        float thirdWidth = thirdScale * 6.1875f * scale;
        float thirdHeight = (thirdScale * 4.5f) * (0.64f + static_cast<float>(info.word[0x70/4]) * 0.12f) * scale;
        float fourthY = 31.763553619384766f + static_cast<float>(info.word[0x88/4]) * 1.0760942697525024f;
        float fourthScale = 1.0f + static_cast<float>(info.word[0x84/4]) * 0.4f;
        float fourthWidth = fourthScale * 4.5f * scale;
        float fourthHeight = fourthScale * 9.0f * scale;
        float fifthX = 17.766164779663086f + static_cast<float>(info.word[0xa4/4]) * 1.7792292833328247f;
        float fifthY = 17.959861755371094f + static_cast<float>(info.word[0xa8/4]) * 1.0760942697525024f;
        float fifthScale = 1.0f + static_cast<float>(info.word[0xa0/4]) * 0.4f;
        setQuad(quads[0], (32.0f-firstX)*scale, firstY*scale, firstWidth,
                leftType == 14 || leftType == 26 ? minimumSpecialHeight(firstHeight) : firstHeight, firstAngle, 2);
        setQuad(quads[1], (firstX+32.0f)*scale, firstY*scale, firstWidth,
                rightType == 14 || rightType == 26 ? minimumSpecialHeight(firstHeight) : firstHeight, 360.0f-firstAngle, 1);
        setQuad(quads[2], (32.0f-secondX)*scale, secondY*scale, secondWidth, secondHeight, secondAngle, 2);
        setQuad(quads[3], (secondX+32.0f)*scale, secondY*scale, secondWidth, secondHeight, 360.0f-secondAngle, 1);
        int thirdType = info.word[0x64/4];
        if (thirdType == 3 || thirdType == 15 || thirdType == 19 || thirdType == 20 || thirdType == 21 || thirdType == 23 || thirdType == 25)
            thirdHeight = minimumSpecialHeight(thirdHeight);
        setQuad(quads[4], scale*32.0f, thirdY*scale, thirdWidth, thirdHeight, 0.0f, 0);
        setQuad(quads[5], scale*32.0f, fourthY*scale, fourthWidth, fourthHeight, 0.0f, 2);
        setQuad(quads[6], scale*32.0f, fourthY*scale, fourthWidth, fourthHeight, 0.0f, 1);
        setQuad(quads[7], fifthX*scale, fifthY*scale, fifthScale*scale, fifthScale*scale, 0.0f, 0);

        float projection[16];
        fn_002841C0(projection, 0.0f, static_cast<float>(size), static_cast<float>(size), 0.0f, -200.0f, 200.0f);
        fn_002873A0(projection);
        void* context = dat_004244A8;
        commandPair(0, dat_003A7A74[2]);
        commandPair(dat_003A7A74[3], dat_003A7A74[4]);
        fn_00284020(1, 0, 0, 1, 0x89890000);
        commandPair(overrideSecond ? dat_003EF06C : dat_003A7D38[info.word[0x80/4]], dat_003A7A74[8]);
        fn_00283960(context, 3, 1);
        fn_00283700(&quads[5]);
        fn_00283700(&quads[6]);
        unsigned commands[18];
        __aeabi_memcpy4(commands, dat_003A81CC + 6, 0x48);
        unsigned mode = *reinterpret_cast<unsigned*>(dat_004244A8 + 0x50);
        commands[0] = 0x0ee00ee0 | (mode << 16) | mode;
        commands[4] = dat_003A825C[info.word[0x68/4]];
        commands[6] = mode | 0x0fff0fe0;
        commands[10] = dat_003A825C[5 + info.word[0x68/4]];
        commands[12] = mode | 0x0fff0fe0;
        commands[16] = ~0u;
        nngxAdd3DCommand(commands, 0x48, 1);
        fn_00283700(&quads[4]);
        commandPair(overrideFirst ? dat_003EF06C : dat_003A7D38[info.word[0x40/4]], dat_003A7A74[8]);
        fn_00283960(context, 3, 1);
        fn_00283700(&quads[2]);
        fn_00283700(&quads[3]);
        unsigned firstColor = info.word[0x20/4] == 9 ? 0xff0082ff : info.word[0x20/4] == 20 ? 0xffffff00 : 0xff000000;
        __aeabi_memcpy4(commands, dat_003A81CC + 6, 0x48);
        mode = *reinterpret_cast<unsigned*>(dat_004244A8 + 0x50);
        commands[0] = 0x0ee00ee0 | (mode << 16) | mode;
        commands[4] = firstColor;
        commands[6] = mode | 0x0fff0fe0;
        commands[10] = ~0u;
        commands[12] = mode | 0x0fff0fe0;
        commands[16] = dat_003A8244[info.word[0x24/4]];
        nngxAdd3DCommand(commands, 0x48, 1);
        fn_00283700(&quads[0]);
        fn_00283700(&quads[1]);
        commandPair(0xff0f0f12, dat_003A7A74[8]);
        fn_00283960(context, 3, 1);
        fn_00283700(&quads[7]);
        commandPair(dat_003A7A74[5], dat_003A7A74[6]);
        fn_00284020(0, 0, 0, 0, 0x01010000);
        fn_00283960(context, 0, 1);
        commandPair(0x00ffffff, dat_003A7A74[8]);
        fn_002840F4(1);
        fn_00283FAC(context, 0.0f, static_cast<float>(size), 0.0f, static_cast<float>(size), 0.0f, 0, 0);
        commandPair(dat_003A7A74[3], dat_003A7A74[4]);
        fn_00284020(0, 0, 0, 1, 0x19190000);
        fn_00283960(context, 1, 0);
        fn_00283700(&quads[5]); fn_00283700(&quads[6]); fn_00283700(&quads[4]);
        fn_00283700(&quads[2]); fn_00283700(&quads[3]); fn_00283700(&quads[0]);
        fn_00283700(&quads[1]); fn_00283700(&quads[7]);
        fn_00284020(1, 0, 0, 1, 0x01010000);
        if (needReadback) {
            syncDraw();
            readPixels(staging, targetBinding, size, size*size*2);
        }
        if (makeOccupancy) {
            int cell = size / 16;
            if (size >= 128) {
                int rowStart = 0;
                for (int row = 0; row < 16; ++row) {
                    int x = 0;
                    for (int col = 0; col < 16; ++col, x += cell) {
                        unsigned char& occupied = occupancy[row*16 + col];
                        for (int y = rowStart; y < rowStart + cell && !occupied; y += 8) {
                            int tile = ((size-y-8)/8) * (size/8) + x/8;
                            const unsigned char* pixels = staging + tile*128;
                            for (int blockX = x; blockX < x+cell && !occupied; blockX += 8) {
                                for (int sample = 0; sample < 64; ++sample, pixels += 2) {
                                    if (*pixels & 15) { occupied = 1; break; }
                                }
                            }
                        }
                    }
                    rowStart += cell;
                }
            } else {
                unsigned char* linear = static_cast<unsigned char*>(fn_002848F4(size*size*2, 16));
                fn_0011635C(linear, staging, size, size, 1, 10);
                int rowStart = 0;
                for (int row = 0; row < 16; ++row) {
                    int x = 0;
                    for (int col = 0; col < 16; ++col, x += cell) {
                        unsigned char& occupied = occupancy[row*16 + col];
                        for (int y = rowStart; y < rowStart+cell && !occupied; ++y) {
                            const unsigned char* pixels = linear + 2*(y*size+x);
                            for (int n = 0; n < cell; ++n, pixels += 2) {
                                if (*pixels & 15) { occupied = 1; break; }
                            }
                        }
                    }
                    rowStart += cell;
                }
                fn_002847BC(linear);
            }
        }
        syncDraw();
        glGetError();
        resetTextureCommands();
        releaseTexture(quads[0].texture); releaseTexture(quads[1].texture);
        releaseTexture(quads[2].texture); releaseTexture(quads[3].texture);
        releaseTexture(quads[4].texture); releaseTexture(quads[5].texture);
        releaseTexture(quads[6].texture); releaseTexture(quads[7].texture);
        if (mipmapped) {
            unsigned memoryKind = resource->textureMemoryKind;
            nngxAdd3DCommand(dat_003A7E50, 0x30, 1);
            nngxAdd3DCommand(dat_003A7E80, 0x30, 1);
            fn_00283960(context, 1, 1);
            commandPair(dat_003A7A74[5], dat_003A7A74[6]);
            commandPair(0, dat_003A7A74[2]);
            Texture mipTextures[3];
            Texture* previous = target;
            int width = target->width;
            int savedWidth = width;
            int savedHeight = target->height;
            unsigned char* levelPixels = staging;
            for (int level = 0; level < levels; ++level) {
                if (level >= 1) {
                    Texture* next = &mipTextures[level-1];
                    fn_00285238(next, width, width, 1, 0, 0, 10, 0, 0x10000, 0);
                    Texture* nextBinding = next;
                    fn_00283A44(&nextBinding, width, width, 0, 0, 0);
                    fn_00284020(1, 0, 0, 0, 0x01010000);
                    fn_002823B8(previous, context, 0x2701, 0, 0, 0);
                    float vertices[12] = {0,0,0, static_cast<float>(width),0,0,
                        static_cast<float>(width),static_cast<float>(width),0, 0,static_cast<float>(width),0};
                    fn_00282094(context, 5, 4, dat_003EF070, vertices, 0, dat_003EF13C);
                    syncDraw();
                    readPixels(levelPixels, nextBinding, width, width*width*2);
                    syncDraw();
                    glGetError();
                    resetTextureCommands();
                    releaseTexture(*previous);
                    previous = next;
                }
                levelPixels += width*width*2;
                width >>= 1;
            }
            resetTextureCommands();
            fn_0028820C(previous);
            fn_00285238(target, savedWidth, savedHeight, levels, 0, 0, 10, staging, memoryKind, 0);
            if (memoryKind != 0x10000) syncDraw();
        } else if (resource->textureMemoryKind != 0x10000) {
            Texture converted;
            fn_00285238(&converted, target->width, target->height, levels, 0, 0, 10, target->pixels, resource->textureMemoryKind, 0);
            if (resource->textureMemoryKind != 0x10000) syncDraw();
            releaseTexture(*target);
            *target = converted;
        }
    }
    fn_002847BC(images[12]); fn_002847BC(images[11]); fn_002847BC(images[10]);
    fn_002847BC(images[9]); fn_002847BC(images[8]); fn_002847BC(images[7]);
    fn_002847BC(images[6]); fn_002847BC(images[5]); fn_002847BC(images[4]);
    fn_002847BC(images[3]); fn_002847BC(images[2]); fn_002847BC(images[1]);
    fn_002847BC(images[0]); fn_002847BC(staging);
}
#endif
