#pragma once

#include <Nerve/alNerveStateBase.h>

namespace al
{
class LiveActor;

class ActorStateBase : public al::NerveStateBase
{
protected:
        LiveActor* const mHost;

public:
        LiveActor* getHost() const
        {
                return mHost;
        }

        ActorStateBase( const char* name, LiveActor* host ) : NerveStateBase( name ), mHost( host )
        {
        }
};

} // namespace al
