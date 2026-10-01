#include <Execute/alExecuteTableHolderUpdate.h>
#include <Execute/alExecutorRegistrationList.h>
#include <Util/alStringUtil.h>

namespace al
{

ExecuteTableHolderUpdate::ExecuteTableHolderUpdate()
    : _0( 0 ), _4( 0 ), _8( 0 ), _C( 0 ), _10( 0 ), _14( 0 ), _18( 0 ), _1C( 0 ), _20( 0 ), _24( 0 ), _28( 0 ), _2C( 0 ),
      _30( 0 ), _34( 0 ), _38( 0 ), _3C( 0 ), _40( 0 )
{
}

bool ExecuteTableHolderUpdate::tryRegisterUser( IUseExecutor* user, const char* name )
{
        bool registered = false;
        for ( int index = 0; index < _30; index++ )
        {
                ExecutorRegistrationList* list = reinterpret_cast<ExecutorRegistrationList**>( _34 )[ index ];
                if ( isEqualString( list->mName, name ) )
                {
                        fn_001E5F0C( list, user );
                        registered = true;
                }
        }
        return registered;
}

bool ExecuteTableHolderUpdate::tryRegisterFunctor( const FunctorBase& functor, const char* name )
{
        bool registered = false;
        for ( int index = 0; index < _3C; index++ )
        {
                ExecutorRegistrationList* list = reinterpret_cast<ExecutorRegistrationList**>( _40 )[ index ];
                if ( isEqualString( list->mName, name ) )
                {
                        fn_001DC870( list, functor );
                        registered = true;
                }
        }
        return registered;
}

} // namespace al
