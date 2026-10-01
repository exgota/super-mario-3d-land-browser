#ifdef NON_MATCHING
#include <framework/seadDualScreenMethodTreeMgr.h>
namespace sead
{
DualScreenMethodTreeMgr::DualScreenMethodTreeMgr()
    : mRootCalc(&mCS),
      mSysCalc(&mCS),
      mAppCalc(&mCS),
      mTopRootDraw(&mCS),
      mTopSysDraw(&mCS),
      mTopAppDraw(&mCS),
      mTopAppDrawFinal(&mCS),
      mBtmRootDraw(&mCS),
      mBtmSysDraw(&mCS),
      mBtmAppDraw(&mCS),
      mBtmAppDrawFinal(&mCS),
      mUnrecovered390(true), mUnrecovered391(false)
{
    mRootCalc.setName("sead::RootCalc");
    mSysCalc.setName("sead::SysCalc");
    mAppCalc.setName("sead::AppCalc");
    mTopRootDraw.setName("sead::TopRootDraw");
    mTopSysDraw.setName("sead::TopSysDraw");
    mTopAppDraw.setName("sead::TopAppDraw");
    mTopAppDrawFinal.setName("sead::TopAppDrawFinal");
    mBtmRootDraw.setName("sead::BtmRootDraw");
    mBtmSysDraw.setName("sead::BtmSysDraw");
    mBtmAppDraw.setName("sead::BtmAppDraw");
    mBtmAppDrawFinal.setName("sead::BtmAppDrawFinal");
    mRootCalc.pushBackChild(&mSysCalc);
    mRootCalc.pushBackChild(&mAppCalc);
    mTopRootDraw.pushBackChild(&mTopAppDraw);
    mTopRootDraw.pushBackChild(&mTopAppDrawFinal);
    mTopRootDraw.pushBackChild(&mTopSysDraw);
    mBtmRootDraw.pushBackChild(&mBtmAppDraw);
    mBtmRootDraw.pushBackChild(&mBtmAppDrawFinal);
    mBtmRootDraw.pushBackChild(&mBtmSysDraw);
    mSysCalc.setPauseFlag(0);
    mAppCalc.setPauseFlag(0);
    mTopSysDraw.setPauseFlag(0);
    mTopAppDraw.setPauseFlag(0);
    mTopAppDrawFinal.setPauseFlag(0);
    mBtmSysDraw.setPauseFlag(0);
    mBtmAppDraw.setPauseFlag(0);
    mBtmAppDrawFinal.setPauseFlag(0);
    mRootCalc.setPauseFlag(3);
    mTopRootDraw.setPauseFlag(3);
    mBtmRootDraw.setPauseFlag(3);
}
}
#endif
