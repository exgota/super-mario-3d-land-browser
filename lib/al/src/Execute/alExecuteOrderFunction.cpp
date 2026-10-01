#include <Execute/alExecuteOrder.h>
#include <Util/alStringUtil.h>

namespace al
{

extern "C" bool fn_00242CC4( const ExecuteOrder* order, const char* kind )
{
        return isEqualString( order->_4, kind );
}

extern "C" int fn_00242CCC( const ExecuteOrder* order, int count, const char* kind )
{
        int matchingCount = 0;
        for ( int index = 0; index < count; index++ )
        {
                if ( isEqualString( order[ index ]._4, kind ) )
                        matchingCount++;
        }
        return matchingCount;
}

} // namespace al
