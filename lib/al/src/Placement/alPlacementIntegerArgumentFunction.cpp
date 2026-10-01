#ifdef NON_MATCHING
#include <LiveActor/alActorInitInfo.h>
#include <Placement/alPlacementFunction.h>

namespace al
{
// Source-local interface follows the independently matching float-reader helper.
static inline bool readIntegerArgumentFamily(const PlacementInfo& info, const char* argName, int* out)
{
        int value;
        if (info.isValid() && info.tryGetIntByKey(&value, argName) && value != -1)
        {
                *out = value;
                return true;
        }
        return false;
}

bool tryGetArg0(int* out, const ActorInitInfo& info)
{
        return readIntegerArgumentFamily(getPlacementInfo(info), "Arg0", out);
}

bool tryGetArg1(int* out, const ActorInitInfo& info)
{
        return readIntegerArgumentFamily(getPlacementInfo(info), "Arg1", out);
}

bool tryGetArg2(int* out, const ActorInitInfo& info)
{
        return readIntegerArgumentFamily(getPlacementInfo(info), "Arg2", out);
}

bool tryGetArg3(int* out, const ActorInitInfo& info)
{
        return readIntegerArgumentFamily(getPlacementInfo(info), "Arg3", out);
}

bool tryGetArg4(int* out, const ActorInitInfo& info)
{
        return readIntegerArgumentFamily(getPlacementInfo(info), "Arg4", out);
}

bool tryGetArg5(int* out, const ActorInitInfo& info)
{
        return readIntegerArgumentFamily(getPlacementInfo(info), "Arg5", out);
}

bool tryGetArg6(int* out, const ActorInitInfo& info)
{
        return readIntegerArgumentFamily(getPlacementInfo(info), "Arg6", out);
}

bool tryGetArg7(int* out, const ActorInitInfo& info)
{
        return readIntegerArgumentFamily(getPlacementInfo(info), "Arg7", out);
}

bool tryGetArg8(int* out, const ActorInitInfo& info)
{
        return readIntegerArgumentFamily(getPlacementInfo(info), "Arg8", out);
}

bool tryGetArg2(int* out, const PlacementInfo& info)
{
        return readIntegerArgumentFamily(info, "Arg2", out);
}

} // namespace al
#endif
