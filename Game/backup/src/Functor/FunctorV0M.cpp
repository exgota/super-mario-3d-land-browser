#include "Scene/CourseSelectScene.h"
#include "Scene/DemoScene.h"
#include "Scene/StageScene.h"

#include <Functor/alFunctorV0M.h>

namespace al
{

template <>
void FunctorV0M<StageScene*, void ( StageScene::* )()>::operator()() const
{
        ( mParent->*mFuncPtr )();
}

template <>
FunctorV0M<StageScene*, void ( StageScene::* )()>*
FunctorV0M<StageScene*, void ( StageScene::* )()>::clone() const
{
        return new FunctorV0M<StageScene*, void ( StageScene::* )()>( *this );
}

template <>
void FunctorV0M<CourseSelectScene*, void ( CourseSelectScene::* )()>::operator()() const
{
        ( mParent->*mFuncPtr )();
}

template <>
FunctorV0M<CourseSelectScene*, void ( CourseSelectScene::* )()>*
FunctorV0M<CourseSelectScene*, void ( CourseSelectScene::* )()>::clone() const
{
        return new FunctorV0M<CourseSelectScene*, void ( CourseSelectScene::* )()>( *this );
}

template <>
void FunctorV0M<DemoScene*, void ( DemoScene::* )()>::operator()() const
{
        ( mParent->*mFuncPtr )();
}

template <>
FunctorV0M<DemoScene*, void ( DemoScene::* )()>*
FunctorV0M<DemoScene*, void ( DemoScene::* )()>::clone() const
{
        return new FunctorV0M<DemoScene*, void ( DemoScene::* )()>( *this );
}

} // namespace al
