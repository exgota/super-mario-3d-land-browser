#pragma once
#include <nn/types.h>
#include <prim/seadSafeString.h>
namespace sead
{
// Partial CTR layouts independently recovered from the retail constructors.
// Unknown base members are intentionally opaque.
class CriticalSection { unsigned char mUnrecovered[0x1c]; };
class MethodTreeNode;
class MethodPauseDelegate
{
public:
    virtual void invoke(MethodTreeNode*, int) = 0;
};
class MethodTreeNode
{
    void* mUnrecoveredVtable;
    unsigned char mUnrecovered04[0x20];
    SafeString mName;
    unsigned char mUnrecovered2C[0x18];
    unsigned int mPauseFlag;
    MethodPauseDelegate* mPauseEventDelegate;
    void* mUserId;
public:
    explicit MethodTreeNode(CriticalSection*);
    void pushBackChild(MethodTreeNode*);
    void lock_();
    void unlock_();
    void setName(const SafeString& name) { mName = name; }
    void setPauseFlag(int flag)
    {
        lock_();
        if (mPauseEventDelegate)
            mPauseEventDelegate->invoke(this, flag);
        mPauseFlag = flag;
        unlock_();
    }
};
class MethodTreeMgr
{
public:
    virtual const void* getRuntimeTypeInfo() const;
    MethodTreeMgr();
    virtual ~MethodTreeMgr();
    virtual void attachMethod(int, MethodTreeNode*) = 0;
    virtual MethodTreeNode* getRootMethodTreeNode(int) = 0;
    virtual void pauseAll(bool) = 0;
    virtual void pauseAppCalc(bool) = 0;
protected:
    CriticalSection mCS;
};
class DualScreenMethodTreeMgr : public MethodTreeMgr
{
public:
    DualScreenMethodTreeMgr();
    virtual const void* getRuntimeTypeInfo() const;
    virtual ~DualScreenMethodTreeMgr();
    virtual void attachMethod(int, MethodTreeNode*);
    virtual MethodTreeNode* getRootMethodTreeNode(int);
    virtual void pauseAll(bool);
    virtual void pauseAppCalc(bool);
private:
    MethodTreeNode mRootCalc;
    MethodTreeNode mSysCalc;
    MethodTreeNode mAppCalc;
    MethodTreeNode mTopRootDraw;
    MethodTreeNode mTopSysDraw;
    MethodTreeNode mTopAppDraw;
    MethodTreeNode mTopAppDrawFinal;
    MethodTreeNode mBtmRootDraw;
    MethodTreeNode mBtmSysDraw;
    MethodTreeNode mBtmAppDraw;
    MethodTreeNode mBtmAppDrawFinal;
    bool mUnrecovered390;
    bool mUnrecovered391;
};
typedef char MethodNodeSizeCheck[sizeof(MethodTreeNode) == 0x50 ? 1 : -1];
typedef char MethodManagerSizeCheck[sizeof(MethodTreeMgr) == 0x20 ? 1 : -1];
typedef char DualManagerSizeCheck[sizeof(DualScreenMethodTreeMgr) == 0x394 ? 1 : -1];
}
