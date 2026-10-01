namespace
{
struct AudioEmitterStateFields
{
        void* mVirtualTable;
        const char* mEmitterName;
        const char* mJointName;
        float mOffset[ 3 ];
        void* mSoundSource;
};
}

extern "C" void fn_001D9D1C( void* state )
{
        AudioEmitterStateFields* fields = static_cast<AudioEmitterStateFields*>( state );
        fields->mEmitterName = 0;
        fields->mJointName = 0;
        fields->mOffset[ 0 ] = 0.0f;
        fields->mOffset[ 1 ] = 0.0f;
        fields->mOffset[ 2 ] = 0.0f;
        fields->mSoundSource = 0;
}
