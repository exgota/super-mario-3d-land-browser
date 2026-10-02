#ifndef RETAIL_SHV_INITIALIZE_VALIDATOR_H
#define RETAIL_SHV_INITIALIZE_VALIDATOR_H
#include <retail/GraphicsGlobals.h>

namespace retail_program_link { struct ReleaseSlot; }
extern "C" {
extern unsigned* dat_003E2E30;
extern unsigned* dat_003E2E34;
extern void* (*dat_003E2654)(unsigned, unsigned, unsigned, unsigned);
extern retail_program_link::ReleaseSlot dat_003E2658;
extern unsigned dat_003E3158;
extern unsigned dat_003E315C;
extern unsigned dat_003E3160;
extern unsigned dat_003E3164;
extern unsigned dat_003E3168;
extern unsigned dat_003E316C;
extern unsigned dat_003E3170;
extern unsigned dat_003E3174;
extern unsigned dat_003E3178;

// Producer 001064E4 fills all 189 entries from 003A421C. The current map has
// no identity for this BSS range, so this declaration does not close linking.
// Keep the missing identity visible; do not substitute an absolute address.
extern unsigned dat_00420F4C[189];
void __cb_fillRegs(unsigned, unsigned, unsigned);
void __cb_multiWriteReg(unsigned, unsigned, const unsigned*);
void fn_0028D1F0(void*, unsigned);
void __shv_initializeShaderValidator(unsigned* flags);
}
#endif
