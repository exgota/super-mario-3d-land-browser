#include <LiveActor/alActorInitInfo.h>
#include <Scene/alISceneObj.h>
#include <Scene/alSceneObjHolder.h>
#include <System/Application.h>

extern "C" al::SceneObjHolder* fn_00267230();

namespace al
{

SceneObjHolder::SceneObjHolder( CreateFunc func, int size )
    : mCreateFunc( func ), mObjs( nullptr ), mSize( size )
{
        mObjs = new ISceneObj*[ mSize ];
        for ( int i = 0; i < mSize; i++ )
                mObjs[ i ] = nullptr;
}

#pragma no_inline // probably belongs in another file

SceneObjHolder* getSceneObjHolder()
{
        return al::getApplication()->getSceneObjHolder();
}

ISceneObj* SceneObjHolder::getObj( int id ) const
{
        return mObjs[ id ];
}

bool SceneObjHolder::isExist( int id ) const
{
        return mObjs[ id ] != nullptr;
}

void SceneObjHolder::setObj( ISceneObj* obj, int id )
{
        mObjs[ id ] = obj;
}

ISceneObj* SceneObjHolder::create( int id )
{
        if ( mObjs[ id ] == nullptr )
        {
                ISceneObj* newObj = static_cast<ISceneObj*>( mCreateFunc( id ) );
                mObjs[ id ]       = newObj;
                newObj->initSceneObj();
        }
        return mObjs[ id ];
}

void SceneObjHolder::initAfterPlacementSceneObj( const ActorInitInfo& info )
{
        for ( int i = 0; i < mSize; i++ )
                if ( mObjs[ i ] )
                        mObjs[ i ]->initAfterPlacementSceneObj( info );
}

ISceneObj* createSceneObj( int id )
{
        return ::fn_00267230()->create( id );
}

ISceneObj* getSceneObj( int id )
{
        return ::fn_00267230()->getObj( id );
}

bool isExistSceneObj( int id )
{
        return ::fn_00267230()->isExist( id );
}

void setSceneObj( ISceneObj* obj, int id )
{
        return ::fn_00267230()->setObj( obj, id );
}

} // namespace al
