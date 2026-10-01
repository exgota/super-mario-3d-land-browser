#pragma once

namespace al
{

class ExecuteRequestKeeper;

class ActorExecuteInfo
{
private:
        ExecuteRequestKeeper* mRequestKeeper;
        unsigned char mOpaqueMembers04[ 0x14 ];
        void* mPointerAtOffset18;

public:
        void* getPointerAtOffset18() const
        {
                return mPointerAtOffset18;
        }

        ExecuteRequestKeeper* getRequestKeeper() const
        {
                return mRequestKeeper;
        }
};

} // namespace al
