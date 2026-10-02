#ifndef RETAIL_PARTICLE_SHAPE_H
#define RETAIL_PARTICLE_SHAPE_H

// Layout and declarations verified against retail22F35C..22F5B0 and2A451C.
namespace nw { namespace gfx {
struct ParticleVertexAttribute {
    int index;
    unsigned type;
    unsigned char width, stream;
    unsigned char padding[2];
    unsigned char* buffers[2];
    unsigned unknown[2];
};
struct ParticleShape {
    unsigned char unknown00[0x30];
    unsigned char swap;
    unsigned char unknown31[0x1c7];
    unsigned char* buffers[2];
    ParticleVertexAttribute* AddVertexParam(int, unsigned, int, float*, unsigned char**);
    ParticleVertexAttribute* AddVertexStream(int, unsigned, int, int, unsigned char**);
    static int AddVertexParamSize(unsigned, int, int);
    static int AddVertexStreamSize(unsigned, int, int, int);
};
} }

#endif
