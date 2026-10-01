// NonMatching: clean-room semantic reconstruction, no exact-byte credit.
#include <retail/shv_ValidatorFront.h>
#include <retail/shv_PartialValidatorTail.h>

#ifdef NON_MATCHING
extern "C" void __shv_partialValidateShaderValidator(ShvReconstruction::Byte* flags,
                                                    ShvReconstruction::Word category) {
    using namespace ShvReconstruction;
    Byte* context = reinterpret_cast<Byte*>(dat_003E3154);
    validateScissor(flags, category, context);
    Byte* validator = reinterpret_cast<Byte*>(dat_003E2E40.current);
    Byte* shader = p(validator, 0);
    if (!shader)
        return;
    validateShaderFront(flags, category, context, shader, validator);
    validatePartialShaderTail(flags, category, context, shader, validator);
}

#endif // NON_MATCHING
