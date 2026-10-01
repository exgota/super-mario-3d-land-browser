#pragma once

#include <nn/types.h>

namespace sead
{

class ListNode
{
public:
        ListNode* mPrevious;
        ListNode* mNext;
        ListNode() : mPrevious( 0 ), mNext( 0 ) {}
        ListNode( ListNode* previous, ListNode* next ) : mPrevious( previous ), mNext( next ) {}
        void insertFront_( ListNode* node );
        void erase_();
};

class ListImpl
{
protected:
        ListNode mSentinel;
        s32 mSize;

public:
        ListImpl() : mSentinel( &mSentinel, &mSentinel ), mSize( 0 ) {}
        void clear();
        ListNode* popFront();
        void pushBack( ListNode* node )
        {
                mSentinel.insertFront_( node );
                ++mSize;
        }
};

} // namespace sead
