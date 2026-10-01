#ifndef SHV_VALIDATOR_ACCESS_H
#define SHV_VALIDATOR_ACCESS_H

// Retail-derived byte layouts used by the two shader validators. Field names
// remain deliberately local until independently checked against other callers.
namespace ShvReconstruction {
typedef unsigned int Word;
typedef unsigned char Byte;
inline Word& w(Byte* base, Word offset) { return *reinterpret_cast<Word*>(base + offset); }
inline Byte& b(Byte* base, Word offset) { return base[offset]; }
inline float& f(Byte* base, Word offset) { return *reinterpret_cast<float*>(base + offset); }
inline Byte*& p(Byte* base, Word offset) { return *reinterpret_cast<Byte**>(base + offset); }
inline Word bits(float value) { union { float f; Word u; } v; v.f = value; return v.u; }
inline float floatBits(Word value) { union { float f; Word u; } v; v.u = value; return v.f; }
}
extern "C" {
extern ShvReconstruction::Word* dat_003E2E30;
extern ShvReconstruction::Word* dat_003E2E34;
extern ShvReconstruction::Word dat_003E2E40[4];
extern ShvReconstruction::Byte* dat_003E3154;
extern const ShvReconstruction::Word dat_003E2E50[24];
extern const ShvReconstruction::Word dat_003E2EB0[6];
extern const ShvReconstruction::Word dat_003E2EC8[6];
extern const ShvReconstruction::Word dat_003E2EE0[157];
extern const ShvReconstruction::Word dat_003A482C[6];
extern const ShvReconstruction::Word dat_003A4844[6];
extern const ShvReconstruction::Word dat_003A485C[6];
extern const ShvReconstruction::Word dat_003A4874[6];
extern const ShvReconstruction::Word dat_003A2F6C[];
extern void* (*dat_003E2654)(ShvReconstruction::Word, ShvReconstruction::Word,
                            ShvReconstruction::Word, ShvReconstruction::Word);
void __cb_writeRegs(ShvReconstruction::Word, ShvReconstruction::Word,
                    const ShvReconstruction::Word*);
void __cb_multiWriteReg(ShvReconstruction::Word, ShvReconstruction::Word,
                        const ShvReconstruction::Word*);
void __cb_fillRegs(ShvReconstruction::Word, ShvReconstruction::Word,
                   ShvReconstruction::Word);
void __cb_addDummyWrite(ShvReconstruction::Word, ShvReconstruction::Word);
ShvReconstruction::Byte* __tx_getBoundTextureLut(ShvReconstruction::Word);
}
namespace ShvReconstruction {
inline void emit(Word value, Word header) {
    Word* cursor = dat_003E2E30;
    if (cursor < dat_003E2E34) {
        cursor[0] = value;
        cursor[1] = header;
        dat_003E2E30 = cursor + 2;
    }
}
}
#endif
