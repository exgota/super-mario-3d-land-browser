#include <Player/PlayerActor.h>

#ifdef NON_MATCHING

#include <Player/Player.h>
#include <Player/PlayerProperty.h>
#include <Player/PlayerActorInitInfo.h>
#include <LiveActor/alActorPoseKeeper.h>
#include <prim/seadSafeString.h>

// Complete guarded reconstruction of 00131ABC. Imported constructors retain
// neutral names until their original classes and complete layouts are known.
namespace nn { namespace math { struct VEC3 { float x, y, z; }; } }
namespace sead {
template <typename T> struct Vector3CalcCtr
{
        static void multScalar( nn::math::VEC3&, const nn::math::VEC3&, float );
};
}

namespace {
struct Quaternion { float x, y, z, w; };
struct InterfaceWords { const void* table; void* target; void* extra; };
inline void*& pointerAt( void* p, unsigned int offset )
{
        return *reinterpret_cast<void**>( static_cast<unsigned char*>( p ) + offset );
}
inline const void* pointerAt( const void* p, unsigned int offset )
{
        return *reinterpret_cast<const void* const*>( static_cast<const unsigned char*>( p ) + offset );
}
inline void* subobject( void* p, unsigned int offset )
{
        return p ? static_cast<unsigned char*>( p ) + offset : 0;
}
inline sead::Vector3f rotateBasis( const Quaternion& q, float x, float y, float z )
{
        float tx = ( q.y * z - q.z * y ) + q.w * x;
        float ty = ( q.z * x - q.x * z ) + q.w * y;
        float tz = ( q.x * y - q.y * x ) + q.w * z;
        float tw = ( -q.x * x - q.y * y ) - q.z * z;
        sead::Vector3f result;
        result.x = ( ( tx * q.w - ty * q.z ) + tz * q.y ) - tw * q.x;
        result.y = ( ( ty * q.w + tx * q.z ) - tz * q.x ) - tw * q.y;
        result.z = ( ( ty * q.x - tx * q.y ) + tz * q.w ) - tw * q.z;
        return result;
}
}

extern "C" {
void fn_002CC99C( PlayerActor* );
void fn_00270AB0( PlayerActor*, const al::ActorInitInfo* );
void fn_0026E674( Quaternion*, float, float, float );
void fn_00279ABC( sead::Vector3f* );
void fn_0026E64C( PlayerActor*, int );
void* fn_0026E5D4( PlayerActor*, const char*, int, const sead::Vector3f*, float );
void* fn_001CD4CC( PlayerActor*, const char*, int, const sead::Vector3f*, float );
void fn_001EB94C( PlayerActor*, const char* );
void fn_001EB9D0( PlayerActor*, const char*, void* );
void* fn_002932B0( unsigned int, const void* );
void* fn_00133D3C( void* );
void* fn_001A9508( void* );
void* fn_001330C4( void* );
void* fn_00155114( void*, void* );
void* fn_001A7140( void*, void* );
void* fn_00182C18( void*, const al::ActorInitInfo*, const PlayerActorInitInfo*, const sead::Vector3f*, const sead::Vector3f*, void*, void* );
void* fn_0019FBE8( void*, void* );
void* fn_0014F554( void*, PlayerActor*, PlayerModelHolder* );
void* fn_0013DE50( void*, PlayerModelHolder* );
void* fn_00181D90( void* );
void* fn_0019FC20( void* );
void* fn_001B911C( void* );
void* fn_002D25EC( void*, int );
void* fn_001B7F40( void* );
void* fn_00198A84( void*, void* );
void* fn_0026E578( void* );
void* fn_001B4CCC( void*, PlayerActor*, const al::ActorInitInfo*, const void*, void* );
void* fn_0030933C( void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void* );
void fn_00308FBC( Player*, void* );
void* fn_0013E070( void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void* );
void* fn_0019F664( void*, void*, void*, const al::ActorInitInfo* );
void* fn_001A49D8( void*, int, void* );
void* fn_001A7FFC( void*, void*, const al::ActorInitInfo*, void* );
void* fn_0018A540( void*, void*, PlayerModelHolder* );
void fn_0018A508( void* );
void* fn_00196CF8( void*, Player*, void*, void*, void*, void* );
void* fn_00182024( void*, void*, void*, void*, void*, void*, void* );
void* fn_001B8574( void*, void*, const al::ActorInitInfo*, void* );
void fn_001314B8( PlayerActor* );
void fn_0026E4C8( PlayerActor*, const al::ActorInitInfo*, const void*, int, const void* );
void fn_001EBDDC( PlayerActor*, const al::ActorInitInfo*, const char* );
void fn_0026E4A4( PlayerActor*, const void*, int, const char* );
void fn_001E622C( PlayerActor*, const al::ActorInitInfo* );
void fn_0028065C( al::LiveActor* );
void fn_0026E2C8( PlayerActor* );
void* fn_001814AC( Player* );
bool fn_001851D0( void* );
void* fn_0015C228( void*, PlayerActor*, const char*, void* );
unsigned int fn_0018511C();
unsigned int fn_00185004( unsigned int );
unsigned int fn_00185040( unsigned int );
const char* fn_001850E0( unsigned int );
const void* fn_00185194( unsigned int );
void fn_0015BC90( void*, const al::ActorInitInfo*, const void*, const char*, unsigned int, unsigned int );
int fn_0028E1E4( void*, const char*, ... );
extern const unsigned char dat_003D0BE8[];
extern const unsigned char dat_003D092C[];
extern const unsigned char dat_003CDE10[];
extern const unsigned char dat_003A8C44[];
extern const unsigned char dat_003A8C54[];
extern const unsigned char dat_003A8C64[];
}

void PlayerActor::initSpecial( const al::ActorInitInfo& info, const PlayerActorInitInfo& playerInfo )
{
        fn_002CC99C( this );
        fn_00270AB0( this, &info );
        sead::Vector3f unit( 1.0f, 1.0f, 1.0f );
        al::setScale( this, unit );
        al::getRotatePtr( this )->x = 0.0f;
        al::getRotatePtr( this )->z = 0.0f;
        sead::Vector3f radians = al::getRotate( this );
        sead::Vector3CalcCtr<float>::multScalar( reinterpret_cast<nn::math::VEC3&>( radians ),
                reinterpret_cast<const nn::math::VEC3&>( radians ), 0.01745329238474369049072265625f );
        Quaternion rotation;
        fn_0026E674( &rotation, radians.x, radians.y, radians.z );
        sead::Vector3f front = rotateBasis( rotation, 0.0f, 0.0f, 1.0f );
        fn_00279ABC( &front );
        sead::Vector3f up = rotateBasis( rotation, 0.0f, 1.0f, 0.0f );
        fn_00279ABC( &up );

        fn_0026E64C( this, 5 );
        sead::Vector3f offset( 0.0f, 0.0f, 0.0f );
        fn_0026E5D4( this, "Foot", 8, &offset, 40.0f );
        offset = sead::Vector3f( 0.0f, 0.0f, 0.0f );
        fn_0026E5D4( this, "Head", 8, &offset, 40.0f );
        offset = sead::Vector3f( 0.0f, 0.0f, 0.0f );
        _90 = fn_0026E5D4( this, "Body", 8, &offset, 50.0f );
        offset = sead::Vector3f( 0.0f, 30.0f, 0.0f );
        fn_0026E5D4( this, "TailAttack", 8, &offset, 120.0f );
        fn_001EB94C( this, "TailAttack" );
        offset = sead::Vector3f( 0.0f, 100.0f, 0.0f );
        fn_001CD4CC( this, "Eye", 8, &offset, 200.0f );
        fn_001EB9D0( this, "Head", &_B8 );
        fn_001EB9D0( this, "Body", &_C4 );
        fn_001EB9D0( this, "Foot", &_D0 );

        void* memory = fn_002932B0( 0x6C, this );
        _78 = memory ? fn_00133D3C( memory ) : 0;
        pointerAt( _78, 0x58 ) = this;
        memory = fn_002932B0( 0x10, this );
        _Dc = memory ? fn_001A9508( memory ) : 0;
        memory = fn_002932B0( 0x14, this );
        _80 = memory ? fn_001330C4( memory ) : 0;
        memory = fn_002932B0( 0x0C, this );
        _FC = memory ? fn_00155114( memory, _80 ) : 0;
        memory = fn_002932B0( 0x0C, this );
        _100 = memory ? fn_001A7140( memory, _80 ) : 0;
        memory = fn_002932B0( 0x50, this );
        if ( memory )
        {
                const sead::Vector3f& rotate = al::getRotate( this );
                const sead::Vector3f& trans = al::getTrans( this );
                memory = fn_00182C18( memory, &info, &playerInfo, &trans, &rotate, _Dc, &_12C );
        }
        mModelHolder = static_cast<PlayerModelHolder*>( memory );
        memory = fn_002932B0( 0x14, this );
        _A8 = memory ? fn_0019FBE8( memory, subobject( mModelHolder, 4 ) ) : 0;
        memory = fn_002932B0( 0x2C, this );
        mPlayerAnimator = static_cast<PlayerAnimator*>( memory ? fn_0014F554( memory, this, mModelHolder ) : 0 );
        memory = fn_002932B0( 8, this );
        _B4 = memory ? fn_0013DE50( memory, mModelHolder ) : 0;
        reinterpret_cast<void ( * )( void*, PlayerModelHolder*, void* )>( static_cast<void**>( pointerAt( _80, 0 ) )[ 9 ] )( _80, mModelHolder, _78 );
        memory = fn_002932B0( 4, this );
        _9C = memory ? fn_00181D90( memory ) : 0;
        memory = fn_002932B0( 0x10, this );
        _F4 = memory ? fn_0019FC20( memory ) : 0;
        pointerAt( _120, 4 ) = mModelHolder;
        memory = fn_002932B0( 8, this );
        void* extra10 = memory ? fn_001B911C( memory ) : 0;
        memory = fn_002932B0( 0x24, this );
        void* sensorList = memory ? fn_002D25EC( memory, 0x10 ) : 0;
        pointerAt( sensorList, 0x1C ) = _90;
        memory = fn_002932B0( 8, this );
        void* extra14 = memory ? fn_001B7F40( memory ) : 0;
        InterfaceWords* receiver = static_cast<InterfaceWords*>( fn_002932B0( 0x0C, this ) );
        if ( receiver )
        {
                receiver->table = dat_003D0BE8;
                receiver->target = 0;
                receiver->extra = 0;
        }
        void* interface092C = fn_002932B0( 4, this );
        if ( interface092C )
                *static_cast<const void**>( interface092C ) = dat_003D092C;
        void* interfaceDE10 = fn_002932B0( 4, this );
        if ( interfaceDE10 )
                *static_cast<const void**>( interfaceDE10 ) = dat_003CDE10;
        memory = fn_002932B0( 0x14, this );
        _108 = memory ? fn_00198A84( memory, interfaceDE10 ) : 0;
        InterfaceWords* adapter44 = static_cast<InterfaceWords*>( fn_002932B0( 8, this ) );
        if ( adapter44 ) { adapter44->table = dat_003A8C44; adapter44->target = 0; }
        InterfaceWords* adapter54 = static_cast<InterfaceWords*>( fn_002932B0( 8, this ) );
        if ( adapter54 ) { adapter54->table = dat_003A8C54; adapter54->target = 0; }
        memory = fn_002932B0( 0x0C, this );
        if ( memory )
        {
                void* modelValue = fn_0026E578( _A8 );
                memory = fn_001B4CCC( memory, this, &info, pointerAt( pointerAt( &playerInfo, 0 ), 0x6C ), modelValue );
        }
        _10C = memory;
        memory = fn_002932B0( 0xF8, this );
        mPlayer = static_cast<Player*>( memory ? fn_0030933C( memory, _78, subobject( _78, 4 ), mPlayerAnimator,
                _B4, _80, &_60, _9C, extra10, sensorList, extra14, adapter44, adapter54, &_64, _F4,
                receiver, interface092C, _108, &_68, _A0, _10C, &_70 ) : 0 );

        sead::Vector3f* position = static_cast<sead::Vector3f*>( pointerAt( mPlayer, 0 ) );
        *position = al::getTrans( this );
        static_cast<PlayerProperty*>( pointerAt( mPlayer, 0 ) )->setFrontVec( front );
        static_cast<PlayerProperty*>( pointerAt( mPlayer, 0 ) )->setUpVec( up );
        Player* player = mPlayer;
        InterfaceWords* actorAdapter = static_cast<InterfaceWords*>( fn_002932B0( 8, this ) );
        if ( actorAdapter ) { actorAdapter->table = dat_003A8C64; actorAdapter->target = this; }
        pointerAt( player, 0x8C ) = actorAdapter;
        fn_00308FBC( mPlayer, &_6C );
        pointerAt( _78, 0x5C ) = pointerAt( mPlayer, 0x14 );
        void* modelValue = fn_0026E578( _A8 );
        memory = fn_002932B0( 0x40, this );
        if ( memory )
        {
                player = mPlayer;
                memory = fn_0013E070( memory, pointerAt( player, 0 ), _80, mPlayerAnimator, pointerAt( player, 0x80 ),
                        modelValue, subobject( mModelHolder, 0x0C ), _78, pointerAt( player, 0x14 ), pointerAt( player, 0x58 ),
                        subobject( _78, 4 ), pointerAt( player, 0xAC ) );
        }
        _94 = memory;
        receiver->target = memory;
        void* outer = fn_002932B0( 0x0C, this );
        if ( outer )
        {
                memory = fn_002932B0( 0x10, this );
                if ( memory )
                        memory = fn_0019F664( memory, pointerAt( mPlayer, 0 ), pointerAt( mModelHolder, 0x18 ), &info );
                outer = fn_001A49D8( outer, 2, memory );
        }
        _E0 = outer;
        adapter44->target = outer;
        memory = fn_002932B0( 0x10, this );
        _E4 = memory ? fn_001A7FFC( memory, pointerAt( mModelHolder, 0x20 ), &info, pointerAt( mPlayer, 0 ) ) : 0;
        adapter54->target = _E4;
        memory = fn_002932B0( 0x18, this );
        _AC = memory ? fn_0018A540( memory, pointerAt( mPlayer, 0x40 ), mModelHolder ) : 0;
        fn_0018A508( _AC );
        memory = fn_002932B0( 0x20, this );
        _98 = memory ? fn_00196CF8( memory, mPlayer, pointerAt( mPlayer, 0x48 ), pointerAt( mPlayer, 0x14 ),
                _AC, pointerAt( mPlayer, 0x58 ) ) : 0;
        pointerAt( _Dc, 4 ) = pointerAt( mPlayer, 0x14 );
        modelValue = fn_0026E578( _A8 );
        memory = fn_002932B0( 0x1C, this );
        _B0 = memory ? fn_00182024( memory, pointerAt( mPlayer, 0x38 ), _AC, pointerAt( mPlayer, 0x54 ),
                _98, pointerAt( mPlayer, 0x40 ), modelValue ) : 0;
        memory = fn_002932B0( 0x28, this );
        _104 = memory ? fn_001B8574( memory, pointerAt( mPlayer, 0 ), &info, _A0 ) : 0;
        pointerAt( _114, 0 ) = mPlayerAnimator;
        pointerAt( _114, 4 ) = _B4;
        pointerAt( _114, 8 ) = _80;
        pointerAt( _114, 0x0C ) = mModelHolder;
        pointerAt( pointerAt( mPlayer, 0xAC ), 0x0C ) = _90;
        fn_001314B8( this );
        fn_0026E4C8( this, &info, pointerAt( pointerAt( &playerInfo, 0 ), 0 ), 5, pointerAt( pointerAt( &playerInfo, 4 ), 0 ) );
        fn_001EBDDC( this, &info, "PlayerHitReaction" );
        fn_0026E4A4( this, pointerAt( pointerAt( &playerInfo, 0 ), 0 ), 0, "ActorHitReactionCtrlPlayer" );
        fn_001E622C( this, &info );
        fn_0028065C( this );
        fn_0026E2C8( this );
        reinterpret_cast<void ( * )( PlayerActor* )>( static_cast<void**>( pointerAt( this, 0 ) )[ 4 ] )( this );
        if ( !fn_001851D0( fn_001814AC( mPlayer ) ) )
                return;
        memory = fn_002932B0( 0x30, &playerInfo );
        _118 = memory ? fn_0015C228( memory, this, "MarioAnimation", 0 ) : 0;
        memory = fn_002932B0( 0x30, &playerInfo );
        _11C = memory ? fn_0015C228( memory, this, "MarioAnimation", _118 ) : 0;
        sead::FixedSafeString<64> alternateName;
        for ( unsigned int i = 0; i < fn_0018511C(); ++i )
        {
                unsigned int arg5 = fn_00185004( i );
                unsigned int arg4 = fn_00185040( i );
                const char* name = fn_001850E0( i );
                const void* value = fn_00185194( i );
                fn_0015BC90( _118, &info, value, name, arg4, arg5 );
                fn_0028E1E4( &alternateName, "%sRaccoonDog", fn_001850E0( i ) );
                arg5 = fn_00185004( i );
                arg4 = fn_00185040( i );
                name = alternateName.cstr();
                value = fn_00185194( i );
                fn_0015BC90( _11C, &info, value, name, arg4, arg5 );
        }
}

#endif
