#pragma once

#include <Placement/alPlacementInfo.h>
#include <math/seadVector.h>

namespace al
{

class Rail
{
private:
        int  _0;
        int  _4;
        int  _8;
        int  _C;
        bool mIsClosed;

public:
        bool isClosed() const
        {
                return mIsClosed;
        }

        const PlacementInfo* getPointPlacementInfo( int index ) const
        {
                return reinterpret_cast<const PlacementInfo* const*>( _0 )[ index ];
        }

        void init( const PlacementInfo& info );

        float getTotalLength() const;
        float normalizeLength( float ) const;
        void  calcPosDir( sead::Vector3f*, sead::Vector3f*, float );
        float calcNearestRailPosCoord( const sead::Vector3f&, float );

public:
        Rail();
};

} // namespace al
