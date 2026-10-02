namespace ExecutorActorListReconstruction
{

struct Actor;

struct ExecutorActorList
{
        const void* mVirtualFunctionTable;
        const char* mName;
        int mCapacity;
        int mSize;
        Actor** mBuffer;
};

extern "C" void fn_001E2BEC( ExecutorActorList* list, Actor* actor )
{
        for ( int index = 0; index < list->mSize; index++ )
        {
                if ( list->mBuffer[ index ] == actor )
                {
                        if ( list->mSize > 1 )
                        {
                                list->mBuffer[ index ] = list->mBuffer[ list->mSize - 1 ];
                                list->mBuffer[ list->mSize - 1 ] = 0;
                        }
                        list->mSize--;
                        return;
                }
        }
}

extern "C" void fn_001E2C60( ExecutorActorList* list )
{
        if ( list->mCapacity > 0 )
        {
                list->mBuffer = new Actor*[ list->mCapacity ];
                for ( int index = 0; index < list->mCapacity; index++ )
                        list->mBuffer[ index ] = 0;
        }
}

extern "C" void fn_001E2CE0( ExecutorActorList* list, Actor* actor )
{
        for ( int index = 0; index < list->mSize; index++ )
                if ( list->mBuffer[ index ] == actor )
                        return;
        list->mBuffer[ list->mSize ] = actor;
        list->mSize++;
}

} // namespace ExecutorActorListReconstruction

namespace al
{
    struct ExecutorStorage
    {
        const void* mVirtualFunctionTable;
        const char* mName;
        int mCapacity;
        int mSize;
        void** mBuffer;
    };

    struct FunctorExecutorStorage
    {
        const void* mVirtualFunctionTable;
        const char* mName;
        void* mFirstEntry;
    };

    extern "C" ExecutorStorage* fn_001E43D4(
    ExecutorStorage*, const char*, int);
    extern "C" ExecutorStorage* fn_001E43BC(
    ExecutorStorage*, const char*, int);
    extern "C" ExecutorStorage* fn_001E85A4(
    ExecutorStorage*, const char*, int);
    extern "C" ExecutorStorage* fn_001E3BC8(
    ExecutorStorage*, const char*, int);
    extern "C" ExecutorStorage* fn_001E7618(
    ExecutorStorage*, const char*, int);
    extern "C" FunctorExecutorStorage* fn_001DC890(
    FunctorExecutorStorage*, const char*);

    struct LayoutDrawExecutorStorage
    {
        const void* mVirtualFunctionTable;
        const char* mName;
        int mCapacity;
        int mSize;
        void** mBuffer;
        void* mDrawInfo;
    };

    struct ModelDrawExecutorStorage
    {
        const void* mVirtualFunctionTable;
        const char* mName;
        int mCapacity;
        int mSize;
        void** mBuffer;
        unsigned char mOpaque14[4];
        void* mDrawInfo;
    };

    struct UserDrawExecutorStorage
    {
        const void* mVirtualFunctionTable;
        const char* mName;
        int mCapacity;
        int mSize;
        void** mBuffer;
    };

    extern "C" void* fn_00243B94(void*, const char*);
    extern "C" ExecutorStorage* fn_002415DC(ExecutorStorage*, const char*, int);
    extern "C" ModelDrawExecutorStorage* fn_001E7518(ModelDrawExecutorStorage*, const char*, int);
    extern "C" const unsigned char dat_003D6EF4[];
    extern "C" const unsigned char dat_003D741C[];
    extern "C" const unsigned char dat_003D74B0[];
    extern "C" const unsigned char dat_003D74CC[];
    extern "C" const unsigned char dat_003D7620[];
    extern "C" const unsigned char dat_003D7638[];
    extern "C" const unsigned char dat_003D7584[];
    extern "C" const unsigned char dat_003D7650[];
    extern "C" const unsigned char dat_003D766C[];
    extern "C" const unsigned char dat_003D7760[];
    extern "C" const unsigned char dat_003D7850[];
    extern "C" const unsigned char dat_003D789C[];
    extern "C" const unsigned char dat_003D78DC[];

    extern "C" FunctorExecutorStorage* fn_001DC890(FunctorExecutorStorage* storage, const char* name)
    {
        storage = static_cast<FunctorExecutorStorage*>(fn_00243B94(storage, name));
        storage->mFirstEntry = 0;
        storage->mVirtualFunctionTable = dat_003D6EF4;
        return storage;
    }

    extern "C" ExecutorStorage* fn_001E43BC(ExecutorStorage* storage, const char* name, int capacity)
    {
        storage = fn_002415DC(storage, name, capacity);
        storage->mVirtualFunctionTable = dat_003D74B0;
        return storage;
    }

    extern "C" ExecutorStorage* fn_001E43D4(ExecutorStorage* storage, const char* name, int capacity)
    {
        storage = fn_002415DC(storage, name, capacity);
        storage->mVirtualFunctionTable = dat_003D74CC;
        return storage;
    }

    extern "C" ModelDrawExecutorStorage* fn_001E7E00(ModelDrawExecutorStorage* storage, const char* name, int capacity)
    {
        storage = fn_001E7518(storage, name, capacity);
        storage->mVirtualFunctionTable = dat_003D7850;
        return storage;
    }

    extern "C" ExecutorStorage* fn_001E85A4(ExecutorStorage* storage, const char* name, int capacity)
    {
        storage = fn_002415DC(storage, name, capacity);
        storage->mVirtualFunctionTable = dat_003D789C;
        return storage;
    }

    extern "C" ModelDrawExecutorStorage* fn_001E8AA8(ModelDrawExecutorStorage* storage, const char* name, int capacity)
    {
        storage = fn_001E7518(storage, name, capacity);
        storage->mVirtualFunctionTable = dat_003D78DC;
        return storage;
    }

    extern "C" ExecutorStorage* fn_001E3BC8(ExecutorStorage* storage, const char* name, int capacity)
    {
        storage = static_cast<ExecutorStorage*>(fn_00243B94(storage, name));
        storage->mVirtualFunctionTable = dat_003D741C;
        storage->mCapacity = capacity;
        storage->mSize = 0;
        storage->mBuffer = 0;
        storage->mBuffer = new void*[capacity];
        for (int index = 0; index < storage->mCapacity; ++index)
            storage->mBuffer[index] = 0;
        return storage;
    }

    template <typename Storage>
    inline Storage* installExecutorTable(Storage* storage, const void* table)
    {
        storage->mVirtualFunctionTable = table;
        return storage;
    }

    inline LayoutDrawExecutorStorage* initializeLayoutDrawExecutorStorage(LayoutDrawExecutorStorage* storage, const char* name, int capacity)
    {
        storage = static_cast<LayoutDrawExecutorStorage*>(fn_00243B94(storage, name));
        storage->mVirtualFunctionTable = dat_003D7584;
        storage->mCapacity = capacity;
        storage->mSize = 0;
        storage->mBuffer = 0;
        storage->mDrawInfo = 0;
        storage->mBuffer = new void*[capacity];
        for (int index = 0; index < storage->mCapacity; ++index)
            storage->mBuffer[index] = 0;
        return storage;
    }

    extern "C" UserDrawExecutorStorage* fn_001E5F24(UserDrawExecutorStorage* storage, const char* name, int capacity)
    {
        storage = static_cast<UserDrawExecutorStorage*>(fn_00243B94(storage, name));
        storage->mVirtualFunctionTable = dat_003D7620;
        storage->mCapacity = capacity;
        storage->mSize = 0;
        storage->mBuffer = 0;
        storage->mBuffer = new void*[capacity];
        for (int index = 0; index < storage->mCapacity; ++index)
            storage->mBuffer[index] = 0;
        return installExecutorTable(storage, dat_003D7638);
    }

    extern "C" LayoutDrawExecutorStorage* fn_001E5FCC(LayoutDrawExecutorStorage* storage, const char* name, int capacity)
    {
        storage = initializeLayoutDrawExecutorStorage(storage, name, capacity);
        storage->mVirtualFunctionTable = dat_003D7650;
        return storage;
    }

    extern "C" LayoutDrawExecutorStorage* fn_001E6078(LayoutDrawExecutorStorage* storage, const char* name, int capacity)
    {
        storage = initializeLayoutDrawExecutorStorage(storage, name, capacity);
        storage->mVirtualFunctionTable = dat_003D766C;
        return storage;
    }

    extern "C" ExecutorStorage* fn_001E7618(ExecutorStorage* storage, const char* name, int capacity)
    {
        storage = static_cast<ExecutorStorage*>(fn_00243B94(storage, name));
        storage->mVirtualFunctionTable = dat_003D7620;
        storage->mCapacity = capacity;
        storage->mSize = 0;
        storage->mBuffer = 0;
        storage->mBuffer = new void*[capacity];
        for (int index = 0; index < storage->mCapacity; ++index)
            storage->mBuffer[index] = 0;
        return installExecutorTable(storage, dat_003D7760);
    }
}
