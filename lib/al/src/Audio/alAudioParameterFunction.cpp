namespace
{
struct AudioParameterFields
{
        void* mVirtualTable;
        void* mSource;
        float mValues[ 3 ];
};
}

extern "C" void* fn_0033603C( void* parameter )
{
        AudioParameterFields* fields = static_cast<AudioParameterFields*>( parameter );
        return fields->mSource;
}

extern "C" void* fn_00336044( void* parameter )
{
        AudioParameterFields* fields = static_cast<AudioParameterFields*>( parameter );
        return fields->mValues;
}
