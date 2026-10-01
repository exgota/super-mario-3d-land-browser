#pragma once

// NonMatching clean-room contracts recovered from the EU executable.
// Descriptive types only; no original class ownership is asserted.
namespace dot269a60 {
struct Vec {
    float x, y, z;
    Vec() {}
    Vec(float a, float b, float c) : x(a), y(b), z(c) {}
    Vec& operator=(const Vec& v) { x=v.x; y=v.y; z=v.z; return *this; }
};
struct Surface {
    void* owner;
    void* key;
    Vec normal;
    Vec sides[3];
    Vec vertices[3];
};
struct Contact {
    Surface surface;
    float distance;
    Vec position;
    unsigned char opaque6c[0x18];
    unsigned char triangle;
};
struct Hit {
    float fraction;
    Vec position;
    Surface surface;
};
struct Sweep {
    Vec start, displacement;
    float startRadius, endRadius, increment, current, previous;
    Sweep() : start(0,0,0), displacement(0,0,0), startRadius(0),
              endRadius(0), increment(0), current(0), previous(0) {}
};
struct Ring {
    void** slots;
    int capacity, head, count;
};
struct Filter {
    const void* vtable;
    Ring ring;
    void* storage[256];
    void* delegatedFilter;
};
struct Director {
    void* vtable;
    void* queryOwner;
    void* queryFilter;
    Filter* exclusionFilter;
    void* list10;
    void* list14;
};
typedef bool (*Less)(const Hit*, const Hit*);
}

extern "C" int fn_00269A60(dot269a60::Hit*, int, const dot269a60::Vec*,
                          const dot269a60::Vec*, float, void*, void*);
