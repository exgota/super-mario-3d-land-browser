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

extern "C" bool fn_00242CC4(const ExecuteOrder* order,
const char* kind);
extern "C" int fn_00242CCC(const ExecuteOrder* order, int count,
const char* kind);

extern "C" ExecutorStorage* fn_001E43D4(
ExecutorStorage*, const char*, int);
extern "C" ExecutorStorage* fn_001E43BC(
ExecutorStorage*, const char*, int);
extern "C" ExecutorStorage* fn_001E85A4(
ExecutorStorage*, const char*, int);
extern "C" ExecutorStorage* fn_001E3BC8(
ExecutorStorage*, const char*, int);
extern "C" ExecutorStorage* fn_001E7618(
ExecutorStorage*, const char*, int);
extern "C" FunctorExecutorStorage* fn_001DC890(
FunctorExecutorStorage*, const char*);

inline int countActorExecutors(const ExecuteOrder* order, int count)
{
int actorCount = 0;
for (int index = 0; index < count; ++index)
{
const ExecuteOrder* entry = order + index;

if (isEqualString(entry->_4, "ActorMovement"))
goto count_actor;
if (isEqualString(entry->_4, "ActorCalcAnim"))
goto count_actor;
if (!isEqualString(entry->_4, "ActorMovementCalcAnim"))
goto next_count;

count_actor:
++actorCount;
next_count:
;
}

return actorCount;
}

#ifdef NON_MATCHING
void ExecuteTableHolderUpdate::init(const ExecuteOrder* order, int count)
{
_C = count;
_10 = reinterpret_cast<int>(new void*[count]);

int actorCount = countActorExecutors(order, count);

_14 = actorCount;
_1C = reinterpret_cast<int>(new void*[actorCount]);
_20 = fn_00242CCC(order, count, "LayoutUpdate");
_28 = reinterpret_cast<int>(new void*[_20]);
_2C = fn_00242CCC(order, count, "Execute");
_34 = reinterpret_cast<int>(new void*[_2C]);
_38 = fn_00242CCC(order, count, "Functor");
_40 = reinterpret_cast<int>(new void*[_38]);

for (int index = 0; index < _C; ++index)
{
const ExecuteOrder* entry = order + index;
void* executor = 0;
void* pending; // Read only on paths which assigned it.

if (!fn_00242CC4(entry, "ActorMovement"))
goto try_calc_anim;
{
ExecutorStorage* value = new ExecutorStorage;
if (value)
value = fn_001E43D4(value, entry->_0, entry->_8);
pending = value;
}
goto append_actor;

try_calc_anim:
if (!fn_00242CC4(entry, "ActorCalcAnim"))
goto try_combined;
{
ExecutorStorage* value = new ExecutorStorage;
if (value)
value = fn_001E43BC(value, entry->_0, entry->_8);
pending = value;
}

append_actor:
reinterpret_cast<void**>(_1C)[_18] = pending;
++_18;
executor = pending;
goto append_all;

try_combined:
if (!fn_00242CC4(entry, "ActorMovementCalcAnim"))
goto try_layout;
{
ExecutorStorage* value = new ExecutorStorage;
executor = value ? fn_001E85A4(value, entry->_0, entry->_8) : 0;
}
reinterpret_cast<void**>(_1C)[_18] = executor;
++_18;
goto append_all;

try_layout:
if (!fn_00242CC4(entry, "LayoutUpdate"))
goto try_execute;
{
ExecutorStorage* value = new ExecutorStorage;
if (value)
value = fn_001E3BC8(value, entry->_0, entry->_8);
pending = value;
}
reinterpret_cast<void**>(_28)[_24] = pending;
++_24;
goto assign_common;

try_execute:
if (!fn_00242CC4(entry, "Execute"))
goto try_functor;
{
ExecutorStorage* value = new ExecutorStorage;
executor = value ? fn_001E7618(value, entry->_0, entry->_8) : 0;
}
reinterpret_cast<void**>(_34)[_30] = executor;
++_30;
goto append_all;

try_functor:
if (!fn_00242CC4(entry, "Functor"))
goto append_all;
{
FunctorExecutorStorage* value = new FunctorExecutorStorage;
if (value)
value = fn_001DC890(value, entry->_0);
pending = value;
}
reinterpret_cast<void**>(_40)[_3C] = pending;
++_3C;

assign_common:
executor = pending;

append_all:
reinterpret_cast<void**>(_10)[_8] = executor;
++_8;
}
}

#endif

} // namespace al
