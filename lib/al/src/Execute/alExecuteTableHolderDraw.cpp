#include <Execute/alExecuteTableHolderDraw.h>
#include <Execute/alExecutorRegistrationList.h>
#include <Execute/alExecuteOrder.h>
#include <Util/alStringUtil.h>

namespace al
{

// Partial storage shapes corroborated by their independent constructors.
struct ExecutorDrawModelFields : ExecutorRegistrationList
{
        unsigned char mOpaque08[0x10];
        void* mDrawInfo;
};
struct ExecutorDrawLayoutFields : ExecutorRegistrationList
{
        unsigned char mOpaque08[0x0C];
        void* mDrawInfo;
};
struct ExecutorDrawUserFields : ExecutorRegistrationList
{
        int mCapacity;
        int mCount;
        void** mBuffer;
};
struct ExecutorDrawFunctorFields : ExecutorRegistrationList
{
        void* mFunctor;
};

extern "C" bool fn_00242CC4(const ExecuteOrder*, const char*);
extern "C" int fn_00242CCC(const ExecuteOrder*, int, const char*);
extern "C" ExecutorDrawModelFields* fn_001E7E00(ExecutorDrawModelFields*, const char*, int);
extern "C" ExecutorDrawModelFields* fn_001E8AA8(ExecutorDrawModelFields*, const char*, int);
extern "C" ExecutorDrawLayoutFields* fn_001E6078(ExecutorDrawLayoutFields*, const char*, int);
extern "C" ExecutorDrawLayoutFields* fn_001E5FCC(ExecutorDrawLayoutFields*, const char*, int);
extern "C" ExecutorDrawUserFields* fn_001E5F24(ExecutorDrawUserFields*, const char*, int);
extern "C" ExecutorDrawFunctorFields* fn_001DC890(ExecutorDrawFunctorFields*, const char*);

#ifdef NON_MATCHING
void ExecuteTableHolderDraw::init(const char* name, const ExecuteOrder* order, int amount)
{
        _0 = name;
        _10 = amount;
        _14 = new ExecutorRegistrationList*[amount];
        int count = 0;
        for (int index = 0; index < amount; ++index)
                if (isEqualString(order[index]._4, "ActorModelDraw") ||
                    isEqualString(order[index]._4, "ActorModelDrawModelCache"))
                        ++count;
        _18 = count;
        _20 = new ExecutorRegistrationList*[count];
        count = 0;
        for (int index = 0; index < amount; ++index)
                if (isEqualString(order[index]._4, "LayoutDraw") ||
                    isEqualString(order[index]._4, "LayoutDrawBottom"))
                        ++count;
        _24 = count;
        _2C = new ExecutorRegistrationList*[count];
        _30 = fn_00242CCC(order, amount, "Draw");
        _38 = new ExecutorRegistrationList*[_30];
        _3C = fn_00242CCC(order, amount, "Functor");
        _44 = new ExecutorRegistrationList*[_3C];
        for (int index = 0; index < _10; ++index)
        {
                const ExecuteOrder& entry = order[index];
                ExecutorRegistrationList* selected = 0;
                if (fn_00242CC4(&entry, "ActorModelDraw"))
                {
                        ExecutorDrawModelFields* list = new ExecutorDrawModelFields;
                        if (list) list = fn_001E7E00(list, entry._0, entry._8);
                        _20[_1C++] = list;
                        if (_48) list->mDrawInfo = _48;
                        selected = list;
                }
                else if (fn_00242CC4(&entry, "ActorModelDrawModelCache"))
                {
                        ExecutorDrawModelFields* list = new ExecutorDrawModelFields;
                        if (list) list = fn_001E8AA8(list, entry._0, entry._8);
                        _20[_1C++] = list;
                        if (_48) list->mDrawInfo = _48;
                        selected = list;
                }
                else if (fn_00242CC4(&entry, "LayoutDraw"))
                {
                        ExecutorDrawLayoutFields* list = new ExecutorDrawLayoutFields;
                        if (list) list = fn_001E6078(list, entry._0, entry._8);
                        _2C[_28++] = list;
                        if (_4C) list->mDrawInfo = _4C;
                        selected = list;
                }
                else if (fn_00242CC4(&entry, "LayoutDrawBottom"))
                {
                        ExecutorDrawLayoutFields* list = new ExecutorDrawLayoutFields;
                        if (list) list = fn_001E5FCC(list, entry._0, entry._8);
                        _2C[_28++] = list;
                        if (_4C) list->mDrawInfo = _4C;
                        selected = list;
                }
                else if (fn_00242CC4(&entry, "Draw"))
                {
                        ExecutorDrawUserFields* list = new ExecutorDrawUserFields;
                        if (list) list = fn_001E5F24(list, entry._0, entry._8);
                        _38[_34++] = list;
                        selected = list;
                }
                else if (fn_00242CC4(&entry, "Functor"))
                {
                        ExecutorDrawFunctorFields* list = new ExecutorDrawFunctorFields;
                        if (list) list = fn_001DC890(list, entry._0);
                        _44[_40++] = list;
                        selected = list;
                }
                _14[_C++] = selected;
        }
}
#endif

ExecuteTableHolderDraw::ExecuteTableHolderDraw()
    : _0( 0 ), _4( 0 ), _8( 0 ), _C( 0 ), _10( 0 ), _14( 0 ), _18( 0 ), _1C( 0 ), _20( 0 ), _24( 0 ), _28( 0 ), _2C( 0 ),
      _30( 0 ), _34( 0 ), _38( 0 ), _3C( 0 ), _40( 0 ), _44( 0 ), _48( 0 ), _4C( 0 )
{
}

bool ExecuteTableHolderDraw::tryRegisterUser( IUseExecutor* user, const char* name )
{
        bool registered = false;
        for ( int index = 0; index < _34; index++ )
        {
                ExecutorRegistrationList* list = reinterpret_cast<ExecutorRegistrationList**>( _38 )[ index ];
                if ( isEqualString( list->mName, name ) )
                {
                        fn_001E5F0C( list, user );
                        registered = true;
                }
        }
        return registered;
}

bool ExecuteTableHolderDraw::tryRegisterFunctor( const FunctorBase& functor, const char* name )
{
        bool registered = false;
        for ( int index = 0; index < _40; index++ )
        {
                ExecutorRegistrationList* list = reinterpret_cast<ExecutorRegistrationList**>( _44 )[ index ];
                if ( isEqualString( list->mName, name ) )
                {
                        fn_001DC870( list, functor );
                        registered = true;
                }
        }
        return registered;
}

} // namespace al
