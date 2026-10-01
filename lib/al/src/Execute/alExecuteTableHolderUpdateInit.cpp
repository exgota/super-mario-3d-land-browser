#include <Execute/alExecuteTableHolderUpdate.h>
#include <Execute/alExecuteOrder.h>
#include <Util/alStringUtil.h>

namespace al
{

struct ExecutorStorage
{
        const void* mVirtualFunctionTable;
        const char* mName;
        int mCapacity;
        int mSize;
        void** mBuffer;
};

struct FunctorExecutorStorage
{
        const void* mVirtualFunctionTable;
        const char* mName;
        void* mFirstEntry;
};

class ExecutorActorMovement : public ExecutorStorage
{
public:
        ExecutorActorMovement( const char* name, int capacity );
};

class ExecutorActorCalcAnim : public ExecutorStorage
{
public:
        ExecutorActorCalcAnim( const char* name, int capacity );
};

class ExecutorActorMovementCalcAnim : public ExecutorStorage
{
public:
        ExecutorActorMovementCalcAnim( const char* name, int capacity );
};

class ExecutorLayoutUpdate : public ExecutorStorage
{
public:
        ExecutorLayoutUpdate( const char* name, int capacity );
};

class ExecutorExecute : public ExecutorStorage
{
public:
        ExecutorExecute( const char* name, int capacity );
};

class ExecutorFunctor : public FunctorExecutorStorage
{
public:
        ExecutorFunctor( const char* name );
};

extern "C" bool fn_00242CC4( const ExecuteOrder* order, const char* kind );
extern "C" int fn_00242CCC( const ExecuteOrder* order, int count, const char* kind );

inline int countActorExecutors( const ExecuteOrder* order, int count )
{
        int actorCount = 0;
        for ( int index = 0; index < count; index++ )
        {
                if ( isEqualString( order[ index ]._4, "ActorMovement" ) ||
                     isEqualString( order[ index ]._4, "ActorCalcAnim" ) ||
                     isEqualString( order[ index ]._4, "ActorMovementCalcAnim" ) )
                        actorCount++;
        }
        return actorCount;
}

#ifdef NON_MATCHING
void ExecuteTableHolderUpdate::init( const ExecuteOrder* order, int count )
{
        _C = count;
        _10 = reinterpret_cast<int>( new void*[ count ] );
        int actorCount = countActorExecutors( order, count );
        _14 = actorCount;
        _1C = reinterpret_cast<int>( new void*[ actorCount ] );
        _20 = fn_00242CCC( order, count, "LayoutUpdate" );
        _28 = reinterpret_cast<int>( new void*[ _20 ] );
        _2C = fn_00242CCC( order, count, "Execute" );
        _34 = reinterpret_cast<int>( new void*[ _2C ] );
        _38 = fn_00242CCC( order, count, "Functor" );
        _40 = reinterpret_cast<int>( new void*[ _38 ] );
        for ( int index = 0; index < _C; index++ )
        {
                const ExecuteOrder* entry = order + index;
                void* executor = nullptr;
                if ( fn_00242CC4( entry, "ActorMovement" ) )
                {
                        ExecutorActorMovement* storage = new ExecutorActorMovement( entry->_0, entry->_8 );
                        executor = storage;
                        reinterpret_cast<void**>( _1C )[ _18 ] = executor;
                        _18++;
                }
                else if ( fn_00242CC4( entry, "ActorCalcAnim" ) )
                {
                        ExecutorActorCalcAnim* storage = new ExecutorActorCalcAnim( entry->_0, entry->_8 );
                        executor = storage;
                        reinterpret_cast<void**>( _1C )[ _18 ] = executor;
                        _18++;
                }
                else if ( fn_00242CC4( entry, "ActorMovementCalcAnim" ) )
                {
                        executor = new ExecutorActorMovementCalcAnim( entry->_0, entry->_8 );
                        reinterpret_cast<void**>( _1C )[ _18 ] = executor;
                        _18++;
                }
                else if ( fn_00242CC4( entry, "LayoutUpdate" ) )
                {
                        ExecutorLayoutUpdate* storage = new ExecutorLayoutUpdate( entry->_0, entry->_8 );
                        reinterpret_cast<void**>( _28 )[ _24 ] = storage;
                        _24++;
                        executor = storage;
                }
                else if ( fn_00242CC4( entry, "Execute" ) )
                {
                        executor = new ExecutorExecute( entry->_0, entry->_8 );
                        reinterpret_cast<void**>( _34 )[ _30 ] = executor;
                        _30++;
                }
                else if ( fn_00242CC4( entry, "Functor" ) )
                {
                        ExecutorFunctor* storage = new ExecutorFunctor( entry->_0 );
                        reinterpret_cast<void**>( _40 )[ _3C ] = storage;
                        _3C++;
                        executor = storage;
                }
                reinterpret_cast<void**>( _10 )[ _8 ] = executor;
                _8++;
        }
}

#endif

} // namespace al
