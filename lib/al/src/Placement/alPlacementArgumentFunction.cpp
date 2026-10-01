#include <Placement/alPlacementFunction.h>

namespace al
{

static inline bool readPlacementFloatArgument( const PlacementInfo& info, const char* argName, float* out )
{
        int value;
        if ( info.isValid() && info.tryGetIntByKey( &value, argName ) && value != -1 )
        {
                *out = static_cast<float>( value );
                return true;
        }
        return false;
}

bool tryGetArg( float* out, const PlacementInfo& info, const char* argName )
{
        return readPlacementFloatArgument( info, argName, out );
}

static inline bool readPlacementIntegerArgument( int* out, const PlacementInfo& info, const char* argName )
{
        int value;
        if ( info.isValid() && info.tryGetIntByKey( &value, argName ) && value != -1 )
        {
                *out = value;
                return true;
        }
        return false;
}

bool tryGetArg( int* out, const PlacementInfo& info, const char* argName )
{
        if ( !info.isValid() )
                return false;
        int value;
        if ( !info.tryGetIntByKey( &value, argName ) )
                return false;
        if ( value != -1 )
        {
                *out = value;
                return true;
        }
        return false;
}

bool tryGetArg( bool* out, const PlacementInfo& info, const char* argName )
{
        int value;
        if ( readPlacementIntegerArgument( &value, info, argName ) )
        {
                if ( value == 0 )
                        *out = false;
                else
                        *out = true;
                return true;
        }
        return false;
}

} // namespace al

// Retail AreaObj and actor callers establish this direct-placement reader.
// The original public name is unknown; keep its existing address identity.
extern "C" const char dat_003A2DBC[];

extern "C" bool fn_00252EC4( int* out, const al::PlacementInfo* info )
{
        *out = -1;
        return al::tryGetArg( out, *info, dat_003A2DBC );
}
