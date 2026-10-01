namespace
{
struct AudioKeeperControlFields
{
        unsigned char mOpaquePrefix[ 8 ];
        void* mControl;
};
}

extern "C" void fn_001D9D7C( void* control );

extern "C" void fn_001D9D74( void* audioKeeper )
{
        AudioKeeperControlFields* fields = static_cast<AudioKeeperControlFields*>( audioKeeper );
        fn_001D9D7C( fields->mControl );
}
