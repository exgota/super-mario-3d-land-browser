#pragma once

// Private pool layouts for ItemHolder construction at 0x00275828.
// These do not change the historical shared sead::PtrArray declaration.
namespace dot275828
{
struct ArrayView
{
    ArrayView();
    int size;
    int capacity;
    void** buffer;
};
struct FreeList
{
    FreeList();
    void* storage;
    void* head;
};
struct NameRef
{
    NameRef(const char*);
    const void* dispatch;
    const char* text;
};
template<int Count> struct FixedArray
{
    ArrayView view;
    void* storage[Count];
    FixedArray();
};
struct PooledRecords
{
    PooledRecords();
    void setStorage(void* storage, int capacity);
    ArrayView active;
    FreeList free;
    unsigned int records[128][4];
    void* pointers[128];
};
template<int Count> struct ItemPool
{
    ItemPool(const char*);
    FixedArray<Count> actors;
    PooledRecords pending;
    NameRef name;
    unsigned int state;
};
}
