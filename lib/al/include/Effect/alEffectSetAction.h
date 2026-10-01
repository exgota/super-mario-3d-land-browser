#pragma once

// The retail entry point consumes r2 as a full word and distinguishes only
// zero from nonzero. Both established callers promote a signed byte.
extern "C" void fn_001E9B30( void* effectSet, const char* actionName, int actionChangeMode );
