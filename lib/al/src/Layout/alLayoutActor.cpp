#include <Layout/alLayoutActor.h>

namespace al
{

namespace
{
// Only the calculation pointer at offset0x14 is established for this view.
// The prefix remains opaque; this is not a complete layout object definition.
struct LayoutCalculationFields
{
        unsigned char mOpaquePrefix[ 0x14 ];
        void*         mCalculationObject;
};
} // namespace

extern "C" void fn_00264694( void* layoutObject );

#ifdef NON_MATCHING
LayoutActor::LayoutActor( const char* name )
    : mName( name ), mNerveKeeper( nullptr ), mLayoutObject( nullptr ), mEffectKeeper( nullptr ),
      _20( nullptr ), mAudioKeeper( nullptr ), _28( nullptr ), mIsAlive( false )
{
}
#endif

NerveKeeper* LayoutActor::getNerveKeeper() const
{
        return mNerveKeeper;
}

void LayoutActor::appear()
{
        mIsAlive = true;
        calcAnim();
}

void LayoutActor::kill()
{
        if ( getEffectKeeper() )
                getEffectKeeper()->deleteAndClearEffectAll();
        mIsAlive = false;
}

AudioKeeper* LayoutActor::getAudioKeeper() const
{
        return mAudioKeeper;
}

EffectKeeper* LayoutActor::getEffectKeeper() const
{
        return mEffectKeeper;
}

void LayoutActor::calcAnim()
{
        if ( mIsAlive )
        {
                const LayoutCalculationFields* layout = static_cast<const LayoutCalculationFields*>( mLayoutObject );
                if ( layout->mCalculationObject )
                        fn_00264694( mLayoutObject );
        }
}

void LayoutActor::control()
{
}

void LayoutActor::initNerve( const Nerve* nerve, int step )
{
        mNerveKeeper = new NerveKeeper( this, nerve, step );
}

} // namespace al
