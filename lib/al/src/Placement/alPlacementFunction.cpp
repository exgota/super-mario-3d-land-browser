#include <LiveActor/alActorInitInfo.h>
#include <Placement/alPlacementFunction.h>
#include <Scene/alSceneObjHolder.h>
#include <Util/alStringUtil.h>

extern "C" const char dat_003A2DE4[];
extern "C" const char dat_003A2DEC[];
extern "C" const char dat_003A2DF4[];
extern "C" const char dat_003A2DAC[];
extern "C" const char dat_003A2DCC[];
extern "C" const char dat_003A2E2C[];
extern "C" const char dat_003A2E50[];

extern "C" const char dat_003A2E58[];

extern "C" bool fn_001E4B3C( sead::Vector3f* out, const al::ByamlIter* iter )
{
        sead::Vector3f value;
        if ( !iter->tryGetFloatByKey( &value.x, "pnt2_x" ) )
                return false;
        if ( !iter->tryGetFloatByKey( &value.y, "pnt2_y" ) )
                return false;
        if ( !iter->tryGetFloatByKey( &value.z, "pnt2_z" ) )
                return false;
        out->x = value.x;
        out->y = value.y;
        out->z = value.z;
        return true;
}

extern "C" bool fn_001E4BD4( sead::Vector3f* out, const al::ByamlIter* iter )
{
        sead::Vector3f value;
        if ( !iter->tryGetFloatByKey( &value.x, "pnt1_x" ) )
                return false;
        if ( !iter->tryGetFloatByKey( &value.y, "pnt1_y" ) )
                return false;
        if ( !iter->tryGetFloatByKey( &value.z, "pnt1_z" ) )
                return false;
        out->x = value.x;
        out->y = value.y;
        out->z = value.z;
        return true;
}

extern "C" bool fn_0023F550( sead::Vector3f* out, const al::ByamlIter* iter )
{
        sead::Vector3f value;
        if ( !iter->tryGetFloatByKey( &value.x, "pnt0_x" ) )
                return false;
        if ( !iter->tryGetFloatByKey( &value.y, "pnt0_y" ) )
                return false;
        if ( !iter->tryGetFloatByKey( &value.z, "pnt0_z" ) )
                return false;
        out->x = value.x;
        out->y = value.y;
        out->z = value.z;
        return true;
}

extern "C" void fn_001BBE0C( float* out, const al::ByamlIter* iter )
{
        iter->tryGetFloatByKey( &out[ 0 ], "InMin" );
        iter->tryGetFloatByKey( &out[ 1 ], "InMax" );
        iter->tryGetFloatByKey( &out[ 2 ], "OutMin" );
        iter->tryGetFloatByKey( &out[ 3 ], "OutMax" );
}

extern "C" const char dat_003A2E14[];
extern "C" const char dat_003A2E1C[];
extern "C" const char dat_003A2E24[];

extern "C" bool fn_00240FA8( sead::Vector3f* out, const al::ByamlIter* iter )
{
        if ( !iter->isValid() )
                return false;
        sead::Vector3f value;
        if ( !iter->tryGetFloatByKey( &value.x, dat_003A2E14 ) )
                return false;
        if ( !iter->tryGetFloatByKey( &value.y, dat_003A2E1C ) )
                return false;
        if ( !iter->tryGetFloatByKey( &value.z, dat_003A2E24 ) )
                return false;
        out->x = value.x;
        out->y = value.y;
        out->z = value.z;
        return true;
}

extern "C" bool fn_001E7964( const al::ByamlIter* iter, const char** name, int* count )
{
        al::ByamlIter sound;
        if ( !iter->tryGetIterByKey( &sound, "Sound" ) )
                return false;
        if ( !sound.tryGetStringByKey( name, "Name" ) )
                return false;
        *count = 4;
        sound.tryGetIntByKey( count, "MaxSound" );
        return true;
}

extern "C" int fn_00192768( const al::ActorInitInfo* info )
{
        int value = -1;
        if ( info->mPlacementInfo->tryGetIntByKey( &value, "ClippingGroupId" ) )
                return value;
        return -1;
}

extern "C" int fn_00257D58( const al::ActorInitInfo* info )
{
        int value = -1;
        if ( info->mPlacementInfo->tryGetIntByKey( &value, "CameraId" ) )
                return value;
        return -1;
}

extern "C" bool fn_00243318( al::ByamlIter* out, const al::ByamlIter* iter, int index )
{
        al::ByamlIter children;
        if ( iter->tryGetIterByKey( &children, "AreaChildren" ) )
                return children.tryGetIterByIndex( out, index );
        return false;
}

extern "C" bool fn_00213474( bool* out, const al::ByamlIter* iter )
{
        return al::tryGetArg( out, *iter, "Arg4" );
}

extern "C" int fn_001D79F4( const al::ByamlIter* iter )
{
        al::ByamlIter children;
        if ( iter->tryGetIterByKey( &children, "AreaChildren" ) )
                return children.getSize();
        return 0;
}

struct ByamlIteratorReference
{
        const al::ByamlIter* iter;
};

struct SceneByamlMetadataStorage
{
        const void* virtualTable;
        const al::ByamlIter* iter;
        const ByamlIteratorReference* footprints;
};

extern "C" const char dat_003AE120[];

static inline bool readSceneMetadataInteger( const char* key, const al::ByamlIter* iter, int* out )
{
        if ( iter == nullptr )
                return false;
        return iter->tryGetIntByKey( out, key );
}

extern "C" bool fn_00185150( int* out )
{
        const SceneByamlMetadataStorage* metadata = reinterpret_cast<const SceneByamlMetadataStorage*>( al::getSceneObj( 15 ) );
        return readSceneMetadataInteger( "StageTimerRestart", metadata->iter, out );
}

extern "C" bool fn_00184FD0( int* out )
{
        const SceneByamlMetadataStorage* metadata = reinterpret_cast<const SceneByamlMetadataStorage*>( al::getSceneObj( 15 ) );
        return readSceneMetadataInteger( dat_003AE120, metadata->iter, out );
}

extern "C" bool fn_0018507C()
{
        int value = 0;
        if ( !al::isExistSceneObj( 15 ) )
                return false;
        if ( !fn_00184FD0( &value ) )
                return false;
        return value < 50;
}

extern "C" const char* fn_0032BD6C( const ByamlIteratorReference* metadata, int index )
{
        al::ByamlIter footprints;
        metadata->iter->tryGetIterByKey( &footprints, "FootPrint" );
        al::ByamlIter entry;
        const char* value;
        if ( !footprints.tryGetIterByIndex( &entry, index ) )
                return nullptr;
        if ( entry.tryGetStringByKey( &value, "AnimName" ) )
                return value;
        return nullptr;
}

extern "C" const char* fn_0032BDEC( const ByamlIteratorReference* metadata, int index )
{
        al::ByamlIter footprints;
        metadata->iter->tryGetIterByKey( &footprints, "FootPrint" );
        al::ByamlIter entry;
        const char* value;
        if ( !footprints.tryGetIterByIndex( &entry, index ) )
                return nullptr;
        if ( entry.tryGetStringByKey( &value, "AnimType" ) )
                return value;
        return nullptr;
}

extern "C" const char* fn_0032BE6C( const ByamlIteratorReference* metadata, int index )
{
        al::ByamlIter footprints;
        metadata->iter->tryGetIterByKey( &footprints, "FootPrint" );
        al::ByamlIter entry;
        const char* value;
        if ( footprints.tryGetIterByIndex( &entry, index ) && entry.tryGetStringByKey( &value, "Material" ) )
                return value;
        return nullptr;
}

extern "C" int fn_0032BEF4( const ByamlIteratorReference* metadata )
{
        if ( metadata->iter == nullptr )
                return 0;
        al::ByamlIter footprints;
        if ( metadata->iter->tryGetIterByKey( &footprints, "FootPrint" ) )
                return footprints.getSize();
        return 0;
}

extern "C" bool fn_0032BF44( const ByamlIteratorReference* metadata )
{
        if ( metadata->iter == nullptr )
                return false;
        al::ByamlIter footprints;
        if ( metadata->iter->tryGetIterByKey( &footprints, "FootPrint" ) )
                return footprints.getSize() > 0;
        return false;
}

extern "C" const char* fn_0032BFA4( const ByamlIteratorReference* metadata, int index )
{
        al::ByamlIter footprints;
        metadata->iter->tryGetIterByKey( &footprints, "FootPrint" );
        al::ByamlIter entry;
        const char* value;
        if ( footprints.tryGetIterByIndex( &entry, index ) && entry.tryGetStringByKey( &value, "Model" ) )
                return value;
        return nullptr;
}

namespace al
{

bool tryGetArg0( float* out, const ActorInitInfo& info )
{
        return tryGetArg( out, getPlacementInfo( info ), "Arg0" );
}

bool tryGetArg1( float* out, const ActorInitInfo& info )
{
        return tryGetArg( out, getPlacementInfo( info ), "Arg1" );
}

bool tryGetArg2( float* out, const ActorInitInfo& info )
{
        return tryGetArg( out, getPlacementInfo( info ), "Arg2" );
}

bool tryGetArg3( float* out, const ActorInitInfo& info )
{
        return tryGetArg( out, getPlacementInfo( info ), "Arg3" );
}

bool tryGetArg4( float* out, const ActorInitInfo& info )
{
        return tryGetArg( out, getPlacementInfo( info ), "Arg4" );
}

bool tryGetArg5( float* out, const ActorInitInfo& info )
{
        return tryGetArg( out, getPlacementInfo( info ), "Arg5" );
}

bool tryGetArg6( float* out, const ActorInitInfo& info )
{
        return tryGetArg( out, getPlacementInfo( info ), "Arg6" );
}

bool tryGetArg7( float* out, const ActorInitInfo& info )
{
        return tryGetArg( out, getPlacementInfo( info ), "Arg7" );
}

bool tryGetArg8( float* out, const ActorInitInfo& info )
{
        return tryGetArg( out, getPlacementInfo( info ), "Arg8" );
}

bool tryGetStringArg( const char** out, const PlacementInfo& info, const char* argName )
{
        const char* arg = dat_003A2DCC;
        if ( info.tryGetStringByKey( &arg, argName ) )
        {
                if ( !isEqualString( dat_003A2E2C, arg ) )
                {
                        *out = arg;
                        return true;
                }
                return false; // this is required to match
        }
        return false;
}

bool tryGetStringArg( const char** out, const ActorInitInfo& info, const char* argName )
{
        return tryGetStringArg( out, getPlacementInfo( info ), argName );
}

bool isPlaced( const ActorInitInfo& info )
{
        return info.mPlacementInfo->isValid();
}

bool tryGetObjectName( const char** out, const al::ActorInitInfo& info )
{
        return tryGetObjectName( out, getPlacementInfo( info ) );
}

bool tryGetObjectName( const char** out, const al::PlacementInfo& info )
{
        return info.tryGetStringByKey( out, dat_003A2DAC );
}

bool isObjectName( const ActorInitInfo& info, const char* objectName )
{
        return isObjectName( getPlacementInfo( info ), objectName );
}

bool isObjectName( const PlacementInfo& info, const char* objectName )
{
        const char* name = nullptr;
        if ( tryGetObjectName( &name, info ) )
                return isEqualString( name, objectName );
        return false;
}

int calcLinkChildNum( const ActorInitInfo& info )
{
        ByamlIter children;
        if ( getPlacementInfo( info ).tryGetIterByKey( &children, "GenerateChildren" ) )
                return children.getSize();
        return 0;
}

#ifdef NON_MATCHING

// 4 bytes less on stack ?
bool tryGetTrans( sead::Vector3f* out, const ActorInitInfo& info )
{
        if ( tryGetTrans( out, getPlacementInfo( info ) ) )
                return true;
        return false;
}
#endif

bool tryGetTrans( sead::Vector3f* out, const PlacementInfo& info )
{
        sead::Vector3f trans;
        bool           valid = info.isValid() && info.tryGetFloatByKey( &trans.x, dat_003A2DE4 ) &&
                     info.tryGetFloatByKey( &trans.y, dat_003A2DEC ) &&
                     info.tryGetFloatByKey( &trans.z, dat_003A2DF4 );
        if ( !valid )
                return valid;

        out->x = trans.x;
        out->y = trans.y;
        out->z = trans.z;
        return true;
}

bool isExistRail( const ActorInitInfo& info )
{
        PlacementInfo rail;
        return tryGetRailIter( &rail, getPlacementInfo( info ) );
}

bool tryGetRailIter( PlacementInfo* out, const PlacementInfo& info )
{
        if ( info.tryGetIterByKey( out, dat_003A2E50 ) )
                return out->isTypeContainer();
        return false;
}

bool getLinksInfoByIndex( PlacementInfo* out, const ActorInitInfo& info, int index )
{
        ByamlIter links;
        if ( getPlacementInfo( info ).tryGetIterByKey( &links, dat_003A2E58 ) )
                return links.tryGetIterByIndex( out, index );
        return false;
}

const char* getLinksActorObjectName( const ActorInitInfo& info, int index )
{
        PlacementInfo placementInfo;
        getLinksInfoByIndex( &placementInfo, info, index );
        const char* objectName = nullptr;
        tryGetObjectName( &objectName, placementInfo );
        return objectName;
}

bool tryGetArg0( bool* out, const ActorInitInfo& info )
{
        return tryGetArg( out, getPlacementInfo( info ), "Arg0" );
}

bool tryGetArg1( bool* out, const ActorInitInfo& info )
{
        return tryGetArg( out, getPlacementInfo( info ), "Arg1" );
}

bool tryGetArg2( bool* out, const ActorInitInfo& info )
{
        return tryGetArg( out, getPlacementInfo( info ), "Arg2" );
}

bool tryGetArg3( bool* out, const ActorInitInfo& info )
{
        return tryGetArg( out, getPlacementInfo( info ), "Arg3" );
}

bool tryGetArg4( bool* out, const ActorInitInfo& info )
{
        return tryGetArg( out, getPlacementInfo( info ), "Arg4" );
}

bool tryGetArg5( bool* out, const ActorInitInfo& info )
{
        return tryGetArg( out, getPlacementInfo( info ), "Arg5" );
}

bool tryGetArg6( bool* out, const ActorInitInfo& info )
{
        return tryGetArg( out, getPlacementInfo( info ), "Arg6" );
}

bool tryGetArg7( bool* out, const ActorInitInfo& info )
{
        return tryGetArg( out, getPlacementInfo( info ), "Arg7" );
}

bool tryGetArg0( float* out, const PlacementInfo& info )
{
        return tryGetArg( out, info, "Arg0" );
}

bool tryGetArg6( float* out, const PlacementInfo& info )
{
        return tryGetArg( out, info, "Arg6" );
}

bool tryGetArg3( bool* out, const PlacementInfo& info )
{
        return tryGetArg( out, info, "Arg3" );
}

bool tryGetArg5( bool* out, const PlacementInfo& info )
{
        return tryGetArg( out, info, "Arg5" );
}

bool tryGetArg7( bool* out, const PlacementInfo& info )
{
        return tryGetArg( out, info, "Arg7" );
}

} // namespace al

namespace alPlacementFunction
{

int getClippingViewId( const al::PlacementInfo& info )
{
        int id = -1;
        if ( !info.tryGetIntByKey( &id, "ViewId" ) )
                return -1;
        return id;
}

} // namespace alPlacementFunction
