#pragma once

#include <container/seadListImpl.h>
#include <stddef.h>

namespace sead
{

template <typename T>
class OffsetListNode
{
public:
        T mValue;
        ListNode mListNode;
        OffsetListNode( T value ) : mValue( value ) {}
        static s32 getListNodeOffset() { return offsetof( OffsetListNode, mListNode ); }
};

template <typename T>
class OffsetList : public ListImpl
{
private:
        s32 mOffset;

public:
        OffsetList() : mOffset( 0 ) {}
        void initOffset( s32 offset ) { mOffset = offset; }
        void pushBack( OffsetListNode<T>& node )
        {
                ListImpl::pushBack( reinterpret_cast<ListNode*>( reinterpret_cast<u8*>( &node ) + mOffset ) );
        }
        class iterator
        {
        private:
                ListNode* mNode;
                s32 mOffset;
        public:
                iterator( ListNode* node, s32 offset ) : mNode( node ), mOffset( offset ) {}
                T& operator*() const
                {
                        return reinterpret_cast<OffsetListNode<T>*>( reinterpret_cast<u8*>( mNode ) - mOffset )->mValue;
                }
                iterator& operator++() { mNode = mNode->mNext; return *this; }
                bool operator!=( const iterator& other ) const { return mNode != other.mNode; }
        };
        iterator begin() { return iterator( mSentinel.mNext, mOffset ); }
        iterator end() { return iterator( &mSentinel, mOffset ); }
};

} // namespace sead
