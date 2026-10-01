#include <Math/alMathUtil.h>
#include <Rail/alRail.h>
#include <Rail/alRailRider.h>

extern "C" float fn_0023e864( const al::Rail* rail, float coordinate );
extern "C" void fn_0023e7f8( al::Rail* rail, sead::Vector3f* position, sead::Vector3f* direction, float coordinate );
extern "C" float fn_00336eac( al::Rail* rail, const sead::Vector3f& position, float interval );
extern "C" float fn_00260fa8( const al::Rail* rail );

namespace al
{

#ifdef NON_MATCHING
// compiler completely skips _4 _10 (????????)
RailRider::RailRider( Rail* rail )
    : mRail( rail ), mCurrentPos( sead::Vector3f::zero ), mCurrentDir( sead::Vector3f::zero ), _1C( 0.0 ),
      mSpeed( 0.0 ), mIsLoop( true )
{
        _1C = mRail->normalizeLength( _1C );
        mRail->calcPosDir( &mCurrentPos, &mCurrentDir, _1C );
}
#endif

void RailRider::moveToRailStart()
{
        _1C = 0.0;
        _1C = fn_0023e864( mRail, _1C );
        fn_0023e7f8( mRail, &mCurrentPos, &mCurrentDir, _1C );
}

void RailRider::moveToNearestRail( const sead::Vector3f& r1 )
{
        _1C = fn_00336eac( mRail, r1, 20.0f );
        _1C = fn_0023e864( mRail, _1C );
        fn_0023e7f8( mRail, &mCurrentPos, &mCurrentDir, _1C );
}

bool RailRider::isReachedGoal() const
{
        if ( !mRail->isClosed() && al::isNearZero( _1C ) )
                return true;
        if ( !mRail->isClosed() && isNearZero( _1C - fn_00260fa8( mRail ) ) )
                return true;

        return false;
}

} // namespace al
