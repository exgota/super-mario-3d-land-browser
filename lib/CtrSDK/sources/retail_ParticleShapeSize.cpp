// Guarded NonMatching: complete source forms remain byte-different.
#ifdef NON_MATCHING
// ParticleShape storage sizing, reconstructed from retail22F554..22F5B0.
#include <retail/ParticleShape.h>
namespace nw { namespace gfx {
int ParticleShape::AddVertexParamSize(unsigned type, int width, int used) {
    int elementSize;
    switch (type) {
    case 0x1406: elementSize = 4; break;
    default: elementSize = 1; break;
    }
    return ((used + 31) & ~31) + width * elementSize;
}
int ParticleShape::AddVertexStreamSize(unsigned type, int width, int count, int used) {
    int elementSize;
    switch (type) {
    case 0x1406: elementSize = 4; break;
    default: elementSize = 1; break;
    }
    int size = (count + 8) * width * elementSize;
    used = ((used + 31) & ~31) + size;
    used = ((used + 31) & ~31) + size;
    return used + size;
}
} }
#endif
