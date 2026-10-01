#pragma once

#include <nn/types.h>
#include <container/seadOffsetList.h>
#include "Player/PlayerAction.h"
#include "Player/PlayerActionCondition.h"
#include "Player/PlayerActionNode.h"
#include "Player/PlayerActionMultiCondition.h"
#include "Player/PlayerActionConditionAnimEnd.h"

// Neutral names encode observed constructor/table addresses, not recovered names.
// Out-of-line constructors are declarations for independently observed callees.
// Provider slot declarations below establish vtable layout only. Their return
// signatures are unrecovered, and this builder never invokes those slots.
// Unrecovered fields preserve the allocation extent without inventing behavior.

class PlayerGraphConfiguration
{
public:
        virtual void unrecovered_000() const;
        virtual void unrecovered_004() const;
        virtual void unrecovered_008() const;
        virtual void unrecovered_00C() const;
        virtual void unrecovered_010() const;
        virtual void unrecovered_014() const;
        virtual void unrecovered_018() const;
        virtual void unrecovered_01C() const;
        virtual void unrecovered_020() const;
        virtual void unrecovered_024() const;
        virtual void unrecovered_028() const;
        virtual void unrecovered_02C() const;
        virtual void unrecovered_030() const;
        virtual void unrecovered_034() const;
        virtual void unrecovered_038() const;
        virtual void unrecovered_03C() const;
        virtual void unrecovered_040() const;
        virtual void unrecovered_044() const;
        virtual void unrecovered_048() const;
        virtual void unrecovered_04C() const;
        virtual void unrecovered_050() const;
        virtual void unrecovered_054() const;
        virtual void unrecovered_058() const;
        virtual void unrecovered_05C() const;
        virtual void unrecovered_060() const;
        virtual void unrecovered_064() const;
        virtual void unrecovered_068() const;
        virtual void unrecovered_06C() const;
        virtual void unrecovered_070() const;
        virtual void unrecovered_074() const;
        virtual void unrecovered_078() const;
        virtual void unrecovered_07C() const;
        virtual void unrecovered_080() const;
        virtual void unrecovered_084() const;
        virtual void unrecovered_088() const;
        virtual void unrecovered_08C() const;
        virtual void unrecovered_090() const;
        virtual void unrecovered_094() const;
        virtual void unrecovered_098() const;
        virtual void unrecovered_09C() const;
        virtual void unrecovered_0A0() const;
        virtual void unrecovered_0A4() const;
        virtual void unrecovered_0A8() const;
        virtual void unrecovered_0AC() const;
        virtual void unrecovered_0B0() const;
        virtual void unrecovered_0B4() const;
        virtual void unrecovered_0B8() const;
        virtual void unrecovered_0BC() const;
        virtual void unrecovered_0C0() const;
        virtual void unrecovered_0C4() const;
        virtual void unrecovered_0C8() const;
        virtual void unrecovered_0CC() const;
        virtual void unrecovered_0D0() const;
        virtual void unrecovered_0D4() const;
        virtual void unrecovered_0D8() const;
        virtual void unrecovered_0DC() const;
        virtual void unrecovered_0E0() const;
        virtual void unrecovered_0E4() const;
        virtual void unrecovered_0E8() const;
        virtual void unrecovered_0EC() const;
        virtual void unrecovered_0F0() const;
        virtual void unrecovered_0F4() const;
        virtual void unrecovered_0F8() const;
        virtual void unrecovered_0FC() const;
        virtual void unrecovered_100() const;
        virtual void unrecovered_104() const;
        virtual float value_108() const;
        virtual void unrecovered_10C() const;
        virtual void unrecovered_110() const;
        virtual void unrecovered_114() const;
        virtual void unrecovered_118() const;
        virtual void unrecovered_11C() const;
        virtual void unrecovered_120() const;
        virtual void unrecovered_124() const;
        virtual void unrecovered_128() const;
        virtual void unrecovered_12C() const;
        virtual void unrecovered_130() const;
        virtual void unrecovered_134() const;
        virtual void unrecovered_138() const;
        virtual void unrecovered_13C() const;
        virtual void unrecovered_140() const;
        virtual void unrecovered_144() const;
        virtual void unrecovered_148() const;
        virtual void unrecovered_14C() const;
        virtual void unrecovered_150() const;
        virtual void unrecovered_154() const;
        virtual void unrecovered_158() const;
        virtual void unrecovered_15C() const;
        virtual void unrecovered_160() const;
        virtual void unrecovered_164() const;
        virtual void unrecovered_168() const;
        virtual void unrecovered_16C() const;
        virtual void unrecovered_170() const;
        virtual void unrecovered_174() const;
        virtual void unrecovered_178() const;
        virtual int value_17C() const;
        virtual void unrecovered_180() const;
        virtual void unrecovered_184() const;
        virtual void unrecovered_188() const;
        virtual void unrecovered_18C() const;
        virtual void unrecovered_190() const;
        virtual int value_194() const;
        virtual void unrecovered_198() const;
        virtual void unrecovered_19C() const;
        virtual void unrecovered_1A0() const;
        virtual int value_1A4() const;
};

template <unsigned int Offset, class T>
inline void* playerGraphInterface( T* object )
{
        return object ? reinterpret_cast<char*>( object ) + Offset : 0;
}

class PlayerGraphActionContext
{
public:
        void* field00;
        void* field04;
        void* field08;
        void* field0C;
        void* field10;
        void* field14;
        void* field18;
        void* field1C;
        void* field20;
        PlayerGraphActionContext( void* p0, void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7, void* p8 )
            : field00( p0 ), field04( p1 ), field08( p2 ), field0C( p3 ), field10( p4 ), field14( p5 ), field18( p6 ), field1C( p7 ), field20( p8 )
        {
        }
};

class PlayerActionAnyCondition : public PlayerActionCondition
{
        sead::OffsetList<PlayerActionCondition*> mConditions;
public:
        PlayerActionAnyCondition();
        void append( PlayerActionCondition* condition );
        virtual bool check();
        virtual void setup();
};

class PlayerActionNotCondition : public PlayerActionCondition
{
        PlayerActionCondition* mCondition;
public:
        PlayerActionNotCondition( PlayerActionCondition* condition );
        virtual bool check();
        virtual void setup();
};

// Constructor 0x00171A78; allocation extent 0x28.
class PlayerGraphAction_00171A78 : public PlayerAction
{
        u32 mUnrecovered[9];
public:
        PlayerGraphAction_00171A78( void* p0, void* p1, void* p2, void* p3, void* p4 );
};

// Constructor 0x00171CDC; allocation extent 0x10.
class PlayerGraphAction_00171CDC : public PlayerAction
{
        u32 mUnrecovered[3];
public:
        PlayerGraphAction_00171CDC( void* p0 );
};

// Constructor 0x00171D64; allocation extent 0x58.
class PlayerGraphAction_00171D64 : public PlayerAction
{
        u32 mUnrecovered[21];
public:
        PlayerGraphAction_00171D64( void* p0, void* p1 );
};

// Constructor 0x00173B5C; allocation extent 0x70.
class PlayerGraphAction_00173B5C : public PlayerAction
{
        u32 mUnrecovered[27];
public:
        PlayerGraphAction_00173B5C( void* p0, void* p1, void* p2, void* p3, void* p4, void* p5 );
};

// Constructor 0x00173F0C; allocation extent 0x14.
class PlayerGraphAction_00173F0C : public PlayerAction
{
        u32 mUnrecovered[4];
public:
        PlayerGraphAction_00173F0C( void* p0 );
};

// Constructor 0x00174840; allocation extent 0xC.
class PlayerGraphAction_00174840 : public PlayerAction
{
        u32 mUnrecovered[2];
public:
        PlayerGraphAction_00174840( void* p0 );
};

// Constructor 0x001749C8; allocation extent 0x8.
class PlayerGraphAction_001749C8 : public PlayerAction
{
        u32 mUnrecovered[1];
public:
        PlayerGraphAction_001749C8( void* p0 );
};

// Constructor 0x00174C18; allocation extent 0xC.
class PlayerGraphAction_00174C18 : public PlayerAction
{
        u32 mUnrecovered[2];
public:
        PlayerGraphAction_00174C18( void* p0, void* p1 );
};

// Constructor 0x00175344; allocation extent 0x30.
class PlayerGraphAction_00175344 : public PlayerAction
{
        u32 mUnrecovered[11];
public:
        PlayerGraphAction_00175344( void* p0, void* p1 );
};

// Constructor 0x0018128C; allocation extent 0xC.
class PlayerGraphAction_0018128C : public PlayerAction
{
        u32 mUnrecovered[2];
public:
        PlayerGraphAction_0018128C( void* p0, void* p1 );
};

// Constructor 0x00181420; allocation extent 0x18.
class PlayerGraphAction_00181420 : public PlayerAction
{
        u32 mUnrecovered[5];
public:
        PlayerGraphAction_00181420( void* p0, void* p1, void* p2 );
};

// Constructor 0x001818FC; allocation extent 0x20.
class PlayerGraphAction_001818FC : public PlayerAction
{
        u32 mUnrecovered[7];
public:
        PlayerGraphAction_001818FC( void* p0, void* p1 );
};

// Constructor 0x0018A390; allocation extent 0x20.
class PlayerGraphAction_0018A390 : public PlayerAction
{
        u32 mUnrecovered[7];
public:
        PlayerGraphAction_0018A390( void* p0, void* p1, void* p2, void* p3, void* p4, void* p5 );
};

// Constructor 0x0018EBA4; allocation extent 0x30.
class PlayerGraphAction_0018EBA4 : public PlayerAction
{
        u32 mUnrecoveredBefore[6];
public:
        void* componentB4;
private:
        u32 mUnrecoveredAfter[4];
public:
        PlayerGraphAction_0018EBA4( void* p0, void* p1, void* p2, void* p3, void* p4 );
};

// Constructor 0x0018F0F4; allocation extent 0x14.
class PlayerGraphAction_0018F0F4 : public PlayerAction
{
        u32 mUnrecovered[4];
public:
        PlayerGraphAction_0018F0F4( void* p0, void* p1 );
};

// Constructor 0x00196300; allocation extent 0x58.
class PlayerGraphAction_00196300 : public PlayerAction
{
        u32 mUnrecovered[21];
public:
        PlayerGraphAction_00196300( void* p0, void* p1 );
};

// Constructor 0x001963DC; allocation extent 0x5C.
class PlayerGraphAction_001963DC : public PlayerAction
{
        u32 mUnrecovered[22];
public:
        PlayerGraphAction_001963DC( void* p0, void* p1, void* p2 );
};

// Constructor 0x00196838; allocation extent 0xC.
class PlayerGraphAction_00196838 : public PlayerAction
{
        u32 mUnrecovered[2];
public:
        PlayerGraphAction_00196838( void* p0, void* p1 );
};

// Constructor 0x00196EB4; allocation extent 0x10.
class PlayerGraphAction_00196EB4 : public PlayerAction
{
        u32 mUnrecovered[3];
public:
        PlayerGraphAction_00196EB4( void* p0 );
};

// Constructor 0x001972EC; allocation extent 0x28.
class PlayerGraphAction_001972EC : public PlayerAction
{
        u32 mUnrecoveredBefore[7];
public:
        void* componentB4;
private:
        u32 mUnrecoveredAfter[1];
public:
        PlayerGraphAction_001972EC( void* p0, void* p1 );
};

// Constructor 0x001974C0; allocation extent 0x58.
class PlayerGraphAction_001974C0 : public PlayerAction
{
        u32 mUnrecovered[21];
public:
        PlayerGraphAction_001974C0( void* p0, void* p1 );
};

// Constructor 0x00197B90; allocation extent 0x8.
class PlayerGraphAction_00197B90 : public PlayerAction
{
        u32 mUnrecovered[1];
public:
        PlayerGraphAction_00197B90( void* p0 );
};

// Constructor 0x00197C64; allocation extent 0x5C.
class PlayerGraphAction_00197C64 : public PlayerAction
{
        u32 mUnrecovered[22];
public:
        PlayerGraphAction_00197C64( void* p0, void* p1, void* p2 );
};

// Constructor 0x00197EF0; allocation extent 0x64.
class PlayerGraphAction_00197EF0 : public PlayerAction
{
        u32 mUnrecovered[24];
public:
        PlayerGraphAction_00197EF0( void* p0, void* p1, void* p2, void* p3, void* p4 );
};

// Constructor 0x0019D950; allocation extent 0x24.
class PlayerGraphAction_0019D950 : public PlayerAction
{
        u32 mUnrecovered[8];
public:
        PlayerGraphAction_0019D950( void* p0, void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7 );
};

// Constructor 0x0019DC48; allocation extent 0x64.
class PlayerGraphAction_0019DC48 : public PlayerAction
{
        u32 mUnrecovered[24];
public:
        PlayerGraphAction_0019DC48( void* p0, void* p1, void* p2, void* p3, void* p4 );
};

// Constructor 0x0019DD70; allocation extent 0x1C.
class PlayerGraphAction_0019DD70 : public PlayerAction
{
        u32 mUnrecovered[6];
public:
        PlayerGraphAction_0019DD70( void* p0, void* p1, void* p2 );
};

// Constructor 0x0019E5D4; allocation extent 0x34.
class PlayerGraphAction_0019E5D4 : public PlayerAction
{
        u32 mUnrecovered[12];
public:
        PlayerGraphAction_0019E5D4( void* p0, void* p1, void* p2 );
};

// Constructor 0x0019EFB4; allocation extent 0x34.
class PlayerGraphAction_0019EFB4 : public PlayerAction
{
        u32 mUnrecovered[12];
public:
        PlayerGraphAction_0019EFB4( void* p0, void* p1, void* p2, void* p3, void* p4, void* p5 );
};

// Constructor 0x0019F5A4; allocation extent 0x20.
class PlayerGraphAction_0019F5A4 : public PlayerAction
{
        u32 mUnrecovered[7];
public:
        PlayerGraphAction_0019F5A4( void* p0, void* p1, void* p2, void* p3 );
};

// Constructor 0x001A46C8; allocation extent 0x44.
class PlayerGraphAction_001A46C8 : public PlayerAction
{
        u32 mUnrecovered[16];
public:
        PlayerGraphAction_001A46C8( void* p0, void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7 );
};

// Constructor 0x001A4948; allocation extent 0x8.
class PlayerGraphAction_001A4948 : public PlayerAction
{
        u32 mUnrecovered[1];
public:
        PlayerGraphAction_001A4948( void* p0 );
};

// Constructor 0x001A725C; allocation extent 0x14.
class PlayerGraphAction_001A725C : public PlayerAction
{
        u32 mUnrecovered[4];
public:
        PlayerGraphAction_001A725C( void* p0 );
};

// Constructor 0x001A73B4; allocation extent 0x8.
class PlayerGraphAction_001A73B4 : public PlayerAction
{
        u32 mUnrecovered[1];
public:
        PlayerGraphAction_001A73B4( void* p0 );
};

// Constructor 0x001A79BC; allocation extent 0x38.
class PlayerGraphAction_001A79BC : public PlayerAction
{
        u32 mUnrecoveredBefore[6];
public:
        void* componentB4;
private:
        u32 mUnrecoveredAfter[6];
public:
        PlayerGraphAction_001A79BC( void* p0, void* p1, void* p2, void* p3, void* p4 );
};

// Constructor 0x001A7FD4; allocation extent 0x60.
class PlayerGraphAction_001A7FD4 : public PlayerAction
{
        u32 mUnrecovered[23];
public:
        PlayerGraphAction_001A7FD4( void* p0, void* p1, void* p2 );
};

// Constructor 0x001A9560; allocation extent 0x8.
class PlayerGraphCondition_001A9560 : public PlayerActionCondition
{
        u32 mUnrecovered[1];
public:
        PlayerGraphCondition_001A9560( void* p0 );
        virtual bool check();
};

// Constructor 0x001B7200; allocation extent 0x28.
class PlayerGraphAction_001B7200 : public PlayerAction
{
        u32 mUnrecovered[9];
public:
        PlayerGraphAction_001B7200( void* p0, void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7, void* p8 );
};

// Constructor 0x001B7290; allocation extent 0x8.
class PlayerGraphCondition_001B7290 : public PlayerActionCondition
{
        u32 mUnrecovered[1];
public:
        PlayerGraphCondition_001B7290( void* p0 );
        virtual bool check();
};

// Constructor 0x001B79AC; allocation extent 0x7C.
class PlayerGraphAction_001B79AC : public PlayerAction
{
        u32 mUnrecovered[30];
public:
        PlayerGraphAction_001B79AC( void* p0, void* p1, void* p2, void* p3 );
};

// Constructor 0x001B8B38; allocation extent 0x5C.
class PlayerGraphAction_001B8B38 : public PlayerAction
{
        u32 mUnrecovered[22];
public:
        PlayerGraphAction_001B8B38( void* p0, void* p1, void* p2 );
};

// Constructor 0x001B9048; allocation extent 0x60.
class PlayerGraphAction_001B9048 : public PlayerAction
{
        u32 mUnrecovered[23];
public:
        PlayerGraphAction_001B9048( void* p0, void* p1, void* p2 );
};

// Constructor 0x001B99C4; allocation extent 0xC.
class PlayerGraphCondition_001B99C4 : public PlayerActionCondition
{
        u32 mUnrecovered[2];
public:
        PlayerGraphCondition_001B99C4( void* p0 );
        virtual bool check();
};

// Constructor 0x001B9DB8; allocation extent 0x8.
class PlayerGraphCondition_001B9DB8 : public PlayerActionCondition
{
        u32 mUnrecovered[1];
public:
        PlayerGraphCondition_001B9DB8( void* p0 );
        virtual bool check();
};

// Constructor 0x001B9EC8; allocation extent 0x8.
class PlayerGraphCondition_001B9EC8 : public PlayerActionCondition
{
        u32 mUnrecovered[1];
public:
        PlayerGraphCondition_001B9EC8( void* p0 );
        virtual bool check();
};

// Constructor 0x001B9F38; allocation extent 0x8.
class PlayerGraphCondition_001B9F38 : public PlayerActionCondition
{
        u32 mUnrecovered[1];
public:
        PlayerGraphCondition_001B9F38( void* p0 );
        virtual bool check();
};

// Constructor 0x001B9F64; allocation extent 0xC.
class PlayerGraphCondition_001B9F64 : public PlayerActionCondition
{
        u32 mUnrecovered[2];
public:
        PlayerGraphCondition_001B9F64( void* p0, void* p1 );
        virtual bool check();
};

// Constructor 0x001B9F9C; allocation extent 0x8.
class PlayerGraphCondition_001B9F9C : public PlayerActionCondition
{
        u32 mUnrecovered[1];
public:
        PlayerGraphCondition_001B9F9C( void* p0 );
        virtual bool check();
};

// Constructor 0x001B9FC8; allocation extent 0xC.
class PlayerGraphCondition_001B9FC8 : public PlayerActionCondition
{
        u32 mUnrecovered[2];
public:
        PlayerGraphCondition_001B9FC8( void* p0, void* p1 );
        virtual bool check();
};

// Constructor 0x001B9FE0; allocation extent 0xC.
class PlayerGraphCondition_001B9FE0 : public PlayerActionCondition
{
        u32 mUnrecovered[2];
public:
        PlayerGraphCondition_001B9FE0( void* p0, int p1 );
        virtual bool check();
};

// Constructor 0x001BA530; allocation extent 0x1C.
class PlayerGraphAction_001BA530 : public PlayerAction
{
        u32 mUnrecovered[6];
public:
        PlayerGraphAction_001BA530( void* p0, void* p1, void* p2, void* p3, void* p4 );
};

// Constructor 0x001BABA4; allocation extent 0x8.
class PlayerGraphCondition_001BABA4 : public PlayerActionCondition
{
        u32 mUnrecovered[1];
public:
        PlayerGraphCondition_001BABA4( void* p0 );
        virtual bool check();
};

// Constructor 0x001BAC1C; allocation extent 0xC.
class PlayerGraphCondition_001BAC1C : public PlayerActionCondition
{
        u32 mUnrecovered[2];
public:
        PlayerGraphCondition_001BAC1C( void* p0, float p1 );
        virtual bool check();
};

// Constructor 0x001BAC48; allocation extent 0x8.
class PlayerGraphCondition_001BAC48 : public PlayerActionCondition
{
        u32 mUnrecovered[1];
public:
        PlayerGraphCondition_001BAC48( void* p0 );
        virtual bool check();
};

// Constructor 0x001BAD04; allocation extent 0x10.
class PlayerGraphCondition_001BAD04 : public PlayerActionCondition
{
        u32 mUnrecovered[3];
public:
        PlayerGraphCondition_001BAD04( void* p0, void* p1 );
        virtual bool check();
};

// Constructor 0x001BAD78; allocation extent 0xC.
class PlayerGraphCondition_001BAD78 : public PlayerActionCondition
{
        u32 mUnrecovered[2];
public:
        PlayerGraphCondition_001BAD78( void* p0, void* p1 );
        virtual bool check();
};

// Constructor 0x001BAE88; allocation extent 0x70.
class PlayerGraphAction_001BAE88 : public PlayerAction
{
        u32 mUnrecovered[27];
public:
        PlayerGraphAction_001BAE88( void* p0, void* p1, void* p2, void* p3, void* p4, void* p5 );
};

// Constructor 0x00251EBC; allocation extent 0x14.
class PlayerGraphCondition_00251EBC : public PlayerActionCondition
{
        u32 mUnrecovered[4];
public:
        PlayerGraphCondition_00251EBC( void* p0, void* p1, void* p2, void* p3 );
        virtual bool check();
};

// Constructor 0x00251EE4; allocation extent 0x10.
class PlayerGraphCondition_00251EE4 : public PlayerActionCondition
{
        u32 mUnrecovered[3];
public:
        PlayerGraphCondition_00251EE4( void* p0, void* p1, void* p2 );
        virtual bool check();
};

// Constructor 0x00251EFC; allocation extent 0xC.
class PlayerGraphCondition_00251EFC : public PlayerActionCondition
{
        u32 mUnrecovered[2];
public:
        PlayerGraphCondition_00251EFC( void* p0, void* p1 );
        virtual bool check();
};

// Constructor 0x00251F14; allocation extent 0x10.
class PlayerGraphCondition_00251F14 : public PlayerActionCondition
{
        u32 mUnrecovered[3];
public:
        PlayerGraphCondition_00251F14( void* p0, int p1, void* p2 );
        virtual bool check();
};

// Constructor 0x00251F2C; allocation extent 0x8.
class PlayerGraphCondition_00251F2C : public PlayerActionCondition
{
        u32 mUnrecovered[1];
public:
        PlayerGraphCondition_00251F2C( void* p0 );
        virtual bool check();
};

// Constructor 0x00251F40; allocation extent 0x8.
class PlayerGraphCondition_00251F40 : public PlayerActionCondition
{
        u32 mUnrecovered[1];
public:
        PlayerGraphCondition_00251F40( void* p0 );
        virtual bool check();
};

// Constructor 0x00251F54; allocation extent 0x10.
class PlayerGraphCondition_00251F54 : public PlayerActionCondition
{
        u32 mUnrecovered[3];
public:
        PlayerGraphCondition_00251F54( int p0, void* p1 );
        virtual bool check();
};

// Constructor 0x00251FEC; allocation extent 0xC.
class PlayerGraphCondition_00251FEC : public PlayerActionCondition
{
        u32 mUnrecovered[2];
public:
        PlayerGraphCondition_00251FEC( void* p0, const int& p1 );
        virtual bool check();
};

// Constructor 0x00252094; allocation extent 0x20.
class PlayerGraphAction_00252094 : public PlayerAction
{
        u32 mUnrecovered[7];
public:
        PlayerGraphAction_00252094( void* p0, void* p1, void* p2, void* p3, void* p4, void* p5 );
};

// Constructor 0x002520D4; allocation extent 0x38.
class PlayerGraphAction_002520D4 : public PlayerAction
{
        u32 mUnrecovered[13];
public:
        PlayerGraphAction_002520D4( void* p0, void* p1, void* p2, void* p3, void* p4, void* p5, void* p6 );
};

// Constructor 0x00252124; allocation extent 0x10.
class PlayerGraphAction_00252124 : public PlayerAction
{
        u32 mUnrecovered[3];
public:
        PlayerGraphAction_00252124( void* p0, void* p1 );
};

// Constructor 0x002D1E14; allocation extent 0x8.
class PlayerGraphCondition_002D1E14 : public PlayerActionCondition
{
        u32 mUnrecovered[1];
public:
        PlayerGraphCondition_002D1E14( void* p0 );
        virtual bool check();
};

// Constructor 0x002D1F20; allocation extent 0x10.
class PlayerGraphCondition_002D1F20 : public PlayerActionCondition
{
        u32 mUnrecovered[3];
public:
        PlayerGraphCondition_002D1F20( void* p0, void* p1 );
        virtual bool check();
};

// Constructor 0x002D1F84; allocation extent 0x8.
class PlayerGraphCondition_002D1F84 : public PlayerActionCondition
{
        u32 mUnrecovered[1];
public:
        PlayerGraphCondition_002D1F84( void* p0 );
        virtual bool check();
};

// Constructor 0x002D2054; allocation extent 0xC.
class PlayerGraphCondition_002D2054 : public PlayerActionCondition
{
        u32 mUnrecovered[2];
public:
        PlayerGraphCondition_002D2054( void* p0, void* p1 );
        virtual bool check();
};

// Constructor 0x002D232C; allocation extent 0xC4.
class PlayerGraphAction_002D232C : public PlayerAction
{
        u32 mUnrecovered[48];
public:
        PlayerGraphAction_002D232C( void* p0, void* p1, void* p2, void* p3, void* p4, void* p5, void* p6 );
};

// Constructor 0x002D26E8; allocation extent 0xC.
class PlayerGraphCondition_002D26E8 : public PlayerActionCondition
{
        u32 mUnrecovered[2];
public:
        PlayerGraphCondition_002D26E8( void* p0, int p1 );
        virtual bool check();
};

// Constructor 0x002D27AC; allocation extent 0xC.
class PlayerGraphCondition_002D27AC : public PlayerActionCondition
{
        u32 mUnrecovered[2];
public:
        PlayerGraphCondition_002D27AC( void* p0, void* p1 );
        virtual bool check();
};

// Constructor 0x002D27D4; allocation extent 0x8.
class PlayerGraphCondition_002D27D4 : public PlayerActionCondition
{
        u32 mUnrecovered[1];
public:
        PlayerGraphCondition_002D27D4( void* p0 );
        virtual bool check();
};

// Constructor 0x002D27F8; allocation extent 0x8.
class PlayerGraphCondition_002D27F8 : public PlayerActionCondition
{
        u32 mUnrecovered[1];
public:
        PlayerGraphCondition_002D27F8( void* p0 );
        virtual bool check();
};

// Constructor 0x002D281C; allocation extent 0x8.
class PlayerGraphCondition_002D281C : public PlayerActionCondition
{
        u32 mUnrecovered[1];
public:
        PlayerGraphCondition_002D281C( void* p0 );
        virtual bool check();
};

// Constructor 0x002D2888; allocation extent 0x10.
class PlayerGraphCondition_002D2888 : public PlayerActionCondition
{
        u32 mUnrecovered[3];
public:
        PlayerGraphCondition_002D2888( void* p0, void* p1 );
        virtual bool check();
};

// Constructor 0x002D2A20; allocation extent 0x14.
class PlayerGraphCondition_002D2A20 : public PlayerActionCondition
{
        u32 mUnrecovered[4];
public:
        PlayerGraphCondition_002D2A20( void* p0, void* p1 );
        virtual bool check();
};

// Constructor 0x002D2A84; allocation extent 0x10.
class PlayerGraphCondition_002D2A84 : public PlayerActionCondition
{
        u32 mUnrecovered[3];
public:
        PlayerGraphCondition_002D2A84( void* p0, void* p1, void* p2 );
        virtual bool check();
};

// Constructor 0x002D2AC0; allocation extent 0x8.
class PlayerGraphCondition_002D2AC0 : public PlayerActionCondition
{
        u32 mUnrecovered[1];
public:
        PlayerGraphCondition_002D2AC0( void* p0 );
        virtual bool check();
};

// Constructor 0x002D2B14; allocation extent 0xC.
class PlayerGraphCondition_002D2B14 : public PlayerActionCondition
{
        u32 mUnrecovered[2];
public:
        PlayerGraphCondition_002D2B14( void* p0, void* p1 );
        virtual bool check();
};

// Constructor 0x002D2BDC; allocation extent 0xC.
class PlayerGraphCondition_002D2BDC : public PlayerActionCondition
{
        u32 mUnrecovered[2];
public:
        PlayerGraphCondition_002D2BDC( void* p0, void* p1 );
        virtual bool check();
};

// Constructor 0x002D2C08; allocation extent 0x8.
class PlayerGraphCondition_002D2C08 : public PlayerActionCondition
{
        u32 mUnrecovered[1];
public:
        PlayerGraphCondition_002D2C08( void* p0 );
        virtual bool check();
};

// Constructor 0x002D2C2C; allocation extent 0x8.
class PlayerGraphCondition_002D2C2C : public PlayerActionCondition
{
        u32 mUnrecovered[1];
public:
        PlayerGraphCondition_002D2C2C( void* p0 );
        virtual bool check();
};

// Constructor 0x002D2C50; allocation extent 0x8.
class PlayerGraphCondition_002D2C50 : public PlayerActionCondition
{
        u32 mUnrecovered[1];
public:
        PlayerGraphCondition_002D2C50( void* p0 );
        virtual bool check();
};

// Constructor 0x002D2CC8; allocation extent 0x10.
class PlayerGraphCondition_002D2CC8 : public PlayerActionCondition
{
        u32 mUnrecovered[3];
public:
        PlayerGraphCondition_002D2CC8( void* p0, void* p1, void* p2 );
        virtual bool check();
};

// Constructor 0x002D2D34; allocation extent 0x10.
class PlayerGraphCondition_002D2D34 : public PlayerActionCondition
{
        u32 mUnrecovered[3];
public:
        PlayerGraphCondition_002D2D34( void* p0, void* p1, void* p2 );
        virtual bool check();
};

// Constructor 0x002D301C; allocation extent 0x18.
class PlayerGraphCondition_002D301C : public PlayerActionCondition
{
        u32 mUnrecovered[5];
public:
        PlayerGraphCondition_002D301C( void* p0, void* p1, void* p2, void* p3, void* p4 );
        virtual bool check();
};

// Constructor 0x002D30E4; allocation extent 0x10.
class PlayerGraphCondition_002D30E4 : public PlayerActionCondition
{
        u32 mUnrecovered[3];
public:
        PlayerGraphCondition_002D30E4( void* p0, void* p1, void* p2 );
        virtual bool check();
};

// Constructor 0x002D3150; allocation extent 0x8.
class PlayerGraphCondition_002D3150 : public PlayerActionCondition
{
        u32 mUnrecovered[1];
public:
        PlayerGraphCondition_002D3150( void* p0 );
        virtual bool check();
};

// Constructor 0x002D34C8; allocation extent 0x10.
class PlayerGraphCondition_002D34C8 : public PlayerActionCondition
{
        u32 mUnrecovered[3];
public:
        PlayerGraphCondition_002D34C8( void* p0, void* p1, void* p2 );
        virtual bool check();
};

// Constructor 0x002D358C; allocation extent 0xC.
class PlayerGraphCondition_002D358C : public PlayerActionCondition
{
        u32 mUnrecovered[2];
public:
        PlayerGraphCondition_002D358C( void* p0, void* p1 );
        virtual bool check();
};

// Constructor 0x002D35C0; allocation extent 0x8.
class PlayerGraphCondition_002D35C0 : public PlayerActionCondition
{
        u32 mUnrecovered[1];
public:
        PlayerGraphCondition_002D35C0( void* p0 );
        virtual bool check();
};

// Constructor 0x002D365C; allocation extent 0x8.
class PlayerGraphCondition_002D365C : public PlayerActionCondition
{
        u32 mUnrecovered[1];
public:
        PlayerGraphCondition_002D365C( void* p0 );
        virtual bool check();
};

// Constructor 0x002D36B4; allocation extent 0xC.
class PlayerGraphCondition_002D36B4 : public PlayerActionCondition
{
        u32 mUnrecovered[2];
public:
        PlayerGraphCondition_002D36B4( void* p0, void* p1 );
        virtual bool check();
};

// Constructor 0x002D36E0; allocation extent 0x8.
class PlayerGraphCondition_002D36E0 : public PlayerActionCondition
{
        u32 mUnrecovered[1];
public:
        PlayerGraphCondition_002D36E0( void* p0 );
        virtual bool check();
};

// Constructor 0x002D3734; allocation extent 0xC.
class PlayerGraphCondition_002D3734 : public PlayerActionCondition
{
        u32 mUnrecovered[2];
public:
        PlayerGraphCondition_002D3734( void* p0, void* p1 );
        virtual bool check();
};

// Constructor 0x002D375C; allocation extent 0x8.
class PlayerGraphCondition_002D375C : public PlayerActionCondition
{
        u32 mUnrecovered[1];
public:
        PlayerGraphCondition_002D375C( void* p0 );
        virtual bool check();
};

// Constructor 0x002D3798; allocation extent 0x8.
class PlayerGraphCondition_002D3798 : public PlayerActionCondition
{
        u32 mUnrecovered[1];
public:
        PlayerGraphCondition_002D3798( void* p0 );
        virtual bool check();
};

// Constructor 0x002D37BC; allocation extent 0x8.
class PlayerGraphCondition_002D37BC : public PlayerActionCondition
{
        u32 mUnrecovered[1];
public:
        PlayerGraphCondition_002D37BC( void* p0 );
        virtual bool check();
};

// Constructor 0x002D3B24; allocation extent 0x10.
class PlayerGraphCondition_002D3B24 : public PlayerActionCondition
{
        u32 mUnrecovered[3];
public:
        PlayerGraphCondition_002D3B24( void* p0, void* p1, void* p2 );
        virtual bool check();
};

// Constructor 0x002D3B94; allocation extent 0x8.
class PlayerGraphCondition_002D3B94 : public PlayerActionCondition
{
        u32 mUnrecovered[1];
public:
        PlayerGraphCondition_002D3B94( void* p0 );
        virtual bool check();
};

// Constructor 0x002D3BD0; allocation extent 0x8.
class PlayerGraphCondition_002D3BD0 : public PlayerActionCondition
{
        u32 mUnrecovered[1];
public:
        PlayerGraphCondition_002D3BD0( void* p0 );
        virtual bool check();
};

// Constructor 0x002D3C40; allocation extent 0x10.
class PlayerGraphCondition_002D3C40 : public PlayerActionCondition
{
        u32 mUnrecovered[3];
public:
        PlayerGraphCondition_002D3C40( void* p0, void* p1, void* p2 );
        virtual bool check();
};

// Constructor 0x002D3C68; allocation extent 0x8.
class PlayerGraphCondition_002D3C68 : public PlayerActionCondition
{
        u32 mUnrecovered[1];
public:
        PlayerGraphCondition_002D3C68( void* p0 );
        virtual bool check();
};

// Constructor 0x002D3CA0; allocation extent 0x8.
class PlayerGraphCondition_002D3CA0 : public PlayerActionCondition
{
        u32 mUnrecovered[1];
public:
        PlayerGraphCondition_002D3CA0( void* p0 );
        virtual bool check();
};

// Constructor 0x002D3CC4; allocation extent 0x8.
class PlayerGraphCondition_002D3CC4 : public PlayerActionCondition
{
        u32 mUnrecovered[1];
public:
        PlayerGraphCondition_002D3CC4( void* p0 );
        virtual bool check();
};

// Constructor 0x002D3D4C; allocation extent 0x8.
class PlayerGraphCondition_002D3D4C : public PlayerActionCondition
{
        u32 mUnrecovered[1];
public:
        PlayerGraphCondition_002D3D4C( void* p0 );
        virtual bool check();
};

// Constructor 0x002D3F74; allocation extent 0x14.
class PlayerGraphCondition_002D3F74 : public PlayerActionCondition
{
        u32 mUnrecovered[4];
public:
        PlayerGraphCondition_002D3F74( void* p0, void* p1, void* p2 );
        virtual bool check();
};

// Constructor 0x002D3FAC; allocation extent 0x8.
class PlayerGraphCondition_002D3FAC : public PlayerActionCondition
{
        u32 mUnrecovered[1];
public:
        PlayerGraphCondition_002D3FAC( void* p0 );
        virtual bool check();
};

// Constructor 0x002D40E4; allocation extent 0x14.
class PlayerGraphCondition_002D40E4 : public PlayerActionCondition
{
        u32 mUnrecovered[4];
public:
        PlayerGraphCondition_002D40E4( void* p0, void* p1, void* p2 );
        virtual bool check();
};

// Constructor 0x002D50CC; allocation extent 0xC.
class PlayerGraphCondition_002D50CC : public PlayerActionCondition
{
        u32 mUnrecovered[2];
public:
        PlayerGraphCondition_002D50CC( void* p0, void* p1 );
        virtual bool check();
};

// Constructor 0x002D50F4; allocation extent 0x8.
class PlayerGraphCondition_002D50F4 : public PlayerActionCondition
{
        u32 mUnrecovered[1];
public:
        PlayerGraphCondition_002D50F4( void* p0 );
        virtual bool check();
};

// Constructor 0x002D5220; allocation extent 0x14.
class PlayerGraphCondition_002D5220 : public PlayerActionCondition
{
        u32 mUnrecovered[4];
public:
        PlayerGraphCondition_002D5220( void* p0, void* p1, void* p2, void* p3 );
        virtual bool check();
};

// Constructor 0x002D564C; allocation extent 0xC.
class PlayerGraphCondition_002D564C : public PlayerActionCondition
{
        u32 mUnrecovered[2];
public:
        PlayerGraphCondition_002D564C( void* p0, void* p1 );
        virtual bool check();
};

// Constructor 0x002D59E0; allocation extent 0x10.
class PlayerGraphCondition_002D59E0 : public PlayerActionCondition
{
        u32 mUnrecovered[3];
public:
        PlayerGraphCondition_002D59E0( void* p0, void* p1, void* p2 );
        virtual bool check();
};

// Constructor 0x002D5A0C; allocation extent 0x8.
class PlayerGraphCondition_002D5A0C : public PlayerActionCondition
{
        u32 mUnrecovered[1];
public:
        PlayerGraphCondition_002D5A0C( void* p0 );
        virtual bool check();
};

// Constructor 0x002D5AC0; allocation extent 0xC.
class PlayerGraphCondition_002D5AC0 : public PlayerActionCondition
{
        u32 mUnrecovered[2];
public:
        PlayerGraphCondition_002D5AC0( void* p0, void* p1 );
        virtual bool check();
};

// Observed pure virtual address point 0x003D58F4.
class PlayerGraphProviderBase_003D58F4
{
public:
        virtual void slot00() const = 0;
        virtual void slot04() const = 0;
        virtual void slot08() const = 0;
};

// Observed derived address point 0x003CD31C.
class PlayerGraphProvider_003CD31C : public PlayerGraphProviderBase_003D58F4
{
public:
        virtual void slot00() const;
        virtual void slot04() const;
        virtual void slot08() const;
};

// Observed derived address point 0x003CE884.
class PlayerGraphProvider_003CE884 : public PlayerGraphProviderBase_003D58F4
{
public:
        virtual void slot00() const;
        virtual void slot04() const;
        virtual void slot08() const;
};

// Observed pure virtual address point 0x003D58AC.
class PlayerGraphProviderBase_003D58AC
{
public:
        virtual void slot00() const = 0;
        virtual void slot04() const = 0;
        virtual void slot08() const = 0;
        virtual void slot0C() const = 0;
        virtual void slot10() const = 0;
        virtual void slot14() const = 0;
        virtual void slot18() const = 0;
        virtual void slot1C() const = 0;
};

// Observed derived address point 0x003CCC68.
class PlayerGraphProvider_003CCC68 : public PlayerGraphProviderBase_003D58AC
{
public:
        virtual void slot00() const;
        virtual void slot04() const;
        virtual void slot08() const;
        virtual void slot0C() const;
        virtual void slot10() const;
        virtual void slot14() const;
        virtual void slot18() const;
        virtual void slot1C() const;
};

// Observed derived address point 0x003CE240.
class PlayerGraphProvider_003CE240 : public PlayerGraphProviderBase_003D58AC
{
public:
        virtual void slot00() const;
        virtual void slot04() const;
        virtual void slot08() const;
        virtual void slot0C() const;
        virtual void slot10() const;
        virtual void slot14() const;
        virtual void slot18() const;
        virtual void slot1C() const;
};

// Observed derived address point 0x003CBAD4.
class PlayerGraphProvider_003CBAD4 : public PlayerGraphProviderBase_003D58AC
{
public:
        virtual void slot00() const;
        virtual void slot04() const;
        virtual void slot08() const;
        virtual void slot0C() const;
        virtual void slot10() const;
        virtual void slot14() const;
        virtual void slot18() const;
        virtual void slot1C() const;
};

// Observed derived address point 0x003D11C4.
class PlayerGraphProvider_003D11C4 : public PlayerGraphProviderBase_003D58AC
{
public:
        virtual void slot00() const;
        virtual void slot04() const;
        virtual void slot08() const;
        virtual void slot0C() const;
        virtual void slot10() const;
        virtual void slot14() const;
        virtual void slot18() const;
        virtual void slot1C() const;
};

// Observed derived address point 0x003D14CC.
class PlayerGraphProvider_003D14CC : public PlayerGraphProviderBase_003D58AC
{
public:
        virtual void slot00() const;
        virtual void slot04() const;
        virtual void slot08() const;
        virtual void slot0C() const;
        virtual void slot10() const;
        virtual void slot14() const;
        virtual void slot18() const;
        virtual void slot1C() const;
};

// Observed derived address point 0x003D119C.
class PlayerGraphProvider_003D119C : public PlayerGraphProviderBase_003D58AC
{
public:
        virtual void slot00() const;
        virtual void slot04() const;
        virtual void slot08() const;
        virtual void slot0C() const;
        virtual void slot10() const;
        virtual void slot14() const;
        virtual void slot18() const;
        virtual void slot1C() const;
};

// Observed pure virtual address point 0x003D595C.
class PlayerGraphProviderBase_003D595C
{
public:
        virtual void slot00() const = 0;
        virtual void slot04() const = 0;
        virtual void slot08() const = 0;
        virtual void slot0C() const = 0;
};

// Observed derived address point 0x003D0718.
class PlayerGraphProvider_003D0718 : public PlayerGraphProviderBase_003D595C
{
public:
        virtual void slot00() const;
        virtual void slot04() const;
        virtual void slot08() const;
        virtual void slot0C() const;
};

// Observed derived address point 0x003D0BA0.
class PlayerGraphProvider_003D0BA0 : public PlayerGraphProviderBase_003D595C
{
public:
        virtual void slot00() const;
        virtual void slot04() const;
        virtual void slot08() const;
        virtual void slot0C() const;
};

// Observed derived address point 0x003D0344.
class PlayerGraphProvider_003D0344 : public PlayerGraphProviderBase_003D595C
{
public:
        virtual void slot00() const;
        virtual void slot04() const;
        virtual void slot08() const;
        virtual void slot0C() const;
};

// Observed derived address point 0x003D1924.
class PlayerGraphProvider_003D1924 : public PlayerGraphProviderBase_003D595C
{
public:
        virtual void slot00() const;
        virtual void slot04() const;
        virtual void slot08() const;
        virtual void slot0C() const;
};

// Observed derived address point 0x003D1B64.
class PlayerGraphProvider_003D1B64 : public PlayerGraphProviderBase_003D595C
{
public:
        virtual void slot00() const;
        virtual void slot04() const;
        virtual void slot08() const;
        virtual void slot0C() const;
};

// Observed condition address point 0x003D1348.
class PlayerGraphCondition_003D1348 : public PlayerActionCondition
{
        void* mComponent;
public:
        PlayerGraphCondition_003D1348( void* component ) : mComponent( component ) {}
        virtual bool check();
};

// Observed condition address point 0x003D1754.
class PlayerGraphCondition_003D1754 : public PlayerActionCondition
{
        void* mComponent;
public:
        PlayerGraphCondition_003D1754( void* component ) : mComponent( component ) {}
        virtual bool check();
};

// Observed condition address point 0x003D1300.
class PlayerGraphCondition_003D1300 : public PlayerActionCondition
{
        void* mComponent;
public:
        PlayerGraphCondition_003D1300( void* component ) : mComponent( component ) {}
        virtual bool check();
};

// Observed condition address point 0x003D1984.
class PlayerGraphCondition_003D1984 : public PlayerActionCondition
{
        void* mComponent;
public:
        PlayerGraphCondition_003D1984( void* component ) : mComponent( component ) {}
        virtual bool check();
};

// Observed condition address point 0x003D1A2C.
class PlayerGraphCondition_003D1A2C : public PlayerActionCondition
{
        void* mComponent;
public:
        PlayerGraphCondition_003D1A2C( void* component ) : mComponent( component ) {}
        virtual bool check();
};

// Observed condition address point 0x003D1C0C.
class PlayerGraphCondition_003D1C0C : public PlayerActionCondition
{
        void* mComponent;
public:
        PlayerGraphCondition_003D1C0C( void* component ) : mComponent( component ) {}
        virtual bool check();
};

class PlayerGraphTransitionGroup
{
        PlayerActionCondition** mConditions;
        PlayerActionNode** mDestinations;
        unsigned int mCapacity;
        unsigned int mCount;
public:
        PlayerGraphTransitionGroup( unsigned int capacity )
        {
                mConditions = new PlayerActionCondition*[capacity];
                mDestinations = new PlayerActionNode*[capacity];
                mCapacity = capacity;
                mCount = 0;
        }

        inline void append( PlayerActionCondition* condition, PlayerActionNode* destination )
        {
                mConditions[mCount] = condition;
                mDestinations[mCount] = destination;
                ++mCount;
        }

        inline void appendTo( PlayerActionNode* source ) const
        {
                for ( unsigned int i = 0; i < mCount; ++i )
                        source->append( mConditions[i], mDestinations[i] );
        }
};
