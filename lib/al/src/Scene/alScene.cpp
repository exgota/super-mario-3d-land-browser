#include <Factory/alActorFactory.h>
#include <LiveActor/alActorInitInfo.h>
#include <Scene/SceneObjFactory.h>
#include <Scene/alScene.h>
#include <Scene/alSceneObjHolder.h>
#include <Stage/alStageResourceKeeper.h>
#include <System/Application.h>

extern "C" al::SceneObjHolder* fn_00166ac8();
extern "C" void fn_001891c0( al::SceneObjHolder* holder );
extern "C" void fn_001dcbe4( al::StageResourceKeeper* keeper, const char* stageName, int scenario, sead::Heap* heap );

namespace al
{

Scene::Scene( const char* name )
    : NerveExecutor( name ), mAudioKeeper( nullptr ), mLiveActorKit( nullptr ), mLayoutKit( nullptr ),
      mSceneObjHolder( nullptr ), mActorFactory( nullptr ), _20( nullptr ), mResourceKeeper( nullptr ),
      _28( nullptr ), _2C( nullptr ), mIsAlive( false )
{
}

void Scene::appear()
{
        if ( !mIsAlive )
                mIsAlive = true;
}

void Scene::kill()
{
        mIsAlive = false;
}

#ifdef NON_MATCHING
void Scene::movement()
{
        if ( mIsAlive )
        {
                updateNerve();
                control();
                if ( mAudioKeeper )
                        mAudioKeeper->update();
        }
}
#endif

void Scene::control()
{
}

AudioKeeper* Scene::getAudioKeeper() const
{
        return mAudioKeeper;
}

void Scene::initAndLoadStageResource( const char* stageName, int scenario, sead::Heap* heap )
{
        mResourceKeeper = new StageResourceKeeper;
        fn_001dcbe4( mResourceKeeper, stageName, scenario, heap );
}

void Scene::initActorFactory()
{
        mActorFactory = new ActorFactory();
}

void Scene::initSceneObjHolder()
{
        SceneObjHolder* holder = fn_00166ac8();
        mSceneObjHolder        = holder;
        fn_001891c0( holder );
}

void Scene::endInit( const ActorInitInfo& info )
{
        if ( mSceneObjHolder )
                mSceneObjHolder->initAfterPlacementSceneObj( info );
}

} // namespace al
