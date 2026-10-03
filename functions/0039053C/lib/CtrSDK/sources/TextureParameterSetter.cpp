#include <observed/TextureParameterState.h>
using namespace observed_texture;
namespace {
float clampBorder(int value) {
    if (value<=0) return 0.0f;
    if (value>1) return 1.0f;
    return static_cast<float>(value);
}
}
extern "C" void fn_0039053C(unsigned target,int parameter,const int* values) {
    StatePrefix* state=dat_003E3154;
    ManagerPrefix* manager=dat_003E3180.current;
    Parameters* texture;
    switch (target) {
    case 0x0DE1:
        texture=state->names2D[state->active] ? manager->bound2D[state->active]->payload
                                             : manager->default2D;
        break;
    case 0x8513:
        texture=state->namesCube[state->active] ? manager->boundCube[state->active]->payload
                                               : manager->defaultCube;
        break;
    default: return;
    }
    switch (parameter) {
    case 0x2800: texture->magnification=values[0]; break;
    case 0x2801: texture->minification=values[0]; break;
    case 0x2802: texture->wrapS=values[0]; break;
    case 0x2803: texture->wrapT=values[0]; break;
    case 0x813A: texture->minimumLod=values[0]; break;
    case 0x8191: texture->rawParameter=static_cast<u8>(values[0]); break;
    case 0x8501: texture->lodBias=static_cast<float>(values[0]); break;
    case 0x1004:
        texture->border[0]=clampBorder(values[0]);
        texture->border[1]=clampBorder(values[1]);
        texture->border[2]=clampBorder(values[2]);
        texture->border[3]=clampBorder(values[3]);
        break;
    default: return;
    }
    u32 bit=state->active+10;
    state->dirtyWords[bit>>5]|=1u<<(bit&31);
}
