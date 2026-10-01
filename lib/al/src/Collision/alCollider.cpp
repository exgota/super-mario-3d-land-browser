#include <Collision/alCollider.h>

extern "C" void fn_003370B4( const al::Collider* collider, sead::Vector3f* position );

namespace al
{

// NonMatching until the complete original interval passes the canonical checker.
#ifdef NON_MATCHING
void Collider::onInvalidate()
{
        clearCollisionResults();
        mCollisionIndex = -1;
        ::fn_003370B4( this, &mPreviousPosition );
        mPreviousRadius = mRadius;
}
#endif

} // namespace al
