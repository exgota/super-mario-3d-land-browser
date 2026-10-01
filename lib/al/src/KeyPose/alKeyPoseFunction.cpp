#include <KeyPose/alKeyPose.h>
#include <KeyPose/alKeyPoseKeeper.h>

namespace al
{

const sead::Vector3f& getCurrentKeyTrans( const KeyPoseKeeper* p )
{
        return p->getCurrentKeyPose()->getTrans();
}

const sead::Vector3f& getNextKeyTrans( const KeyPoseKeeper* p )
{
        return p->getNextKeyPose()->getTrans();
}

const PlacementInfo* getNextKeyPlacementInfo( const KeyPoseKeeper* p )
{
        return p->getNextKeyPose()->getPlacementInfo();
}

} // namespace al
