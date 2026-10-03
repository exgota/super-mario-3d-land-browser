namespace nn { namespace math { struct MTX34 {}; } }

namespace sead {
template <typename T> struct Matrix34CalcCtr {
    static void copy(nn::math::MTX34&, const nn::math::MTX34&);
};
}

extern "C" void fn_0031FFA8(void* self, const nn::math::MTX34& source) {
    sead::Matrix34CalcCtr<float>::copy(
        *reinterpret_cast<nn::math::MTX34*>(static_cast<char*>(self) + 0x60),
        source);
}
