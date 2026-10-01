#pragma once

#include <nn/types.h>

namespace sead
{

class Controller
{
private:
        u32 mVirtualTable;
        u32 mTriggerMask;

public:
        enum PadIndex
        {
                cPadIdx_A = 0,
                cPadIdx_B = 1,
                cPadIdx_X = 3,
                cPadIdx_Y = 4,
                cPadIdx_Select = 12,
                cPadIdx_Touch = 15,
                cPadIdx_Up = 16,
                cPadIdx_Down = 17,
                cPadIdx_Left = 18,
                cPadIdx_Right = 19,
                cPadIdx_LeftStickUp = 20,
                cPadIdx_LeftStickDown = 21,
                cPadIdx_LeftStickLeft = 22,
                cPadIdx_LeftStickRight = 23
        };
        // These indices have not yet been recovered from a named game wrapper.
        static const s32 cPadIdx_L;
        static const s32 cPadIdx_R;
        static const s32 cPadIdx_Start;
        static const s32 cPadIdx_Home;
        static const s32 cPadIdx_Minus;
        const u32* getTrigMaskPtr() const { return &mTriggerMask; }
};

class ControllerMgr
{
private:
        u8 mUnrecoveredPrefix[ 0xD0 ];
        u32 mControllerCount;
        u32 mUnrecoveredD4;
        Controller** mControllers;
        static ControllerMgr* sInstance;

public:
        static ControllerMgr* instance() { return sInstance; }
        Controller* getController( s32 index ) const
        {
                return mControllerCount > static_cast<u32>( index ) ? mControllers[ index ] : 0;
        }
};

} // namespace sead
