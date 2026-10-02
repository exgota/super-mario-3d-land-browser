namespace sead {
template <typename T> class SafeStringBase;
class Event;
}

namespace {
struct SystemKit {
    void* field_0;
    void* field_4;
    void* resource;
};
}

extern "C" SystemKit* _ZN18alProjectInterface12getSystemKitEv();
extern "C" void fn_00101E80(void*, const sead::SafeStringBase<char>&);
extern "C" void fn_001CADD4(void*, const void*);
extern "C" void fn_001CAFE4(void*, const void*);
extern "C" void fn_001CB204(void*, const void*);
extern "C" void fn_001CB28C(void*, const void*);

namespace al {
void createCategoryResourceAll(const sead::SafeStringBase<char>& name, sead::Event*) {
    return fn_00101E80(_ZN18alProjectInterface12getSystemKitEv()->resource, name);
}
}

extern "C" void fn_001CADB8(const void* name) {
    return fn_001CADD4(_ZN18alProjectInterface12getSystemKitEv()->resource, name);
}

extern "C" void fn_001CAFC8(const void* name) {
    return fn_001CAFE4(_ZN18alProjectInterface12getSystemKitEv()->resource, name);
}

extern "C" void fn_001CB1E8(const void* name) {
    return fn_001CB204(_ZN18alProjectInterface12getSystemKitEv()->resource, name);
}

extern "C" void fn_001CB270(const void* name) {
    return fn_001CB28C(_ZN18alProjectInterface12getSystemKitEv()->resource, name);
}
