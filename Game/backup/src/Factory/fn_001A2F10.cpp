#include <Execute/alObservedExecutorActorList.h>

namespace al
{

extern "C" ObservedExecutorActorList* fn_001A2F10(ObservedExecutorActorList* list)
{
    list = fn_001E2BCC(list);
    list->mBuffer = 0;
    list->mVirtualFunctionTable = &dat_003D0384;
    return list;
}

} // namespace al
