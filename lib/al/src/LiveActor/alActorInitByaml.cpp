#include <LiveActor/alActorInitByaml.h>
#include <LiveActor/alActorInitInfo.h>
#include <LiveActor/alLiveActor.h>
#include <LiveActor/alLiveActorFunction.h>
#include <LiveActor/alSensorFunction.h>
#include <Placement/alPlacementFunction.h>
#include <Resource/alResource.h>
#include <Util/alStringUtil.h>
#include <Yaml/alByamlIter.h>

#ifdef NON_MATCHING

// Observed string ABI views. These hold ordinary data, not code. The virtual
// termination call is retained because callees can observe its buffer writes.
// Use these local views until the shared SafeString class-layer repair lands.
namespace ActorInitByaml {
struct String {
        void* const* table;
        const char* text;
        const char* cstr() const {
                typedef void (*Terminate)( const String* );
                reinterpret_cast<Terminate>( table[2] )( this );
                return text;
        }
        const sead::SafeString& safe() const {
                return *reinterpret_cast<const sead::SafeString*>( this );
        }
};
template <int N> struct Buffer : String {
        int capacity;
        char storage[N];
        void init( unsigned tableAddress ) {
                text = storage;
                capacity = N;
                storage[N - 1] = 0;
                storage[0] = 0;
                table = reinterpret_cast<void* const*>( tableAddress );
        }
};
struct PoseInitializer {
        const char* name;
        void (*initialize)( al::LiveActor* );
};
}

// Each neutral import below is a directly observed original BL destination.
// Signatures express this call site's ABI; names do not assert original symbols.
extern "C" {
void fn_00250FAC( void*, const char*, const char* );
void fn_0028E1E4( void*, const char*, ... );
ActorInitByaml::String* fn_0028E350( void*, const char*, ... );
ActorInitByaml::String* fn_0028CB38( void*, const char*, ... );
void fn_00270AB0( al::LiveActor*, const al::ActorInitInfo& );
void fn_001E7F7C( al::LiveActor*, const al::ActorInitInfo&, const al::ByamlIter&,
                  const char*, int, const char* );
bool fn_0024C7A4( const al::LiveActor* );
bool fn_00243A54( const al::LiveActor*, const char*, const char* );
const u8* fn_00260500( const al::LiveActor*, const char*, const char* );
void fn_001CA5E8( al::LiveActor*, const al::ByamlIter& );
void fn_001EBE94( al::LiveActor*, float, float );
void fn_001E94E8( al::LiveActor*, float, float );
void fn_002627A4( al::LiveActor*, const al::ActorInitInfo&, const char* );
void fn_001DCF34( al::LiveActor*, const al::ActorInitInfo& );
bool fn_002690EC( const al::Resource*, const sead::SafeString& );
void fn_0026E64C( al::LiveActor*, int );
bool fn_002253C8( sead::Vector3f*, const al::ByamlIter& );
al::HitSensor* fn_001C2CC8( al::LiveActor*, const char*, al::SensorType,
                           unsigned short, const sead::Vector3f&, float );
void fn_001DEA48( al::LiveActor*, const char*, const char* );
const char* fn_00272610( const al::Resource* );
const sead::Matrix34f* fn_002519A8( const al::LiveActor*, const char* );
void fn_0024F8CC( al::LiveActor*, al::Resource*, const sead::SafeString&,
                  al::HitSensor*, const sead::Matrix34f*, const char* );
void fn_001EBC64( al::LiveActor*, unsigned, float, float );
void fn_001EBDDC( al::LiveActor*, const al::ActorInitInfo&, const char* );
bool fn_001E7964( const al::ByamlIter&, const char**, int* );
void fn_0026D9EC( al::LiveActor*, const sead::SafeString&, int,
                  const sead::Vector3f*, const sead::Matrix34f* );
void fn_00280538( al::IUseStageSwitch*, const al::ActorInitInfo& );
void fn_00270724( al::IUseStageSwitch*, const al::ActorInitInfo& );
void fn_0026FC64( al::IUseStageSwitch*, const al::ActorInitInfo& );
void fn_0027FCBC( al::IUseStageSwitch*, const al::ActorInitInfo& );
void fn_002640FC( al::IUseStageSwitch*, const al::ActorInitInfo& );
void fn_0026EDDC( al::IUseStageSwitch*, const al::ActorInitInfo& );
void fn_002640A8( al::IUseStageSwitch*, const al::ActorInitInfo& );
void fn_0026F5A8( al::LiveActor*, const al::ActorInitInfo& );
void fn_0027FD10( al::LiveActor*, const sead::Vector3f*, float );
float fn_001678B0( const al::LiveActor* );
void fn_001DBFA0( al::LiveActor*, float );
void fn_0026F56C( al::LiveActor*, const al::ActorInitInfo&, int );
void fn_0026DA54( al::LiveActor*, const al::ActorInitInfo&, const char* );
void fn_001DEC68( al::LiveActor* );
void fn_0026E4A4( al::LiveActor*, const char*, const char*, const char* );
bool fn_0027063C( al::LiveActor*, const char* );
void fn_0024FDB8( al::ActorActionKeeper*, const char* );
void fn_001E33F8( al::LiveActor* );
void fn_002742E0( al::LiveActor*, const al::ActorInitInfo&, const char*, const char* );
}
namespace al { const sead::Vector3f& getScale( const LiveActor* ); }

extern "C" void fn_002417E8( al::LiveActor* actor, const al::ActorInitInfo& info,
        const sead::SafeString& objectName, const sead::SafeString& archivePath,
        const char* suffix )
{
        using namespace al;
        using namespace ActorInitByaml;
        Resource* resource = findOrCreateResource( archivePath );
        Buffer<128> initName;
        initName.init( 0x003D7ABC );
        fn_00250FAC( &initName, "InitActor", suffix );
        ByamlIter init( resource->getByml( initName.safe() ) );
        const char* pose = 0;
        if ( init.tryGetStringByKey( &pose, "Pose" ) && pose ) {
                const PoseInitializer* entries = reinterpret_cast<const PoseInitializer*>( 0x003EF600 );
                for ( int i = 0; i < 4; ++i ) {
                        if ( isEqualString( entries[i].name, pose ) ) {
                                if ( entries[i].initialize ) entries[i].initialize( actor );
                                break;
                        }
                }
        }
        fn_00270AB0( actor, info );
        {
                ByamlIter model;
                if ( init.tryGetIterByKey( &model, "Model" ) ) {
                        const char* animArchive = 0;
                        model.tryGetStringByKey( &animArchive, "AnimArc" );
                        int blendCount = 1;
                        model.tryGetIntByKey( &blendCount, "BlendAnimMax" );
                        const char* animPath = 0;
                        Buffer<256> path;
                        if ( animArchive ) {
                                fn_0028E350( &path, "ObjectData/%s", animArchive );
                                animPath = path.cstr();
                        }
                        fn_001E7F7C( actor, info, init, archivePath.cstr(), blendCount, animPath );
                }
        }
        if ( actor->getModelKeeper() && fn_0024C7A4( actor ) ) {
                Buffer<64> lightName;
                lightName.init( 0x003D7AF8 );
                const char* name = "InitLight";
                if ( suffix ) {
                        fn_0028E1E4( &lightName, "InitLight_%s", suffix );
                        if ( fn_00243A54( actor, lightName.cstr(), 0 ) ) name = lightName.cstr();
                }
                if ( fn_00243A54( actor, name, 0 ) ) {
                        ByamlIter light( fn_00260500( actor, name, 0 ) );
                        fn_001CA5E8( actor, light );
                }
        }
        {
                ByamlIter material;
                if ( init.tryGetIterByKey( &material, "MaterialController" ) ) {
                        ByamlIter projection;
                        if ( material.tryGetIterByKey( &projection, "ProjTex1dRelTransY" ) ) {
                                float offset = 0.0f, heightScale = 300.0f;
                                projection.tryGetFloatByKey( &offset, "Offset" );
                                projection.tryGetFloatByKey( &heightScale, "HeightScale" );
                                fn_001EBE94( actor, offset, heightScale );
                        } else if ( material.tryGetIterByKey( &projection, "ProjTex1dLocalTransY" ) ) {
                                float offset = 0.0f, heightScale = 300.0f;
                                projection.tryGetFloatByKey( &offset, "Offset" );
                                projection.tryGetFloatByKey( &heightScale, "HeightScale" );
                                fn_001E94E8( actor, offset, heightScale );
                        }
                }
        }
        {
                ByamlIter executor;
                if ( init.tryGetIterByKey( &executor, "Executor" ) ) {
                        const char* category = 0;
                        if ( executor.tryGetStringByKey( &category, "CategoryName" ) )
                                fn_002627A4( actor, info, category );
                        else fn_001DCF34( actor, info );
                }
        }
        {
                Buffer<128> sensorName, fallbackName;
                sensorName.init( 0x003D7ABC );
                fn_00250FAC( &sensorName, "InitSensor", suffix );
                fallbackName.init( 0x003D7ABC );
                fn_00250FAC( &fallbackName, "init", suffix );
                ByamlIter sensors;
                Buffer<128> bymlName;
                if ( fn_002690EC( resource, fn_0028CB38( &bymlName, "%s.byml", sensorName.cstr() )->safe() ) ) {
                        sensors = ByamlIter( resource->getByml( sensorName.safe() ) );
                } else if ( fn_002690EC( resource, fn_0028CB38( &bymlName, "%s.byml", fallbackName.cstr() )->safe() ) ) {
                        ByamlIter fallback( resource->getByml( fallbackName.safe() ) );
                        fallback.tryGetIterByKey( &sensors, "Sensor" );
                        if ( sensors.getSize() <= 0 ) goto sensorsDone;
                } else goto sensorsDone;
                {
                        int count = sensors.getSize();
                        if ( count > 0 ) {
                                fn_0026E64C( actor, count );
                                for ( int i = 0; i < count; ++i ) {
                                        ByamlIter entry;
                                        if ( !sensors.tryGetIterByIndex( &entry, i ) ) continue;
                                        const char* name = 0;
                                        if ( !entry.tryGetStringByKey( &name, "Name" ) ) continue;
                                        const char* typeName = 0;
                                        if ( !entry.tryGetStringByKey( &typeName, "Type" ) ) continue;
                                        float radius = 0.0f;
                                        entry.tryGetFloatByKey( &radius, "Radius" );
                                        int maxCount = 8;
                                        entry.tryGetIntByKey( &maxCount, "MaxCount" );
                                        sead::Vector3f offset = sead::Vector3f::zero;
                                        fn_002253C8( &offset, entry );
                                        SensorType type = alSensorFunction::findSensorTypeByName( typeName );
                                        if ( type == 13 ) maxCount = 0;
                                        fn_001C2CC8( actor, name, type, static_cast<unsigned short>( maxCount ), offset, radius );
                                        const char* joint = 0;
                                        entry.tryGetStringByKey( &joint, "Joint" );
                                        if ( joint ) fn_001DEA48( actor, name, joint );
                                }
                        }
                }
        }
sensorsDone:
        {
                ByamlIter collision;
                if ( init.tryGetIterByKey( &collision, "Collision" ) ) {
                        const char* name = 0;
                        collision.tryGetStringByKey( &name, "Name" );
                        // Retail initializes this ordinary scratch string even
                        // though this caller never subsequently reads it.
                        Buffer<256> unused;
                        unused.init( 0x003D7AD0 );
                        if ( !name ) name = getBaseName( fn_00272610( resource ) );
                        HitSensor* sensor = 0;
                        const char* sensorName = 0;
                        if ( collision.tryGetStringByKey( &sensorName, "Sensor" ) )
                                sensor = getHitSensor( actor, sensorName );
                        const sead::Matrix34f* jointMatrix = 0;
                        const char* jointName = 0;
                        collision.tryGetStringByKey( &jointName, "Joint" );
                        if ( jointName ) jointMatrix = fn_002519A8( actor, jointName );
                        String collisionName;
                        collisionName.table = reinterpret_cast<void* const*>( 0x003D9C3C );
                        collisionName.text = name;
                        fn_0024F8CC( actor, resource, collisionName.safe(), sensor, jointMatrix, suffix );
                }
        }
        {
                ByamlIter collider;
                if ( init.tryGetIterByKey( &collider, "Collider" ) ) {
                        float radius = 0.0f;
                        collider.tryGetFloatByKey( &radius, "Radius" );
                        sead::Vector3f offset = sead::Vector3f::zero;
                        fn_002253C8( &offset, collider );
                        fn_001EBC64( actor, 0, radius, offset.y );
                }
        }
        {
                ByamlIter effect;
                if ( init.tryGetIterByKey( &effect, "Effect" ) ) {
                        const char* name = 0;
                        if ( effect.tryGetStringByKey( &name, "Name" ) ) fn_001EBDDC( actor, info, name );
                }
        }
        {
                const char* name = 0;
                int maxCount = 0;
                if ( fn_001E7964( init, &name, &maxCount ) ) {
                        String soundName;
                        soundName.table = reinterpret_cast<void* const*>( 0x003D9C3C );
                        soundName.text = name;
                        fn_0026D9EC( actor, soundName.safe(), maxCount, 0, 0 );
                }
        }
        {
                ByamlIter rail;
                if ( init.tryGetIterByKey( &rail, "Rail" ) && isExistRail( info ) ) actor->initRailKeeper( info );
        }
        {
                ByamlIter switches;
                if ( init.tryGetIterByKey( &switches, "Switch" ) ) {
                        if ( switches.isExistKey( "UseAppear" ) ) fn_00280538( actor, info );
                        if ( switches.isExistKey( "UseKill" ) ) fn_00270724( actor, info );
                        if ( switches.isExistKey( "UseDeadOn" ) ) fn_0026FC64( actor, info );
                        if ( switches.isExistKey( "UseReadA" ) ) fn_0027FCBC( actor, info );
                        if ( switches.isExistKey( "UseWriteA" ) ) fn_002640FC( actor, info );
                        if ( switches.isExistKey( "UseReadB" ) ) fn_0026EDDC( actor, info );
                        if ( switches.isExistKey( "UseWriteB" ) ) fn_002640A8( actor, info );
                }
        }
        fn_0026F5A8( actor, info );
        {
                Buffer<128> clippingName;
                clippingName.init( 0x003D7ABC );
                fn_00250FAC( &clippingName, "InitClipping", suffix );
                ByamlIter clipping;
                Buffer<128> fileName;
                if ( fn_002690EC( resource, fn_0028CB38( &fileName, "%s.byml", clippingName.cstr() )->safe() ) ) {
                        clipping = ByamlIter( resource->getByml( clippingName.safe() ) );
                } else {
                        Buffer<128> fallbackName;
                        fallbackName.init( 0x003D7ABC );
                        fn_00250FAC( &fallbackName, "InitActor", suffix );
                        if ( fn_002690EC( resource, fn_0028CB38( &fileName, "%s.byml", fallbackName.cstr() )->safe() ) ) {
                                ByamlIter fallback( resource->getByml( fallbackName.safe() ) );
                                fallback.tryGetIterByKey( &clipping, "Clipping" );
                        }
                }
                bool invalid = false;
                clipping.tryGetBoolByKey( &invalid, "Invalidate" );
                if ( invalid ) invalidateClipping( actor );
                float radius = 0.0f;
                if ( clipping.tryGetFloatByKey( &radius, "Radius" ) ) {
                        fn_0027FD10( actor, 0, radius );
                } else if ( actor->getModelKeeper() ) {
                        const sead::Vector3f& scale = getScale( actor );
                        float x = scale.x > 0.0f ? scale.x : -scale.x;
                        float y = scale.y > 0.0f ? scale.y : -scale.y;
                        float z = scale.z > 0.0f ? scale.z : -scale.z;
                        float xy = x > y ? x : y;
                        float maximum = xy > z ? xy : z;
                        fn_0027FD10( actor, 0, fn_001678B0( actor ) * maximum );
                }
                float nearDistance = 0.0f;
                if ( clipping.tryGetFloatByKey( &nearDistance, "NearClipDistance" ) ) fn_001DBFA0( actor, nearDistance );
        }
        {
                ByamlIter group;
                if ( init.tryGetIterByKey( &group, "GroupClipping" ) ) {
                        int maxCount = 16;
                        group.tryGetIntByKey( &maxCount, "MaxCount" );
                        fn_0026F56C( actor, info, maxCount );
                }
        }
        if ( actor->getModelKeeper() && fn_0024C7A4( actor ) ) {
                Buffer<64> shadowName;
                shadowName.init( 0x003D7AF8 );
                const char* name = "InitShadow";
                if ( suffix ) {
                        fn_0028E1E4( &shadowName, "InitShadow_%s", suffix );
                        if ( fn_00243A54( actor, shadowName.cstr(), 0 ) ) name = shadowName.cstr();
                }
                if ( fn_00243A54( actor, name, 0 ) ) fn_0026DA54( actor, info, name );
        }
        {
                ByamlIter flags;
                if ( init.tryGetIterByKey( &flags, "Flag" ) ) {
                        ByamlIter materialCode;
                        if ( flags.tryGetIterByKey( &materialCode, "MaterialCode" ) ) fn_001DEC68( actor );
                }
        }
        const char* baseName = getBaseName( objectName.cstr() );
        fn_0026E4A4( actor, baseName, suffix, 0 );
        if ( actor->getModelKeeper() && !fn_0027063C( actor, baseName ) ) {
                if ( actor->getActorActionKeeper() ) fn_0024FDB8( actor->getActorActionKeeper(), baseName );
        }
        if ( actor->getNerveKeeper() && actor->getNerveKeeper()->getActionCtrl() ) fn_001E33F8( actor );
        if ( !*reinterpret_cast<void**>( reinterpret_cast<unsigned char*>( actor ) + 0x50 ) )
                fn_002742E0( actor, info, suffix, 0 );
}
#endif
