#include "Scene/SceneObjFactory.h"

// Factory callback recovered from the literal at 0x00166AF0.
extern "C" void* fn_0015df30( int type );

al::SceneObjHolder* SceneObjFactory::createSceneObjHolder()
{
        return new al::SceneObjHolder( &fn_0015df30, SceneObj_Max );
}
