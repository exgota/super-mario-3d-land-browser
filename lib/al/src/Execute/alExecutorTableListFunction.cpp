#include <Execute/alExecuteTableHolderDraw.h>
#include <Execute/alExecuteTableHolderUpdate.h>

namespace al
{

struct ExecutorTableRecord;

struct ExecutorTableMethods
{
        const void* mUnresolvedMethods[ 3 ];
        bool ( *hasRegisteredEntries )( ExecutorTableRecord* record );
};

struct ExecutorTableRecord
{
        const ExecutorTableMethods* mMethods;
};

extern "C" void fn_001E5A68( ExecutorTableRecord* executor );
extern "C" void fn_001E7328( ExecutorTableRecord* executor );

void ExecuteTableHolderUpdate::createExecutorListTable()
{
        for ( int index = 0; index < _18; index++ )
                fn_001E5A68( reinterpret_cast<ExecutorTableRecord**>( _1C )[ index ] );
        _0 = 0;
        for ( int index = 0; index < _8; index++ )
        {
                ExecutorTableRecord* executor = reinterpret_cast<ExecutorTableRecord**>( _10 )[ index ];
                if ( executor->mMethods->hasRegisteredEntries( executor ) )
                        _0++;
        }
        _4 = reinterpret_cast<int>( new ExecutorTableRecord*[ _0 ] );
        int selectedIndex = 0;
        for ( int index = 0; index < _8; index++ )
        {
                ExecutorTableRecord* executor = reinterpret_cast<ExecutorTableRecord**>( _10 )[ index ];
                if ( executor->mMethods->hasRegisteredEntries( executor ) )
                        reinterpret_cast<ExecutorTableRecord**>( _4 )[ selectedIndex++ ] = executor;
        }
}

void ExecuteTableHolderDraw::createExecutorListTable()
{
        for ( int index = 0; index < _1C; index++ )
                fn_001E7328( reinterpret_cast<ExecutorTableRecord**>( _20 )[ index ] );
        _4 = 0;
        for ( int index = 0; index < _C; index++ )
        {
                ExecutorTableRecord* executor = reinterpret_cast<ExecutorTableRecord**>( _14 )[ index ];
                if ( executor->mMethods->hasRegisteredEntries( executor ) )
                        _4++;
        }
        _8 = reinterpret_cast<int>( new ExecutorTableRecord*[ _4 ] );
        int selectedIndex = 0;
        for ( int index = 0; index < _C; index++ )
        {
                ExecutorTableRecord* executor = reinterpret_cast<ExecutorTableRecord**>( _14 )[ index ];
                if ( executor->mMethods->hasRegisteredEntries( executor ) )
                        reinterpret_cast<ExecutorTableRecord**>( _8 )[ selectedIndex++ ] = executor;
        }
}

} // namespace al
