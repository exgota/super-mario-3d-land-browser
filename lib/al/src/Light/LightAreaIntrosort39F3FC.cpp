#include <algorithm>
#include <math/seadVector.h>

// Descriptive private types recovered from the original lighting readers and
// copy operations. Their spelling is not an original-symbol claim.
namespace dot39f3fc {
struct Color {
    float r, g, b, a;
    Color(float, float, float, float);
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
        direction.set(other.direction);
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
typedef bool (*Compare)(const Area&, const Area&);
static_assert_(sizeof(Light) == 0x60);
static_assert_(sizeof(Area) == 0x128);
}

#ifdef NON_MATCHING
// Retail's median partition, halved depth budget, and heap fallback identify
// this standard-library specialization independently of its unknown spelling.
template void std::__introsort_loop<dot39f3fc::Area*, int, dot39f3fc::Compare>(
    dot39f3fc::Area*, dot39f3fc::Area*, int, dot39f3fc::Compare);
#endif
