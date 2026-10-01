#include <Stage/alStageSwitchAccesser.h>
#include <Stage/alStageSwitchKeeper.h>

namespace al
{

namespace StageSwitchKeeperReconstruction
{
int getStageSwitchTypeCount();
}

StageSwitchKeeper::StageSwitchKeeper() : mSwitches( nullptr ), mSwitchCount( 0 )
{
        mSwitchCount = StageSwitchKeeperReconstruction::getStageSwitchTypeCount();
        mSwitches    = new StageSwitchAccesser[ mSwitchCount ];
}

StageSwitchAccesser* StageSwitchKeeper::getStageSwitchAccesser( int type )
{
        return &mSwitches[ type ];
}

} // namespace al
