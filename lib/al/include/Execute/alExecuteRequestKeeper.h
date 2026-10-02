#pragma once

#include <stddef.h>

namespace al
{

class LiveActor;
struct ExecuteRequestQueue;

class ExecuteRequestKeeper
{
private:
        ExecuteRequestQueue* _0[ 4 ];

public:
        void request( LiveActor*, int );

public:
        ExecuteRequestKeeper( size_t );
};

} // namespace al
