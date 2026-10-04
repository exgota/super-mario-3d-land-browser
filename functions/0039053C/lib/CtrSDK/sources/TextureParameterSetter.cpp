#include <observed/TextureParameterState.h>
using namespace observed_texture;

extern "C" void fn_0039053C(unsigned target, int parameter, const int* values) {
    StatePrefix* state = dat_003E3154;
    switch (target) {
    case 0x0DE1: {
        ManagerPrefix* manager = dat_003E3180.current;
        Texture2D** textureSlot = state->names2D[state->active]
            ? &manager->bound2D[state->active]->payload : &manager->default2D;
        Texture2D* texture = *textureSlot;
        if (!texture->setIntegerParameter(parameter, values)) return;
        break;
    }
    case 0x8513: {
        ManagerPrefix* manager = dat_003E3180.current;
        TextureCube* texture = !state->namesCube[state->active]
            ? manager->defaultCube : manager->boundCube[state->active]->payload;
        if (!texture->setIntegerParameter(parameter, values)) return;
        break;
    }
    default: return;
    }
    u32 word = (state->active + 10) >> 5;
    state->dirtyWords[word] |= 1u << ((state->active + 10) & 31);
}
