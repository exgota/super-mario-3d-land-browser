#include <AreaObj/alAreaObj.h>
#include <AreaObj/alAreaShape.h>
#include <Stage/alStageSwitchKeeper.h>

namespace al
{

#ifdef NON_MATCHING
AreaObj::AreaObj( const char* name )
    : mName( name ), mAreaShape( nullptr ), mStageSwitchKeeper( nullptr ), _10( sead::Matrix34f::ident ),
      _40( nullptr ), _44( -1 ), _48( 1 )
{
}
#endif

StageSwitchKeeper* AreaObj::getStageSwitchKeeper() const
{
        return mStageSwitchKeeper;
}

void AreaObj::initStageSwitchKeeper()
{
        mStageSwitchKeeper = new StageSwitchKeeper;
}

bool AreaObj::isInVolume( const sead::Vector3f& trans ) const
{
        if ( _48 )
                return mAreaShape->isInVolume( trans );
        return false;
}

} // namespace al

#include <Yaml/alByamlIter.h>

static inline int readAreaPlacementInteger( const char* key, const al::AreaObj* area )
{
        const al::ByamlIter* iter = area->getPlacementInfo();
        if ( iter != nullptr )
        {
                int value = -1;
                if ( iter->tryGetIntByKey( &value, key ) )
                        return value;
        }
        return -1;
}

extern "C" int fn_00267C1C( const al::AreaObj* area )
{
        return readAreaPlacementInteger( "Arg0", area );
}

extern "C" int fn_00253420( const al::AreaObj* area )
{
        return readAreaPlacementInteger( "Arg1", area );
}

extern "C" int fn_001CBE10( const al::AreaObj* area )
{
        return readAreaPlacementInteger( "Arg2", area );
}

extern "C" int fn_0024C664( const al::AreaObj* area )
{
        return readAreaPlacementInteger( "Arg3", area );
}

extern "C" int fn_0024C764( const al::AreaObj* area )
{
        return readAreaPlacementInteger( "Arg4", area );
}

extern "C" int fn_0024C724( const al::AreaObj* area )
{
        return readAreaPlacementInteger( "Arg5", area );
}

extern "C" int fn_0024C6E4( const al::AreaObj* area )
{
        return readAreaPlacementInteger( "Arg6", area );
}

extern "C" int fn_0024C6A4( const al::AreaObj* area )
{
        return readAreaPlacementInteger( "Arg7", area );
}

