#pragma once

class PlayerActionNode;

class PlayerActionGraph
{
private:
        PlayerActionNode* mCurrentNode;
        PlayerActionNode* _4;
        bool mNeedsSetup;

public:
        PlayerActionGraph();

        PlayerActionNode* getCurrentNode() const
        {
                return mCurrentNode;
        }

        void setCurrentNode( PlayerActionNode* node )
        {
                mCurrentNode = node;
        }

        void move();
};
