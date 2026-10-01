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

void PlayerActionMultiCondition::setup()
{
        sead::OffsetList<PlayerActionCondition*>::iterator cur = mConditions.begin();
        goto iterationCondition;
iterationBody:
        ( *cur )->setup();
        ++cur;
iterationCondition:
        if ( cur != mConditions.end() )
                goto iterationBody;
}
