#include <string.h>

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
        static void inverse( nn::math::MTX33& output, const nn::math::MTX33& input );
        static void copy( nn::math::MTX33& output, const nn::math::MTX33& input );
        static void transposeTo( nn::math::MTX33& output, const nn::math::MTX33& input );
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

template <>
void Matrix33CalcCtr<float>::transposeTo( nn::math::MTX33& output, const nn::math::MTX33& input )
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
        output.m[ 1 ][ 0 ] = m01;
        output.m[ 2 ][ 0 ] = m02;
        output.m[ 0 ][ 1 ] = m10;
        output.m[ 1 ][ 1 ] = m11;
        output.m[ 2 ][ 1 ] = m12;
        output.m[ 0 ][ 2 ] = m20;
        output.m[ 1 ][ 2 ] = m21;
        output.m[ 2 ][ 2 ] = m22;
}

// NonMatching: bounded ARM1176 replay agrees; the 404-byte body is not byte-exact.
// Preserve the target's sign inversion after each rounded product, including -0.
#ifdef NON_MATCHING
static float matrix33NegateProduct( float product )
{
        unsigned int word;
        memcpy( &word, &product, sizeof( word ) );
        word ^= 0x80000000U;
        memcpy( &product, &word, sizeof( product ) );
        return product;
}

template <>
void Matrix33CalcCtr<float>::inverse( nn::math::MTX33& output, const nn::math::MTX33& input )
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
        const float determinant =
                ( m00 * m11 * m22 - m20 * m11 * m02 ) +
                ( m01 * m12 * m20 - m10 * m01 * m22 ) +
                ( m02 * m10 * m21 - m00 * m21 * m12 );
        unsigned int determinantWord;
        memcpy( &determinantWord, &determinant, sizeof( determinantWord ) );
        if ( determinantWord == 0x80000000U || determinantWord == 0 )
        {
                output.m[ 0 ][ 0 ] = 1.0f;
                output.m[ 0 ][ 1 ] = 0.0f;
                output.m[ 0 ][ 2 ] = 0.0f;
                output.m[ 1 ][ 0 ] = 0.0f;
                output.m[ 1 ][ 1 ] = 1.0f;
                output.m[ 1 ][ 2 ] = 0.0f;
                output.m[ 2 ][ 0 ] = 0.0f;
                output.m[ 2 ][ 1 ] = 0.0f;
                output.m[ 2 ][ 2 ] = 1.0f;
                return;
        }
        const float reciprocal = 1.0f / determinant;
        const float o00 = ( m11 * m22 - m21 * m12 ) * reciprocal;
        const float o01 = matrix33NegateProduct( ( m01 * m22 - m21 * m02 ) * reciprocal );
        const float o02 = ( m01 * m12 - m11 * m02 ) * reciprocal;
        const float o10 = matrix33NegateProduct( ( m10 * m22 - m20 * m12 ) * reciprocal );
        const float o11 = ( m00 * m22 - m20 * m02 ) * reciprocal;
        const float o12 = matrix33NegateProduct( ( m00 * m12 - m10 * m02 ) * reciprocal );
        const float o20 = ( m10 * m21 - m20 * m11 ) * reciprocal;
        const float o21 = matrix33NegateProduct( ( m00 * m21 - m20 * m01 ) * reciprocal );
        const float o22 = ( m00 * m11 - m10 * m01 ) * reciprocal;
        output.m[ 0 ][ 0 ] = o00;
        output.m[ 0 ][ 1 ] = o01;
        output.m[ 0 ][ 2 ] = o02;
        output.m[ 1 ][ 0 ] = o10;
        output.m[ 1 ][ 1 ] = o11;
        output.m[ 1 ][ 2 ] = o12;
        output.m[ 2 ][ 0 ] = o20;
        output.m[ 2 ][ 1 ] = o21;
        output.m[ 2 ][ 2 ] = o22;
}

#endif

} // namespace sead

