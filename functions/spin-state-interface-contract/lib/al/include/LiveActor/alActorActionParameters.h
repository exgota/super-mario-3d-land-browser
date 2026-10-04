#pragma once

namespace al
{
class LiveActor;
}

// The payload is numeric storage; four floats are a readable prefix, not its extent.
extern "C" const float* fn_0026C984( const al::LiveActor* actor, const char* action );
