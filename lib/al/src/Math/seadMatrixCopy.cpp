namespace nn
{
namespace math
{

struct MTX33
{
        float m[ 3 ][ 3 ];
};

} // namespace math
} // namespace nn

namespace sead
{

template <typename T>
class Matrix33CalcCtr
{
public:
        static void copy( nn::math::MTX33& output, const nn::math::MTX33& input );
};

template <>
void Matrix33CalcCtr<float>::copy( nn::math::MTX33& output, const nn::math::MTX33& input )
{
        const float m00 = input.m[ 0 ][ 0 ];
        const float m01 = input.m[ 0 ][ 1 ];
        const float m02 = input.m[ 0 ][ 2 ];
        const float m10 = input.m[ 1 ][ 0 ];
        const float m11 = input.m[ 1 ][ 1 ];
        const float m12 = input.m[ 1 ][ 2 ];
        const float m20 = input.m[ 2 ][ 0 ];
        const float m21 = input.m[ 2 ][ 1 ];
        const float m22 = input.m[ 2 ][ 2 ];

        output.m[ 0 ][ 0 ] = m00;
        output.m[ 0 ][ 1 ] = m01;
        output.m[ 0 ][ 2 ] = m02;
        output.m[ 1 ][ 0 ] = m10;
        output.m[ 1 ][ 1 ] = m11;
        output.m[ 1 ][ 2 ] = m12;
        output.m[ 2 ][ 0 ] = m20;
        output.m[ 2 ][ 1 ] = m21;
        output.m[ 2 ][ 2 ] = m22;
}

} // namespace sead
