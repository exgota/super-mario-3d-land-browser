#include "Player/PlayerActionGraph.h"

#include "Player/PlayerAction.h"
#include "Player/PlayerActionNode.h"

void PlayerActionGraph::move()
{
        mCurrentNode->getAction()->update();
}

#pragma no_inline

PlayerAction* PlayerActionNode::getAction() const
{
        return mAction;
}
