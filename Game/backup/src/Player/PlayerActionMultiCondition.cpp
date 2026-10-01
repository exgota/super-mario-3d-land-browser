#include "Player/PlayerActionMultiCondition.h"

PlayerActionMultiCondition::PlayerActionMultiCondition()
{
        mConditions.initOffset( sead::OffsetListNode<PlayerActionCondition*>::getListNodeOffset() );
}

void PlayerActionMultiCondition::append( PlayerActionCondition* condition )
{
        sead::OffsetList<PlayerActionCondition*>& conditions = mConditions;
        sead::OffsetListNode<PlayerActionCondition*>* node = new sead::OffsetListNode<PlayerActionCondition*>( condition );
        conditions.pushBack( *node );
}

#ifdef NON_MATCHING
bool PlayerActionMultiCondition::check()
{
        return true;
}
#endif

#ifdef NON_MATCHING
// really very incorrect
void PlayerActionMultiCondition::setup()
{
        for ( sead::OffsetList<PlayerActionCondition*>::iterator cur = mConditions.begin();
                cur != mConditions.end();
                ++cur )
                ( *cur )->setup();
}
#endif
