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
                T* mElement;
                s32 mOffset;
        public:
                iterator( ListNode* node, s32 offset ) : mElement( reinterpret_cast<T*>( reinterpret_cast<u8*>( node ) - offset ) ), mOffset( offset ) {}
                T& operator*() const
                {
                        return *mElement;
                }
                iterator& operator++()
                {
                        ListNode* node = reinterpret_cast<ListNode*>( reinterpret_cast<u8*>( mElement ) + mOffset );
                        mElement = reinterpret_cast<T*>( reinterpret_cast<u8*>( node->mNext ) - mOffset );
                        return *this;
                }
                bool operator!=( const iterator& other ) const { return mElement != other.mElement; }
        };
        iterator begin() { return iterator( mSentinel.mNext, mOffset ); }
        iterator end() { return iterator( &mSentinel, mOffset ); }
};

} // namespace sead
