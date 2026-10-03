#pragma once

namespace nn
{
namespace math
{

struct MTX34
{
        float m[ 3 ][ 4 ];
};

struct MTX44
{
        float m[ 4 ][ 4 ];
};

} // namespace math
} // namespace nn

namespace sead
{

template <typename T>
class Matrix34CalcCtr
{
public:
        static void copy( nn::math::MTX34& output, const nn::math::MTX44& input );
        static void copy( nn::math::MTX34& output, const nn::math::MTX34& input );
        static void multiply( nn::math::MTX34& output, const nn::math::MTX34& left,
                              const nn::math::MTX34& right );
};
} // namespace sead
