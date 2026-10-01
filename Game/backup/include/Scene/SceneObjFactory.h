#pragma once

#include <Scene/alSceneObjHolder.h>

class SceneObjFactory
{
public:
        static al::SceneObjHolder* createSceneObjHolder();
};

enum SceneObjType
{
        SceneObjType_CoinRotater = 7,
        SceneObjType_GhostPlayerRecorder = 20,
        SceneObj_Max = 23
};
