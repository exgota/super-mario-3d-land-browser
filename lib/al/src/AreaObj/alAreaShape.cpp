#include <AreaObj/alAreaShape.h>

namespace al
{

AreaShape::AreaShape() : mBaseMtxPtr( nullptr ), mScale( sead::Vector3f( 1, 1, 1 ) )
{
}

/* TODO: Move this to header */
void AreaShape::setScale( const sead::Vector3f& scale )
{
        mScale.x = scale.x;
        mScale.y = scale.y;
        mScale.z = scale.z;
}

} // namespace al
