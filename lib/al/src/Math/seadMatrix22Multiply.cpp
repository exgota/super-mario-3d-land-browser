namespace nn { namespace math {
struct MTX22 { float m[ 2 ][ 2 ]; };
} }

namespace sead
{
template <typename T> class Matrix22CalcCtr
{
public:
        static void multiply( nn::math::MTX22&, const nn::math::MTX22&, const nn::math::MTX22& );
};

// NonMatching: preserve both input matrices until all result words are ready.
#ifdef NON_MATCHING
template <>
void Matrix22CalcCtr<float>::multiply( nn::math::MTX22& output,
        const nn::math::MTX22& left, const nn::math::MTX22& right )
{
        const float a00 = left.m[ 0 ][ 0 ];
        const float a01 = left.m[ 0 ][ 1 ];
        const float a10 = left.m[ 1 ][ 0 ];
        const float a11 = left.m[ 1 ][ 1 ];
        const float b00 = right.m[ 0 ][ 0 ];
        const float b01 = right.m[ 0 ][ 1 ];
        const float b10 = right.m[ 1 ][ 0 ];
        const float b11 = right.m[ 1 ][ 1 ];
        const float m00 = a00 * b00 + a01 * b10;
        const float m01 = a00 * b01 + a01 * b11;
        const float m10 = a10 * b00 + a11 * b10;
        const float m11 = a10 * b01 + a11 * b11;
        output.m[ 0 ][ 0 ] = m00;
        output.m[ 0 ][ 1 ] = m01;
        output.m[ 1 ][ 0 ] = m10;
        output.m[ 1 ][ 1 ] = m11;
}
#endif
} // namespace sead
