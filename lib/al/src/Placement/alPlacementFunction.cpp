#include <LiveActor/alActorInitInfo.h>
#include <Placement/alPlacementFunction.h>
#include <Util/alStringUtil.h>

extern "C" const char dat_003A2DE4[];
extern "C" const char dat_003A2DEC[];
extern "C" const char dat_003A2DF4[];
extern "C" const char dat_003A2DAC[];
extern "C" const char dat_003A2DCC[];
extern "C" const char dat_003A2E2C[];
extern "C" const char dat_003A2E50[];

extern "C" const char dat_003A2E58[];

namespace al
{

bool tryGetArg0( float* out, const ActorInitInfo& info )
{
        return tryGetArg( out, getPlacementInfo( info ), "Arg0" );
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

// Capped packet baseline, reconstructed from the independently named wrapper.
bool tryGetTrans( sead::Vector3f* out, const ActorInitInfo& info )
{
        const PlacementInfo& placement = getPlacementInfo( info );
        sead::Vector3f trans;
        bool valid = placement.isValid();
        bool complete = false;
        if ( valid )
                complete = placement.tryGetFloatByKey( &trans.x, dat_003A2DE4 ) &&
                           placement.tryGetFloatByKey( &trans.y, dat_003A2DEC ) &&
                           placement.tryGetFloatByKey( &trans.z, dat_003A2DF4 );
        if ( !complete )
                return complete;
        out->x = trans.x;
        out->y = trans.y;
        out->z = trans.z;
        return true;
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
