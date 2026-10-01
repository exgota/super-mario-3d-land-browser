#include <Util/alStringUtil.h>

extern "C" const char* const staticd( sPhotoScenarioNames )[ 12 ] = {
        "PhotoOpening",
        "PhotoWorldInterval01",
        "PhotoWorldInterval02",
        "PhotoWorldInterval03",
        "PhotoWorldInterval04",
        "PhotoWorldInterval05",
        "PhotoWorldInterval06",
        "PhotoWorldInterval07",
        "PhotoEnding",
        "PhotoEndingLuigi",
        "PhotoLuigi",
        "PhotoComplete"
};

// Original semantic name is unknown. The address identifies the retail interval.
extern "C" int fn_00140e94( const char* name )
{
        const int size = sizeof( sPhotoScenarioNames ) / sizeof( sPhotoScenarioNames[ 0 ] );
        for ( int index = 0; index < size; ++index )
        {
                if ( al::isEqualString( name, sPhotoScenarioNames[ index ] ) )
                        return index;
        }
        return -1;
}
