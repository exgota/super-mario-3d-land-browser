#pragma once

#include <stddef.h>
#include <heap/seadHeap.h>
#include <math/seadVector.h>
#include <prim/seadSafeString.h>
#include <Yaml/alByamlIter.h>
#include <Resource/alResource.h>

void* operator new[](size_t, sead::Heap*, int);

// Layout names are descriptive recovery labels, not recovered source symbols.
namespace dot1c6410 {
struct Color;
struct Path;
}
extern "C" void fn_00292D20(dot1c6410::Color*);
extern "C" const unsigned int dat_003D7ABC[];

namespace dot1c6410 {
// Retail installs this address point here and independently at 0x00243A9C
// and 0x0028CB50; slot +8 dispatches 0x0039E0E4. This raw prefix does not
// establish the complete table allocation or owning C++ string class.
struct Path {
    const unsigned int* table;
    char* data;
    int capacity;
    char storage[128];
    Path() : table(dat_003D7ABC), data(storage), capacity(128) {
        storage[127] = storage[0] = 0;
    }
    const char* cstr() const {
        reinterpret_cast<void (*)(const Path*)>(table[2])(this);
        return data;
    }
};
struct Color {
    float r, g, b, a;
    Color(float red, float green, float blue, float alpha)
        : r(red), g(green), b(blue), a(alpha) { fn_00292D20(this); }
};

struct Light {
    Color ambient, diffuse, specular0, specular1, constantColor5;
    sead::Vector3f direction;
    bool cameraFollow;
    Light(const Color&, const Color&, const Color&, const Color&, const Color&,
          const sead::Vector3f&, bool);
    Light(const Light&);
    Light& operator=(const Light& other) {
        ambient = other.ambient;
        diffuse = other.diffuse;
        specular0 = other.specular0;
        specular1 = other.specular1;
        constantColor5 = other.constantColor5;
        direction = other.direction;
        cameraFollow = other.cameraFollow;
        return *this;
    }
};
struct Area {
    const char* name;
    int interpolation;
    Light playerLight, objectLight, mapObjectLight;
    Area();
    ~Area() {}
    Area(const char*, const char*);
};
struct Map {
    const char* name;
    Light light;
    Map();
    ~Map() {}
    Map(const char*, const char*);
};
struct Entry {
    void* first;
    void* second;
    Entry();
};
template<class T> struct Ring {
    T* buffer;
    int capacity, head, count;
    Ring() : buffer(0), capacity(0), head(0), count(0) {}
    void allocate(int size) {
        if (size > 0) {
            T* result = new (static_cast<sead::Heap*>(0), 4) T[size];
            if (result) { buffer = result; capacity = size; head = 0; count = 0; }
        }
    }
    void append(const T& value) {
        if (capacity > count) {
            int index = head + count++;
            if (capacity <= index) index -= capacity;
            buffer[index] = value;
        }
    }
    T* front() {
        if (count <= 0) return buffer;
        int index = head;
        if (capacity <= index) index -= capacity;
        return buffer + index;
    }
    T* back() {
        if (count <= 0) return buffer;
        int index = head + count - 1;
        if (capacity <= index) index -= capacity;
        return buffer + index;
    }
};
struct EntryRing : Ring<Entry> {
    Entry entries[128];
    EntryRing() {
        Entry* memory = entries;
        if (memory) {
            count = 0;
            buffer = memory;
            capacity = 128;
            head = 0;
        }
    }
};
struct Allocator {
    const void* vtable;
    sead::Heap* heap;
    explicit Allocator(sead::Heap*);
};
struct CreateArgument {
    bool first, second;
    unsigned short untouched;
    void* resource;
    int zero, one;
    CreateArgument() : first(true), second(true), resource(0), zero(0), one(1) {}
};
struct CreatedObject {
    unsigned int field0, field4;
    void* resource;
};
struct Flags {
    unsigned char first, second, third, fourth;
    Flags() : first(255), second(0), third(0), fourth(0) {}
};
struct Director {
    Area* stageArea;                  // 000
    Map* stageMap;                    // 004
    Ring<Area> areas;                 // 008
    Ring<Map> maps;                   // 018
    void* unknown28[5];               // 028
    EntryRing entries;                // 03C, entry storage 04C..44B
    void* unknown44c;
    unsigned int untouched450[5];
    CreatedObject* firstObject;       // 464
    void* firstResource;              // 468
    CreatedObject* secondObject;      // 46C
    Flags flags;           // 470
    Light* fallback0;                 // 474
    Light* fallback1;                 // 478
    void* unknown47c;
    Director();
};
}

#ifdef __arm__
static_assert_(sizeof(dot1c6410::Light) == 0x60);
static_assert_(sizeof(dot1c6410::Area) == 0x128);
static_assert_(sizeof(dot1c6410::Map) == 0x64);
static_assert_(sizeof(dot1c6410::Director) == 0x480);
#endif
