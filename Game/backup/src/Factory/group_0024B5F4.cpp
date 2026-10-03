namespace {
extern "C" void fn_0024B5FC(void *, void *);
extern "C" void fn_00276AB8(void *, void *);
}

namespace sead {
template <typename T> class Quat {};
}
namespace al {
class ByamlIter {};
void tryGetQuat(sead::Quat<float> *, const ByamlIter &);
}

extern "C" void fn_0024B5F4(void *a0, void **a1) {
    fn_0024B5FC(a0, *a1);
}

extern "C" void fn_00250D28(void *a0, void **a1) {
    al::tryGetQuat(static_cast<sead::Quat<float> *>(a0), **reinterpret_cast<al::ByamlIter **>(a1));
}

extern "C" void fn_00276AB0(void *a0, void **a1) {
    fn_00276AB8(a0, *a1);
}
