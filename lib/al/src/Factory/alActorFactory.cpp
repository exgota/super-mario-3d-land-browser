#include <Factory/alActorFactory.h>
#include <Resource/alResource.h>
#include <Yaml/alByamlIter.h>

namespace al
{

ActorFactory::ActorFactory()
    : mArchive( nullptr ), mConvertNameData( nullptr )
{
        mArchive = al::findOrCreateResource( "SystemData/CreatorClassNameTable" );
        mConvertNameData =
                new ByamlIter( static_cast<const u8*>( mArchive->getByml( "CreatorClassNameTable" ) ) );
}

} // namespace al

#include <MapObj/alFallMapParts.h>
#include <Npc/alEffectObj.h>
#include <Npc/alSky.h>

#include "Enemy/Fugumannen.h"
#include "MapObj/AppearStep.h"
#include "MapObj/TransparentWall.h"
#include "Player/GhostPlayer.h"
#include "Enemy/Togezo.h"
#include "MapObj/NoteObj.h" // GAMEUSE
#include "MapObj/NoteObjGenerator.h"
#include "MapObj/TrickHintPanel.h"

namespace al
{

template <typename T>
LiveActor* createActorFunction( const char* name )
{
        return new T( name );
}

const NameToActorCreator staticd( sActorFactoryEntries )[] = {
        { "Kinopio", nullptr },
        { "Luigi", nullptr },
        { "Peach", nullptr },
        { "PeachForTitle", nullptr },
        { "BlockDragonGenerator", nullptr },
        { "BombHei", nullptr },
        { "BombHeiGenerator", nullptr },
        { "BombHeiTail", nullptr },
        { "BombHeiTailGenerator", nullptr },
        { "BoomerangBros", nullptr },
        { "Bubble", nullptr },
        { "Bug", nullptr },
        { "Chorobon", nullptr },
        { "ChorobonGenerator", nullptr },
        { "Choropu", nullptr },
        { "Dossun", nullptr },
        { "DossunTail", nullptr },
        { "EnemyCounter", nullptr },
        { "Fugumannen", createActorFunction<Fugumannen> },
        { "FugumannenGenerator", nullptr },
        { "GamaguchiKun", nullptr },
        { "Gesso", nullptr },
        { "GhostPlayer", createActorFunction<GhostPlayer> },
        { "HammerBros", nullptr },
        { "Hoppun", nullptr },
        { "Indy", nullptr },
        { "Kameck", nullptr },
        { "Karon", nullptr },
        { "Killer", nullptr },
        { "KillerGenerator", nullptr },
        { "KillerMagnum", nullptr },
        { "Kuribo", nullptr },
        { "KuriboTail", nullptr },
        { "KuriboTailSearch", nullptr },
        { "KuriboTower", nullptr },
        { "Meragon", nullptr },
        { "Nokonoko", nullptr },
        { "PackunFire", nullptr },
        { "PackunFlower", nullptr },
        { "PackunInk", nullptr },
        { "PataKuribo", nullptr },
        { "Poo", nullptr },
        { "Pukupuku", nullptr },
        { "PukupukuGenerator", nullptr },
        { "PukupukuLoopRail", nullptr },
        { "SamboBody", nullptr },
        { "SamboHead", nullptr },
        { "Sokuten", nullptr },
        { "TentenGenerator", nullptr },
        { "Teresa", nullptr },
        { "TeresaGroup", nullptr },
        { "TeresaRail", nullptr },
        { "TogeMetbo", nullptr },
        { "Togezo", createActorFunction<Togezo> },
        { "Utsubo", nullptr },
        { "Wanwan", nullptr },
        { "WooGan", nullptr },
        { "WooGanSand", nullptr },
        { "Bunbun", nullptr },
        { "BunbunAndPunpunTagObj", nullptr },
        { "BunbunMysteryBox", nullptr },
        { "Koopa", nullptr },
        { "Punpun", nullptr },
        { "PunpunStageDamageFire", nullptr },
        { "BoomerangFlower", nullptr },
        { "Coin", nullptr },
        { "CoinBox", nullptr },
        { "CoinCollect", nullptr },
        { "CoinGathering", nullptr },
        { "CoinGenerator", nullptr },
        { "CoinRailGenerator", nullptr },
        { "CoinRailMoveGenerator", nullptr },
        { "CoinRedStarter", nullptr },
        { "CoinRing", nullptr },
        { "FireFlower", nullptr },
        { "KickKoura", nullptr },
        { "KinokoOneUp", nullptr },
        { "KinokoOneUpFast", nullptr },
        { "KinokoPoison", nullptr },
        { "KinokoPoisonFast", nullptr },
        { "KinokoSuper", nullptr },
        { "KinokoSuperFast", nullptr },
        { "PatapataWing", nullptr },
        { "PropellerBlock", nullptr },
        { "SuperLeaf", nullptr },
        { "SuperLeafSpecial", nullptr },
        { "SuperStar", nullptr },
        { "Telescope", nullptr },
        { "TelescopeTarget", nullptr },
        { "TimerClock", nullptr },
        { "AppearStep", createActorFunction<AppearStep> },
        { "AquariumSwimDebris", nullptr },
        { "BalanceTruck", nullptr },
        { "BeatBlock", nullptr },
        { "BeatBlockHolder", nullptr },
        { "Bird", nullptr },
        { "BlindCloud", nullptr },
        { "BlockBrick", nullptr },
        { "BlockBrickItem", nullptr },
        { "BlockEmpty", nullptr },
        { "BlockHelp", nullptr },
        { "BlockNote", nullptr },
        { "BlockNoteSuper", nullptr },
        { "BlockQuestion", nullptr },
        { "BlockQuestionFlying", nullptr },
        { "BlockQuestionLong", nullptr },
        { "BlockRailCurve", nullptr },
        { "BlockRailStraight", nullptr },
        { "BlockRailTerminater", nullptr },
        { "BlockRoulette", nullptr },
        { "BlockTransparent", nullptr },
        { "BoneRollerCoaster", nullptr },
        { "BridgePukupukuBreakBridge", nullptr },
        { "Burner", nullptr },
        { "Candlestand", nullptr },
        { "CandlestandFire", nullptr },
        { "CandlestandObserver", nullptr },
        { "CourseSelectObj", nullptr },
        { "EntranceCameraObj", nullptr },
        { "FallStone", nullptr },
        { "FireBarCore", nullptr },
        { "FrameOutChecker", nullptr },
        { "Garigari", nullptr },
        { "GeneratorBox", nullptr },
        { "GeneratorBoxHop", nullptr },
        { "GeneratorBoxHopChild", nullptr },
        { "GeneratorEnemy", nullptr },
        { "GoAndReturnParts", nullptr },
        { "Gorori", nullptr },
        { "GororiGenerator", nullptr },
        { "Grass", nullptr },
        { "GrassMike", nullptr },
        { "GroundRiserAttachment", nullptr },
        { "GyroLauncher", nullptr },
        { "JumpFlipBoard", nullptr },
        { "Kabehei", nullptr },
        { "KeyMoveAndFallParts", nullptr },
        { "KickStone", nullptr },
        { "KickStoneStrong", nullptr },
        { "KoopaCollapseStep", nullptr },
        { "KoopaDoor", nullptr },
        { "KoopaDoorLast", nullptr },
        { "KoopaFireGenerator", nullptr },
        { "KoopaPillar", nullptr },
        { "KoopaSwitch", nullptr },
        { "LavaConveyer", nullptr },
        { "LavaGeyser", nullptr },
        { "LiftGeyser", nullptr },
        { "LiftGeyserStep", nullptr },
        { "MarinePlant", nullptr },
        { "NeedleBar", nullptr },
        { "NeedleBarCore", nullptr },
        { "NeedleBlock", nullptr },
        { "NeedleFloor", nullptr },
        { "NeedlePlant", nullptr },
        { "NeedleRoller", nullptr },
        { "NeedleTrap", nullptr },
        { "NutLift", nullptr },
        { "PatanBoard", nullptr },
        { "PatanBoardStarter", nullptr },
        { "PatanBox", nullptr },
        { "PeraObj", nullptr },
        { "PhotoAlbum", nullptr },
        { "Picket", nullptr },
        { "Plant", nullptr },
        { "Pole", nullptr },
        { "PresentBox", nullptr },
        { "RailDotDrawer", nullptr },
        { "RestartObj", nullptr },
        { "Rope", nullptr },
        { "ShadowObj", nullptr },
        { "SpikeStick", nullptr },
        { "StickNeedle", nullptr },
        { "SwingNeedleRoller", nullptr },
        { "SwingSpike", nullptr },
        { "TrampleSwitch", nullptr },
        { "Trampoline", nullptr },
        { "TransparentWall", createActorFunction<TransparentWall> },
        { "TreeA", nullptr },
        { "TrickHintPanel", createActorFunction<TrickHintPanel> },
        { "Trilift", nullptr },
        { "UpperBlock", nullptr },
        { "WarpLift", nullptr },
        { "WoodBox", nullptr },
        { "Seagull", nullptr },
        { "SoundEmitObj", nullptr },
        { "NoteObj", createActorFunction<NoteObj> },
        { "NoteObjGenerator", createActorFunction<NoteObjGenerator> },
        { "WarpAreaPoint", nullptr },
        { "WarpCube", nullptr },
        { "WarpPorter", nullptr },
        { "Dokan", nullptr },
        { "WarpDoor", nullptr },
        { "AttackRevolvingPanel", nullptr },
        { "ClockMapParts", nullptr },
        { "CollapseBreakMapParts", nullptr },
        { "CollapseMapParts", nullptr },
        { "FallMapParts", createActorFunction<al::FallMapParts> },
        { "FixMapParts", nullptr },
        { "FloaterMapParts", nullptr },
        { "KeyMoveMapParts", nullptr },
        { "RailMoveMapParts", nullptr },
        { "RollingReflectMapParts", nullptr },
        { "RollingShakeMapParts", nullptr },
        { "RotateMapParts", nullptr },
        { "SeesawMapParts", nullptr },
        { "SlideMapParts", nullptr },
        { "SwingLift", nullptr },
        { "SwingMapParts", nullptr },
        { "SwitchRotateMapParts", nullptr },
        { "SwitchSlideMapParts", nullptr },
        { "VisibleSwitchMapParts", nullptr },
        { "WheelMapParts", nullptr },
        { "WobbleMapParts", nullptr },
        { "Sky", createActorFunction<al::Sky> },
        { "EffectObj", createActorFunction<al::EffectObj> },
        { "EffectObjFollowCamera", nullptr },
        { "EffectObjLiftLiner", nullptr },
        { "EffectObjReEmit", nullptr },
        { "EffectObjSensor", nullptr },
        { "EffectObjStream", nullptr },
        { "EffectObjSwitch", nullptr },
        { "WaterFlowCube", nullptr },
        { "StreetPassObj", nullptr },
        { "DemoStarter", nullptr } };

} // namespace al

#include <Util/alStringUtil.h>

extern const char dat_003BA0F8[];
extern const char dat_003BA104[];

namespace al
{

        inline const char* ActorFactory::findConvertedNameForCreator( const char* objectName ) const
        {
                const int conversionCount = mConvertNameData->getSize();
                for ( int index = 0; index < conversionCount; ++index )
                {
                        ByamlIter entry;
                        mConvertNameData->tryGetIterByIndex( &entry, index );
                        const char* entryObjectName = nullptr;
                        if ( entry.tryGetStringByKey( &entryObjectName, dat_003BA0F8 ) &&
                        isEqualString( entryObjectName, objectName ) )
                        {
                                const char* className = nullptr;
                                if ( entry.tryGetStringByKey( &className, dat_003BA104 ) )
                                return className;
                        }
                }
                return nullptr;
        }

        CreateActorFuncPtr ActorFactory::getCreator( const char* objectName ) const
        {
                const char* convertedName = findConvertedNameForCreator( objectName );
                if ( convertedName )
                {
                        const int creatorCount =
                        sizeof( sActorFactoryEntries ) / sizeof( sActorFactoryEntries[ 0 ] );
                        for ( int creatorIndex = 0; creatorIndex < creatorCount; ++creatorIndex )
                        {
                                const NameToActorCreator& entry = sActorFactoryEntries[ creatorIndex ];
                                if ( isEqualString( convertedName, entry.name ) )
                                return entry.creator;
                        }
                }
                return nullptr;
        }

} // namespace al
