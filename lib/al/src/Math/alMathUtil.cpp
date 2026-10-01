#include <Math/alMathUtil.h>
#include <math/seadMatrix.h>

namespace al
{

bool isNearZero( float value, float range )
{
        return ( value > 0 ? value : -value ) < range;
}

} // namespace al

namespace sead
{

template <>
void Matrix34<float>::makeST( const Vector3<float>& scale, const Vector3<float>& translation )
{
        m[ 0 ][ 0 ] = scale.x;
        m[ 1 ][ 0 ] = 0;
        m[ 2 ][ 0 ] = 0;
        m[ 0 ][ 1 ] = 0;
        m[ 1 ][ 1 ] = scale.y;
        m[ 2 ][ 1 ] = 0;
        m[ 0 ][ 2 ] = 0;
        m[ 1 ][ 2 ] = 0;
        m[ 2 ][ 2 ] = scale.z;
        m[ 0 ][ 3 ] = translation.x;
        m[ 1 ][ 3 ] = translation.y;
        m[ 2 ][ 3 ] = translation.z;
}

} // namespace sead
