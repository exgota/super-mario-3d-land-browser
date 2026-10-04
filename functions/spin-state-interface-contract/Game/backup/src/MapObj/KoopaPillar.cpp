#include <LiveActor/alActorInitializationImports.h>
#include <MapObj/KoopaPillar.h>

#include <LiveActor/alActorInitUtil.h>
#include <LiveActor/alLiveActorFunction.h>
#include <Nerve/alNerve.h>
#include <Placement/alPlacementFunction.h>
#include <Util/alStringUtil.h>

namespace al
{
bool tryGetArg1( bool* out, const ActorInitInfo& info );

// Descriptive name for the 0x60-byte model constructor at 0x0026FCB8.
// Its primary vtable at 0x003D6124 has the LiveActor virtual interface.
// Caller 0x00314214 passes the named host joint matrix as the fifth argument.
class PillarBaseModel : public LiveActor
{
public:
        PillarBaseModel( LiveActor* host, const ActorInitInfo& info,
                const char* modelName, const sead::Matrix34f* jointMtx );
};
}

// Constructor 0x0016E7A0 stores host/modelName at 0x60/0x64 after
// MapObjActor construction; its vtable is at 0x003CBE94.
class KoopaPillarBreakModel : public al::MapObjActor
{
        al::LiveActor* mHost;
        const char* mModelName;

public:
        KoopaPillarBreakModel( al::LiveActor* host, const char* name,
                const char* modelName );
};

class KoopaPillarInitialNerve : public al::Nerve
{
public:
        virtual void execute( al::NerveKeeper* keeper ) const;
};

extern "C" const KoopaPillarInitialNerve dat_003F2934;
extern "C" void fn_0027B51C( al::LiveActor*, const al::ActorInitInfo&, int );
extern "C" void fn_00277E5C( al::LiveActor*, const al::ActorInitInfo&, int );
extern "C" void fn_00226C98( al::LiveActor*, const al::ActorInitInfo& );
extern "C" void fn_0026FC64( al::IUseStageSwitch*, const al::ActorInitInfo& );

void KoopaPillar::init( const al::ActorInitInfo& info )
{
        al::initActor( this, info );
        al::initNerve( this, &dat_003F2934 );
        const char* objectName;
        al::tryGetObjectName( &objectName, info );
        if ( al::isEqualString( objectName, "KoopaPillar" ) )
                mPillarType = 0;
        else if ( al::isEqualString( objectName, "KoopaLavaBreakPillar" ) )
        {
                mPillarType = 1;
                mBaseModel = new al::PillarBaseModel( this, info,
                        "KoopaLavaBreakPillarBase", nullptr );
                mBaseModel->makeActorDead();
        }

        al::StringTmp<128> modelName( "%sBreak", objectName );
        mBreakModel = new KoopaPillarBreakModel( this,
                "\x83\x4e\x83\x62\x83\x70\x89\xf3\x82\xea\x92\x8c\x89\xf3\x82\xea\x83\x82\x83\x66\x83\x8b", modelName.cstr() );
        mBreakModel->init( info );

        fn_002794F8( &mItemType, info );
        switch ( mItemType )
        {
        case 0:
                break;
        case 1:
                fn_0027B51C( this, info, 1 );
                break;
        case 2:
                fn_00277E5C( this, info, 1 );
                break;
        case 3:
                fn_00226C98( this, info );
                break;
        default:
                fn_0027B51C( this, info, 1 );
                break;
        }
        al::tryGetArg1( &mArg1, info );
        fn_0026FC64( this, info );
        makeActorAppeared();
}
