#include <AreaObj/alAreaShapeCube.h>

namespace al
{

AreaShapeCube::AreaShapeCube( bool isCubeBase ) : mIsCubeBase( isCubeBase )
{
}

bool AreaShapeCube::isInVolume( const sead::Vector3f& trans ) const
{
        sead::Vector3f localPos = sead::Vector3f::zero;
        calcLocalPos( &localPos, trans );
        sead::Vector2f bottomTopYBounds( mIsCubeBase == true ? 0.0f : -500.0f,
                                        mIsCubeBase == true ? 1000.0f : 500.0f );

        if ( localPos.y < bottomTopYBounds.x || localPos.y > bottomTopYBounds.y ||
             localPos.x < -500 || localPos.x > 500 || localPos.z < -500 || localPos.z > 500 )
                return false;

        return true;
}

} // namespace al
