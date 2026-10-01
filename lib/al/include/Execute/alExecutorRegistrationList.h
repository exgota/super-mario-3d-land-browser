#pragma once

namespace al
{

class IUseExecutor;
class FunctorBase;

// The independently recovered prefix shared by the registered executor lists.
struct ExecutorRegistrationList
{
        const void* mVirtualFunctionTable;
        const char* mName;
};

extern "C" void fn_001E5F0C( ExecutorRegistrationList* list, IUseExecutor* user );
extern "C" void fn_001DC870( ExecutorRegistrationList* list, const FunctorBase& functor );

} // namespace al
