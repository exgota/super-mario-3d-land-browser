#pragma once

namespace al
{

class ExecuteOrder;
class FunctorBase;

class IUseExecutor;
struct ExecutorRegistrationList;

class ExecuteTableHolderDraw
{
        friend class ExecuteDirector;

private:
        const char* _0;
        int   _4;
        int   _8;
        int   _C;
        int   _10;
        ExecutorRegistrationList** _14;
        int   _18;
        int   _1C;
        ExecutorRegistrationList** _20;
        int   _24;
        int   _28;
        ExecutorRegistrationList** _2C;
        int   _30;
        int   _34;
        ExecutorRegistrationList** _38;
        int   _3C;
        int   _40;
        ExecutorRegistrationList** _44;
        void* _48;
        void* _4C;

public:
        void init( const char*, const ExecuteOrder* order, int );
        void createExecutorListTable();
        bool tryRegisterUser( al::IUseExecutor* p, const char* name );
        bool tryRegisterFunctor( const al::FunctorBase& base, const char* name );

public:
        ExecuteTableHolderDraw();
};

} // namespace al
