#include <container/seadListImpl.h>

namespace sead
{

void ListNode::insertFront_( ListNode* node )
{
        ListNode* previous = mPrevious;
        mPrevious = node;
        node->mNext = this;
        node->mPrevious = previous;
        if ( previous )
                previous->mNext = node;
}

// NonMatching: bounded ARM11 whole-function checks agree; seven bytes differ.
#ifdef NON_MATCHING
void ListImpl::clear()
{
        ListNode* node = mSentinel.mNext;
        while ( node != &mSentinel )
        {
                ListNode* removed = node;
                node = node->mNext;
                removed->mNext = 0;
                removed->mPrevious = 0;
        }
        mSentinel.mNext = &mSentinel;
        mSize = 0;
        mSentinel.mPrevious = &mSentinel;
}

#endif

} // namespace sead
