namespace nn { namespace math { struct MTX34; } }
namespace sead {
template <typename T> struct Matrix34CalcCtr {
    static void copy(nn::math::MTX34 &, const nn::math::MTX34 &);
};
}

extern "C" void fn_002251FC(void *destination, const void *source) {
    sead::Matrix34CalcCtr<float>::copy(
        *reinterpret_cast<nn::math::MTX34 *>(static_cast<char *>(destination) + 0x24),
        *reinterpret_cast<const nn::math::MTX34 *>(source));
}
