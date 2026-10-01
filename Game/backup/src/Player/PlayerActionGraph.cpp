#include "Player/PlayerActionGraph.h"

#include "Player/PlayerAction.h"
#include "Player/PlayerActionNode.h"

PlayerActionGraph::PlayerActionGraph()
    : mCurrentNode( 0 ), _4( 0 ), mNeedsSetup( true )
{
}

void PlayerActionGraph::move()
{
        mCurrentNode->getAction()->update();
}

#pragma no_inline

PlayerAction* PlayerActionNode::getAction() const
{
        return mAction;
}
