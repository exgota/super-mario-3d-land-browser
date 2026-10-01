#pragma once

#include <container/seadListImpl.h>

class PlayerAction;
class PlayerActionCondition;

class PlayerActionNode
{
private:
        PlayerAction*  mAction;
        sead::ListImpl mList;

public:
        PlayerActionNode( PlayerAction* action );
        void append( PlayerActionCondition* condition, PlayerActionNode* destination );
        PlayerAction* getAction() const;

        virtual ~PlayerActionNode();
};
