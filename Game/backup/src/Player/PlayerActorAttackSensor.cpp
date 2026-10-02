#include <Player/PlayerActor.h>

#ifdef NON_MATCHING

#include <Player/PlayerTrigger.h>
#include <HitSensor/alHitSensor.h>
#include <LiveActor/alHitSensorFunction.h>
#include <LiveActor/alActorPoseKeeper.h>
#include <prim/seadSafeString.h>
#include <math.h>
#include <stddef.h>

// The complete 00130438 root is still NonMatching. The private views below
// record only offsets observed in this root and the 002768AC constructor.
// They do not change PlayerActor, Player, or any accepted shared header.
namespace nn { namespace math { struct VEC3 { float x, y, z; }; } }
namespace sead {
template <typename T> struct Vector3CalcCtr
{
        static void sub( nn::math::VEC3&, const nn::math::VEC3&, const nn::math::VEC3& );
};
}
namespace al {
bool sendMsgPlayerInvincibleAttack( HitSensor*, HitSensor* );
bool sendMsgPlayerTailAttack( HitSensor*, HitSensor* );
bool sendMsgPlayerKick( HitSensor*, HitSensor* );
bool sendMsg9( HitSensor*, HitSensor* );
}

extern "C" {
void* fn_00173F3C( void* );
bool fn_0026EAEC( void* );
bool fn_0026EC10( al::HitSensor* );
bool fn_002785F8( al::HitSensor* );
bool fn_0026EAAC( void* );
bool fn_0026EA74( al::HitSensor* );
bool fn_001DEA2C( al::HitSensor*, al::HitSensor* );
bool fn_0026EA04( al::HitSensor* );
void fn_001B46B0( void*, al::HitSensor*, float );
bool fn_001E7274( al::HitSensor*, al::HitSensor* );
bool fn_001E95D0( al::HitSensor*, al::HitSensor* );
bool fn_0027D5C4( sead::Vector3f* );
bool fn_0026E9F0( al::HitSensor* );
bool fn_00326B70( PlayerActor* );
bool fn_001E79F0( al::HitSensor*, al::HitSensor* );
bool fn_0026E9D4( al::HitSensor*, al::HitSensor* );
bool fn_001E8824( al::HitSensor*, al::HitSensor* );
bool fn_0026E99C( al::HitSensor*, al::HitSensor* );
bool fn_0026E980( al::HitSensor*, al::HitSensor* );
bool fn_001E5788( al::HitSensor*, al::HitSensor* );
}

namespace {
struct Interface { void** entries; };
struct PropertyView
{
        sead::Vector3f position;
        sead::Vector3f front;
        sead::Vector3f up;
        sead::Vector3f velocity;
        unsigned char unknown30[ 0x3C ];
        sead::Vector3f gravity;
};
struct FigureView
{
        int form;
        unsigned char unknown04[ 0x0C ];
        unsigned char state10;
};
struct PlayerView
{
        PropertyView* property;
        unsigned char unknown04[ 0x1C ];
        PlayerTrigger* trigger;
        unsigned char unknown24[ 0x14 ];
        Interface* interface38;
        void* unknown3C;
        FigureView* figure;
        void* unknown44;
        void* graph;
        unsigned char unknown4C[ 8 ];
        Interface* interface54;
        unsigned char unknown58[ 0x54 ];
        Interface* interfaceAC;
};
struct ActorView
{
        unsigned char unknown00[ 0x74 ];
        PlayerView* player;
        void* unknown78;
        Interface* animator;
        unsigned char unknown80[ 8 ];
        void* penetration;
        unsigned char unknown8C[ 0x0C ];
        Interface* status;
        unsigned char unknown9C[ 0x60 ];
        Interface* reactionFC;
        Interface* reaction100;
        unsigned char unknown104[ 0x20 ];
        al::HitSensor* selectedSensor;
        int selectedIndex;
        unsigned char unknown12C[ 0x0E ];
        unsigned char enableMessage13A;
        unsigned char unknown13B[ 5 ];
        Interface* interface140;
};
static_assert( offsetof( PropertyView, gravity ) == 0x6C, "property gravity" );
static_assert( offsetof( PlayerView, interfaceAC ) == 0xAC, "player interface" );
static_assert( offsetof( ActorView, selectedSensor ) == 0x124, "sensor selection" );
static_assert( offsetof( ActorView, interface140 ) == 0x140, "actor interface" );

inline bool query( Interface* object, unsigned int slot )
{
        return reinterpret_cast<bool ( * )( Interface* )>( object->entries[ slot ] )( object );
}
inline bool animationQuery( Interface* object, const sead::SafeString& name )
{
        return reinterpret_cast<bool ( * )( Interface*, const sead::SafeString& )>( object->entries[ 6 ] )( object, name );
}
inline void playAnimation( Interface* object, const sead::SafeString& name )
{
        reinterpret_cast<void ( * )( Interface*, const sead::SafeString& )>( object->entries[ 10 ] )( object, name );
}
inline void react( Interface* object, const sead::Vector3f& position )
{
        reinterpret_cast<void ( * )( Interface*, const sead::Vector3f& )>( object->entries[ 0 ] )( object, position );
}
inline sead::Vector3f difference( const sead::Vector3f& left, const sead::Vector3f& right )
{
        sead::Vector3f result;
        sead::Vector3CalcCtr<float>::sub( reinterpret_cast<nn::math::VEC3&>( result ),
                reinterpret_cast<const nn::math::VEC3&>( left ), reinterpret_cast<const nn::math::VEC3&>( right ) );
        return result;
}
inline float dot( const sead::Vector3f& a, const sead::Vector3f& b )
{
        return ( a.x * b.x + a.y * b.y ) + a.z * b.z;
}
inline bool isNan( float value )
{
        // Retail's unordered distance comparison retains the previous sensor.
        // ARMCC's default floating mode removes the unordered arm of !(a > b),
        // so classify the IEEE binary32 representation explicitly.
        union FloatBits { float value; unsigned int bits; } number;
        number.value = value;
        return ( number.bits & 0x7FFFFFFFU ) > 0x7F800000U;
}
inline bool statusPair( Interface* status )
{
        return query( status, 7 ) || query( status, 8 );
}
inline bool downward( ActorView* actor )
{
        PropertyView* p = actor->player->property;
        const sead::Vector3f& gravity = p->gravity;
        const sead::Vector3f& velocity = p->velocity;
        return query( actor->status, 11 ) && dot( gravity, velocity ) < -20.0f;
}
inline void setTrigger( ActorView* actor, int value )
{
        actor->player->trigger->set( static_cast<PlayerTrigger::ESensorTrigger>( value ) );
}
}

void PlayerActor::attackSensor( al::HitSensor* me, al::HitSensor* other )
{
        ActorView* actor = reinterpret_cast<ActorView*>( this );
        if ( fn_0026EAEC( fn_00173F3C( actor->player->graph ) ) )
                return;
        if ( fn_0026EC10( me ) )
        {
                if ( !al::isSensorEnemy( other ) && !al::isSensorMapObj( other ) && !fn_002785F8( other ) )
                        return;
                const sead::Vector3f& center = me->getPos();
                if ( actor->selectedSensor )
                {
                        sead::Vector3f previous = difference( actor->selectedSensor->getPos(), center );
                        sead::Vector3f current = difference( other->getPos(), center );
                        float previousLength = dot( previous, previous );
                        float currentLength = dot( current, current );
                        if ( !( previousLength > currentLength ) || isNan( previousLength ) || isNan( currentLength ) )
                                return;
                }
                actor->selectedSensor = other;
                actor->selectedIndex = -1;
                return;
        }

        if ( al::isSensorName( me, "TailAttack" ) )
        {
                if ( !fn_0026EAAC( actor->player->figure ) )
                        return;
                sead::Vector3f delta = difference( other->getPos(), me->getPos() );
                float projection = dot( delta, actor->player->property->gravity );
                if ( projection <= 0.0f )
                        projection = -projection;
                if ( other->getRadius() + 30.0f <= projection )
                        return;
        }
        if ( actor->player->figure->form == 6 && al::sendMsgPlayerInvincibleAttack( other, me ) )
                return;
        if ( query( actor->player->interface54, 0 ) && al::sendMsgPlayerInvincibleAttack( other, me ) )
        {
                if ( al::isSensorEnemy( other ) )
                {
                        Interface* reaction = actor->reaction100;
                        react( reaction, al::getTrans( this ) );
                }
                return;
        }
        if ( !fn_0026EA74( other ) )
                return;
        if ( al::isSensorName( me, "TailAttack" ) )
        {
                if ( ( ( !actor->player->trigger->isOn( static_cast<PlayerTrigger::ESensorTrigger>( 0 ) ) &&
                         !actor->player->trigger->isOn( static_cast<PlayerTrigger::ECollisionTrigger>( 4 ) ) ) ||
                       query( actor->player->interface38, 7 ) || query( actor->player->interfaceAC, 1 ) ) &&
                     actor->player->figure->state10 != 2 )
                        al::sendMsgPlayerTailAttack( other, me );
        }
        if ( !fn_0026EA74( other ) )
                return;
        if ( al::isSensorMapObj( other ) && actor->enableMessage13A && fn_001DEA2C( other, me ) )
                return;
        if ( al::isSensorName( me, "TailAttack" ) || !fn_0026EA74( other ) )
                return;
        if ( fn_0026EA04( other ) )
        {
                float radius = me->getRadius() + other->getRadius();
                sead::Vector3f delta = difference( me->getPos(), other->getPos() );
                fn_001B46B0( actor->penetration, other, radius - sqrtf( dot( delta, delta ) ) );
        }
        if ( query( actor->interface140, 0 ) )
        {
                sead::Vector3f delta = difference( other->getPos(), me->getPos() );
                if ( !( 0.0f > dot( delta, actor->player->property->front ) ) && fn_001E7274( other, me ) )
                        return;
        }
        if ( al::isSensorName( me, "Foot" ) )
        {
                if ( al::isSensorEnemy( other ) && query( actor->status, 11 ) && downward( actor ) && fn_001E95D0( other, me ) )
                {
                        setTrigger( actor, 1 );
                        return;
                }
                sead::Vector3f delta = difference( me->getPos(), other->getPos() );
                fn_0027D5C4( &delta );
                if ( dot( delta, actor->player->property->up ) > 0.34202015399932861328125f )
                {
                        if ( al::isSensorEnemy( other ) || fn_0026E9F0( other ) || al::isSensorMapObj( other ) )
                        {
                                bool secondaryAccepted = false;
                                if ( fn_00326B70( this ) )
                                {
                                        if ( fn_001E79F0( other, me ) )
                                        {
                                                setTrigger( actor, 1 );
                                                return;
                                        }
                                        secondaryAccepted = fn_0026E9D4( other, me );
                                }
                                else if ( query( actor->status, 11 ) )
                                {
                                        if ( downward( actor ) )
                                        {
                                                if ( fn_001E8824( other, me ) )
                                                {
                                                        setTrigger( actor, 1 );
                                                        return;
                                                }
                                                secondaryAccepted = al::sendMsg9( other, me );
                                        }
                                }
                                else if ( !animationQuery( actor->animator, sead::SafeString( "HipDropStart" ) ) &&
                                          !animationQuery( actor->animator, sead::SafeString( "SwimHipDropStart" ) ) && fn_0026E99C( other, me ) )
                                {
                                        setTrigger( actor, 1 );
                                        if ( !statusPair( actor->status ) && !al::isSensorMapObj( other ) )
                                        {
                                                Interface* reaction = actor->reactionFC;
                                                react( reaction, al::getTrans( this ) );
                                        }
                                        return;
                                }
                                if ( secondaryAccepted )
                                {
                                        if ( statusPair( actor->status ) )
                                        {
                                                setTrigger( actor, 1 );
                                                return;
                                        }
                                        if ( !statusPair( actor->status ) && !al::isSensorMapObj( other ) )
                                        {
                                                Interface* reaction = actor->reactionFC;
                                                react( reaction, al::getTrans( this ) );
                                        }
                                }
                        }
                        else if ( fn_002785F8( other ) )
                        {
                                bool accepted;
                                if ( fn_00326B70( this ) )
                                        accepted = fn_0026E9D4( other, me );
                                else if ( query( actor->status, 11 ) )
                                        accepted = downward( actor ) && al::sendMsg9( other, me );
                                else
                                        accepted = !animationQuery( actor->animator, sead::SafeString( "HipDropStart" ) ) &&
                                                   !animationQuery( actor->animator, sead::SafeString( "SwimHipDropStart" ) ) && fn_0026E99C( other, me );
                                if ( accepted )
                                {
                                        setTrigger( actor, 1 );
                                        return;
                                }
                        }
                }
                if ( !fn_0026EA74( other ) )
                        return;
                if ( query( actor->status, 11 ) && fn_0026E980( other, me ) )
                        return;
                if ( al::sendMsgPlayerKick( other, me ) )
                {
                        if ( query( actor->animator, 12 ) )
                                playAnimation( actor->animator, sead::SafeString( "Kick" ) );
                        return;
                }
        }
        if ( al::isSensorName( me, "Head" ) )
        {
                sead::Vector3f delta = difference( other->getPos(), me->getPos() );
                fn_0027D5C4( &delta );
                if ( dot( delta, actor->player->property->up ) > 0.34202015399932861328125f && fn_001E5788( other, me ) )
                {
                        setTrigger( actor, 2 );
                        PropertyView* p = actor->player->property;
                        if ( dot( p->velocity, p->gravity ) > 0.0f )
                                p->velocity.y = 0.0f;
                        return;
                }
                if ( query( actor->status, 11 ) && fn_0026E980( other, me ) )
                        return;
        }
        if ( al::isSensorName( me, "Body" ) && query( actor->status, 11 ) )
                fn_0026E980( other, me );
}

#endif
