#include "retail/shv_ValidatorFront.h"
#include "shv_ValidatorTail.h"

// NonMatching: complete retail-derived semantic proposal for 0037B8D0..0037F0E0.
// Shared helpers expose the observed layout without importing SDK implementation.
#ifdef NON_MATCHING
extern "C" void __shv_validateShaderValidator(ShvReconstruction::Byte* flags) {
    using namespace ShvReconstruction;
    Byte* context = reinterpret_cast<Byte*>(dat_003E3154);
    const Word allowed = ~w(context, 8);
    validateScissor(flags, allowed, context);
    Byte* validator = reinterpret_cast<Byte*>(dat_003E2E40.current);
    Byte* shader = p(validator, 0);
    if (!shader) return;
    validateShaderFront(flags, allowed, context, shader, validator, true);
    validateShaderTail(flags, context, shader, validator);
}
#endif
