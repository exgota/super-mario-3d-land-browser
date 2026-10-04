#pragma once

#include <Layout/alLayoutActor.h>

// Layout and state identity recovered from the PauseMenu resource constructor.
class PauseMenu : public al::LayoutActor
{
public:
        void exeAppear();

private:
        void updateMenuEntries();

        void* mMenuGroup;          // 0x30
        void* mUnrecovered34;      // 0x34
        void* mUnrecovered38;      // 0x38
        bool mDisableEntry1;       // 0x3C
        bool mDisableEntry2;       // 0x3D
};

static_assert( sizeof( PauseMenu ) == 0x40, "PauseMenu constructor layout" );
