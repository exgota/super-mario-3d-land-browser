#include <observed/TextureParameterState.h>
using namespace observed_texture;

extern "C" void fn_0039053C(unsigned target, int parameter, const int* values) {
    StatePrefix* state = dat_003E3154;
    ManagerPrefix* manager = dat_003E3180.current;
    switch (target) {
    case 0x0DE1: {
        Texture2D* texture = state->names2D[state->active]
            ? manager->bound2D[state->active]->payload : manager->default2D;
        if (!texture->setIntegerParameter(parameter, values)) return;
        break;
    }
    case 0x8513: {
        TextureCube* texture = state->namesCube[state->active]
            ? manager->boundCube[state->active]->payload : manager->defaultCube;
        if (!texture->setIntegerParameter(parameter, values)) return;
        break;
    }
    default: return;
    }
    u32 bit = state->active + 10;
    state->dirtyWords[bit >> 5] |= 1u << (bit & 31);
}
