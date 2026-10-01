#include <Util/alStringUtil.h>

struct CategoryNameValue
{
        const char* name;
        int value;
};

extern "C" const CategoryNameValue staticd( sPlacementCategoryEntries )[ 5 ] = {
        { "Exception", 0 },
        { "Map", 1 },
        { "Entrance", 2 },
        { "Object", 3 },
        { "Event", 4 }
};

// Original semantic name is unknown. The address identifies the retail interval.
extern "C" int fn_00148c7c( const char* name )
{
        const int size = sizeof( sPlacementCategoryEntries ) / sizeof( sPlacementCategoryEntries[ 0 ] );
        for ( int index = 0; index < size; ++index )
        {
                if ( al::isEqualString( sPlacementCategoryEntries[ index ].name, name ) )
                        return sPlacementCategoryEntries[ index ].value;
        }
        return -1;
}
