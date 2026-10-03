#pragma once

namespace al
{
class LiveActor;
}

// Reconstructed neutral import: squared translation distance < radius squared.
// The original returns 0 or 1; bool and const express the observed contract,
// not recovered original source spelling. No body is supplied here.
extern "C" bool fn_0021E2D0( const al::LiveActor* firstActor,
                           const al::LiveActor* secondActor, float radius );
