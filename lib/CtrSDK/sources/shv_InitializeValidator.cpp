#include <retail/shv_InitializeValidator.h>

// Clean-room reconstruction of EU 00108690..00108F74. The original routine
// emits initialization commands; _shm_initializeShaderManager owns the lookup.
#ifdef NON_MATCHING
namespace {
inline void emit(unsigned value, unsigned header) {
    unsigned* cursor = dat_003E2E30;
    if (cursor < dat_003E2E34) {
        cursor[0] = value;
        cursor[1] = header;
        dat_003E2E30 = cursor + 2;
    }

}
// The original loads cached globals only inside the available-space branch.
inline void emitStored(const unsigned& value, unsigned header) {
    unsigned* cursor = dat_003E2E30;
    if (cursor < dat_003E2E34) {
        cursor[0] = value;
        cursor[1] = header;
        dat_003E2E30 = cursor + 2;
    }
}
}

extern "C" void __shv_initializeShaderValidator(unsigned* flags) {
    *flags = 0x03fdc3fc;
    emit(1, 0x00010245);
    emit(0, 0x00010244);
    emit(0x80000000, 0x00080289);
    emit(0, 0x000b0229);
    emit(0, 0x000f0252);
    emit(0, 0x000f0251);
    emit(0, 0x000f0254);
    emit(0, 0x00010253);
    emit(0, 0x000f0242);
    emit(0, 0x000f024a);
    emit(0, 0x0005025e);
    __cb_fillRegs(0x101, 7, 0);
    emit(0x10140, 0x000f011f);

    dat_003E3158 = 0x01010000;
    dat_003E315C = 0x1f40;
    dat_003E3160 = 0xff00ff10;
    dat_003E3170 = 3;
    dat_003E3174 = 0x00ffffff;
    dat_003E3164 = 0;
    dat_003E3168 = 3;
    dat_003E316C = 0;
    dat_003E3178 = 0x03000000;
    emit(0x00e40100, 0x000f0100);
    emitStored(dat_003E3158, 0x000f0101);
    emitStored(dat_003E315C, 0x000f0107);
    emitStored(dat_003E3160, 0x000f0105);
    emitStored(dat_003E3170, 0x00010061);
    emit(0, 0x00010062);
    emit(0, 0x000f0065);
    emit(0, 0x000f0066);
    emit(0, 0x000f0067);
    emit(0, 0x00010118);
    emit(0, 0x000f011b);
    emitStored(dat_003E3174, 0x0007006a);
    emitStored(dat_003E3168, 0x000f0102);
    emitStored(dat_003E3178, 0x00080126);
    __cb_fillRegs(0x40, 16, 0);
    __cb_fillRegs(0x50, 7, 0x1f1f1f1f);
    emit(0x100, 0x000f0058);
    emit(1, 0x000f004c);
    emit(0, 0x000f006f);
    emit(0, 0x00020060);
    emit(0x20000, 0x000c0069);
    emit(15, 0x000f0113);
    emit(15, 0x000f0112);
    emit(3, 0x000f0114);
    emit(3, 0x000f0115);

    for (int table = 0; table < 7; ++table) {
        emit(static_cast<unsigned>(table) << 8, 0x000f01c5);
        for (int index = 0; index < 256; index += 8)
            __cb_fillRegs(0x1c8, 8, 0);
    }
    emit(0x80000000, 0x000f0290);
    for (int index = 0; index < 96; index += 2)
        __cb_fillRegs(0x291, 8, 0);

    unsigned* zeros = dat_003E2654 ? static_cast<unsigned*>(
        dat_003E2654(0x10000, 0x100, 0, 0x4000)) : 0;
    fn_0028D1F0(zeros, 0x4000);
    emit(0, 0x000f02cb);
    __cb_multiWriteReg(0x2cc, 512, zeros);
    emit(512, 0x000f029b);
    __cb_multiWriteReg(0x29c, 3584, zeros);
    emit(0, 0x000f02bf);
    typedef void (*Release)(unsigned, unsigned, unsigned, void*);
    Release release = *reinterpret_cast<Release*>(&dat_003E2658);
    if (release) release(0x10000, 0x100, 0, zeros);

    for (int index = 0; index < 4; ++index)
        emit(0, static_cast<unsigned>(0x2b1 + index) | 0x000f0000);
    __cb_fillRegs(0x28b, 2, ~0u);
    __cb_fillRegs(0x205, 36, 0);
    emit(1, 0x000f01d4);
    emit(0xfefcf8f0, 0x000f00ae);

    unsigned char* validator = reinterpret_cast<unsigned char*>(dat_003E2E40.current);
    for (int index = 0; index < 189; ++index) {
        if (validator[0x15f4 + index] && dat_003E2E30 < dat_003E2E34) {
            *dat_003E2E30++ = *reinterpret_cast<unsigned*>(validator + 0x100c + 4*index);
            *dat_003E2E30++ = dat_00420F4C[index] |
                (static_cast<unsigned>(validator[0x15f4 + index]) << 16);
        }
    }
}
#endif
