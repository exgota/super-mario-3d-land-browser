extern "C" bool _ZNK2al9ByamlIter7isValidEv(const void*);
extern "C" bool _ZNK2al9ByamlIter16tryGetFloatByKeyEPfPKc(const void*, float*, const char*);
extern "C" const char dat_003A2DFC[];
extern "C" const char dat_003A2E04[];
extern "C" const char dat_003A2E0C[];

namespace {
namespace sead {
struct Vector3f {
    float x, y, z;
    void set(const Vector3f& value) {
        x = value.x;
        y = value.y;
        z = value.z;
    }
};
}
namespace al {
struct ByamlIter {
    bool isValid() const {
        return _ZNK2al9ByamlIter7isValidEv(this);
    }
    bool tryGetFloatByKey(float* value, const char* key) const {
        return _ZNK2al9ByamlIter16tryGetFloatByKeyEPfPKc(this, value, key);
    }
};
}
}

extern "C" bool fn_00276AB8(sead::Vector3f* out, const al::ByamlIter* iter)
{
    if (!iter->isValid())
        return false;
    sead::Vector3f value;
    if (!iter->tryGetFloatByKey(&value.x, dat_003A2DFC))
        return false;
    if (!iter->tryGetFloatByKey(&value.y, dat_003A2E04))
        return false;
    if (!iter->tryGetFloatByKey(&value.z, dat_003A2E0C))
        return false;
    out->set(value);
    return true;
}
