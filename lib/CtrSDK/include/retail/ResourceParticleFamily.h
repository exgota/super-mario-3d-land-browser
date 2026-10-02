#ifndef RETAIL_RESOURCE_PARTICLE_FAMILY_H
#define RETAIL_RESOURCE_PARTICLE_FAMILY_H

// Private boundary shared by the resource factory and particle-binding factory.
// Retail 002995CC passes owner/resource/object allocator/stream allocator in
// r0..r3 and ParticleShape in stack[0]; the result is returned in r0.
// Opaque pointers preserve the independently recovered, TU-private layouts.
extern "C" void* fn_002A451C(void*, const void*, void*, void*, void*);

#endif
