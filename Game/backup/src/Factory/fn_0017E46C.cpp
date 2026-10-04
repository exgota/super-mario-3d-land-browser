namespace {
struct NameSource;
}

extern "C" const char* fn_00262580(const NameSource*, int);

namespace al {
extern bool isEqualSubString(const char*, const char*);
}

extern "C" bool fn_0017E46C(const NameSource* source) {
    return al::isEqualSubString(fn_00262580(source, 0), "Land");
}
