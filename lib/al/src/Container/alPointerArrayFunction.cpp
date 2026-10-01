namespace
{
// Constructor, allocation and append code independently establish these fields.
// The original class identity and the complete object extent remain unknown.
struct PointerArrayFields
{
        const char* mName;
        void** mElements;
        int mElementCount;
        int mCapacity;
};
}
extern "C" void fn_001BF86C( void* pointerArray )
{
        ++static_cast<PointerArrayFields*>( pointerArray )->mCapacity;
}
extern "C" void fn_001BF87C( void* pointerArray, void* element )
{
        PointerArrayFields* fields = static_cast<PointerArrayFields*>( pointerArray );
        if ( fields->mElementCount < fields->mCapacity )
        {
                fields->mElements[ fields->mElementCount ] = element;
                ++fields->mElementCount;
        }
}
extern "C" void* fn_001BF8A0( void* pointerArray, const char* name )
{
        PointerArrayFields* fields = static_cast<PointerArrayFields*>( pointerArray );
        fields->mName = name;
        fields->mElements = 0;
        fields->mElementCount = 0;
        fields->mCapacity = 0;
        return fields;
}

extern "C" void fn_001BF848( void* pointerArray )
{
        PointerArrayFields* fields = static_cast<PointerArrayFields*>( pointerArray );
        if ( fields->mCapacity > 0 )
                fields->mElements = new void*[ fields->mCapacity ];
}
