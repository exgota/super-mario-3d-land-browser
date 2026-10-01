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

} // namespace al
