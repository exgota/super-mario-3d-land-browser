#pragma once

class PlayerActionNode;

// Descriptive reconstruction name. The original class name is unresolved.
// The object is constructed at 0x001B4658 and filled by 0x001A9574.
class PlayerActionGraphBuildOutputs
{
public:
        PlayerActionNode* node00;
        PlayerActionNode* node04;
        PlayerActionNode* node08;
        PlayerActionNode* node0C;
        void* actionInterface10;
        void* actionInterface14;
        void* actionInterface18;
        PlayerActionNode* node1C;
        PlayerActionNode* node20;

        PlayerActionGraphBuildOutputs();
};
