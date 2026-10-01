#include "Enemy/BombHei.h"
#include <LiveActor/alActorPoseKeeper.h>
#include <Nerve/alNerveFunction.h>
#include <Audio/alAudioKeeper.h>
#include <prim/seadSafeString.h>

#ifdef NON_MATCHING

extern "C" bool fn_00279ED4( const al::LiveActor*, int );
extern "C" void fn_00337474( const al::LiveActor*, sead::Quatf* );
extern "C" void fn_0024E9F8( al::IUseAudioKeeper*, const sead::SafeString&, int );
extern "C" void fn_00271330( al::LiveActor*, const char* );
extern "C" void fn_00271300( sead::Vector3f*, float );
extern "C" bool fn_00271294( al::LiveActor* );
extern "C" const al::Nerve dat_003F1E8C;
extern "C" const al::Nerve dat_003F1E88;
extern "C" const al::Nerve dat_003F1E84;
extern "C" const al::Nerve dat_003F1E80;
extern "C" const al::Nerve dat_003F1E74;
extern "C" const al::Nerve dat_003F1E78;
extern "C" const al::Nerve dat_003F1E7C;

inline sead::Quatf multiplyQuaternionVector( const sead::Quatf& rotation, const sead::Vector3f& vector )
{
        return sead::Quatf(
                rotation.y * vector.z - rotation.z * vector.y + rotation.w * vector.x,
                rotation.z * vector.x - rotation.x * vector.z + rotation.w * vector.y,
                rotation.x * vector.y - rotation.y * vector.x + rotation.w * vector.z,
                -rotation.x * vector.x - rotation.y * vector.y - rotation.z * vector.z );
}

inline void rotateVectorByQuaternion( sead::Vector3f& vector, const sead::Quatf& rotation )
{
        const sead::Quatf product = multiplyQuaternionVector( rotation, vector );
        vector.x = product.x * rotation.w - product.y * rotation.z + product.z * rotation.y - product.w * rotation.x;
        vector.y = product.y * rotation.w + product.x * rotation.z - product.z * rotation.x - product.w * rotation.y;
        vector.z = product.y * rotation.x - product.x * rotation.y + product.z * rotation.w - product.w * rotation.z;
}

// NON_MATCHING: quaternion register allocation remains under investigation.
void BombHei::control()
{
        if ( fn_00279ED4( this, 0 ) )
        {
                sead::Quatf rotation;
                fn_00337474( this, &rotation );
                rotateVectorByQuaternion( *al::getFrontPtr( this ), rotation );
        }
        if ( _70 > 0 )
                --_70;
        if ( _84 )
        {
                fn_0024E9F8( this, "SeEmLvBombHeiFuse", 2 );
                if ( _74 <= 90 )
                {
                        fn_0024E9F8( this, "SeEmLvBombHeiBlinkFast", 2 );
                        if ( _74 == 90 )
                                fn_00271330( this, "Blink" );
                }
                if ( --_74 <= 0 )
                {
                        _84 = false;
                        if ( !al::isNerve( this, &dat_003F1E8C ) && !al::isNerve( this, &dat_003F1E88 ) )
                        {
                                al::setNerve( this, &dat_003F1E8C );
                                return;
                        }
                }
        }
        if ( al::isNerve( this, &dat_003F1E84 ) || al::isNerve( this, &dat_003F1E80 ) ||
             al::isNerve( this, &dat_003F1E74 ) || al::isNerve( this, &dat_003F1E78 ) ||
             al::isNerve( this, &dat_003F1E7C ) )
        {
                fn_00271300( al::getFrontPtr( this ), _88 );
                // Retail uses signed integer ordering of the IEEE-754 word. In
                // particular, negative NaNs take the +0 path.
                union { float value; int bits; } rate;
                rate.value = _88;
                _88 = rate.bits < 0x40600000 ? 0.0f : _88 * 0.96f;
        }
        fn_00271294( this );
}

#endif
