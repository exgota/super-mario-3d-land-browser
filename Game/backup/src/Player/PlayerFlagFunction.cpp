namespace
{
// The PlayerActor subsystem identity is partial. Only these fields are used.
struct PlayerFlagTransitionFields
{
        unsigned char mOpaquePrefix[ 0x18 ];
        signed char   mCurrentFlag;
        signed char   mPreviousFlag;
        unsigned char mOpaqueMembers1A[ 2 ];
        int           mTransitionCountdown;
};
} // namespace

extern "C" void fn_001BB19C( void* playerSubsystem, signed char currentFlag )
{
        PlayerFlagTransitionFields* fields = static_cast<PlayerFlagTransitionFields*>( playerSubsystem );
        const signed char previousFlag = fields->mCurrentFlag;
        fields->mCurrentFlag = currentFlag;
        fields->mPreviousFlag = previousFlag;
        if ( !previousFlag && currentFlag )
                fields->mTransitionCountdown = 2;
}
