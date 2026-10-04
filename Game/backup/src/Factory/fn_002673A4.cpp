namespace
{
struct HitReactionOwner
{
        void* mUnknown;
};

struct HitReactionState
{
        char mPadding[0x120];
        void* mValue;
};
}

extern "C" void* fn_002673A4( HitReactionOwner* owner )
{
        return static_cast<HitReactionState*>( owner->mUnknown )->mValue;
}
